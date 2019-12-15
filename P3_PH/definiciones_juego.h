/*********************************************************************************************
* File:		definiciones_juego.h
* Author:	Pedro Jos� P�rez Garc�a(756642) y Fernando Pe�a Bes(756012)
* Desc:		Definiciones comunes para reversi8, jugada_por_botones y elementos_pantalla
* 			As� evitamos tener que redefinir todo 200 veces en cada sitio haciendo guarradas
* History:	v1
*********************************************************************************************/

#ifndef __EP_H__
#define __EP_H__

enum estado_casilla{		//Antes estaba en reversi8.c
	CASILLA_VACIA = 0,
	FICHA_BLANCA = 1,
	FICHA_NEGRA = 2,
	FICHA_GRIS = 3
};

/* Variables para representar en pantalla, antes en elementos_pantalla.c, las muevo de sitio */
enum{num_filas = 8, num_columnas = 8};

/* Antes en jugada_por_botones.h, lo muevo de sitio */
enum final_partida{no_fin, jugador_gana, cpu_gana, empate};

#endif /*__EP_H__*/

