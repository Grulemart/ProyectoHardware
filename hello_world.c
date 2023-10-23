
#include "hello_world.h"

static GPIO_HAL_PIN_T helloWorldPin;
static GPIO_HAL_PIN_BITS_T helloWorldPinBits;

static GPIO_HAL_PIN_BITS_T currentBit;

void hello_world_inicializar(GPIO_HAL_PIN_T bit, GPIO_HAL_PIN_BITS_T pinBits) {
	helloWorldPin = bit;
	helloWorldPinBits = pinBits;
	gpio_drv_sentido(helloWorldPin, helloWorldPinBits, GPIO_HAL_PIN_DIR_OUTPUT);
}

void hello_world_tick_tack(){ 
	
	uint32_t nuevoPin;
	
	if (currentBit == 0) {
		nuevoPin = 0;
	} else {
		nuevoPin = (1 << (currentBit - 1));
	}
	
	gpio_drv_escribir(helloWorldPin, helloWorldPinBits, nuevoPin);
	
	currentBit = (currentBit + 1) % helloWorldPinBits;
	
}