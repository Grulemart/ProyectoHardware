
#include "fifo.h"

#define NO_HAY_EVENTO_A_PROCESAR 0
#define HAY_EVENTO_A_PROCESAR 1

static enum EVENTO_T fifo[FIFO_SIZE];				// Dirección de memoria de vector de eventos registrados
static uint32_t auxDataArray[FIFO_SIZE];		// Array de datos auxiliares de los eventos
static enum BOOLEAN procesado[FIFO_SIZE];		// Array de registro de eventos procesados
static uint32_t eventRegister[EVENT_TYPES];	// Registro de numero de eventos de un tipo producidos
static uint8_t indiceUltimoEncolado;										  // Indice de ultimo evento registrado
static uint8_t indiceProcesoATratar;						// Indice para registrar eventos procesados
static GPIO_HAL_PIN_T overflowPin;


void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow) {
	uint8_t i;
	indiceUltimoEncolado = 0;
	indiceProcesoATratar = 0;
	overflowPin = pin_overflow;
	
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
	if(procesado[indiceUltimoEncolado] == FALSE){
		// Enciende led de overflow
		gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1);
	}else{
		fifo[indiceUltimoEncolado] = ID_evento;
		procesado[indiceUltimoEncolado] = FALSE;
		auxDataArray[indiceUltimoEncolado] = auxData;
		
		eventRegister[(uint8_t)ID_evento] += 1;
		eventRegister[VOID] += 1;
		
		indiceUltimoEncolado = (indiceUltimoEncolado + 1) % FIFO_SIZE;
	}
		
}

uint8_t FIFO_extraer(enum EVENTO_T *ID_evento, uint32_t *auxData) {
	if (procesado[indiceProcesoATratar] == TRUE) {
		return NO_HAY_EVENTO_A_PROCESAR;
	} 
	// Hay eventos a procesar
	*ID_evento = fifo[indiceProcesoATratar];
	*auxData = auxDataArray[indiceProcesoATratar];
	procesado[indiceProcesoATratar] = TRUE;
	indiceProcesoATratar = (indiceProcesoATratar + 1) % FIFO_SIZE;
	return HAY_EVENTO_A_PROCESAR;
	
}
	
void FIFO_procesar_evento(void) {
	procesado[indiceProcesoATratar] = TRUE;
	indiceProcesoATratar++;
}

uint32_t FIFO_estadisticas(uint8_t ID_evento) {
	
	return eventRegister[ID_evento];
	
}
