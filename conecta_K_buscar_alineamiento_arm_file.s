
; Autor ?lvaro L?pez y Pablo Ter?s Pueyo

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
									mov r12, SP
									push{r4-r11,lr}
									sub r11,r12, #4
									ldr r4, [r11, #4]	; Cogemos de la pila el argumento delta_fila
									ldr r5, [r11, #8]	; Cogemos de la pila el argumento delta_culumna


									; Funcion tablero_buscar_color	
									cmp r1, #MAX_FILA
									bgt errortag
									cmp r2, #MAX_COLUMNA
									bgt errortag
										
									
									mov r6,#0				; col = 0
for									cmp r6, #MAX_NO_CERO
									bge done

									mov r10, #MAX_COLUMNA
									mul r7, r1, r10  ;
									add r7, r7, r6
									ldrb r7, [r0, r7]    		; Cargamos en r7 el valor t->columnas[fila][col]
									
									cmp r7, r2				; t->columnas[fila][col] != columna
									
									beq done
									
									add r6,r6,#1
									b for

done	
									; ???
									cmp r6, #MAX_NO_CERO ; col == MAX_NO_CERO
									beq errortag
									
									mul r7, r1, r10
									add r7, r7, r6
									add r7, r7, #MAX_TAMANYO_T_COLUMNAS
									ldrb r7, [r0, r7]    ; Cargamos en r8 el valor t->no_ceros[fila][col]
									and r7, r7, #3 
									cmp r7, r3			; t->no_ceros[fila][col] == color
									bne errortag
									
									mov r9, #EXITO	
									b final_tablero_buscar_color

errortag	
									mov r9, #ERROR

				





									; Salimos de funcion tablero_buscar_color

final_tablero_buscar_color			cmp r9, #EXITO
									bne retorno_0
									
									add r1, r1, r4	; delta_fila + 1
									add r2, r2, r5	; delta_columna + 1
									
									push{r0-r9,lr}
									
									bl conecta_K_buscar_alineamiento_arm
									
									mov r10, r0
									pop{r0-r9,lr}

									add r0, r10, #1 ; 1 + conecta_k_buscar_alimento_arm 
									pop {r4-r11, pc} ; Recuperamos registros

retorno_0	
									mov r0, #0
									pop {r4-r11,pc} ; Recuperamos registros
	
	END