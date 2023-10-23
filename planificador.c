
#include "planificador.h"

static uint8_t overflow = FALSE;
static enum EVENTO_T evento;
static uint32_t auxData;
static int i;

void planificador(void) {
	
	// Inicialización de I/O
	FIFO_inicializar(GPIO_OVERFLOW);
	gpio_hal_iniciar();
	temporizador_drv_empezar();
	hello_world_inicializar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	
	
	// Encolamos eventos a modo de prueba para helloWorld

	for (i = 0; i < 20; i++) {
		//FIFO_encolar(TIMER0, (uint32_t)hello_world_tick_tack, &overflow);
	}
	
	while(overflow != TRUE) {
		
		while((FIFO_extraer(&evento, &auxData)) == 0);
		
		// Datos a procesar
		if (evento == TIMER0) {
			// Procesar evento TIMER0
		} else if (evento == TIMER1) {
			// Procesar evento TIMER1
			//temporizador_drv_reloj(1000, hello_world_tick_tack);
		} else if (evento == GPIO) {
			// Procesar evento GPIO
		} else {
			// Procesar evento VOID (error)
		}
		
		
	}
	
	
	
}
