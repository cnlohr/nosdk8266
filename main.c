#include "c_types.h"
#include "esp8266_auxrom.h"
#include "esp8266_rom.h"
#include "eagle_soc.h"
#include "ets_sys.h"
#include "gpio.h"
#include "useful/phy_info.h"

void rom_rfpll_set_freq( int i, int k, int l );

static void my_intr( uint32 intr_mask, void * id )
{
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(0));
	printf( "INTERRUPT %d %p\n", intr_mask, id );
}

void SetupGPIOInterrupt()
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U,FUNC_GPIO0);
	PIN_DIR_INPUT = _BV(0); //GPIO 0 Button is input
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(0));
	ETS_GPIO_INTR_ATTACH( my_intr, 0xaaaa0000 );
	gpio_pin_intr_state_set(GPIO_ID_PIN(0),GPIO_PIN_INTR_POSEDGE);  //Rising Edge Trigger.
	ETS_GPIO_INTR_ENABLE();
	printf( "GPIO Interrupt setup.\n" );
}

int main()
{
	int i = 0;

	romlib_init();

	SetupGPIOInterrupt();

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

