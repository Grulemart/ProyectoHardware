
#include "linea_serie_drv.h"
#include "gpio_hal.h"
#include "config_conecta_K.h"

#define TRUE 1
#define FALSE 0

#define RECEIVE_BUFFER_SIZE 3
#define SEND_BUFFER_SIZE 300

#define MAX_BUFFERS 5

static volatile int estado = ESTADO_ESPERANDO_INICIO; // Estado de la UART para enviar caracteres

static volatile char receive_buffer[RECEIVE_BUFFER_SIZE];
static volatile uint8_t receive_buffer_index = 0;

static char send_array[SEND_BUFFER_SIZE] = {0}; 
static char send_buffer[MAX_BUFFERS][SEND_BUFFER_SIZE] = {0};
static volatile uint32_t send_buffer_index = 0;
static volatile uint8_t is_sending = FALSE;
static volatile uint8_t is_waiting[MAX_BUFFERS] = { FALSE };
static volatile uint8_t current_send_index = 0;
static volatile uint8_t queue_send_index = 0;

//static volatile uint8_t mandando_serie = FALSE;

static void (*funcionEncolarEvento)(uint8_t, uint32_t); // Funcion callback FIFO_encolar

static uint8_t gpio_serie_error; // Indice del pin de error

static uint8_t idEventoRX, idEventoTX; // Indice de los eventos a enviar

// Devuelve TRUE (1), si el comando introducido es válido y FALSE si no lo es
// En el caso de introducir un comando de jugada, devuelve TRUE si la jugada está dentro de los limites
uint8_t check_command(void){
	if(receive_buffer[0] == 'E' && receive_buffer[1] == 'N' && receive_buffer[2] == 'D'){
		return TRUE;
	}
	if(receive_buffer[0] == 'N' && receive_buffer[1] == 'E' && receive_buffer[2] == 'W'){
		return TRUE;
	}
	if(receive_buffer[0] == 'T' && receive_buffer[1] == 'A' && receive_buffer[2] == 'B'){
		return TRUE;
	}
	if(receive_buffer [1] == '-'){
		int fila = receive_buffer[0] - '0';
		int columna = receive_buffer[2] - '0';
		
		// Las jugadas son del 1 al 7 (formato humano), no del 0 al 6 (formato pagina)
		return (fila >= 1 && fila <= NUM_FILAS && columna >= 1 && columna <= NUM_COLUMNAS); 
	}
	return FALSE;
}


// Maneja las recepciones de carácteres de la UART
// Si aun no se ha iniciado un comando se espera a que se escriba '$'
// Despues se escribe el comando, si es valido se envia el evento EVENTO_RX_SERIE que maneja las acciones
// Si no se produce un error y se vuelve al estado de inicio
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
					for(i = 0; i < RECEIVE_BUFFER_SIZE; i++){
						auxdata |= (uint32_t)receive_buffer[i] << (8*i);
					}
					funcionEncolarEvento(idEventoRX, auxdata);
					array[1] = '\n';
					array[2] = '\0';
					linea_serie_drv_enviar_array(array);
				} else {
					char* mensajeError = "!\n Comando no reconocido.\n\0";
					linea_serie_drv_enviar_array(mensajeError);
				}
				estado = ESTADO_ESPERANDO_INICIO;
				receive_buffer_index = 0;
			}else if(receive_buffer_index >= 3){	// Comando > 3, no valido
					receive_buffer_index = 0;
					gpio_hal_escribir(gpio_serie_error, 1, 1);
			}else{
					linea_serie_drv_enviar_array(array);
					receive_buffer[receive_buffer_index++] = c;	
			}
	}		
}

// Envia el current_send_index a la UART
void sendToUART() {
	send_buffer_index = 0;
	uart0_enviar_caracter(send_array[send_buffer_index++]);
}

// Copia los contenidos de buffer en array
void copyArray(char* array, char* buffer) {
	uint16_t i;
	for(i = 0; i < SEND_BUFFER_SIZE || buffer[i] == '\0'; i++){
		array[i] = buffer[i];
		if(buffer[i] == '\0'){
			break;
		}
	}
}

// Recive una cadena de carácteres y los envia a la UART para mostrarlos
// No envia carácteres si está vacía '\0' o si ya se está enviando carácteres
void linea_serie_drv_enviar_array(char* array){
	if(*array == '\0'){ return; }
	
	if(is_sending == TRUE){
		if (is_waiting[queue_send_index] == TRUE) { return; } // OVERFLOW
		is_waiting[queue_send_index] = TRUE;
		copyArray(send_buffer[queue_send_index], array);
		queue_send_index = (queue_send_index + 1) % MAX_BUFFERS;
		return;
	}
	is_sending = TRUE;
	copyArray(send_array, array);
	sendToUART();
}

void linea_serie_drv_continuar_envio(void){
	// Se para de enviar carácteres
	if(send_buffer_index >= SEND_BUFFER_SIZE || send_array[send_buffer_index] == '\0'){
		if (is_waiting[current_send_index] == TRUE) {
			is_waiting[current_send_index] = FALSE;
			copyArray(send_array, send_buffer[current_send_index]);
			current_send_index = (current_send_index + 1) % MAX_BUFFERS;
			sendToUART();
		} else {
			is_sending = FALSE;
		}
		funcionEncolarEvento(idEventoTX, 0);
		return;
	}
	// Sigue habiendo carácteres por enviar
	uart0_enviar_caracter(send_array[send_buffer_index++]);
}

// Inicializa las variables y registros necesarios desde el modulo linea_serie_hal
void iniciar_linea_serie(uint8_t _idEventoRX, uint8_t _idEventoTX, void(*funcion_encolar_evento)(uint8_t, uint32_t), uint8_t _gpio_serie_error){
	funcionEncolarEvento = funcion_encolar_evento;
	gpio_serie_error = _gpio_serie_error;
	idEventoRX = _idEventoRX;
	idEventoTX = _idEventoTX;
	gpio_hal_sentido(gpio_serie_error, 1, GPIO_HAL_PIN_DIR_OUTPUT); // Solo es necesario declarar un bit de overflow
	send_buffer_index = 0;
	receive_buffer_index = 0;
	estado = ESTADO_ESPERANDO_INICIO;
	uart0_iniciar(&recibir_caracter, &linea_serie_drv_continuar_envio);
}
