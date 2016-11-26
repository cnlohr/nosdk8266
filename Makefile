TARGET_OUT = image.elf
FW_1 = image.elf-0x10000.bin
FW_2 = image.elf-0x00000.bin

all : $(TARGET_OUT)

#First, select whether you are targeting PICO or REGULAR.
#
#PICO Operates at 52 MHz, and makes no calls to ROM.
# It's ideal for the HackADay 1kB Challenge.
# You can also run PICO at 104 MHz but it takes a couple extra bytes to set the overclocking bits.
#
#REGULAR Operates at a variety of frequencies and allows for a number of
# ROM functions.

BUILD:=PICO
#BUILD:=REGULAR
MAIN_MHZ:=52  #Pick from *52, *80, 104 or *115, 160, *173, *189#, 231, 346, 378#  * = peripheral clock at processor clock. # = Mine won't boot + on ESP8285, Clock Lower and unreliable.  Warning. Peripheral clocks of >115 will NOT boot without a full power-down and up. (Don't know why)
#USE_I2S:=YES


GCC_FOLDER:=~/esp8266/esp-open-sdk/xtensa-lx106-elf
ESPTOOL:=~/esp8266/esptool/esptool.py
ESPTOOLOPTS:=-b 115200
PREFIX:=$(GCC_FOLDER)/bin/xtensa-lx106-elf-
SIZE:=$(PREFIX)size
OBJDUMP:=$(PREFIX)objdump
OBJCOPY:=$(PREFIX)objcopy
GCC:=$(PREFIX)gcc

#-mno-serialize-volatile will prevent extra memw things from being generated.

LDFLAGS:=-T linkerscript.ld -T addresses.ld
FOLDERPREFIX:=$(GCC_FOLDER)/bin
PORT:=/dev/ttyUSB0


ifeq (REGULAR, $(BUILD))
	#Non-PIOC66 mode (Regular, 80 MHz, etc.)
	PRINTOK:=
	CFLAGS:=-mlongcalls -flto
	SRCS:=main.c
else ifeq (PICO, $(BUILD))
	#PICO66 Mode... If you want an absolutely strip down environment (For the HaD 1kB challenge)
	PRINTOK:=-DPICONOPRINT
	CFLAGS:=-mlongcalls -DPICO66 $(PRINTOK) -flto
		#TODO: Why can't we use -fwhole-program instead of -flto?
	SRCS:=pico.c
else
	ERR:=$(error Need either REGULAR or PICO to be defined to BUILD.  Currently $(BUILD))
endif

ifeq (YES, $(USE_I2S))
	SRCS:=$(SRCS) nosdki2s.c
	CFLAGS:=$(CFLAGS) -DUSE_I2S
endif

#Adding the -g flag makes our assembly easier to read and does not increase size of final executable.
CFLAGS:=$(CFLAGS) -Os -Iinclude -nostdlib  -DMAIN_MHZ=$(MAIN_MHZ)  -mno-serialize-volatile
SRCS:=$(SRCS) startup.S nosdk8266.c

$(TARGET_OUT) : $(SRCS)
	@echo $(shell echo $(shell cat count.txt)+1) | bc > count.txt
	$(GCC) $(CFLAGS) $^  $(LDFLAGS) -o $@
	nm -S -n $(TARGET_OUT) > image.map
	$(SIZE) $@
	$(PREFIX)objdump -S $@ > image.lst
	PATH=$(FOLDERPREFIX):$$PATH;$(ESPTOOL) elf2image $(TARGET_OUT) 

burn : $(FW_FILE_1) $(FW_FILE_2)
	($(ESPTOOL) --port $(PORT) write_flash 0x00000 image.elf-0x00000.bin -ff 80m -fm dout)||(true)
	sleep .1
	($(ESPTOOL) --port $(PORT) run)||(true)

clean :
	rm -rf $(TARGET_OUT) image.map image.lst $(FW_1) $(FW_2)

