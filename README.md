# nosdk8266

There is an awesome little $2 processor called an "ESP8266."  It's the definitive chip that is bringing the internet of things to life. Ever wonder what the ESP8266 can do *without* wifi?  Well, this project is it!  No longer shackled by an SDK that takes up 200 kB of flash, and tons of RAM, you're free to experiment and do so quickly.  Little did we realize how limited the clocks were and just how fast this chip can be whe unleashed.

This is a working ESP8266/ESP8285 minimial, non-SDK application.  It can optionally use the ROM functions to deal with I/O, interrupts, printf'ing, etc.

Alternatively, a "PICO66" build can be made, which is intended to be compliant to the HackADay [1kB challenge](https://hackaday.io/contest/18215-the-1kb-challenge). In PICO66 mode, printf'ing is disabled (Though it can be overridden for debugging)  In order to be compliant, all ROM functions will count against you.  Basically if you want to participate you'll have to make sure USE_PRINT is turned off in your final submission.

PICO66 Code-size-table (with USE_PRINT disabled) as of 755c743de4d430a6a1b3ee2c11e583838d50f422

| Desired Frequency | I2S Size | Size, without I2S bus | Remarks | Peripheral Bus Clock | Voids Warranty |
| ----------------- | ----------------------- | --------------------- | ------- | ----- | -------------- |
| 52 MHz | Unavailable | 135+8 | No PLL, No overclocking, Default behavior | 52 MHz | N |
| 104 MHz | Unavailable | 151+8 | No PLL, Overclocking bit set | 52 MHz | ? |
| 80 MHz | 448+32 | 228+8 | PLL, Normal speed. | 80 MHz | N |
| 160 MHz | 464+32 | 244+8 | PLL, Normal "overclock" mode | 80 MHz | ? |
| 115 MHz | 448+32 | 228+8 | Overclock peripheral bus. (Voids warranty, may not work) | 115.5 MHz | Y |
| 231 MHz | 464+32 | 244+8 | Overclock peripheral bus. (Voids warranty, may not work) | 115.5 MHz | Y |
| 173 MHz | 448+32 | 228+8 | Needs >.2s reset to boot. | 173 MHz | Y |
| 346 MHz | 464+32 | 244+8 | Needs >.2s reset to boot. | 173 MHz | Y |
| 189 MHz | 448+32 | 228+8 | Frequently will not boot. | ~189 MHz | **YES** |
| 378 MHz | 464+32 | 244+8 | Runs slower on ESP8285. | ~189 MHz | **YES** |

Interestingly, you might notice that the way this works is with a 1040 MHz high speed PLL clock and divides from that.  When the clock rate is very high, i.e. 189/378 MHz, the PLL may or may not lock if the processor boots at all.  I found that my clock was wandering around when operating up there.

# Prerequisites and Building

You'll need the xtensa compiler and a copy of esptool.py.  Both of whih will come with the [ESP Open SDK](https://github.com/pfalcon/esp-open-sdk).  Pick a folder and install the esp open sdk.  NOTE: If you will want to use the combined toolchain, select ```STANDALONE=y```.  For this system, we will not need that, though.

I strongly recommend using a Linux system.  Follow the information on esp-open-sdk's site for more details and lists of Linux package prerequisites.

```
$ git clone --recursive https://github.com/pfalcon/esp-open-sdk.git
$ cd esp-open-sdk
$ make STANDALONE=n
```

## Altering the Makefile

The makefile has some defaults, however, you will likely need to modify the ESP_OPEN_SDK parameter to compile on your system, so they point to your esp open sdk.

```
ESP_OPEN_SDK:=~/esp8266/esp-open-sdk
```

Once done, save your Makefile and type:

```
make clean all burn
```

Cleaning, building and burning should only take about two seconds.  Yes, that means you can comfortably run through about 10 development cycles per minute.  Let's call this your DCPM rate. Remember kids, always find ways of maximizing your DCPM rate.


# Additional Remarks

All PLL settings are hard-coded for a 26 MHz external crystal.  This is probaly okay because almost everyone uses this.  However, it does mean you cannot use this project with any other crystals that are compatible with the ESP (i.e. 40 MHz crystals).

## Credits

A large portion of figuring out what's what was done by @pvvx, over at  his [esp8266web](https://github.com/pvvx/esp8266web/) repository.  Additionally, some of the header files are still licensed Espressif.  So, don't let the MIT license on the overarching project confuse you.

# Todo

Find out why -fwhole-program dosen't work.  It could reduce overhead of function calls.



