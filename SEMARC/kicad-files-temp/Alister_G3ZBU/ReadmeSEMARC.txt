Compile Notes.

I cloned the github rep into a directory PicoRX

Note that the Pico-SDK must be version 2.1.0 as 2.1.1 and 2.1.2 don't work according allegedly.

Import the code by letting VS Code know the location and it will do the rest automatically, almost!

The VS Code Compile button gives this error:

ninja: Entering directory `/home/al/PicoRX/build'
ninja: error: loading 'build.ninja': No such file or directory

AI said this:

The build system hasn't been configured yet. 
You need to generate the build files first using CMake.

Run this in your terminal:

cd PicoRX
cmake -B build

Then you can build with VS Code 'compile' button.

The LPF or BPF limits can be changed in a menu with a proviso that limits need to be monotonic, 
therefore it should be easy to change them to suit Amateur Bands band-pass filters. 
Conversely it should be fairly simple to hard-code the 3 GPIO pins to match each band's lower and 
upper limits and select position 8 (or 1) for general coverage as a straight through unfiltered link.

Operating Notes.

If the encoder push button is pressed and
the menu button is held then rotating the encoder will change the channel.mode, or
if the back button is held then rotating the encoder will change the squelch level, or
otherwise the volume will change.

Very fast tuning is achieved by holding both menu and back buttons.
Fast tuning is achieved by holding the menu button.
Slow tuning is achieved by holding the back button.


Back button Views:
Original, BigSpectrum, CombinedSpectrum, Waterfall, Oscilloscope, Status, S-meter, Fun.


Top Level Menus:
Frequency, Recall, Store, Volume, Mode, AGC, AGC Gain, Bandwidth, Squelch, Squelch Timeout, Noise Reduction, Impulse Blanker, Auto Notch
De-Emphasis, Bass, Treble, IQ Correction, Spectrum, Aux Display, Band Start, Band Stop, Frequency Step, CW Tone Frequency
USB Stream, HW Config

Volume Menu:
0..9

Mode Menu:
AM, AM-Sync, LSB, USB, FM, CW

AGC Menu:
Fast, Normal, Slow, Very slow, Manual

AGC Gain Menu:
0dB, 6dB, 12dB, 18dB, 24dB, 30dB, 36dB, 42dB, 48dB, 54dB, 60dB

Bandwidth Menu:
V Narrow, Narrow, Normal, Wide, Very Wide, 

Squelch Menu:
S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, S9+10dB, S9+20dB, S9+30dB

Squelch Timeout Menu:
50ms, 100ms, 200ms, 500ms, 1s, 2s, 3s, 5s

Noise Menu:
Enable, Noise Estimation, Noise Threshold.

Impulse Threshold Menu:
Off, 3.0, 2.8, 2.6, 2.4, 2.2, 2.0

Auto Notch Menu:
Off, On

De-emphasis Menu:
Off, 50us, 75us

Bass Menu:
Off, +5dB, +10dB, +15dB, +20dB

Treble Menu:
Off, +5dB, +10dB, +15dB, +20dB

IQ Correction Menu:
Off, On

Spectrum Menu:
Spectrum, Zoom Spectrum, Smoothing.

Aux Display Menu:
Waterfall, SSTV



Bands Menu:
Band 1, Band 2, Band 3, Band 4, Band 5, Band 6, Band 7.
Band Stop: ?
Band Start: ?

Frequency Step Menu:
10Hz, 50Hz, 100Hz, 500Hz, 1kHz, 5kHz, 6.25kHz, 9kHz, 10kHz, 12.5kHz, 25kHz, 50kHz, 100kHz

CW Tone Frequency:
Steps of 100Hz

USB Stream Menu:
Audio, Raw IQ

HW Config Menu:
Tuning Options, Display Timeout, Regulator Mode, Reverse Encoder, Encoder Resolution, Swap IQ, Gain Cal, 
Freq Cal, Flip OLED, OLED Type, Display Contrast, TFT Settings, TFT Colour, TFT Invert, TFT Driver, 
Bands, IF Mode, IF Frequency, External NCO, USB Upload, Watchdog Test.

















