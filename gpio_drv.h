
#ifndef __GPIO_DRV_H__
#define __GPIO_DRV_H__

#include <inttypes.h>
#include "gpio_hal.h"

void gpio_drv_iniciar();
void gpio_drv_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, gpio_hal_pin_dir_t direccion);
uint32_t gpio_drv_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits);
void gpio_drv_escribir(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, uint32_t valor);

#endif