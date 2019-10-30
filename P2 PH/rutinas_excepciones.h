/*********************************************************************************************
* Fichero:	rutinas_excepciones.h
* Autor: 	Fernando Peña Bes (756012) y Pedro José Pérez García (756642)
* Descrip:	Rutinas de tratamiento de las excepciones DAbort, Undefined y SWI
* Version:  
*********************************************************************************************/

#ifndef _RUTINAS_EXCEPCIONES_H_
#define _RUTINAS_EXCEPCIONES_H_

//Valores posibles que puede tomar la variable compartida hay_excepcion
enum {NO_EXCEPT = 0, SWI = 1, DABORT = 2, UNDEF = 3};

/*--- declaracion de funciones visibles del módulo ---*/
void Gestion_excepciones_init(void);

#endif /* _RUTINAS_EXCEPCIONES_H_ */