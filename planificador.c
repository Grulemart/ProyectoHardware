
#include "planificador.h"


static uint8_t overflow = NO_HAY_OVERFLOW;
static enum EVENTO_T evento;
static uint32_t auxData;

void planificador(void) {
	
	// Inicializaci�n de I/O
	gpio_hal_iniciar();
	FIFO_inicializar(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS);
	
	temporizador_drv_iniciar();
	temporizador_drv_empezar();
	
	hello_world_inicializar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	
	juego_inicializar();
	visualizar_inicializar();
	
	while(overflow != HAY_OVERFLOW) {
		
		// Extraccion de eventos a procesar
		while((FIFO_extraer(&evento, &auxData)) == NO_HAY_EVENTO_A_PROCESAR){
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
		} else if (evento == ALARMA) {
			// Procesar evento ALARMA
			alarma_tratar_evento();
		}	else if (evento == ALARMA_OVERFLOW) {
			// Procesar overflow si hay overflow de alarmas
			gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1);
		} else if (evento == EINT1) {
			// Procesar evento EINT1
			juego_tratar_evento(VISUALIZAR_CUENTA, 0);
		} else if (evento == EINT2) {
			// Procesar evento EINT2
		} else if (evento == DEJAR_BOTON) {
			// Comprueba a los 100ms si un boton sigue estando pulsado
			if (sigue_pulsado((uint8_t)auxData) == FALSE) {
				habilitar_interrupcion((uint8_t)auxData);
			}
		} else if (evento == VISUALIZAR_CUENTA) {
			// Visualiza el intervalo entre pulsaciones en ms
			visualizar((uint8_t)auxData);
		}

		
		else {
			// Procesar evento VOID (error)
		}
		
		
		// Lee el pin de overflow y sale del bucle si está activo
		overflow = gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS);
		
	}
	
	
	
}
