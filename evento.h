
#ifndef __EVENTO_H__
#define __EVENTO_H__

// Declaracion de los ID de los eventos de interrupción
enum EVENTO_T {
	VOID = 0,
	TIMER0 = 1,
	TIMER1 = 2,
	GPIO = 3,
	ALARMA = 4,
	ALARMA_OVERFLOW = 5,
	EINT1 = 6,
	EINT2 = 7,
	DEJAR_BOTON = 8,
	VISUALIZAR_CUENTA = 9,
	LATIDO = 10,
	VISUALIZAR_HELLO = 11
};

// Declaracion de tipo Boolean
enum BOOLEAN {
	TRUE = 1,
	FALSE = 0
};

#endif
