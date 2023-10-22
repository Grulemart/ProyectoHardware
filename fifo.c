
#include "fifo.h"
#include <stdlib.h>

static enum EVENTO_T *fifo;							  	// Dirección de memoria de vector de eventos registrados
static uint32_t auxDataArray[FIFO_SIZE];		// Array de datos auxiliares de los eventos
static enum BOOLEAN procesados[FIFO_SIZE];	// Array de registro de eventos procesados
static uint32_t eventRegister[EVENT_TYPES];	// Registro de numero de eventos de un tipo producidos
static uint8_t indice;										  // Indice de ultimo evento registrado
static uint8_t indiceProcesados;						// Indice para registrar eventos procesados
static GPIO_HAL_PIN_T overflow;							// Pin de overflow

void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow) {
	
	indice = 0;
	indiceProcesados = 0;
	overflow = pin_overflow;
	
	fifo = malloc(sizeof(enum EVENTO_T) * FIFO_SIZE);
	for (uint8_t i = 0; i < FIFO_SIZE; i++) {
		fifo[i] = VOID;
		procesados[i] = FALSE;
		auxDataArray[i] = 0;
	}
	
	for (uint8_t i = 0; i < EVENT_TYPES; i++) {
		eventRegister[i] = 0;
	}
	
}

void FIFO_encolar(enum EVENTO_T ID_evento, uint32_t auxData) {
	
	fifo[indice] = ID_evento;
	procesados[indice] = FALSE;
	auxDataArray[indice] = auxData;
	
	eventRegister[(uint8_t)ID_evento] += 1;
	eventRegister[VOID] += 1;
	
	indice = (indice + 1) % FIFO_SIZE;
	
}

uint8_t FIFO_extraer(enum EVENTO_T *ID_evento, uint32_t *auxData) {
	
	// Se produce overflow
	if (indice == indiceProcesados && procesados[indice] == FALSE) {
		// Enciende led de overflow
		gpio_hal_escribir(overflow, GPIO_OVERFLOW_BITS, 1);
		return 2;
	}
	// No hay eventos a procesar
	else if (indice == indiceProcesados && procesados[indice] == TRUE) {
			return 0;
	} 
	// Hay eventos a procesar
	else {
		*ID_evento = fifo[indiceProcesados];
		*auxData = auxDataArray[indiceProcesados];
		return 1;
	}
	
}

uint32_t FIFO_estadisticas(uint8_t ID_evento) {
	
	return eventRegister[ID_evento];
	
}