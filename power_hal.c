#include "power_hal.h"

// Pone al procesador en estado idle
void power_hal_wait (void)  {
	PCON |= 0x1; 
}

// Pone al procesador en estado power_down
void power_hal_deep_sleep(void) {
	PCON |= 0x3;
}
