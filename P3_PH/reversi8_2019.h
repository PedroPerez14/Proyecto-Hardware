/*********************************************************************************************
* Fichero:	reversi8_2019.h
* Autor:	Profesores de la asignatura
			Fernando Peña Bes (NIA: 756012)
			Pedro José Pérez García (NIA: 756642)
* Descrip:	Fichero de cabecera para el juego reversi8.
* 			Incluye funciones para probar patron_volteo()
* Version:
*********************************************************************************************/

#ifndef _REVERSI8_2019_H_
#define _REVERSI8_2019_H_

#include "timer2.h"
#include "definiciones_juego.h"

void reversi8();
void reversi8_inicializar(void);
void reversi_procesar(char f, char c);
enum estado_casilla obtener_ficha_en(int fila, int columna);
enum final_partida obtener_fin(void);
int contar_blancas(void);
int contar_negras(void);
int reversi_t_pv(void);
int reversi_veces_pv(void);
void obtener_tablero(char** tablero);
//TODO hacer las funciones que te dicen qué ficha hay en cada casilla
#endif /* _REVERSI8_2019_H_ */
