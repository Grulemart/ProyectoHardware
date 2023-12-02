
#ifndef __TIMER_DRV_H__
#define __TIMER_DRV_H__

#include <stdint.h>
#include "evento.h"

void temporizador_drv_iniciar(void);
void temporizador_drv_empezar(void);
uint64_t temporizador_drv_leer(void);
uint64_t temporizador_drv_parar(void);
void temporizador_drv_reloj(uint32_t periodo, void(*funcion_encolar_evento)(), EVENTO_T ID_evento);


#endif
