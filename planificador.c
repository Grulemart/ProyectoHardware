
#include "planificador.h"
#include "power_hal.h"

static uint8_t overflow = FALSE;
static enum EVENTO_T evento;
static uint32_t auxData;

void planificador(void) {
	
	// Inicialización de I/O
	FIFO_inicializar(GPIO_OVERFLOW);
	gpio_hal_iniciar();
	temporizador_drv_empezar();
	hello_world_inicializar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	
	// Inicializacion de cadena de eventos;
	hello_world_tick_tack();
	
	while(overflow != TRUE) {
		
		
		while((FIFO_extraer(&evento, &auxData)) == 0){
			power_hal_wait();
		}
		
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
		overflow = gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS);
		
	}
	
	
	
}
