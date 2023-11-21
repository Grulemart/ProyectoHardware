
#include "semaforo.h"

static volatile uint32_t state;

void lock(void) {
	state = read_IRQ_bit();
	disable_irq();
}

void unlock(void) {
	if (state == IRQ_DESACTIVADA) {	// Si estan desactivadas vuelve a activar
		disable_irq();
	}else{
		enable_irq();
	}
}
