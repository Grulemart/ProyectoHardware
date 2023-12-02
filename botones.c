
#include "botones.h"
#include "int_externas_hal.h"
#include "alarma.h"

#define TRUE 0
#define FALSE 1

const uint32_t RETARDO_MONITORIZAR_BOTON = 100;

static void (*funcionEncolarEvento)();

void iniciar_botones(void(*funcion_encolar_evento)(), uint8_t idEventoBotonPulsado, uint8_t idEventoMonitorizarBoton){
	funcionEncolarEvento = funcion_encolar_evento;
	iniciar_id_evento(idEventoBotonPulsado, idEventoMonitorizarBoton);
	iniciar_ext1(BOTON_1);
	iniciar_ext2(BOTON_2);
}

void pulsar_boton(uint8_t id, uint8_t idEventoBotonPulsado, uint8_t idEventoMonitorizarBoton){
	uint32_t retardo = RETARDO_MONITORIZAR_BOTON;
	retardo |= 1U << 31; // Ponemos un 1 en el bit de mayor peso para que se repita la alarma 
	funcionEncolarEvento(idEventoBotonPulsado,(uint32_t) id);
	alarma_activar(idEventoMonitorizarBoton, retardo , (uint32_t)id);
}

uint8_t sigue_pulsado(uint8_t id){
	if(id == BOTON_1){
		if(eint1_activada()){
			return TRUE;
		}else{
			return FALSE;
		}
	}
	if(eint2_activada()){
		return TRUE;
	}else{
		return FALSE;
	}
}

void habilitar_interrupcion(uint8_t id){
	if(id == BOTON_1){
		habilitar_interrupcion_eint1();
	}else{
		habilitar_interrupcion_eint2();
	}
}
