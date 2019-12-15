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
#include "jugada_por_botones.h"
#include "elementos_pantalla.h"
#include "definiciones_juego.h"
#include "lcd.h"					//Se incluye por haciendo_DMA, como nos ha dicho Darío
/*--- variables del módulo ---*/

static enum estados{Reglas, Jugando, Fin} jugada_botones;
static int ev_bot_izq = 0;			//Indica cuándo hay que  actualizar el valor que aparece en el 8led, por nueva pulsación del botón izquierdo o porque hay que autoincrementar, puede valer más de 1
static int ev_bot_der = 0;			//Flag que señaliza cuándo han pulsado el botón derecho, y cuántas veces
static int ev_tp = 0;				//Indica que hay que atender un toque del touchpad de alguien
static char mi_fila = 0;			//La fila donde queremos poner ficha
static char mi_columna = 0;			//La columna donde queremos poner ficha
volatile int haciendo_DMA = 0;		//Si estamos haciendo un DMA al LCD
static int ev_timer = 0;			//Interrupción del timer que indica que hay que refrescar tiempos en pantalla
static enum final_partida fin;		//Determina si la partida ha terminado, y cómo lo ha hecho: Por empate, victoria del jugador o de la CPU
/* PROFILING */
static int tiempo_calc = 0;
static int tiempo_total = 0;
static int tiempo_pv = 0;
static int veces_pv = 0;
/* TABLEROS PARA ACTUALIZAR SOLO LAS FICHAS NECESARIAS */
static char tablero_actual[num_filas][num_columnas];		//Aquí se guardarán las fichas después de que la cpu haga su movimiento
static char tablero_anterior[num_filas][num_columnas];		//Aquí se guardarán las fichas antes de que la cpu haga su movimiento
															//Se hará la diferencia, y solo se pintarán aquellas que cambien.
															//También, después de esto, anterior = actual, para el próximo turno.


void inicializar_jugada_botones()
{
	reversi8_inicializar();
	haciendo_DMA = 1;
	mi_fila = 0;
	mi_columna = 0;
	ev_bot_izq = 0;
	ev_bot_der = 0;
	ev_tp = 0;
	ev_timer = 0;
	fin = no_fin;
	tiempo_calc = 0;
	tiempo_total = 0;
	tiempo_pv = 0;
	veces_pv = 0;
	obtener_tablero(tablero_actual);
	obtener_tablero(tablero_anterior);
	jugada_botones = Reglas;
	pintar_reglas();
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
				pintar_jugando();
			}
		}
		break;
	case Jugando:
		if(ev_bot_der == 1)		//Incrementar fila
		{
			ev_bot_der = 0;
			int aux = mi_fila;
			do
			{
				mi_fila = ((mi_fila + 1) & 0x7);			//8 porque hay 8 columnas en el tablero, [0-7], 0x7 = num_columnas
			}
			while((obtener_ficha_en(mi_fila, mi_columna) != CASILLA_VACIA) && (obtener_ficha_en(mi_fila, mi_columna) != FICHA_GRIS));

			while(haciendo_DMA != 0)
			{
			}
			haciendo_DMA = 1;
			//DMA act ficha gris
			mover_gris_abajo(mi_fila, mi_columna, aux);		//Mover en la pantalla
		}
		if(ev_bot_izq == 1)		//Incrementar columna
		{
			ev_bot_izq = 0;
			int aux = mi_columna;
			do
			{
				mi_columna = ((mi_columna + 1) & 0x7);		//8 porque hay 8 columnas en el tablero, [0-7], 0x7 = num_filas
			}
			while((obtener_ficha_en(mi_fila, mi_columna) != CASILLA_VACIA) && (obtener_ficha_en(mi_fila, mi_columna) != FICHA_GRIS));

			while(haciendo_DMA != 0)		//TODO espera activa que nos ha dicho dario, retrasamos llegar a ella lo máximo posible
			{								//	pero con -O3 creo que esto va a dar dolor de cabeza
			}
			haciendo_DMA = 1;
			//DMA act ficha gris
			mover_gris_derecha(mi_fila, mi_columna, aux);	//Mover en la pantalla
		}
		if(haciendo_DMA == 0 && ev_timer > 0)
		{
			ev_timer--;
			//Actualizar tiempo total
			tiempo_total++;
			//DMA act pantalla con nuevos tiempos
			pintar_profiling(tiempo_total, tiempo_calc, tiempo_pv, veces_pv);
			haciendo_DMA = 1;
			iniciar_DMA();
		}
		if(ev_tp == 1)
		{
			ev_tp = 0;
			if(haciendo_DMA == 0)
			{
				unsigned int delta1 = timer2_leer();
				reversi_procesar(mi_fila, mi_columna);
				unsigned int delta2 = timer2_leer();
				tiempo_calc += (delta2-delta1);
				actualizar_movimientos_pantalla();		//Pone la ficha que hemos jugado, la ficha de la cpu, y voltea lo que haga falta
				//obtener datos de profiling
				tiempo_pv += reversi_t_pv();
				veces_pv += reversi_veces_pv();
				pintar_profiling(tiempo_total, tiempo_calc, tiempo_pv, veces_pv);
				haciendo_DMA = 1;
				iniciar_DMA();
				fin = obtener_fin();
			}
		}
		if(haciendo_DMA == 0 && fin != no_fin)
		{
			haciendo_DMA = 1;
			if(fin == jugador_gana)
			{
				pintar_fin_victoria(contar_blancas(), contar_negras());
			}
			else if(fin == cpu_gana)
			{
				pintar_fin_derrota(contar_blancas(), contar_negras());
			}
			else	//En caso de empate
			{
				pintar_fin_empate(contar_blancas(), contar_negras());
			}
			jugada_botones = Fin;
		}
		break;
	default:	//case Fin
		if(haciendo_DMA == 0 && ev_tp == 1)		//Reiniciar partida, restaurando el estado inicial del autómata
		{
			inicializar_jugada_botones();
		}
		break;
	}
}

//Se llama cuando se pulsa en tsp, confirmando la jugada
void jugada_ev_tsp()
{
	ev_tp = 1;
	jugada_por_botones();
}

//Se llama cuando el botón derecho es pulsado, para tratar el evento
void jugada_ev_der()
{
	ev_bot_der = 1;
	jugada_por_botones();
}

//Se llama cuando, por autoincremento o por pulsación nueva,
//	hay que incrementar el valor del 8Led y la variable asociada a él.
void jugada_ev_izq()
{
	ev_bot_izq = 1;
	jugada_por_botones();
}
//Se llama cuando haya que incrementar la cuenta de tiempo de juego en segundos
void jugada_ev_timer()
{
	ev_timer++;
	jugada_por_botones();
}

//Se llama para avisar al autómata de que la transferencia por DMA ha terminado
void termina_DMA()
{
	haciendo_DMA = 0;
	jugada_por_botones();
}

void actualizar_movimientos_pantalla()
{
	obtener_tablero(tablero_actual);		//TODO hay que hacer la funcion
	int i,j;
	for(i = 0; i < num_filas; i++)
	{
		for(j = 0; j < num_columnas; j++)
		{
			if(tablero_actual[i][j] != tablero_anterior[i][j])
			{
				if(tablero_anterior[i][j] != CASILLA_VACIA)
				{
					borrar_ficha(i,j);
				}
				pintar_ficha(i, j, tablero_actual[i][j]);
				tablero_anterior[i][j] = tablero_actual[i][j];
			}
		}
	}
}
