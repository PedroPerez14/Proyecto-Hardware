/*********************************************************************************************
* Fichero:		cola_depuracion.c
* Autor:		Fernando PeÃ±a Bes (NIA: 756012)
* 				Pedro JosÃ© PÃ©rez GarcÃ­a (NIA: 756642)
* Descrip:		Cola para depurar los eventos que llegan por los periféricos,
* 				gestionada de forma circular.
*				IMPORTANTE: Cima apunta al lugar donde corresponde el siguiente elemento
*							a encolar, mientras que base no, apunta al elemento directamente
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "cola_depuracion.h"

//Variables para la gestión circular de la cola
//La dirección del límite inferior está en option.h, si no ha cambiado es 0x0c77fe00
const int _COLA_DEP_STARTADDRESS = _ISR_STARTADDRESS-0xf00*2;		/* c7fe100 */

static volatile uint32_t * cima;
static volatile uint32_t * base;
static volatile int limite_sup;
static volatile int numElem;
static volatile int maxAsignado;

//Cada elemento de la cola es una tupla <Evento,Timestamp> que ocupa 8 Bytes
//Por lo que se pasa como parámetro el número de tuplas que se desean almacenar.
void cola_depuracion_inicializar(int maxElem)
{
	cima = (uint32_t*)_COLA_DEP_STARTADDRESS;
	base = (uint32_t*)_COLA_DEP_STARTADDRESS;
	limite_sup = _COLA_DEP_STARTADDRESS - 8 * maxElem;
	numElem = 0;
	maxAsignado = maxElem;
	timer2_inicializar();
	timer2_empezar();
}


/* Devuelve, por separado, el ID del evento y la información adicional de este, tal y como le fueron suministrados a push_debug */
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
			cima = (uint32_t *)_COLA_DEP_STARTADDRESS;
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
			base = (uint32_t *)_COLA_DEP_STARTADDRESS;
		}
	}
}


//Subimos la base y restamos un elemento de la cola.
//Si desborda la base por el límite superior, la corregimos
void pop_debug(uint8_t *ID_evento, uint32_t  *auxData)
{
	if(!esVacia())
	{
		uint32_t * aux = base;
		base -= 2;
		if((int)base <= limite_sup)
		{
			base = (uint32_t *)_COLA_DEP_STARTADDRESS;
		}
		numElem -= 1;
		uint32_t dato = *aux;
		*auxData = dato & 0x00FFFFFF;
		dato &= 0xFF000000;
		*ID_evento = dato >> 24;
	}
}

int esVacia(void)
{
	return (numElem == 0);
}
