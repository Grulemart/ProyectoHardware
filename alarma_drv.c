
#include "alarma_drv.h"

//// TODO: Finalizar cuando planificador este implementado
void temporizador_drv_reloj (uint32_t periodo, CallBack funcion_callback, enum EVENTO_T ID_evento) {
	
	// Llamar a la fifo para crear un nuevo evento y como dato auxiliar enviar el callback
	
	
	// Llamar al hal para configurar la alarma
	
}

void iniciar_temporizador_drv_alarma (uint32_t periodo) {
	iniciar_temporizador_hal_alarma(periodo);
}
