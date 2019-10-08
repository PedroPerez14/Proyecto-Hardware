/*********************************************************************************************
* Fichero:		timer.h
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _TIMER2_H_
#define _TIMER2_H_

/*--- variables visibles del m�dulo timer2.c/timer2.h ---*/
static volatile unsigned int timer2_num_int;
enum
{
	COMP_VALUE = 0,
	PERIOD_INT = 2048, //2047.96 microsegundos entre dos interrupciones del timer2
	CYCLES_EACH_MICROSEC = 32, //Ticks del reloj necesarios pra que pase un microsegundo
	MAX_COUNT = 65535 //Valor del contador de timer2, el valor m�ximo de un registro de 16 bits.
};

/*--- declaracion de funciones visibles del m�dulo timer.c/timer.h ---*/

void timer2_inicializar(void);

void timer2_empezar(void);

unsigned int timer2_leer(void);

unsigned int timer2_parar(void);

#endif /* _TIMER2_H_ */
