
#ifndef __FIFO_H__
#define __FIFO_H__

#include <inttypes.h>

#include "gpio_hal.h"


#define NO_HAY_EVENTO_A_PROCESAR 0
#define HAY_EVENTO_A_PROCESAR 1

#define NO_HAY_OVERFLOW 0
#define HAY_OVERFLOW 1


// Tama�o de la cola
#define FIFO_SIZE 32


void FIFO_inicializar(GPIO_HAL_PIN_T newOverflowPin, GPIO_HAL_PIN_BITS_T newOverflowPinNumber);

void FIFO_encolar(uint8_t ID_evento, uint32_t auxData);

uint8_t FIFO_extraer(uint8_t *ID_evento, uint32_t *auxData);

uint32_t FIFO_estadisticas(uint8_t ID_evento);

#endif
