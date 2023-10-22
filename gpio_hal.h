#ifndef GPIO_HAL_H
#define GPIO_HAL_H
#include <stdint.h>

// Enumeración para la dirección del pin
typedef enum {
    GPIO_HAL_PIN_DIR_INPUT,
    GPIO_HAL_PIN_DIR_OUTPUT
} gpio_hal_pin_dir_t;

// Tipo de datos para representación del pin GPIO
typedef uint8_t  GPIO_HAL_PIN_T;

//// TODO: Añadir estas definiciones en io_reserva.h
#define GPIO_OVERFLOW 32
#define GPIO_OVERFLOW_BITS 1

void gpio_hal_iniciar(void);
void gpio_hal_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, gpio_hal_pin_dir_t direccion);
uint32_t gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits);
void gpio_hal_escribir(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, uint32_t valor);

#endif // GPIO_HAL_H