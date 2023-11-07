#include "int_externas_hal.h"

static int id_boton_eint1;
static int id_boton_eint2;


void eint1_ISR (void) __irq {
	EXTINT = EXTINT | 0x2;
	VICVectAddr = 0;
	// Disable the EXTINT1 Interrupt
	VICVectCntl2 = 0;
	VICIntEnable &= ~(0x00008000);
	pulsar_boton(id_boton_eint1);
}

void eint2_ISR (void){
	EXTINT = EXTINT |0x4;
	VICVectAddr = 0;
	// Disable the EXTINT2 Interrupt
	VICVectCntl3 = 0;
	VICIntEnable &= ~(0x00010000);
	pulsar_boton(id_boton_eint2);
}

void iniciar_ext1(int id){
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

void iniciar_ext2(int id){
	id_boton_eint2 = id;
	EXTINT = EXTINT | 0x4; // Clear interrupt flag
	EXTWAKE = EXTWAKE | 0X4; // Awake the processor when interrupt
	VICVectAddr3 = (unsigned long)eint2_ISR;
	// Set P0.15 to EINT2
	PINSEL0 &= ~(1 << 30); // Set the bit 30 to 0
	PINSEL0 |= (1 << 31); // Set the bit 31 to 1
	
	VICVectCntl3 = 0x20 | 16; // 0x20 bit 5 enables vectored IRQs 15 is the number of the interrupt eint1
	VICIntEnable = VICIntEnable |  0x00010000;	// Enable EXTINT1 Interrupt
}

int eint1_activada(void){
	int bit;
	// Hacemos clear del bit 2 si el pin esta en low level no deber�a dejar hacer el clear
	EXTINT |= 0x2; // 
	// Cogemos el bit 2 que corresponde a eint2
	bit = (EXTINT >> 1) & 1;
	// Si bit sigues estando activado 1 entonces el pin sigue en low level
	if(bit == 1){
		return TRUE;
	}
	return FALSE;
}

int eint2_activada(void) {
	int bit;
	// Hacemos clear del bit 3 si el pin esta en low level no deber�a dejar hacer el clear
	EXTINT |= 0x4; // 
	// Cogemos el bit 2 que corresponde a eint2
	bit = (EXTINT >> 2) & 1;
	// Si bit sigues estando activado 1 entonces el pin sigue en low level
	if(bit == 1){
		return TRUE;
	}
	return FALSE;
}

void habilitar_interrupcion_eint1(void){
	VICVectCntl2 = 0x20 | 15; // 0x20 bit 5 enables vectored IRQs 15 is the number of the interrupt eint1
	VICIntEnable = VICIntEnable |  0x00008000;	// Enable EXTINT1 Interrupt
}

void habilitar_interrupcion_eint2(void){
	VICVectCntl3 = 0x20 | 16; // 0x20 bit 5 enables vectored IRQs 15 is the number of the interrupt eint1
	VICIntEnable = VICIntEnable |  0x00010000;	// Enable EXTINT1 Interrupt
}





