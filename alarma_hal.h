
#ifndef __ALARMA_HAL_H__
#define __ALARMA_HAL_H__

#include <stdint.h>
#include <LPC210X.H>    // LPC21XX Peripheral Registers

typedef void (*CallBack)(void);

void temporizador_hal_reloj (uint32_t periodo, CallBack funcion_callback);

void iniciar_temporizador_hal_alarma (uint32_t periodo);

#endif