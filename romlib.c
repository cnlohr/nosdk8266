#include <esp8266_auxrom.h>
#include <uart_dev.h>
#include <eagle_soc.h>
#include <ets_sys.h>

#define UART0 0

extern UartDevice UartDev;

int puts( const char * str )
{
	int i = 0;
	while( str[i] )
		ets_putc( str[i++] );
}

void romlib_init()
{
	ets_update_cpu_frequency( MAIN_MHZ );
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_U0TXD_U);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD);
	uart_div_modify(UART0, (MAIN_MHZ*1000000)/115200);
	rom_rfpll_reset();
	gpio_init();

	
}

