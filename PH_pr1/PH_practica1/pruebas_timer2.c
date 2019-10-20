/*********************************************************************************************
* Fichero:	pruebas_timer2.c
* Autor:	Fernando Peña Bes (NIA: 756012)
			Pedro José Pérez García (NIA: 756642)
* Descrip:	Fichero de pruebas para el timer2. Realiza 10 medidas de tiempo para los casos de 1ms, 10ms, 1s y 10s.
			También calcula el valor medio de las 10 mediciones de cada caso
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "pruebas_timer2.h"

/*--- codigo de funciones ---*/
void pruebas_timer2(void) {
	timer2_inicializar();	//Para la prueba se inicializa el timer2

	int i;
	unsigned int suma_1ms = 0;
	unsigned int medidas_1ms[10];
	for(i=0; i < 10; i = i + 1)
	{
		timer2_empezar();
		unsigned int t_0 = timer2_leer();
		Delay(10);
		unsigned int t_1 = timer2_parar();
		medidas_1ms[i] = t_1 - t_0;
		suma_1ms += medidas_1ms[i];
	}
	unsigned int media_1ms = suma_1ms / 10;

	unsigned int suma_10ms = 0;
	unsigned int medidas_10ms[10];
	for(i=0; i < 10; i = i + 1)
	{
		timer2_empezar();
		unsigned int t_0 = timer2_leer();
		Delay(100);
		unsigned int t_1 = timer2_parar();
		medidas_10ms[i] = t_1 - t_0;
		suma_10ms += medidas_10ms[i];
	}
	unsigned int media_10ms = suma_10ms / 10;

	unsigned int suma_1s = 0;
	unsigned int medidas_1s[10];
	for(i=0; i < 10; i = i + 1)
	{
		timer2_empezar();
		unsigned int t_0 = timer2_leer();
		Delay(10000);
		unsigned int t_1 = timer2_parar();
		medidas_1s[i] = t_1 - t_0;
		suma_1s += medidas_1s[i];
	}
	unsigned int media_1s = suma_1s / 10;

	unsigned int suma_10s = 0;
	unsigned int medidas_10s[10];
	for(i=0; i < 10; i = i + 1)
	{
		timer2_empezar();
		unsigned int t_0 = timer2_leer();
		Delay(100000);
		unsigned int t_1 = timer2_parar();
		medidas_10s[i] = t_1 - t_0;
		suma_10s += medidas_10s[i];
	}
	unsigned int media_10s = suma_10s / 10;
}
