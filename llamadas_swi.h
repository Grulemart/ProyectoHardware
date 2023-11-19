#ifndef __LLAMADAS_SWI_H__
#define __LLAMADAS_SWI_H__

#include <LPC210x.H> 
#include <inttypes.h>

uint32_t __swi(0) clock_get_us(void);

uint8_t __swi(1) read_IRQ_bit(void);

void __swi(0xFF) disable_fiq(void);

void __swi(0xFE) enable_irq(void);

void __swi(0xFD) disable_irq(void);

uint32_t __SWI_0(void) { return T1TC; }

#endif 