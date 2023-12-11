
#include "desactivar_interrupciones.h"
#include "llamadas_swi.h"
#include "power_hal.h"

static volatile uint32_t state;

// Desactiva las interrupciónes y guarda el estado de las interrupciones antes de hacerlo
void lock(void) {
	state = read_IRQ_bit();
	disable_irq();
}

// Reactiva las interrupciones, en caso de que antes ya no estuviesen desactivadas
void unlock(void) {
	if (state == IRQ_DESACTIVADA) {	// Si estan desactivadas vuelve a activar
		disable_irq();
	}else{
		enable_irq();
	}
}
