#ifndef __LLAMADAS_SISTEMA_H__
#define __LLAMADAS_SISTEMA_H__

#include <LPC210x.H> 
#include <inttypes.h>

uint32_t __swi(0) clock_gettime(void);

uint32_t __swi(1) clock_get_us(void);

uint8_t __swi(2) read_IRQ_bit(void);

uint8_t __swi(3) read_FIQ_bit(void);

void __swi(0xFF) enable_fiq(void);

void __swi(0xFE) disable_fiq(void);

void __swi(0xFD) enable_irq_fiq(void);

void __swi(0xFC) disable_irq_fiq(void);

void __swi(0xFB) enable_irq(void);

void __swi(0xFA) disable_irq(void);

uint32_t __SWI_0() { return CTIME0; }

uint32_t __SWI_1(void) { return T1TC; }

#endif 