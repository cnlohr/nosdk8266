#include "c_types.h"
#include "esp8266_auxrom.h"
#include "esp8266_rom.h"
#include "eagle_soc.h"
#include "ets_sys.h"
#include "gpio.h"
#include "romlib.h"

#define call_delay_us( time ) { asm volatile( "mov.n a2, %0\n_call0 delay4clk" : : "r"(time*13) : "a2" ); }

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
		call_delay_us( 500000 );
		PIN_OUT_CLEAR = _BV(2); //Turn GPIO2 light off.
		call_delay_us( 500000 );
		i++;
	}
}

