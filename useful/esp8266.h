/******************************************************************************
 * FileName: esp8266.h
 * Description: esp8266 map, i/o regs
 * Alternate SDK
 * Author: PV`
 * (c) PV` 2015
*******************************************************************************/
#ifndef ESP8266_H
#define ESP8266_H

//#include "hw/eagle_soc.h"

#define	Q_CLK_FREQ	26000000ul	// unit: Hz, 40000000 Hz

#ifndef ICACHE_FLASH_ATTR
#define ICACHE_FLASH_ATTR	__attribute__((section(".irom0.text")))
#endif
#ifndef ICACHE_RODATA_ATTR
#define ICACHE_RODATA_ATTR	__attribute__((aligned(4), section(".irom.text")))
#endif
#ifndef DATA_IRAM_ATTR
#define DATA_IRAM_ATTR __attribute__((aligned(4), section(".iram.data")))
#endif

extern volatile uint32 dport_[64];		// 0x3ff00000
extern volatile uint32 io2_regs_[1536]; // 0x3ff20000
extern volatile uint32 uart0_[64];		// 0x60000000
extern volatile uint32 spi1_[64];		// 0x60000100
extern volatile uint32 spi0_[64];		// 0x60000200
extern volatile uint32 gpio_[64];		// 0x60000300
extern volatile uint32 timer_[64];		// 0x60000600
extern volatile uint32 rtc_[64];		// 0x60000700
extern volatile uint32 iomux_[64];		// 0x60000800
extern volatile uint32 wdt_[64];		// 0x60000900
extern volatile uint32 scl_[64];		// 0x60000B00
extern volatile uint32 sar_[64];		// 0x60000D00
extern volatile uint32 i2s_[64];		// 0x60000E00
extern volatile uint32 uart1_[64];		// 0x60000F00
extern volatile uint32 rtc_ram_[64];	// 0x60001000
extern volatile uint32 rtc_mem_[192];	// 0x60001100
extern volatile uint32 io4_regs_[384];	// 0x60009800

#define MEMW() __asm__ __volatile__("memw" : : : "memory") // синхронизация и ожидание отработки fifo-write на шинах CPU

/* dport (io1) section */
#define DPORT_BASE		dport_		// 0x3ff00000

/* io2 section */
#define IO2_BASE		io2_regs_	// 0x3ff20000	// Size: 6144 bytes

/* io3 section */
#define UART0_BASE		uart0_		// 0x60000000
#define SPI1_BASE		spi1_		// 0x60000100
#define SPI0_BASE		spi0_		// 0x60000200
#define GPIO_BASE		gpio_		// 0x60000300
#define TIMER_BASE		timer_		// 0x60000600
#define RTC_BASE		rtc_		// 0x60000700
#define IOMUX_BASE		iomux_		// 0x60000800
#define WDT_BASE		wdt_		// 0x60000900
#define SCL_BASE		scl_		// 0x60000B00
#define SAR_BASE		sar_		// 0x60000D00
#define I2S_BASE		i2s_		// 0x60000E00
#define UART1_BASE		uart1_		// 0x60000F00
#define RTC_RAM_BASE	rtc_ram_	// 0x60001000	// Size: 1024 bytes
#define RTC_MEM_BASE	rtc_mem_	// 0x60001100

/* io4 section */
#define IO4_BASE		io4_regs_	// 0x60009800	// Size: 1536 bytes

/* RAM */
#define RAM_BASE		0x3FFE8000
#define RAM_SIZE		0x00018000	// Size: 98304 bytes
#define RAM_BIOS_DATA_BASE 	0x3FFFC000 // ROM-BIOS system data RAM. Size: 16384 bytes.
/* IRAM */
#define IRAM_BASE		0x40100000
#define IRAM_SIZE		0x00008000	// Size: 32768 bytes
/* FLASH */
#define FLASH_BASE		0x40200000
#define FLASH_MIN_SIZE	0x00080000
#define FLASH_MAX_SIZE	0x01000000

/* interrupt related */
#define ETS_SLC_INUM		1
#define ETS_SPI_INUM        2
#define ETS_GPIO_INUM       4
#define ETS_UART_INUM       5
#define ETS_MAX_INUM        6
#define ETS_SOFT_INUM       7
#define ETS_WDT_INUM        8
#define ETS_FRC_TIMER0_INUM 9

/* IO helper macro */
//#define HWREG(BASE, OFF) 	(((volatile unsigned int *)(BASE))[OFF>>2])
//#define HWREG(BASE, OFF) (*((volatile uint32_t *)((BASE)+(OFF))))
#define HWREG(BASE, OFF) 	BASE[OFF>>2]
#define IOREG(ADDR) 		(*((volatile unsigned int *)(ADDR)))

/* DPORT regs */
/* INTC_EDGE_EN: 0x3FF00004
	bit0: WDT edge int enable
	bit1: TIMER0 edge int enable	*/
#define INTC_EDGE_EN	dport_[1]

/* DPORT_SPI_READY: 0x3FF0000C
	bit9: use wait SPI idle */
#define DPORT_SPI_IDLE	dport_[3]

/* CLK_PRE_PORT: 0x3FF00014
	bit0: =1 CPU 160 MHz, = 0 CPU 80 MHz */
#define CLK_PRE_PORT	dport_[5]
#define Select_CLKx2() CLK_PRE_PORT |= 1
#define Select_CLKx1() CLK_PRE_PORT = (CLK_PRE_PORT >> 1) << 1

// 0x3FF00018
//#define DPORT_OFF18		dport_[6] // use clockgate_watchdog(flg) { if(flg) 0x3FF00018 &= 0x77; else 0x3FF00018 |= 8; }

/* 0x3ff00020 is isr flag register, (ESP8266 SPI Module User Guide)
  bit4 is for spi isr,
  bit7 is for hspi isr,
  bit9 is for i2s isr */
//#define DPORT_OFF20		dport_[8]

/* 0x3ff00024
 bit7 16k IRAM base 0x40108000 = SPI cache flash  
 bit8 16k IRAM base 0x4010C000 = SPI cache flash   */
//#define DPORT_OFF24		dport_[9]

/* PERI_IO_SWAP: 0x3FF00028
	bit0: swap two uart
	bit1: swap two spi
	bit2: swap uart0 pins (u0rxd <-> u0cts), (u0txd <-> u0rts)
	bit3: swap uart1 pins (u1rxd <-> u1cts), (u1txd <-> u1rts)
	bit5: hspi is with the higher prior
	bit6: two spi masters on hspi
	bit7: two spi masters on cspi (reg_cspi_overlap) */
#define PERI_IO_SWAP	dport_[10] // HOST_INF_SEL (eagle_soc.h)

/* SLC_TX_DESC_DEBUG_REG: 0x3ff0002c  
	[15:0] set to 0xcccc */
#define SLC_TX_DESC_DEBUG_REG dport_[11] 

/* OTP:0x3FF00050 registers */
#define OTP_MAC0	dport_[20]
#define OTP_MAC1	dport_[21]
#define OTP_CHIPID	dport_[22]

/* REG_UART_INTS: 0x3FF20020
 reg 0x3ff20020, bit2, bit0 represents uart1 and uart0 respectively
uart0 and uart1 intr combine together, when interrupt occur */
#define REG_UART_INTS io2_regs_[8]

/* SPIx controller registers index */
#define IDX_SPI_CMD			0
#define IDX_SPI_ADDR		1
#define IDX_SPI_CTRL		2
#define IDX_SPI_CTRL1		3
#define IDX_SPI_RD_STATUS	4
#define IDX_SPI_CTRL2		5
#define IDX_SPI_CLOCK		6
#define IDX_SPI_USER		7
#define IDX_SPI_USER1		8
#define IDX_SPI_USER2		9
#define IDX_SPI_WR_STATUS	10
#define IDX_SPI_PIN			11
#define IDX_SPI_SLAVE		12
#define IDX_SPI_SLAVE1		13
#define IDX_SPI_SLAVE2		14
#define IDX_SPI_SLAVE3		15
#define IDX_SPI_FIFO		16
#define IDX_SPI_W0			16
#define IDX_SPI_W1			17
#define IDX_SPI_W2			18
#define IDX_SPI_W3			19
#define IDX_SPI_W4			20
#define IDX_SPI_W5			21
#define IDX_SPI_W6			22
#define IDX_SPI_W7			23
#define IDX_SPI_W8			24
#define IDX_SPI_W9			25
#define IDX_SPI_W10			26
#define IDX_SPI_W11			27
#define IDX_SPI_W12			28
#define IDX_SPI_W13			29
#define IDX_SPI_W14			30
#define IDX_SPI_W15			31
#define IDX_SPI_EXT0		60
#define IDX_SPI_EXT1		61
#define IDX_SPI_EXT2		62
#define IDX_SPI_EXT3		63

/* SPI0:0x60000200 controller registers */
#define SPI0_CMD		spi0_[IDX_SPI_CMD]
#define SPI0_ADDR		spi0_[IDX_SPI_ADDR]
#define SPI0_CTRL		spi0_[IDX_SPI_CTRL]
#define SPI0_CTRL1		spi0_[IDX_SPI_CTRL1]
#define SPI0_RD_STATUS	spi0_[IDX_SPI_RD_STATUS]
#define SPI0_CTRL2		spi0_[IDX_SPI_CTRL2]
#define SPI0_CLOCK		spi0_[IDX_SPI_CLOCK]
#define SPI0_USER		spi0_[IDX_SPI_USER]
#define SPI0_USER1		spi0_[IDX_SPI_USER1]
#define SPI0_USER2		spi0_[IDX_SPI_USER2]
#define SPI0_WR_STATUS	spi0_[IDX_SPI_WR_STATUS]
#define SPI0_PIN		spi0_[IDX_SPI_PIN]
#define SPI0_SLAVE		spi0_[IDX_SPI_SLAVE]
#define SPI0_SLAVE1		spi0_[IDX_SPI_SLAVE1]
#define SPI0_SLAVE2		spi0_[IDX_SPI_SLAVE2]
#define SPI0_SLAVE3		spi0_[IDX_SPI_SLAVE3]
#define SPI0_W0			spi0_[IDX_SPI_W0]
#define SPI0_W1			spi0_[IDX_SPI_W1]
#define SPI0_W2			spi0_[IDX_SPI_W2]
#define SPI0_W3			spi0_[IDX_SPI_W3]
#define SPI0_W4			spi0_[IDX_SPI_W4]
#define SPI0_W5			spi0_[IDX_SPI_W5]
#define SPI0_W6			spi0_[IDX_SPI_W6]
#define SPI0_W7			spi0_[IDX_SPI_W7]
#define SPI0_W8			spi0_[IDX_SPI_W8]
#define SPI0_W9			spi0_[IDX_SPI_W9]
#define SPI0_W10		spi0_[IDX_SPI_W10]
#define SPI0_W11		spi0_[IDX_SPI_W11]
#define SPI0_W12		spi0_[IDX_SPI_W12]
#define SPI0_W13		spi0_[IDX_SPI_W13]
#define SPI0_W14		spi0_[IDX_SPI_W14]
#define SPI0_W15		spi0_[IDX_SPI_W15]
#define SPI0_EXT0		spi0_[IDX_SPI_EXT0]
#define SPI0_EXT1		spi0_[IDX_SPI_EXT1]
#define SPI0_EXT2		spi0_[IDX_SPI_EXT2]
#define SPI0_EXT3		spi0_[IDX_SPI_EXT3]

/* SPI1:0x60000100 controller registers */
#define SPI1_CMD		spi1_[IDX_SPI_CMD]
#define SPI1_ADDR		spi1_[IDX_SPI_ADDR]
#define SPI1_CTRL		spi1_[IDX_SPI_CTRL]
#define SPI1_CTRL1		spi1_[IDX_SPI_CTRL1]
#define SPI1_RD_STATUS	spi1_[IDX_SPI_RD_STATUS]
#define SPI1_CTRL2		spi1_[IDX_SPI_CTRL2]
#define SPI1_CLOCK		spi1_[IDX_SPI_CLOCK]
#define SPI1_USER		spi1_[IDX_SPI_USER]
#define SPI1_USER1		spi1_[IDX_SPI_USER1]
#define SPI1_USER2		spi1_[IDX_SPI_USER2]
#define SPI1_WR_STATUS	spi1_[IDX_SPI_WR_STATUS]
#define SPI1_PIN		spi1_[IDX_SPI_PIN]
#define SPI1_SLAVE		spi1_[IDX_SPI_SLAVE]
#define SPI1_SLAVE1		spi1_[IDX_SPI_SLAVE1]
#define SPI1_SLAVE2		spi1_[IDX_SPI_SLAVE2]
#define SPI1_SLAVE3		spi1_[IDX_SPI_SLAVE3]
#define SPI1_W0			spi1_[IDX_SPI_W0]
#define SPI1_W1			spi1_[IDX_SPI_W1]
#define SPI1_W2			spi1_[IDX_SPI_W2]
#define SPI1_W3			spi1_[IDX_SPI_W3]
#define SPI1_W4			spi1_[IDX_SPI_W4]
#define SPI1_W5			spi1_[IDX_SPI_W5]
#define SPI1_W6			spi1_[IDX_SPI_W6]
#define SPI1_W7			spi1_[IDX_SPI_W7]
#define SPI1_W8			spi1_[IDX_SPI_W8]
#define SPI1_W9			spi1_[IDX_SPI_W9]
#define SPI1_W10		spi1_[IDX_SPI_W10]
#define SPI1_W11		spi1_[IDX_SPI_W11]
#define SPI1_W12		spi1_[IDX_SPI_W12]
#define SPI1_W13		spi1_[IDX_SPI_W13]
#define SPI1_W14		spi1_[IDX_SPI_W14]
#define SPI1_W15		spi1_[IDX_SPI_W15]
#define SPI1_EXT0		spi1_[IDX_SPI_EXT0]
#define SPI1_EXT1		spi1_[IDX_SPI_EXT1]
#define SPI1_EXT2		spi1_[IDX_SPI_EXT2]
#define SPI1_EXT3		spi1_[IDX_SPI_EXT3]

/* UARTx controller registers index */
#define IDX_UART_FIFO		0
#define IDX_UART_INT_RAW	1
#define IDX_UART_INT_ST		2
#define IDX_UART_INT_ENA	3
#define IDX_UART_INT_CLR	4
#define IDX_UART_CLKDIV		5
#define IDX_UART_AUTOBAUD	6
#define IDX_UART_STATUS		7
#define IDX_UART_CONF0		8
#define IDX_UART_CONF1		9
#define IDX_UART_LOWPULSE	10
#define IDX_UART_HIGHPULSE	11
#define IDX_UART_PULSE_NUM	12
#define IDX_UART_DATE		30
#define IDX_UART_ID			21

/* UART0:0x60000000 controller registers */
#define UART0_FIFO		uart0_[IDX_UART_FIFO]
#define UART0_INT_RAW	uart0_[IDX_UART_INT_RAW]
#define UART0_INT_ST	uart0_[IDX_UART_INT_ST]
#define UART0_INT_ENA	uart0_[IDX_UART_INT_ENA]
#define UART0_INT_CLR	uart0_[IDX_UART_INT_CLR]
#define UART0_CLKDIV	uart0_[IDX_UART_CLKDIV]
#define UART0_AUTOBAUD	uart0_[IDX_UART_AUTOBAUD]
#define UART0_STATUS	uart0_[IDX_UART_STATUS]
#define UART0_CONF0		uart0_[IDX_UART_CONF0]
#define UART0_CONF1		uart0_[IDX_UART_CONF1]
#define UART0_LOWPULSE	uart0_[IDX_UART_LOWPULSE]
#define UART0_HIGHPULSE	uart0_[IDX_UART_HIGHPULSE]
#define UART0_PULSE_NUM	uart0_[IDX_UART_PULSE_NUM]
#define UART0_DATE		uart0_[IDX_UART_DATE]
#define UART0_ID		uart0_[IDX_UART_ID]

/* UART1:0x60000F00 controller registers */
#define UART1_FIFO		uart1_[IDX_UART_FIFO]
#define UART1_INT_RAW	uart1_[IDX_UART_INT_RAW]
#define UART1_INT_ST	uart1_[IDX_UART_INT_ST]
#define UART1_INT_ENA	uart1_[IDX_UART_INT_ENA]
#define UART1_INT_CLR	uart1_[IDX_UART_INT_CLR]
#define UART1_CLKDIV	uart1_[IDX_UART_CLKDIV]
#define UART1_AUTOBAUD	uart1_[IDX_UART_AUTOBAUD]
#define UART1_STATUS	uart1_[IDX_UART_STATUS]
#define UART1_CONF0		uart1_[IDX_UART_CONF0]
#define UART1_CONF1		uart1_[IDX_UART_CONF1]
#define UART1_LOWPULSE	uart1_[IDX_UART_LOWPULSE]
#define UART1_HIGHPULSE	uart1_[IDX_UART_HIGHPULSE]
#define UART1_PULSE_NUM	uart1_[IDX_UART_PULSE_NUM]
#define UART1_DATE		uart1_[IDX_UART_DATE]
#define UART1_ID		uart1_[IDX_UART_ID]

/* TIMER0_LOAD: 0x60000600 controller registers */
#define TIMER0_LOAD		timer_[0]	// the load value into the counter
#define TIMER0_LOAD_DATA_MASK	0x007fffff
/* TIMER0_COUNT: 0x60000600 */
#define TIMER0_COUNT	timer_[1]	// the current value of the counter. It is a decreasing counter.
#define TIMER0_COUNT_DATA_MASK	0x007fffff
/* 	TIMER0_CTRL: 0x60000608
	bit[8]: the status of the interrupt, when the count is dereased to zero
	bit[7]: timer enable
	bit[6]: automatically reload, when the counter  is equal to zero
	bit[3:2]: prescale-divider, 0: divided by 1, 1: divided by 16, 2 or 3: divided by 256
	bit[0]: interrupt type, 0:edge, 1:level */
#define TIMER0_CTRL		timer_[2]
#define TIMER0_CTRL_DATA_MASK 0x0FF
#define TM_ENABLE_TIMER		BIT(7)
#define TM_AUTO_RELOAD_CNT	BIT(6)
typedef enum {
    TM_DIVDED_BY_1 = 0,
	TM_DIVDED_BY_16 = 4,
	TM_DIVDED_BY_256 = 8,
} TIMER_PREDIVED_MODE;
typedef enum {
    TM_EDGE_INT	= 0,
    TM_LEVEL_INT = 1
} TIMER_INT_MODE;
/* TIMER0_INT:  0x6000060C */
#define TIMER0_INT		timer_[3]
#define TIMER0_INT_CLR_MASK 1

/* TIMER1_LOAD: 0x60000620 */
#define TIMER1_LOAD		timer_[8]
#define TIMER1_LOAD_DATA_MASK	0xffffffff
/* TIMER1_COUNT: 0x60000624 */
#define TIMER1_COUNT	timer_[9]
/* 	TIMER1_CTRL: 0x60000628
    bit[8]: the status of the interrupt, when the count is dereased to zero
	bit[7]: timer enable
	bit[6]: automatically reload, when the counter  is equal to zero
	bit[3:2]: prescale-divider, 0: divided by 1, 1: divided by 16, 2 or 3: divided by 256
	bit[0]: interrupt type, 0:edge, 1:level */
#define TIMER1_CTRL		timer_[10]
#define TIMER1_CTRL_DATA_MASK 0x0FF
/* TIMER1_INT: 0x6000062C */
#define TIMER1_INT		timer_[11]
#define TIMER0_INT_CLR_MASK 1
/* TIMER1_ALARM: 0x60000630
	bit[0]: write to clear the status of the interrupt, if the interrupt type is "level" */
#define TIMER1_ALARM	timer_[12] // the alarm value for the counter
#define TIMER1_ALARM_DATA_MASK	0xffffffff
/*	Returns	the	current	time	according	to	the	timer	timer. */
#define	NOW()			TIMER1_COUNT // FRC2_COUNT


/* WDT:0x60000900 registers */
/* WDT_CTRL:0x60000900 register */
#define WDT_CTRL		wdt_[0]
/* WDT_REG1:0x60000904 register */
#define WDT_REG1		wdt_[1]
/* WDT_REG2:0x60000908 register */
#define WDT_REG2		wdt_[2]
/* WDT_FEED:0x60000914 register */
#define WDT_FEED		wdt_[5]
#define WDT_FEED_MAGIC	0x73

/* I2S:0x60000E00 registers */
/* I2STXFIFO:0x60000E00 */
#define I2STXFIFO	i2s_[0]
/* I2SRXFIFO:0x60000E04 */
#define I2SRXFIFO	i2s_[1]
/* I2SCONF:0x60000E08 */
#define I2SCONF		i2s_[2]
/* I2SINT_RAW:0x60000E0C */
#define I2SINT_RAW	i2s_[3]
/* I2SINT_ST:0x60000E10 */
#define I2SINT_ST	i2s_[4]
/* I2SINT_ENA:0x60000E14 */
#define I2SINT_ENA	i2s_[5]
/* I2SINT_CLR:0x60000E18 */
#define I2SINT_CLR	i2s_[6]
/* I2STIMING:0x60000E1C */
#define I2STIMING	i2s_[7]
/* I2S_FIFO_CONF:0x60000E20 */
#define I2S_FIFO_CONF	i2s_[8]
/* I2SRXEOF_NUM:0x60000E24 */
#define I2SRXEOF_NUM	i2s_[9]
/* I2SCONF_SIGLE_DATA:0x60000E28 */
#define I2SCONF_SIGLE_DATA	i2s_[10]
/* I2SCONF_CHAN:0x60000E2C */
#define I2SCONF_CHAN	i2s_[11]

#define IDX_GPIO_PIN	10
/* GPIO:0x60000300 registers */
#define	GPIO_OUT			gpio_[0]
#define	GPIO_OUT_W1TS		gpio_[1]
#define	GPIO_OUT_W1TC		gpio_[2]
#define	GPIO_ENABLE			gpio_[3]
#define	GPIO_ENABLE_W1TS	gpio_[4]
#define	GPIO_ENABLE_W1TC	gpio_[5]
#define	GPIO_IN				gpio_[6]
#define	GPIO_STATUS			gpio_[7]
#define	GPIO_STATUS_W1TS	gpio_[8]
#define	GPIO_STATUS_W1TC	gpio_[9]
#define GPIO_PIN0			gpio_[IDX_GPIO_PIN]
#define GPIO_PIN1			gpio_[IDX_GPIO_PIN+1]
#define GPIO_PIN2			gpio_[IDX_GPIO_PIN+2]
#define GPIO_PIN3			gpio_[IDX_GPIO_PIN+3]
#define GPIO_PIN4			gpio_[IDX_GPIO_PIN+4]
#define GPIO_PIN5			gpio_[IDX_GPIO_PIN+5]
#define GPIO_PIN6			gpio_[IDX_GPIO_PIN+6]
#define GPIO_PIN7			gpio_[IDX_GPIO_PIN+7]
#define GPIO_PIN8			gpio_[IDX_GPIO_PIN+8]
#define GPIO_PIN9			gpio_[IDX_GPIO_PIN+9]
#define GPIO_PIN10			gpio_[IDX_GPIO_PIN+10]
#define GPIO_PIN11			gpio_[IDX_GPIO_PIN+11]
#define GPIO_PIN12			gpio_[IDX_GPIO_PIN+12]
#define GPIO_PIN13			gpio_[IDX_GPIO_PIN+13]
#define GPIO_PIN14			gpio_[IDX_GPIO_PIN+14]
#define GPIO_PIN15			gpio_[IDX_GPIO_PIN+15]
#define GPIO_SIGMA_DELTA	gpio_[26]
#define RTC_CALIB_SYNC		gpio_[27]
#define RTC_CALIB_VALUE		gpio_[28]

#define GPIO_PIN(n)			gpio_[IDX_GPIO_PIN + n]
#define GPIOx_PIN(n)		gpio_[IDX_GPIO_PIN + n]
#define GPIO_PIN_CONFIG                        0x00000003
#define GPIO_PIN_CONFIG_S                      11
#define GPIO_PIN_WAKEUP_ENABLE                 (1<<10)
#define GPIO_PIN_WAKEUP_ENABLE_S               10
#define GPIO_PIN_INT_TYPE                      0x00000007
#define GPIO_PIN_INT_TYPE_S                    7
#define GPIO_PIN_DRIVER                        BIT2
#define GPIO_PIN_DRIVER_S                      2
#define GPIO_PIN_SOURCE                        1
#define GPIO_PIN_SOURCE_S                      0

//0x60000700
#define IO_RTC_0				rtc_[0] // bit31 - software_reset()
/* RTC_SLP_VAL: 0x60000704 */
#define IO_RTC_SLP_VAL			rtc_[1]	// the target value of RTC_COUNTER for wakeup from light-sleep/deep-sleep
//0x60000708
#define IO_RTC_2				rtc_[2]	// bit21 - rtc_get_reset_reason(), bit20 - rtc_enter_sleep()
//0x6000070C
#define IO_RTC_3				rtc_[3]
//0x60000710
#define IO_RTC_4				rtc_[4] // rtc_enter_sleep() = 0;
//0x60000714
#define IO_RTC_5				rtc_[5]	// bitrtc_get_reset_reason()
//0x60000718
#define IO_RTC_6				rtc_[6]	// bitrtc_get_reset_reason()
/* RTC_SLP_CNT_VAL:	0x6000071C */
#define IO_RTC_SLP_CNT_VAL		rtc_[7]	// the current value of RTC_COUNTER
/* IO_RTC_INT_ST:	0x60000720 */
#define IO_RTC_INT_ST			rtc_[8]
/* IO_RTC_INT_CLR:	0x60000724 */
#define IO_RTC_INT_CLR			rtc_[9]
/* IO_RTC_INT_ENA:	0x60000728 */
#define IO_RTC_INT_ENA			rtc_[10]
/* RTC_SCRATCH0: 0x60000730 */
#define IO_RTC_SCRATCH0			rtc_[12] // the register for software to save some values for watchdog reset
/* RTC_SCRATCH0:	0x60000734 */
#define IO_RTC_SCRATCH1			rtc_[13] // the register for software to save some values for watchdog reset
/* RTC_SCRATCH0:	0x60000738 */
#define IO_RTC_SCRATCH2			rtc_[14] // the register for software to save some values for watchdog reset
/* RTC_SCRATCH0:	0x6000073C */
#define IO_RTC_SCRATCH3			rtc_[15] // the register for software to save some values for watchdog reset
/* RTC_GPIO_OUT:	0x60000768 */
#define IO_RTC_GPIO_OUT			rtc_[26] // used by gpio16
/* RTC_GPIO_ENABLE:	0x60000774 */
#define IO_RTC_GPIO_ENABLE		rtc_[29]
/* RTC_GPIO_IN_DATA:0x6000078C */
#define IO_RTC_GPIO_IN_DATA		rtc_[35]
/* RTC_GPIO_CONF:	0x60000790 */
#define IO_RTC_GPIO_CONF		rtc_[36]
#define RTC_GPI0_CFG			rtc_[37]
#define RTC_GPI1_CFG			rtc_[38]
#define RTC_GPI2_CFG			rtc_[39]
/* RTC_GPIO0_CFG:	0x600007A0 */
#define IO_PAD_XPD_DCDC_CONF	rtc_[40] // GPIO16, XPD_DCDC
#define RTC_GPIO3_CFG			rtc_[40]
#define GPIO_CFG_FUN_BIT0				0
#define GPIO_CFG_FUN_BIT1				1
#define GPIO_CFG_PULLUP_BIT				2 // ?
#define GPIO_CFG_PULLDOWN_BIT			3
#define GPIO_CFG_SLEEP_PULLUP_BIT		4 // ?
#define GPIO_CFG_SLEEP_PULLDOWN_BIT		5
#define GPIO_CFG_FUN_BIT2				8
#define GPIO_CFG_FUN_MASK				((1<<GPIO_CFG_FUN_BIT0)|(1<<GPIO_CFG_FUN_BIT1)|(1<<GPIO_CFG_FUN_BIT2))
/* RTC_GPIO4_CFG:	0x600007A4 */
#define RTC_GPIO4_CFG			rtc_[41]
/* RTC_GPIO5_CFG:	0x600007A8 */
#define RTC_GPIO5_CFG			rtc_[42]

/* IDX_MUX */
#define IDX_MUX0	13
#define IDX_MUX1	6
#define IDX_MUX2	14
#define IDX_MUX3	5
#define IDX_MUX4	15
#define IDX_MUX5	16
#define IDX_MUX6	7
#define IDX_MUX7	8
#define IDX_MUX8	9
#define IDX_MUX9	10
#define IDX_MUX10	11
#define IDX_MUX11	12
#define IDX_MUX12	1
#define IDX_MUX13	2
#define IDX_MUX14	3
#define IDX_MUX15	4

/* GPIOx_MUX:	0x60000800 */
#define GPIO_MUX_CFG	iomux_[0]
#define MUX_CFG_MASK	0xFFF
#define MUX_SPI0_CLK_BIT	8 // =1 при 80 MHz, =0 при 40, 26, 20 MHz на SPI Flash

#define GPIO0_MUX		iomux_[IDX_MUX0]
#define GPIO1_MUX		iomux_[IDX_MUX1]
#define GPIO2_MUX		iomux_[IDX_MUX2]
#define GPIO3_MUX		iomux_[IDX_MUX3]
#define GPIO4_MUX		iomux_[IDX_MUX4]
#define GPIO5_MUX		iomux_[IDX_MUX5]
#define GPIO6_MUX		iomux_[IDX_MUX6]
#define GPIO7_MUX		iomux_[IDX_MUX7]
#define GPIO8_MUX		iomux_[IDX_MUX8]
#define GPIO9_MUX		iomux_[IDX_MUX9]
#define GPIO10_MUX		iomux_[IDX_MUX10]
#define GPIO11_MUX		iomux_[IDX_MUX11]
#define GPIO12_MUX		iomux_[IDX_MUX12]
#define GPIO13_MUX		iomux_[IDX_MUX13]
#define GPIO14_MUX		iomux_[IDX_MUX14]
#define GPIO15_MUX		iomux_[IDX_MUX15]

#define GPIO_MUX_SLEEP_OE_BIT8		0
#define GPIO_MUX_SLEEP_SEL_BIT9		1
#define GPIO_MUX_SLEEP_PULLDOWN_BIT	2 // ?
#define GPIO_MUX_SLEEP_PULLUP_BIT	3
#define GPIO_MUX_FUN_BIT0			4
#define GPIO_MUX_FUN_BIT1			5
#define GPIO_MUX_PULLDOWN_BIT		6 // ?
#define GPIO_MUX_PULLUP_BIT			7
#define GPIO_MUX_FUN_BIT2			8
#define GPIO_MUX_FUN_MASK			((1<<GPIO_MUX_FUN_BIT0)|(1<<GPIO_MUX_FUN_BIT1)|(1<<GPIO_MUX_FUN_BIT2))

#define VAL_MUX_GPIO0_SDK_DEF	(1<<GPIO_MUX_PULLUP_BIT)	// GPIO0, input
#define VAL_MUX_GPIO1_SDK_DEF	0		// UART0, TX0, Outnput
#define VAL_MUX_GPIO2_SDK_DEF	0		// UART1, TX1, Outnput
#define VAL_MUX_GPIO3_SDK_DEF	(1<<GPIO_MUX_PULLUP_BIT)	// UART0, RX0, Input
#define VAL_MUX_GPIO4_SDK_DEF	(1<<GPIO_MUX_PULLUP_BIT)	// GPIO4, input
#define VAL_MUX_GPIO5_SDK_DEF	(1<<GPIO_MUX_PULLUP_BIT)	// GPIO5, input
#define VAL_MUX_GPIO6_SDK_DEF	(1<<GPIO_MUX_FUN_BIT0)	// SPI0CLK
#define VAL_MUX_GPIO7_SDK_DEF	(1<<GPIO_MUX_FUN_BIT0)	// SPI0Q
#define VAL_MUX_GPIO8_SDK_DEF	(1<<GPIO_MUX_FUN_BIT0)	// SPI0D
#define VAL_MUX_GPIO9_SDK_DEF	(1<<GPIO_MUX_FUN_BIT0)	// SPI0HD
#define VAL_MUX_GPIO10_SDK_DEF	(1<<GPIO_MUX_FUN_BIT0)	// SPI0WP
#define VAL_MUX_GPIO11_SDK_DEF	(1<<GPIO_MUX_FUN_BIT0)	// SPI0CS0
#define VAL_MUX_GPIO12_SDK_DEF	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO12, input
#define VAL_MUX_GPIO13_SDK_DEF	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO13, input
#define VAL_MUX_GPIO14_SDK_DEF	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO14, input
#define VAL_MUX_GPIO15_SDK_DEF	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO15, input

#define VAL_MUX_GPIO0_IOPORT	(1<<GPIO_MUX_PULLUP_BIT)	// GPIO0, input
#define VAL_MUX_GPIO1_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// UART0, TX0, Outnput
#define VAL_MUX_GPIO2_IOPORT	(1<<GPIO_MUX_PULLUP_BIT)	// UART1, TX1, Outnput
#define VAL_MUX_GPIO3_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// UART0, RX0, Input
#define VAL_MUX_GPIO4_IOPORT	(1<<GPIO_MUX_PULLUP_BIT)	// GPIO4, input
#define VAL_MUX_GPIO5_IOPORT	(1<<GPIO_MUX_PULLUP_BIT)	// GPIO5, input
#define VAL_MUX_GPIO6_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// SPI0CLK
#define VAL_MUX_GPIO7_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// SPI0Q
#define VAL_MUX_GPIO8_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// SPI0D
#define VAL_MUX_GPIO9_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// SPI0HD
#define VAL_MUX_GPIO10_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// SPI0WP
#define VAL_MUX_GPIO11_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// SPI0CS0
#define VAL_MUX_GPIO12_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO12, input
#define VAL_MUX_GPIO13_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO13, input
#define VAL_MUX_GPIO14_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO14, input
#define VAL_MUX_GPIO15_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO15, input

// таблица (по 4 бита на номер пина) адресов IO_MUX в соответствии с номерами GPIOn
#define _IO_MUX_GPIO ((uint64_t)0x3210BA9876FE4D5Cull)
// получить адрес IO_MUX в соответствии с номером GPIOn
#define GPIOx_MUX(PIN_NUM) iomux_[1 + (uint32)((_IO_MUX_GPIO>>((PIN_NUM)<<2)) & 0x0F)]

// таблица (по 2 бита на номер пина) номеров функций пинов для установки в режим GPIO
#define _FUN_IO_PORT  ((uint32_t)0xFFFFF0CCul)
// таблица (по 4 бита на номер пина) номеров функций пинов для установки в режим по умочанию в SDK
#define _FUN_DEF_SDK  ((uint64_t)0x3333111111000200ull)

// получить номер функции для установки I/O пина в режим порта GPIOn
#define MUX_FUN_IO_PORT(PIN_NUM) ((uint32_t)(_FUN_IO_PORT >> (PIN_NUM<<1)) & 0x03)
// получить номер функции для установки I/O пина в режим по умочанию в SDK
#define MUX_FUN_DEF_SDK(PIN_NUM) ((uint32_t)(_FUN_DEF_SDK >> (PIN_NUM<<2)) & 0x07)

#define SET_PIN_FUNC(PIN_NUM, FUN) GPIOx_MUX(PIN_NUM) = (GPIOx_MUX(PIN_NUM) & (~GPIO_MUX_FUN_MASK)) | ((FUN&3)<<GPIO_MUX_FUN_BIT0) | ((FUN&4)<<(GPIO_MUX_FUN_BIT2-2))
#define GET_PIN_FUNC(PIN_NUM) (((GPIOx_MUX(PIN_NUM)>>GPIO_MUX_FUN_BIT0)&3) | ((GPIOx_MUX(PIN_NUM)>>(GPIO_MUX_FUN_BIT2 + 2))&4))
#define SET_PIN_PULLUP_ENA(PIN_NUM)  GPIOx_MUX(PIN_NUM) |= 1 << GPIO_MUX_PULLUP_BIT
#define SET_PIN_PULLUP_DIS(PIN_NUM) GPIOx_MUX(PIN_NUM) &= ~(1 << GPIO_MUX_PULLUP_BIT)
#define SET_PIN_PULL_DIS(PIN_NUM) GPIOx_MUX(PIN_NUM) &= ~((1 << GPIO_MUX_PULLUP_BIT) | (1 << GPIO_MUX_PULLDOWN_BIT))
#define SET_PIN_PULLDOWN_ENA(PIN_NUM)  GPIOx_MUX(PIN_NUM) |= 1 << GPIO_MUX_PULLDOWN_BIT
#define SET_PIN_PULLDOWN_DIS(PIN_NUM) GPIOx_MUX(PIN_NUM) &= ~(1 << GPIO_MUX_PULLDOWN_BIT)


// установить функцию GPIOn как I/O port
#define SET_PIN_FUNC_IOPORT(PIN_NUM) SET_PIN_FUNC(PIN_NUM, MUX_FUN_IO_PORT(PIN_NUM))
// установить функцию GPIOn по умолчанию для SDK
#define SET_PIN_FUNC_DEF_SDK(PIN_NUM) SET_PIN_FUNC(PIN_NUM, MUX_FUN_DEF_SDK(PIN_NUM))

/* SCL:0x60000B00 registers */
/* SLC_CONF0:0x60000B00 */
#define SLC_CONF0		scl_[0]
/* SLC_INT_RAW:0x60000B04 */
#define SLC_INT_RAW		scl_[1]
/* SLC_INT_STATUS:0x60000B08 */
#define SLC_INT_STATUS	scl_[2]
/* SLC_INT_ENA:0x60000B0C */
#define SLC_INT_ENA		scl_[3]
/* SLC_INT_CLR:0x60000B10 */
#define SLC_INT_CLR		scl_[4]
/* SLC_RX_STATUS:0x60000B14 */
#define SLC_RX_STATUS	scl_[5]
/* SLC_RX_FIFO_PUSH:0x60000B18 */
#define SLC_RX_FIFO_PUSH	scl_[6]
/* SLC_TX_STATUS:0x60000B1C */
#define SLC_TX_STATUS	scl_[7]
/* SLC_TX_FIFO_POP:0x60000B20 */
#define SLC_TX_FIFO_POP		scl_[8]
/* SLC_RX_LINK:0x60000B24 */
#define SLC_RX_LINK		scl_[9]
/* SLC_TX_LINK:0x60000B28 */
#define SLC_TX_LINK		scl_[10]
/* SLC_INTVEC_TOHOST:0x60000B2C */
#define SLC_INTVEC_TOHOST	scl_[11]
/* SLC_TOKEN0:0x60000B30 */
#define SLC_TOKEN0		scl_[12]
/* SLC_TOKEN1:0x60000B34 */
#define SLC_TOKEN1		scl_[13]
/* SLC_CONF1:0x60000B38 */
#define SLC_CONF1		scl_[14]
/* SLC_STATE0:0x60000B3C */
#define SLC_STATE0		scl_[15]
/* SLC_STATE1:0x60000B40 */
#define SLC_STATE1		scl_[16]
/* SLC_BRIDGE_CONF:0x60000B44 */
#define SLC_BRIDGE_CONF	scl_[17]
/* SLC_RX_EOF_DES_ADDR:0x60000B48 */
#define SLC_RX_EOF_DES_ADDR	scl_[18]
/* SLC_TX_EOF_DES_ADDR:0x60000B4C */
#define SLC_TX_EOF_DES_ADDR	scl_[19]
/* SLC_RX_EOF_BFR_DES_ADDR:0x60000B50 */
#define SLC_RX_EOF_BFR_DES_ADDR		scl_[20]
/* SLC_AHB_TEST:0x60000B54 */
#define SLC_AHB_TEST scl_[21]
/* SLC_SDIO_ST:0x60000B58 */
#define SLC_SDIO_ST		scl_[22]
/* SLC_RX_DSCR_CONF:0x60000B5C */
#define SLC_RX_DSCR_CONF	scl_[23]
/* SLC_TXLINK_DSCR:0x60000B60 */
#define SLC_TXLINK_DSCR	scl_[24]
/* SLC_TXLINK_DSCR_BF0:0x60000B64 */
#define SLC_TXLINK_DSCR_BF0	scl_[25]
/* SLC_TXLINK_DSCR_BF1:0x60000B68 */
#define SLC_TXLINK_DSCR_BF1		scl_[26]
/* SLC_RXLINK_DSCR:0x60000B6C */
#define SLC_RXLINK_DSCR		scl_[27]
/* SLC_RXLINK_DSCR_BF0:0x60000B70 */
#define SLC_RXLINK_DSCR_BF0	scl_[28]
/* SLC_RXLINK_DSCR_BF1:0x60000B74 */
#define SLC_RXLINK_DSCR_BF1	scl_[29]
/* SLC_DATE:0x60000B78 */
#define SLC_DATE		scl_[30]
/* SLC_ID:0x60000B7C */
#define SLC_ID		scl_[31]

/* SLC_HOST_INTR_RAW:0x60000B88 */
#define SLC_HOST_INTR_RAW	scl_[34]

/* SLC_HOST_CONF_W0:0x60000B94 */
#define SLC_HOST_CONF_W0	scl_[37]
/* SLC_HOST_CONF_W1:0x60000B98 */
#define SLC_HOST_CONF_W1	scl_[38]
/* SLC_HOST_INTR_ST:0x60000B9C */
#define SLC_HOST_INTR_ST	scl_[39]
/* SLC_HOST_CONF_W2:0x60000BA0 */
#define SLC_HOST_CONF_W2	scl_[40]
/* SLC_HOST_CONF_W3:0x60000BA4 */
#define SLC_HOST_CONF_W3	scl_[41]
/* SLC_HOST_CONF_W4:0x60000BA8 */
#define SLC_HOST_CONF_W4	scl_[42]

/* SLC_HOST_INTR_CLR:0x60000BB0 */
#define SLC_HOST_INTR_CLR	scl_[44]
/* SLC_HOST_INTR_ENA:0x60000BB4 */
#define SLC_HOST_INTR_ENA	scl_[45]
/* SLC_HOST_CONF_W5:0x60000BB8 */
#define SLC_HOST_CONF_W5	scl_[46]

/* USER RTC RAM:	0x60001100 */
#define RTC_MEM(IDX)	rtc_mem_[IDX]
#define RTC_MEM_SIZE 	0x300 // user RTC RAM 768 bytes, 192 dword registers

#endif
