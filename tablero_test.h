#ifndef TABLEROS_H
#define TABLEROS_H

#include <inttypes.h>
//tableros de test
// 0: celda vacia, 1: ficha jugador uno, 2: ficha jugador dos


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


//se pueden definir otros tableros para comprobar casos

uint8_t
tablero_test_vertical[7][7] =
{
0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0};

uint8_t
tablero_test_diagonal1[7][7] =
{
0, 0, 0, 0, 0, 0, 0,
0, 1, 0, 2, 0, 0, 0,
0, 0, 1, 0, 0, 0, 0,
0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 2, 1, 0, 0,
0, 2, 0, 0, 0, 1, 0,
0, 0, 0, 0, 0, 0, 1};

uint8_t
tablero_test_diagonal2[7][7] =
{
0, 0, 0, 0, 0, 0, 0,
0, 1, 0, 0, 0, 1, 0,
0, 0, 1, 2, 1, 0, 0,
0, 0, 0, 1, 0, 0, 0,
0, 0, 1, 2, 0, 0, 2,
0, 2, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0};
	
uint8_t
tablero_test_horizontal[7][7] =
{
0, 0, 0, 0, 0, 0, 0,
0, 0, 1, 0, 0, 2, 0,
2, 0, 0, 0, 0, 0, 0,
0, 0, 1, 1, 1, 1, 0,
0, 0, 0, 0, 0, 0, 0,
0, 2, 0, 0, 1, 0, 0,
0, 0, 0, 0, 0, 0, 0};



#endif
