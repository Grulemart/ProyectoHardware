
#include "fifo.h"

static enum EVENTO_T fifo[FIFO_SIZE];					// Dirección de memoria de vector de eventos registrados
static uint32_t auxDataArray[FIFO_SIZE];			// Array de datos auxiliares de los eventos
static enum BOOLEAN procesado[FIFO_SIZE];			// Array de registro de eventos procesados
static uint32_t eventRegister[EVENT_TYPES];		// Registro de numero de eventos de un tipo producidos
static uint8_t indiceUltimoEncolado;					// Indice de ultimo evento registrado
static uint8_t indiceProcesoATratar;					// Indice para registrar eventos procesados
static GPIO_HAL_PIN_T overflowPin;						// Pin de overflow
static GPIO_HAL_PIN_BITS_T overflowPinNumber;	// Numero de pines de overflow

void FIFO_inicializar(GPIO_HAL_PIN_T newPinOverflow, GPIO_HAL_PIN_BITS_T newOverflowPinNumber) {
	
	// Inicialización de variables
	uint8_t i;
	indiceUltimoEncolado = 0;
	indiceProcesoATratar = 0;
	
	overflowPin = newPinOverflow;
	overflowPinNumber = newOverflowPinNumber;
	gpio_hal_sentido(overflowPin, overflowPinNumber, GPIO_HAL_PIN_DIR_OUTPUT);
	
	for (i = 0; i < FIFO_SIZE; i++) {
		fifo[i] = VOID;
		procesado[i] = TRUE;
		auxDataArray[i] = 0;
	}
	
	for (i = 0; i < EVENT_TYPES; i++) {
		eventRegister[i] = 0;
	}
	
}

void FIFO_encolar(enum EVENTO_T ID_evento, uint32_t auxData) {
	// Se produce overflow
	if(indiceUltimoEncolado == indiceProcesoATratar && procesado[indiceUltimoEncolado] == FALSE){
		// Enciende led de overflow
		gpio_hal_escribir(overflowPin, overflowPinNumber, 1);
	}else{
		// Evento se encola como no procesado
		fifo[indiceUltimoEncolado] = ID_evento;
		procesado[indiceUltimoEncolado] = FALSE;
		auxDataArray[indiceUltimoEncolado] = auxData;
		
		// Actualización de indices
		eventRegister[(uint8_t)ID_evento] += 1;
		eventRegister[VOID] += 1;
		
		indiceUltimoEncolado = (indiceUltimoEncolado + 1) % FIFO_SIZE;
	}
		
}

uint8_t FIFO_extraer(enum EVENTO_T *ID_evento, uint32_t *auxData) {
	
	if (procesado[indiceProcesoATratar] == TRUE) {
		return NO_HAY_EVENTO_A_PROCESAR;
	} 
	// Hay eventos a procesar y se pasan por parámetros
	*ID_evento = fifo[indiceProcesoATratar];
	*auxData = auxDataArray[indiceProcesoATratar];
	procesado[indiceProcesoATratar] = TRUE;
	indiceProcesoATratar = (indiceProcesoATratar + 1) % FIFO_SIZE;
	return HAY_EVENTO_A_PROCESAR;
	
}

uint32_t FIFO_estadisticas(uint8_t ID_evento) {
	// Se devuelve el numero de eventos registrados de ID_evento
	return eventRegister[ID_evento];
	
}
