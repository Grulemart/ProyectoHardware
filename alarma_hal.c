
#include "alarma_hal.h"

void timer1_RSI (void) __irq;
static CallBack funcion_alarma;

void iniciar_temporizador_hal_alarma(uint32_t periodo) {
	T1PR = 14;          		// TC se activa cuando PC llega a PR+1 ciclos: 15 ticks = 1 us
	T1MR0 = periodo * 1000;	// Interrumpe cada cada numero de ms especificado por periodo
	T1MCR = 3; 							// Generates an interrupt and resets the count when the value of MR0 is reached
	
	VICVectAddr1 = (unsigned long)timer1_RSI;     // Guarda la dirección de memoria de la subrutina de interrupción
	VICVectCntl1 = 0x20 | 5;                   		// Activa la interrupción en el vector de control, tabla 40 del manual
  VICIntEnable = VICIntEnable | 1; 							// Enable Timer1 Interrupt
}

void temporizador_hal_reloj (uint32_t periodo, CallBack funcion_callback) {
	
	if (periodo == 0) { // Parar alarma si periodo = 0
		T1TCR = 3;
		T1TCR = 0;
		return;
	}
	
	T1MR0 = periodo * 1000;
	funcion_alarma = funcion_callback;
	
}

void timer1_RSI(void) __irq {
	T1IR = 1;					// Resetear interrupción del timer1
	VICVectAddr = 0;  // Resetear dirección de interrupción
	
	//// TODO: Encolar evento en planificador con funcion callback
	
	
}