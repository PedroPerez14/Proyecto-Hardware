/*********************************************************************************************
* Fichero:		cola_depuracion.c
* Autor:		Fernando PeÃ±a Bes (NIA: 756012)
* 				Pedro JosÃ© PÃ©rez GarcÃ­a (NIA: 756642)
* Descrip:		Cola para depurar los eventos que llegan por los periféricos,
* 				gestionada de forma circular
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "cola_depuracion.h"
#include "timer2.h"
#include <stdint.h>
//Variables para la gestión circular de la cola
const int LIMITE_INF_COLA_DEBUG = 0x0c77fe00;

static volatile uint32_t * cima;
static volatile uint32_t * base;
static volatile int limite_sup;
static volatile int numElem;
static volatile int maxAsignado;

//Cada elemento de la cola es una tupla <Evento,Timestamp> que ocupa 8 Bytes
//Por lo que se pasa como parámetro el número de tuplas que se desean almacenar.
void cola_depuracion_inicializar(int maxElem)
{
	cima = (uint32_t*)LIMITE_INF_COLA_DEBUG;
	base = (uint32_t*)LIMITE_INF_COLA_DEBUG;
	limite_sup = LIMITE_INF_COLA_DEBUG - 8 * maxElem;
	numElem = 0;
	maxAsignado = maxElem;
	timer2_inicializar();
	timer2_empezar();
}

void push_debug(uint8_t ID_evento, uint32_t auxData)
{
	uint32_t dato = (uint32_t) ID_evento << 24;
	auxData &= 0x00FFFFFF;
	dato |= auxData;
	if(numElem < maxAsignado)	//Si no hay que gestionar de forma circular
	{
		//encolar en cima y cima -=8
		//Y numElem--
		if((int)cima <= limite_sup)
		{
			cima = (uint32_t *)LIMITE_INF_COLA_DEBUG;
		}
		*cima = dato;
		cima -= 1;
		*cima = timer2_leer();
		cima -= 1;
		numElem += 1;
	}
	else
	{
		//base-=8
		//encolar en base vieja
		//Y si base se ha desbordado, corregimos

		cima = base;
		base -= 2;
		*cima = dato;
		cima -= 1;
		*cima = timer2_leer();
		cima -= 1;
		if((int)base <= limite_sup)
		{
			base = (uint32_t *)LIMITE_INF_COLA_DEBUG;
		}
	}
}


//Subimos la base y restamos un elemento de la cola.
//Si desborda la base por el límite superior, la corregimos
uint32_t pop_debug(void)
{
	if(esVacia() == -1)
	{
		uint32_t * aux = base;
		base -= 2;
		if((int)base <= limite_sup)
		{
			base = (uint32_t *)LIMITE_INF_COLA_DEBUG;
		}
		numElem -= 1;
		return *aux;
	}
	return 0;
}

//1 si es vacía y -1 si no
int esVacia(void)
{
	if(numElem == 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}
