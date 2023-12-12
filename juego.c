
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

static volatile uint8_t cuenta;
static volatile uint64_t intervalo;
static TABLERO cuadricula;
static int filaJugada = -1;
static int columnaJugada = -1;
static char array[13];
static volatile uint64_t tiempoSistema;
static int estado = ESPERANDO_INICIO;
static void (*funcionEncolarEvento)(uint8_t, uint32_t);
static int turno = 1;

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
	tiempoSistema = clock_get_us();
	uint64ToAsciiArray(tiempoSistema, array);
	array[8] = ' ';
	array[9] = 'u';
	array[10] = 's';
	array[11] = '\n';
	array[12] = '\0';
	linea_serie_drv_enviar_array(array);
	
}

// Carga los datos de un tablero a mitad partida para comprobar que el programa procesa un estado acabado
void conecta_K_test_cargar_tablero(TABLERO *t){
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
			tablero_insertar_color(t, i, j, tablero_test[i][j]);
		}
	}
}

// Carga un tablero vacio en memoria
void conecta_K_vacio_cargar_tablero(TABLERO *t){
	uint8_t i;
	uint8_t j;
	for (i = 0; i < NUM_FILAS; i++){
		for(j = 0; j < NUM_COLUMNAS; j++){
			tablero_insertar_color(t, i, j, VACIO);
		}
	}
}

// Visualiza el tablero cargado en memoria por pantalla
void conecta_K_visualizar_tablero(){
	static char array[200];
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

// Inicializa un juego con un tablero test
void juego_inicializar(void(*funcion_encolar_evento)(uint8_t, uint32_t)) {
	cuenta = 0;
	funcionEncolarEvento = funcion_encolar_evento;
	
	tablero_inicializar(&cuadricula);
	conecta_K_test_cargar_tablero(&cuadricula);
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

// Encola un evento con un valor de auxData+cuenta en la cola
void juego_tratar_evento(uint8_t ID_evento, uint32_t auxData) {
	//uint64_t currentCheck = clock_get_us();
	//intervalo = currentCheck - intervalo;
	cuenta += auxData;
	
	(*funcionEncolarEvento)(ID_evento, (uint32_t)cuenta);
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

// Trata los comandos llegados de la terminal y actualiza el estado
void juego_tratar_comando(char comando[3]){
	if(estado == ESPERANDO_INICIO){
		if(compararVectorConString(comando, "NEW")){
			tablero_inicializar(&cuadricula);
			estado = ESPERANDO_TX_FIN_COMANDO_1;
		}
	}else if(estado == ESTADO_ESPERANDO_JUGADA){
		if(esJugadaValida(comando)){
			filaJugada = comando[0] - '0' - 1;
			columnaJugada = comando[2] - '0' - 1;
			alarma_activar(HACER_JUGADA, TIEMPO_JUGADA, 0);
			estado = ESPERANDO_TX_FIN_COMANDO_2;
		}
	}
}

// 
void juego_trasmision_realizada(void){
	if (estado == ESPERANDO_TX_FIN_COMANDO_1){
		conecta_K_visualizar_tablero();
		estado = ESTADO_ESPERANDO_JUGADA;
	}else if(estado == ESPERANDO_TX_FIN_COMANDO_2){
		conecta_K_visualizar_tablero();
		estado = ESPERANDO_DECISION_JUGADA;
	}
}

//
void juego_boton_pulsado(int boton){
	//TODO
}

//
void juego_alarma(void){
	if(estado == ESPERANDO_DECISION_JUGADA){
		if(turno == 0 || turno == 1){
			tablero_insertar_color(&cuadricula, filaJugada, columnaJugada, 1); //constantes
		}else{
			tablero_insertar_color(&cuadricula, filaJugada, columnaJugada, 2); // constantes
		}
		filaJugada = -1;
		columnaJugada = -1;
		turno = (turno + 1) % 4; 
		conecta_K_visualizar_tablero();
		estado = ESTADO_ESPERANDO_JUGADA;
	}
}
