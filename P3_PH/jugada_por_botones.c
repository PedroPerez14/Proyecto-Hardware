/*********************************************************************************************
* Fichero:		jugada_por_botones.c
* Autor:		Pedro Jos� P�rez Garc�a (756642) y Fernando Pe�a Bes (756012)
* Descrip:		M�quina de estados que gestiona la l�gica del juego reversi.
* 				Las rutinas llamadas desde reversi_main invocan la funci�n del aut�mata,
*				que comprueba si debe realizar alguna acci�n o no.
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
#include "jugada_por_botones.h"

/*--- variables del m�dulo ---*/

static enum estados{Reglas, Jugando, Fin} jugada_botones;
static int ev_bot_izq = 0;			//Indica cu�ndo hay que  actualizar el valor que aparece en el 8led, por nueva pulsaci�n del bot�n izquierdo o porque hay que autoincrementar, puede valer m�s de 1
static int ev_bot_der = 0;			//Flag que se�aliza cu�ndo han pulsado el bot�n derecho, y cu�ntas veces
static int ev_tp = 0;				//Indica que hay que atender un toque del touchpad de alguien
static char mi_fila = 0;			//La fila donde queremos poner ficha
static char mi_columna = 0;			//La columna donde queremos poner ficha
static int haciendo_DMA = 0;		//Si estamos haciendo un DMA al LCD
static int ev_timer2 = 0;			//Interrupci�n del timer2 que indica que hay que refrescar tiempos en pantalla
static int fin = 0;					//0 si no ha terminado, 1 para jugador gana y 2 para CPU gana
/* PROFILING */
static int tiempo_calc = 0;
static int tiempo_total = 0;
static int tiempo_pv = 0;
static int veces_pv = 0;

void inicializar_jugada_botones()
{
	haciendo_DMA = 1;
	mi_fila = 0;
	mi_columna = 0;
	ev_bot_izq = 0;
	ev_bot_der = 0;
	ev_tp = 0;
	ev_timer2 = 0;
	fin = 0;
	tiempo_calc = 0;
	tiempo_total = 0;
	tiempo_pv = 0;
	veces_pv = 0;
	jugada_botones = Reglas;
	//Poner pantalla de reglas por DMA
	//
	//
}

void jugada_por_botones()
{
	switch(jugada_botones)
	{
	case Reglas:
		if(ev_bot_der > 0)
		{
			ev_bot_der = 0;
		}
		if(ev_bot_izq > 0)
		{
			ev_bot_izq = 0;
		}
		if(ev_tp == 1)
		{
			ev_tp = 0;
			if(haciendo_DMA == 0)
			{
				haciendo_DMA = 1;
				jugada_botones = Jugando;
				//poner DMA de pantalla de juego
			}
		}
		break;
	case Jugando:
		if(haciendo_DMA == 0 && ev_bot_der > 0)		//Incrementar fila
		{
			ev_bot_der --;
			mi_fila++;
			haciendo_DMA = 1;
			//DMA act pantalla
		}
		if(haciendo_DMA == 0 && ev_bot_izq > 0)		//Incrementar columna
		{
			ev_bot_izq --;
			mi_columna++;
			haciendo_DMA = 1;
			//DMA act pantalla
		}
		if(haciendo_DMA == 0 && ev_timer2 > 0)
		{
			ev_timer2--;
			haciendo_DMA = 1;
			//Actualizar tiempo total
			tiempo_total++;
			//DMA act pantalla
		}
		if(ev_tp == 1)
		{
			ev_tp = 0;
			if(haciendo_DMA == 0)
			{
				haciendo_DMA = 1;
				unsigned int delta1 = timer2_leer();
				reversi_procesar(mi_fila, mi_columna);
				unsigned int delta2 = timer2_leer();
				tiempo_calc += (delta2-delta1);
				//obtener datos de profiling
				//DMA de nuevo tablero
				//Y de nuevos datos que tengo que pintar
				//Calcular_fin
			}
		}
		if(haciendo_DMA == 0 && fin != 0)
		{
			haciendo_DMA = 1;
			if(fin == 1)
			{
				//DMA de pantalla de victoria
			}
			else	//fin == 2
			{
				//DMA de pantalla de derrota
			}
			jugada_botones = Fin;
		}
		break;
	default:	//case Fin
		if(haciendo_DMA == 0 && ev_tp == 1)		//Reiniciar partida, restaurando el estado
		{
			inicializar_jugada_botones();
		}
		break;
	}
}


//Se llama cuando se pulsa en tsp. confirmando la jugada
void jugada_ev_tsp()
{
	ev_tp = 1;
	jugada_por_botones();
}

//Se llama cuando el bot�n derecho es pulsado, para tratar el evento
void jugada_ev_der()
{
	ev_bot_der++;
	jugada_por_botones();
}

//Se llama cuando, por autoincremento o por pulsaci�n nueva,
//	hay que incrementar el valor del 8Led y la variable asociada a �l.
void jugada_ev_izq()
{
	ev_bot_izq++;
	jugada_por_botones();
}
//Se llama cuando haya que incrementar la cuenta de tiempo de juego en segundos
void jugada_ev_timer2()
{
	ev_timer2++;
	jugada_por_botones();
}

//Se llama para avisar al aut�mata de que la transferencia por DMA ha terminado
void termina_DMA()
{
	haciendo_DMA = 0;
	jugada_por_botones();
}
