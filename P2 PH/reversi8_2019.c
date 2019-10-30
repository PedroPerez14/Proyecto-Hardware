/*********************************************************************************************
* Fichero:	reversi8_2019.c
* Autor:	Profesores de la asignatura
			Fernando Peña Bes (NIA: 756012)
			Pedro José Pérez García (NIA: 756642)
* Descrip:	Implementación del juego reversi8.
* 			Incluye funciones para probar patron_volteo()
* Version:
*********************************************************************************************/
#include "reversi8_2019.h"

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


// candidatas: indica las posiciones a explorar
// Se usa para no explorar todo el tablero innecesariamente
// Sus posibles valores son NO, SI, CASILLA_OCUPADA
const char  NO              = 0;
const char  SI              = 1;
const char  CASILLA_OCUPADA = 2;

/////////////////////////////////////////////////////////////////////////////
// TABLAS AUXILIARES
// declaramos las siguientes tablas como globales para que sean más fáciles visualizarlas en el simulador
// __attribute__ ((aligned (8))): specifies a minimum alignment for the variable or structure field, measured in bytes, in this case 8 bytes

static const char __attribute__ ((aligned (8))) tabla_valor[DIM][DIM] =
{
    {8,2,7,3,3,7,2,8},
    {2,1,4,4,4,4,1,2},
    {7,4,6,5,5,6,4,7},
    {3,4,5,0,0,5,4,3},
    {3,4,5,0,0,5,4,3},
    {7,4,6,5,5,6,4,7},
    {2,1,4,4,4,4,1,2},
    {8,2,7,3,3,7,2,8}
};


// Tabla de direcciones. Contiene los desplazamientos de las 8 direcciones posibles
const char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
const char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};

//////////////////////////////////////////////////////////////////////////////////////
// Variables globales que no deberían serlo
// tablero, fila, columna y ready son varibles que se deberían definir como locales dentro de reversi8.
// Sin embargo, las hemos definido como globales para que sea más fácil visualizar el tablero y las variables en la memoria
//////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Tablero sin inicializar
////////////////////////////////////////////////////////////////////
static char __attribute__ ((aligned (8))) tablero[DIM][DIM] = {
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
	    };


  ////////////////////////////////////////////////////////////////////
     // VARIABLES PARA INTERACCIONAR CON LA ENTRADA SALIDA
     // Pregunta: ¿hay que hacer algo con ellas para que esto funcione bien?
     // (por ejemplo añadir alguna palabra clave para garantizar que la sincronización a través de esa variable funcione)
volatile char fila=0, columna=0, ready = 0;


extern int patron_volteo_arm_c(char tablero[][DIM], int *longitud,char FA, char CA, char SF, char SC, char color);
extern int patron_volteo_arm_arm(char tablero[][DIM], int *longitud,char FA, char CA, char SF, char SC, char color);
extern int patron_volteo_arm_arm_opt(char tablero[][DIM], int *longitud,char FA, char CA, char SF, char SC, char color);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 0 indica CASILLA_VACIA, 1 indica FICHA_BLANCA y 2 indica FICHA_NEGRA
// pone el tablero a cero y luego coloca las fichas centrales.
void init_table(char tablero[][DIM], char candidatas[][DIM])
{
    int i, j;

    for (i = 0; i < DIM; i++)
    {
        for (j = 0; j < DIM; j++)
            tablero[i][j] = CASILLA_VACIA;
    }
#if 0
    for (i = 3; i < 5; ++i) {
	for(j = 3; j < 5; ++j) {
	    tablero[i][j] = i == j ? FICHA_BLANCA : FICHA_NEGRA;
	}
    }

    for (i = 2; i < 6; ++i) {
	for (j = 2; j < 6; ++j) {
	    if((i>=3) && (i < 5) && (j>=3) && (j<5)) {
		candidatas[i][j] = CASILLA_OCUPADA;
	    } else {
		candidatas[i][j] = SI; //CASILLA_LIBRE;
	    }
	}
    }
#endif
    // arriba hay versión alternativa
    tablero[3][3] = FICHA_BLANCA;
    tablero[4][4] = FICHA_BLANCA;
    tablero[3][4] = FICHA_NEGRA;
    tablero[4][3] = FICHA_NEGRA;

    candidatas[3][3] = CASILLA_OCUPADA;
    candidatas[4][4] = CASILLA_OCUPADA;
    candidatas[3][4] = CASILLA_OCUPADA;
    candidatas[4][3] = CASILLA_OCUPADA;

    // casillas a explorar:
    candidatas[2][2] = SI;
    candidatas[2][3] = SI;
    candidatas[2][4] = SI;
    candidatas[2][5] = SI;
    candidatas[3][2] = SI;
    candidatas[3][5] = SI;
    candidatas[4][2] = SI;
    candidatas[4][5] = SI;
    candidatas[5][2] = SI;
    candidatas[5][3] = SI;
    candidatas[5][4] = SI;
    candidatas[5][5] = SI;
}

////////////////////////////////////////////////////////////////////////////////
// Espera a que ready valga 1.
// CUIDADO: si el compilador coloca esta variable en un registro, no funcionará.
// Hay que definirla como "volatile" para forzar a que antes de cada uso la cargue de memoria

void esperar_mov(volatile char *ready)
{
    while (*ready == 0) {};  // bucle de espera de respuestas hasta que el se modifique el valor de ready (hay que hacerlo manualmente)

    *ready = 0;  //una vez que pasemos el bucle volvemos a fijar ready a 0;
}

////////////////////////////////////////////////////////////////////////////////
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// IMPORTANTE: AL SUSTITUIR FICHA_VALIDA() Y PATRON_VOLTEO()
// POR RUTINAS EN ENSAMBLADOR HAY QUE RESPETAR LA MODULARIDAD.
// DEBEN SEGUIR SIENDO LLAMADAS A FUNCIONES Y DEBEN CUMPLIR CON EL ATPCS
// (VER TRANSPARENCIAS Y MATERIAL DE PRACTICAS):
//  - DEBEN PASAR LOS PARAMETROS POR LOS REGISTROS CORRESPONDIENTES
//  - GUARDAR EN PILA SOLO LOS REGISTROS QUE TOCAN
//  - CREAR UN MARCO DE PILA TAL Y COMO MUESTRAN LAS TRANSPARENCIAS
//    DE LA ASIGNATURA (CON EL PC, FP, LR,....)
//  - EN EL CASO DE LAS VARIABLES LOCALES, SOLO HAY QUE APILARLAS
//    SI NO SE PUEDEN COLOCAR EN UN REGISTRO.
//    SI SE COLOCAN EN UN REGISTRO NO HACE FALTA
//    NI GUARDARLAS EN PILA NI RESERVAR UN ESPACIO EN LA PILA PARA ELLAS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////////////////////////////////////////////////////////////////////////////////
// Devuelve el contenido de la posición indicadas por la fila y columna actual.
// Además informa si la posición es válida y contiene alguna ficha.
// Esto lo hace por referencia (en *posicion_valida)
// Si devuelve un 0 no es válida o está vacia.
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

// Función para probar que las diferentes implementaciones de patron_volteo() devuelven el mismo resultado
// Además, se mide el tiempo de ejecutar cada implementación
int patron_volteo_test_tiempos(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color)
{
	volatile int patron_c_c, patron_arm_c, patron_arm_arm;

	volatile int timeA;
	volatile int timeD;
	volatile int timeT;

	timer2_inicializar();

	// Ejecutar patron_volteo
	timer2_empezar();
	timeA = timer2_leer();
	patron_c_c = patron_volteo(tablero, longitud, FA, CA, SF, SC, color);
	timeD = timer2_parar();
	timeT = timeD - timeA;

	volatile int longitud_c_c = *longitud;
	*longitud = 0;

	// Ejecutar patron_volteo_arm_c
	timer2_empezar();
	timeA = timer2_leer();
	patron_arm_c = patron_volteo_arm_c(tablero, longitud, FA, CA, SF, SC, color);
	timeD = timer2_parar();
	timeT = timeD - timeA;

	volatile int longitud_arm_c = *longitud;
	*longitud = 0;

	// Ejecutar patron_volteo_arm_arm
	timer2_empezar();
	timeA = timer2_leer();
	patron_arm_arm = patron_volteo_arm_arm_opt(tablero, longitud, FA, CA, SF, SC, color);
	timeD = timer2_parar();
	timeT = timeD - timeA;

	volatile int longitud_arm_arm = *longitud;

	// Comprobar que los resultados de todas las funciones sean iguales
	if (patron_c_c != patron_arm_c || patron_c_c != patron_arm_arm) {
		while (1);
	}
	if (longitud_c_c != longitud_arm_c || longitud_c_c != longitud_arm_arm) {
		while(1);
	}
	return patron_c_c;
}

// Función para probar que las diferentes implementaciones de patron_volteo() devuelven el mismo resultado
int patron_volteo_test(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color)
{
	int patron_c_c, patron_arm_c, patron_arm_arm;

	// Ejecutar patron_volteo
	patron_c_c = patron_volteo(tablero, longitud, FA, CA, SF, SC, color);
	int longitud_c_c = *longitud;
	*longitud = 0;

	// Ejecutar patron_volteo_arm_c
	patron_arm_c = patron_volteo_arm_c(tablero, longitud, FA, CA, SF, SC, color);
	int longitud_arm_c = *longitud;
	*longitud = 0;

	// Ejecutar patron_volteo_arm_arm
	patron_arm_arm = patron_volteo_arm_arm(tablero, longitud, FA, CA, SF, SC, color);
	int longitud_arm_arm = *longitud;

	// Comprobar que los resultados de todas las funciones sean iguales
	if (patron_c_c != patron_arm_c || patron_c_c != patron_arm_arm) {
		while (1);
	}
	if (longitud_c_c != longitud_arm_c || longitud_c_c != longitud_arm_arm) {
		while(1);
	}
	return patron_c_c;
}

////////////////////////////////////////////////////////////////////////////////
// voltea n fichas en la dirección que toque
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// FA y CA son la fila y columna a analizar
void voltear(char tablero[][DIM], char FA, char CA, char SF, char SC, int n, char color)
{
    int i;

    for (i = 0; i < n; i++)
    {
        FA = FA + SF;
        CA = CA + SC;
        tablero[FA][CA] = color;
    }
}
////////////////////////////////////////////////////////////////////////////////
// comprueba si hay que actualizar alguna ficha
// no comprueba que el movimiento realizado sea válido
// f y c son la fila y columna a analizar
// char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
// char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};
int actualizar_tablero(char tablero[][DIM], char f, char c, char color)
{
    char SF, SC; // cantidades a sumar para movernos en la dirección que toque
    int i, flip, patron;

    for (i = 0; i < DIM; i++) // 0 es Norte, 1 NE, 2 E ...
    {
        SF = vSF[i];
        SC = vSC[i];
        // flip: numero de fichas a voltear
        flip = 0;
        //patron = patron_volteo(tablero, &flip, f, c, SF, SC, color); //WASD
        //patron = patron_volteo_arm_c(tablero, &flip, f, c, SF, SC, color);
        patron = patron_volteo_test(tablero, &flip, f, c, SF, SC, color);
        //printf("Flip: %d \n", flip);
        if (patron == PATRON_ENCONTRADO )
        {
            voltear(tablero, f, c, SF, SC, flip, color);
        }
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Recorre todo el tablero comprobando en cada posición si se puede mover
// En caso afirmativo, consulta la puntuación de la posición y si es la mejor
// que se ha encontrado la guarda
// Al acabar escribe el movimiento seleccionado en f y c

// Candidatas
// NO    0
// SI    1
// CASILLA_OCUPADA 2
int elegir_mov(char candidatas[][DIM], char tablero[][DIM], char *f, char *c)
{
    int i, j, k, found;
    int mf = -1; // almacena la fila del mejor movimiento encontrado
    int mc;      // almacena la columna del mejor movimiento encontrado
    char mejor = 0; // almacena el mejor valor encontrado
    int patron, longitud;
    char SF, SC; // cantidades a sumar para movernos en la dirección que toque

    // Recorremos todo el tablero comprobando dónde podemos mover
    // Comparamos la puntuación de los movimientos encontrados y nos quedamos con el mejor
    for (i=0; i<DIM; i++)
    {
        for (j=0; j<DIM; j++)
        {   // indica en qué casillas quizá se pueda mover
            if (candidatas[i][j] == SI)
            {
                if (tablero[i][j] == CASILLA_VACIA)
                {
                    found = 0;
                    k = 0;

                    // en este bucle comprobamos si es un movimiento válido
                    // (es decir si implica voltear en alguna dirección)
                    while ((found == 0) && (k < DIM))
                    {
                        SF = vSF[k];    // k representa la dirección que miramos
                        SC = vSC[k];    // 1 es norte, 2 NE, 3 E ...

                        // nos dice qué hay que voltear en cada dirección
                        longitud = 0;
                        //patron = patron_volteo(tablero, &longitud, i, j, SF, SC, FICHA_BLANCA); //WASD
                        //patron = patron_volteo_arm_c(tablero, &longitud, i, j, SF, SC, FICHA_BLANCA);
                        patron = patron_volteo_test(tablero, &longitud, i, j, SF, SC, FICHA_BLANCA);
                        //  //printf("%d ", patron);
                        if (patron == PATRON_ENCONTRADO)
                        {
                            found = 1;
                            if (tabla_valor[i][j] > mejor)
                            {
                                mf = i;
                                mc = j;
                                mejor = tabla_valor[i][j];
                            }
                        }
                        k++;
                        // si no hemos encontrado nada probamos con la siguiente dirección
                    }
                }
            }
        }
    }
    *f = (char) mf;
    *c = (char) mc;
    // si no se ha encontrado una posición válida devuelve -1
    return mf;
}
////////////////////////////////////////////////////////////////////////////////
// Cuenta el número de fichas de cada color.
// Los guarda en la dirección b (blancas) y n (negras)
void contar(char tablero[][DIM], int *b, int *n)
{
    int i,j;

    *b = 0;
    *n = 0;

    // recorremos todo el tablero contando las fichas de cada color
    for (i=0; i<DIM; i++)
    {
        for (j=0; j<DIM; j++)
        {
            if (tablero[i][j] == FICHA_BLANCA)
            {
                (*b)++;
            }
            else if (tablero[i][j] == FICHA_NEGRA)
            {
                (*n)++;
            }
        }
    }
}

void actualizar_candidatas(char candidatas[][DIM], char f, char c)
{
    // donde ya se ha colocado no se puede volver a colocar
    // En las posiciones alrededor sí
    candidatas[f][c] = CASILLA_OCUPADA;
    if (f > 0)
    {
        if (candidatas[f-1][c] != CASILLA_OCUPADA)
            candidatas[f-1][c] = SI;

        if ((c > 0) && (candidatas[f-1][c-1] != CASILLA_OCUPADA))
            candidatas[f-1][c-1] = SI;

        if ((c < 7) && (candidatas[f-1][c+1] != CASILLA_OCUPADA))
            candidatas[f-1][c+1] = SI;
    }
    if (f < 7)
    {
        if (candidatas[f+1][c] != CASILLA_OCUPADA)
            candidatas[f+1][c] = SI;

        if ((c > 0) && (candidatas[f+1][c-1] != CASILLA_OCUPADA))
            candidatas[f+1][c-1] = SI;

        if ((c < 7) && (candidatas[f+1][c+1] != CASILLA_OCUPADA))
            candidatas[f+1][c+1] = SI;
    }
    if ((c > 0) && (candidatas[f][c-1] != CASILLA_OCUPADA))
        candidatas[f][c-1] = SI;

    if ((c < 7) && (candidatas[f][c+1] != CASILLA_OCUPADA))
        candidatas[f][c+1] = SI;
}




////////////////////////////////////////////////////////////////////////////////
// Proceso principal del juego
// Utiliza el tablero,
// y las direcciones en las que indica el jugador la fila y la columna
// y la señal de ready que indica que se han actualizado fila y columna
// tablero, fila, columna y ready son variables globales aunque deberían ser locales de reversi8,
// la razón es que al meterlas en la pila no las pone juntas, y así jugar es más complicado.
// en esta versión el humano lleva negras y la máquina blancas
// no se comprueba que el humano mueva correctamente.
// Sólo que la máquina realice un movimiento correcto.
void reversi8()
{

	 ////////////////////////////////////////////////////////////////////
	 // Tablero candidatas: se usa para no explorar todas las posiciones del tablero
	// sólo se exploran las que están alrededor de las fichas colocadas
	 ////////////////////////////////////////////////////////////////////
	char __attribute__ ((aligned (8))) candidatas[DIM][DIM] =
    {
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO}
    };


    int done;     // la máquina ha conseguido mover o no
    int move = 0; // el humano ha conseguido mover o no
    int blancas, negras; // número de fichas de cada color
    volatile int fin = 0;  // fin vale 1 si el humano no ha podido mover
                  // (ha introducido un valor de movimiento con algún 8)
                  // y luego la máquina tampoco puede
    char f, c;    // fila y columna elegidas por la máquina para su movimiento

    timer2_inicializar();
    init_table(tablero, candidatas);

    while (fin == 0)
    {
        move = 0;
        esperar_mov(&ready);
        // si la fila o columna son 8 asumimos que el jugador no puede mover
        if (((fila) != DIM) && ((columna) != DIM))
        {
            tablero[fila][columna] = FICHA_NEGRA;
            actualizar_tablero(tablero, fila, columna, FICHA_NEGRA);
            actualizar_candidatas(candidatas, fila, columna);
            move = 1;
        }

        // escribe el movimiento en las variables globales fila columna
        done = elegir_mov(candidatas, tablero, &f, &c);
        if (done == -1)
        {
            if (move == 0)
                fin = 1;
        }
        else
        {
            tablero[f][c] = FICHA_BLANCA;
            actualizar_tablero(tablero, f, c, FICHA_BLANCA);
            actualizar_candidatas(candidatas, f, c);
        }
    }
    contar(tablero, &blancas, &negras);
}











//////////////////////////////////// PRUEBAS de patron_volteo() //////////////////////////////////////////////////////
/*
 * Autor:	Fernando Peña Bes (NIA: 756012)
			Pedro José Pérez García (NIA: 756012)
 * Descrip:	Pruebas para comprobar las diferentes implentaciones de patron_volteo()
 *
 */

// Pruebas para comprobar que todas las implementaciones devuelven el mismo resultado

// Ejecuta patrón volteo en las 8 direcciones desde una casilla, probando a colocar primero una ficha negra y luego una blanca
void comprobarFicha(char tablero[][DIM], char f, char c) {
	int k, patron;
	char SF, SC;
	for (k=0; k < DIM; k++) {
		int longitud = 0;
		SF = vSF[k];    // k representa la dirección que miramos
		SC = vSC[k];    // 1 es norte, 2 NE, 3 E ...
		patron = patron_volteo_test(tablero, &longitud, f, c, SF, SC, FICHA_NEGRA);
		longitud = 0;
		patron = patron_volteo_test(tablero, &longitud, f, c, SF, SC, FICHA_BLANCA);
	}
}

// Llama a comprobarFicha() desde todas las casillas de diferentes tableros
void patron_volteo_prueba_resultado_igual(void)
{
	static char __attribute__ ((aligned (8))) tableroVacio[DIM][DIM] = {
		        {FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
		        {FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
		        {FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
		        {FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
		        {FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
		        {FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
		        {FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
		        {FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA}
		    };

	static char __attribute__ ((aligned (8))) tableroLleno[DIM][DIM] = {
				{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
				{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
				{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
				{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
				{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
				{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
				{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
				{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
			};

	static char __attribute__ ((aligned (8))) tablero1[DIM][DIM] = {
				{FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA}
			};
	static char __attribute__ ((aligned (8))) tablero2[DIM][DIM] = {
				{FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA}
			};
	static char __attribute__ ((aligned (8))) tablero3[DIM][DIM] = {
				{FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
				{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA}
			};
	static char __attribute__ ((aligned (8))) tablero4[DIM][DIM] = {
			{FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA},
			{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
			{FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
			{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,FICHA_BLANCA},
			{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
			{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,FICHA_BLANCA},
			{FICHA_NEGRA,CASILLA_VACIA,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
			{FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA}
		};


	char i, j;

	for (i=0; i < DIM; i++) {
		for (j=0; j < DIM; j++) {
			comprobarFicha(tableroVacio, i, j);
		}
	}

	for (i=0; i < DIM; i++) {
			for (j=0; j < DIM; j++) {
				comprobarFicha(tableroLleno, i, j);
			}
		}

	for (i=0; i < DIM; i++) {
			for (j=0; j < DIM; j++) {
				comprobarFicha(tablero1, i, j);
			}
		}

	for (i=0; i < DIM; i++) {
			for (j=0; j < DIM; j++) {
				comprobarFicha(tablero2, i, j);
			}
		}

	for (i=0; i < DIM; i++) {
			for (j=0; j < DIM; j++) {
				comprobarFicha(tablero3, i, j);
			}
		}

	for (i=0; i < DIM; i++) {
			for (j=0; j < DIM; j++) {
				comprobarFicha(tablero4, i, j);
			}
		}
}



// Pruebas para evaluar que el resultado de evaluar patron_volteo es correcto

// Dadas un tablero y una casilla comprueba que los resultados de patron_volteo() al colocar una ficha de un color dado en la casilla
// introducida son iguales que los introducidos en los vectores <resultados_correctos> y <longitudes_correctas>
void evaluar_patron_volteo(char tablero[][DIM], char f, char c, char color, int resultados_correctos[], int longitudes_correctas[]) {
	int i;
	for (i=0; i < DIM; i++) {
		int resultado;
		int longitud = 0;
		resultado = patron_volteo_test(tablero, &longitud, f, c, vSF[i], vSC[i], color);
		if (resultado != resultados_correctos[i] || longitud != longitudes_correctas[i]) {
			while(1);
		}
	}
}

// Diferentes pruebas para evaluar que patron_volteo() devuelve el resultado esperado
void patron_volteo_prueba_resultado_correcto() {
	static char __attribute__ ((aligned (8))) tableroVacio[DIM][DIM] = {
			        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
			    };

	int longitud = 0;
	int resultado;

	// Probar desplazamiento 0
	resultado = patron_volteo_test(tableroVacio, &longitud, 0, 0, 0, 0, FICHA_NEGRA);
	if (!(resultado == NO_HAY_PATRON && longitud == 0)) {
		while (1);
	}

	// Probar que devuelve error cuando la casilla está fuera de los límites
	longitud = 0;
	resultado = patron_volteo_test(tableroVacio, &longitud, -1, 0, 1, 1, FICHA_NEGRA);
	if (!(resultado == NO_HAY_PATRON && longitud == 0)) {
		while(1);
	}
	resultado = patron_volteo_test(tableroVacio, &longitud, 0, -1, 1, 1, FICHA_NEGRA);
	if (!(resultado == NO_HAY_PATRON && longitud == 0)) {
		while(1);
	}
	resultado = patron_volteo_test(tableroVacio, &longitud, 8, 0, 1, 1, FICHA_NEGRA);
	if (!(resultado == NO_HAY_PATRON && longitud == 0)) {
		while(1);
	}
	resultado = patron_volteo_test(tableroVacio, &longitud, 0, 8, 1, 1, FICHA_NEGRA);
	if (!(resultado == NO_HAY_PATRON && longitud == 0)) {
		while(1);
	}
	resultado = patron_volteo_test(tableroVacio, &longitud, -1, -1, 1, 1, FICHA_NEGRA);
	if (!(resultado == NO_HAY_PATRON && longitud == 0)) {
		while(1);
	}
	resultado = patron_volteo_test(tableroVacio, &longitud, 8, 8, 1, 1, FICHA_NEGRA);
	if (!(resultado == NO_HAY_PATRON && longitud == 0)) {
		while(1);
	}



	// Evaluar casillas dentro de los límites

	// A partir de aquí, y para que el código sea más legible,
	// se ha usan los valores numéricos de PATRON_ENCONTRADO Y NO_HAY_PATRON.

	// PRUEBAS COLOCANDO FICHA NEGRA //
	// Encontrar patrones desde esquina (0,0)
	// Se ha usado el valor numérico de PATRON_ENCONTRADO Y NO_HAY_PATRON para que el código sea más legible.
	static char __attribute__ ((aligned (8))) tablero1[DIM][DIM] = {
				        {CASILLA_VACIA,FICHA_BLANCA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
				        {FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
				        {FICHA_BLANCA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
				        {FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
				        {FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
				        {FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
				        {FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
				        {FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
				    };
	int resultados_correctos1[] = {0, 0, 1, 1, 1, 0, 0, 0};
	int longitudes_correctas1[] = {0, 0, 1, 2, 6, 0, 0, 0};
	evaluar_patron_volteo(tablero1, 0, 0, FICHA_NEGRA, resultados_correctos1, longitudes_correctas1);

	// Encontrar patrones desde un lado (3,0)
	static char __attribute__ ((aligned (8))) tablero2[DIM][DIM] = {
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_NEGRA},
						{FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
					};
	int resultados_correctos2[] = {1, 1, 1, 1, 1, 0, 0, 0};
	int longitudes_correctas2[] = {1, 2, 6, 1, 2, 0, 0, 0};
	evaluar_patron_volteo(tablero2, 3, 0, FICHA_NEGRA, resultados_correctos2, longitudes_correctas2);

	// Encontrar patrones desde el centro (3,3). Casillas vacías alrededor de los patrones
	static char __attribute__ ((aligned (8))) tablero3[DIM][DIM] = {
						{FICHA_NEGRA  ,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA ,CASILLA_VACIA},
						{CASILLA_VACIA,FICHA_BLANCA ,CASILLA_VACIA,FICHA_BLANCA  ,CASILLA_VACIA,FICHA_BLANCA ,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA ,FICHA_NEGRA ,FICHA_BLANCA ,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA  ,FICHA_BLANCA ,FICHA_BLANCA ,CASILLA_VACIA,FICHA_BLANCA ,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA ,FICHA_BLANCA ,FICHA_BLANCA ,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,FICHA_NEGRA  ,CASILLA_VACIA,FICHA_BLANCA ,CASILLA_VACIA,FICHA_BLANCA ,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA  ,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA ,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA  }
					};
	int resultados_correctos3[] = {0, 1, 1, 1, 1, 1, 1, 1};
	int longitudes_correctas3[] = {0, 2, 1, 3, 2, 1, 2, 2};
	evaluar_patron_volteo(tablero3, 3, 3, FICHA_NEGRA, resultados_correctos3, longitudes_correctas3);

	// Encontrar patrones desde el centro. Casillas no vacías alrededor de los patrones
	static char __attribute__ ((aligned (8))) tablero4[DIM][DIM] = {
						{FICHA_NEGRA ,FICHA_BLANCA,FICHA_NEGRA ,FICHA_BLANCA ,FICHA_NEGRA ,FICHA_NEGRA ,FICHA_NEGRA ,FICHA_BLANCA},
						{FICHA_NEGRA ,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA ,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
						{FICHA_NEGRA ,FICHA_BLANCA,FICHA_BLANCA,FICHA_NEGRA  ,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
						{FICHA_NEGRA ,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,FICHA_BLANCA,FICHA_NEGRA ,FICHA_NEGRA ,FICHA_BLANCA},
						{FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA ,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
						{FICHA_BLANCA,FICHA_NEGRA ,FICHA_BLANCA,FICHA_BLANCA ,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
						{FICHA_NEGRA ,FICHA_BLANCA,FICHA_NEGRA ,FICHA_NEGRA  ,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
						{FICHA_BLANCA,FICHA_NEGRA ,FICHA_NEGRA ,FICHA_NEGRA  ,FICHA_BLANCA,FICHA_NEGRA ,FICHA_NEGRA ,FICHA_NEGRA  }
					};
	int resultados_correctos4[] = {0, 1, 1, 1, 1, 1, 1, 1};
	int longitudes_correctas4[] = {0, 2, 1, 3, 2, 1, 2, 2};
	evaluar_patron_volteo(tablero4, 3, 3, FICHA_NEGRA, resultados_correctos4, longitudes_correctas4);

	// Falsos patrones desde esquina (0,0)
	// Se ha usado el valor numérico de PATRON_ENCONTRADO Y NO_HAY_PATRON para que el código sea más legible.
	static char __attribute__ ((aligned (8))) tablero5[DIM][DIM] = {
						{CASILLA_VACIA,FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
					};
	int resultados_correctos5[] = {0, 0, 0, 0, 0, 0, 0, 0};
	int longitudes_correctas5[] = {0, 0, 0, 0, 7, 0, 0, 0};
	evaluar_patron_volteo(tablero5, 0, 0, FICHA_NEGRA, resultados_correctos5, longitudes_correctas5);

	// Falsos patrones desde un lado (3,0)
	static char __attribute__ ((aligned (8))) tablero6[DIM][DIM] = {
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
					};
	int resultados_correctos6[] = {0, 0, 0, 0, 0, 0, 0, 0};
	int longitudes_correctas6[] = {1, 0, 1, 0, 2, 0, 0, 0};
	evaluar_patron_volteo(tablero6, 3, 0, FICHA_NEGRA, resultados_correctos6, longitudes_correctas6);

	// Falsos patrones desde el centro (3,3). Casillas vacías alrededor de los patrones
	static char __attribute__ ((aligned (8))) tablero7[DIM][DIM] = {
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA ,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA ,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,FICHA_BLANCA ,FICHA_BLANCA ,FICHA_BLANCA ,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA  ,FICHA_NEGRA  ,FICHA_NEGRA  ,FICHA_NEGRA  },
						{CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA  ,FICHA_BLANCA ,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA  ,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,FICHA_NEGRA  ,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
					};
	int resultados_correctos7[] = {0, 0, 0, 0, 0, 0, 0, 0};
	int longitudes_correctas7[] = {3, 0, 0, 0, 1, 0, 0, 1};
	evaluar_patron_volteo(tablero7, 3, 3, FICHA_NEGRA, resultados_correctos7, longitudes_correctas7);






	// PRUEBAS COLOCANDO FICHA BLANCA //
	// Encontrar patrones desde esquina (0,0)
	// Se ha usado el valor numérico de PATRON_ENCONTRADO Y NO_HAY_PATRON para que el código sea más legible.
	static char __attribute__ ((aligned (8))) tablero8[DIM][DIM] = {
						{CASILLA_VACIA,FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
					};
	int resultados_correctos8[] = {0, 0, 1, 1, 1, 0, 0, 0};
	int longitudes_correctas8[] = {0, 0, 1, 2, 6, 0, 0, 0};
	evaluar_patron_volteo(tablero8, 0, 0, FICHA_BLANCA, resultados_correctos8, longitudes_correctas8);

	// Encontrar patrones desde un lado (3,0)
	static char __attribute__ ((aligned (8))) tablero9[DIM][DIM] = {
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_BLANCA},
						{FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
					};
	int resultados_correctos9[] = {1, 1, 1, 1, 1, 0, 0, 0};
	int longitudes_correctas9[] = {1, 2, 6, 1, 2, 0, 0, 0};
	evaluar_patron_volteo(tablero9, 3, 0, FICHA_BLANCA, resultados_correctos9, longitudes_correctas9);

	// Encontrar patrones desde el centro (3,3). Casillas vacías alrededor de los patrones
	static char __attribute__ ((aligned (8))) tablero10[DIM][DIM] = {
						{FICHA_BLANCA  ,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA ,CASILLA_VACIA},
						{CASILLA_VACIA,FICHA_NEGRA ,CASILLA_VACIA,FICHA_NEGRA  ,CASILLA_VACIA,FICHA_NEGRA ,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA ,FICHA_BLANCA ,FICHA_NEGRA ,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_BLANCA  ,FICHA_NEGRA ,FICHA_NEGRA ,CASILLA_VACIA,FICHA_NEGRA ,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA ,FICHA_NEGRA ,FICHA_NEGRA ,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,FICHA_BLANCA  ,CASILLA_VACIA,FICHA_NEGRA ,CASILLA_VACIA,FICHA_NEGRA ,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA  ,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA ,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA  }
					};
	int resultados_correctos10[] = {0, 1, 1, 1, 1, 1, 1, 1};
	int longitudes_correctas10[] = {0, 2, 1, 3, 2, 1, 2, 2};
	evaluar_patron_volteo(tablero10, 3, 3, FICHA_BLANCA, resultados_correctos10, longitudes_correctas10);

	// Encontrar patrones desde el centro. Casillas no vacías alrededor de los patrones
	static char __attribute__ ((aligned (8))) tablero11[DIM][DIM] = {
						{FICHA_BLANCA ,FICHA_NEGRA,FICHA_BLANCA ,FICHA_NEGRA ,FICHA_BLANCA ,FICHA_BLANCA ,FICHA_BLANCA ,FICHA_NEGRA},
						{FICHA_BLANCA ,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA ,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA},
						{FICHA_BLANCA ,FICHA_NEGRA,FICHA_NEGRA,FICHA_BLANCA  ,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA},
						{FICHA_BLANCA ,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,FICHA_NEGRA,FICHA_BLANCA ,FICHA_BLANCA ,FICHA_NEGRA},
						{FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA ,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA},
						{FICHA_NEGRA,FICHA_BLANCA ,FICHA_NEGRA,FICHA_NEGRA ,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA},
						{FICHA_BLANCA ,FICHA_NEGRA,FICHA_BLANCA ,FICHA_BLANCA  ,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA},
						{FICHA_NEGRA,FICHA_BLANCA ,FICHA_BLANCA ,FICHA_BLANCA  ,FICHA_NEGRA,FICHA_BLANCA ,FICHA_BLANCA ,FICHA_BLANCA  }
					};
	int resultados_correctos11[] = {0, 1, 1, 1, 1, 1, 1, 1};
	int longitudes_correctas11[] = {0, 2, 1, 3, 2, 1, 2, 2};
	evaluar_patron_volteo(tablero11, 3, 3, FICHA_BLANCA, resultados_correctos11, longitudes_correctas11);

	// Falsos patrones desde esquina (0,0)
	// Se ha usado el valor numérico de PATRON_ENCONTRADO Y NO_HAY_PATRON para que el código sea más legible.
	static char __attribute__ ((aligned (8))) tablero12[DIM][DIM] = {
						{CASILLA_VACIA,FICHA_BLANCA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
					};
	int resultados_correctos12[] = {0, 0, 0, 0, 0, 0, 0, 0};
	int longitudes_correctas12[] = {0, 0, 0, 0, 7, 0, 0, 0};
	evaluar_patron_volteo(tablero12, 0, 0, FICHA_BLANCA, resultados_correctos12, longitudes_correctas12);

	// Falsos patrones desde un lado (3,0)
	static char __attribute__ ((aligned (8))) tablero13[DIM][DIM] = {
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
					};
	int resultados_correctos13[] = {0, 0, 0, 0, 0, 0, 0, 0};
	int longitudes_correctas13[] = {1, 0, 1, 0, 2, 0, 0, 0};
	evaluar_patron_volteo(tablero13, 3, 0, FICHA_BLANCA, resultados_correctos13, longitudes_correctas13);

	// Falsos patrones desde el centro (3,3). Casillas vacías alrededor de los patrones
	static char __attribute__ ((aligned (8))) tablero14[DIM][DIM] = {
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA},
						{CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
						{CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
					};
	int resultados_correctos14[] = {0, 0, 0, 0, 0, 0, 0, 0};
	int longitudes_correctas14[] = {3, 0, 0, 0, 1, 0, 0, 1};
	evaluar_patron_volteo(tablero14, 3, 3, FICHA_BLANCA, resultados_correctos14, longitudes_correctas14);

}


