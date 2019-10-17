				.section .text
				.global patron_volteo_arm_c
				.global patron_volteo_arm_arm
				.global patron_volteo_arm_arm_opt

.equ DIM, 8
.equ DESPL, 3   @2^DESPL = DIM

.equ CASILLA_VACIA, 0
.equ NO_HAY_PATRON, 0
.equ PATRON_ENCONTRADO, 1

				


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
				ldr r10,[r5]		@Cargo longitud en r10
				
WHILE_C:		add r6,r6,r8		@FA = FA + SF
				add r7, r7, r9		@CA = CA + SC
				@Llamada a la función ficha_valida
				mov r0, r4 			@r0 = @tablero
				mov r1, r6			@r1 = FA
				mov r2, r7			@r2 = CA
				sub r3, fp, #36		@r3 = @ de posicion_valida
				bl ficha_valida		@ En r0 tenemos casilla y en fp - 36 posicion_valida
				ldr r1,[fp,#-36]	@r1 = posicion_valida
				cmp r1, #1			@ver si la posicion es valida
				bne FIN_WHILE_C
				ldr r3,[fp, #12]    @r3 = color
				cmp r0, r3			@comparar casilla con color
				beq FIN_WHILE_C
				add r10,r10,#1		@Incrementar longitud
				b WHILE_C

FIN_WHILE_C:	cmp r1,#1			@posicion_valida == 1
				cmpeq r0, r3		@casilla == color
				cmpeq r10, #0		@longitud > 0
				movhi r0, #PATRON_ENCONTRADO
				movls r0, #NO_HAY_PATRON
				str	r10,[r5]		@Guardar longitud
				ldmdb fp,{r4-r10, fp, sp, pc}


/*Función optimizada*/
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
WHILE_ASM_OPT:		add r2, r2, r4
				@ CA = CA + SC
				add r3, r3, r5
				
				add r10, r0, r2, LSL #DESPL @añadir fila
				add r10, r10, r3	@añadir columna
				
				@ ficha válida
				@ FA < DIM && FA >= 0 && CA < DIM && CA >= 0 && (tablero[FA][CA] != CASILLA_VACIA)
				cmp r2, #DIM
				bge FIN_ASM_OPT 	@ FA >= DIM
				cmp r2, #0	
				blt FIN_ASM_OPT		@ FA < 0
				cmp r3, #DIM
				bge FIN_ASM_OPT		@ CA >= DIM
				cmp r3, #0
				blt FIN_ASM_OPT		@ CA < 0

				ldrb r7, [r10]	@ r7=tablero[FA][CA]
				cmp r7, #CASILLA_VACIA
				beq FIN_ASM_OPT		@r7=CASILLA_VACIA
				
				
				@ En este punto la posición es válida
				cmp r7, r6
				beq FIN_VALIDA_ASM_OPT		@ tablero[FA][CA] == color
				@ sumar 1 a longitud
				add r8, r8, #1
				b WHILE_ASM_OPT
				
FIN_VALIDA_ASM_OPT: cmp r8, #0
				movne r9, #PATRON_ENCONTRADO
				
FIN_ASM_OPT:		str r8, [r1]	@ Almacenar longitud calculada
				mov r0, r9		@ Mover resultado de la subrutina a r0
				ldmdb fp,{r4-r10, fp, sp, pc}

/* Respetando el código */
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
WHILE_ASM:		add r2, r2, r4
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
				bne WHILE_COND_ASM		@r7 != CASILLA_VACIA

NO_VALIDA:		mov r9, #0			@posicion_valida=0
				mov r7, #CASILLA_VACIA

WHILE_COND_ASM: @(posicion_valida == 1) && (casilla != color)
				cmp r9, #1
				bne IF_ASM
				cmp r7, r6
				beq IF_ASM		@ tablero[FA][CA] == color
				@ sumar 1 a longitud
				add r8, r8, #1
				b WHILE_ASM


IF_ASM: 		cmp r9, #1   @posicion_valida==1
				cmpeq r7, r6 @casilla==color
				cmpeq r8, #0
				movhi r1, #1
				movls r0, #0
				str r8, [r1]	@ Almacenar longitud calculada
				ldmdb fp,{r4-r10, fp, sp, pc}
				
				
				
				
				

