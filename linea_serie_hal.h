#ifndef __LINEA_SERIE_HAL_H__
#define __LINEA_SERIE_HAL_H__
#include "LPC210x.H"
#include <inttypes.h>


void uart0_iniciar(void (*function_callback_recibir)(char c), void (*function_callback_enviar)(void));
void uart0_enviar_caracter(char c);


#endif
