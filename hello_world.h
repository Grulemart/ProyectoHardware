
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

#include "io_reserva.h"
#include "gpio_hal.h"
#include "timer_drv.h"
#include "fifo.h"
#include "alarma.h"

void hello_world_inicializar(GPIO_HAL_PIN_T bit, GPIO_HAL_PIN_BITS_T pinBits);

void hello_world_tick_tack(void);

#endif
