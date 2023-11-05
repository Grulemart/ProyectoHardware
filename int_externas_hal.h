#ifndef __INT_EXTERNAS_HAL_H__
#define __INT_EXTERNAS_HAL_H__
#include <LPC210X.H>
#include "botones.h"


void iniciar_ext1(int id);
void iniciar_ext2(int id);
int eint1_activada(void);
int eint2_activada(void);
void habilitar_interrupcion_eint1(void);
void habilitar_interrupcion_eint2(void);


#endif
