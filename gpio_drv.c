
#include "gpio_drv.h"

void gpio_drv_iniciar() {
	gpio_hal_iniciar();
}

void gpio_drv_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, gpio_hal_pin_dir_t direccion) {
	gpio_hal_sentido(gpio_inicial, num_bits, direccion);
}

uint32_t gpio_drv_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits) {
	return gpio_hal_leer(gpio_inicial, num_bits);
}

void gpio_drv_escribir(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, uint32_t valor) {
	gpio_hal_escribir(gpio_inicial, num_bits, valor);
}