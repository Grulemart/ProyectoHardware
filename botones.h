#ifndef __BOTONES_H__
#define __BOTONES_H__

#include <stdint.h>

#define BOTON_1 1
#define BOTON_2 2
#define PULSADO 1
#define NO_PULSADO 2

void iniciar_botones(void(*funcion_encolar_evento)(), uint8_t idEventoBotonPulsado, uint8_t idEventoMonitorizarBoton);
void pulsar_boton(uint8_t id, uint8_t idEventoBotonPulsado, uint8_t idEventoMonitorizarBoton);
uint8_t sigue_pulsado(uint8_t id);
void habilitar_interrupcion(uint8_t id);


#endif
