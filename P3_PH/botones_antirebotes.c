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
#include "8led.h"
#include "button.h"
#include "cola_depuracion.h"
#include "codigos_eventos.h"
#include "jugada_por_botones.h"

/*--- variables del módulo ---*/

static enum {TICKS_PARA_30MS = 2,trp_en_ticks_timer0 = 4, trd_en_ticks_timer0 = 4, int_timer0_enable_autoincr = 20, int_timer0_autoincr = 11};
/*Los valores de trp y trd se pueden cambiar en función de la placa para controlar mejor los rebotes
	Si seguimos pulsando después de 1/3 de segundo (= 20 interrupciones de timer0 * 1/60 seg. = 1/3 seg.), hay que empezar a autoincrementar
 		Por eso int_timer0_enable_autoincr = 20
	Hay que incrementar cada 180ms, que en interrupciones de timer0 es 180/16 ~= 11 <- int_timer0_autoincr

*/
static enum estados{Inicio, contando_trp, encuestando, contando_trd} maquina_estados;
static int atendiendoPulsacion;
static int cuenta_ticks = 0;			//Cuenta ticks de timer0 para gestionar los retardos de rebotes y de encuesta al botón
static enum estado_button que_button;	//Para saber qué botón hemos pulsado y saber cuál mirar a la hora de comprobar si se ha soltado
static int cuenta_autoincremento;		//Cuenta ticks de timer0 para gestionar el autoincremento (pulsado > 1/3s y el incr. cada 180ms)
static int hay_autoincremento;			//Variable auxiliar de cuenta_autoincremento que sirve para ver cuándo estamos contando 1/3s o 180ms. Puede valer 0 o 1.

void botones_antirebotes_inicializar()
{
	maquina_estados = Inicio;
	cuenta_autoincremento = 0;
	hay_autoincremento = 0;
	atendiendoPulsacion = 0; 			//Inicialmente no se está atendiendo ninguna pulsación, por eso se inicializa a 0
}

void antirebotes(void)
{
	switch(maquina_estados)
	{
		case Inicio :
			if(atendiendoPulsacion)
			{
				cuenta_ticks = 0;
				maquina_estados = contando_trp;
			}
			break;
		case contando_trp :
			if(cuenta_ticks == trp_en_ticks_timer0)
			{	//Volvemos a usar timer0 para encuestar cada 30ms a1 botón, esperando que deje de estar oprimido
				cuenta_ticks = 0;
				maquina_estados = encuestando;
			}
			break;
		case encuestando:
			if(cuenta_ticks >= TICKS_PARA_30MS)
			{//Si es hora de encuestar al botón
				if(button_estado() != que_button)
				{	 //y vemos que ya no está presionado
					cuenta_ticks = 0;
					cuenta_autoincremento = 0;
					hay_autoincremento = 0;
					maquina_estados = contando_trd;
				}
				if(cuenta_autoincremento == int_timer0_enable_autoincr)		//TODO poner autoincemento para boton derecho
				{	//Tras mantener pulsado durante 1/3 de segundo
					//	autoincremento cada 180ms a partir de ahora
					cuenta_autoincremento = 0;
					hay_autoincremento = 1;
					if(que_button == button_iz)
					{
						jugada_ev_izq();
					}
					else if(que_button == button_dr)
					{
						jugada_ev_der();
					}
				}
				if(hay_autoincremento && cuenta_autoincremento == int_timer0_autoincr) 	//11 ticks de timer0 son 182.6 milisegs ~= 180 ms, para incrementar de forma auto
				{
					cuenta_autoincremento = 0;
					//push_debug(ev_autoincr, no_info);
					//Lo hago como una llamada directa a jugada_por_botones porque la cola es concurrente y así se usa el mínimo posible

					if(que_button == button_iz)
					{
						jugada_ev_izq();
					}
					else if(que_button == button_dr)
					{
						jugada_ev_der();
					}
				}
			}
			break;
		default: //Si estamos en contando_trd
			if(cuenta_ticks == trd_en_ticks_timer0)
			{	//Si ha pasado trd, rehabilitamos interrupciones botón y volvemos a admitir el procesado de otras pulsaciones
				button_resetear();
				atendiendoPulsacion = 0;
				maquina_estados = Inicio;
				cuenta_ticks = 0;
			}
			break;
	}
}

void button_ev_pulsacion(enum estado_button boton)
{	//Solo se hace algo si no se está gestionando otra pulsación
	//	así aseguramos que solo se gestiona un evento de pulsación a la vez
	if(!atendiendoPulsacion)
	{
		que_button = boton;
		atendiendoPulsacion = 1;
		antirebotes();					//Se avisa a la máquina de estados para que inicie su ejecución
	}
}

void button_ev_tick0(void)
{	//Solo se incrementa el contador si es útil para la máquina de estados
	//	por tanto, si no estamos atendiendo ningún evento de pulsación no
	//	nos molestamos en hacer nada
	if(atendiendoPulsacion)
	{
		cuenta_ticks += 1;
		cuenta_autoincremento += 1;
		antirebotes();
		//Se avisa a la máquina de estados del cambio en las variables
		//	por si se activa alguna transición
	}
}
