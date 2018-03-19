TARGET_OUT = image.elf
all : $(TARGET_OUT)

#First, select whether you are targeting PICO or REGULAR.
#
#PICO Operates at 52 MHz, and makes no calls to ROM.
# It's ideal for the HackADay 1kB Challenge.
# You can also run PICO at 104 MHz but it takes a couple extra bytes to set the overclocking bits.
#
#REGULAR Operates at a variety of frequencies and allows for a number of
# ROM functions.

#SUBMODULE if we are including this in another project.
SUBMODULE?=NO

BUILD?=PICO
#BUILD:=REGULAR
MAIN_MHZ?=320 #Pick from *52, *80, 104 or *115, 160, *173, *189#, 231, 346, 378#  * = peripheral clock at processor clock. # = Mine won't boot + on ESP8285, Clock Lower and unreliable.  Warning. Peripheral clocks of >115 will NOT boot without a full power-down and up. (Don't know why)
USE_I2S?=YES
#USE_PRINT:=YES

ESP_OPEN_SDK:=~/esp8266/esp-open-sdk

FW_1 = image.elf-0x10000.bin
FW_2 = image.elf-0x00000.bin
GCC_FOLDER:=$(ESP_OPEN_SDK)/xtensa-lx106-elf
ESPTOOL:=$(ESP_OPEN_SDK)/esptool/esptool.py
ESPTOOLOPTS?=-b 115200
PREFIX:=$(GCC_FOLDER)/bin/xtensa-lx106-elf-
SIZE:=$(PREFIX)size
OBJDUMP:=$(PREFIX)objdump
OBJCOPY:=$(PREFIX)objcopy
GCC:=$(PREFIX)gcc
ADDITIONAL_DEPS?=

#-mno-serialize-volatile will prevent extra memw things from being generated.

ifneq (YES, $(SUBMODULE))
	SRCPREFIX:=./
	ifeq (REGULAR, $(BUILD))
		#Non-PIOC66 mode (Regular, 80 MHz, etc.)
		CFLAGS:=$(CFLAGS) -flto
		SRCS:=$(SRCS) main.c
	else ifeq (PICO, $(BUILD))
		#PICO66 Mode... If you want an absolutely strip down environment (For the HaD 1kB challenge)
		CFLAGS:=$(CFLAGS) -DPICO66 -flto
			#TODO: Why can't we use -fwhole-program instead of -flto?
		SRCS:=$(SRCS) pico.c
	else
		ERR:=$(error Need either REGULAR or PICO to be defined to BUILD.  Currently $(BUILD))
	endif
else
	SRCPREFIX:=nosdk8266/
endif


LDFLAGS:=-T $(SRCPREFIX)ld/linkerscript.ld -T $(SRCPREFIX)ld/addresses.ld
FOLDERPREFIX:=$(GCC_FOLDER)/bin
PORT:=/dev/ttyUSB0


ifeq (YES, $(USE_PRINT))
	CFLAGS:=$(CFLAGS)
else
	CFLAGS:=$(CFLAGS) -DPICONOPRINT
endif


ifeq (YES, $(USE_I2S))
	SRCS:=$(SRCS) $(SRCPREFIX)/src/nosdki2s.c
	CFLAGS:=$(CFLAGS) -DUSE_I2S
else ifeq (SUBMODULE, $(USE_I2S))
	CFLAGS:=$(CFLAGS) -DUSE_I2S
endif

#Adding the -g flag makes our assembly easier to read and does not increase size of final executable.
CFLAGS:=$(CFLAGS) -Os -I$(SRCPREFIX)include -nostdlib  -DMAIN_MHZ=$(MAIN_MHZ)  -mno-serialize-volatile -mlongcalls -g
SRCS:=$(SRCS) $(SRCPREFIX)src/startup.S $(SRCPREFIX)src/nosdk8266.c

$(TARGET_OUT) : $(ADDITIONAL_DEPS) $(SRCS)
	@echo $(shell echo $(shell cat count.txt)+1) | bc > count.txt
	$(GCC) $(CFLAGS) $(SRCS)  $(LDFLAGS) -o $@
	#objdump -t $(TARGET_OUT) > image.map
	nm -S -n $(TARGET_OUT) > image.map
	$(SIZE) $@
	$(PREFIX)objdump -S $@ > image.lst
	PATH=$(FOLDERPREFIX):$$PATH;$(ESPTOOL) elf2image $(TARGET_OUT) 

burn : $(FW_FILE_1) $(FW_FILE_2) $(TARGET_OUT)
	($(ESPTOOL) --port $(PORT) write_flash 0x00000 image.elf-0x00000.bin -ff 80m -fm dout)||(true)
	sleep .1
	($(ESPTOOL) --port $(PORT) run)||(true)

clean :
	rm -rf $(TARGET_OUT) image.map image.lst $(FW_1) $(FW_2)

