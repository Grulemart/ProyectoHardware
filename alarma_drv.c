
#include "alarma_drv.h"

//// TODO: Finalizar cuando planificador este implementado
void temporizador_drv_reloj (uint32_t periodo, CallBack funcion_callback, enum EVENTO_T ID_evento) {
	
}

void iniciar_temporizador_drv_alarma (uint32_t periodo) {
	iniciar_temporizador_hal_alarma(periodo);
}
