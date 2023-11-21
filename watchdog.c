
#include "watchdog.h"

void WD_hal_inicializar(uint32_t sec) {
	WDTC = sec;
	WDMOD = 3; // Activar enable y reset
	
	WD_hal_feed(); // Alimentar watchdog por primera vez
}

void WD_hal_feed(void) {
	lock();
	WDFEED = 0xAA;
	WDFEED = 0x55;
	unlock();
}

void WD_hal_test(void) {
	while(1);
}
