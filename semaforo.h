
#ifndef __SEMAFORO_H__
#define __SEMAFORO_H__

#include "llamadas_swi.h"
#include "power_hal.h"

#define IRQ_DESACTIVADA 1

void lock(void);
void unlock(void);

#endif
