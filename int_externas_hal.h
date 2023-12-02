#ifndef __INT_EXTERNAS_HAL_H__
#define __INT_EXTERNAS_HAL_H__

#include <inttypes.h>

void iniciar_ext1(uint32_t id);
void iniciar_ext2(uint32_t id);
void iniciar_id_evento(uint8_t _idEventoBotonPulsado, uint8_t _idEventoMonitorizarBoton);
int eint1_activada(void);
int eint2_activada(void);
void habilitar_interrupcion_eint1(void);
void habilitar_interrupcion_eint2(void);


#endif
