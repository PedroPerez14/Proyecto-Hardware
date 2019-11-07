/*********************************************************************************************
* Fichero:		led.c
* Autor:
* Descrip:		funciones de control de los LED de la placa
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "led.h"
#include "44b.h"
#include "44blib.h"
#include "botones_antirebotes.h"


/*--- variables del módulo ---*/
#define TICKS_ESPERA 50;
static int nuevaPulsacion;
static int cuenta_ticks_timer0;

void button_ev_pulsacion(enum estado_button boton)
{
	//HAY QUE HACER COSAS CON BOTONES_ANTIREBOTES
	// desactivar interrupciones de boton
	// esperar retardo trp
	//

	nuevaPulsacion = 1;
	//COSAS
}

void button_ev_tick0(void)
{
	//MAS COSAS

}
