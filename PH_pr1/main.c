/*********************************************************************************************
* Fichero:	main.c
* Autor:	Profesores de la asignatura, Fernando Peña Bes, Pedro José Pérez
* Descrip:	Punto de entrada de C, preparado para realizar 10 medidas del timer, para los casos de 1ms, 10ms, 1s y 10s.
			También calcula el valor medio de las 10 mediciones de cada caso
* Version:  <P4-ARM.timer-leds>
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "8led.h"
#include "button.h"
#include "led.h"
#include "timer.h"
#include "44blib.h"
#include "44b.h"
#include "timer2.h"

/*--- variables globales ---*/


/*--- codigo de funciones ---*/
void Main(void)
{
	/* Inicializa controladores */
	sys_init();        		// Inicializacion de la placa, interrupciones y puertos
	timer_init();	  	 	// Inicializacion del temporizador
	Eint4567_init();		// inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led
	D8Led_init();       	// inicializamos el 8led
	timer2_inicializar();	//Para la prueba se inicializa el timer2

	int i = 0;
	unsigned int suma_1ms = 0;
	unsigned int medidas_1ms[10];
	for(i; i < 10; i = i + 1)
	{
		timer2_empezar();
		unsigned int t_0 = timer2_leer();
		Delay(10);
		unsigned int t_1 = timer2_parar();
		medidas_1ms[i] = t_1 - t_0;
		suma_1ms += medidas_1ms[i];
	}
	unsigned int media_1ms = suma_1ms / 10;

	i = 0;
	unsigned int suma_10ms = 0;
	unsigned int medidas_10ms[10];
	for(i; i < 10; i = i + 1)
	{
		timer2_empezar();
		unsigned int t_0 = timer2_leer();
		Delay(100);
		unsigned int t_1 = timer2_parar();
		medidas_10ms[i] = t_1 - t_0;
		suma_10ms += medidas_10ms[i];
	}
	unsigned int media_10ms = suma_10ms / 10;

	i = 0;
	unsigned int suma_1s = 0;
	unsigned int medidas_1s[10];
	for(i; i < 10; i = i + 1)
	{
		timer2_empezar();
		unsigned int t_0 = timer2_leer();
		Delay(10000);
		unsigned int t_1 = timer2_parar();
		medidas_1s[i] = t_1 - t_0;
		suma_1s += medidas_1s[i];
	}
	unsigned int media_1s = suma_1s / 10;

	i = 0;
	unsigned int suma_10s = 0;
	unsigned int medidas_10s[10];
	for(i; i < 10; i = i + 1)
	{
		timer2_empezar();
		unsigned int t_0 = timer2_leer();
		Delay(100000);
		unsigned int t_1 = timer2_parar();
		medidas_10s[i] = t_1 - t_0;
		suma_10s += medidas_10s[i];
	}
	unsigned int media_10s = suma_10s / 10;


	//A partir de aquí es código proporcionado por los profesores
	/* Valor inicial de los leds */
	leds_off();
	led1_on();

	reversi8();

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
