#include "llamadas_swi.h"

uint32_t __SWI_0(void) {
	return temporizador_drv_leer();
}
