/*********************************************************************************************
* Fichero:		funciones_itoa.h
* Autor:		Pedro José Pérez García (756642) y Fernando Peña Bes (756012)
* Descrip:		Conjunto de funciones auxiliares para poder pasar de entero a cadenas de caracteres.
* Version:
*********************************************************************************************/
#ifndef _FUNCIONES_ITOA_H_
#define _FUNCIONES_ITOA_H_

/* Funciones disponibles para el usuario */
inline void swap(char *x, char *y);
char* reverse(char *buffer, int i, int j);
char* itoa(int value, char* buffer, int base);
#endif /* _FUNCIONES_ITOA_H_ */
