/*********************************************************************************************
* Fichero:		cola_depuracion.h
* Autor:		Fernando Peña Bes (NIA: 756012)
* 				Pedro José Pérez García (NIA: 756642)
* Descrip:		Cola para depurar los eventos que llegan por los periféricos,
* 				gestionada de forma circular
*********************************************************************************************/

#ifndef _COLA_DEPURACION_H_
#define _COLA_DEPURACION_H_
#include "option.h"
#include <stdint.h>
#include "timer2.h"

/*--- declaracion de funciones visibles del módulo ---*/

void cola_depuracion_inicializar(int maxElem);

void push_debug(uint8_t ID_evento, uint32_t auxData);

void pop_debug(uint8_t* ID_evento, uint32_t* auxData);

int esVacia(void);

#endif /* _COLA_DEPURACION_H_ */
