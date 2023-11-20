
#include "llamadas_swi.h"

uint64_t __swi(0) clock_get_us(void);
uint64_t __SWI_0(void) {
		return temporizador_drv_leer();
}

uint32_t __swi(1) read_IRQ_bit (void);
void __swi(0xFF) disable_fiq (void);
void __swi(0xFE) enable_irq (void);
void __swi(0xFD) disable_irq (void);
