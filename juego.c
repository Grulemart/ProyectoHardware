
#include "juego.h"

static uint8_t cuenta;
static uint64_t intervalo;

void juego_inicializar() {
	cuenta = 0;
	intervalo = 0;
}

void juego_tratar_evento(EVENTO_T ID_evento, uint32_t auxData) {
	
	uint64_t currentCheck = temporizador_drv_leer();
	intervalo = currentCheck - intervalo;
	cuenta += auxData;
	
	FIFO_encolar(ID_evento, (uint32_t)cuenta);
	
}
