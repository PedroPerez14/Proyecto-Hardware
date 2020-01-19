
/*********************************************************************************************
* Fichero:	rutinas_excepciones.c
* Autor: 	Fernando Peña Bes (756012) y Pedro José Pérez García (756642)
* Descrip:	Rutinas de tratamiento de las excepciones DAbort, Undefined y SWI
* Version:
*********************************************************************************************/
/*--- ficheros de cabecera ---*/
#include "rutinas_excepciones.h"
#include "8led.h"
#include "led.h"
#include "44blib.h"
#include "44b.h"
#include "def.h"
#include "debug.h"
#include <stdint.h>

/*--- variables globales del módulo (hacen falta?)---*/
volatile static int hay_excepcion;
volatile uint32_t causa_fallo;
/*--- Declaraciones de las diferentes rutinas para tratamiento de las excepciones ---*/
volatile void Gestion_excepciones(void) __attribute__((interrupt("ABORT")));
volatile void Gestion_excepciones(void) __attribute__((interrupt("SWI")));
volatile void Gestion_excepciones(void) __attribute__((interrupt("UNDEF")));

//Ignorar warning, sí que devuelve lo que debe, pero sin hacer return en C como tal
volatile uint32_t __get_CPSR() 					//Devuelve CPSR
	{
		__asm volatile ("MRS r0, CPSR");
		__asm volatile ("bx lr");
	}

void __attribute__((optimize("O0"))) parpadear_error(int caracter)		//8-led se queda parpadeando con el código del error
{																		//según lo definido en el enum del .h
	while(1)
	{
		D8Led_symbol(caracter);
		Delay(2500);
		D8Led_symbol(16);
		Delay(1250);
	}
}

volatile void Gestion_excepciones(void)
{
	asm volatile(" mov %0,lr\n" : "=r" (causa_fallo));
	volatile uint32_t cpsr = __get_CPSR();
	if((cpsr & 0x0000001F) == 0x1b) 		//Si Estamos en modo Undef
	{
		hay_excepcion = UNDEF;
		causa_fallo = causa_fallo - 4;
		parpadear_error(UNDEF);
	}
	else if((cpsr & 0x0000001F) == 0x13) 	//Si Estamos en modo SVC de SWI
	{
		hay_excepcion = SWI;
		causa_fallo = causa_fallo - 4;
		parpadear_error(SWI);
	}
	else									//Estamos en modo ABORT
	{
		hay_excepcion = DABORT;
		causa_fallo = causa_fallo - 8;
		parpadear_error(DABORT);
	}
}

//Inicializa la gestión de las excepciones de los tipos UNDEF, SWi y DABORT

volatile void Gestion_excepciones_init(void)
{
	pISR_DABORT = (int) Gestion_excepciones;
	pISR_SWI = (int) Gestion_excepciones;
	pISR_UNDEF = (int) Gestion_excepciones;
	hay_excepcion = NO_EXCEPT;
}
