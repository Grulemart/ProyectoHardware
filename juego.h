#ifndef __JUEGO_H__
#define __JUEGO_H__

#include <inttypes.h>

void conecta_K_visualizar_tiempo(void);
void juego_inicializar(void(*funcion_encolar_evento)());
void conecta_K_visualizar_tablero(void);
void uint64ToAsciiArray(uint64_t value, char asciiArray[9]);
void juego_tratar_evento(uint8_t ID_evento, uint32_t auxData);


#endif
