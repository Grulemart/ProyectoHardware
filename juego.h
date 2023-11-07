
#ifndef __JUEGO_H__
#define __JUEGO_H__

#include <inttypes.h>
#include "fifo.h"
#include "evento.h"
#include "timer_drv.h"

void juego_inicializar(void);

void juego_tratar_evento(EVENTO_T ID_evento, uint32_t auxData);


#endif
