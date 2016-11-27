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

