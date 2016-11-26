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

#BUILD:=PICO
BUILD:=PICO
MAIN_MHZ:=346  #Pick from *52, *80, 104 or *115, 160, *173, *189#, 231, 346, 378#  * = peripheral clock at processor clock. # = Mine won't boot + on ESP8285, Clock Lower and unreliable.  Warning. Peripheral clocks of >115 will NOT boot without a full power-down and up. (Don't know why)


ESPTOOL:=~/esp8266/esptool/esptool.py
ESPTOOLOPTS:=-b 115200
GCC_FOLDER:=~/esp8266/esp-open-sdk/xtensa-lx106-elf
PREFIX:=$(GCC_FOLDER)/bin/xtensa-lx106-elf-
SIZE:=$(PREFIX)size
OBJDUMP:=$(PREFIX)objdump
OBJCOPY:=$(PREFIX)objcopy
GCC:=$(PREFIX)gcc

#-mno-serialize-volatile will prevent extra memw things from being generated.

LDFLAGS:=-T linkerscript.ld -T addresses.ld
FOLDERPREFIX:=$(GCC_FOLDER)/bin
GCC_FOLDER:=~/esp8266/esp-open-sdk/xtensa-lx106-elf
ESPTOOL_PY:=~/esp8266/esptool/esptool.py
PORT:=/dev/ttyUSB0


ifeq (REGULAR, $(BUILD))
	#Non-PIOC66 mode (Regular, 80 MHz, etc.)
	PRINTOK:=
	CFLAGS:=-mlongcalls -Os -Iinclude -nostdlib -DMAIN_MHZ=$(MAIN_MHZ) -flto -mno-serialize-volatile
	SRCS:=main.c
else ifeq (PICO, $(BUILD))
	#PICO66 Mode... If you want an absolutely strip down environment (For the HaD 1kB challenge)
	PRINTOK:=-DPICONOPRINT
	CFLAGS:=-mlongcalls -Os -Iinclude -nostdlib -DMAIN_MHZ=$(MAIN_MHZ) -flto -DPICO66 $(PRINTOK) -mno-serialize-volatile
	SRCS:=pico.c
else
	ERR:=$(error Need either REGULAR or PICO to be defined to BUILD.  Currently $(BUILD))
endif


SRCS:=$(SRCS) startup.S nosdk8266.c nosdki2s.c

$(TARGET_OUT) : $(SRCS)
	@echo $(shell echo $(shell cat count.txt)+1) | bc > count.txt
	$(GCC) $(CFLAGS) $^  $(LDFLAGS) -o $@  -Wa,-a,-ad > image_inline.lst
	nm -S -n $(TARGET_OUT) > image.map
	$(SIZE) $@
	$(PREFIX)objdump -S $@ > image.lst
	PATH=$(FOLDERPREFIX):$$PATH;$(ESPTOOL_PY) elf2image $(TARGET_OUT) 

burn : $(FW_FILE_1) $(FW_FILE_2)
	($(ESPTOOL_PY) --port $(PORT) write_flash 0x00000 image.elf-0x00000.bin -ff 80m -fm dout)||(true)
	sleep .1
	($(ESPTOOL_PY) --port $(PORT) run)||(true)

clean :
	rm -rf $(TARGET_OUT) image.map image.lst $(FW_1) $(FW_2)

