
#include "planificador.h"

static uint8_t overflow = FALSE;
static enum EVENTO_T evento;
static uint32_t auxData;

void planificador() {
	
	uint8_t state;
	
	FIFO_inicializar(GPIO_OVERFLOW);
	
	while(overflow != TRUE) {
		
		while((state = FIFO_extraer(&evento, &auxData)) == 0);
		
		if (state == 2) {
			overflow = TRUE;
		} else if (state == 1) {
			
			// Datos a procesar
			
		}
		
	}
	
	
	
}