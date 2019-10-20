/*********************************************************************************************
* Fichero:	patron_volteo_arm_c.asm
* Autor:	Fernando Pe�a Bes (NIA: 756012)
			Pedro Jos� P�rez Garc�a (NIA: 756642)
* Descrip:	Contiene la versi�n ARM-C de patron_volteo()
*********************************************************************************************/

				.section .text
				.global patron_volteo_arm_c

.equ DIM, 8
.equ DESPL, 3   @2^DESPL = DIM

.equ CASILLA_VACIA, 0
.equ NO_HAY_PATRON, 0
.equ PATRON_ENCONTRADO, 1

				
@ Versi�n ARM-C optimizada de patron_volteo()

@ La funci�n comprueba si hay que actualizar una determinada direcci�n, busca
@ el patr�n de volteo (n fichas del rival seguidas de una ficha del jugador actual) en
@ una direcci�n determinada

@ Par�metros:
@ Par. entrada 0 = tablero
@ Par. entrada/salida 1 = *longitud
@ Par. entrada 2 = FA	@ casilla a analizar
@ Par. entrada 3 = CA
@ Par. entrada 4 = SF	@ direcci�n de movimiento a analizar
@ Par. entrada 5 = SC
@ Par. entrada 6 = color

@ Devuelve si ha encontrado un patr�n de volteo y la longitud analizada en el par�metro *longitud
patron_volteo_arm_c:
				mov ip, sp
				push {r4-r10, fp, ip, lr, pc}
				sub fp, ip, #4
				sub sp, #4 @ Espacio para posicion_valida
				@ En r0 est� @tablero
				@ En r1, @longitud
				@ En r2, FA
				@ En r3, CA
				mov r4, r0 	@ r4 = @tablero
				mov r5, r1 	@ r5 = @longitud
				mov r6, r2	@ r6 = FA
				mov r7, r3	@ r7 = CA
				@ Cargar par�metros pasados por la pila
				ldrsb r8,[fp,#4]	@r8 = SF
				ldrsb r9,[fp,#8]	@r9 = SC
				ldr r10,[r5]		@Cargo longitud en r10
				
WHILE_C:		add r6,r6,r8		@ FA = FA + SF
				add r7, r7, r9		@ CA = CA + SC
				@ Llamada a la funci�n ficha_valida
				mov r0, r4 			@ r0 = @tablero
				mov r1, r6			@ r1 = FA
				mov r2, r7			@ r2 = CA
				sub r3, fp, #36		@ r3 = @ de posicion_valida
				bl ficha_valida		@ En r0 tenemos casilla y en fp - 36 posicion_valida

				@ ((posicion_valida == 1) && (casilla != color))
				ldr r1,[fp,#-36]	@ r1 = posicion_valida
				cmp r1, #1			@ Comprobar si la posicion es valida
				bne FIN_WHILE_C
				ldr r3,[fp, #12]    @ r3 = color
				cmp r0, r3			@ Comparar casilla con color
				beq FIN_WHILE_C
				add r10,r10,#1		@ Incrementar longitud
				b WHILE_C

FIN_WHILE_C:	@ ((posicion_valida == 1) && (casilla == color) && (longitud > 0))
				cmp r1,#1			@ posicion_valida == 1
				cmpeq r0, r3		@ casilla == color
				movne r0, #NO_HAY_PATRON @ Si posicion_valida != 0 o casilla != color, no hay patron. Devolver no hay patron
				bne FIN_C
				cmpeq r10, #0		@ longitud > 0
				movhi r0, #PATRON_ENCONTRADO @ devolver patr�n encontrado
				movls r0, #NO_HAY_PATRON @ devolver patr�n no encontrado
FIN_C:			str	r10,[r5]		@ Almacenar longitud calculada
				ldmdb fp,{r4-r10, fp, sp, pc}

