
#include "int_externas_hal.h"
#include <LPC210X.H>
#include "botones.h"

#define TRUE 1
#define FALSE 0

static int id_boton_eint1;
static int id_boton_eint2;

static uint8_t idEventoBotonPulsado;
static uint8_t idEventoMonitorizarBoton;

void eint1_ISR (void) __irq {
	
	// Disable the EXTINT1 Interrupt
	VICIntEnClr = 0x00008000;
	EXTINT = EXTINT | 0x2;
	pulsar_boton(id_boton_eint1, idEventoBotonPulsado, idEventoMonitorizarBoton);
	VICVectAddr = 0;
}

void eint2_ISR (void) __irq{
	
	// Disable the EXTINT2 Interrupt
	VICIntEnClr=0x00010000;
	EXTINT = EXTINT |0x4;
	pulsar_boton(id_boton_eint2, idEventoBotonPulsado, idEventoMonitorizarBoton);
	VICVectAddr = 0;
}

// Inicialización de variables de eventos
void iniciar_id_evento(uint8_t _idEventoBotonPulsado, uint8_t _idEventoMonitorizarBoton) {
	idEventoBotonPulsado = _idEventoBotonPulsado;
	idEventoMonitorizarBoton = _idEventoMonitorizarBoton;
}

// Inicializa los registros de eint1 con los parámetros necesarios
void iniciar_ext1(uint32_t id){
	id_boton_eint1 = id;
	EXTINT = EXTINT | 0x2; // Clear interrupt flag
	EXTWAKE = EXTWAKE | 0X2;	 // Awake the processor when interrupt
	VICVectAddr2 = (unsigned long)eint1_ISR;
	// Set P0.14 to EINT1
	PINSEL0 &= ~(1 << 28); // Set the bit 28 to 0
	PINSEL0 |= (1 << 29); // Set the bit 29 to 1
	VICVectCntl2 = 0x20 | 15; // 0x20 bit 5 enables vectored IRQs 15 is the number of the interrupt eint1
	VICIntEnable = VICIntEnable |  0x00008000;	// Enable EXTINT1 Interrupt
}

// Inicializa los registros de eint2 con los parámetros necesarios
void iniciar_ext2(uint32_t id){
	id_boton_eint2 = id;
	EXTINT = EXTINT | 0x4; // Clear interrupt flag
	EXTWAKE = EXTWAKE | 0X4; // Awake the processor when interrupt
	VICVectAddr3 = (unsigned long)eint2_ISR;
	// Set P0.15 to EINT2
	PINSEL0 &= ~(1U << 30); // Set the bit 30 to 0
	PINSEL0 |= (1U << 31); // Set the bit 31 to 1
	
	VICVectCntl3 = 0x20 | 16; // 0x20 bit 5 enables vectored IRQs 15 is the number of the interrupt eint1
	VICIntEnable = VICIntEnable |  0x00010000;	// Enable EXTINT1 Interrupt
}

// Comprueba si se sigue produciendo una interrupción externa de tipo eint1
int eint1_activada(void){
	int bit;
	// Hacemos clear del bit 2 si el pin esta en low level no debería dejar hacer el clear
	EXTINT |= 0x2; // 
	// Cogemos el bit 2 que corresponde a eint2
	bit = (EXTINT >> 1) & 1;
	// Si bit sigues estando activado 1 entonces el pin sigue en low level
	if(bit == 1){
		return TRUE;
	}
	return FALSE;
}

// Comprueba si se sigue produciendo una interrupción externa de tipo eint2
int eint2_activada(void) {
	int bit;
	// Hacemos clear del bit 3 si el pin esta en low level no debería dejar hacer el clear
	EXTINT |= 0x4; // 
	
	// Cogemos el bit 2 que corresponde a eint2
	bit = (EXTINT >> 2) & 1;
	// Si bit sigues estando activado 1 entonces el pin sigue en low level
	if(bit == 1){
		return TRUE;
	}
	return FALSE;
}

// Habilita la interrupción eint1
void habilitar_interrupcion_eint1(void){
	VICVectCntl2 = 0x20 | 15; // 0x20 bit 5 enables vectored IRQs 15 is the number of the interrupt eint1
	VICIntEnable = VICIntEnable |  0x00008000;	// Enable EXTINT1 Interrupt
}

// Habilita la interrupción eint2
void habilitar_interrupcion_eint2(void){
	VICVectCntl3 = 0x20 | 16; // 0x20 bit 5 enables vectored IRQs 16 is the number of the interrupt eint2
	VICIntEnable = VICIntEnable |  0x00010000;	// Enable EXTINT1 Interrupt
}





