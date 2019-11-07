/*********************************************************************************************
* Fichero:		botones_antirebotes.c
* Autor:		Pedro José Pérez García (756642) y Fernando Peña Bes (756012)
* Descrip:		Máquina de estados que elimina los rebotes de los pulsadores.
* 				Las rutinas llamadas desde reversi_main invocan la función del autómata,
*				que comprueba si debe realizar alguna acción o no.
*				También se incluyen funciones que ayudan a la gestión de los rebotes.
* Version:		
*********************************************************************************************/

#ifndef _BOTONES_ANTIREBOTES_H_
#define _BOTONES_ANTIREBOTES_H_

//BLABLABLA
void button_ev_pulsacion(enum estado_button boton);
void button_ev_tick0(void);
//Creo que no hace falta poner nada más aquí

#endif /* _BOTONES_ANTIREBOTES_H_ */
