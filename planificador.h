#ifndef __PLANIFICADOR_H__
#define __PLANIFICADOR_H__

#include "fifo.h"
#include "timer_drv.h"
#include "hello_world.h"
#include <inttypes.h>
#include "power_hal.h"
#include "alarma.h"
#include "botones.h"
#include "juego.h"
#include "visualizar.h"
#include "alarma.h"
#include "linea_serie_drv.h"
#include "watchdog.h"

#define WATCHDOG_TIME 0x1000000

#define ESTADO_ESPERANDO_COMANDO 0
#define ESTADO_ESPERANDO_TIEMPO_1 1
#define ESTADO_ESPERANDO_TIEMPO_2 2
#define ESTADO_ESPERANDO_TABLERO 3
#define ESTADO_ESPERANDO_FIN_COMANDO 4


void planificador(void);

#endif
