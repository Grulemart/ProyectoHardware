#include <stdint.h>
#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "pulsacion.h"
#include "timer_hal.h"
// variable para contabilizar el número de interrupciones
static volatile uint64_t timer0_int_count = 0;
const uint64_t temporizador_hal_ticks2us = 60;

void timer0_ISR (void) __irq;

void temporizador_hal_iniciar(){
	T0MR0 = 2^32 -1;
	T0MCR = 3;                     // Generates an interrupt and resets the count when the value of MR0 is reached

  // configuration of the IRQ slot number 0 of the VIC for Timer 0 Interrupt
	VICVectAddr0 = (unsigned long)timer0_ISR;          // set interrupt vector in 0
  // 0x20 bit 5 enables vectored IRQs. 
	// 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
	VICVectCntl0 = 0x20 | 4;                   
  VICIntEnable = VICIntEnable | 0x00000010; 		// Enable Timer0 Interrupt
}

void temporizador_hal_empezar(){
	timer0_int_count = 0;
	T0TCR = 1;                             // Timer0 Enable
}

uint64_t temporizador_hal_leer(){
	return timer0_int_count * T0MR0 + T0TC;
}

uint64_t temporizador_hal_parar(){
	T0TCR = 0;															// Timer0 Disable
	return timer0_int_count * T0MR0 + T0TC;
}

/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void timer0_ISR (void) __irq {
    timer0_int_count++;
    T0IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                            // Acknowledge Interrupt
}




