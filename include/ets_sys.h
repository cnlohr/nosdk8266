/*
 * copyright (c) 2008 - 2011 Espressif System
 *
 * Define user specified Event signals and Task priorities here
 *
 */

#ifndef _ETS_SYS_H
#define _ETS_SYS_H

#include "c_types.h"

typedef uint32_t ETSSignal;
typedef uint32_t ETSParam;

typedef struct ETSEventTag ETSEvent;

struct ETSEventTag {
    ETSSignal sig;
    ETSParam  par;
};

typedef void (*ETSTask)(ETSEvent *e);

/* timer related */
typedef uint32_t ETSHandle;
typedef void ETSTimerFunc(void *timer_arg);

typedef struct _ETSTIMER_ {
    struct _ETSTIMER_    *timer_next;
    uint32_t              timer_expire;
    uint32_t              timer_period;
    ETSTimerFunc         *timer_func;
    void                 *timer_arg;
} ETSTimer;

void ets_isr_attach( int, void(*)( void * ), void * );
void ets_isr_unmask( unsigned );


// From  https://github.com/espressif/ESP31_RTOS_SDK/blob/master/include/esp32/ets_sys.h --- MAY NOT BE CORRECT OR APPLICABLE.

#define ETS_WMAC_SOURCE          0
#define ETS_SLC_SOURCE           1
#define ETS_UART_SOURCE         13
#define ETS_UART1_SOURCE        14
#define ETS_FRC_TIMER2_SOURCE   43
#define ETS_PCNT_SOURCE         19
#define HW_GPIO_INUM            ETS_PCNT_SOURCE

/* interrupt related */
#define ETS_SDIO_INUM       1
#define ETS_SPI_INUM	    2
#define ETS_RTC_INUM        3
#define ETS_I2S_INUM        2
#define ETS_GPIO_INUM       4
#define ETS_UART_INUM       5
#define ETS_UART1_INUM      5
#define ETS_FRC_TIMER1_INUM 9  /* use edge*/
#define ETS_FRC_TIMER2_INUM 10 /* use edge*/
#define ETS_WDT_INUM        8  /* use edge*/

#define ETS_INTR_LOCK() \
    ets_intr_lock()

#define ETS_INTR_UNLOCK() \
    ets_intr_unlock()

#define ETS_FRC_TIMER1_INTR_ATTACH(func, arg) \
    ets_isr_attach(ETS_FRC_TIMER1_INUM, (func), (void *)(arg))

#define ETS_FRC_TIMER1_NMI_INTR_ATTACH(func) \
	NmiTimSetFunc(func)

#define ETS_SDIO_INTR_ATTACH(func, arg)\
    ets_isr_attach(ETS_SDIO_INUM, (func), (void *)(arg))

#define ETS_GPIO_INTR_ATTACH(func, arg) \
    ets_isr_attach(ETS_GPIO_INUM, (func), (void *)(arg))

#define ETS_UART_INTR_ATTACH(func, arg) \
    ets_isr_attach(ETS_UART_INUM, (func), (void *)(arg))

#define ETS_SPI_INTR_ATTACH(func, arg) \
    ets_isr_attach(ETS_SPI_INUM, (func), (void *)(arg))

#define ETS_INTR_ENABLE(inum) \
    ets_isr_unmask((1<<inum))

#define ETS_INTR_DISABLE(inum) \
    ets_isr_mask((1<<inum))

#define ETS_UART_INTR_ENABLE() \
    ETS_INTR_ENABLE(ETS_UART_INUM)

#define ETS_UART_INTR_DISABLE() \
    ETS_INTR_DISABLE(ETS_UART_INUM)

#define ETS_FRC1_INTR_ENABLE() \
	ETS_INTR_ENABLE(ETS_FRC_TIMER1_INUM)

#define ETS_FRC1_INTR_DISABLE() \
	ETS_INTR_DISABLE(ETS_FRC_TIMER1_INUM)

#define ETS_GPIO_INTR_ENABLE() \
    ETS_INTR_ENABLE(ETS_GPIO_INUM)

#define ETS_GPIO_INTR_DISABLE() \
    ETS_INTR_DISABLE(ETS_GPIO_INUM)
    
#define ETS_SPI_INTR_ENABLE() \
    ETS_INTR_ENABLE(ETS_SPI_INUM)

#define ETS_SPI_INTR_DISABLE() \
    ETS_INTR_DISABLE(ETS_SPI_INUM)

#define ETS_SDIO_INTR_ENABLE() \
    ETS_INTR_ENABLE(ETS_SDIO_INUM)

#define ETS_SDIO_INTR_DISABLE() \
    ETS_INTR_DISABLE(ETS_SDIO_INUM)
    
#endif /* _ETS_SYS_H */

