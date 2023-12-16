
#include "hello_world.h"
#include "alarma.h"

#define REPROGRAMAR_ALARMA 0x80000000
#define TIEMPO_ENTRE_BLINK_182 100

static GPIO_HAL_PIN_T helloWorldPin;
static GPIO_HAL_PIN_BITS_T helloWorldPinBits;

static uint32_t contador;

// Inicializa las variables, pines de GPIO y alarma necesaria para el módulo
void hello_world_inicializar(GPIO_HAL_PIN_T bit, GPIO_HAL_PIN_BITS_T pinBits, uint8_t evento) {
	// Inicializaci�n de variables y de pins en GPIO
	helloWorldPin = bit;
	helloWorldPinBits = pinBits;
	gpio_hal_sentido(helloWorldPin, helloWorldPinBits, GPIO_HAL_PIN_DIR_OUTPUT);
	
	alarma_activar(evento, TIEMPO_ENTRE_BLINK_182 + REPROGRAMAR_ALARMA, 0);
}

// Procesa un ciclo del módulo, se añade 1 al contador de bits en la GPIO
void hello_world_tick_tack(){ 
	
	// Calculo de nuevo pin a escribir
	contador += 1;
	
	// Escritura de pins en GPIO
	gpio_hal_escribir(helloWorldPin, helloWorldPinBits, contador);
	
	
	if(contador % 63 == 0){
		contador = 0;
	}
	
}
