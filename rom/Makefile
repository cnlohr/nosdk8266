all :

.PHONY : disrom

ESPTOOL:=~/esp8266/esptool/esptool.py
ESPTOOLOPTS:=-b 1000000
GCC_BASE:=~/esp8266/esp-open-sdk/xtensa-lx106-elf/bin/xtensa-lx106-elf-
OBJDUMP:=$(GCC_BASE)objdump
OBJCOPY:=$(GCC_BASE)objcopy
GCC:=$(GCC_BASE)gcc

CFLAGS:=-mlongcalls -Os

rom.bin :
	$(ESPTOOL) $(ESPTOOLOPTS) dump_mem 0x40000000 65536 rom.bin

rom.elf : rom.bin
	g++ make_s.cpp -o make_s
	./make_s > rom.s
	$(GCC) rom.s -o rom.elf -nostdlib -T eagle.rom.addr.v6.ld -T re_rom.ld -g

disrom : rom.elf
	$(OBJDUMP) $^ -m xtensa -D > disrom.s


clean :
	rm -rf rom.s rom.bin
