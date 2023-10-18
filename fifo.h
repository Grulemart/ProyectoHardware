
#ifndef __FIFO_H__
#define __FIFO_H__

#include <inttypes.h>

enum BOOLEAN {
	TRUE = 1,
	FALSE = 0
};

// Tamaño de la cola
const uint8_t FIFO_SIZE = 32;
const uint8_t EVENT_TYPES = 4;

// Declaracion de los ID de los eventos de interrupción
enum ID_EVENTO {
	VOID = 0,
	TIMER0 = 1,
	TIMER1 = 2,
	GPIO = 3
};

// Declaración de tipo EVENTO_T
typedef struct EVENTO_T {
	
	uint8_t ID;
	uint8_t procesado;
	
} EVENTO_T;

//// TODO: Cambiar tipo a GPIO_HAL_PIN_T cuando este declarado
void FIFO_inicializar(uint8_t pin_overflow);

void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData);

uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData);

uint32_t FIFO_estadisticas(uint8_t ID_evento);

#endif