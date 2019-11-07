/*********************************************************************************************
* Fichero:	codigos_eventos.h
* Autor: 	Fernando Peña Bes (756012) y Pedro José Pérez García (756642)
* Descrip:	Fichero que contiene los códigos utilizados para identificar los
* 			diferentes eventos que se van a ir pusheando en la cola de depuración.
* Version:  
*********************************************************************************************/
#ifndef _CODIGOS_EVENTOS_H_
#define _CODIGOS_EVENTOS_H_

/*--- declaracion los códigos en un enum ---*/

//Los códigos de 8 bits representan eventos y empiezan por ev_ , y los de 24 representan info adicional
enum {
    ev_tick_timer0			= 0xFF,
    ev_button_int			= 0xBB,
    no_info				= 0xAAAAAAAA,
    button_izq			= 0x0000000F,
    button_der			= 0x0000000E
    //Hay que pensar el resto y codificarlos aquí
	};

#endif /* _CODIGOS_EVENTOS_H_ */
