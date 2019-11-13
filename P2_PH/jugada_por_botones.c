/*********************************************************************************************
* Fichero:		botones_antirebotes.c
* Autor:		Pedro José Pérez García (756642) y Fernando Peña Bes (756012)
* Descrip:		Máquina de estados que gestiona la lógica del juego reversi.
* 				Las rutinas llamadas desde reversi_main invocan la función del autómata,
*				que comprueba si debe realizar alguna acción o no.
* Version:		
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "led.h"
#include "44b.h"
#include "44blib.h"
#include "botones_antirebotes.h"
#include "8led.h"
#include "button.h"
#include "codigos_eventos.h"
#include "reversi8_2019.h"

/*--- variables del módulo ---*/

static enum estados{q0, q1, q2, q3} jugada_botones;
static int ev_8led = 0;				//Indica cuándo hay que  actualizar el valor que aparece en el 8led, por nueva pulsación del botón izquierdo o porque hay que autoincrementar
static int ev_bot_der = 0;			//Flag que señaliza cuándo han pulsado el botón derecho
static char mi_fila = 0;			//La fila donde queremos poner ficha
static char mi_columna = 0;			//La columna donde queremos poner ficha

void inicializar_jugada_botones()
{
	jugada_botones = q0;
	mi_fila = 0;
	mi_columna = 0;
	ev_8led = 0;
	ev_bot_der = 0;
	D8Led_symbol(0xF);
}

void jugada_por_botones()
{
	switch(jugada_botones)
	{
	case q0:
		if(ev_bot_der == 1)
		{
			ev_bot_der = 0;
		}
		if(ev_8led == 1)
		{
			ev_8led = 0;
			D8Led_symbol(mi_columna+1);
			jugada_botones = q1;
		}
		break;
	case q1:
		if(ev_bot_der == 1)
		{
			ev_bot_der = 0;
			D8Led_symbol(0xC);
			jugada_botones = q2;
		}
		if(ev_8led == 1)
		{
			ev_8led = 0;
			mi_fila +=1;
			mi_fila %= 8;
			D8Led_symbol(mi_fila+1);
		}
		break;
	case q2:
		if(ev_bot_der == 1)
		{
			ev_bot_der = 0;
		}
		if(ev_8led == 1)
		{
			ev_8led = 0;
			D8Led_symbol(mi_columna+1);
			jugada_botones = q3;
		}
		break;
	default://q3
		if(ev_bot_der == 1)
		{
			ev_bot_der = 0;
			reversi_procesar(mi_fila,mi_columna);
			mi_fila = 0;
			mi_columna = 0;
			D8Led_symbol(0xF);
			jugada_botones = q0;
		}
		if(ev_8led == 1)
		{
			ev_8led = 0;
			mi_columna +=1;
			mi_columna %= 8;
			D8Led_symbol(mi_columna+1);
		}
		break;
	}
}

void jugada_ev_der()
{
	ev_bot_der = 1;
	jugada_por_botones();
}

void jugada_ev_incrementar()
{
	ev_8led = 1;
	jugada_por_botones();
}

	//contador_8led +=1;
	//D8Led_symbol(contador_8led & 0xf);
