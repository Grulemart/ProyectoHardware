#ifndef __JUEGO_H__
#define __JUEGO_H__

#include <inttypes.h>
#include "fifo.h"
#include "evento.h"
#include "llamadas_swi.h"
#include "tablero.h"
#include "celda.h"
#include "linea_serie_drv.h"


void conecta_K_visualizar_tiempo(void);
void juego_inicializar(void);
void conecta_K_visualizar_tablero(void);
void uint64ToAsciiArray(uint64_t value, char asciiArray[9]);
void juego_tratar_evento(EVENTO_T ID_evento, uint32_t auxData);


#endif
