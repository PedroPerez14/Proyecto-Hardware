/*********************************************************************************************
* Fichero:	main.c
*Autor:		Fernando Peña Bes (NIA: 756012)
*			Pedro José Pérez García (NIA: 756642)
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

// Tamaño del tablero
enum { DIM=8 };

// Valores que puede devolver la función patron_volteo())
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
    // ya que algún índice puede ser negativo

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
// La función patrón volteo comprueba si hay que actualizar una determinada direccion,
// busca el patrón de volteo (n fichas del rival seguidas de una ficha del jugador actual)
// en una dirección determinada
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// la función devuelve PATRON_ENCONTRADO (1) si encuentra patrón y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un parámetro por referencia. Sirve para saber la longitud del patrón que se está analizando.
//          Se usa para saber cuantas fichas habría que voltear
int patron_volteo(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color)
{
	int posicion_valida; // indica si la posición es valida y contiene una ficha de algún jugador
	char casilla;   // casilla es la casilla que se lee del tablero

	FA = FA + SF;
	CA = CA + SC;
	casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
	while ((posicion_valida == 1) && (casilla != color))
	// mientras la casilla está en el tablero, no está vací­a,
	// y es del color rival seguimos buscando el patron de volteo
	{
		FA = FA + SF;
		CA = CA + SC;
		*longitud = *longitud + 1;
		casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
	}
    // si la ultima posición era válida y la ficha es del jugador actual,
    // entonces hemos encontrado el patrón
	if ((posicion_valida == 1) && (casilla == color) && (*longitud >0))
		return PATRON_ENCONTRADO; // si hay que voltear una ficha o más hemos encontrado el patrón
	else
		return NO_HAY_PATRON; // si no hay que voltear no hay patrón
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
	Eint4567_init();	// inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led
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
