# nosdk8266

Ever wonder what the ESP8266 can do /without/ wifi?

This is a working ESP8266/ESP8285 minimial, non-SDK application.  It can optionally use the ROM functions to deal with I/O, interrupts, printf'ing, etc.

Alternatively, a "PICO" build can be made, which is intended to be compliant to the HackADay [1kB challenge](https://hackaday.io/contest/18215-the-1kb-challenge). In PICO mode, printf'ing is disabled (Though it can be overridden for debugging)  In order to be compliant, all ROM functions will count against you.  Basically if you want to participate you'll have to make sure USE_PRINT is turned off in your final submission.

PICO Code-size-table (with USE_PRINT disabled) as of 755c743de4d430a6a1b3ee2c11e583838d50f422

| Desired Frequency | I2S Size | Size, without I2S bus | Remarks | Peripheral Bus Clock | Voids Warranty |
| ----------------- | ----------------------- | --------------------- | ------- | ----- | -------------- |
| 52 MHz | Unavailable | 135+8 | No PLL, No overclocking, Default behavior | 52 MHz | N |
| 104 MHz | Unavailable | 151+8 | No PLL, Overclocking bit set | 52 MHz | ? |
| 80 MHz | 464+32 | 248+8 | PLL, Normal speed. | 80 MHz | N |
| 160 MHz | 464+32 | 244+8 | PLL, Normal "overclock" mode | 80 MHz | ? |
| 115 MHz |  |  | Overclock peripheral bus. (Voids warranty, may not work) | 115.5 MHz | Y |
| 231 MHz |  |  | Overclock peripheral bus. (Voids warranty, may not work) | 115.5 MHz | Y |
| 173 MHz |  |  | Needs >.2s reset to boot. | 173 MHz | Y |
| 346 MHz |  |  | Needs >.2s reset to boot. | 173 MHz | Y |
| 189 MHz |  |  | Frequently will not boot. | ~189 MHz | SUPER YES |
| 378 MHz |  |  | Frequently will not boot. Will sometimes boot on ESP8285, but operates a little slower. | ~189 MHz | SUPER YES|


Interestingly, you might notice that the way this works is with a 1040 MHz.

## Additional Remarks

All PLL settings are hard-coded for a 26 MHz external crystal.  This is probaly okay because almost everyone uses this.  However, it does mean you cannot use this project with any other crystals that are compatible with the ESP (i.e. 40 MHz crystals).

# Todo

Find out why -fwhole-program dosen't work.  It could reduce overhead of function calls.



