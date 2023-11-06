#include "botones.h"

static int estado_boton1;
static int estado_boton2;


void iniciar_botones(void){
	iniciar_ext1(BOTON_1);
	iniciar_ext2(BOTON_2);
	estado_boton1 = NO_PULSADO;
	estado_boton2 = NO_PULSADO;
	
}

void pulsar_boton(uint8_t id){
	
	if (id == BOTON_1 && estado_boton1 == NO_PULSADO) {
		FIFO_encolar(EINT1, 0);
	} else if (id == BOTON_2 && estado_boton2 == NO_PULSADO) {
		FIFO_encolar(EINT2, 0);
	}
	
	alarma_activar(DEJAR_BOTON, 100, (uint32_t)id);
	
}

enum BOOLEAN sigue_pulsado(uint8_t id){
	if(id == BOTON_1){
		if(eint1_activada()){
			return TRUE;
		}else{
			estado_boton1 = NO_PULSADO;
			return FALSE;
		}
	}
	if(eint2_activada()){
		return TRUE;
	}else{
		estado_boton2 = NO_PULSADO;
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
