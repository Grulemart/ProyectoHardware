
#include "juego.h"
#include "llamadas_swi.h"
#include "tablero.h"
#include "celda.h"
#include "linea_serie_drv.h"
#include "alarma.h"
#include "config_conecta_K.h"

#define TIEMPO_JUGADA 1000 //ms

#define TRUE 1
#define FALSE 0

#define VACIO 0

#define JUGADOR_1_COLOR 1
#define JUGADOR_2_COLOR 2

#define ESPERANDO_INICIO 0
#define ESPERANDO_TX_FIN_COMANDO_1 1
#define ESTADO_ESPERANDO_JUGADA 2
#define ESPERANDO_TX_FIN_COMANDO_2 3
#define ESPERANDO_DECISION_JUGADA 4
#define ESPERANDO_TX_FIN_COMANDO_3 5
#define ESPERANDO_TX_RESULTADOS 6
#define ESPERANDO_FIN_COMANDO_4 7
#define ESPERANDO_TX_ERROR 8
#define ESPERANDO_FIN_COMANDO_5 9
#define ESPERANDO_TX_ERROR2 10
#define TERMINAR_JUEGO 11
#define ESPERANDO_TX_DECISION_JUGADA 12

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
static int primeraPartida = TRUE;
static uint64_t tiempoInicioPartida;

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
char* conecta_K_visualizar_tiempo(uint32_t tiempo){
	uint64ToAsciiArray(tiempo, array);
	array[8] = ' ';
	array[9] = 'u';
	array[10] = 's';
	array[11] = '\n';
	array[12] = '\0';
	return array;
	
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

void mostrar_turno_jugada(){
	char *array;
	if(turno == 0 | turno == 1){
		array = "Turno jugador 1:\n";
	}else{
		array = "Turno jugador 2:\n";
	}
	linea_serie_drv_enviar_array(array);
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
	array[indiceArray++] = '\0';
	linea_serie_drv_enviar_array(array);
	mostrar_turno_jugada();
}

// Muestra las intrucciones de tutorial por pantalla
void juego_mostrar_instrucciones(void) {
	
	char* instrucciones = "Instrucciones de juego:\n"\
		"Escribe $NEW! para una nueva partida\n"\
		"Escribe $#-#! (fila-columna) para realizar una jugada\n"\
	  "Escribe $TAB! para mostrar el tablero\n"\
	  "Escribe $END! para rendirse\n\0";
	
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
			return celda_vacia(tablero_leer_celda(&cuadricula, fila - 1, columna - 1));
		}
	}
	return FALSE;
}

// Envia un error al modulo
void enviar_error(uint8_t tipoError) {
	
	error = TRUE;
	errorEstado = tipoError;
	
}

void mostrar_resultados(){
	uint8_t turnoJugador = (turno / 2) + 1;
	char* buffer;
	if(ganador == VACIO){
		char array[27] = "El jugador 1 se ha rendido\n";
		array[11] = '0' + turnoJugador;
		linea_serie_drv_enviar_array(array);
	} else {
		char array[100] = "Ha ganado jugador 1\n" \
			"Tiempo CPU: ";
		array[18] = '0' + turnoJugador;
		buffer = conecta_K_visualizar_tiempo(clock_get_us() - tiempoInicioPartida);
		appendArray(array, buffer, 32, 13);
		linea_serie_drv_enviar_array(array);
	}
}

// Cancela una jugada pendiente
void cancelar_jugada(void) {
	if (estado != ESPERANDO_DECISION_JUGADA) {
		return;
	}
	filaJugada = -1;
	columnaJugada = -1;
	estado = ESPERANDO_TX_DECISION_JUGADA;
	linea_serie_drv_enviar_array("Jugada cancelada\n");
}

void juego_inicializar(){
	tablero_inicializar(&cuadricula);
	turno = 1;
	ganador = VACIO;
	filaJugada = -1;
	columnaJugada = -1;
	tiempoInicioPartida = clock_get_us();
}

// Trata los comandos llegados de la terminal y actualiza el estado
void juego_tratar_comando(char comando[3]){
	if(estado == ESPERANDO_INICIO){
		if(compararVectorConString(comando, "NEW")){
			juego_inicializar();
			if (primeraPartida) {
				conecta_K_test_cargar_tablero();
				primeraPartida = FALSE;
			}
			estado = ESPERANDO_TX_FIN_COMANDO_1;
		} else {
			enviar_error(COMANDO_NO_VALIDO);
		}
	}else if(estado == ESTADO_ESPERANDO_JUGADA){
		if (compararVectorConString(comando, "TAB")) {
			mostrar_tablero = TRUE;
		} else if(compararVectorConString(comando, "END")) {
			estado = ESPERANDO_TX_FIN_COMANDO_3;
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
void juego_transmision_realizada(void){
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
		estado = ESPERANDO_TX_DECISION_JUGADA;
		conecta_K_visualizar_tablero();
	}else if(estado == ESPERANDO_TX_FIN_COMANDO_2){
		estado = ESPERANDO_DECISION_JUGADA;
		conecta_K_visualizar_tablero();
	}else if (estado == ESPERANDO_TX_FIN_COMANDO_3){
		estado = ESPERANDO_INICIO;
		mostrar_resultados();
	} else if (estado == ESPERANDO_TX_DECISION_JUGADA){
		//mostrar_turno_jugada();
		estado = ESTADO_ESPERANDO_JUGADA;
	}
}

// Realizar la jugada dentro del tablero
void juego_alarma(void){
	if(estado == ESPERANDO_DECISION_JUGADA){
		uint8_t color = 0;
		if(turno == 0 || turno == 1){
			color = JUGADOR_1_COLOR;
			tablero_insertar_color(&cuadricula, filaJugada, columnaJugada, JUGADOR_1_COLOR); 
		}else{
			color = JUGADOR_2_COLOR;
			tablero_insertar_color(&cuadricula, filaJugada, columnaJugada, JUGADOR_2_COLOR); 
		}
			
		if(conecta_K_hay_linea_arm_arm(&cuadricula, filaJugada, columnaJugada, color)) {
			if(turno == 0 || turno == 1){
				ganador = JUGADOR_1_COLOR;
			}else {
				ganador = JUGADOR_2_COLOR;
			}
			estado = ESPERANDO_INICIO;
			mostrar_resultados();
		} else {
			filaJugada = -1;
			columnaJugada = -1;
			turno = (turno + 1) % 4; 
			estado = ESPERANDO_TX_DECISION_JUGADA;
			conecta_K_visualizar_tablero();
		}
		
		
	}
}
