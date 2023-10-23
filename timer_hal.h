
#ifndef __TIMER_HAL_H__
#define __TIMER_HAL_H__

#include <stdint.h>
#include <LPC210X.H>    // LPC21XX Peripheral Registers
#define temporizador_hal_ticks2us 15;
void temporizador_hal_iniciar(void);
void temporizador_hal_empezar(void);
uint64_t temporizador_hal_leer(void);
uint64_t temporizador_hal_parar(void);
void temporizador_hal_reloj(uint32_t periodo, void (*function_callback)());

#endif
