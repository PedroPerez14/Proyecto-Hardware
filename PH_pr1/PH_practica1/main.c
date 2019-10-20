/*********************************************************************************************
* Fichero:	main.c
* Autor:	Profesores de la asignatura
*			Fernando Peña Bes (NIA: 756012)
*			Pedro José Pérez García (NIA: 756642)
* Descrip:	punto de entrada de C
* Version:  <P4-ARM.timer-leds>
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "8led.h"
#include "button.h"
#include "led.h"
#include "timer.h"
#include "44blib.h"
#include "44b.h"
#include "reversi8_2019.h"
#include "pruebas_timer2.h"

/*--- variables globales ---*/


/*--- codigo de funciones ---*/
void Main(void)
{
	/* Inicializa controladores */
	sys_init();         // Inicializacion de la placa, interrupciones y puertos
	timer_init();	    // Inicializacion del temporizador
	Eint4567_init();	// inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led
	D8Led_init();       // inicializamos el 8led


	//reversi8();	// función principal del juego


	//// pruebas automáticas de timer2 ////
	//pruebas_timer2();

	//// pruebas automáticas de patron_volteo ////
	//patron_volteo_prueba_resultado_igual();
	//patron_volteo_prueba_resultado_correcto();


	/* Valor inicial de los leds */
	//leds_off();
	//led1_on();

	while (1)
	{
		/* Cambia los leds con cada interrupcion del temporizador */
		if (switch_leds == 1)
		{
			leds_switch();
			switch_leds = 0;
		}
	}
}
