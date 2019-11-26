/*********************************************************************************************
* Fichero:		jugada_por_botones.h
* Autor:		Pedro José Pérez García (756642) y Fernando Peña Bes (756012)
* Descrip:		Máquina de estados que gestiona la lógica del juego reversi.
* 				Las rutinas llamadas desde reversi_main invocan la función del autómata,
*				que comprueba si debe realizar alguna acción o no.
* Version:		
*********************************************************************************************/

#ifndef _JUGADA_POR_BOTONES_H_
#define _JUGADA_POR_BOTONES_H_

void inicializar_jugada_botones(void);
void jugada_ev_incrementar(void);
void jugada_ev_der(void);

#endif /* _JUGADA_POR_BOTONES_H_ */
