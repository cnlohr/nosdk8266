#ifndef _ROMLIB_H
#define _ROMLIB_H

//Useful, more bloated ROM functions.

#if MAIN_MHZ == 52
	#define PERIPH_FREQ 52
#elif MAIN_MHZ == 104
	#define PERIPH_FREQ 52
#elif MAIN_MHZ == 80
	#define PERIPH_FREQ 80
#elif MAIN_MHZ == 115
	#define PERIPH_FREQ 115
#elif MAIN_MHZ == 173
	#define PERIPH_FREQ 173
#elif MAIN_MHZ == 189
	#define PERIPH_FREQ 189
#elif MAIN_MHZ == 160	
	#define PERIPH_FREQ 80
#elif MAIN_MHZ == 231
	#define PERIPH_FREQ 115
#elif MAIN_MHZ == 346
	#define PERIPH_FREQ 173
#elif MAIN_MHZ == 378  //Won't boot on my ESP.  PLL@5.5 (how?)
	#define PERIPH_FREQ 189
#else
	#error System MHz must be 52, 80, or 160
#endif

extern uint32_t _bss_start;
extern uint32_t _bss_end;
extern volatile uint32_t * DPORT_BASEADDR;// = (volatile uint32_t *)0x3ff00000;
extern volatile uint32_t * PIN_BASE; // = (volatile uint32_t *)0x60000300;
extern volatile uint32_t * IO_BASE;  // = (volatile uint32_t *)0x60000000;
extern volatile uint32_t * IOMUX_BASE; // = (volatile uint32_t *)0x60000800
extern volatile uint32_t * SPI0_BASE; // = (volatile uint32_t *)0x60000200;
extern volatile uint8_t  * RTCRAM; //Pointer to RTC Ram (1024 bytes)

#define HWREG(BASE, OFF)       BASE[OFF>>2]

//Sets clock frequency, PLL and initializes BSS.
void nosdk8266_init();


#endif

