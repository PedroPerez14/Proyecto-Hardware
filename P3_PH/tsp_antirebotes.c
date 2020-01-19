/*********************************************************************************************
* Fichero:		tsp_antirebotes.c
* Autor:		Pedro José Pérez García (756642) y Fernando Peña Bes (756012)
* Descrip:		Máquina de estados que elimina los rebotes de la pantalla táctil.
* Version:		
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "led.h"
#include "44b.h"
#include "44blib.h"
#include "tsp_antirebotes.h"
#include "tp.h"
#include "cola_depuracion.h"
#include "codigos_eventos.h"
#include "jugada_por_botones.h"

/*--- variables del módulo ---*/
enum {t_espera_ticks_timer0 = 30};
static int cuenta_ticks_tsp = 0;
static int atendiendo_pulsacion_tsp = 0;
/*
   Los valores se pueden cambiar en función de la placa para controlar mejor los rebotes
	Desactivamos las interrupciones durante 30 ticks = 30 * 1/60 seg = 0.5 segundos.
*/
static enum estados_tsp_antirebotes{Inicio, deshabilitadas_int} maquina_estados_tsp;
//Estados de la máquina de estados

void tsp_antirebotes_inicializar()
{
	maquina_estados_tsp = Inicio;
	cuenta_ticks_tsp = 0;
	atendiendo_pulsacion_tsp = 0; 		//Inicialmente no se está atendiendo
										//ninguna pulsación, por eso se inicializa a 0
}

void tsp_antirebotes(void)
{
	switch(maquina_estados_tsp)
	{
		case Inicio :
			if(atendiendo_pulsacion_tsp)
			{
				//Las interrupciones de tsp se deshabilitan en el propio tsp por si acaso
				maquina_estados_tsp = deshabilitadas_int;
			}
			break;
		default: //Si estamos en deshabilitadas_int
			if(cuenta_ticks_tsp == t_espera_ticks_timer0)
			{	//Si ha pasado trd, rehabilitamos interrupciones tsp
				// y volvemos a admitir el procesado de otras pulsaciones
				tsp_resetear();
				tsp_antirebotes_inicializar();
			}
			break;
	}
}

void tsp_ev_pulsacion()
{
	if(!atendiendo_pulsacion_tsp)
	{
		atendiendo_pulsacion_tsp = 1;
		tsp_antirebotes();					//Se avisa a la máquina de estados para que inicie su ejecución
	}
}

void tsp_ev_tick0(void)
{	//Solo se incrementa el contador si es útil para la máquina de estados
	//	por tanto, si no estamos atendiendo ningún evento de pulsación no
	//	nos molestamos en hacer nada
	if(atendiendo_pulsacion_tsp)
	{
		cuenta_ticks_tsp += 1;
		tsp_antirebotes();
	}
}
