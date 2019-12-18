/*********************************************************************************************
* Fichero:		tsp_antirebotes.h
* Autor:		Pedro José Pérez García (756642) y Fernando Peña Bes (756012)
* Descrip:		Máquina de estados que elimina los rebotes de la pantalla táctil.
* Version:		
*********************************************************************************************/

#ifndef _TSP_ANTIREBOTES_H_
#define _TSP_ANTIREBOTES_H_

void tsp_antirebotes_inicializar(void);
void tsp_ev_pulsacion(void);
void tsp_ev_tick0(void);

#endif /* _TSP_ANTIREBOTES_H_ */
