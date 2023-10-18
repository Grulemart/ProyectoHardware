
#include "fifo.h"
#include <stdlib.h>

struct EVENTO_T *fifo;											// Dirección de memoria de vector de eventos registrados
static uint32_t eventRegister[EVENT_TYPES];	// Registro de numero de eventos de un tipo producidos
static uint8_t iterador;										// Iterador de ultimo evento registrado
static uint8_t size;												// Contador de eventos registrados

void FIFO_inicializar(uint8_t pin_overflow) {
	
	size = 0;
	iterador = 0;
	
	fifo = malloc(sizeof(struct EVENTO_T) * FIFO_SIZE);
	for (uint8_t i = 0; i < FIFO_SIZE; i++) {
		fifo[i].ID = VOID;
		fifo[i].procesado = TRUE;
	}
	
	for (uint8_t i = 0; i < EVENT_TYPES; i++) {
		eventRegister[i] = 0;
	}
	
}

void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData) {
	
	fifo[iterador].ID = ID_evento.ID;
	fifo[iterador].procesado = FALSE;
	
	eventRegister[ID_evento.ID] += 1;
	eventRegister[VOID] += 1;
	
	//// TODO: Asignar auxData
	
	iterador = (iterador + 1) % FIFO_SIZE;
	size = size+1 < FIFO_SIZE ? size+1 : FIFO_SIZE;

}

uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData) {
	
	uint8_t retVal = 0;
	
	// Comprobar que existe algun elemento no procesado
	if (size > 0) {
		for (uint8_t i = 0; i < size; i++) {
			uint8_t auxAddress = (iterador - 1 - size) % FIFO_SIZE;
			if (fifo[auxAddress].procesado == FALSE) {
				retVal = 1;
			}				
		}
	}
	
	// Si existe retornar valores
	if (retVal == 1) {
		ID_evento = &fifo[iterador - 1];
		//// TODO: Asignar auxData
	}
	
	return retVal;
	
}

uint32_t FIFO_estadisticas(uint8_t ID_evento) {
	
	return eventRegister[ID_evento];
	
}