#ifndef __LINEA_SERIE_DRV_H__
#define __LINEA_SERIE_DRV_H__

#include "linea_serie_hal.h"
#include <inttypes.h>

#define ESTADO_ESPERANDO_INICIO 1
#define ESTADO_RECIBIENDO_TRAMA 2
#define START_DELIMETER '$'
#define END_DELIMETER '!'
#define SEND_BUFFER_SIZE 200
#define AUX_DATA_COMANDO_TERMINADO 18

#define TRUE 0
#define FALSE 1

void iniciar_linea_serie(uint8_t _idEvento, void(*funcion_encolar_evento)(), uint8_t _gpio_serie_error);
void linea_serie_drv_enviar_array(char* array);


#endif
