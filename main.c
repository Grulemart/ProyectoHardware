                  
#include <LPC210x.H>    
#include "planificador.h"
#include "timer_drv.h"
#include "fifo.h"
#include "io_reserva.h"
#include "gpio_hal.h"

int main() {
	gpio_hal_iniciar();
	gpio_hal_sentido(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, GPIO_HAL_PIN_DIR_OUTPUT);
	FIFO_inicializar(GPIO_OVERFLOW);
	temporizador_drv_reloj(30,FIFO_encolar, TIMER1);
	while (gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS) != 1){
	}
	FIFO_inicializar(GPIO_OVERFLOW);
	temporizador_drv_reloj(0,FIFO_encolar,TIMER1);
	planificador();
	
	return 0;
}



