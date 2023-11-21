
#include "semaforo.h"

static uint32_t state;

void lock(void) {
	state = read_IRQ_bit();
	while(state == IRQ_DESACTIVADA) {			// Esperar a que se deje de usar el lock
		power_hal_wait();
	}
	disable_irq();
}

void unlock(void) {
	if (state == IRQ_DESACTIVADA) {	// Si estan desactivadas vuelve a activar
		enable_irq();
	}
}
