#include <stdint.h>
#include "timer_hal.h"

void temporizador_drv_iniciar();
void temporizador_drv_empezar();
uint64_t temporizador_drv_leer();
uint64_t temporizador_drv_parar();