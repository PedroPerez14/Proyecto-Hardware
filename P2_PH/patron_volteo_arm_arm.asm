/*********************************************************************************************
* Fichero:	patron_volteo_arm_arm.asm
* Autor:	Fernando Peña Bes (NIA: 756012)
			Pedro José Pérez García (NIA: 756642)
* Descrip:	Contiene la versión ARM-ARM de patron_volteo()
*********************************************************************************************/

				.section .text
				.global patron_volteo_arm_arm

.equ DIM, 8
.equ DESPL, 3   @2^DESPL = DIM

.equ CASILLA_VACIA, 0
.equ NO_HAY_PATRON, 0
.equ PATRON_ENCONTRADO, 1


@ Versión ARM-ARM de patron_volteo()

@ La función comprueba si hay que actualizar una determinada dirección, busca
@ el patrón de volteo (n fichas del rival seguidas de una ficha del jugador actual) en
@ una dirección determinada

@ Parámetros:
@ Par. entrada 0 = tablero
@ Par. entrada/salida 1 = *longitud
@ Par. entrada 2 = FA	@ casilla a analizar
@ Par. entrada 3 = CA
@ Par. entrada 4 = SF	@ dirección de movimiento a analizar
@ Par. entrada 5 = SC
@ Par. entrada 6 = color

@ Devuelve si ha encontrado un patrón de volteo y la longitud analizada en el parámetro *longitud
patron_volteo_arm_arm:
				mov ip, sp
				push {r4-r10, fp, ip, lr, pc}
				sub fp, ip, #4
				@ r0=@tablero
				@ r1=@longitud
				@ r2=FA
				@ r3=CA
				ldrsb r4,[fp,#4]	@r4 = SF
				ldrsb r5,[fp,#8]	@r5 = SC
				ldrb r6,[fp,#12]	@r6 = color

				ldr r8, [r1] @ r8 = longitud inicial

				@ FA = FA + SF
WHILE_ARM:		add r2, r2, r4
				@ CA = CA + SC
				add r3, r3, r5

				@ Llamada a ficha válida
				@ r7=tablero[FA][CA] (casilla), r9=posicion_valida
				@ FA < DIM && FA >= 0 && CA < DIM && CA >= 0 && (tablero[FA][CA] != CASILLA_VACIA)
				cmp r2, #DIM
				bge NO_VALIDA 		@ FA >= DIM
				cmp r2, #0
				blt NO_VALIDA		@ FA < 0
				cmp r3, #DIM
				bge NO_VALIDA		@ CA >= DIM
				cmp r3, #0
				blt NO_VALIDA		@ CA < 0
				@ r10=@ de la posicion en el tablero a analizar
				add r10, r0, r2, LSL #DESPL @añadir fila
				add r10, r10, r3	@añadir columna
				ldrb r7, [r10]		@ r7=tablero[FA][CA]
				cmp r7, #CASILLA_VACIA
				mov r9, #1
				bne WHILE_COND_ARM		@r7 != CASILLA_VACIA

NO_VALIDA:		mov r9, #0			@posicion_valida=0
				mov r7, #CASILLA_VACIA

WHILE_COND_ARM: @(posicion_valida == 1) && (casilla != color)
				cmp r9, #1
				bne IF_ARM
				cmp r7, r6
				beq IF_ARM		@ tablero[FA][CA] == color
				@ sumar 1 a longitud
				add r8, r8, #1
				b WHILE_ARM


IF_ARM: 		cmp r9, #1   @posicion_valida==1
				cmpeq r7, r6 @casilla==color
				movne r0, #NO_HAY_PATRON @ Si posicion_valida != 0 o casilla != color, no hay patron. Devolver no hay patron
				bne FIN_ARM
				cmpeq r8, #0
				movhi r0, #PATRON_ENCONTRADO @ Devolver patron encontrado
				movls r0, #NO_HAY_PATRON @ Devolver no hay patron
FIN_ARM:		str r8, [r1]	@ Almacenar longitud calculada
				ldmdb fp,{r4-r10, fp, sp, pc}
				
				
				
				
				

