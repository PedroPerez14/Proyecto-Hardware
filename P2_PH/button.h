/*********************************************************************************************
* Fichero:	button.h
* Autor:
* Descrip:	Funciones de manejo de los pulsadores y gestión para evitar rebotes (EINT6-7)
* Version:
*********************************************************************************************/

#ifndef _BUTTON_H_
#define _BUTTON_H_

/*--- Declaración del enum de estados posibles de los botones ---*/

enum estado_button {button_none, button_iz, button_dr};

/*--- declaracion de funciones visibles del módulo button.c/button.h ---*/
void button_iniciar(void);
void button_resetear(void);
enum estado_button button_estado(void);
void button_ev_pulsacion(enum estado_button boton);
void button_ev_tick0(void);

#endif /* _BUTTON_H_ */
