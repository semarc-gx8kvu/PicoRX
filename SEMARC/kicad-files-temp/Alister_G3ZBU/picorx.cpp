#include "pico/stdlib.h"
#include <stdio.h>

#include "pico/multicore.h"
#include "pico/time.h"
#include "hardware/watchdog.h"

#include "rx.h"
#include "ui.h"
#include "waterfall.h"
#include "cat.h"
#include "hardware/uart.h"

#define UI_REFRESH_HZ (10UL)
#define UI_REFRESH_US (1000000UL / UI_REFRESH_HZ)
#define CAT_REFRESH_US (1000UL) //10ms
#define SERCAT_REFRESH_US (10000UL) //100ms
#define BUTTONS_REFRESH_US (50000UL) // 50ms <=> 20Hz
#define WATERFALL_REFRESH_US (50000UL) // 50ms <=> 20Hz

// ZBU
#define UART_ID1 uart1
#define BAUD_RATE1 19200
#define UART_TX_PIN 8
#define UART_RX_PIN 9
#define TESTPIN 7


uint8_t spectrum[256];
uint8_t audio[128];
uint8_t dB10=10;
uint8_t zoom=1;
static rx_settings settings_to_apply;
static rx_status status;
static rx receiver(settings_to_apply, status);
waterfall waterfall_inst;
static ui user_interface(settings_to_apply, status, receiver, spectrum, audio, dB10, zoom, waterfall_inst);

void core1_main()
{
    multicore_lockout_victim_init();
    receiver.run();
}

void process_sercat_control(rx_settings & settings_to_apply, rx_status & status, rx &receiver, s_settings &settings)
{
  static char cmd_buffer[64];
  static uint8_t cmd_idx = 0;

  // ZBU - this is a placeholder for processing serial CAT control commands received over UART
  // In a real implementation, this would read from the UART buffer, parse commands, and update settings accordingly
  while (uart_is_readable(UART_ID1)) {
    char c = uart_getc(UART_ID1);
    cmd_buffer[cmd_idx++] = c;
    if (c==';' ) // assuming commands are terminated with a semicolon
    {
      cmd_buffer[cmd_idx] = '\0'; // null-terminate the command string
      cmd_idx = 0; // reset index for next command
      // printf("Received CAT command: %s\n", cmd_buffer); // For testing, we can just print the received command
      
      // Here you would add code to parse the command and update settings_to_apply as needed
      if (strstr(cmd_buffer, "MD;") == cmd_buffer) // example: Poll cmd MD;;
      {
        // This is just an example of how you might parse a command. 
        uart_putc_raw(UART_ID1, 'M'); // Respond with 'M' for MD command
        uart_putc_raw(UART_ID1, 'D');
        c='0'+settings_to_apply.mode; // Convert mode to character
        uart_putc_raw(UART_ID1, c); // example mode value
        uart_putc_raw(UART_ID1, ';'); // End the response with a semicolon
      }
      else if (strstr(cmd_buffer, "MD") == cmd_buffer) // example: Set cmd MD1; (set mode to 1)
      {
        // This is just an example of how you might parse a command. 
        settings.channel.mode = cmd_buffer[2] - '0'; // Set mode to 1 based on command
      //  printf("Mode=%x\n", settings_to_apply.mode); // For testing, print the current mode
        apply_settings_to_rx(receiver, settings_to_apply, settings, false, true); // Indicate that settings have changed and need to be applied
      } 
      // clear the string buffer after processing the command
      memset(cmd_buffer, 0, sizeof(cmd_buffer));
    }
    // For testing, we can just print the received character
   
    // Here you would add code to parse the command and update settings_to_apply as needed
  }
}

void test_uart_cat()
{
  static uint32_t i;

  if (i++>1000000)
  { 
       gpio_put(TESTPIN,1);  // ZBU just to time this.
          i=0;  // this is best down when CPU clock is changed.
    uart_init(UART_ID1, BAUD_RATE1); // ZBU mods for serial CAT control
        gpio_put(TESTPIN,0);
  }
}

int main() 
{
  gpio_set_function(LED, GPIO_FUNC_SIO);
  gpio_set_dir(LED, GPIO_OUT);
  gpio_put(LED, 1);

  uart_set_format(UART_ID1, 8, 1, UART_PARITY_NONE); 
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); 
  gpio_set_function(TESTPIN, GPIO_FUNC_SIO);
  gpio_set_dir(TESTPIN, GPIO_OUT);
  gpio_put(TESTPIN, 0);
  uart_set_fifo_enabled(UART_ID1, true); // Enable FIFO for less immediate response
  uart_init(UART_ID1, BAUD_RATE1); // ZBU mods for serial CAT control

  stdio_init_all();
  watchdog_enable(2000, true);
  multicore_launch_core1(core1_main);

  // create an alarm pool for USB streaming with highest priority (0), so
  // that it can pre-empt the default pool
  receiver.set_alarm_pool(alarm_pool_create(0, 16));
  user_interface.autorestore();


  uint32_t last_ui_update = 0;
  uint32_t last_cat_update = 0;
  uint32_t last_sercat_update = 0;
  uint32_t last_buttons_update = 0;
  uint32_t last_waterfall_update = 0;

  while(1)
  {

    watchdog_update();

    //schedule tasks
    if (time_us_32() - last_buttons_update > BUTTONS_REFRESH_US) 
    {
      last_buttons_update = time_us_32();
      user_interface.update_buttons();
    }
    receiver.tune();

    if(time_us_32() - last_ui_update > UI_REFRESH_US)
    {
      last_ui_update = time_us_32();
      user_interface.do_ui();
      receiver.get_spectrum(spectrum, dB10, zoom);
      receiver.get_audio(audio);
    }

    if(time_us_32() - last_cat_update > CAT_REFRESH_US)
    {
      last_cat_update = time_us_32();
      process_cat_control(settings_to_apply, status, receiver, user_interface.get_settings());
      
    }

    if(time_us_32() - last_waterfall_update > WATERFALL_REFRESH_US)
    {
      last_waterfall_update = time_us_32();
      waterfall_inst.update_spectrum(receiver, user_interface.get_settings(), settings_to_apply, status, spectrum, dB10, zoom);
    }
    
    if(time_us_32() - last_sercat_update > SERCAT_REFRESH_US)
    {
      last_sercat_update = time_us_32();
      process_sercat_control(settings_to_apply, status, receiver, user_interface.get_settings());
      
    }
    test_uart_cat(); // ZBU test code for timing serial CAT control

  }
}
