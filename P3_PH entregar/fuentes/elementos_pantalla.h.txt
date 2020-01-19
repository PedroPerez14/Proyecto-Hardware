/*********************************************************************************************
* File£º	elementos_pantalla.h
* Author:	Fernando Peña Bes (756012) y Pedro José Pérez García (756642)
* Descrip:	Colección de funciones que permiten dibujar en la pantalla las pantallas
* 			que corresponden a la ejecución del reversi8 en la placa
* History:	A long time ago, in galaxy a far away...
*********************************************************************************************/
#ifndef _ELEMENTOS_PANTALLA_H_
#define _ELEMENTOS_PANTALLA_H_
#include "definiciones_juego.h"
#include "Bmp.h"

/* Funciones disponibles para el usuario */
void pintar_ficha(int fila, int col, enum estado_casilla color);
void borrar_ficha(int fila, int columna);							//TODO hace falta ponerla 'publica'?
void pintar_profiling(int t_total, int t_calc, int t_pvolteo, int veces_pvolteo);
void pintar_jugando(void);
void mover_gris(int fila, int columna, int fila_anterior_gris, int columna_anterior_gris);
void pintar_prueba(void);
void pintar_reglas(void);
void pintar_fin_victoria(int blancas, int negras);
void pintar_fin_derrota(int blancas, int negras);
void pintar_fin_empate(int blancas, int negras);
void iniciar_DMA(void);

#endif /* _ELEMENTOS_PANTALLA_H_ */


