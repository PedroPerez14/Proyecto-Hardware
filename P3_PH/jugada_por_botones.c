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
#include "button.h"
#include "codigos_eventos.h"
#include "reversi8_2019.h"
#include "jugada_por_botones.h"
#include "elementos_pantalla.h"
#include "definiciones_juego.h"
#include "lcd.h"					//Se incluye por haciendo_DMA, como nos ha dicho Dar�o
/*--- variables del m�dulo ---*/

static enum estados{Reglas, Jugando, Fin} jugada_botones;
static int ev_bot_izq = 0;			//Indica cu�ndo hay que actualizar la posici�n de la ficha gris, por nueva pulsaci�n del bot�n izquierdo o porque hay que autoincrementar, puede valer m�s de 1
static int ev_bot_der = 0;			//Flag que se�aliza cu�ndo han pulsado el bot�n derecho, y cu�ntas veces
static int ev_tp = 0;				//Indica que hay que atender un toque del touchpad de alguien
static char mi_fila = 0;			//La fila donde queremos poner ficha
static char mi_columna = 0;			//La columna donde queremos poner ficha
volatile int haciendo_DMA = 0;		//Si estamos haciendo un DMA al LCD
static int ev_timer = 0;			//Interrupci�n del timer que indica que hay que refrescar tiempos en pantalla
static enum final_partida final;	//Determina si la partida ha terminado, y c�mo lo ha hecho: Por empate, victoria del jugador o de la CPU
/* PROFILING */
static int tiempo_calc = 0;
volatile static int tiempo_total = 0;
static int tiempo_pv = 0;
static int veces_pv = 0;
/* TABLEROS PARA ACTUALIZAR SOLO LAS FICHAS NECESARIAS */
static char tablero_actual[num_filas][num_columnas];		//Aqu� se guardar�n las fichas despu�s de que la cpu haga su movimiento
static char tablero_anterior[num_filas][num_columnas];		//Aqu� se guardar�n las fichas antes de que la cpu haga su movimiento
															//Se har� la diferencia, y solo se pintar�n aquellas que cambien.
															//Tambi�n, despu�s de esto, anterior = actual, para el pr�ximo turno.


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
	final = no_fin;
	tiempo_calc = 0;
	tiempo_total = 0;
	tiempo_pv = 0;
	veces_pv = 0;
	obtener_tablero(tablero_actual);
	obtener_tablero(tablero_anterior);
	jugada_botones = Reglas;
	pintar_reglas();
}

//Hace los c�lculos necesarios para mover la ficha gris a la derecha en pantalla
void calcular_mov_gris_der()
{
	if((contar_blancas() + contar_negras()) == num_filas * num_columnas)	//Si no hay que terminar la partida
	{
		contar_fichas_final();
	}
	else
	{
		volatile int aux = mi_fila;
		volatile int aux2 = mi_columna;
		mi_fila = ((mi_fila + 1) % 8);
		while((obtener_ficha_en(mi_fila, mi_columna) == FICHA_BLANCA) || (obtener_ficha_en(mi_fila, mi_columna) == FICHA_NEGRA) && mi_fila != aux)
		{
			mi_fila = ((mi_fila + 1) % 8);			//8 porque hay 8 columnas en el tablero, [0-7], 0x7 = num_columnas
		}
		if(mi_fila == aux)							//Si la ficha gris se queda atrapada
		{
			volatile int i;
			volatile int j;
			for(i = 0; i < num_filas; i++)			//se recorre el tablero buscando otra posici�n libre y
			{										// se mueve a esa casilla libre para ver si se puede 'liberar'
				for(j = 0; j < num_columnas; j++)
				{
					if(tablero_actual[i][j] == CASILLA_VACIA)
					{
						mi_fila = i;
						mi_columna = j;
					}
				}
			}
		}
		while(haciendo_DMA != 0)
		{
		}
		haciendo_DMA = 1;
		//DMA act ficha gris
		mover_gris(mi_fila, mi_columna, aux, aux2);		//Mover en la pantalla
	}
}

void calcular_mov_gris_izq()
{
	if((contar_blancas() + contar_negras()) == num_filas * num_columnas)	//Si no hay que terminar la partida
	{
		contar_fichas_final();
	}
	else
	{
		volatile int aux = mi_columna;
		volatile int aux2 = mi_fila;
		mi_columna = ((mi_columna + 1) % 8);
		while((obtener_ficha_en(mi_fila, mi_columna) == FICHA_BLANCA) || (obtener_ficha_en(mi_fila, mi_columna) == FICHA_NEGRA) && mi_columna != aux)
		{
			mi_columna = ((mi_columna + 1) % 8);		//8 porque hay 8 columnas en el tablero, [0-7], 0x7 = num_filas
		}
		if(mi_columna == aux)							//Si la ficha gris se queda atrapada
		{
			volatile int i;
			volatile int j;
			for(i = 0; i < num_filas; i++)			//se recorre el tablero buscando otra posici�n libre y
			{										// se mueve a esa casilla libre para ver si se puede 'liberar'
				for(j = 0; j < num_columnas; j++)
				{
					if(tablero_actual[i][j] == CASILLA_VACIA)
					{
						mi_fila = i;
						mi_columna = j;
					}
				}
			}
		}

		while(haciendo_DMA != 0)
		{
		}
		haciendo_DMA = 1;
		mover_gris(mi_fila, mi_columna, aux2, aux);	//Mover en la pantalla
	}
}

//Funci�n encargada de gestionar la confirmaci�n de movimientos del usuario
void procesar_jugada()
{
	unsigned int delta1 = timer2_leer();
	reversi_procesar(mi_fila, mi_columna);		//Calcula la jugada de la IA y calcula tiempos de procesamiento
	unsigned int delta2 = timer2_leer();
	tiempo_calc += (delta2-delta1);
	if(obtener_jugada_valida() == 1)
	{
		borrar_ficha(mi_fila, mi_columna);
		actualizar_movimientos_pantalla();		//Pone la ficha que hemos jugado, la ficha de la cpu, y voltea lo que haga falta
		reiniciar_posicion_gris();				//Devuelve la ficha gris a 0,0, y si est� ocupada, a la primera casilla libre
		//obtener datos de profiling
		tiempo_pv += reversi_t_pv();
		veces_pv += reversi_veces_pv();
		pintar_profiling(tiempo_total, tiempo_calc, tiempo_pv, veces_pv);
		haciendo_DMA = 1;
		iniciar_DMA();
	}
	else
	{
		tiempo_pv += reversi_t_pv();
		veces_pv += reversi_veces_pv();
		pintar_profiling(tiempo_total, tiempo_calc, tiempo_pv, veces_pv);
		haciendo_DMA = 1;
		iniciar_DMA();
	}
	final = obtener_fin();
}

void pintar_fin()
{
	if(final == jugador_gana)
	{
		pintar_fin_victoria(contar_blancas(), contar_negras());
	}
	else if(final == cpu_gana)
	{
		pintar_fin_derrota(contar_blancas(), contar_negras());
	}
	else	//En caso de empate
	{
		pintar_fin_empate(contar_blancas(), contar_negras());
	}
}

//El aut�mata de estados como tal
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
		if(haciendo_DMA == 0 && final != no_fin)
		{
			haciendo_DMA = 1;
			pintar_fin();
			jugada_botones = Fin;
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
		if(ev_bot_der == 1)		//Incrementar fila
		{
			ev_bot_der = 0;
			calcular_mov_gris_der();
		}
		if(ev_bot_izq == 1)		//Incrementar columna
		{
			ev_bot_izq = 0;
			calcular_mov_gris_izq();
		}
		if(ev_tp == 1)
		{
			ev_tp = 0;
			if(haciendo_DMA == 0)
			{
				procesar_jugada();
			}
		}
		break;
	default:	//case Fin
		if(haciendo_DMA == 0 && ev_tp == 1)
		//Reiniciar partida, restaurando el estado inicial del aut�mata
		{
			Lcd_Clr();
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

//Se llama cuando el bot�n derecho es pulsado, para tratar el evento
void jugada_ev_der()
{
	ev_bot_der = 1;
	jugada_por_botones();
}

//Se llama por autoincremento o por pulsaci�n nueva
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

//Se llama para avisar al aut�mata de que la transferencia por DMA ha terminado
void termina_DMA()
{
	haciendo_DMA = 0;
	jugada_por_botones();
}


//Se llama tras hacer un movimiento
//	Devuelve la ficha gris a 0,0 o a la siguiente posici�n libre del tablero,
//	para hacer la siguiente jugada.
void reiniciar_posicion_gris()
{
	mi_fila = 0;
	mi_columna = 0;
	int i,j;
	int fin_bucle = 0;
	for(i = 0; i < num_filas && fin_bucle == 0; i++)
	{
		for(j = 0; j < num_columnas && fin_bucle == 0; j++)
		{
			if(tablero_actual[i][j] == CASILLA_VACIA)
			{
				mi_fila = i;
				mi_columna = j;
				fin_bucle = 1;
			}
		}
	}
	pintar_ficha(mi_fila, mi_columna, FICHA_GRIS);
}	

void actualizar_movimientos_pantalla()
{
	obtener_tablero(tablero_actual);
	int i,j;
	for(i = 0; i < num_filas; i++)
	{
		for(j = 0; j < num_columnas; j++)
		{
			if(tablero_actual[i][j] != tablero_anterior[i][j])
			{	//Si algo ha cambiado al mover lo actualizamos
				borrar_ficha(i,j);
				pintar_ficha(i, j, tablero_actual[i][j]);
				//Y de paso actualizamos esto para la siguiente vez
				tablero_anterior[i][j] = tablero_actual[i][j];
			}
		}
	}
}

void jugada_ev_tecla0(void)		//Pasar turno de jugador
{
	if(jugada_botones == Jugando)
	{
		unsigned int delta1 = timer2_leer();
		mover_IA();
		unsigned int delta2 = timer2_leer();
		tiempo_calc += (delta2-delta1);
		borrar_ficha(mi_fila, mi_columna);
		actualizar_movimientos_pantalla();
		//Pone la ficha de la cpu, y voltea lo que haga falta
		reiniciar_posicion_gris();
		//Devuelve la ficha gris a 0,0, y si est� ocupada, a la primera casilla libre
		tiempo_pv += reversi_t_pv();	//obtener datos de profiling
		veces_pv += reversi_veces_pv();
		pintar_profiling(tiempo_total, tiempo_calc, tiempo_pv, veces_pv);
		haciendo_DMA = 1;
		iniciar_DMA();
		final = obtener_fin();
	}
}

void contar_fichas_final()		//Para saber qu� final poner, en caso de que el tablero est� lleno o se aborte la partida
{
	int blancas = contar_blancas();
	int negras = contar_negras();
	if(blancas > negras)
	{
		final = cpu_gana;
	}
	else if(blancas < negras)
	{
		final = jugador_gana;
	}
	else	//En caso de empate
	{
		final = empate;
	}
}

void jugada_ev_tecla1()			//Finalizar la partida de forma prematura
{
	contar_fichas_final();
	jugada_por_botones();
}
