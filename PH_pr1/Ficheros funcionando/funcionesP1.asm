				.section .text
				.global patron_volteo_arm_c
				.global patron_volteo_arm_arm
@ La funcion patron_volteo comprueba si hay que actualizar una determinada dirección

patron_volteo_arm_c:

				PUSH {lr}
				PUSH {r11}
				mov fp, sp
				
				@ En r2 esta FA y en r3 CA
				PUSH {r4-r8}					@ falta apilar registros utilizados r1-r3
				mov r4, r0 @ r4 = @tablero
				mov r5, r1 @ r5 = @flip
				mov r1, r2 @ r1 = FA
				mov r2, r3 @ r2 = CA
				 
				@ Cargar parámetros pasados por la pila
				ldrb r6, [fp, #8] @ r6 = SF
				ldrb r7, [fp, #12] @ r7 = SC
				ldrb r8, [fp, #16] @ r8 = color (se puede optimizar con un load mÃºltiple)
				
				@ reservar espacio para variables locales
				sub sp, sp, #8
				
				add r1, r1, r6 @ FA = FA + SF     Se puede optimizar saltando aquÃ­ en vez de repetir codigo
				add r2, r2, r7 @ CA = CA + SC
				
				@ casilla = ficha válida(tablero, FA, CA, &posicion_valida)
				
				@ llamar a la función ficha valida
				@ r0 está bien, mover 1 registro r2 y r3, poner la direccion en la pila de posicion valida
				sub r3, fp, #24
while:
				bl ficha_valida
				@ En r0 está casilla y en fp - 24, posicion valida
				cmp r0, #1
				bne fin_while
				ldr r0, [fp, #-24] @ r0 = posicion valida
				cmp r0, r8
				beq fin_while
				
				add r1, r1, r6 @ FA = FA + SF
				add r2, r2, r7 @ CA = CA + SC
				ldr r0, [r5] @ r0 = *longitud
				add r0, r0, #1 @ *longitud = *longitud + 1
				str r0, [r5]
				mov r0, r4
				b while
				
				@ if ((posicion_valida == 1) && (casilla == color) && (*longitud > 0))
fin_while:
				ldr r1, [fp, #-24]
				cmp r1, #1
				cmpeq r0, r8
				ldreq r0, [r5] @ r0 = *longitud (se puede optimizar)
				cmpeq r0, #0
				movhi r0, #1
				movls r0, #0
				
				add sp, sp, #8
				POP {r4-r8}
				POP {r11}
				POP {pc}

patron_volteo_arm_arm:
				PUSH {lr}
				PUSH {r11}
				PUSH {r0-r10}
				POP {lr}
				POP {r11}
				
