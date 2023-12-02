
#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include <LPC210x.H> 
#include <inttypes.h>


void WD_hal_inicializar(uint32_t sec);

void WD_hal_feed(void);

void WD_hal_test(void);

#endif
