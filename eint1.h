#ifndef __EINT1_H__
#define __EINT1_H__

#include "fifo.h"

void eint1_init(void);

unsigned int eint1_read_count(void);

int eint1_read_nueva_pulsacion(void);
void eint1_clear_nueva_pulsacion(void);

int eint1_sigue_pulsado(void);

void eint1_habilitar_interrupciones(void);

#endif