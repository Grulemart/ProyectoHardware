
#ifndef __EVENTO_H__
#define __EVENTO_H__

// Declaracion de los ID de los eventos de interrupción
typedef enum {
	VOID = 0,
	TIMER0 = 1,
	TIMER1 = 2,
	GPIO = 3,
	ALARMA = 4,
	ALARMA_OVERFLOW = 5,
	BOTON_PULSADO = 6,
	MONITORIZAR_BOTON = 7,
	VISUALIZAR_CUENTA = 8,
	LATIDO = 9,
	VISUALIZAR_HELLO = 10,
	NUM_EVENTS = 11,
} EVENTO_T;

// Declaracion de tipo Boolean
typedef enum{
	TRUE = 1,
	FALSE = 0
} BOOLEAN;

#endif
