#include "nosdki2s.h"
#include "pin_mux_register.h"


//Change these if you need!
#define WS_I2S_BCK 1	
#define WS_I2S_DIV 1


volatile uint32_t * DR_REG_I2S_BASEL = (volatile uint32_t*)0x60000e00;
volatile uint32_t * DR_REG_SLC_BASEL = (volatile uint32_t*)0x60000B00;

static unsigned int i2sData[2][16];
static struct sdio_queue i2sBufDesc[2] = {
	{ .owner = 1, .eof = 1, .sub_sof = 0, .datalen = 16*4,  .blocksize = 16*4, .buf_ptr = (uint32_t)&i2sData[0], .next_link_ptr = (uint32_t)&i2sBufDesc[1] },
	{ .owner = 1, .eof = 1, .sub_sof = 0, .datalen = 16*4,  .blocksize = 16*4, .buf_ptr = (uint32_t)&i2sData[1], .next_link_ptr = (uint32_t)&i2sBufDesc[0] },
};

volatile int isrs = 0;

LOCAL void slc_isr(void) {
	WRITE_PERI_REG(SLC_INT_CLR, 0xffffffff);//slc_intr_status);

	//This is a little wacky.  This function actually gets called twice.
	//Once for the initial transfer, but by the time we tell it to stop
	//The other zero transfer's already begun.
//	SET_PERI_REG_MASK(SLC_RX_LINK, SLC_RXLINK_STOP);

	isrs++;
}

#define USE_2812_INTERRUPTS 1


void InitI2S()
{
	int i;

	//Reset DMA
//Experimentally determined: Not needed
//	SET_PERI_REG_MASK(SLC_CONF0, SLC_RXLINK_RST);//|SLC_TXLINK_RST);
//	CLEAR_PERI_REG_MASK(SLC_CONF0, SLC_RXLINK_RST);//|SLC_TXLINK_RST);

	//Clear DMA int flags
		//SET_PERI_REG_MASK(SLC_INT_CLR,  0xffffffff);
		//CLEAR_PERI_REG_MASK(SLC_INT_CLR,  0xffffffff);
	DR_REG_SLC_BASEL[4] = 0;

	//Enable and configure DMA
		//CLEAR_PERI_REG_MASK(SLC_CONF0, (SLC_MODE<<SLC_MODE_S));
		//SET_PERI_REG_MASK(SLC_CONF0,(1<<SLC_MODE_S));
		//SET_PERI_REG_MASK(SLC_RX_DSCR_CONF,SLC_INFOR_NO_REPLACE|SLC_TOKEN_NO_REPLACE);
		//CLEAR_PERI_REG_MASK(SLC_RX_DSCR_CONF, SLC_RX_FILL_EN|SLC_RX_EOF_MODE | SLC_RX_FILL_MODE);

	SLC_CONF0L = (1<<SLC_MODE_S);
	SLC_RX_DSCR_CONFL = SLC_INFOR_NO_REPLACE|SLC_TOKEN_NO_REPLACE;

/*	for( i = 0; i < 2; i++ )
	{
		i2sBufDesc[i].owner = 1;
		i2sBufDesc[i].eof = 1;
		i2sBufDesc[i].sub_sof = 0;
		i2sBufDesc[i].datalen = 16*4;  //Size (in bytes)
		i2sBufDesc[i].blocksize = 16*4; //Size (in bytes)
		i2sBufDesc[i].buf_ptr=(uint32_t)&i2sData[i];
		i2sBufDesc[i].unused=0;
		i2sBufDesc[i].next_link_ptr=(uint32_t)&i2sBufDesc[(i+1)&1]; //At the end, just redirect the DMA to the zero buffer.
	}*/

	for( i = 0; i < 16; i++ )
	{
		i2sData[0][i] = 0x00000000;
		i2sData[1][i] = 0xffffffff;
	}

		//CLEAR_PERI_REG_MASK(SLC_RX_LINK,SLC_RXLINK_DESCADDR_MASK);
		//SET_PERI_REG_MASK(SLC_RX_LINK, ((uint32)&i2sBufDesc[0]) & SLC_RXLINK_DESCADDR_MASK);
	SLC_RX_LINKL = ((uint32)&i2sBufDesc[0]) & SLC_RXLINK_DESCADDR_MASK;

#if USE_2812_INTERRUPTS

	//Attach the DMA interrupt
	ets_isr_attach(ETS_SLC_INUM, slc_isr);
	//Enable DMA operation intr
		//WRITE_PERI_REG(SLC_INT_ENA,  SLC_RX_EOF_INT_ENA);
	SLC_INT_ENAL = SLC_RX_EOF_INT_ENA; //Select the interrupt.

	//clear any interrupt flags that are set
		//WRITE_PERI_REG(SLC_INT_CLR, 0xffffffff);
	SLC_INT_CLRL = 0xffffffff;
	///enable DMA intr in cpu
	ets_isr_unmask(1<<ETS_SLC_INUM);

#endif

	//Start transmission Don't do this here!
//	SET_PERI_REG_MASK(SLC_TX_LINK, SLC_TXLINK_START);
	//	SET_PERI_REG_MASK(SLC_RX_LINK, SLC_RXLINK_START);
//	SLC_RX_LINKL |= SLC_RXLINK_START;
	//SLC_RX_LINKL = SLC_RXLINK_STOP;

//----

	//Init pins to i2s functions
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_I2SO_DATA);
//	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_I2SO_WS);
//	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_I2SO_BCK);

	//Enable clock to i2s subsystem
	//i2c_writeReg_Mask_def(i2c_bbpll, i2c_bbpll_en_audio_clock_out, 1);
	//i2c_writeReg_Mask_def(i2c_bbpll, i2c_bbpll_en_audio_clock_out, 1);

	//rom_i2c_writeReg(i2c_bbpll, 0x02000093); //Interesting: this crashes the proc. (so does 0x0100000...)
	//rom_i2c_writeReg(i2c_bbpll, 0x0000400); //Interesting: sets output to 0x19 (so does 0x200)
	//rom_i2c_writeReg(i2c_bbpll, 0x0000400); //Interesting: this crashes the proc.
	//rom_i2c_writeReg(i2c_bbpll, 0x0000204); //Interesting: this crashes the proc.

//  rom_i2c_writeReg_Mask(i2c_bbpll, 4,  4,  7,  7,  1);

	//From 13 to 93.
	pico_i2c_writereg(103,4,4,0x93);

/*
#define i2c_bbpll_en_audio_clock_out            4
#define i2c_bbpll_en_audio_clock_out_msb        7
#define i2c_bbpll_en_audio_clock_out_lsb        7
*/

	printf( "READREG: %08x\n", rom_i2c_readReg( i2c_bbpll, 4, 4 ) );

	//Reset I2S subsystem
	//CLEAR_PERI_REG_MASK(I2SCONF,I2S_I2S_RESET_MASK);  //Not needed.
	//SET_PERI_REG_MASK(I2SCONF,I2S_I2S_RESET_MASK);    //Not needed.
	//CLEAR_PERI_REG_MASK(I2SCONF,I2S_I2S_RESET_MASK);  //Not needed.
	I2SCONFL = I2S_I2S_RESET_MASK;
	I2SCONFL = 0;

	//Select 16bits per channel (FIFO_MOD=0), no DMA access (FIFO only)
		//CLEAR_PERI_REG_MASK(I2S_FIFO_CONF, I2S_I2S_DSCR_EN|(I2S_I2S_RX_FIFO_MOD<<I2S_I2S_RX_FIFO_MOD_S)|(I2S_I2S_TX_FIFO_MOD<<I2S_I2S_TX_FIFO_MOD_S));
	//Enable DMA in i2s subsystem
		//SET_PERI_REG_MASK(I2S_FIFO_CONF, I2S_I2S_DSCR_EN);
	I2S_FIFO_CONFL = I2S_I2S_DSCR_EN;

	//tx/rx binaureal
//	CLEAR_PERI_REG_MASK(I2SCONF_CHAN, (I2S_TX_CHAN_MOD<<I2S_TX_CHAN_MOD_S)|(I2S_RX_CHAN_MOD<<I2S_RX_CHAN_MOD_S));

#if USE_2812_INTERRUPTS

	//Clear int
		//SET_PERI_REG_MASK(I2SINT_CLR,  
		//		I2S_I2S_RX_WFULL_INT_CLR|I2S_I2S_PUT_DATA_INT_CLR|I2S_I2S_TAKE_DATA_INT_CLR);
		//CLEAR_PERI_REG_MASK(I2SINT_CLR, 
		//	I2S_I2S_RX_WFULL_INT_CLR|I2S_I2S_PUT_DATA_INT_CLR|I2S_I2S_TAKE_DATA_INT_CLR);
	I2SINT_CLRL = 0;
#endif
	//trans master&rece slave,MSB shift,right_first,msb right
	//		CLEAR_PERI_REG_MASK(I2SCONF, I2S_TRANS_SLAVE_MOD|
	//							(I2S_BITS_MOD<<I2S_BITS_MOD_S)|
	//							(I2S_BCK_DIV_NUM <<I2S_BCK_DIV_NUM_S)|
	//							(I2S_CLKM_DIV_NUM<<I2S_CLKM_DIV_NUM_S));
	//		SET_PERI_REG_MASK(I2SCONF, I2S_RIGHT_FIRST|I2S_MSB_RIGHT|I2S_RECE_SLAVE_MOD|
	//							I2S_RECE_MSB_SHIFT|I2S_TRANS_MSB_SHIFT|
	//							(((WS_I2S_BCK)&I2S_BCK_DIV_NUM )<<I2S_BCK_DIV_NUM_S)|
	//							(((WS_I2S_DIV)&I2S_CLKM_DIV_NUM)<<I2S_CLKM_DIV_NUM_S));
	I2SCONFL = I2S_RIGHT_FIRST|I2S_MSB_RIGHT|I2S_RECE_SLAVE_MOD|
						I2S_RECE_MSB_SHIFT|I2S_TRANS_MSB_SHIFT|
						(((WS_I2S_BCK)&I2S_BCK_DIV_NUM )<<I2S_BCK_DIV_NUM_S)|
						(((WS_I2S_DIV)&I2S_CLKM_DIV_NUM)<<I2S_CLKM_DIV_NUM_S);


	//No idea if ints are needed...
	//clear int
		//SET_PERI_REG_MASK(I2SINT_CLR, I2S_I2S_RX_WFULL_INT_CLR|I2S_I2S_PUT_DATA_INT_CLR|I2S_I2S_TAKE_DATA_INT_CLR);
		//CLEAR_PERI_REG_MASK(I2SINT_CLR, I2S_I2S_RX_WFULL_INT_CLR|I2S_I2S_PUT_DATA_INT_CLR|I2S_I2S_TAKE_DATA_INT_CLR);
	//enable int
	//SET_PERI_REG_MASK(I2SINT_ENA,  I2S_I2S_RX_REMPTY_INT_ENA|I2S_I2S_RX_TAKE_DATA_INT_ENA);
	I2SINT_ENAL = I2S_I2S_RX_REMPTY_INT_ENA|I2S_I2S_RX_TAKE_DATA_INT_ENA;

	//Start transmission
	//SET_PERI_REG_MASK(I2SCONF,I2S_I2S_TX_START);
	I2SCONFL |= I2S_I2S_TX_START;

}

void SendI2S()
{
	SLC_RX_LINKL = SLC_RXLINK_STOP;
	SLC_RX_LINKL = (((uint32)&i2sBufDesc[0]) & SLC_RXLINK_DESCADDR_MASK) | SLC_RXLINK_START;

/*	SET_PERI_REG_MASK(SLC_RX_LINK, SLC_RXLINK_STOP);
	CLEAR_PERI_REG_MASK(SLC_RX_LINK,SLC_RXLINK_DESCADDR_MASK);
	SET_PERI_REG_MASK(SLC_RX_LINK, ((uint32)&i2sBufDesc[0]) & SLC_RXLINK_DESCADDR_MASK);
	SET_PERI_REG_MASK(SLC_RX_LINK, SLC_RXLINK_START);*/
}

