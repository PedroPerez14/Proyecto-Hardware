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
#include "jugada_por_botones.h"
#include "tsp_antirebotes.h"
#include "teclado_antirebotes.h"
#include "keyboard.h"
#include <stdint.h>

/*--- variables ---*/
static enum {ticks_latido_A = 7, ticks_latido_B = 8, ticks_segundo_de_juego = 60};
static int cuenta_int_latido;
static int cuenta_int_t_juego;
static char estado_led1;

/*--- Código de funciones ---*/

void reversi_main_inicializar()
{
	//Inicializar las variables que hagan falta para proesar bien los eventos
	timer_init();
	cuenta_int_t_juego = 0;	//Cada 60 interrupciones de timer0 pasa un segundo, y tendremos que aumentar la cuenta en la pantalla
	estado_led1 = 0;		//Para decidir si hay que esperar 7 u 8 eventos de tipo tick_latido (en la teoría es cada 7.5)
							//Porque timer0 interrumpe 60 veces/seg. y el led debe latir(encender y apagar) 4 veces por segundo
							// 	por tanto 60/4 = 15, y 15/2 = 7.5, el número de interrupciones del timer
							//	tras el que debe cambiar su estado (on/off)
							//También codifica el estado del led1 si estado_led1 == 0, codifica apagado y 1, encendido

	cuenta_int_latido = 0;	//Cada 7 u 8 hay que cambiar el led izquierdo, como ya se ha explicado
	led1_off();				//El led empieza apagado
	botones_antirebotes_inicializar();
	tsp_antirebotes_inicializar();
	init_keyboard();
	tec_antirebotes_inicializar();
	inicializar_jugada_botones();
}

void dormir_procesador(void)
{
	;						//La función que más me ha costado hacer en toda mi vida
}

void Latido_ev_new_tick(void)
{
	cuenta_int_latido += 1;
	if(estado_led1 == 0)	//Unas veces 7 y otras 8, 7.5 de media
	{
		if(cuenta_int_latido == ticks_latido_A)
		{
			estado_led1 = 1;
			led1_on();
			cuenta_int_latido = 0;
		}
	}
	else
	{
		if(cuenta_int_latido == ticks_latido_B)
		{
			estado_led1 = 0;
			led1_off();
			cuenta_int_latido = 0;
		}
	}
}

void tiempo_juego_ev_tick()
{
	cuenta_int_t_juego++;
	if(cuenta_int_t_juego == ticks_segundo_de_juego)
	{
		cuenta_int_t_juego = 0;
		jugada_ev_timer();
	}
}

void reversi_main()
{
	reversi_main_inicializar();
	while(1)
	{
		while(!esVacia())
		{
			//Desencolar para poder procesar la información
			uint8_t evento;
			uint32_t info;
			pop_debug(&evento, &info);
			switch(evento)
			{
			case ev_tick_timer0 :	//Atender eventos de timer0
				Latido_ev_new_tick();
				tiempo_juego_ev_tick();
				button_ev_tick0();
				tsp_ev_tick0();
				tec_ev_tick0();
				break;
			case ev_button_int :	//Atender eventos de los botones
				if(info == button_izq)
				{	//Botón izquierdo
					button_ev_pulsacion(button_iz);
					jugada_ev_izq();
				}
				else
				{	//Botón derecho
					button_ev_pulsacion(button_dr);
					jugada_ev_der();
				}
				break;
			case ev_finLCD:
				termina_DMA();
				break;
			case ev_tsp:
				tsp_ev_pulsacion();
				jugada_ev_tsp();
				break;
			case ev_keyboard:
				if(info == tecla_0)
				{	//tecla 0
					tec_ev_pulsacion();
					jugada_ev_tecla0();
				}
				else
				{	//tecla 1
					tec_ev_pulsacion();
					jugada_ev_tecla1();
				}
				break;
			default : //Si es otra cosa desconocida, no lo atendemos
				break;
			}
		}
		dormir_procesador();
	}
}




