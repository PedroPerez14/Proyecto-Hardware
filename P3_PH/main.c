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
#include "rutinas_excepciones.h"
#include "cola_depuracion.h"
#include "reversi_main.h"
#include "tp.h"
#include "lcd.h"
#include "elementos_pantalla.h"
#include <stdint.h>

inline void pasar_a_user()
{
	uint32_t _cpsr;
	asm volatile (" mrs %0, cpsr\n" : "=r" (_cpsr));
	_cpsr &= 0xFFFFFFE0;
	_cpsr |= 0x00000010;
	asm volatile (" msr cpsr,%0\n" :: "r" (_cpsr));
}

/*--- codigo de funciones ---*/
//extern void Lcd_Test();
void Main(void)
{
	/* Inicializa controladores */
	sys_init();         				// Inicializacion de la placa, interrupciones y puertos
	timer_init();	    				// Inicializacion del temporizador
	button_iniciar();					// inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led
	D8Led_init();      					// inicializamos el 8led
	cola_depuracion_inicializar(32);	// Debe darse un tamaño suficiente para almacenar todos los eventos de un ciclo
	Gestion_excepciones_init();			// Inicializa la gestión de excepciones SWI, UNDEF y DABORT
	Lcd_Init();
	Lcd_Clr();
	Lcd_Active_Clr();
	TS_init();


	/// Pruebas de excepciones ///
	//asm volatile ("SWI 0x55");			//lanzar SWI
	//asm volatile ("mov r3, #3");
	//asm volatile ("ldr r2,[r3]");			//Forzar DABORT
	//asm volatile (".word 0xe7f000f0\n");	//Forzar UNDEF

	pasar_a_user();
	pintar_prueba();
	reversi_main();
//41


	//// pruebas automáticas de timer2 ////
	//pruebas_timer2();

	//// pruebas automáticas de patron_volteo ////
	//patron_volteo_prueba_resultado_igual();
	//patron_volteo_prueba_resultado_correcto();


	/* Valor inicial de los leds */
	//leds_off();
	//led1_on();
}
