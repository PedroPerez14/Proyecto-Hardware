/*********************************************************************************************
* Fichero:	debug.h
* Autor: 	Fernando Peña Bes (756012) y Pedro José Pérez García (756642)
* Descrip:	Fichero para cambiar entre modo debug y modo placa, para la correcta utilización
*			de la compilación condicional, junto con las variables que ilustran los diferentes estados
*			de los periféricos y componentes de la placa.
* Version:  
*********************************************************************************************/
//Esto va a estar muy mal definido, hay que preguntar cómo se define bien
#define DEBUG_SIMULADOR TRUE
//Si true, es para compilar sobre el simulador, mientras que si es false, el código será compilado para la placa
#if DEBUG_SIMULADOR==TRUE
	#define __SIM__8LED //Definir variables que representen los periféricos en las que se puedan codificar sus estados
#endif