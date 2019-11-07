/*********************************************************************************************
* Fichero:	codigos_eventos.h
* Autor: 	Fernando Pe�a Bes (756012) y Pedro Jos� P�rez Garc�a (756642)
* Descrip:	Fichero que contiene los c�digos utilizados para identificar los
* 			diferentes eventos que se van a ir pusheando en la cola de depuraci�n.
* Version:  
*********************************************************************************************/
#ifndef _CODIGOS_EVENTOS_H_
#define _CODIGOS_EVENTOS_H_

/*--- declaracion los c�digos en un enum ---*/

//Los c�digos de 8 bits representan eventos y empiezan por ev_ , y los de 24 representan info adicional
enum {
    ev_tick_timer0			= 0xFF,
    ev_button_int			= 0xBB,
    no_info				= 0xAAAAAAAA,
    button_izq			= 0x0000000F,
    button_der			= 0x0000000E
    //Hay que pensar el resto y codificarlos aqu�
	};

#endif /* _CODIGOS_EVENTOS_H_ */
