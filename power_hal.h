
#ifndef __POWER_HAL_H__
#define __POWER_HAL_H__

#include <LPC210x.H>                       /* LPC210x definitions */
#include "alarma.h"

#define USUARIO_AUSENTE 0x8000BB20 // Tiempo a transcurrir entre inactividad para activar power-down mode (12 s)

void power_hal_wait(void);
void power_hal_deep_sleep(void);

#endif
