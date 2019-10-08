/*********************************************************************************************
* Fichero:		timer2.c
* Autor:		Fernando Pe�a Bes(756012) y Pedro Jos� P�rez Garc�a(756642)
* Descrip:		funciones de control del timer2 del s3c44b0x
* Version:		
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer2.h"
#include "44b.h"
#include "44blib.h"

/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer2_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
void timer2_ISR(void)
{
	timer2_num_int += 1;
	/* borrar bit en I_ISPC para desactivar la solicitud de interrupci�n*/
	rI_ISPC |= BIT_TIMER2; // BIT_TIMER0 est� definido en 44b.h y pone un uno en el bit 13 que correponde al Timer0
}

void timer2_inicializar(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD |= 0x800; // Configura la l�nea del timer2 como IRQ
	rINTCON &= 0x1; // Habilita int. vectorizadas y la linea IRQ, dejando FIQ como estuviera
	rINTMSK &= ~(BIT_TIMER2); // habilitamos en vector de mascaras de interrupcion el Timer0 (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 est�n definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER0 */
	pISR_TIMER2 = (unsigned) timer2_ISR;

	/* Configura el Timer2 */
	rTCFG0 &= 0xFFFF00FF; // ajusta el preescalado a 0
	rTCFG1 &= 0xFFFFF0FF; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
}

void timer2_empezar(void)
{
	timer2_num_int = 0;	//Reinicia la variable que usamos para contar interrupciones.

	rTCNTB2 = MAX_COUNT;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB2 = COMP_VALUE;// valor de comparaci�n
	
	/* establecer update=manual (bit 13) */
	rTCON &= 0xFFFF0FFF;
	rTCON |= 0x00002000;
	/*Empezar la cuenta*/
	rTCON &= 0xFFFF0FFF;
	rTCON |= 0x00009000;
}

unsigned int timer2_leer(void)
{
	return timer2_num_int * PERIOD_INT + (rTCNTB2 - rTCNTO2) / CYCLES_EACH_MICROSEC;
	//Si queremos optimizar, como la multiplicaci�n es por 2048, se pueden mover los bits 16 lugares a al izquierda y en la divisi�n, al ser por 32, se pueden mover 5 a la derecha.
}

unsigned int timer2_parar(void)
{
	rTCON &= 0xFFFF0FFF;	//Detener la cuenta
	return timer2_leer();
}
