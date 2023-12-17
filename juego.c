
#include "juego.h"
#include "llamadas_swi.h"
#include "tablero.h"
#include "celda.h"
#include "linea_serie_drv.h"
#include "alarma.h"
#include "config_conecta_K.h"
#include <stdio.h>

#define TIEMPO_DECISION_JUGADA 3000 //ms

#define SECS_TO_US 1000000

#define TRUE 1
#define FALSE 0

#define VACIO 0

#define JUGADOR_1_COLOR 1
#define JUGADOR_2_COLOR 2

#define ESPERANDO_INICIO 0
#define ESPERANDO_JUGADA 1
#define ESPERANDO_DECISION 2

#define TURNO_JUGADOR_1 1
#define TURNO_JUGADOR_2 2


static TABLERO cuadricula; // Estructura que guarda los datos del tablero
static int filaJugada = -1;	// Fila de la jugada por confirmar (-1 no se cconfirma ninguna)
static int columnaJugada = -1; // Columna de la jugada por confirmar (-1 no se cconfirma ninguna)
static int estado = ESPERANDO_INICIO;
static int turno = 1;
static uint8_t ganador = VACIO;
static int primeraPartida = TRUE;
static uint64_t tiempoInicioPartida;
static uint64_t tiempoTotalHayLinea;
static int numVecesHayLinea = 0;
static uint64_t tiempoInicioJugada;
static uint64_t tiempoTotalPensarJugada;
static int numVecesPensarJugada = 0;

static char* errorComandoNoReconocido = " Comando no reconocido\n";
static char* errorComandoNoValido = " Comando no valido en el contexto actual\n";
static char* errorJugadaNoValida = " Jugada no valida (los valores tienen que estar dentro de los limites del tablero)\n";

extern uint8_t conecta_K_buscar_alineamiento_arm(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color, int8_t delta_fila, int8_t delta_columna);
extern uint8_t conecta_K_hay_linea_arm_arm(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color);


// Funciones Auxiliares

// Funci�n para convertir un entero a una cadena de caracteres (ASCII)
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



// A�ade el contenido de buffer al final de array
// Devuelve el �ndice del �ltimo componente a�adido al array
uint8_t appendArray(char array[250], char buffer[50], uint8_t index, uint8_t size) {
	uint8_t i;
	for (i = 0; i < size; i++) {
		array[index + i] = buffer[i];
	}
	return index + size;
}

int juego_turno_jugador(){
	if(turno == 0 || turno == 1){
		return TURNO_JUGADOR_1;
	}
	return TURNO_JUGADOR_2;
	
}


void juego_mostrar_turno_jugada(){
	char array[100];
	sprintf(array, "Turno jugador %d\n", juego_turno_jugador());
	
	linea_serie_drv_enviar_array(array);
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


// Comprueba si una jugada introducida es v�lida
// La jugada es un array de car�cteres de tipo #-#
// donde # deben ser numeros de 1 al numero especificado de filas o columnas
int juego_es_jugada_valida(char comando[]){
	if(comando[1] == '-'){
		int fila = comando[0] - '0';
		int columna = comando[2] - '0';
		
		if(fila >= 1 && fila <= NUM_FILAS && columna >= 1 && columna <= NUM_COLUMNAS) {	
			return celda_vacia(tablero_leer_celda(&cuadricula, fila - 1, columna - 1));
		}
	}
	return FALSE;
}



// Carga los datos de un tablero a mitad partida para comprobar que el programa procesa un estado acabado
void juego_cargar_test_tablero(){
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



// Visualiza el tablero cargado en memoria por pantalla
void juego_mostrar_tablero(){
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


void juego_mostrar_estadisticas(){
		uint64_t tiempoTotalPartida = clock_get_us() - tiempoInicioPartida; 
		char array[250];
		sprintf(array, "Tiempo Total CPU: %llu us (~%llu segundos)\n"
		"Tiempo Total en hacer jugadas: %llu us (~%llu segundos)\n"
		"Tiempo Medio en hacer jugada: %llu us (~%llu segundos)\n"
		"Tiempo Total computo hay linea: %llu us (~%llu segundos)\n"
		"Tiempo Medio computo hay linea: %llu us (~%llu segundos)\n", 
		tiempoTotalPartida, tiempoTotalPartida/SECS_TO_US,
		tiempoTotalPensarJugada, tiempoTotalPensarJugada/SECS_TO_US, 
		tiempoTotalPensarJugada/numVecesPensarJugada, tiempoTotalPensarJugada/(numVecesPensarJugada * SECS_TO_US),
		tiempoTotalHayLinea, tiempoTotalHayLinea/SECS_TO_US,
		tiempoTotalHayLinea/numVecesHayLinea, tiempoTotalHayLinea/(numVecesHayLinea * SECS_TO_US));
		linea_serie_drv_enviar_array(array);
}

void juego_mostrar_resultados(){
	char array[50];
	if(ganador == VACIO){
		sprintf(array, "El jugador %d se ha rendido\n",juego_turno_jugador());
	} else {
		sprintf(array, "El jugador %d ha ganado\n",juego_turno_jugador());
	}
	linea_serie_drv_enviar_array(array);
	juego_mostrar_estadisticas();
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
			if(primeraPartida){
				juego_cargar_test_tablero();
				primeraPartida = FALSE;
			}
			
			juego_mostrar_tablero();
			juego_mostrar_turno_jugada();
			tiempoInicioJugada = clock_get_us();
			tiempoInicioPartida = clock_get_us();
			estado = ESPERANDO_JUGADA;
		} else {
			linea_serie_drv_enviar_array("Comando erroneo se esperaba NEW\n");
		}
	}else if(estado == ESPERANDO_JUGADA){
		if(compararVectorConString(comando, "END")){
			juego_mostrar_resultados();
			estado = ESPERANDO_INICIO;
		}else if(juego_es_jugada_valida(comando)){
			numVecesPensarJugada++;
			tiempoTotalPensarJugada += clock_get_us() - tiempoInicioJugada;
			filaJugada = comando[0] - '0' -1;
			columnaJugada = comando[2] - '0' -1;
			alarma_activar(HACER_JUGADA, TIEMPO_DECISION_JUGADA, 0);
			linea_serie_drv_enviar_array("Pulse el boton 1 para cancelar la jugada\n"
			"Si quiere que la jugada se procese espera 3 segundos\n");
			juego_mostrar_tablero();
			estado = ESPERANDO_DECISION;
		}else {
			linea_serie_drv_enviar_array("Jugada no v�lida\n");
		}
	}else if(estado == ESPERANDO_DECISION){
		linea_serie_drv_enviar_array("Pulse el boton 1 para cancelar la jugada\n"
			"Si quiere que la jugada se procese espera 3 segundos\n");
	}
}

// Realizar la jugada dentro del tablero
void juego_alarma(void){
	uint64_t tiempoAntesHayLinea;
	int hayLinea;
	if(estado == ESPERANDO_DECISION){
		uint8_t color = 0;
		if(juego_turno_jugador() == TURNO_JUGADOR_1){
			color = JUGADOR_1_COLOR;
			tablero_insertar_color(&cuadricula, filaJugada, columnaJugada, JUGADOR_1_COLOR); 
		}else{
			color = JUGADOR_2_COLOR;
			tablero_insertar_color(&cuadricula, filaJugada, columnaJugada, JUGADOR_2_COLOR); 
		}
		
		tiempoAntesHayLinea = clock_get_us();
		hayLinea = conecta_K_hay_linea_arm_arm(&cuadricula, filaJugada, columnaJugada, color);
		tiempoTotalHayLinea += clock_get_us() - tiempoAntesHayLinea;
		numVecesHayLinea++;
		
		if(hayLinea) {
			if(juego_turno_jugador() == TURNO_JUGADOR_1){
				ganador = JUGADOR_1_COLOR;
			}else {
				ganador = JUGADOR_2_COLOR;
			}
			filaJugada = -1;
			columnaJugada = -1;
			estado = ESPERANDO_INICIO;
			juego_mostrar_tablero();
			juego_mostrar_resultados();
		} else {
			filaJugada = -1;
			columnaJugada = -1;
			turno = (turno + 1) % 4; 
			tiempoInicioJugada = clock_get_us();
			estado = ESPERANDO_JUGADA;
			juego_mostrar_tablero();
			juego_mostrar_turno_jugada();
		}
	}
}

void juego_terminar_partida(void){
	if(estado == ESPERANDO_JUGADA){
		juego_mostrar_resultados();
		estado = ESPERANDO_INICIO;
	}
}

void juego_cancelar_jugada(void){
	if(estado == ESPERANDO_DECISION){
		// Cancelamos la alarma
		alarma_activar(HACER_JUGADA, 0, 0);
		filaJugada = -1;
		columnaJugada= -1;
		juego_mostrar_tablero();
	}
	
}

