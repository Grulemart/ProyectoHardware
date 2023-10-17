#include <stdint.h>
#include <LPC210X.H>    // LPC21XX Peripheral Registers
void temporizador_hal_iniciar();
void temporizador_hal_empezar();
uint64_t temporizador_hal_leer();
uint64_t temporizador_hal_parar();
