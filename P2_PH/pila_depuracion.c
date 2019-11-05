/*********************************************************************************************
* Fichero:		pila_depuracion.c
* Autor:		Fernando Peña Bes (NIA: 756012)
* 				Pedro José Pérez García (NIA: 756642)
* Descrip:		Pila para depurar los eventos que llegan por los perif�ricos,
* 				gestionada de forma circular
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "pila_depuracion.h"
#include "timer2.h"
#include <stdint.h>
//Variables para la gesti�n circular de la pila
const int LIMITE_INF_PILA_DEBUG = 0x0c77fe00;

static volatile uint32_t * cima;
static volatile uint32_t * base;
static volatile int limite_sup;
static volatile int numElem;
static volatile int maxAsignado;

//Cada elemento de la pila es una tupla <Evento,Timestamp> que ocupa 8 Bytes
void pila_depuracion_inicializar(int maxElem)
{
	cima = (uint32_t*)LIMITE_INF_PILA_DEBUG;
	base = (uint32_t*)LIMITE_INF_PILA_DEBUG;
	limite_sup = LIMITE_INF_PILA_DEBUG - 8 * maxElem;
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
		//apilar en cima y cima -=8
		//Y numElem--
		if((int)cima <= limite_sup)
		{
			cima = (uint32_t *)LIMITE_INF_PILA_DEBUG;
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
		//apilar en base vieja
		//Y si base se ha desbordado, corregimos

		cima = base;
		base -= 2;
		*cima = dato;
		cima -= 1;
		*cima = timer2_leer();
		cima -= 1;
		if((int)base <= limite_sup)
		{
			base = (uint32_t *)LIMITE_INF_PILA_DEBUG;
		}
	}
}

uint32_t pop_debug()
{
	if(esVacia() == -1)
	{
		uint32_t * aux = base;
		base -= 2;
		if((int)base <= limite_sup)
		{
			base = (uint32_t *)LIMITE_INF_PILA_DEBUG;
		}
		numElem -= 1;
		return *aux;
	}
	return 0;
}


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
