/*********************************************************************************************
* Fichero:		botones_antirebotes.c
* Autor:		Pedro José Pérez García (756642) y Fernando Peña Bes (756012)
* Descrip:		Máquina de estados que elimina los rebotes de los pulsadores.
* 				Las rutinas llamadas desde reversi_main invocan la función del autómata,
*				que comprueba si debe realizar alguna acción o no.
*				También se incluyen funciones que ayudan a la gestión de los rebotes.
* Version:		
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "led.h"
#include "44b.h"
#include "44blib.h"
#include "botones_antirebotes.h"
#include "button.h"
//TODO los includes van a fallar casi seguro, pero es fácil de arreglar

/*--- variables del módulo ---*/

#define trp_en_ticks_timer0 50;			//Se puede cambiar para ajustarse a cada placa
#define trd_en_ticks_timer0 50;			//Se puede cambiar también
#define ticks_para_30ms 2;				//TODO estos defines junto con alguno del timer0 se pueden poner en un .h de config y que se calcule todo a partir de ahí, supongo (?)

static enum estados{q0, q1, q2, q3} maquina_estados; 
static int atendiendoPulsacion = 0;		//Inicialmente no se está atendiendo ninguna pulsación, por eso se inicializa a 0
static int cuenta_ticks;
static enum estado_button que_button;

void botones_antirebotes_inicializar(enum estado_button que_boton)
{
	maquina_estados = q0;
	que_button = que_boton;
	atendiendoPulsacion = 1;
}

void antirebotes(void)
{
	switch (maquina_estados)
	{
		case q0 :
				if(atendiendoPulsacion)
				{
					bloquear_IRQ_button(); //TODO pongo la función aquí o en button.c para que esto sea más modular y de alto nivel?
					cuenta_ticks = 0;						
					maquina_estados = q1;
				}
			break;
		case q1 :
				if(cuenta_ticks == trp_en_ticks_timer0)
				{	//Volvemos a usar timer0 para encuestar cada 30ms a1 botón, esperando que deje de estar oprimido
					cuenta_ticks = 0;
					maquina_estados = q2;
				}
			break;
		case q2:
					//TODO ticks para 30ms ????? -> debería inicializarlo a 2, porque 1 "tick" entra cada 16ms por como tengo timer0 y necesito 30ms, ¿la precisión es aceptable, cambio timer0 o uso otro timer?
				if(cuenta_ticks == ticks_para_30ms && button_estado() != que_button)					
				{	//Si es hora de encuestar al botón y vemos que ya no está presionado
					cuenta_ticks = 0;
					maquina_estados = q3;
				}
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
{	//Solo se hace algo si no se está gestionando otra pulsación
	if(!atendiendoPulsacion)
	{
		botones_antirebotes_inicializar(boton);
		antirebotes();					//Se avisa a la máquina de estados para que inicie su ejecución
	}
}

void button_ev_tick0(void)
{	//Solo se incrementa el contador si es útil para la máquina de estados
	if(atendiendoPulsacion)		
	{
		cuenta_ticks += 1;		
		antirebotes();					//Se avisa a la máquina de estados por si se activa alguna transición
	}
}
