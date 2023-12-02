
#ifndef __VISUALIZAR_H__
#define __VISUALIZAR_H__

#include "gpio_hal.h"
#include <inttypes.h>

void visualizar_inicializar(GPIO_HAL_PIN_T _gpio_visualizar, GPIO_HAL_PIN_BITS_T _gpio_visualizar_bits);

void visualizar(uint8_t cantidad);

#endif
