/*********************************************************************************************
* File: 	elementos_pantalla.h
* Author:	Fernando Peña Bes (756012) y Pedro José Pérez García (756642)
* Descrip:	Colección de funciones y variables que permiten dibujar en el LCD las pantallas
* 			que corresponden a la ejecución del juego reversi8 en la placa.
* History:	A long time ago, in galaxy a far away...
*********************************************************************************************/
#include "lcd.h"
#include "elementos_pantalla.h"
#include "funciones_itoa.h"
#include "bmp.h"

void pintar_cuadricula()
{
	volatile int i;
	volatile int j;
	for(i = 0; i < num_filas; i++)
	{
		for(j = 0; j < num_columnas; j++)
		{
			Lcd_Draw_Box(100+25*j,20+25*i,125+25*j,45+25*i,BLACK);
		}
	}
}

void pintar_numeros_tablero()
{
	int i;
	char buffer[15];
	for(i = 0; i < num_filas; i++)
	{
		Lcd_DspAscII8x16(110 + 25*i, 0,BLACK,itoa(i+1, buffer,10));	//10 para pasar a decimal
		Lcd_DspAscII8x16(310,25 + 25*i,BLACK,itoa(i+1, buffer,10));
	}
}

void pintar_textos()
{
	Lcd_DspAscII8x16(5,10,BLACK,"Tiempo total");
	Lcd_DspAscII8x16(20,20,BLACK,"de juego:");
	Lcd_DspAscII8x16(5,60,BLACK,"Tiempo para");
	Lcd_DspAscII8x16(20,70,BLACK,"calculos:");
	Lcd_DspAscII8x16(5,110,BLACK,"Tiempo para");
	Lcd_DspAscII8x16(20,120,BLACK,"p.volteo:");
	Lcd_DspAscII8x16(5,160,BLACK,"Numero inv.");
	Lcd_DspAscII8x16(20,170,BLACK,"p.volteo:");
	Lcd_DspAscII8x16(0,220,BLACK,"-> TOCA LA PANTALLA PARA HACER TU JUGADA");
}

void pintar_ficha(int fila, int col, enum estado_casilla color)
{
	// Fichas con letras
	/*
	switch(color)
	{
		case FICHA_BLANCA:
			Lcd_DspAscII8x16(110+25*col, 25+25*fila,0xa, "O");
			break;
		case FICHA_NEGRA:
			Lcd_DspAscII8x16(110+25*col, 25+25*fila,BLACK, "X");
			break;
		case FICHA_GRIS:
			Lcd_DspAscII8x16(110+25*col, 25+25*fila,0xd, "@");
			break;
	}
	*/

	// Fichas circulares

	switch(color)
	{
		case FICHA_BLANCA:
			BitmapView(110+25*col, 25+25*fila, Stru_Bitmap_fichaBlanca);
			break;
		case FICHA_NEGRA:
			BitmapView(110+25*col, 25+25*fila, Stru_Bitmap_fichaNegra);
			break;
		case FICHA_GRIS:
			BitmapView(110+25*col, 25+25*fila, Stru_Bitmap_fichaGris);
			break;
	}
}

void borrar_ficha(int f, int c)		//TODO acabar
{
	LcdClrRect((100+25*c) + 1, (20+25*f) + 1, (125+25*c) - 1, (45+25*f) - 1, 0x0);
}

//Mueve hacia abajo en el LCD la ficha gris a partir de la fila y columna en la que está (0-7),
//	teniendo en cuenta lo que hubiera en la casilla donde está actualmente, porque lo restaura
void mover_gris_abajo(int fila, int columna, int fila_anterior_gris)		//TODO testear
{
	pintar_ficha(fila, columna, FICHA_GRIS);
	borrar_ficha(fila_anterior_gris, columna);
	Lcd_Dma_Trans();
}

//Mueve hacia la derecha en el LCD la ficha gris a partir de la fila y columna en la que está (0-7),
//	teniendo en cuenta lo que hubiera en la casilla donde estaba antes de actualizar el movimiento, porque lo restaura
void mover_gris_derecha(int fila, int columna, int columna_anterior_gris)	//TODO testear y medir tiempos
{
	pintar_ficha(fila, columna, FICHA_GRIS);
	borrar_ficha(fila, columna_anterior_gris);
	Lcd_Dma_Trans();
}

void pintar_profiling(int t_total, int t_calc, int t_pvolteo, int veces_pvolteo)
{
	char buffer[15];						//15 por darle un tamaño decente

	//Se borra la zona de pantalla virtual donde están los numeritos del profiling que si no no funciona :)
	LcdClrRect(20, 35, 75, 50, 0x0000);
	LcdClrRect(20, 85, 95, 100, 0x0000);
	LcdClrRect(20, 135, 95, 150, 0x0000);
	LcdClrRect(20, 185, 75, 200, 0x0000);

	itoa(t_total, buffer, 10);				//10 es para indicar que pasamos a números decimales
	Lcd_DspAscII8x16(20,35,BLACK,buffer);
	itoa(t_calc, buffer, 10);
	Lcd_DspAscII8x16(20,85,BLACK,buffer);
	itoa(t_pvolteo, buffer, 10);
	Lcd_DspAscII8x16(20,135,BLACK,buffer);
	itoa(veces_pvolteo, buffer, 10);
	Lcd_DspAscII8x16(20,185,BLACK,buffer);
}

void pintar_jugando()
{
	Lcd_Clr();
	pintar_cuadricula();
	pintar_numeros_tablero();
	pintar_textos();
	pintar_profiling(0,0,0,0);
	pintar_ficha(0,0,3);	//Pintar la ficha gris en 0,0
	pintar_ficha(3,3,FICHA_BLANCA);	//Pintar las fichas blancas iniciales en 3,3 y 4,4
	pintar_ficha(4,4,FICHA_BLANCA);
	pintar_ficha(3,4,FICHA_NEGRA);	//Pintar las fichas negras en 3,4 y 4,3
	pintar_ficha(4,3,FICHA_NEGRA);
	Lcd_Dma_Trans();
}

void pintar_prueba()
{
	pintar_cuadricula();
	/*pintar_numeros_tablero();
	pintar_textos();
	pintar_ficha(0,0,1);
	pintar_ficha(7,7,2);
	pintar_ficha(3,3,3);
	pintar_profiling(451245, 84512, 56456, 469352);*/
	//Lcd_DspAscII8x16(0,0,BLACK,",,,,,,%#########%%%%%#####################################((/**************//////");
	//Lcd_DspAscII8x16(8,16,BLACK,",,,,%###########%%%%%%####################################((/*********///////((((");
	//Lcd_DspAscII8x16(16,32,BLACK,"################%%%%%%%###################################((/********////((((((##");
	//Lcd_DspAscII8x16(24,48,BLACK,"REGLAS");
	pintar_ficha(7, 0, FICHA_BLANCA);
	Lcd_Dma_Trans();
	Delay(250);
	borrar_ficha(7, 0);
	Lcd_Dma_Trans();
	Delay(250);
	pintar_ficha(7, 0, FICHA_BLANCA);
	Lcd_Dma_Trans();
	Delay(250);
	pintar_ficha(7, 0, 4);
	Lcd_Dma_Trans();
	Delay(250);
}

void pintar_reglas()
{
	Lcd_Active_Clr();
	Lcd_DspAscII8x16(1,30,BLACK,"REVERSI 8");


	Lcd_DspAscII8x16(1,50,BLACK,"En cada turno, coloca una ficha que voltee");
	Lcd_DspAscII8x16(1,60,BLACK,"alguna del rival.");

	Lcd_DspAscII8x16(1,80,BLACK,"El juego termina cuando nadie puede");
	Lcd_DspAscII8x16(1,70,BLACK,"mover. El jugador que tenga más fichas");
	Lcd_DspAscII8x16(1,90,BLACK,"volteadas con su color, gana.");


	Lcd_DspAscII8x16(1,110,BLACK,"Selecciona una posición con los botones");
	Lcd_DspAscII8x16(1,120,BLACK,"y coloca tu ficha tocando la pantalla.");




	Lcd_DspAscII8x16(1,180,BLACK,"Pedro José Pérez García");
	Lcd_DspAscII8x16(1,190,BLACK,"Fernando Peña");

	Lcd_Dma_Trans();
}

void pintar_fin_victoria(int blancas, int negras)
{
	Lcd_Clr();
	Lcd_DspAscII8x16(1,30,BLACK,"HAS GANADO!");
	Lcd_Dma_Trans();
}

void pintar_fin_derrota(int blancas, int negras)
{
	Lcd_Clr();
	Lcd_DspAscII8x16(1,30,BLACK,"HAS PERDIDO, TROZO DE ESCORIA!");
	Lcd_Dma_Trans();
}

void pintar_fin_empate(int blancas, int negras)
{
	Lcd_Clr();
	Lcd_DspAscII8x16(1,30,BLACK,"ERES TAN INÚTIL QUE NO GANAS PERO");
	Lcd_DspAscII8x16(1,50,BLACK,"TAMPOCO PIERDES CON UNA PUTA CPU!");
	Lcd_Dma_Trans();
}

void iniciar_DMA()
{
	Lcd_Dma_Trans();
}
