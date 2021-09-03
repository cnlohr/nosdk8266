TARGET_OUT = image.elf
all : $(TARGET_OUT)

#SUBMODULE if we are including this in another project.
SUBMODULE?=NO

#Pick from *52, *80, 104 or *115, 160, *173, *189, 231, 346
# = Clock Lower and unreliable.  Warning. Peripheral clocks of >115 will NOT boot without a full power-down and up. (Don't know why). * = peripheral clock at processor clock.
MAIN_MHZ?=346
USE_I2S?=YES

ESP_OPEN_SDK:=~/esp/esp-open-sdk

GCC_FOLDER:=$(ESP_OPEN_SDK)/xtensa-lx106-elf
ESPTOOL:=$(ESP_OPEN_SDK)/esptool/esptool.py
PREFIX:=$(GCC_FOLDER)/bin/xtensa-lx106-elf-
SIZE:=$(PREFIX)size
GCC:=$(PREFIX)gcc

#-mno-serialize-volatile will prevent extra memw things from being generated.

ifneq (YES, $(SUBMODULE))
	SRCPREFIX:=
	CFLAGS:=$(CFLAGS) -flto
	SRCS:=$(SRCS) main.c
else
	SRCPREFIX:=nosdk8266/
endif

LDFLAGS:=-T $(SRCPREFIX)ld/linkerscript.ld -T $(SRCPREFIX)ld/addresses.ld
FOLDERPREFIX:=$(GCC_FOLDER)/bin
PORT:=/dev/ttyS12

ifeq (YES, $(USE_I2S))
	SRCS:=$(SRCS) $(SRCPREFIX)src/nosdki2s.c
	CFLAGS:=$(CFLAGS) -DUSE_I2S
else ifeq (SUBMODULE, $(USE_I2S))
	CFLAGS:=$(CFLAGS) -DUSE_I2S
endif

#Adding the -g flag makes our assembly easier to read and does not increase size of final executable.
CFLAGS:=$(CFLAGS) -Ofast -I$(SRCPREFIX)include -nostdlib -DMAIN_MHZ=$(MAIN_MHZ) -mno-serialize-volatile -mlongcalls -g
SRCS:=$(SRCS) $(SRCPREFIX)src/startup.S $(SRCPREFIX)src/nosdk8266.c

$(TARGET_OUT) : $(SRCS)
	$(GCC) $(CFLAGS) $(SRCS)  $(LDFLAGS) -o $@
	nm -S -n $(TARGET_OUT) > image.map
	$(SIZE) $@
	$(PREFIX)objdump -S $@ > image.lst
	PATH=$(FOLDERPREFIX):$$PATH;$(ESPTOOL) elf2image $(TARGET_OUT) 

burn : $(FW_FILE_1) $(FW_FILE_2) $(TARGET_OUT)
	($(ESPTOOL) --port $(PORT) write_flash 0x00000 image.elf-0x00000.bin -ff 80m -fm dout)||(true)

clean :
	rm -rf $(TARGET_OUT) image.map image.lst image.elf-0x10000.bin image.elf-0x00000.bin

