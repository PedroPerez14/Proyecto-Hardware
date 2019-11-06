/*********************************************************************************************
* Fichero:	codigos_eventos.h
* Autor: 	Fernando Peña Bes (756012) y Pedro José Pérez García (756642)
* Descrip:	Fichero que contiene los códigos utilizados para identificar los
* 			diferentes eventos que se van a ir pusheando en la cola de depuración.
* Version:  
*********************************************************************************************/
#ifndef _CODIGOS_H_
#define _CODIGOS_H_

/*--- declaracion los códigos en un enum ---*/

//Los códigos de 8 bits representan eventos, y los de 24 representan info adicional
enum {
    tick_timer0			= 0xFF,
    no_info				= 0xAAAAAAAA
    //Hay que pensar el resto y codificarlos aquí
	};

#endif /* _CODIGOS_H_ */
