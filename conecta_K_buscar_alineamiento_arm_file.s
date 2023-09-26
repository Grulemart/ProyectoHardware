
; Autor Álvaro López y Pablo Terés Pueyo

;   6 argumnentos:
;    r0 t
;    r1 fila
;    r2 columna
;    r3 color
;    r4 delta_fila
;    r5 delta_columna

;  Otros registros usados:
;	r6 = col
;	r7 = t->columnas[fila][col]
;	r8 = t->no_ceros[fila][col]
;	r9 = t->no_ceros[fila][col] == color
;	r10 = Dirrecion de la fia

	PRESERVE8
	AREA datos, DATA, READWRITE
                 
ERROR EQU 1
EXITO EQU 0
	
MAX_FILA EQU 7
MAX_COLUMNA EQU 7
MAX_NO_CERO EQU 6
	
MAX_TAMANYO_T_COLUMNAS EQU 49

	AREA codigo, CODE
	EXPORT conecta_K_buscar_alineamiento_arm
					 
conecta_K_buscar_alineamiento_arm

	ldr r4, [SP, #24]	; Cogemos de la pila el argumento delta_fila
	ldr r5, [SP, #28]	; Cogemos de la pila el argumento delta_culumna

	; Funcion tablero_buscar_color	
	cmp r1, #MAX_FILA
	bgt ERRORTAG
	cmp r2, #MAX_COLUMNA
	bgt ERRORTAG
	
	mov r6,#0				; col = 0
FOR	cmp r6, #MAX_NO_CERO
	bge DONE
	
	mul r7, r1, r2
	add r7, r7, r6
	ldr r7, [r0,r7]    		; Cargamos en r7 el valor t->columnas[fila][col]
	cmp r7, r2				; t->columnas[fila][col] != columna
	beq DONE
	
	add r6,r6,#1
	b FOR

DONE	
	cmp r6, #MAX_NO_CERO ; col == MAX_NO_CERO
	beq ERROR
	
	mul r7, r1, r2
	add r7, r7, r6
	add r7, r7, #MAX_TAMANYO_T_COLUMNAS
	ldr r7, [r0, r7]    ; Cargamos en r8 el valor t->no_ceros[fila][col]
	cmp r7, r3	; t->no_ceros[fila][col] == color
	bne ERROR
	
	mov r9, #EXITO	
	b FIN

ERRORTAG	
	mov r9, #ERROR

	; Salimos de funcion tablero_buscar_color

FIN	cmp r9, #EXITO
	bne RETORNO_0
	add r1, r1, r4	; delta_fila + 1
	add r2, r2, r5	; delta_columna + 1
	
	bl conecta_K_buscar_alineamiento_arm

	add r0, r0, #1 ; 1 + conecta_k_buscar_alimento_arm 
	pop {r4-r9, pc} ; Recuperamos registros

RETORNO_0	
	mov r0, #0
	pop {r4-r9,pc} ; Recuperamos registros
	
	END