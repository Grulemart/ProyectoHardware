                  
#include <LPC210x.H>    
#include "planificador.h"
#include "timer_drv.h"
#include "fifo.h"
#include "io_reserva.h"
#include "gpio_hal.h"
#include "alarma.h"

void testFifoOverflow(){
	gpio_hal_iniciar();
	FIFO_inicializar(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS);
	temporizador_drv_reloj(30,FIFO_encolar, TIMER1);
}

int main() {
	/*
	temporizador_drv_iniciar();
	temporizador_drv_empezar();
	testFifoOverflow();
	while (gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS) != 1){
	}
	temporizador_drv_parar();
	gpio_hal_escribir(GPIO_OVERFLOW, GPIO_HELLO_WORLD_BITS, 0); //Quitamos el bit de overflow
	temporizador_drv_reloj(0,FIFO_encolar,TIMER1);	//Paramos el timer1
	*/
	
	planificador();
	
	return 0;
}



