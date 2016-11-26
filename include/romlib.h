#ifndef _ROMLIB_H
#define _ROMLIB_H

//Useful, more bloated ROM functions.

extern uint32_t _bss_start;
extern uint32_t _bss_end;
extern volatile uint32_t * DPORT_BASEADDR;// = (volatile uint32_t *)0x3ff00000;
extern volatile uint32_t * PIN_BASE; // = (volatile uint32_t *)0x60000300;
extern volatile uint32_t * IO_BASE;  // = (volatile uint32_t *)0x60000000;
#define HWREG(BASE, OFF)       BASE[OFF>>2]

//Sets clock frequency, PLL and initializes BSS.
void romlib_init();



#endif

