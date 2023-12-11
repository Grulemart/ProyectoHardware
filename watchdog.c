
#include "watchdog.h"
#include "desactivar_interrupciones.h"

// Inicializa los parámetros del watchdog
void WD_hal_inicializar(uint32_t sec) {
	WDTC = sec;
	WDMOD = 3; // Activar enable y reset
	
	WD_hal_feed(); // Alimentar watchdog por primera vez
}

// Reestablece el temporizador del watchdog
// Cuando se esta reestableciendo el temporizador no se pueden producir interrupciones
void WD_hal_feed(void) {
	lock();
	WDFEED = 0xAA;
	WDFEED = 0x55;
	unlock();
}

// Test para comprobar que el temporizador funciona
void WD_hal_test(void) {
	while(1);
}
