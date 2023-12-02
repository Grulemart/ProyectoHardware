
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

#include "gpio_hal.h"

void hello_world_inicializar(GPIO_HAL_PIN_T bit, GPIO_HAL_PIN_BITS_T pinBits, uint8_t evento);

void hello_world_tick_tack(void);

#endif
