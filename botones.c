#include "botones.h"

static int estado_boton1;
static int estado_boton2;


void iniciar_botones(void){
	iniciar_ext1(BOTON_1);
	iniciar_ext2(BOTON_2);
	estado_boton1 = NO_PULSADO;
	estado_boton2 = PULSADO;
	
}

void pulsar_boton(int id){
	FIFO_encolar(BOTON_PULSADO, id);
	// TODO: Crear alarma de 100ms para monitorizar cuando el boton se ha dejado de pulsar
	
	
	
}

int sigue_pulsado(int id){
	if(id == BOTON_1){
		if(eint1_activada()){
			return 1;
		}else{
			estado_boton1 = NO_PULSADO;
			return 0;
		}
	}
	if(eint2_activada()){
		return 1;
	}else{
		estado_boton2 = NO_PULSADO;
		return 0;
	}
}

void habilitar_interrupcion(int id){
	if(id == BOTON_1){
		habilitar_interrupcion_eint1();
	}else{
		habilitar_interrupcion_eint2();
	}
}
