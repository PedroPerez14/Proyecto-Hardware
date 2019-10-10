				.section .text
				.global patron_volteo_arm_c
				.global patron_volteo_arm_arm

patron_volteo_arm_c:
				mov ip, sp
				push {r4-r10, fp, ip, lr, pc}
				sub fp, ip, #4
				sub sp, #4
				@En r0 está @tablero
				@En r1, @longitud
				@En r2, FA
				@En r3, CA
				mov r4, r0 	@r4 = @tablero
				mov r5, r1 	@r5 = @longitud
				mov r6, r2	@r6 = FA
				mov r7, r3	@r7 = CA
				@Cargar parámetros pasados por la pila
				ldrsb r8,[fp,#4]	@r8 = SF
				ldrsb r9,[fp,#8]	@r9 = SC
				ldrb r10,[fp,#12]	@r10 = color
WHILE:			add r6,r6,r8		@FA = FA + SF
				add r7, r7, r9		@CA = CA + SC
				@Llamada a la función ficha_valida
				mov r0, r4 			@r0 = @tablero
				mov r1, r6			@r1 = FA
				mov r2, r7			@r2 = CA
				sub r3, fp, #36		@r3 = @ de posicion_valida
				bl ficha_valida		@ En r0 tenemos casilla y en fp - 36 posicion_valida
				ldr r2,[r5]			@Cargo longitud en r2
				ldr r1,[fp,#-36]	@r1 = posicion_valida
				cmp r0, r10
				beq FIN_WHILE
				cmp r1, #1			@Segunda comparación del bucle
				bne FIN_WHILE
				add r2,r2,#1
				str	r2,[r5]			@ *longitud = *longitud + 1;
				b WHILE
FIN_WHILE:		cmp r1,#1
				cmpeq r0, r10
				cmpeq r2, #0
				movhi r0, #1
				bhi FIN
				mov r0, #0
FIN:			ldmdb fp,{r4-r10, fp, sp, pc}
