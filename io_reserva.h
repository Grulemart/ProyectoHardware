
#ifndef __IO_RESERVA_H__
#define __IO_RESERVA_H__

#include <inttypes.h>

// Enumeración para la dirección del pin
typedef enum {
    GPIO_HAL_PIN_DIR_INPUT,
    GPIO_HAL_PIN_DIR_OUTPUT
} gpio_hal_pin_dir_t;

// Tipo de datos para representación del pin GPIO
typedef uint8_t  GPIO_HAL_PIN_T;

#define GPIO_OVERFLOW 32
#define GPIO_OVERFLOW_BITS 1

////TODO: Añadir más definiciones de resto de pins

#endif