.global delay4clk
.align 4
delay4clk:
	_addi.n a2, a2, -1
	bnez a2, delay4clk
	_ret.n