
;  4 argumentos:
;	r0 t
;	r1 fila
;	r2 columna 
;	r3 color

;  Variables locales:
;	r4 deltas_fila[i] 
;	r5 deltas_columna[i]
;	r6 i
;	r7 linea
;	r8 long_linea

	PRESERVE8
	AREA   Datos, DATA, READWRITE

N_DELTAS EQU 4
TRUE EQU 1
FALSE EQU 0

deltas_fila DCD 0,-1,-1,1
deltas_columna DCD -1,0,-1,-1

	AREA   Codigo, CODE, READONLY 
	
	IMPORT conecta_K_buscar_alineamiento_c
	EXPORT conecta_K_hay_linea_arm_c
	
conecta_K_hay_linea_arm_c
	push{r4-r11,lr} ; Guardamos los registros preservados
	

	mov r6, #0 ; i = 0
	
	mov r4, #N_DELTAS ; i < N_DELTAS
FOR	cmp r6, r4
	bge NOT_FOUND
	;cmp r8, #FALSE
	;bne NOT_FOUND
	
	ldr r5, =deltas_fila
	ldr r4, [r5, r6] ; r4 = deltas_fila[i]
	ldr r5, =deltas_columna
    ldr r5, [r5, r6] ; r5 = deltas_columna[i]
	
	push{r0-r5}	; Guardamos t y pasamos los argumentos por la pila

	bl conecta_K_buscar_alineamiento_c

	mov r8, r0	; long_linea = conecta_k_buscar_alineamiento_c(...)
	pop{r0-r5}	; Recuperamos t y los argumnetos
	cmp r8, r4	; long_linea >= N_DELTAS
	bge FOUND
	
	sub r1,r1,r4	; fila-deltas_fila[i]
	sub r2,r2,r5	; columna-deltas_columna[i]

	ldr r9, [r5, r7]	    ; r9 = deltas_fila[i]
	ldr r10, [r5, r7]	; r10 = deltas_columna[i]
	sub r9, r10, r10, lsl #1	; r9 = -r9
	sub r10, r11, r11, lsl #1	; r10 = -r10
	push {r0-r4, r9, r10}	; Guardamos argumentos no preservados
	bl conecta_K_buscar_alineamiento_c
	add r9, r9, r0	; long_linea + conecta_k_buscar_alineamiento_c(...)
	pop {r0-r4, r9, r10} ; Recuperamos t
	cmp r9, r4
	bge FOUND

	add r7, r7, #1	; ++i
	b FOR
	
NOT_FOUND	mov r8, #FALSE
			b DONE

FOUND 		mov r8, #TRUE

DONE 		pop {r4-r11,pc}

	END
	