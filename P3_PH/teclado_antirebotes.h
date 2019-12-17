/*********************************************************************************************
* Fichero:		teclado_antirebotes.h
* Autor:		Pedro José Pérez García (756642) y Fernando Peña Bes (756012)
* Descrip:		Máquina de estados que elimina los rebotes del teclado matricial.
* Version:		
*********************************************************************************************/

#ifndef _TECLADO_ANTIREBOTES_H_
#define _TECLADO_ANTIREBOTES_H_

void tec_antirebotes_inicializar(void);
void tec_ev_pulsacion(void);
void tec_ev_tick0(void);

#endif /* _TECLADO_ANTIREBOTES_H_ */
