/*********************************************************************************************
* Fichero:	main.c
* Autor:	Profesores de la asignatura
*			Fernando Pe�a Bes (NIA: 756012)
*			Pedro Jos� P�rez Garc�a (NIA: 756642)
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
#include "rutinas_excepciones.h"
#include "pila_depuracion.h"
#include <stdint.h>
/*--- variables globales ---*/


/*--- codigo de funciones ---*/
void Main(void)
{
	/* Inicializa controladores */
	sys_init();         // Inicializacion de la placa, interrupciones y puertos
	timer_init();	    // Inicializacion del temporizador
	Eint4567_init();	// inicializamos los pulsadores. Cada vez que se pulse se ver� reflejado en el 8led
	D8Led_init();       // inicializamos el 8led
	pila_depuracion_inicializar(5);

	//Gestion_excepciones_init();
	//asm volatile ("SWI 0x55");			//lanzar SWI
	//asm volatile ("mov r3, #3");
	//asm volatile ("ldr r2,[r3]");			//Forzar DABORT
	//asm volatile (".word 0xe7f000f0\n");	//Forzar UNDEF


	//reversi8();	// funci�n principal del juego


	//// pruebas autom�ticas de timer2 ////
	//pruebas_timer2();

	//// pruebas autom�ticas de patron_volteo ////
	//patron_volteo_prueba_resultado_igual();
	//patron_volteo_prueba_resultado_correcto();


	/* Valor inicial de los leds */
	leds_off();
	led1_on();


	pop_debug();
	push_debug(0xFF,0xAAAAAAAA);
	pop_debug();
	pop_debug();
	push_debug(0xFF,0xAAAAAAAA);
	push_debug(0xFF,0xAAAAAAAA);
	push_debug(0xFF,0xAAAAAAAA);
	push_debug(0xFF,0xAAAAAAAA);
	push_debug(0xFF,0xAAAAAAAA);
	push_debug(0xFF,0xBBBBBBBB);
	pop_debug();
	push_debug(0xFF,0xBBBBBBBB);
	push_debug(0xFF,0xBBBBBBBB);
	push_debug(0xFF,0xBBBBBBBB);
	push_debug(0xFF,0xBBBBBBBB);
	pop_debug();
	pop_debug();
	pop_debug();
	pop_debug();
	pop_debug();
	push_debug(0xFF,0xCCCCCCCC);
	push_debug(0xFF,0xCCCCCCCC);
	push_debug(0xFF,0xCCCCCCCC);
	push_debug(0xFF,0xCCCCCCCC);
	push_debug(0xFF,0xCCCCCCCC);
	push_debug(0xFF,0xCCCCCCCC);



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
