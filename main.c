//#include "c_types.h"
#include "esp8266_auxrom.h"
#include "eagle_soc.h"
#include "nosdk8266.h"
//#include <math.h>

// TODO: Use float number (related to 8) to fix the drift
#define call_delay_us(time) { asm volatile("mov.n a2, %0\n_call0 delay4clk" : : "r"(time * (MAIN_MHZ / 8)) : "a2" ); }

int main() {
	int i = 0;
	nosdk8266_init();

	// Configure GPIO5 (TX) and GPIO2 (LED)
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U,FUNC_GPIO2);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U,FUNC_GPIO5);
	PIN_DIR_OUTPUT = _BV(2); //Enable GPIO2 light off.

	//call_delay_us( 3000000 );
	//int j = 0;
	//for (j = 0; j < 200; j++) {
	//	uart_div_modify(0, (j * 1000000) / 115200);
	//	printf("pllworkingfreq: %d ", j);
	//}

 	ets_update_cpu_frequency(80);
 	rom_i2c_writeReg(103, 4, 1, 0x88);
 	rom_i2c_writeReg(103, 4, 2, 0xb1);
	DPORT_BASEADDR[0x14 / 4] |= 0x01; //Overclock bit.


	while(1) {
		PIN_OUT_SET = _BV(2); //Turn GPIO2 light off.
		call_delay_us(1000000);
		printf("Hello World %d\n", i);
		//printf("PLL divider register values: (1)0x%x | (2)0x%x\n", rom_i2c_readReg(103, 4, 1), rom_i2c_readReg(103, 4, 2));
		PIN_OUT_CLEAR = _BV(2); //Turn GPIO2 light off.
		call_delay_us(1000000);
		i++;
	}
}

