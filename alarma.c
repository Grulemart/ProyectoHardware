
#include "alarma.h"
#include "timer_drv.h"

#define PERIODO_TIMER1 4000	// Programar TIMER1 para que interrumpa cada 1 ms

#define TRUE 1
#define FALSE 0

static volatile uint8_t alarmasActivas[MAX_ALARMAS];					// Alarmas activas
static volatile uint32_t alarmaRetardoInicial[MAX_ALARMAS];		// Retardo incial proporcionado para reprogramar alarmas
static volatile uint32_t alarmaEnd[MAX_ALARMAS];							// Tick de expiracion de las alarmas
static volatile uint32_t alarmaReprogramar[MAX_ALARMAS]; // Reprogramacion de alarmas
static volatile uint8_t alarmaEvento[MAX_ALARMAS];			// Evento a encolar tras expiracion
static volatile uint32_t alarmaAuxData[MAX_ALARMAS];					// Datos auxiliares del evento a encolar

static volatile uint32_t ticksAlarma;	// Ticks totales transcurridos desde inicializacion
static uint8_t idEventoAlarmaOverflow;

static void (*funcionEncolarEvento)(uint8_t, uint32_t);

// Inicializa las variables e interrupciones necesarias para el modulo alarma
void alarma_inicializar(uint8_t idEventoAlarma, uint8_t _idEventoAlarmaOverflow, void(*funcion_encolar_evento)(uint8_t, uint32_t)) {
	
	ticksAlarma = 0;
	funcionEncolarEvento = funcion_encolar_evento;
	idEventoAlarmaOverflow = _idEventoAlarmaOverflow;
	// Inicializar temporizador con interrupcion a 1ms 
	// Cada 1ms el timer1 encolara el evento ALARMA en la cola FIFO
	temporizador_drv_reloj(PERIODO_TIMER1, funcion_encolar_evento, idEventoAlarma);

}

// Activa una nueva alarma
// Si no hay espacio para más alarmas se produce overflow y se muestra en el pin de la GPIO
void alarma_activar(uint8_t ID_evento, uint32_t retardo, uint32_t auxData) {
	uint8_t i;
	uint32_t mascara;
	uint32_t retardoReal;
	// Tratamos el cancelamiento de una alarma
	if (retardo == 0) { // Si retardo es igual a 0 cancelar la alarma
		for(i = 0; i < MAX_ALARMAS; i++){
			if(alarmasActivas[i] == TRUE){
				if(alarmaEvento[i] == ID_evento && alarmaAuxData[i] == auxData){
					alarmasActivas[i] = FALSE;
					break;
				}
			}
		}
		return;
	}
	
	// Iterar sobre el array de alarmas hasta encontrar una disponible
	
	for (i = 0; i < MAX_ALARMAS; i++) {
		if (alarmasActivas[i] == FALSE) {
			// Iniciar alarma
			alarmasActivas[i] = TRUE;
			mascara = (1U << 31) - 1U; 
			retardoReal = retardo & mascara;
			alarmaRetardoInicial[i] = retardoReal;
			alarmaEnd[i] = retardoReal + ticksAlarma;
			alarmaReprogramar[i] = (retardo >> 31);
			alarmaEvento[i] = ID_evento;
			alarmaAuxData[i] = auxData;
			return;
		}
	}
	
	// Si no hay alarmas disponibles se encola evento ALARMA_OVERFLOW
	
	funcionEncolarEvento(idEventoAlarmaOverflow, 0);
	return;	
}

// Reprograma la primera alarma con ID_evento y auxData que encuentra en el array
// En este caso si se requiere de una alarma cancelable se vale del auxData para reconocerla
void alarma_reprogramar(uint8_t ID_evento, uint32_t auxData) {
	int i;
	for ( i = 0; i < MAX_ALARMAS; i++) {
		if (alarmaEvento[i] == ID_evento && alarmaAuxData[i] == auxData && alarmasActivas[i] == TRUE) {
			alarmaEnd[i] = ticksAlarma + alarmaRetardoInicial[i];
			return;
		}
	}
}

// Procesa un tick y trata las alarmas que han expirado, encolando sus respectivos eventos en la cola FIFO
void alarma_tratar_evento(void) {
	uint8_t i;
	// Aumentar numero de ciclos
	ticksAlarma++;
	
	// Comprobar que han expirado las alarmas
	for(i = 0; i < MAX_ALARMAS; i++){
		if(alarmasActivas[i] == TRUE && alarmaEnd[i] <= ticksAlarma){
			if(alarmaReprogramar[i] == TRUE){
				alarmaEnd[i] = ticksAlarma + alarmaRetardoInicial[i];
			}else{
				alarmasActivas[i] = FALSE;
			}
			funcionEncolarEvento(alarmaEvento[i], alarmaAuxData[i]);
		}
	}
}
