#include "llamadas_swi.h"

// Define el comportamiento de la llamada al sistema swi0
// Devuelve la cantidad de us pasados desde la iniciación del temporizador
uint32_t __SWI_0(void) {
	return temporizador_drv_leer();
}
