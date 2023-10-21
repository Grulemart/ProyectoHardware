
#ifndef __ALARMA_DRV_H__
#define __ALARMA_DRV_H__

#include <stdint.h>
#include <LPC210X.H>    // LPC21XX Peripheral Registers

#include "alarma_hal.h"

//// TODO: Cambiar esta dependencia a planificador cuando esté hecho
#include "fifo.h"

void temporizador_drv_reloj (uint32_t periodo, CallBack funcion_callback, enum EVENTO_T ID_evento);

void iniciar_temporizador_drv_alarma (uint32_t periodo);

#endif