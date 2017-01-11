.global delay4clk
.align 4
delay4clk:
	_addi.n a2, a2, -1
	bnez a2, delay4clk
	_ret.n

//Look-alike function to rom_i2c_writeReg
#if MAIN_MHZ==52 || MAIN_MHZ==104

//No pico_i2c_writereg_asm isneeded if no PLL used.

#else

//This is similar to the i2c_writereg found in the ESP8266's ROM, however, we frontload a lot of the mathematics into the compiler.

.global pico_i2c_writereg_asm
.align 4
pico_i2c_writereg_asm:
	_s32i.n	a3, a2, 0
	_memw
	_l32i.n	a3, a2, 0
	bbci	a3, 25, .term_pico_writereg
.reloop_pico_writereg:
	_memw
	_l32i.n	a3, a2, 0
	bbsi	a3, 25, .reloop_pico_writereg
.term_pico_writereg:
	_ret.n

#endif


/*

//An example function

.global myfunc

myfunc:
	addi a1, a1, -68
	s32i.n a0, a1, 0   // Working reg
	s32i.n a2, a1, 4   // Running byte
	s32i.n a3, a1, 8   // Running CRC
	s32i.n a4, a1, 12  // Anding mask

	movi a0, 320000
	movi a2, PIN_BASE
	l32i a4, a2, 0
keep_going:
	//At 80 MHz, this instruction takes 12 cycles
	//At 160 MHz, this instruction takes 12 cycles
	//At 320 MHz, this instruction takes 16 cycles
	//At 346.6666 MHz, this is 16 cycles.  -> We have 28.8888 cycles
	
	//l32i a3, a4, 0
	l8ui a3, a2, 0  //1 cycle!!!
	addi a0, a0, -1
	bnez a0, keep_going


	l32i.n a0, a1, 0
	l32i.n a2, a1, 4
	l32i.n a3, a1, 8
	l32i.n a4, a1, 12
	addi a1, a1, 68
	ret
*/
