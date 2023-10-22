#include "gpio_hal.h"
#include <stdint.h>
#include "LPC210X.H"
	
void gpio_hal_iniciar(){
	PINSEL0 = 0x00000000;
	
}

void gpio_hal_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, gpio_hal_pin_dir_t direccion){
	// Verifica que los bits a configurar estén en el rango correcto
    if (num_bits > 0 && num_bits <= 32) {
        // Máscara para los bits a configurar
        uint32_t mascara = (1U << num_bits) - 1U;
        
        if (direccion == GPIO_HAL_PIN_DIR_INPUT) {
            // Configura los bits como entrada
            IODIR &= ~(mascara << gpio_inicial);
        } else if (direccion == GPIO_HAL_PIN_DIR_OUTPUT) {
            // Configura los bits como salida
            IODIR |= (mascara << gpio_inicial);
        }
    }
}

uint32_t gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits){
	// Verifica que los bits a leer estén en el rango correcto
    if (num_bits > 0 && num_bits <= 32) {
        // Máscara para los bits a leer
        uint32_t mascara = (1U << num_bits) - 1U;
        
        // Lee y devuelve los bits indicados
        return (IOPIN >> gpio_inicial) & mascara;
    }
    
    // En caso de error, devuelve un valor predeterminado
    return 0;
}

void gpio_hal_escribir(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, uint32_t valor){
	// Verifica que los bits a escribir estén en el rango correcto
    if (num_bits > 0 && num_bits <= 32) {
        // Máscara para los bits a escribir
        uint32_t mascara = (1U << num_bits) - 1U;
				uint32_t gpio_auxiliar = IOPIN;
				
        // Clear de los bits sobre los que vamos a escribir
        IOCLR = (mascara << gpio_inicial);

        // Set the el nuevo valor de los bits
        IOSET = (valor & mascara) << gpio_inicial;
    }
}
