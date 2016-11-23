#include "c_types.h"
#include "esp8266_auxrom.h"
#include "esp8266_rom.h"
#include "eagle_soc.h"
#include "ets_sys.h"
#include "gpio.h"

//PICO66 is ideal for running an absolutely minimal build.
//If you want printf(...) you'll need PICOWITHPRINT and call romlib_init().
//If you are running for the HackADay 1k challenge: https://hackaday.io/contest/18215-the-1kb-challenge
//you will need to #define PICONOPRINT.
//
//I recommend testing without it, i.e. allow printf.  Then, strip it.


#ifdef PICO66

//Tricky: We do it this way to force GCC to emit the code for the 'call' command instead of trying to do a callx0 or something.
#define call_delay_us( time ) { asm volatile( "mov.n a2, %0\n_call0 delay4clk" : : "r"(time*13) : "a2" ); }

int call_user_start()
{
	//This handles zeroin'g the BSS RAM, as well as setting up the serial prot to 115k Baud
	romlib_init();

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U,FUNC_GPIO2);
	PIN_DIR_OUTPUT = _BV(2); //Enable GPIO2 light off.

	while(1)
	{
		printf( "Hello\n" );
		PIN_OUT_SET = _BV(2); //Turn GPIO2 light off.
		call_delay_us(1000000);
		PIN_OUT_CLEAR = _BV(2); //Turn GPIO2 light off.
		call_delay_us(1000000);
	}
}


#else

int main()
{
	int i = 0;
	romlib_init();

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U,FUNC_GPIO2);
	PIN_DIR_OUTPUT = _BV(2); //Enable GPIO2 light off.

	while(1)
	{
		printf( "Hello World %d\n", i );

		PIN_OUT_SET = _BV(2); //Turn GPIO2 light off.
		ets_delay_us( 500000 );
		PIN_OUT_CLEAR = _BV(2); //Turn GPIO2 light off.
		ets_delay_us( 500000 );

		i++;
	}
}
#endif

