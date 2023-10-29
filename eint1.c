#include <LPC210X.H>
#include "eint1.h"

static unsigned int eint1_count = 0;
static int eint1_nueva_pulsacion = 0;

void eint1_ISR(void) __irq;

void eint1_init(void) {
 eint1_nueva_pulsacion = 0;
 eint1_count = 0;
 EXTINT = EXTINT | 2;
 PINSEL0 = PINSEL0 & 0xcfffffff;
 PINSEL0 = PINSEL0 | 0x20000000;
 VICVectCntl2 = 0x20 | 15;
 VICIntEnable = VICIntEnable | 0x00008000;
 VICVectAddr2 = (unsigned long)eint1_ISR;
}

unsigned int eint1_read_count(void) {
 return eint1_count;
}

int eint1_read_nueva_pulsacion(void){
 return eint1_nueva_pulsacion;
}

void eint1_clear_nueva_pulsacion(void){
 eint1_nueva_pulsacion = 0;
}

int eint1_sigue_pulsado(void) {
 EXTINT = EXTINT | 2;
 if ((EXTINT & 2) == 2) {
	return 1;
 }
	else return 0;
}

 void eint1_habilitar_interrupciones(void) {
 VICIntEnable = VICIntEnable | 0x00008000;
}

void eint1_ISR(void) __irq {
 eint1_count++;
 FIFO_encolar(EINT1, 0);
 VICIntEnClr = 0x00008000;
 EXTINT = EXTINT | 2;
 VICVectAddr = 0;
 eint1_nueva_pulsacion = 1;
}