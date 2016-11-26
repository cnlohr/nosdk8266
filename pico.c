#include "c_types.h"
#include "esp8266_auxrom.h"
#include "esp8266_rom.h"
#include "eagle_soc.h"
#include "ets_sys.h"
#include "gpio.h"
#include "nosdk8266.h"

//PICO66 is ideal for running an absolutely minimal build.
//If you want printf(...) you'll need PICOWITHPRINT and call romlib_init().
//If you are running for the HackADay 1k challenge: https://hackaday.io/contest/18215-the-1kb-challenge
//you will need to #define PICONOPRINT.
//
//I recommend testing without it, i.e. allow printf.  Then, strip it.

//Tricky: We do it this way to force GCC to emit the code for the 'call' command instead of trying to do a callx0 or something.
#define call_delay_us( time ) { asm volatile( "mov.n a2, %0\n_call0 delay4clk" : : "r"(time*13) : "a2" ); }

#ifndef PICO66
#error This is intended only to be compiled with PICO66
#endif

int main()
{
	//This handles zeroin'g the BSS RAM, as well as setting up the serial prot to 115k Baud
	uint32_t *addr = &_bss_start;
	for (addr = &_bss_start; addr < &_bss_end; addr++)  *addr = 0; //Safe, _bss_start doesn't have to == _bss_end

#if MAIN_MHZ!=52
	nosdk8266_clock();
#endif

	uart_div_modify(UART0, (PERIPH_MHZ*1000000)/115200);  //Code will not be emitted unless printing is turned on.

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U,FUNC_GPIO2);
	PIN_DIR_OUTPUT = _BV(2); //Enable GPIO2 light off.

#ifdef USE_I2S
	InitI2S();
#endif

	while(1)
	{
#ifdef USE_I2S
		SendI2S();
#endif
		printf( "Hello\n" ); //PICO Won't print here unless printing is turned on.
		PIN_OUT_SET = _BV(2); //Turn GPIO2 light off.
		call_delay_us(1000000);
		PIN_OUT_CLEAR = _BV(2); //Turn GPIO2 light off.
		call_delay_us(1000000);
	}
}

