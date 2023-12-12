
#include "linea_serie_drv.h"
#include "gpio_hal.h"

#define TRUE 1
#define FALSE 0

static volatile int estado = ESTADO_ESPERANDO_INICIO;
static volatile char receiveBuffer[3];
static volatile uint8_t buffer_index = 0;
static volatile char sendBuffer[SEND_BUFFER_SIZE];
static volatile uint32_t send_buffer_index = 0;
static volatile uint8_t mandando_serie = FALSE;
static void (*funcionEncolarEvento)();
static uint8_t gpio_serie_error;
static uint8_t idEventoRX, idEventoTX;

uint8_t check_command(void){
	if(receiveBuffer[0] == 'E' && receiveBuffer[1] == 'N' && receiveBuffer[2] == 'D'){
		return TRUE;
	}
	if(receiveBuffer[0] == 'N' && receiveBuffer[1] == 'E' && receiveBuffer[2] == 'W'){
		return TRUE;
	}
	if(receiveBuffer[0] == 'T' && receiveBuffer[1] == 'A' && receiveBuffer[2] == 'B'){
		return TRUE;
	}
	if(receiveBuffer [1] == '-'){
		int fila = receiveBuffer[0] - '0';
		int columna = receiveBuffer[2] - '0';
		
		return (fila >= 1 && fila <= 7 && columna >= 1 && columna <= 7); //TODO: cambiar por constantes
	}
	return FALSE;
}

void recibir_caracter(char c){	
	static char array[3];
	static int i;
	static uint32_t auxdata;
	if(c == '\r'){
		array[0] = '\n';
	}else{
		array[0] = c;
	}
	array[1] = '\0';

	switch(estado) {
		case ESTADO_ESPERANDO_INICIO:
			if(c == START_DELIMETER){
				gpio_hal_escribir(gpio_serie_error, 1, 0);
				estado = ESTADO_RECIBIENDO_TRAMA;
				linea_serie_drv_enviar_array(array);
			}
			break;
		case ESTADO_RECIBIENDO_TRAMA:
			if(c == END_DELIMETER){
				if(check_command()){
					auxdata = 0;
					for(i = 0; i < 3; i++){
						auxdata |= (uint32_t)receiveBuffer[i] << (8*i);
					}
					buffer_index = 0;
					estado = ESTADO_ESPERANDO_INICIO;
					funcionEncolarEvento(idEventoRX, auxdata);
					array[1] = '\n';
					array[2] = '\0';
					linea_serie_drv_enviar_array(array);
				}
			}else if(buffer_index >= 3){
					estado = ESTADO_ESPERANDO_INICIO;
					buffer_index = 0;
					gpio_hal_escribir(gpio_serie_error, 1, 1);
				}else{
					linea_serie_drv_enviar_array(array);
					receiveBuffer[buffer_index++] = c;	
				}
		}		
}

void linea_serie_drv_enviar_array(char* array){
	if(*array == '\0'){
		return;
	}
	if(mandando_serie == TRUE){
		return;
	}
	mandando_serie = TRUE;
	for(send_buffer_index = 0; send_buffer_index < SEND_BUFFER_SIZE; send_buffer_index++){
		sendBuffer[send_buffer_index] = array[send_buffer_index];
		if(array[send_buffer_index] == '\0'){
			break;
		}
	}
	
	send_buffer_index = 0;
	uart0_enviar_caracter(sendBuffer[send_buffer_index++]);
}

void linea_serie_drv_continuar_envio(void){
	if(send_buffer_index >= SEND_BUFFER_SIZE || sendBuffer[send_buffer_index ] == '\0'){
		mandando_serie = FALSE;
		if(sendBuffer[0] == '!' && sendBuffer[1] == '\n' && sendBuffer[2] == '\0'){
			funcionEncolarEvento(idEventoTX, AUX_DATA_COMANDO_TERMINADO);
		}else{
			funcionEncolarEvento(idEventoTX, 0);
		}
		return;
	}
	uart0_enviar_caracter(sendBuffer[send_buffer_index++]);
}

void iniciar_linea_serie(uint8_t _idEventoRX, uint8_t _idEventoTX, void(*funcion_encolar_evento)(), uint8_t _gpio_serie_error){
	funcionEncolarEvento = funcion_encolar_evento;
	gpio_serie_error = _gpio_serie_error;
	idEventoRX = _idEventoRX;
	idEventoTX = _idEventoTX;
	gpio_hal_sentido(gpio_serie_error, 1, GPIO_HAL_PIN_DIR_OUTPUT); // Solo es necesario declarar un bit de overflow
	send_buffer_index = 0;
	buffer_index = 0;
	mandando_serie = FALSE;
	estado = ESTADO_ESPERANDO_INICIO;
	uart0_iniciar(&recibir_caracter, &linea_serie_drv_continuar_envio);
}
