/*********************************************************************************************
* Fichero:	button.h
* Autor:	Pedro José Pérez García (756642) y Fernando Peña Bes (756012)
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
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
void bloquear_IRQ_button(void);

#endif /* _BUTTON_H_ */
