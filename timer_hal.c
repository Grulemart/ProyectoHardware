
#include "timer_hal.h"
// variable para contabilizar el número de interrupciones
static volatile uint64_t timer0_int_count = 0;
static void (*callback_funcion)() = 0;
void timer0_RSI (void) __irq;
void timer1_RSI (void) __irq; 
void temporizador_hal_iniciar(){
	timer0_int_count = 0;
	T0MR0 = 0xFFFFFFFE;                        // Interrumpe cada 0,05ms = 150.000-1 counts
  T0MCR = 3;                     // Generates an interrupt and resets the count when the value of MR0 is reached

  // configuration of the IRQ slot number 0 of the VIC for Timer 0 Interrupt
	VICVectAddr0 = (unsigned long)timer0_RSI;          // set interrupt vector in 0
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
	return (uint64_t)T0TC + timer0_int_count * (uint64_t)T0MR0;
}

uint64_t temporizador_hal_parar(){
	T0TCR = 3;
	T0TCR = 2;	// Timer0 Disable and reset
	return temporizador_hal_leer();
}

/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void timer0_RSI (void) __irq {
    timer0_int_count++;
    T0IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                            // Acknowledge Interrupt
}

void temporizador_hal_reloj(uint32_t periodo, void (*function_callback)()){
	if(periodo == 0){
		T1TCR = 3;
		T1TCR = 0;	// Timer1 Disable and reset
	}else{
		T1PR = 14;	// Prescalar para que el timer 1 cuente cuando pase 1us
		T1MR0 = periodo;	// Interrumpe cada periodo en ms
		T1MCR = 3;	//// Generates an interrupt and resets the count when the value of MR0 is reached
		// configuration of the IRQ slot number 1 of the VIC for Timer 1 Interrupt
		VICVectAddr1 = (unsigned long)timer1_RSI;	// Cuando interrumpe llama a funcion_callback
		// 5 is the number of the interrupt assigned. Number 5 is the Timer 1 
		VICVectCntl1 = 0x20 | 5;
		VICIntEnable = VICVectCntl1 | 0x00000020;	// Enable Timer1 Interrupt
		T1TCR = 1;		// Timer1 enable
		callback_funcion = function_callback;
	}
}

void timer1_RSI (void) __irq {
    T1IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                            // Acknowledge Interrupt
    callback_funcion();
}
