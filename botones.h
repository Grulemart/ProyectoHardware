#ifndef __BOTONES_H__
#define __BOTONES_H__
#include "int_externas_hal.h"
#include <stdint.h>
#include "fifo.h"
#include "evento.h"


#define BOTON_1 1
#define BOTON_2 2
#define PULSADO 1
#define NO_PULSADO 2

void iniciar_botones(void);
void pulsar_boton(int id);
int sigue_pulsado(int id);


#endif
