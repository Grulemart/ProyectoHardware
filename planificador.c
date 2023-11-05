
#include "planificador.h"


static uint8_t overflow = NO_HAY_OVERFLOW;
static enum EVENTO_T evento;
static uint32_t auxData;

void planificador(void) {
	
	// Inicializaci�n de I/O
	gpio_hal_iniciar();
	FIFO_inicializar(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS);
	
	// hello_world_inicializar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	
	alarma_inicializar();
	alarma_activar(BOTON_PULSADO, 10, 0);
	while(overflow != HAY_OVERFLOW) {
		
		// Extraccion de eventos a procesar
		while((FIFO_extraer(&evento, &auxData)) == NO_HAY_EVENTO_A_PROCESAR){
			//power_hal_wait();
		}
		
		// Datos a procesar
		if (evento == TIMER0) {
			// Procesar evento TIMER0
		} else if (evento == TIMER1) {
			// Procesar evento TIMER1
			hello_world_tick_tack();
		} else if (evento == GPIO) {
			// Procesar evento GPIO
		} else if (evento == ALARMA) {
			alarma_tratar_evento();
			// Procesar evento ALARMA
		}	else if (evento == ALARMA_OVERFLOW) {
			// Procesar overflow si hay overflow de alarmas
			gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1);
		} else if (evento == BOTON_PULSADO) {
			// Procesar evento EINT1
		}

		
		else {
			// Procesar evento VOID (error)
		}
		
		
		// Lee el pin de overflow y sale del bucle si está activo
		overflow = gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS);
		
	}
	
	
	
}
