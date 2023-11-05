
#ifndef __ALARMA_H__
#define __ALARMA_H__

#include "evento.h"
#include "timer_drv.h"
#include "fifo.h"

#define MAX_ALARMAS 4

void alarma_inicializar();

// El retardo codifica en el
// bit de más peso si la alarma es periódica (debe repetirse), los restantes bits
// indican el retardo en milisegundos
void alarma_activar(enum EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData);

// Procesar interrupciones y encolar eventos
void alarma_tratar_evento();

#endif