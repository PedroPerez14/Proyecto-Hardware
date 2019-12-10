/*********************************************************************************************
* File£º	elementos_pantalla.h
* Author:	Fernando Peña Bes (756012) y Pedro José Pérez García (756642)
* Descrip:	Colección de funciones que permiten dibujar en la pantalla las pantallas
* 			que corresponden a la ejecución del reversi8 en la placa
* History:	A long time ago, in galaxy a far away...
*********************************************************************************************/
#include "lcd.h"
#include "elementos_pantalla.h"

void pintar_cuadricula()
{
	volatile int i;
	volatile int j;
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			Lcd_Draw_Box(100+25*j,20+25*i,125+25*j,45+25*i,BLACK);
		}
	}
}

void pintar_numeros_tablero()
{
	//Poner índices de filas
	Lcd_DspAscII8x16(110,0,BLACK,"1");
	Lcd_DspAscII8x16(135,0,BLACK,"2");
	Lcd_DspAscII8x16(160,0,BLACK,"3");
	Lcd_DspAscII8x16(185,0,BLACK,"4");
	Lcd_DspAscII8x16(210,0,BLACK,"5");
	Lcd_DspAscII8x16(235,0,BLACK,"6");
	Lcd_DspAscII8x16(260,0,BLACK,"7");
	Lcd_DspAscII8x16(285,0,BLACK,"8");
	//Poner índices de columnas
	Lcd_DspAscII8x16(310,25,BLACK,"1");
	Lcd_DspAscII8x16(310,50,BLACK,"2");
	Lcd_DspAscII8x16(310,75,BLACK,"3");
	Lcd_DspAscII8x16(310,100,BLACK,"4");
	Lcd_DspAscII8x16(310,125,BLACK,"5");
	Lcd_DspAscII8x16(310,150,BLACK,"6");
	Lcd_DspAscII8x16(310,175,BLACK,"7");
	Lcd_DspAscII8x16(310,200,BLACK,"8");
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


//La fila
void pintar_ficha(int fila, int col, int color)	//1 -> blanco 2-> negro 3-> gris
{
	switch(color)
	{
		case 1:
			Lcd_DspAscII8x16(110+25*col, 25+25*fila,0x4, "O");
			break;
		case 2:
			Lcd_DspAscII8x16(110+25*col, 25+25*fila,BLACK, "X");
			break;
		default:
			Lcd_DspAscII8x16(110+25*col, 25+25*fila,0xb, "()");
			break;
	}
}

void pintar_profiling(int t_total, int t_calc, int t_pvolteo, int veces_pvolteo)
{
	Lcd_DspAscII8x16(20,35,BLACK,"451245");
	Lcd_DspAscII8x16(20,85,BLACK,"84512");
	Lcd_DspAscII8x16(20,135,BLACK,"56456");
	Lcd_DspAscII8x16(20,185,BLACK,"469352");
}

void pintar_tablero()
{
	pintar_cuadricula();
	pintar_numeros_tablero();
}

//TODO es necesaria esta funcion?
void pintar_pantalla_juego()
{
	pintar_tablero();
	pintar_textos();
	pintar_profiling(451245, 84512, 56456, 469352);
	//pintar_ficha(/* FICHAS */); //EN BUCLE
}

void pintar_pantalla_inicio()
{
	//Poner textos con reglas y demás por pantalla
	pintar_cuadricula();
	pintar_numeros_tablero();
	pintar_textos();
	Lcd_Dma_Trans();
}

void pintar_prueba()
{
	/*
	pintar_cuadricula();
	pintar_numeros_tablero();
	pintar_textos();
	pintar_ficha(0,0,1);
	pintar_ficha(7,7,2);
	pintar_ficha(3,3,3);
	pintar_profiling(451245, 84512, 56456, 469352);*/
	Lcd_DspAscII8x16(0,0,BLACK,"#############%%%%%,&((((((##############################################((((///////((((((((((((####");
	Lcd_DspAscII8x16(0,10,BLACK,"############&&%%%%,,,,,&((((##############################################(((((((((((((((((((((####");
	Lcd_DspAscII8x16(0,20,BLACK,"###############%%%,,,,,,,,,###################################################################&&%");
	Lcd_Dma_Trans();
}

void pintar_reglas()
{

}

void pintar_fin_victoria(int blancas, int negras)
{

}

void pintar_fin_derrota(int blancas, int negas)
{

}
