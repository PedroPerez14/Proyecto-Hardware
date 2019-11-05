/*********************************************************************************************
* Fichero:		pila_depuracion.h
* Autor:		Fernando Pe�a Bes (NIA: 756012)
* 				Pedro Jos� P�rez Garc�a (NIA: 756642)
* Descrip:		Pila para depurar los eventos que llegan por los perif�ricos,
* 				gestionada de forma circular
*********************************************************************************************/

#ifndef _PILA_DEPURACION_H_
#define _PILA_DEPURACION_H_

#include <stdint.h>

/*--- variables visibles del m�dulo timer2.c/timer2.h ---*/

/*--- declaracion de funciones visibles del m�dulo timer.c/timer.h ---*/

void pila_depuracion_inicializar(int maxElem);

void push_debug(uint8_t ID_evento, uint32_t auxData);

uint32_t pop_debug();

int esVacia(void);

#endif /* _PILA_DEPURACION_H_ */
