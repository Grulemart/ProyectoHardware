
#ifndef __ALARMA_H__
#define __ALARMA_H__

#include "evento.h"
#include "timer_drv.h"
#include "fifo.h"

#define MAX_ALARMAS 8

void alarma_inicializar(void);

// El retardo codifica en el
// bit de más peso si la alarma es periódica (debe repetirse), los restantes bits
// indican el retardo en milisegundos
void alarma_activar(EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData);

void cancelar_alarma(EVENTO_T ID_evento, uint32_t auxData);

void reprogramar_alarma(EVENTO_T ID_evento, uint32_t auxData);

// Procesar interrupciones y encolar eventos
void alarma_tratar_evento(void);

#endif
