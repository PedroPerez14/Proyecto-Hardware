/*********************************************************************************************
* Fichero:	button.h
* Autor:	Pedro Jos� P�rez Garc�a (756642) y Fernando Pe�a Bes (756012)
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

#ifndef _BUTTON_H_
#define _BUTTON_H_

/*--- Declaraci�n del enum de estados posibles de los botones ---*/

enum estado_button {button_none, button_iz, button_dr};

/*--- declaracion de funciones visibles del m�dulo button.c/button.h ---*/
void button_iniciar(void);
void button_resetear(void);
enum estado_button button_estado(void);
void button_ev_pulsacion(enum estado_button boton);
void button_ev_tick0(void);
void bloquear_IRQ_button(void);

#endif /* _BUTTON_H_ */
