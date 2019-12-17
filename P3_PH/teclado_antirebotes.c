/*********************************************************************************************
* Fichero:		teclado_antirebotes.h
* Autor:		Pedro Jos� P�rez Garc�a (756642) y Fernando Pe�a Bes (756012)
* Descrip:		M�quina de estados que elimina los rebotes del teclado matricial.
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "led.h"
#include "44b.h"
#include "44blib.h"
#include "teclado_antirebotes.h"
#include "keyboard.h"
#include "cola_depuracion.h"
#include "codigos_eventos.h"
#include "jugada_por_botones.h"

/*--- variables del m�dulo ---*/

static const int t_tec_espera_ticks_timer0 = 30;	//TODO para probarlo usa un valor exagerado
static int cuenta_ticks_tec = 0;
static int atendiendo_pulsacion_tec = 0;

/*
   Los valores se pueden cambiar en funci�n de la placa para controlar mejor los rebotes
	Desactivamos las interrupciones durante 30 ticks = 30 * 1/60 seg = 0.5 segundos.
*/
static enum estados_tec_antirebotes{Inicio, deshabilitadas_int} maquina_estados_tec;
//Estados de la m�quina de estados

void tec_antirebotes_inicializar()
{
	maquina_estados_tec = Inicio;
	cuenta_ticks_tec = 0;
	atendiendo_pulsacion_tec = 0; 			//Inicialmente no se est� atendiendo ninguna pulsaci�n, por eso se inicializa a 0
}

void tec_antirebotes(void)
{
	switch(maquina_estados_tec)
	{
		case Inicio :
			if(atendiendo_pulsacion_tec)
			{
				//Las interrupciones de tec se deshabilitan en el propio tec por si acaso
				maquina_estados_tec = deshabilitadas_int;
			}
			break;
		default: //Si estamos en deshabilitadas_int
			if(cuenta_ticks_tec == t_tec_espera_ticks_timer0)
			{	//Si ha pasado trd, rehabilitamos interrupciones tec y volvemos a admitir el procesado de otras pulsaciones
				tec_resetear();
				tec_antirebotes_inicializar();
			}
			break;
	}
}

void tec_ev_pulsacion()
{
	if(!atendiendo_pulsacion_tec)
	{
		atendiendo_pulsacion_tec = 1;
		tec_antirebotes();					//Se avisa a la m�quina de estados para que inicie su ejecuci�n
	}
}

void tec_ev_tick0(void)
{	//Solo se incrementa el contador si es �til para la m�quina de estados
	//	por tanto, si no estamos atendiendo ning�n evento de pulsaci�n no
	//	nos molestamos en hacer nada
	if(atendiendo_pulsacion_tec)
	{
		cuenta_ticks_tec += 1;
		tec_antirebotes();
	}
}
