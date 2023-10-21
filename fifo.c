
#include "fifo.h"
#include <stdlib.h>

static enum EVENTO_T *fifo;							  	// Dirección de memoria de vector de eventos registrados
static BOOLEAN procesados[FIFO_SIZE];				// Array de registro de eventos procesados
static uint32_t eventRegister[EVENT_TYPES];	// Registro de numero de eventos de un tipo producidos
static uint8_t indice;										  // Indice de ultimo evento registrado
static uint8_t indiceProcesados;						// Indice para registrar eventos procesados

void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow) {
	
	indice = 0;
	indiceProcesados = 0;
	
	fifo = malloc(sizeof(enum EVENTO_T) * FIFO_SIZE);
	for (uint8_t i = 0; i < FIFO_SIZE; i++) {
		fifo[i] = VOID;
		procesados[i] = FALSE;
	}
	
	for (uint8_t i = 0; i < EVENT_TYPES; i++) {
		eventRegister[i] = 0;
	}
	
}

void FIFO_encolar(enum EVENTO_T ID_evento, uint32_t auxData) {
	
	fifo[indice] = ID_evento;
	procesados[indice] = FALSE;
	
	eventRegister[(uint8_t)ID_evento] += 1;
	eventRegister[VOID] += 1;
	
	indice = (indice + 1) % FIFO_SIZE;

}

// TODO: Comprobar si indice procesado es igual a indice de encolar
uint8_t FIFO_extraer(enum EVENTO_T *ID_evento, uint32_t *auxData) {
	
	uint8_t retVal = 0;
	
	// TODO: Comprobar que existe algun elemento no procesado
	
	
	// Si existe retornar valores
	if (retVal == 1) {
		ID_evento = &fifo[indice - 1];
	}
	
	return retVal;
	
}

uint32_t FIFO_estadisticas(uint8_t ID_evento) {
	
	return eventRegister[ID_evento];
	
}