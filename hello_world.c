
#include "hello_world.h"
#include "timer_drv.h"
#include "fifo.h"

static GPIO_HAL_PIN_T helloWorldPin;
static GPIO_HAL_PIN_BITS_T helloWorldPinBits;

static GPIO_HAL_PIN_BITS_T currentBit;

void hello_world_inicializar(GPIO_HAL_PIN_T bit, GPIO_HAL_PIN_BITS_T pinBits) {
	helloWorldPin = bit;
	helloWorldPinBits = pinBits;
	gpio_hal_sentido(helloWorldPin, helloWorldPinBits, GPIO_HAL_PIN_DIR_OUTPUT);
}

void hello_world_tick_tack(){ 
	
	uint32_t nuevoPin;
	nuevoPin = (1 << (currentBit));
	
	
	//gpio_hal
	gpio_hal_escribir(helloWorldPin, helloWorldPinBits, nuevoPin);
	temporizador_drv_reloj(10000, FIFO_encolar, TIMER1);
	
	currentBit = (currentBit + 1) % helloWorldPinBits;
	
}
