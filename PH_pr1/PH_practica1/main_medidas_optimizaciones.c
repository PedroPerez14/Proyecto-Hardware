/*********************************************************************************************
* Fichero:	main.c
*Autor:		Fernando Pe�a Bes (NIA: 756012)
*			Pedro Jos� P�rez Garc�a (NIA: 756642)
* Descrip:	punto de entrada de C
* Version:  <P4-ARM.timer-leds>
*********************************************************************************************/
/*
//--- ficheros de cabecera ---//
#include "8led.h"
#include "button.h"
#include "led.h"
#include "timer.h"
#include "44blib.h"
#include "44b.h"

#include "timer2.h"

//--- variables globales ---//

// Tama�o del tablero
enum { DIM=8 };

// Valores que puede devolver la funci�n patron_volteo())
enum {
	NO_HAY_PATRON = 0,
	PATRON_ENCONTRADO = 1
};

// Estados de las casillas del tablero
// deberia ser enum, lo dejamos como const para usar char...
//const char CASILLA_VACIA = 0;
//const char FICHA_BLANCA  = 1;
//const char FICHA_NEGRA   = 2;

enum {
	CASILLA_VACIA = 0,
	FICHA_BLANCA = 1,
	FICHA_NEGRA = 2
};

const char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
const char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};




char ficha_valida(char tablero[][DIM], char f, char c, int *posicion_valida)
{
    char ficha;

    // ficha = tablero[f][c];
    // no puede accederse a tablero[f][c]
    // ya que alg�n �ndice puede ser negativo

    if ((f < DIM) && (f >= 0) && (c < DIM) && (c >= 0) && (tablero[f][c] != CASILLA_VACIA))
    {
        *posicion_valida = 1;
        ficha = tablero[f][c];
    }
    else
    {
        *posicion_valida = 0;
        ficha = CASILLA_VACIA;
    }
    return ficha;
}

////////////////////////////////////////////////////////////////////////////////
// La funci�n patr�n volteo comprueba si hay que actualizar una determinada direccion,
// busca el patr�n de volteo (n fichas del rival seguidas de una ficha del jugador actual)
// en una direcci�n determinada
// SF y SC son las cantidades a sumar para movernos en la direcci�n que toque
// color indica el color de la pieza que se acaba de colocar
// la funci�n devuelve PATRON_ENCONTRADO (1) si encuentra patr�n y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un par�metro por referencia. Sirve para saber la longitud del patr�n que se est� analizando.
//          Se usa para saber cuantas fichas habr�a que voltear
int patron_volteo(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color)
{
	int posicion_valida; // indica si la posici�n es valida y contiene una ficha de alg�n jugador
	char casilla;   // casilla es la casilla que se lee del tablero

	FA = FA + SF;
	CA = CA + SC;
	casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
	while ((posicion_valida == 1) && (casilla != color))
	// mientras la casilla est� en el tablero, no est� vac�a,
	// y es del color rival seguimos buscando el patron de volteo
	{
		FA = FA + SF;
		CA = CA + SC;
		*longitud = *longitud + 1;
		casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
	}
    // si la ultima posici�n era v�lida y la ficha es del jugador actual,
    // entonces hemos encontrado el patr�n
	if ((posicion_valida == 1) && (casilla == color) && (*longitud >0))
		return PATRON_ENCONTRADO; // si hay que voltear una ficha o m�s hemos encontrado el patr�n
	else
		return NO_HAY_PATRON; // si no hay que voltear no hay patr�n
}

extern int patron_volteo_arm_c(char tablero[][DIM], int *longitud,char FA, char CA, char SF, char SC, char color);
extern int patron_volteo_arm_arm(char tablero[][DIM], int *longitud,char FA, char CA, char SF, char SC, char color);
extern int patron_volteo_arm_arm_opt(char tablero[][DIM], int *longitud,char FA, char CA, char SF, char SC, char color);

//--- codigo de funciones ---//
void Main(void)
{
	
	//Inicializa controladores
	sys_init();         // Inicializacion de la placa, interrupciones y puertos
	timer_init();	    // Inicializacion del temporizador
	Eint4567_init();	// inicializamos los pulsadores. Cada vez que se pulse se ver� reflejado en el 8led
	D8Led_init();       // inicializamos el 8led

	static char __attribute__ ((aligned (8))) tablero1[DIM][DIM] = {
			{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
		    {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
		    {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
		};

	volatile int patron_c_c, patron_arm_c, patron_arm_arm;

	volatile unsigned int timeA;
	volatile unsigned int timeD;
	volatile unsigned int timeT;

	int longitud = 0;

	timer2_inicializar();

	timer2_empezar();
	timeA = timer2_leer();
	patron_arm_c = patron_volteo(tablero1, &longitud, 2, 3, -1, 0, 2);
	timeD = timer2_parar();
	timeT = timeD - timeA;

	timer2_empezar();
	timeA = timer2_leer();
	patron_arm_c = patron_volteo(tablero1, &longitud, 2, 3, -1, 1, 2);
	timeD = timer2_parar();
	timeT = timeD - timeA;

	timer2_empezar();
	timeA = timer2_leer();
	patron_arm_c = patron_volteo(tablero1, &longitud, 2, 3, 0, 1, 2);
	timeD = timer2_parar();
	timeT = timeD - timeA;

	timer2_empezar();
	timeA = timer2_leer();
	patron_arm_c = patron_volteo(tablero1, &longitud, 2, 3, 1, 1, 2);
	timeD = timer2_parar();
	timeT = timeD - timeA;

	timer2_empezar();
	timeA = timer2_leer();
	patron_arm_c = patron_volteo(tablero1, &longitud, 2, 3, 1, 0, 2);
	timeD = timer2_parar();
	timeT = timeD - timeA;

	timer2_empezar();
	timeA = timer2_leer();
	patron_arm_c = patron_volteo(tablero1, &longitud, 2, 3, 1, -1, 2);
	timeD = timer2_parar();
	timeT = timeD - timeA;

	timer2_empezar();
	timeA = timer2_leer();
	patron_arm_c = patron_volteo(tablero1, &longitud, 2, 3, 0, -1, 2);
	timeD = timer2_parar();
	timeT = timeD - timeA;

	timer2_empezar();
	timeA = timer2_leer();
	patron_arm_c = patron_volteo(tablero1, &longitud, 2, 3, -1, -1, 2);
	timeD = timer2_parar();
	timeT = timeD - timeA;


}
*/
