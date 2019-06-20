# DC-China-VU-Meter
Quick and dirty badge hack to turn the DEF CON China 1.0 badge into a VU meter

Badge looks for audio input on pin A5 (pin 7 on the FPC port). For this project, I used my cigar mic from my [DC24 Bender Badge Hack](https://github.com/ideo/Cigar_Mic_DC24). The audio signal is read with analogread and the amplitude change of the signal (from the DC offset) is recorded to calculate volume.

This project was developed in Arduino IDE 1.8.3, and it requires no external libraries. The badge can be programmed as an "Arduino Pro or Pro Mini" board and "Atmega328 (3.3V 8MHz)" processor. For more details on the badge and programming, see [Joe Grand's notes](http://www.grandideastudio.com/defcon-china-2019-badge/).

This code was scrappily thrown together, mostly during the con. Do not rely heavily on it.
