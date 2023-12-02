
#include "fifo.h"
#include "desactivar_interrupciones.h"

#define TRUE 0
#define FALSE 1

#define MAX_NUM_EVENTOS 32

static volatile uint8_t fifo[FIFO_SIZE];					// Dirección de memoria de vector de eventos registrados
static volatile uint32_t auxDataArray[FIFO_SIZE];			// Array de datos auxiliares de los eventos
static volatile uint8_t procesado[FIFO_SIZE];			// Array de registro de eventos procesados
static volatile uint32_t eventRegister[MAX_NUM_EVENTOS];		// Registro de numero de eventos de un tipo producidos
static volatile uint8_t indiceUltimoEncolado;					// Indice de ultimo evento registrado
static volatile uint8_t indiceProcesoATratar;					// Indice para registrar eventos procesados
static volatile GPIO_HAL_PIN_T overflowPin;						// Pin de overflow
static volatile GPIO_HAL_PIN_BITS_T overflowPinNumber;	// Numero de pines de overflow

void FIFO_inicializar(GPIO_HAL_PIN_T newPinOverflow, GPIO_HAL_PIN_BITS_T newOverflowPinNumber) {
	
	// Inicialización de variables
	uint8_t i;
	indiceUltimoEncolado = 0;
	indiceProcesoATratar = 0;
	
	overflowPin = newPinOverflow;
	overflowPinNumber = newOverflowPinNumber;
	gpio_hal_sentido(overflowPin, overflowPinNumber, GPIO_HAL_PIN_DIR_OUTPUT);
	gpio_hal_escribir(overflowPin, overflowPinNumber, NO_HAY_OVERFLOW);
	
	for (i = 0; i < FIFO_SIZE; i++) {
		fifo[i] = 0;
		procesado[i] = TRUE;
		auxDataArray[i] = 0;
	}
	
	for (i = 0; i < MAX_NUM_EVENTOS; i++) {
		eventRegister[i] = 0;
	}
	
}

void FIFO_encolar(uint8_t ID_evento, uint32_t auxData) {
	lock();
	// Se produce overflow
	if(procesado[indiceUltimoEncolado] == FALSE){
		// Enciende led de overflow
		gpio_hal_escribir(overflowPin, overflowPinNumber, HAY_OVERFLOW);
	}else{
		// Evento se encola como no procesado
		fifo[indiceUltimoEncolado] = ID_evento;
		procesado[indiceUltimoEncolado] = FALSE;
		auxDataArray[indiceUltimoEncolado] = auxData;
		
		// Actualización de indices
		eventRegister[ID_evento] += 1;
		eventRegister[0] += 1;
		
		indiceUltimoEncolado = (indiceUltimoEncolado + 1) % FIFO_SIZE;
	}
	unlock();
}

uint8_t FIFO_extraer(uint8_t *ID_evento, uint32_t *auxData) {
	lock();
	if (procesado[indiceProcesoATratar] == TRUE) {
		unlock();
		return NO_HAY_EVENTO_A_PROCESAR;
	} 
	// Hay eventos a procesar y se pasan por parámetros
	*ID_evento = fifo[indiceProcesoATratar];
	*auxData = auxDataArray[indiceProcesoATratar];
	procesado[indiceProcesoATratar] = TRUE;
	indiceProcesoATratar = (indiceProcesoATratar + 1) % FIFO_SIZE;
	unlock();
	return HAY_EVENTO_A_PROCESAR;
}

uint32_t FIFO_estadisticas(uint8_t ID_evento) {
	// Se devuelve el numero de eventos registrados de ID_evento
	return eventRegister[ID_evento];
	
}
