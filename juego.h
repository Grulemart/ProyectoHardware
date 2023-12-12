#ifndef __JUEGO_H__
#define __JUEGO_H__

#include <inttypes.h>

#define ESPERANDO_INICIO 0
#define ESPERANDO_TX_FIN_COMANDO_1 1
#define ESTADO_ESPERANDO_JUGADA 2
#define ESPERANDO_TX_FIN_COMANDO_2 3
#define ESPERANDO_DECISION_JUGADA 4
#define ESPERANDO_TX_FIN_COMANDO_3 5
#define ESPERANDO_TX_RESULTADOS 6
#define ESPERANDO_FIN_COMANDO_4 7
#define ESPERANDO_TX_ERROR 8
#define ESPERANDO_FIN_COMANDO_5 9
#define ESPERANDO_TX_ERROR2 10

#define COMANDO_NO_RECONOCIDO 0
#define COMANDO_NO_VALIDO 1
#define JUGADA_NO_VALIDA 2

void conecta_K_visualizar_tiempo(void);
void juego_inicializar(void);
void conecta_K_visualizar_tablero(void);
void uint64ToAsciiArray(uint64_t value, char asciiArray[9]);
void juego_tratar_evento(uint8_t ID_evento, uint32_t auxData);
void juego_tratar_comando(char comando[3]);
void juego_trasmision_realizada(void);
void juego_alarma(void);

void conecta_K_test_cargar_tablero(void);
void conecta_K_vacio_cargar_tablero(void);
	
void juego_mostrar_instrucciones(void);

#endif
