/*********************************************************************************************
* Fichero:	button.c
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "button.h"
#include "8led.h"
#include "44blib.h"
#include "44b.h"
#include "def.h"

/*--- variables globales del módulo ---*/
/* int_count la utilizamos para sacar un número por el 8led.
  Cuando se pulsa un botón sumamos y con el otro restamos. ¡A veces hay rebotes! */
static unsigned int int_count = 0;

/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void Eint4567_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de funciones ---*/
void Eint4567_ISR(void)
{
	/* Identificar la interrupcion (hay dos pulsadores)*/
	int which_int = rEXTINTPND;
	switch (which_int)
	{
		case 4:
			int_count++; // incrementamos el contador
			break;
		case 8:
			int_count--; // decrementamos el contador
			break;
		default:
			break;
	}
	// }
	D8Led_symbol(int_count & 0x000f); // sacamos el valor por pantalla (módulo 16)

	/* Finalizar ISR */
	rEXTINTPND |= 0xa;				// borra los bits 6 y 7 en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND
}

void Eint4567_init(void)
{
	/* Configuracion del controlador de interrupciones pensando SOLO en usar los bits
	 * 6 y 7 para los pulsadores. Estos registros están definidos en 44b.h */
	rI_ISPC    |= BIT_EINT4567;		// Borra INTPND escribiendo 1s en I_ISPC
	rEXTINTPND = 0xf;       		// Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMOD    &= ~(BIT_EINT4567);	// Configura la linea EINT4567 como de tipo IRQ
	rINTCON    &= 0x1;	    		// Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK    &= ~(BIT_EINT4567); 	// habilitamos interrupcion linea eint4567 en vector de mascaras

	/* Establece la rutina de servicio para Eint4567 */
	pISR_EINT4567 = (int) Eint4567_ISR;

	/* Configuracion del puerto G */
	rPCONG  |= 0xf000;      // Establece la funcion de los pines (EINT6-7)
	rPUPG   &= 0x3f;        // Habilita el "pull up" de los pines 6 y 7, de los pulsadores
	rEXTINT &= 0x00ffffff;	//
	rEXTINT |= 0x22000000;	// Configura las lineas de int. de los pulsadores como de flanco de bajada

	/* Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND
}
