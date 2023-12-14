#ifndef __JUEGO_H__
#define __JUEGO_H__

#include <inttypes.h>



#define COMANDO_NO_RECONOCIDO 0
#define COMANDO_NO_VALIDO 1
#define JUGADA_NO_VALIDA 2

char* conecta_K_visualizar_tiempo(uint32_t tiempo);
void juego_inicializar(void);
void uint64ToAsciiArray(uint64_t value, char asciiArray[9]);
void juego_tratar_comando(char comando[3]);
void juego_transmision_realizada(void);
void juego_alarma(void);
void cancelar_jugada(void);

void conecta_K_test_cargar_tablero(void);
void conecta_K_vacio_cargar_tablero(void);
	
void juego_mostrar_instrucciones(void);

#endif
