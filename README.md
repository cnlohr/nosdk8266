# nosdk8266

If you don't need wifi on your ESP8266 and want to use it as a microcontroller to use its unbridled power, and spend only a second in your build cycles, this might be the project for you.

There is an awesome little $2 processor called an "ESP8266."  It's the definitive chip that is bringing the internet of things to life. Ever wonder what the ESP8266 can do *without* wifi?  Well, this project is it!  No longer shackled by an SDK that takes up 200 kB of flash, and tons of RAM, you're free to experiment and do so quickly.  Little did we realize how limited the clocks were and just how fast this chip can be once unleashed.

This is a working ESP8266/ESP8285 minimial, non-SDK application.  It can optionally use the ROM functions to deal with I/O, interrupts, printf'ing, etc.

If you don't need to access FLASH at all after booting, that frees up some of those GPIOs attached to the flash chip, provided you disable its CS line.

| Desired Frequency | I2S Size (in bytes) | Size, without I2S bus | Remarks | Peripheral Bus Clock | Voids Warranty |
| ----------------- | ----------------------- | --------------------- | ------- | ----- | -------------- |
| 52 MHz | Unavailable | 131+0 | No PLL, No overclocking, Default behavior | 52 MHz | N |
| 80 MHz | 356+24 | 188+0 | PLL, Normal speed. | 80 MHz | N |
| 115 MHz | 356+24 | 188+0 | Overclock peripheral bus. (Voids warranty, may not work) | 115.5 MHz | Y |
| 160 MHz | 372+24 | 204+0 | PLL, Normal "overclock" mode | 80 MHz | ? |
| 173 MHz | 356+24 | 188+0 | Needs >.2s reset to boot. | 173 MHz | Y |
| 189 MHz | 356+24 | 188+0 | Frequently will not boot. | ~189 MHz | **YES** |
| 231 MHz | 372+24 | 204+0 | Overclock peripheral bus. (Voids warranty, may not work) | 115.5 MHz | Y |
| 320 MHz | 372+24 | 204+0 | Needs >.2s reset to boot. | 80 MHz | Y |
| 346 MHz | 372+24 | 204+0 | Needs >.2s reset to boot. | 173 MHz | Y |

Interestingly, you might notice that the way this works is with a 1040 MHz high speed PLL clock and divides from that.  When the clock rate is very high, i.e. 189/378 MHz, the PLL may or may not lock if the processor boots at all.  I found that my clock was wandering around when operating up there.

# Example project

You can see some examples here: https://github.com/cnlohr/nosdk8266_example

# Prerequisites and Building

You'll need the xtensa compiler and a copy of esputil

Get and install esputil, my version of cpq's dependency-free, C esptool, so we don't need python.
```c
mkdir ~/esp
cd ~/esp
git clone https://github.com/cnlohr/esputil
cd esputil
make

cd ~/esp
wget https://dl.espressif.com/dl/xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-amd64.tar.gz
tar xzvpf xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-amd64.tar.gz
```

The second instructions from https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/linux-setup.html for xtensa-lx106 compiler
(Remember that you just need the compiler, so just download and uncompress the compressed file depending on your system and edit the makefile)

I strongly recommend using a Linux system.  Follow the information on Espressif docs site for more details and lists of Linux package prerequisites.

NOTE: On some linux systems, brltty may take over unnamed serial ports and make your day awful.  Uninstall it. `apt purge brltty pop-desktop`


## Altering the Makefile

The makefile has some defaults, however, you will likely need to modify the ESP_OPEN_SDK parameter to compile on your system, so they point to your esp open sdk.

```
ESPUTIL:=/.../esputil
GCC_FOLDER:=/.../xtensa-lx106-elf
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

## Tips I Learned

1. GCC Xtensa will create smaller code if you have volatile pointers to arrays, rather than setting absolute memory addresses.  Indirect accessing isn't slower, and it makes smaller code.  Additionally, if you just store the pointer to the base of an array, like the IO MUX register, and you index into it multiple times, it doesn't need to take up prescious space holding that.

2. Avoid using macro's to do pointer arithmatic.  Also, try to find out where you are or'ing masks, etc. where you don't need to be.

3. Always make sure to have your function declarations available when used.  Failure to do this gimps -flto's ability to inline functions.

4. Compile with -g to make the assembly listing much easier to read. 

I'm sure there's more...
# Todo

* Figure out how to get rid of the prologue to main().  Even though it's marked as noreturn, GCC is doing stuff with the stack, etc.

* Add Sleep feature.

* Figure out why power consumption is higher than I would expect.


# Ancient notes

# PLL Valid Values

This table represents each possible value for the function rom_i2c_writeReg (103, 4, 1, 0xVAL), (you need replace "VAL" with the desired register, for example if in "VAL" you put 0xF9, you should expect the PLL running at 52 MHz)
"ERR" means that the ESP8266 won't boot
All frequencies are in MHz.

| | 0  | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | A   | B   | C   | D   | E   | F   |
|-| -- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
|0| 45 | 45  | 45  | 45  | 45  | 45  | 45  | 46  | 45  | 46  | 45  | 46  | 45  | 46  | 46  | 45  |
|1| 46 | 46  | 46  | 46  | 45  | 45  | 45  | 46  | 45  | 46  | 46  | 46  | 46  | 45  | 46  | 45  |
|2| 46 | 46  | 46  | 45  | 45  | 45  | 45  | 46  | 45  | 45  | 45  | 46  | 45  | 46  | 45  | 46  |
|3| 46 | 46  | 46  | 45  | 46  | 44  | 46  | 45  | 46  | 46  | 45  | 46  | 45  | 42  | 41  | 39  |
|4| 92 | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  |
|5| 92 | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  | 92  |
|6| 46 | 92  | 92  | 92  | 92  | 92  | 92  | 91  | 90  | 92  | 92  | 93  | 91  | 86  | 82  | 78  |
|7| 92 | 89  | 84  | 76  | 73  | 68  | 64  | 59  | 57  | 50  | 51  | 48  | 46  | 43  | 42  | 39  |
|8| 45 | 184 | 183 | 183 | 183 | 184 | 184 | 183 | 184 | 183 | 183 | 184 | 183 | 183 | 183 | 184 |
|9| 45 | 184 | 183 | 184 | 183 | 183 | 183 | 183 | 183 | 184 | 184 | 184 | 184 | 184 | 184 | 184 |
|A| 46 | 178 | 170 | 155 | 147 | 136 | 129 | 120 | 114 | 106 | 102 | 95  | 91  | 86  | 83  | 78  |
|B| 46 | 89  | 85  | 78  | 74  | 68  | 65  | 60  | 58  | 54  | 51  | 48  | 46  | 43  | 42  | 40  |
|C| 46 | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR |
|D| 46 | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR | ERR |
|E| 45 | 176 | 167 | 153 | 145 | 134 | 127 | 118 | 112 | 105 | 100 | 94  | 90  | 85  | 81  | 77  |
|F| 48 | 88  | 84  | 77  | 73  | 67  | 64  | 59  | 56  | 52  | 50  | 47  | 45  | 42  | 41  | 38  |

# Benchmark

To perform this benchmark I used this function, basically what I wanted to measure was the time that the ESP8266 took to define what numbers if the numbers from 0 to 100000 are prime.

I use this function on a for loop:

```
int is_prime(unsigned int n) {
   	if (n <= 1) {
   		return 0; // zero and one are not prime
   	}
   	unsigned int i = 0;
   	for (i = 2; i * i <= n; i++) {
       	if (n % i == 0) {
       		return 0;
       	}
    }
    return 1;
} 
```
(ofc all the other code was commented and we was just running this function)

The result of calculate 100k prime numbers was:

| Frequency | Required Time |
| --------- | ------------- |
| 52 MHz | 5055 ms |
| 80 MHz | 3281 ms |
| 115 MHz | 2298 ms |
| 160 MHz | 1634 ms |
| 173 MHz | 1540 ms |
| 189 MHz | 1400 ms |
| 231 MHz | 1170 ms |
| 320 MHz | 841 ms |
| 346 MHz | 750 ms |

