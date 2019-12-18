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
    ev_tsp					= 0x11,
    ev_keyboard				= 0x22,
    /*ev_autoincr			= 0xCC,		//Ya no se usa al haber cambiado la forma en que se comunican antirebotes y jugada_por_botones
    ev_fila					= 0x55,		//Se deja por si en el futuro se puede recuperar
    ev_columna				= 0x44,		//El motivo del cambio es que la cola es concurrente, y se ha decidido no apilar los eventos
    ev_ready				= 0x33,*/	//	que no sean necesarios, para aliviar su carga
    ev_finLCD				= 0x69,
    tecla_0					= 0x00000070,
    tecla_1					= 0x00000071,
    no_info					= 0xAAAAAAAA,
    button_izq				= 0x0000000F,
    button_der				= 0x0000000E
	};

#endif /* _CODIGOS_EVENTOS_H_ */
