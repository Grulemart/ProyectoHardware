
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
	POWER_DOWN = 11,
	EV_RX_SERIE = 12,
	EV_TX_SERIE = 13,
	HACER_JUGADA = 14,
	NUM_EVENTS = 15
} EVENTO_T;

#endif
