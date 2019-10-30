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
static int hay_excepcion;
static int causa_fallo;
/*--- Declaraciones de las diferentes rutinas para tratamiento de las excepciones ---*/
void Gesion_excepciones(void) __attribute__((interrupt("ABORT")));
void Gesion_excepciones(void) __attribute__((interrupt("SWI")));
void Gesion_excepciones(void) __attribute__((interrupt("UNDEF")));

uint32_t __get_CPSR() 					//Devuelve CPSR
	{
		__asm("MRS r0, CPSR");
		__asm("bx lr");
	}

void parpadear_error(int caracter)		//8-led se queda parpadeando con el código del error
{										//según lo definido en el enum del .h
	#if DEBUG_SIMULADOR==TRUE
		__SIM__8LED = caracter;			//TODO revisar
	#else
		while(1)
		{
			D8Led_Symbol(caracter);
			Delay(5000);
			D8Led_Symbol(blank);
			Delay(2500);
		}
	#endif
}

void Gestion_excepciones(void)
{
	uint32_t cpsr = __get_CPSR();
	if(cpsr & 0x0000001F == 0x1b) 		//Si Estamos en modo Undef
	{
		hay_excepcion = UNDEF;
		register uint32_t lr asm("lr");
		causa_fallo = lr;
		parpadear_error(UNDEF);
	}
	else if(cpsr & 0x0000001F == 0x13) 	//Si Estamos en modo SVC de SWI
	{
		hay_excepcion = SWI;
		register uint32_t lr asm("lr");
		causa_fallo = lr;
		parpadear_error(SWI);
	}
	else								//Estamos en modo ABORT
	{
		hay_excepcion = ABORT;
		register uint32_t lr asm("lr");
		causa_fallo = lr - 8;
		parpadear_error(ABORT);
	}
}

void Gestion_excepciones_init(void)		//Inicializa la gestión de las excepciones de los tipos UNDEF, SWi y DABORT
{
	pISR_DABORT = (int) Gestion_excepciones;
	pISR_SWI = (int) Gestion_excepciones;
	pISR_UNDEF = (int) Gestion_excepciones;
	hay_excepcion = NO_EXCEPT;
}