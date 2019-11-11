/*********************************************************************************************
* Fichero:		botones_antirebotes.c
* Autor:		Pedro Jos� P�rez Garc�a (756642) y Fernando Pe�a Bes (756012)
* Descrip:		M�quina de estados que elimina los rebotes de los pulsadores.
* 				Las rutinas llamadas desde reversi_main invocan la funci�n del aut�mata,
*				que comprueba si debe realizar alguna acci�n o no.
*				Tambi�n se incluyen funciones que ayudan a la gesti�n de los rebotes.
* Version:		
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "led.h"
#include "44b.h"
#include "44blib.h"
#include "botones_antirebotes.h"
#include "8led.h"
#include "button.h"
//TODO los includes van a fallar casi seguro, pero es f�cil de arreglar

/*--- variables del m�dulo ---*/

const int ticks_para_30ms = 2;			//TODO estos const int junto con alguno del timer0 se pueden poner en un .h de config y que se calcule todo a partir de ah�, supongo (?)
const int trp_en_ticks_timer0 = 2;		//Se puede cambiar para ajustarse a cada placa
const int trd_en_ticks_timer0 = 2;		//Se puede cambiar tambi�n



static enum estados{q0, q1, q2, q3} maquina_estados; 
static int atendiendoPulsacion = 0;		//Inicialmente no se est� atendiendo ninguna pulsaci�n, por eso se inicializa a 0
static int cuenta_ticks;				//Cuenta ticks de timer0 para gestionar los retardos de rebotes y de encuesta al bot�n
static enum estado_button que_button;	//Para saber qu� bot�n hemos pulsado y saber cu�l mirar a la hora de comprobar si se ha soltado
static int contador_debug = 0;			//Valor que aparecer� (mod 16) en el 8led.
static int cuenta_autoincremento;		//Cuenta ticks de timer0 para gestionar el autoincremento (pulsado > 1/3s y el incr. cada 180ms)
static int hay_autoincremento;			//Variable auxiliar de cuenta_autoincremento que sirve para ver cu�ndo estamos contando 1/3s o 180ms. Puede valer 0 o 1.

void cambiar_8led()
{
	if(que_button == button_iz)
	{
		contador_debug -= 1;
	}
	else if(que_button == button_dr)
	{
		contador_debug += 1;
	}
	D8Led_symbol(contador_debug & 0xf);
}

void botones_antirebotes_inicializar(enum estado_button que_boton)
{
	maquina_estados = q0;
	que_button = que_boton;
	atendiendoPulsacion = 1;
	cuenta_autoincremento = 0;
	hay_autoincremento = 0;
}

void antirebotes(void)
{
	switch(maquina_estados)
	{
		case q0 :
			if(atendiendoPulsacion)
			{
				bloquear_IRQ_button(); //TODO pongo la funci�n aqu� o en button.c para que esto sea m�s modular y de alto nivel?
				cuenta_ticks = 0;
				maquina_estados = q1;
			}
			break;
		case q1 :
			if(cuenta_ticks == trp_en_ticks_timer0)
			{	//Volvemos a usar timer0 para encuestar cada 30ms a1 bot�n, esperando que deje de estar oprimido
				cambiar_8led();
				cuenta_ticks = 0;
				maquina_estados = q2;
			}
			break;
		case q2:
				//TODO ticks para 30ms ????? -> deber�a inicializarlo a 2, porque 1 "tick" entra cada 16ms por como tengo timer0 y necesito 30ms, �la precisi�n es aceptable, cambio timer0 o uso otro timer?
			if(cuenta_ticks >= ticks_para_30ms)
			{//Si es hora de encuestar al bot�n
				if(button_estado() != que_button)
				{	 //y vemos que ya no est� presionado
					cuenta_ticks = 0;
					cuenta_autoincremento = 0;
					hay_autoincremento = 0;
					maquina_estados = q3;
				}
				//TODO esto es un poco guarro y ser�a mejor calcularlo relativo a la precision de timer0 y otras variables :)
				else if(cuenta_autoincremento == 20)	//Si seguimos pulsando despu�s de 1/3 de segundo, autoincremento cada 180ms
				{
					cuenta_autoincremento = 0;
					hay_autoincremento = 1;
				}
				if(hay_autoincremento && cuenta_autoincremento ==  11) //11 ticks de timer0 son 182.6 milisegs ~= 180 ms, para incrementar de forma auto
				{
					cuenta_autoincremento = 0;
					cambiar_8led();
				}
			}
			break;
		default: //Si estamos en q3
			if(cuenta_ticks == trd_en_ticks_timer0)
			{	//Si ha pasado trd, rehabilitamos interrupciones boton y volvemos a admitir el procesado de otras pulsaciones
				button_resetear();
				atendiendoPulsacion = 0;
			}
			break;
	}
}

void button_ev_pulsacion(enum estado_button boton)
{	//Solo se hace algo si no se est� gestionando otra pulsaci�n
	if(!atendiendoPulsacion)
	{
		botones_antirebotes_inicializar(boton);
		antirebotes();					//Se avisa a la m�quina de estados para que inicie su ejecuci�n
	}
}

void button_ev_tick0(void)
{	//Solo se incrementa el contador si es �til para la m�quina de estados
	if(atendiendoPulsacion)
	{
		cuenta_ticks += 1;
		cuenta_autoincremento += 1;
		antirebotes();					//Se avisa a la m�quina de estados por si se activa alguna transici�n
	}
}
