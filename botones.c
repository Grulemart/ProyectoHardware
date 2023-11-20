#include "botones.h"


const uint32_t RETARDO_MONITORIZAR_BOTON = 100;


void iniciar_botones(void){
	iniciar_ext1(BOTON_1);
	iniciar_ext2(BOTON_2);

	
}

void pulsar_boton(uint8_t id){
	uint32_t retardo = RETARDO_MONITORIZAR_BOTON;
	retardo |= 1U << 31; // Ponemos un 1 en el bit de mayor peso para que se repita la alarma 
	FIFO_encolar(BOTON_PULSADO,(uint32_t) id);
	alarma_activar(MONITORIZAR_BOTON, retardo , (uint32_t)id);
	
}

BOOLEAN sigue_pulsado(uint8_t id){
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
