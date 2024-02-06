#include <esp8266_auxrom.h>
#include <esp8266_rom.h>
#include <eagle_soc.h>
#include <ets_sys.h>
#include <uart_dev.h>
#include "nosdk8266.h"

#define ets_update_cpu_frequency(x)

extern UartDevice UartDev;

volatile uint32_t * DPORT_BASEADDR = (volatile uint32_t *)0x3ff00000;
volatile uint32_t * PIN_BASE = (volatile uint32_t *)0x60000300;
volatile uint32_t * IO_BASE = (volatile uint32_t *)0x60000000;
volatile uint32_t * IOMUX_BASE = (volatile uint32_t *)0x60000800;
volatile uint32_t * SPI0_BASE = (volatile uint32_t *)0x60000200;
volatile uint8_t  * RTCRAM = (volatile uint8_t *)0x60001000; //Pointer to RTC Ram (1024 bytes)

//Thanks, https://github.com/pvvx/esp8266web/blob/2e25559bc489487747205db2ef171d48326b32d4/app/sdklib/system/app_main.c

//This is actually 0x88.  You can set this to 0xC8 to double-overclock the low-end bus.
//This can get you to a 160 MHz peripheral bus if setting it normally to 80 MHz.
//rom_i2c_writeReg(103,4,1,0xc8);
//rom_i2c_writeReg(103,4,1,0x88);

//	... Looks like the main PLL operates at 1040 MHz
//	If the divisor is <9, it seems the chip cannot boot on its own, without reboot.
//	It looks like when divisor goes less than 5, it somehow jumps to exactly 5.5.
//
//   0x0X: 190ISH MHz (Won't link)	| x5.5*
//   0x4X: 189.4ISH MHz?			| x5.5*
//   0x2X: 189.0ISH MHz 			| x5.5*
//   0x6X: 189.0ISH MHz?			| x5.5*
//   0x8X: 189.0ISH MHz?			| x5.5*
//   0xA1: 173.3? MHz?  			| x6
//   0xC1: 148.49 MHz? 				| x7
//   0xE1: 130 MHz?					| x8
//   0x1X: 115.5ISH MHz 			| x9
//   0x3X: 104.0ISH MHz?			| x10
//   0x5X: 94.61 MHz?				| x11
//   0x7X: 86.6ISH MHz?				| x12
//   0x91: 80 MHz Stock				| x13 (default)
//   0xB1: 74.33 MHz?				| x14
//   0xD1: 69.32 MHz?				| x15
//	 0xF1: 65 MHz.					| x16

//Processor requires a hard reboot to link when peripheral bus is operating past 115 MHz (Not sure why)
//ESP8285, when operating at divisors less than 6 seems to go to close to 5, and will boot, but never reaches 5.

extern uint32_t _bss_start;
extern uint32_t _bss_end;

void nosdk8266_init() {
#if MAIN_MHZ == 52
	ets_update_cpu_frequency(52);
#elif MAIN_MHZ == 80
	rom_i2c_writeReg(103, 4, 1, 0x88);
	rom_i2c_writeReg(103, 4, 2, 0x91);
 	ets_update_cpu_frequency(80);
#elif MAIN_MHZ == 115
 	rom_i2c_writeReg(103, 4, 1, 0x88);
 	rom_i2c_writeReg(103, 4, 2, 0x11);
 	ets_update_cpu_frequency(80);
#elif MAIN_MHZ == 160
 	rom_i2c_writeReg(103, 4, 1, 0xc8);
 	rom_i2c_writeReg(103, 4, 2, 0x91);
 	ets_update_cpu_frequency(80);
#elif MAIN_MHZ == 173
 	rom_i2c_writeReg(103, 4, 1, 0x88);
 	rom_i2c_writeReg(103, 4, 2, 0xa1);
 	ets_update_cpu_frequency(80);
#elif MAIN_MHZ == 189
 	rom_i2c_writeReg(103, 4, 1, 0x88);
 	rom_i2c_writeReg(103, 4, 2, 0x81);
 	ets_update_cpu_frequency(80);
#elif MAIN_MHZ == 231
 	rom_i2c_writeReg(103, 4, 1, 0x88);
 	rom_i2c_writeReg(103, 4, 2, 0x11);
	DPORT_BASEADDR[0x14 / 4] |= 0x01; //Overclock bit.
	ets_update_cpu_frequency(160);
#elif MAIN_MHZ == 320
 	rom_i2c_writeReg(103, 4, 1, 0xc8);
 	rom_i2c_writeReg(103, 4, 2, 0x91);
	DPORT_BASEADDR[0x14 / 4] |= 0x01; //Overclock bit.
	ets_update_cpu_frequency(160);
#elif MAIN_MHZ == 346
 	rom_i2c_writeReg(103, 4, 1, 0x88);
 	rom_i2c_writeReg(103, 4, 2, 0xa1);
	DPORT_BASEADDR[0x14 / 4] |= 0x01; //Overclock bit.
	ets_update_cpu_frequency(160);
#else
	#error System MHz must be 52, 80, 115, 160, 173, 189, 231, 320 or 346 (for now)
#endif

	uint32_t *addr;
	for (addr = &_bss_start; addr < &_bss_end; addr++) { *addr = 0; }

	PIN_PULLUP_DIS(PERIPHS_IO_MUX_U0TXD_U);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD);

	uart_div_modify(0, (PERIPH_FREQ * 1000000) / 115200);
}
