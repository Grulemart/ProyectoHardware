
#ifndef __TIMER_HAL_H__
#define __TIMER_HAL_H__

#include <stdint.h>

#define TICKS_A_US 15;

void temporizador_hal_iniciar(void);
void temporizador_hal_empezar(void);
uint64_t temporizador_hal_leer(void);
uint64_t temporizador_hal_parar(void);
void temporizador_hal_reloj(uint32_t periodo, void (*function_callback)());

#endif
