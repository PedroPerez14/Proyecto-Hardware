/*********************************************************************************************
* Fichero:	codigos_eventos.h
* Autor: 	Fernando Pe�a Bes (756012) y Pedro Jos� P�rez Garc�a (756642)
* Descrip:	Fichero que contiene los c�digos utilizados para identificar los
* 			diferentes eventos que se van a ir pusheando en la cola de depuraci�n.
* Version:  
*********************************************************************************************/
#ifndef _CODIGOS_H_
#define _CODIGOS_H_

/*--- declaracion los c�digos en un enum ---*/

//Los c�digos de 8 bits representan eventos, y los de 24 representan info adicional
enum {
    tick_timer0			= 0xFF,
    no_info				= 0xAAAAAAAA
    //Hay que pensar el resto y codificarlos aqu�
	};

#endif /* _CODIGOS_H_ */
