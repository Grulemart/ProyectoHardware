
#ifndef __FIFO_H__
#define __FIFO_H__

#include <inttypes.h>

//// TODO: Cambiar esta dependencia a io_reserva cuando esté hecha
#include "gpio_hal.h"
#include "evento.h"

typedef uint8_t BOOLEAN;

// Tamaño de la cola
const uint8_t FIFO_SIZE = 32;
const uint8_t EVENT_TYPES = 4;


void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow);

void FIFO_encolar(enum EVENTO_T ID_evento, uint32_t auxData);

uint8_t FIFO_extraer(enum EVENTO_T *ID_evento, uint32_t *auxData);

uint32_t FIFO_estadisticas(uint8_t ID_evento);

#endif