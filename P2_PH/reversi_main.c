/*********************************************************************************************
* Fichero:		reversi_main.c
* Autor:		Fernando Peña Bes (NIA: 756012)
* 				Pedro José Pérez García (NIA: 756642)
* Descrip:		Bucle principal del reversi que gestiona los diferentes
* 				eventos que entran en la pila de depuración.
* 				También inicializa los periféricos necesarios para el juego.
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer2.h"
#include "44b.h"
#include "44blib.h"
#include "8led.h"
#include "button.h"
#include "led.h"
#include "timer.h"
#include "44blib.h"
#include "44b.h"
#include "reversi8_2019.h"
#include "pruebas_timer2.h"
#include "rutinas_excepciones.h"
#include "cola_depuracion.h"
#include "reversi_main.h"
#include "codigos_eventos.h"
#include "botones_antirebotes.h"
#include <stdint.h>

/*--- variables ---*/
static int cuenta_int_latido;
static char estado_led1;

/*--- Código de funciones ---*/

void reversi_inicializar(void)
{
	//Inicializar las variables que hagan falta para proesar bien los eventos
	timer_init();
	estado_led1 = 0;		//Para decidir si hay que esperar 7 u 8 eventos de tipo tick_latido (en la teoría es cada 7.5)
							//también codifica el estado del led1 si estado_led1 == 0, codifica apagado y 1, encendido
	cuenta_int_latido = 0;	//Cada 7 u 8 hay que cambiar el led izquierdo, variable de la cuenta
	led1_off();				//El led empieza apagado
}

void dormir_procesador(void)
{
	;						//La función que más me ha costado hacer en toda mi vida
}

void Latido_ev_new_tick(void)
{
	cuenta_int_latido += 1;
	if(estado_led1 == 0)
	{
		if(cuenta_int_latido == 7)
		{
			estado_led1 = 1;
			led1_on();
			cuenta_int_latido = 0;
		}
	}
	else
	{
		if(cuenta_int_latido == 8)
		{
			estado_led1 = 0;
			led1_off();
			cuenta_int_latido = 0;
		}
	}
}

void reversi_main()
{
	reversi_inicializar();
	while(1)
	{
		while(!esVacia())
		{
			//procesar
			uint8_t evento;
			uint32_t info;
			pop_debug(&evento, &info);
			switch(evento)
			{
			case ev_tick_timer0 :
				Latido_ev_new_tick();
				button_ev_tick0();
				break;
			case ev_button_int :
				if(info == button_izq)
				{
					button_ev_pulsacion(button_iz);
				}
				else
				{
					button_ev_pulsacion(button_dr);
				}
				break;
			default :
				break;
			}
		}
		dormir_procesador();
	}
}




