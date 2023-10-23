
#ifndef __IO_RESERVA_H__
#define __IO_RESERVA_H__

#include <inttypes.h>

// Enumeraci�n para la direcci�n del pin
typedef enum {
    GPIO_HAL_PIN_DIR_INPUT,
    GPIO_HAL_PIN_DIR_OUTPUT
} gpio_hal_pin_dir_t;

// Tipo de datos para representaci�n del pin GPIO
typedef uint8_t  GPIO_HAL_PIN_T;
typedef uint8_t  GPIO_HAL_PIN_BITS_T;

#define GPIO_OVERFLOW 32
#define GPIO_OVERFLOW_BITS 1

#define GPIO_HELLO_WORLD 0
#define GPIO_HELLO_WORLD_BITS 8

#endif