#include "nosdkradio.h"
#include "nosdk8266.h"
#include "esp8266_rom.h"
/*
const uint8 esp_init_data_default[128] = {
	    5,    0,    4,    2,    5,    5,    5,    2,    5,    0,    4,    5,    5,    4,    5,    5,
	    4, 0xFE, 0xFD, 0xFF, 0xF0, 0xF0, 0xF0, 0xE0, 0xE0, 0xE0, 0xE1,  0xA, 0xFF, 0xFF, 0xF8,    0,
	 0xF8, 0xF8, 0x52, 0x4E, 0x4A, 0x44, 0x40, 0x38,    0,    0,    1,    1,    2,    3,    4,    5,
	    1,    0,    0,    0,    0,    0,    2,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	 0xE1,  0xA,    0,    0,    0,    0,    0,    0,    0,    0,    1, 0x93, 0x43,    0,    0,    0,
	    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	    3,    0,    2,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0
};*/

#define OTP_MAC0	DPORT_BASEADDR[20]
#define OTP_MAC1	DPORT_BASEADDR[21]
#define OTP_CHIPID	DPORT_BASEADDR[22] 

uint8 * phy_rx_gain_dc_table;
uint8  MyMAC[6];
uint8_t epb[500];

void rom_chip_50_set_channel( uint32_t ch );

void init_wifi()
{
	//Step 1: Read MAC
	MyMAC[0] = 0x18;
	MyMAC[1] = 0xfe;
	MyMAC[2] = 0x34;
	MyMAC[3] = (OTP_MAC1 >> 8) & 0xff;
	MyMAC[4] = OTP_MAC1 & 0xff;
	MyMAC[5] = (OTP_MAC0 >> 24) & 0xff;

	eprintf_init_buf( epb );
	phy_disable_agc();
	rom_chip_v5_rx_init();
	rom_chip_v5_tx_init();
	printf( "Enabled AGC\n" );
	rom_set_txbb_atten();
	rom_cal_tos_v50();

//	rom_set_channel_freq( 0 ); //Channel
	rom_chip_50_set_channel( 1 );
	printf( "Set channel\n" );
	phy_enable_agc();

	//read_macaddr_from_otp();
//	ets_memcpy( pbuf, esp_init_data_default, sizeof( esp_init_data_default ) );
//	phy_rx_gain_dc_table = &pbuf[0x100];
//	int xflg;
/*
	if(flash_data_check(&pbuf[128]) != 0 || phy_check_data_table(phy_rx_gain_dc_table, 125, 1) != 0)
		xflg = 1;
	else
		xflg = 0;
*/
//	if(rst_if.reason != REASON_DEEP_SLEEP_AWAKE) {
//		phy_afterwake_set_rfoption(1);
//		if(xflg == 0) write_data_to_rtc(pbuf+128);
//	}
//	pbuf[0xf8] = 0;
	//

	//init_wifi_internal(pbuf, MyMAC);

	//
//	if((pbuf[sys_const_freq_correct_en] & 5) == 1) g_ic.c[491] = 1;
/*	else g_ic.c[491] = 0;
	if(xflg != 0) {

		if((rgrr == 1) // ch_pd
		|| ((rst_if.reason != REASON_DEEP_SLEEP_AWAKE) && (rgrr == 2))) { // pin reset
#ifdef DEBUG_UART
			os_printf("Save rf_cal (%u, %u, %u, %u)\n", xflg, g_ic.c[491], rst_if.reason, rgrr);
#endif
			get_data_from_rtc(pbuf+128);
			spi_flash_write(rf_cal_sec*flashchip_sector_size, (uint32 *)&pbuf[128], SIZE_SAVE_SYS_CONST-128);
		}
	}
	os_free(pbuf);

	//
#ifdef DEBUG_UART
	os_print_reset_error(); // вывод фатальных ошибок, вызвавших рестарт. см. в модуле wdt
#endif
	RTC_MEM(0) = 0; //	system_rtc_mem_write(0, &rst_if, sizeof(rst_if));
#else	// DEF_SDK_VERSION < 2000
#if DEF_SDK_VERSION >= 1400
	uint8 * buf = os_malloc(SIZE_SAVE_SYS_CONST);
	spi_flash_read(esp_init_data_default_faddr,(uint32 *)buf, SIZE_SAVE_SYS_CONST); // esp_init_data_default.bin + ???
#if DEF_SDK_VERSION >= 2000
	if(buf[sys_const_freq_correct_en] == 3) g_ic.c[491] = 1; // esp_init_data_default: freq_correct_en[112]
	else g_ic.c[491] = 0;

#elif DEF_SDK_VERSION >= 1410
	if(buf[sys_const_freq_correct_en] == 3) g_ic.c[471] = 1; // esp_init_data_default: freq_correct_en[112]
	else g_ic.c[471] = 0;
#endif
	buf[0xf8] = 0;
*/
//	phy_rx_gain_dc_table = &buf[0x100];
//	phy_rx_gain_dc_flag = 0;
	// **
	// user_rf_pre_init(); // не использется, т.к. мождно вписать что угодно и тут :)
    //	system_phy_set_powerup_option(0);
	//	system_phy_set_rfoption(1);
	// **
/*
#elif DEF_SDK_VERSION >= 1300
	uint8 *buf = (uint8 *)os_malloc(256); // esp_init_data_default.bin
	spi_flash_read(esp_init_data_default_faddr,(uint32 *)buf, esp_init_data_default_size); // esp_init_data_default.bin
#else
	uint8 *buf = (uint8 *)os_malloc(esp_init_data_default_size); // esp_init_data_default.bin
	spi_flash_read(esp_init_data_default_faddr,(uint32 *)buf, esp_init_data_default_size); // esp_init_data_default.bin
#endif
	//
	if(buf[0] != 5) { // первый байт esp_init_data_default.bin не равен 5 ? - бардак!
#ifdef DEBUG_UART
		os_printf("\nError esp_init_data! Set default.\n");
#endif
		ets_memcpy(buf, esp_init_data_default, esp_init_data_default_size);
	}
//	system_restoreclock(); // STARTUP_CPU_CLK
//	init_wifi(buf, info.st_mac); // инициализация WiFi

#if DEF_SDK_VERSION >= 1400
	if(buf[0xf8] == 1 || phy_rx_gain_dc_flag == 1) { // сохранить новые калибровки RF/VCC33 ?
#ifdef DEBUG_UART
		os_printf("\nSave rx_gain_dc table (%u, %u)\n", buf[0xf8], phy_rx_gain_dc_flag );
#endif
		wifi_param_save_protect_with_check(esp_init_data_default_sec, flashchip_sector_size, buf, SIZE_SAVE_SYS_CONST);
	}
#endif
	os_free(buf);
*/

/*

	//
#if DEF_SDK_VERSION >= 1400 // (SDK 1.4.0)
	system_rtc_mem_read(0, &rst_if, sizeof(rst_if));
//	os_printf("RTC_MEM(0) = %u,%u,%p \n", rst_if.reason, IO_RTC_SCRATCH0, RTC_RAM_BASE[0x78>>2]);
#if DEF_SDK_VERSION >= 1520
	{
		uint32 reset_reason = IO_RTC_SCRATCH0;
		if(reset_reason >= REASON_EXCEPTION_RST && reset_reason < REASON_DEEP_SLEEP_AWAKE) {
			// reset_reason == REASON_EXCEPTION_RST, REASON_SOFT_WDT_RST, REASON_SOFT_RESTART, REASON_DEEP_SLEEP_AWAKE
			TestStaFreqCalValInput = RTC_RAM_BASE[0x78>>2]>>16;
			chip_v6_set_chan_offset(1, TestStaFreqCalValInput);
		}
		else {
			TestStaFreqCalValInput = 0;
			RTC_RAM_BASE[0x78>>2] &= 0xFFFF;
			if(reset_reason == REASON_DEFAULT_RST) {
				reset_reason = rtc_get_reset_reason();
				if(reset_reason == 1) { // =1 - ch_pd
					ets_memset(&rst_if, 0, sizeof(rst_if)); // rst_if.reason = REASON_DEFAULT_RST
				}
				else if(reset_reason == 2) { // =2 - reset
					if(rst_if.reason != REASON_DEEP_SLEEP_AWAKE
					 ||	rst_if.epc1 != 0
					 || rst_if.excvaddr != 0) {
						ets_memset(&rst_if, 0, sizeof(rst_if));
						rst_if.reason = REASON_EXT_SYS_RST;
						RTC_MEM(0) = REASON_EXT_SYS_RST;
					}
				}
			}
			else if(reset_reason > REASON_EXT_SYS_RST) {
				ets_memset(&rst_if, 0, sizeof(rst_if)); // rst_if.reason = REASON_DEFAULT_RST
			}
		}
	}
#else
	if (rst_if.reason >= REASON_EXCEPTION_RST && rst_if.reason < REASON_DEEP_SLEEP_AWAKE) { // >= 2 < 5
		// 2,3,4 REASON_EXCEPTION_RST, REASON_SOFT_WDT_RST, REASON_SOFT_RESTART
		TestStaFreqCalValInput = RTC_RAM_BASE[0x78>>2]>>16; // *((volatile uint32 *)0x60001078) >> 16
		chip_v6_set_chan_offset(1, TestStaFreqCalValInput);
	}
	else {
		TestStaFreqCalValInput = 0;
		RTC_RAM_BASE[0x78>>2] &= 0xFFFF; // *((volatile uint32 *)0x60001078) &= &0xFFFF;
		if(rst_if.reason > REASON_EXT_SYS_RST) rst_if.reason = REASON_DEFAULT_RST;
		if(rst_if.reason != REASON_DEEP_SLEEP_AWAKE && rtc_get_reset_reason() == 2) {
			rst_if.reason = REASON_EXT_SYS_RST; // = 6
		}
//		else if(rst_if.reason == REASON_WDT_RST && rtc_get_reset_reason() == 1) rst_if.reason = REASON_DEFAULT_RST;
	}
#endif
*/

}

