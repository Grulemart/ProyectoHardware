#include "linea_serie_drv.h"


static volatile int estado = ESTADO_ESPERANDO_INICIO;
static volatile char receiveBuffer[3];
static volatile uint8_t buffer_index = 0;
static volatile char sendBuffer[SEND_BUFFER_SIZE];
static volatile uint32_t send_buffer_index = 0;
static volatile BOOLEAN mandando_serie = FALSE;

int check_command(void){
	if(receiveBuffer[0] == 'E' && receiveBuffer[1] == 'N' && receiveBuffer[2] == 'D'){
		return TRUE;
	}
	if(receiveBuffer[0] == 'N' && receiveBuffer[1] == 'E' && receiveBuffer[2] == 'W'){
		return TRUE;
	}
	if(receiveBuffer [1] == '-'){
		int fila = receiveBuffer[0] - '0';
		int columna = receiveBuffer[1] - '0';
		
		return (fila >= 1 && fila <= 7 && columna >= 1 && columna <= 7); //TODO: cambiar por constantes
	}
	return FALSE;
}

void recibir_caracter(char c){	
	char array[2];
	switch(estado) {
		case ESTADO_ESPERANDO_INICIO:
			if(c == START_DELIMETER){
				array[0] = c;
				array[1] = '\0';
				linea_serie_drv_enviar_array(array);
				estado = ESTADO_RECIBIENDO_TRAMA;
			}
			break;
		case ESTADO_RECIBIENDO_TRAMA:
				array[0] = c;
				array[1] = '\0';
				linea_serie_drv_enviar_array(array);

			if(c == END_DELIMETER){
				if(check_command())
					FIFO_encolar(EV_RX_SERIE, (uint32_t)receiveBuffer);
			}else if(buffer_index >= 3){
					gpio_hal_escribir(GPIO_SERIE_ERROR, 1, 1);
				}else{
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

void linea_serie_drv_continuar_envio(){
	if(send_buffer_index >= SEND_BUFFER_SIZE || sendBuffer[send_buffer_index ] == '\0'){
		mandando_serie = FALSE;
		FIFO_encolar(EV_TX_SERIE,0);
		return;
	}
	uart0_enviar_caracter(sendBuffer[send_buffer_index++]);
}

void iniciar_linea_serie(void){
	gpio_hal_sentido(GPIO_SERIE_ERROR, 1, GPIO_HAL_PIN_DIR_OUTPUT);
	uart0_iniciar(&recibir_caracter, &linea_serie_drv_continuar_envio);
}