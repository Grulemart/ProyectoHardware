
#include "timer_drv.h"


static void (*funcionEncolarEvento)();
static volatile EVENTO_T idEvento;
// M?dulo del manejador o driver

// Inicializa el temporizador
void temporizador_drv_iniciar() {
    // Inicializa el temporizador hardware llamando a temporizador_hal_iniciar()
    temporizador_hal_iniciar();
}

// Inicia la cuenta del temporizador
void temporizador_drv_empezar() {
    // Inicia el temporizador hardware llamando a temporizador_hal_empezar()
    temporizador_hal_empezar();
}

// Lee el tiempo transcurrido en microsegundos
uint64_t temporizador_drv_leer() {
    // Lee el tiempo en ticks utilizando temporizador_hal_leer() y lo convierte a microsegundos
	return clock_get_us()/TICKS_A_US;
}

// Detiene el temporizador y devuelve el tiempo transcurrido en microsegundos
uint64_t temporizador_drv_parar() {
    // Detiene el temporizador hardware llamando a temporizador_hal_parar()
    return temporizador_hal_parar()/TICKS_A_US;
}

void callbackFunction(){
	(*funcionEncolarEvento)(idEvento,0);
}

void temporizador_drv_reloj(uint32_t periodo, void(*funcion_encolar_evento)(), 
	EVENTO_T ID_evento){
	// Se inicia el temporizador y cuando este acaba se encola un nuevo evento
	funcionEncolarEvento = funcion_encolar_evento;
	idEvento = ID_evento;
	temporizador_hal_reloj(periodo, callbackFunction);
			
}
