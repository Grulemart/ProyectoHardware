
#include "juego.h"
#include "llamadas_swi.h"
#include "tablero.h"
#include "celda.h"
#include "linea_serie_drv.h"
#include "alarma.h"
#include "config_conecta_K.h"

#define TIEMPO_JUGADA 10000 //ms

#define TRUE 1
#define FALSE 0

#define VACIO 0

#define JUGADOR_1_COLOR 1
#define JUGADOR_2_COLOR 2

static volatile uint8_t cuenta; // Variable que maneja el numero de turnos realizados
static volatile uint64_t intervalo;
static TABLERO cuadricula; // Estructura que guarda los datos del tablero
static int filaJugada = -1;	// Fila de la jugada por confirmar (-1 no se cconfirma ninguna)
static int columnaJugada = -1; // Columna de la jugada por confirmar (-1 no se cconfirma ninguna)
static char array[13]; // Array auxiliar para enviar comandos a UART
static int estado = ESPERANDO_INICIO; // Estado de juego
static int error = FALSE;	// Variable que confirma si hay un error
static int errorEstado = COMANDO_NO_RECONOCIDO; // Tipo de error del modulo
static int turno = 1;
static uint8_t mostrar_tablero = FALSE;
static uint8_t ganador = VACIO;

static char* errorComandoNoReconocido = " Comando no reconocido\n";
static char* errorComandoNoValido = " Comando no valido en el contexto actual\n";
static char* errorJugadaNoValida = " Jugada no valida (los valores tienen que estar dentro de los limites del tablero)\n";

extern uint8_t conecta_K_buscar_alineamiento_arm(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color, int8_t delta_fila, int8_t delta_columna);
extern uint8_t conecta_K_hay_linea_arm_arm(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color);

// Función para convertir un entero a una cadena de caracteres (ASCII)
void uint64ToAsciiArray(uint64_t value, char asciiArray[9]){
    // Assuming a 64-bit unsigned integer and 8 characters for the ASCII representation
    // (including the null terminator)

    // Convert the digits
    int i;
    for (i = 7; i >= 0; --i) {
        asciiArray[i] = '0' + value % 10;
        value /= 10;
    }

    // Null-terminate the string
    asciiArray[8] = '\0';
}

// Muestra el tiempo transcurrido en us desde que se inicia el temporizador por UART
void conecta_K_visualizar_tiempo(void){
	uint64ToAsciiArray(clock_get_us(), array);
	array[8] = ' ';
	array[9] = 'u';
	array[10] = 's';
	array[11] = '\n';
	array[12] = '\0';
	linea_serie_drv_enviar_array(array);
	
}

// Carga los datos de un tablero a mitad partida para comprobar que el programa procesa un estado acabado
void conecta_K_test_cargar_tablero(){
	uint8_t i;
	uint8_t j;
	uint8_t 
tablero_test[7][7] =
	{
	0, 1, 0, 0, 0, 2, 0,
	1, 1, 0, 0, 0, 2, 2,
	0, 0, 1, 0, 2, 0, 0,
	0, 0, 0, 0, 0, 2, 0,
	0, 0, 2, 0, 0, 0, 0,
	2, 2, 0, 1, 0, 1, 1,
	0, 2, 0, 0, 0, 1, 0};
	
	for (i = 0; i < NUM_FILAS; i++){
		for(j = 0; j < NUM_COLUMNAS; j++){
			tablero_insertar_color(&cuadricula, i, j, tablero_test[i][j]);
		}
	}
	
}

// Carga un tablero vacio en memoria
void conecta_K_vacio_cargar_tablero(){
	uint8_t i;
	uint8_t j;
	for (i = 0; i < NUM_FILAS; i++){
		for(j = 0; j < NUM_COLUMNAS; j++){
			tablero_insertar_color(&cuadricula, i, j, VACIO);
		}
	}
}

// Añade el contenido de buffer al final de array
// Devuelve el índice del último componente añadido al array
uint8_t appendArray(char array[250], char buffer[50], uint8_t index, uint8_t size) {
	uint8_t i;
	for (i = 0; i < size; i++) {
		array[index + i] = buffer[i];
	}
	return index + size;
}

// Visualiza el tablero cargado en memoria por pantalla
void conecta_K_visualizar_tablero(){
	static char array[250];
	int fila;
	int columna;
	int indiceArray = 0;
	CELDA celda;
		
	array[indiceArray++] = '\n';
	array[indiceArray++] = '-';
	array[indiceArray++] = '|';


	for(columna = 1; columna <= NUM_COLUMNAS; columna++){
		array[indiceArray++] = columna + '0';
		array[indiceArray++] = '|';
	}
	array[indiceArray++] = '\n';
	for(fila = 0; fila < NUM_FILAS; fila++ ){
		array[indiceArray++] = fila + 1 + '0';
		array[indiceArray++] = '|';
		for(columna = 0; columna < NUM_COLUMNAS; columna++){
			celda = tablero_leer_celda(&cuadricula, fila, columna);
			if(columna == columnaJugada && fila == filaJugada){
				array[indiceArray++] = '?';
				array[indiceArray++] = '|';
			}else if(celda_vacia(celda)){
				array[indiceArray++] = ' ';
				array[indiceArray++] = '|';
			}else if(celda_color(celda) == 1){
				array[indiceArray++] = 'B';
				array[indiceArray++] = '|';
			}else{
				array[indiceArray++] = 'N';
				array[indiceArray++] = '|';
			}
			
		}
		array[indiceArray++] = '\n';
	}
	array[indiceArray++] = '\n';
	
	if (estado == ESTADO_ESPERANDO_JUGADA || estado == ESPERANDO_INICIO) {
		uint8_t bufferSize = 21;
		char* buffer = "Turno del jugador X\n\n";
		indiceArray = appendArray(array, buffer, indiceArray, bufferSize);
		array[indiceArray-3] = '0' + (turno / 2) + 1;
	} else if (estado == TERMINAR_JUEGO) {
		if (ganador == VACIO) {
			uint8_t bufferSize = 53;
			char* buffer = "Se ha terminado el juego\n" \
				"Se ha producido un empate.\n\n";
			indiceArray = appendArray(array, buffer, indiceArray, bufferSize);
		} else {
			uint8_t bufferSize = 45;
			char* buffer = "Se ha terminado el juego\n" \
				"Gana el jugador X\n\n";
			indiceArray = appendArray(array, buffer, indiceArray, bufferSize);
			array[indiceArray-3] = '0' + ganador;
		}
	}
	array[indiceArray++] = '\0';
	linea_serie_drv_enviar_array(array);
}

// Inicializa las variables del modulo
void juego_inicializar() {
	cuenta = 0;
	tablero_inicializar(&cuadricula);
	conecta_K_vacio_cargar_tablero();
}

// Muestra las intrucciones de tutorial por pantalla
void juego_mostrar_instrucciones(void) {
	
	char* instrucciones = "Instrucciones de juego:\n"\
		"Escribe $NEW! para una nueva partida\n"\
		"Escribe $#-#! (fila-columna) para realizar una jugada\n"\
	  "Escribe $TAB! para mostrar el tablero\n"\
	  "Escribe $END! para rendirse\n";
	
	linea_serie_drv_enviar_array(instrucciones);
}

// Compara dos vectores 
int compararVectorConString(char vector[], char cadena[]){
	int i;
	for(i = 0; i < 3; i++){
		if(vector[i] != cadena[i]){
			return FALSE;
		}
	}
	return TRUE;
}

// Comprueba si una jugada introducida es válida
// La jugada es un array de carácteres de tipo #-#
// donde # deben ser numeros de 1 al numero especificado de filas o columnas
int esJugadaValida(char comando[]){
	if(comando[1] == '-'){
		int fila = comando[0] - '0';
		int columna = comando[2] - '0';
		
		if(fila >= 1 && fila <= NUM_FILAS && columna >= 1 && columna <= NUM_COLUMNAS) {	
			return celda_vacia(tablero_leer_celda(&cuadricula, fila, columna));
		}
	}
	return FALSE;
}

// Envia un error al modulo
void enviar_error(uint8_t tipoError) {
	
	error = TRUE;
	errorEstado = tipoError;
	
}

// Termina una partida del juego y muestra quien ha ganado
void termina_juego(uint8_t ganadorCuenta) {
	
	if (estado == ESPERANDO_INICIO) {
		char* mensaje = "\nTodavia no se ha iniciado un juego.\n";
		linea_serie_drv_enviar_array(mensaje);
		return;
	}
	
	ganador = ganadorCuenta;
	conecta_K_visualizar_tablero();
	estado = ESPERANDO_INICIO;
	
}

// Cancela una jugada pendiente
void cancelar_jugada(void) {
	
	if (estado != ESPERANDO_DECISION_JUGADA) {
		return;
	}
	
	estado = ESTADO_ESPERANDO_JUGADA;
	filaJugada = -1;
	columnaJugada = -1;
	conecta_K_visualizar_tablero(); // Esto igual no se visualiza
	estado = ESTADO_ESPERANDO_JUGADA;
	
}

// Trata los comandos llegados de la terminal y actualiza el estado
void juego_tratar_comando(char comando[3]){
	if(estado == ESPERANDO_INICIO){
		if(compararVectorConString(comando, "NEW")){
			conecta_K_test_cargar_tablero();
			estado = ESPERANDO_TX_FIN_COMANDO_1;
		} else {
			enviar_error(COMANDO_NO_VALIDO);
		}
	}else if(estado == ESTADO_ESPERANDO_JUGADA){
		if (compararVectorConString(comando, "TAB")) {
			mostrar_tablero = TRUE;
		} else if(compararVectorConString(comando, "END")) {
			estado = TERMINAR_JUEGO;
		} else if(esJugadaValida(comando)){
			filaJugada = comando[0] - '0' - 1;
			columnaJugada = comando[2] - '0' - 1;
			alarma_activar(HACER_JUGADA, TIEMPO_JUGADA, 0);
			estado = ESPERANDO_TX_FIN_COMANDO_2;
		} else {
			enviar_error(JUGADA_NO_VALIDA);
		}
	}
}

// Muestra por pantalla los resultados del comando introducido
void juego_trasmision_realizada(void){
	
	if (mostrar_tablero == TRUE) {
		mostrar_tablero = FALSE;
		conecta_K_visualizar_tablero();
		return;
	}
	
	if (error == TRUE) {
		error = FALSE;
		if (errorEstado == COMANDO_NO_RECONOCIDO) {
			linea_serie_drv_enviar_array(errorComandoNoReconocido);
		} else if (errorEstado == COMANDO_NO_VALIDO) {
			linea_serie_drv_enviar_array(errorComandoNoValido);
		} else if (errorEstado == JUGADA_NO_VALIDA) {
			linea_serie_drv_enviar_array(errorJugadaNoValida);
		}
		return;
	}
	
	if (estado == ESPERANDO_TX_FIN_COMANDO_1){
		estado = ESTADO_ESPERANDO_JUGADA;
		conecta_K_visualizar_tablero();
	}else if(estado == ESPERANDO_TX_FIN_COMANDO_2){
		estado = ESPERANDO_DECISION_JUGADA;
		conecta_K_visualizar_tablero();
	} else if (estado == TERMINAR_JUEGO) {
		termina_juego(VACIO);
	}
}

// Realizar la jugada dentro del tablero
void juego_alarma(void){
	if(estado == ESPERANDO_DECISION_JUGADA){
		uint8_t color = 0;
		if(turno == 0 || turno == 1){
			color = 1;
			tablero_insertar_color(&cuadricula, filaJugada, columnaJugada, JUGADOR_1_COLOR); 
		}else{
			color = 2;
			tablero_insertar_color(&cuadricula, filaJugada, columnaJugada, JUGADOR_2_COLOR); 
		}
			
		if (conecta_K_hay_linea_arm_arm(&cuadricula, filaJugada, columnaJugada, color)) {
			uint8_t ganadorCuenta = (turno / 2) +1;
			termina_juego(ganadorCuenta);
		} else {
			turno = (turno + 1) % 4; 
			conecta_K_visualizar_tablero();
			estado = ESTADO_ESPERANDO_JUGADA;
		}
		
		filaJugada = -1;
		columnaJugada = -1;
	}
}
