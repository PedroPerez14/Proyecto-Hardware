/*********************************************************************************************
* Fichero:		botones_antirebotes.h
* Autor:		Pedro José Pérez García (756642) y Fernando Peña Bes (756012)
* Descrip:		Máquina de estados que elimina los rebotes de los pulsadores.
* 				Las rutinas llamadas desde reversi_main invocan la función del autómata,
*				que comprueba si debe realizar alguna acción o no.
*				También se incluyen funciones que ayudan a la gestión de los rebotes.
* Version:		
*********************************************************************************************/

#ifndef _BOTONES_ANTIREBOTES_H_
#define _BOTONES_ANTIREBOTES_H_

#include "keyboard.h"

void teclado_antirebotes_inicializar(void);
void keyboard_ev_pulsacion(enum estado_button boton);
void keyboard_ev_tick0(void);

#endif /* _BOTONES_ANTIREBOTES_H_ */
