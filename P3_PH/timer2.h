/*********************************************************************************************
* Fichero:		timer2.h
* Autor:		Fernando Pe�a Bes (NIA: 756012)
* 				Pedro Jos� P�rez Garc�a (NIA: 756642)
* Descrip:		Funciones de control del timer0 del s3c44b0x
*********************************************************************************************/

#ifndef _TIMER2_H_
#define _TIMER2_H_

/*--- variables visibles del m�dulo timer2.c/timer2.h ---*/

enum
{
	COMP_VALUE = 0,
	PERIOD_INT = 2048, 			//2047.96 microsegundos entre dos interrupciones del timer2
	CYCLES_EACH_MICROSEC = 32, 	//Ticks del reloj necesarios pra que pase un microsegundo
	TICKS_FOR_A_SECOND = 488, 	//Tienen que pasar 488.28 ciclos de 32 ticks para que pase un segundo
	MAX_COUNT = 65535 			//Valor del contador de timer2, el valor m�ximo de un registro de 16 bits.
};

/*--- declaracion de funciones visibles del m�dulo timer.c/timer.h ---*/

void timer2_inicializar(void);

void timer2_empezar(void);

unsigned int timer2_leer(void);

unsigned int timer2_parar(void);

#endif /* _TIMER2_H_ */
