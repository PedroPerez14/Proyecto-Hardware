/*********************************************************************************************
* File£º	keyboard.c
* Author:	embest
* Desc£º	keyboard source code
* History:	
*********************************************************************************************/

/*--- include files ---*/
#include "44b.h"
#include "44blib.h"
#include "def.h"
#include "keyboard.h"
#include "codigos_eventos.h"
#include "cola_depuracion.h"

/*--- global variables ---*/

/* keyboard control address */
volatile UCHAR *keyboard_base = (UCHAR *)0x06000000;

/*--- function declare ---*/

void init_keyboard();
void close_keyboard();
void KeyboardInt(void) __attribute__ ((interrupt ("IRQ")));

volatile static int teclaPulsada;




/*********************************************************************************************
* name:		init_keyboard
* func:		init keyboard interrupt
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void init_keyboard()
{
	/* enable interrupt */
	rINTMOD &= ~(BIT_EINT1); 	// Configura las linas de TSP como de tipo IRQ
	rINTCON &= 0x1;				// Habilita int. vectorizadas y la linea IRQ (FIQ la deja como estuviera)
	rI_ISPC |= BIT_EINT1;		// clear pending bit

	/* set EINT1 interrupt handler */
	rINTMSK &= ~(BIT_EINT1);
    pISR_EINT1 = (int)KeyboardInt;
    
    /* PORT G */
	rPCONG  |= 0x000c;      		// Establece la funcion del pin 1
	rPUPG   &= 0xfd;        		// Habilita el "pull up" del pin 1
	rEXTINT &= 0xffffffc7;			//
	rEXTINT |= 0x00000020;			// Configura la línea de interrupción del teclado matricial para que se active en el flanco de bajada


	/* Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */
	rEXTINTPND = 0xf;					// clear EXTINTPND reg
}

/*********************************************************************************************
* name:		close_keyboard
* func:		close keyboard interrupt
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void close_keyboard()
{
	pISR_EINT1 = NULL;
	/* Mask interrupt */
	rINTMSK |= BIT_GLOBAL | BIT_EINT1;
}

/*********************************************************************************************
* name:		KeyboardInt
* func:		keyboard interrupt handler function
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void KeyboardInt(void)
{
	rINTMOD |= (BIT_EINT1); 		// Desactiva las interrupciones de EINT1 para el antirebotes
	int value;
	rI_ISPC |= BIT_EINT1;			// clear pending bit
	
	teclaPulsada = key_read();

	if (teclaPulsada == 0) {
		push_debug(ev_keyboard, tecla_0);
	}
	else if (teclaPulsada == 0) {
		push_debug(ev_keyboard, tecla_1);
	}

}

/*********************************************************************************************
* name:		tec_resetear()
* func:		Rehabilita las interrupciones en teclado
* para:		none
* ret:		none
* modify:
* comment:
*********************************************************************************************/
void tec_resetear(void)
{
	/* Por precaucion, se vuelven a borrar los bits de INTPND */
	rI_ISPC   |= (BIT_EINT1);			// borra el bit pendiente en INTPND
	rINTMSK    &= ~(BIT_EINT1); 		// habilitamos interrupcion linea eint1 en vector de mascaras
}

/*********************************************************************************************
* name:		key_read
* func:		read key value
* para:		none
* ret:		key value, -1 -- error
* modify:
* comment:		
********************************************************************************************/
inline int key_read()
{
	int value;
	char temp;

	/* read line 1 */
	temp = *(keyboard_base+0xfd);
	/* not 0xF mean key down */
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 3;
		else if( (temp&0x2) == 0 )
			value = 2;
		else if( (temp&0x4) == 0 )
			value = 1;
		else if( (temp&0x8) == 0 )
			value = 0;
		return value;
	}
	
	/* read line 2 */
	temp = *(keyboard_base+0xfb);
	/* not 0xF mean key down */
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 7;
		else if( (temp&0x2) == 0 )
			value = 6;
		else if( (temp&0x4) == 0 )
			value = 5;
		else if( (temp&0x8) == 0 )
			value = 4;
		return value;
	}
	
	/* read line 3 */
	temp = *(keyboard_base+0xf7);
	/* not 0xF mean key down */
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 0xb;
		else if( (temp&0x2) == 0 )
			value = 0xa;
		else if( (temp&0x4) == 0 )
			value = 9;
		else if( (temp&0x8) == 0 )
			value = 8;
		return value;
	}
	
	/* read line 4 */
	temp = *(keyboard_base+0xef);
	/* not 0xF mean key down */
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 0xf;
		else if( (temp&0x2) == 0 )
			value = 0xe;
		else if( (temp&0x4) == 0 )
			value = 0xd;
		else if( (temp&0x8) == 0 )
			value = 0xc;
		return value;
	}
	
	return -1;
}
