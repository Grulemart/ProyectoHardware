
#include "planificador.h"
#include "fifo.h"
#include "timer_drv.h"
#include "hello_world.h"
#include <inttypes.h>
#include "power_hal.h"
#include "alarma.h"
#include "botones.h"
#include "juego.h"
#include "visualizar.h"
#include "alarma.h"
#include "linea_serie_drv.h"
#include "watchdog.h"
#include "io_reserva.h"
#include "llamadas_swi.h"
#include "juego.h"
#include "evento.h"

#define TRUE 1
#define FALSE 0

static uint8_t overflow = NO_HAY_OVERFLOW;
static uint8_t evento;
static uint32_t auxData;
static uint8_t isPowerDown = FALSE;


void planificador(void) {
	int i;
	static char comando[3];
	
	// Inicializacion de modulos
	gpio_hal_iniciar();
	FIFO_inicializar(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS);
	temporizador_drv_iniciar();
	temporizador_drv_empezar();
	iniciar_botones(FIFO_encolar, BOTON_PULSADO, MONITORIZAR_BOTON);
	alarma_inicializar(ALARMA, ALARMA_OVERFLOW, FIFO_encolar);
	iniciar_linea_serie(EV_RX_SERIE, EV_TX_SERIE, FIFO_encolar, GPIO_SERIE_ERROR);
	hello_world_inicializar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS, VISUALIZAR_HELLO);
	juego_inicializar();
	visualizar_inicializar(GPIO_VISUALIZAR, GPIO_VISUALIZAR_BITS);
	WD_hal_inicializar(WATCHDOG_TIME);
	
	alarma_activar(POWER_DOWN, USUARIO_AUSENTE, 0);
	
	juego_mostrar_instrucciones();
	
	while(overflow != HAY_OVERFLOW) {
		
		// Extraccion de eventos a procesar
		while((FIFO_extraer(&evento, &auxData)) == NO_HAY_EVENTO_A_PROCESAR){
			WD_hal_feed();
			power_hal_wait();
		}
		WD_hal_feed();
		
		// Datos a procesar
		if (evento == TIMER0) {
			// Procesar evento TIMER0
		} else if (evento == TIMER1) {
			// Procesar evento TIMER1
		} else if (evento == GPIO) {
			// Procesar evento GPIO
		} else if (evento == ALARMA) {
			// Procesar evento ALARMA
			alarma_tratar_evento();
		}	else if (evento == ALARMA_OVERFLOW) {
			// Procesar overflow si hay overflow de alarmas
			gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1);
		} else if (evento == BOTON_PULSADO) {
				alarma_reprogramar(POWER_DOWN, 0);
			if (isPowerDown == FALSE) {
				if (auxData == 1) { // EINT1
					cancelar_jugada();
				} else { // EINT2
					// Equivalente a escribir por comando '$END!'
					juego_tratar_comando("END");
					linea_serie_drv_enviar_array("$END!\n");
					//juego_transmision_realizada();
				}
			} else {
				isPowerDown = FALSE;
			}
		} else if (evento == MONITORIZAR_BOTON) {
			// Comprueba a los 100ms si un boton sigue estando pulsado
			if (sigue_pulsado((uint8_t)auxData) == FALSE) {
				// Cancelamos la alarma si ha dejado de pulsar el boton
				alarma_activar(evento, CANCELAR_ALARMA, auxData);
				habilitar_interrupcion((uint8_t)auxData);
			}
		} else if(evento == VISUALIZAR_CUENTA) {
			// Visualiza el intervalo entre pulsaciones en ms
			visualizar((uint8_t)auxData);
		} else if (evento == VISUALIZAR_HELLO) {
			hello_world_tick_tack();
		}
		else if(evento == POWER_DOWN) {
			power_hal_deep_sleep();
			//iniciar_linea_serie(EV_RX_SERIE, EV_TX_SERIE, FIFO_encolar, GPIO_SERIE_ERROR);
			isPowerDown = TRUE;
			alarma_reprogramar(POWER_DOWN, 0);
		
		}else if (evento == EV_RX_SERIE){
			for(i = 0; i<3; i++){
				comando[i] = (char)((auxData >> (8*i))& 0xFF);
			}
			auxData = 0;
			juego_tratar_comando(comando);
		}else if (evento == EV_TX_SERIE){
			juego_transmision_realizada();
			alarma_reprogramar(POWER_DOWN, 0);
		}else if (evento == HACER_JUGADA){
			juego_alarma();
		}else {
			// Procesar evento VOID (error)
		}
		
		// Lee el pin de overflow y sale del bucle si está activo
		overflow = gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS);
		
	}
	
}
