#include "semaforo_interrupciones.h"

static uint32_t recover;

void bloquear_interrupciones() {
  recover = VICIntEnable;
  VICIntEnClr = UINT32_MAX;
}

void liberar_interrupciones() { 
	VICIntEnable = recover; 
}