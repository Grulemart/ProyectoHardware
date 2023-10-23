
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
	hello_world_inicializar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	
	
	// Encolamos eventos a modo de prueba para helloWorld
	for (uint8_t i = 0; i < 20; i++) {
		FIFO_encolar(TIMER0, (uint32_t)hello_world_tick_tack, &overflow);
	}
	
	while(overflow != TRUE) {
		
		while((state = FIFO_extraer(&evento, &auxData)) == 0);
		
		// Datos a procesar
		if (evento == TIMER0) {
			// Procesar evento TIMER0
		} else if (evento == TIMER1) {
			// Procesar evento TIMER1
		} else if (evento == GPIO) {
			// Procesar evento GPIO
		} else {
			// Procesar evento VOID (error)
		}
		
		
	}
	
	
	
}