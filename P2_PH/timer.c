/*********************************************************************************************
* Fichero:		timer.c
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:		Modificado para el latido de la práctica 2.
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer.h"
#include "44b.h"
#include "44blib.h"
#include "cola_depuracion.h"
#include "codigos_eventos.h"
/*--- variables globales ---*/

/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
void timer_ISR(void)
{
	push_debug(ev_tick_timer0, no_info);		//TODO recuerda quitarlo cuando tengas que probar más cosas

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
	rI_ISPC |= BIT_TIMER0; // BIT_TIMER0 está definido en 44b.h y pone un uno en el bit 13 que correponde al Timer0
}

void timer_init(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD &= ~(BIT_TIMER0); // Configura las linas de timer0 como de tipo IRQ
	rINTCON &= 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ la deja como estuviera)
	rINTMSK &= ~(BIT_TIMER0); // habilitamos en vector de mascaras de interrupcion el Timer0 (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 están definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER0 */
	pISR_TIMER0 = (unsigned) timer_ISR;

	/* Configura el Timer0 a resolución de 1e-4 segundos para el latido */
	rTCFG0 &= 0xffffff00;
	rTCFG0 |= 0xc7; 		// ajusta el preescalado del timer 0 a 199 (200 de forma interna) para el latido
	rTCFG1 &= 0xffffff0;
	rTCFG1 |= 0xffffff7;    // selecciona la entrada del mux que proporciona el reloj 0. La 1xx (0x...7) corresponde a un divisor de 1/32.
	rTCNTB0 = 166;			// valor inicial de cuenta. Con esto hay una interrupción cada 1/60 de segundo.
	rTCMPB0 = 0;			// valor de comparación
	/* establecer update=manual (bit 1) + inverter=on (¿? será inverter off un cero en el bit 2 pone el inverter en off)*/
	rTCON &= 0xfffffff0;
	rTCON |= 0x00000002;
	/* iniciar timer (bit 0) con auto-reload (bit 3)*/
	rTCON &= 0xfffffff0;
	rTCON |= 0x00000009;
}

