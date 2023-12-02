
#include "visualizar.h"

static GPIO_HAL_PIN_T gpio_visualizar;
static GPIO_HAL_PIN_BITS_T gpio_visualizar_bits;

void visualizar_inicializar(GPIO_HAL_PIN_T _gpio_visualizar, GPIO_HAL_PIN_BITS_T _gpio_visualizar_bits) {
	gpio_visualizar = _gpio_visualizar;
	gpio_visualizar_bits = _gpio_visualizar_bits;
	gpio_hal_sentido(gpio_visualizar, gpio_visualizar_bits, GPIO_HAL_PIN_DIR_OUTPUT);
}

void visualizar(uint8_t cantidad) {
	gpio_hal_escribir(gpio_visualizar, gpio_visualizar_bits, (uint32_t)cantidad);
}
