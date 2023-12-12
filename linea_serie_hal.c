#include "linea_serie_hal.h"

static void (*callback_funciton_recibir)(char c);
static void (*callback_funciton_enviar)(void); 

void uart0_RSI (void) __irq;

void uart0_RSI(void) __irq {
	uint32_t mascara = 0x7;
	uint32_t identificadorInterrupcion = (U0IIR >> 1) & mascara;

	if(identificadorInterrupcion == 0x2){
		(*callback_funciton_recibir)(U0RBR);
	}else if(identificadorInterrupcion == 0x1){
		(*callback_funciton_enviar)();
	}
	VICVectAddr = 0;
}
	
void uart0_iniciar(void (*function_callback_recibir)(char c), void (*function_callback_enviar)(void)){
	callback_funciton_recibir = function_callback_recibir;
	callback_funciton_enviar = function_callback_enviar;
	PINSEL0 = PINSEL0 | 0x5;
	
	// Configuramos la tasa de baudios
	U0LCR = 0x80;
	U0DLM = 0;
	U0DLL = 97;
	U0LCR = 0x03;
	U0FCR = (U0FCR | 0x1) & ~0xc0;
	U0IER = U0IER | 0x3;
	
	// Asignamos al vector de interrupciones 4 nuestra funcion de interrupcion
	VICVectAddr4 = (unsigned long)uart0_RSI;
	// Habilitamos la interrupción UART0
	VICVectCntl4 = VICVectCntl4 | 0x26;
	VICIntEnable = VICIntEnable | 0x40;
}

void uart0_enviar_caracter(char c){
		U0THR = c;
}


