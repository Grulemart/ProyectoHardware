
#include "planificador.h"


static uint8_t overflow = NO_HAY_OVERFLOW;
static EVENTO_T evento;
static uint32_t auxData;

void planificador(void) {
	
	// Inicializaci�n de I/O
	gpio_hal_iniciar();
	FIFO_inicializar(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS);
	
	temporizador_drv_iniciar();
	temporizador_drv_empezar();
	iniciar_botones();
	alarma_inicializar();
	
	hello_world_inicializar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	
	juego_inicializar();
	visualizar_inicializar();

	alarma_activar(POWER_DOWN, USUARIO_AUSENTE, 0);
	iniciar_linea_serie();
	linea_serie_drv_enviar_array("Prueba array");
	
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
		} else if (evento == GPIO) {
			// Procesar evento GPIO
		} else if (evento == ALARMA) {
			alarma_tratar_evento();
			// Procesar evento ALARMA
			alarma_tratar_evento();
		}	else if (evento == ALARMA_OVERFLOW) {
			// Procesar overflow si hay overflow de alarmas
			gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1);
		} else if (evento == BOTON_PULSADO) {
				alarma_reprogramar(POWER_DOWN, 0);
			if (auxData == 1) { // EINT1
				juego_tratar_evento(VISUALIZAR_CUENTA, (uint32_t)1); // Constatne añade 1 a cuenta en modulo juego
			} else { // EINT2
				juego_tratar_evento(VISUALIZAR_CUENTA, (uint32_t)-1); // Constante resta 1 a cuenta en modulo juego
			}
			
			//juego_tratar_evento(VISUALIZAR_CUENTA, 0);
		} else if (evento == MONITORIZAR_BOTON) {
			// Comprueba a los 100ms si un boton sigue estando pulsado
			if (sigue_pulsado((uint8_t)auxData) == FALSE) {
				// Cancelamos la alarma si ha dejado de pulsar el boton
				alarma_activar(evento, CANCELAR_ALARMA, auxData);
				habilitar_interrupcion((uint8_t)auxData);
			}
		} else if (evento == VISUALIZAR_CUENTA) {
			// Visualiza el intervalo entre pulsaciones en ms
			visualizar((uint8_t)auxData);
		} else if (evento == VISUALIZAR_HELLO) {
			hello_world_tick_tack();
		}
		else if (evento == POWER_DOWN) {
			power_hal_deep_sleep();
			alarma_reprogramar(POWER_DOWN, 0);
			iniciar_linea_serie();
		}	else if (evento == EV_RX_SERIE){
			//Procesar evento recepcion serie
			
		} else if (evento == EV_TX_SERIE){
			//Procesar evento transimisón serie
		}else {
			// Procesar evento VOID (error)
		}
		
		// Lee el pin de overflow y sale del bucle si está activo
		overflow = gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS);
		
	}
	
	
	
}
