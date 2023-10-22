
#include "planificador.h"

static uint8_t overflow = FALSE;
static enum EVENTO_T evento;
static uint32_t auxData;

void planificador() {
	
	uint8_t state;
	
	// Inicialización de I/O
	FIFO_inicializar(GPIO_OVERFLOW);
	gpio_drv_iniciar();
	temporizador_drv_empezar();
	iniciar_temporizador_drv_alarma(1000);	// 1000ms = 1s
	
	while(overflow != TRUE) {
		
		while((state = FIFO_extraer(&evento, &auxData)) == 0);
		
		// Sale del bucle infinito y acaba el programa.
		if (state == 2) {
			overflow = TRUE;
		} 
		else if (state == 1) {
			// Datos a procesar
			if (evento == TIMER0) {
				// Procesar evento TIMER0
			} else if (evento == TIMER1) {
				// Procesar evento TIMER1
			} else if (evento == GPIO) {
				// Procesar evento GPIO
			}
		}
		
	}
	
	
	
}