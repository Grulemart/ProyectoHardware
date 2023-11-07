
#include "hello_world.h"


#define TIEMPO_ENTRE_BLINK_182 0x8000000A 

static GPIO_HAL_PIN_T helloWorldPin;
static GPIO_HAL_PIN_BITS_T helloWorldPinBits;

static GPIO_HAL_PIN_BITS_T currentBit;

void hello_world_inicializar(GPIO_HAL_PIN_T bit, GPIO_HAL_PIN_BITS_T pinBits) {
	// Inicializaci�n de variables y de pins en GPIO
	helloWorldPin = bit;
	helloWorldPinBits = pinBits;
	gpio_hal_sentido(helloWorldPin, helloWorldPinBits, GPIO_HAL_PIN_DIR_OUTPUT);
	
	
	alarma_activar(VISUALIZAR_HELLO, TIEMPO_ENTRE_BLINK_182, 0);
	//temporizador_drv_reloj(TIEMPO_ENTRE_BLINK_182, FIFO_encolar, TIMER1);
}

void hello_world_tick_tack(){ 
	
	// Calculo de nuevo pin a escribir
	uint32_t nuevoPin = (1 << (currentBit));
	
	// Escritura de pins en GPIO
	gpio_hal_escribir(helloWorldPin, helloWorldPinBits, nuevoPin);
	
	
	currentBit = (currentBit + 1) % helloWorldPinBits;
	
}
