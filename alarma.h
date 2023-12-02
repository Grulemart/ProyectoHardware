
#ifndef __ALARMA_H__
#define __ALARMA_H__

#include <inttypes.h>

#define MAX_ALARMAS 8
#define CANCELAR_ALARMA 0

void alarma_inicializar(void(*funcion_encolar_evento)());

// El retardo codifica en el
// bit de más peso si la alarma es periódica (debe repetirse), los restantes bits
// indican el retardo en milisegundos
void alarma_activar(uint8_t ID_evento, uint32_t retardo, uint32_t auxData);

void alarma_reprogramar(uint8_t ID_evento, uint32_t auxData);

// Procesar interrupciones y encolar eventos
void alarma_tratar_evento(void);

#endif
