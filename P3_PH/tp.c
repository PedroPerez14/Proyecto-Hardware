/*********************************************************************************************
* File£º	tp.c
* Author:	embest	
* Desc£º	LCD touch screen control function
* History:	
*********************************************************************************************/

/*--- include files ---*/
#include <string.h>
#include "tp.h"
#include "lcd.h"
#include "codigos_eventos.h"
#include "cola_depuracion.h"

void TSInt(void) __attribute__((interrupt("IRQ")));

/*--- function code ---*/
/*********************************************************************************************
* name:		TSInt
* func:		TouchScreen interrupt handler function
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void TSInt(void)
{
	rINTMSK |= (BIT_EINT2);	//Deshabilitamos interrupcion linea eint1 en vector de mascaras para el antirebotes de tsp
	push_debug(ev_tsp, no_info);
	rI_ISPC |= BIT_EINT2;	//Limpiar bit pendiente en INTPND
	//El código que había aquí ha sido eliminado porque no nos hace falta
}
			
/*********************************************************************************************
* name:		tsp_resetear()
* func:		Rehabilita las interrupciones en tsp
* para:		none
* ret:		none
* modify:
* comment:
*********************************************************************************************/
void tsp_resetear(void)
{
	/* Por precaucion, se vuelven a borrar los bits de INTPND */
	rI_ISPC   |= BIT_EINT2;			// borra el bit pendiente en INTPND
	rINTMSK    &= ~(BIT_EINT2); 	// habilitamos interrupcion linea eint4567 en vector de mascaras
}

/*********************************************************************************************
* name:		TS_init
* func:		initialize TouchScreen
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void TS_init(void)
{
    /* enable interrupt */
	rINTMOD &= ~(BIT_EINT2);		 // Configura las linas de TSP como de tipo IRQ
	rINTCON &= 0x1;					 // Habilita int. vectorizadas y la linea IRQ (FIQ la deja como estuviera)
    rI_ISPC |= BIT_EINT2;            // clear pending_bit
	
	// TSPX(GPE4_Q4(-)) TSPY(GPE5_Q3(-)) TSMY(GPE6_Q2(-)) TSMX(GPE7_Q1(+)) 
	//          1               1                0                 1
    rPUPE  = 0x0;	                 // Pull up
    rPDATE = 0xb8;                   // should be enabled	
    //Delay(100);
    
    rEXTINT |= 0x200;                // falling edge trigger
    pISR_EINT2=(unsigned *)TSInt;    // set interrupt handler
    
    rCLKCON = 0x7ff8;                // enable clock
    rADCPSR = 0x1;//0x4;             // A/D prescaler
    rINTMSK &= ~(BIT_EINT2);

    oneTouch = 0;
}

/*********************************************************************************************
* name:		TS_close
* func:		close TouchScreen
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void TS_close(void)
{
	/* Mask interrupt */
	rINTMSK |=BIT_GLOBAL|BIT_EINT2;
	pISR_EINT2 = (int)NULL;
}

/*********************************************************************************************
* name:		DesignREC
* func:		confirm the coordinate rang
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void DesignREC(ULONG tx, ULONG ty)
{
    int tm;
    
//  Uart_Printf("\n\r User touch coordinate(X,Y) is :");
//	Uart_Printf("(%04d",tx);	
//	Uart_Printf(",%04d)\n",ty);    
    if(oneTouch == 0)
     {
       Vx = tx;                   // Vx as Xmax
       Vy = ty;                   // Vy as Ymax
       oneTouch = 1;       		           
    }else{
    
    if(Vx < tx )
     {
       tm = tx; tx = Vx; Vx = tm; // tx as Xmin
     }
    if(Vy < ty )
     { 
       tm = ty; ty = Vy; Vy = tm; // ty as Ymin
     }
     
    Xmax = Vx;    Xmin = tx;
    Ymax = Vy;    Ymin = ty;
    
    oneTouch = 0;
	CheckTSP = 0;// has checked
	
	}// end if(oneTouch == 0)

}



void Check_Sel(void)
{
	char yn;
    
	do{
	  
	  rINTMSK |=BIT_GLOBAL|BIT_EINT2;
//    Uart_Printf("\n\r Touch Screen coordinate Rang in:\n");
//    Uart_Printf("   (Xmin,Ymin) is :(%04d,%04d)\n",Xmin,Ymin);	
//	  Uart_Printf("   (Xmax,Ymax) is :(%04d,%04d)\n",Xmax,Ymax);	
//	  Uart_Printf("\n  To use current settings. Press N/n key. ");
//	  Uart_Printf("\n\n\r  Want to Set Again(Y/N)? ");	        	      
//	  yn = Uart_Getch();
      rI_ISPC = BIT_EINT2;              // clear pending_bit
      rINTMSK =~(BIT_GLOBAL|BIT_EINT2);
//	  if((yn == 0x59)|(yn == 0x79)|(yn == 0x4E)|(yn == 0x6E)) Uart_SendByte(yn);	  
	  if((yn == 0x59)|(yn == 0x79))
	    {
//	      Uart_Printf("\n\n Touch TSP's Cornor to ensure Xmax,Ymax,Xmax,Xmin");	      

         //Init X Y rectangle
          Xmax = 750;    Xmin = 200;
          Ymax = 620;    Ymin = 120;

	      oneTouch = 0;
	      CheckTSP = 1; // mask to check
	      while(CheckTSP);
	      
	    }else break;	    
	  
	}while(1);
	
}

