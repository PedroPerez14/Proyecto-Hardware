/*********************************************************************************************
* Fichero:	patron_volteo_arm_arm_opt.asm
* Autor:	Fernando Peña Bes (NIA: 756012)
			Pedro José Pérez García (NIA: 756642)
* Descrip:	Contiene la versión ARM-ARM optimizada de patron_volteo()
*********************************************************************************************/

				.section .text
				.global patron_volteo_arm_arm_opt

.equ DIM, 8
.equ DESPL, 3   @2^DESPL = DIM

.equ CASILLA_VACIA, 0
.equ NO_HAY_PATRON, 0
.equ PATRON_ENCONTRADO, 1


@ Versión ARM-ARM optimizada de patron_volteo()

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
patron_volteo_arm_arm_opt:
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
				
				mov r9, #NO_HAY_PATRON	@r9=resultado de la subrutina
				ldr r8, [r1] @ r8 = longitud inicial

				@ FA = FA + SF
WHILE_ARM_OPT:		add r2, r2, r4
				@ CA = CA + SC
				add r3, r3, r5
				
				add r10, r0, r2, LSL #DESPL @añadir fila
				add r10, r10, r3	@añadir columna
				
				@ ficha válida
				@ FA < DIM && FA >= 0 && CA < DIM && CA >= 0 && (tablero[FA][CA] != CASILLA_VACIA)
				cmp r2, #DIM
				bge FIN_ARM_OPT 	@ FA >= DIM
				cmp r2, #0	
				blt FIN_ARM_OPT		@ FA < 0
				cmp r3, #DIM
				bge FIN_ARM_OPT		@ CA >= DIM
				cmp r3, #0
				blt FIN_ARM_OPT		@ CA < 0

				ldrb r7, [r10]	@ r7=tablero[FA][CA]
				cmp r7, #CASILLA_VACIA
				beq FIN_ARM_OPT		@r7=CASILLA_VACIA
				
				
				@ En este punto la posición es válida
				cmp r7, r6
				beq FIN_VALIDA_ARM_OPT		@ tablero[FA][CA] == color
				@ sumar 1 a longitud
				add r8, r8, #1
				b WHILE_ARM_OPT
				
FIN_VALIDA_ARM_OPT: cmp r8, #0
					movne r9, #PATRON_ENCONTRADO
				
FIN_ARM_OPT:	str r8, [r1]	@ Almacenar longitud calculada
				mov r0, r9		@ Mover resultado de la subrutina a r0
				ldmdb fp,{r4-r10, fp, sp, pc}

