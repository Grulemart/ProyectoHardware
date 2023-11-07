
#ifndef __POWER_HAL_H__
#define __POWER_HAL_H__

#include <LPC210x.H>                       /* LPC210x definitions */
#include "alarma.h"

#define USUARIO_AUSENTE 0x800007D0 // 12s

void power_hal_wait(void);
void power_hal_deep_sleep(void);

#endif
