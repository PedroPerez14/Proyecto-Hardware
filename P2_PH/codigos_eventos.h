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
    /*ev_autoincr			= 0xCC,		//Ya no se usa al haber cambiado la forma en que se comunican antirebotes y jugada_por_botones
    ev_fila					= 0x55,
    ev_columna				= 0x44,
    ev_ready				= 0x33,*/
    no_info				= 0xAAAAAAAA,
    button_izq			= 0x0000000F,
    button_der			= 0x0000000E
    //Los valores 0x00000001 a 0x00000008 no deben asignarse, puesto que podrían dar conflictos si se apilan con ev_fila o ev_columna de forma descontrolada
	};

#endif /* _CODIGOS_EVENTOS_H_ */
