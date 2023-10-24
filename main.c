                  
#include <LPC210x.H>    
#include "planificador.h"
#include "timer_drv.h"
#include "fifo.h"
#include "io_reserva.h"
#include "gpio_hal.h"

int main() {
	
	planificador();
	
	return 0;
}



