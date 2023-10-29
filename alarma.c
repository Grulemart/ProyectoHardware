
#include "alarma.h"

#define US_A_MS 1000	// Programar TIMER1 para que interrumpa cada ms

static uint8_t alarmasActivas[MAX_ALARMAS];					// Alarmas activas
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
	// y tratar la funcion alarma_tratar_evento en cada interrupcion
	temporizador_hal_reloj(US_A_MS, alarma_tratar_evento);
}

void alarma_activar(enum EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData) {
	
	if (retardo == 0) { // Si retardo es igual a 0 cancelar la alarma
		return;
	}
	
	if (alarmasActivas[indice] == FALSE){ // Hay espacio para alarmas
		
		alarmasActivas[indice] = TRUE;
		alarmaEnd[indice] = (retardo >> 1) + ticks;
		alarmaReprogramar[indice] = (retardo >> 31);
		alarmaEvento[indice] = ID_evento;
		alarmaAuxData[indice] = auxData;
		
	} else {
		FIFO_encolar(ALARMA_OVERFLOW, 0);
	}
	
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
			}
		}
	}
	
}
