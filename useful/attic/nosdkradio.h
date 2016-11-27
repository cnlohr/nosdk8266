#ifndef _NOSDKRADIO_H
#define _NOSDKRADIO_H

#include <c_types.h>


//DOES NOT WORK AT ALL DO NOT USE!!!


enum phy_mode {
	PHY_MODE_11B	= 1,
	PHY_MODE_11G	= 2,
	PHY_MODE_11N    = 3
};

#define PHY_MODE_SET PHY_MODE_11N

void rom_chip_v5_disable_cca(void);
void rom_chip_v5_enable_cca(void);

#define phy_disable_agc rom_chip_v5_disable_cca
#define phy_enable_agc rom_chip_v5_enable_cca


//Mostly ripped from: https://github.com/pvvx/esp8266web/blob/master/app/sdklib/system/app_main.c

#define SIZE_SAVE_SYS_CONST 756 
	// размер сохранения блока системных констант в секторе с номером (max_flash - 4). SDK 1.4.0
	// GOOGLE TRANSLATE: the size of the conservation unit system constants in the sector number (max_flash - 4). SDK 1.4.0

extern uint8  MyMAC[6]; //Populated by init_wifi

ICACHE_FLASH_ATTR void init_wifi();


#endif

