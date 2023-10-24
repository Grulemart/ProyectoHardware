
#include "planificador.h"

static uint8_t overflow = FALSE;
static enum EVENTO_T evento;
static uint32_t auxData;

void planificador(void) {
	
	uint8_t error;
	
	// Inicialización de I/O
	gpio_hal_iniciar();
	temporizador_drv_empezar();
	FIFO_inicializar(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS);
	
	// Inicializacion de tests
	hello_world_inicializar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	
	// Inicializacion de cadena de eventos;
	hello_world_tick_tack();
	
	while(overflow != HAY_OVERFLOW) {
		
		// Extraccion de eventos a procesar
		while((error = FIFO_extraer(&evento, &auxData)) == NO_HAY_EVENTO_A_PROCESAR);
		
		if (error != HAY_OVERFLOW) {
			// Datos a procesar
			if (evento == TIMER0) {
				// Procesar evento TIMER0
			} else if (evento == TIMER1) {
				// Procesar evento TIMER1
				hello_world_tick_tack();
				
			} else if (evento == GPIO) {
				// Procesar evento GPIO
			} else {
				// Procesar evento VOID (error)
			}
		}
		
		// Se procesa un evento
		FIFO_procesar_evento();
		
	}
	
	
	
}
