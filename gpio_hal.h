#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include "io_reserva.h"

void gpio_hal_iniciar(void);
void gpio_hal_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, gpio_hal_pin_dir_t direccion);
uint32_t gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits);
void gpio_hal_escribir(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, uint32_t valor);

#endif // GPIO_HAL_H
