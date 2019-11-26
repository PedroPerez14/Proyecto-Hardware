/*********************************************************************************************
* Fichero:		jugada_por_botones.c
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

static enum estados{Inicio, elige_fila, espera_pulsacion_boton_izq, elige_columna} jugada_botones;
static int ev_8led = 0;				//Indica cuándo hay que  actualizar el valor que aparece en el 8led, por nueva pulsación del botón izquierdo o porque hay que autoincrementar
static int ev_bot_der = 0;			//Flag que señaliza cuándo han pulsado el botón derecho
static char mi_fila = 0;			//La fila donde queremos poner ficha
static char mi_columna = 0;			//La columna donde queremos poner ficha

void inicializar_jugada_botones()
{
	jugada_botones = Inicio;
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
	case Inicio:
		if(ev_bot_der == 1)		//En este punto ignoramos el botón derecho
		{
			ev_bot_der = 0;
		}
		if(ev_8led == 1)		//Quita la F y pone el 1
		{
			ev_8led = 0;
			D8Led_symbol(mi_fila+1);
			jugada_botones = elige_fila;
		}
		break;
	case elige_fila:
		if(ev_bot_der == 1)		//Confirmar elección de fila
		{
			ev_bot_der = 0;
			D8Led_symbol(0xC);
			jugada_botones = espera_pulsacion_boton_izq;
		}
		if(ev_8led == 1)		//Incrementar fila
		{
			ev_8led = 0;
			mi_fila +=1;
			mi_fila %= 8;
			D8Led_symbol(mi_fila+1);
		}
		break;
	case espera_pulsacion_boton_izq:
		if(ev_bot_der == 1)		//En este punto ignoramos el botón derecho
		{
			ev_bot_der = 0;
		}
		if(ev_8led == 1)		//Quita la C y pone el 1
		{
			ev_8led = 0;
			D8Led_symbol(mi_columna+1);
			jugada_botones = elige_columna;
		}
		break;
	default://elige_columna
		if(ev_bot_der == 1)		//Confirmar elección de columna
		{
			ev_bot_der = 0;
			reversi_procesar(mi_fila,mi_columna);
			mi_fila = 0;
			mi_columna = 0;
			D8Led_symbol(0xF);
			jugada_botones = Inicio;
		}
		if(ev_8led == 1)		//Incrementar columna
		{
			ev_8led = 0;
			mi_columna +=1;
			mi_columna %= 8;
			D8Led_symbol(mi_columna+1);
		}
		break;
	}
}

//Se llama cuando el botón derecho es pulsado, para tratar el evento
void jugada_ev_der()
{
	ev_bot_der = 1;
	jugada_por_botones();
}

//Se llama cuando, por autoincremento o por pulsación nueva,
//	hay que incrementar el valor del 8Led y la variable asociada a él.
void jugada_ev_incrementar()
{
	ev_8led = 1;
	jugada_por_botones();
}
