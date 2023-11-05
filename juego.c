
#include "juego.h"

static uint64_t cuenta;
static uint8_t intervalo;

void juego_inicializar() {
	cuenta = 0;
	intervalo = 0;
}

void juego_tratar_evento(enum EVENTO_T ID_evento, uint32_t auxData) {
	
	uint64_t currentCheck = temporizador_drv_leer();
	intervalo = currentCheck - cuenta;
	cuenta = currentCheck;
	
	FIFO_encolar(ID_evento, (uint32_t)intervalo);
	
}
