.global delay4clk
.align 4
delay4clk:
	addi.n a2, a2, -1
	bnez a2, delay4clk
	ret.n

//Look-alike function to rom_i2c_writeReg
.global pico_i2c_writereg_asm
.align 4
pico_i2c_writereg_asm:
	slli	a8, a5, 16
	slli	a7, a4, 8
	movi	a9, 0x60000a00
	or	a7, a7, a8
	movi	a8, 0x01000000
	or	a7, a2, a7
	or	a7, a7, a8
	slli	a8, a3, 2
	add.n	a2, a8, a9
	memw
	s32i	a7, a2, 0x300
	memw
	l32i	a6, a2, 0x300
	bbci	a6, 25, .term_pico_writereg
.reloop_pico_writereg:
	memw
	l32i	a9, a2, 0x300
	bbsi	a9, 25, .reloop_pico_writereg
.term_pico_writereg:
	ret.n


#ifdef PICO66

#else

#endif

