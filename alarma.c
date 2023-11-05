
#include "alarma.h"

#define PERIODO_TIMER1 1	// Programar TIMER1 para que interrumpa cada 1 ms

static uint8_t alarmasActivas[MAX_ALARMAS];					// Alarmas activas
static uint8_t alarmaRetardoInicial[MAX_ALARMAS];		// Retardo incial proporcionado para reprogramar alarmas
static uint32_t alarmaEnd[MAX_ALARMAS];							// Tick de expiracion de las alarmas
static enum BOOLEAN alarmaReprogramar[MAX_ALARMAS]; // Reprogramacion de alarmas
static enum EVENTO_T alarmaEvento[MAX_ALARMAS];			// Evento a encolar tras expiracion
static uint32_t alarmaAuxData[MAX_ALARMAS];					// Datos auxiliares del evento a encolar


static uint32_t ticks;	// Ticks totales transcurridos desde inicializacion
static uint8_t indice;  // Indice de la siguiente alarma libre

void alarma_inicializar() {
	
	ticks = 0;
	indice = 0;
	
	// Inicializar temporizador con interrupcion a 1ms 
	// Cada 1ms el timer1 encolara el evento ALARMA en la cola FIFO
	temporizador_drv_reloj(PERIODO_TIMER1, FIFO_encolar, ALARMA);

}

void alarma_activar(enum EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData) {
	
	if (retardo == 0) { // Si retardo es igual a 0 cancelar la alarma
		return;
	}
	
	// Iterar sobre el array de alarmas hasta encontrar una disponible
	enum BOOLEAN no_hay_alarmas = TRUE;
	for (uint8_t i = 0; i < MAX_ALARMAS; i++) {
		uint8_t newIndice = (i+indice) % MAX_ALARMAS;
		if (alarmasActivas[newIndice] == FALSE) {
			no_hay_alarmas = FALSE;
			indice = newIndice;
			break;
		}
	}
	
	// Si no hay alarmas disponibles se encola evento ALARMA_OVERFLOW
	if (no_hay_alarmas == TRUE) {
		FIFO_encolar(ALARMA_OVERFLOW, 0);
		return;
	}
	
	// Iniciar alarma
	alarmasActivas[indice] = TRUE;
	alarmaRetardoInicial[indice] = (retardo >> 1);
	alarmaEnd[indice] = alarmaRetardoInicial[indice] + ticks;
	alarmaReprogramar[indice] = (retardo >> 31);
	alarmaEvento[indice] = ID_evento;
	alarmaAuxData[indice] = auxData;
	
}

void alarma_tratar_evento() {

	// Aumentar numero de ciclos
	ticks++;
	
	// Comprobar que han expirado las alarmas
	for (uint8_t i = 0; i < MAX_ALARMAS; i++) {
		if (alarmasActivas[i] == TRUE && alarmaEnd[i] >= ticks) {
			FIFO_encolar(alarmaEvento[i], alarmaAuxData[i]);
			if (alarmaReprogramar[i] == FALSE) {
				// Si no se activa reprogramacion la alarma no se reactiva
				alarmasActivas[i] = FALSE;
			} else {
				alarmaEnd[indice] = alarmaRetardoInicial[indice] + ticks;
			}
		}
	}
	
}

