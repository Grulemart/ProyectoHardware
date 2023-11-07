
#include "visualizar.h"

void visualizar_inicializar(void) {
	gpio_hal_sentido(GPIO_VISUALIZAR, GPIO_VISUALIZAR_BITS, GPIO_HAL_PIN_DIR_OUTPUT);
}

void visualizar(uint8_t cantidad) {
	gpio_hal_escribir(GPIO_VISUALIZAR, GPIO_VISUALIZAR_BITS, (uint32_t)cantidad);
}
