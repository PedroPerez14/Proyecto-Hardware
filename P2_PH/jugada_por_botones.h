/*********************************************************************************************
* Fichero:		jugada_por_botones.h
* Autor:		Pedro Jos� P�rez Garc�a (756642) y Fernando Pe�a Bes (756012)
* Descrip:		M�quina de estados que gestiona la l�gica del juego reversi.
* 				Las rutinas llamadas desde reversi_main invocan la funci�n del aut�mata,
*				que comprueba si debe realizar alguna acci�n o no.
* Version:		
*********************************************************************************************/

#ifndef _JUGADA_POR_BOTONES_H_
#define _JUGADA_POR_BOTONES_H_

void inicializar_jugada_botones(void);
void jugada_ev_incrementar(void);
void jugada_ev_der(void);

#endif /* _JUGADA_POR_BOTONES_H_ */
