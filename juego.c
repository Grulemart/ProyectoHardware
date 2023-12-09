
#include "juego.h"
#include "llamadas_swi.h"
#include "tablero.h"
#include "celda.h"
#include "linea_serie_drv.h"

static volatile uint8_t cuenta;
static volatile uint64_t intervalo;
static TABLERO cuadricula;
static char array[13];
static volatile uint64_t tiempoSistema;
static void (*funcionEncolarEvento)();

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
			if(celda_vacia(celda)){
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

void juego_inicializar(void(*funcion_encolar_evento)()) {
	cuenta = 0;
	intervalo = 0;
	funcionEncolarEvento = funcion_encolar_evento;
	
	
	
	tablero_inicializar(&cuadricula);
	conecta_K_test_cargar_tablero(&cuadricula);
}

void juego_mostrar_instrucciones() {
	
	char* instrucciones = "Instrucciones de juego:\n"\
		"Escribe $NEW! para una nueva partida\n"\
		"Escribe $#-#! (fila-columna) para realizar una jugada\n"\
	  "Escribe $TAB! para mostrar el tablero\n"\
	  "Escribe $END! para rendirse\n";
	
	linea_serie_drv_enviar_array(instrucciones);
}

void juego_tratar_evento(uint8_t ID_evento, uint32_t auxData) {
	//uint64_t currentCheck = clock_get_us();
	//intervalo = currentCheck - intervalo;
	cuenta += auxData;
	
	(*funcionEncolarEvento)(ID_evento, (uint32_t)cuenta);
	
}
