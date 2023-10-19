
#ifndef __TIMER_HAL_H__
#define __TIMER_HAL_H__

#include <stdint.h>
#include <LPC210X.H>    // LPC21XX Peripheral Registers
void temporizador_hal_iniciar();
void temporizador_hal_empezar();
uint64_t temporizador_hal_leer();
uint64_t temporizador_hal_parar();
extern const uint64_t temporizador_hal_ticks2us;

#endif