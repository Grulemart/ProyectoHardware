#ifndef __JUEGO_H__
#define __JUEGO_H__

#include <inttypes.h>



#define COMANDO_NO_RECONOCIDO 0
#define COMANDO_NO_VALIDO 1
#define JUGADA_NO_VALIDA 2

void juego_tratar_comando(char comando[3]);
void juego_alarma(void);
void juego_terminar_partida(void);
void juego_cancelar_jugada(void);

	
void juego_mostrar_instrucciones(void);

#endif
