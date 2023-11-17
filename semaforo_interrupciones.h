#ifndef __SEMAFORO_INTERRUPCIONES__
#define __SEMAFORO_INTERRUPCIONES__

#include <LPC210x.H>
#include <inttypes.h>

static uint32_t recover;

void bloquear_interrupciones(void);

void liberar_interrupciones(void);

#endif