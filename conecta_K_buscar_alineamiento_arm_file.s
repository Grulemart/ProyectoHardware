
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

									; Protocolo de epilogo de pila
									mov r12, SP
									push{r4-r11,lr}
									sub r11,r12, #4
									
									; Cogen los valores delta_fila y delta_columna
									ldr r4, [r11, #4]	
									ldr r5, [r11, #8]	

									; Funcion tablero_buscar_color	
									
									; !tablero_fila_valida(fila)
									cmp r1, #MAX_FILA
									bge errortag
									; !tablero_columna_valida(columna)
									cmp r2, #MAX_COLUMNA
									bge errortag
										
									;col = 0
									mov r6, #0	

									; col < MAX_NO_CERO
for									cmp r6, #MAX_NO_CERO
									bge done

									; columna != t->columnas[fila][col]
									mov r10, #6		 		; Cada fila del array tiene 6 unidades
									mul r7, r1, r10  		; Acceder a la fila del tablero
									add r7, r7, r6			; r7 <= @[fila][col]
									ldrb r7, [r0, r7]    	; r7 <= t->columnas[fila][col]
									cmp r7, r2				
				
									beq done
									
									; ++col
									add r6,r6,#1
									b for

done								; Si sale del bucle por iteraciones da error
									; No ha encontrado la celda
									cmp r6, #MAX_NO_CERO ; col == MAX_NO_CERO
									beq errortag
									
									; celda_color(t->no_ceros[fila][col] == color)
									mul r7, r1, r10						 ; Cada fila del array tiene 6 unidades
									add r7, r7, r6						 ; Acceder a la fila del tablero
									add r7, r7, #MAX_TAMANYO_T_COLUMNAS  ; Añadir todas la unidades de t.columnas para acceder a no_ceros
									ldrb r7, [r0, r7]                    ; r7 <= t->no_ceros[fila][col]
									and r7, r7, #3 						 ; Traducir celda a color
									cmp r7, r3							 ; t->no_ceros[fila][col] == color
									bne errortag						 
									
									mov r9, #EXITO						
									b final_tablero_buscar_color

errortag							mov r9, #ERROR
									
									; Salimos de funcion tablero_buscar_color

									; Si la celda no coincide devuelve 0
final_tablero_buscar_color			cmp r9, #EXITO
									bne retorno_0
									
									; nueva_fila = fila + delta_fila
									; nueva_columna = columna + delta_columna
									add r1, r1, r4	
									and r1, #0x000000FF
									add r2, r2, r5	
									and r2, #0x000000FF
									
									; return 1 + conecta_ K_buscar_alineamiento_arm(t, nueva_fila, nueva_columna, color, delta_fila, delta_columna)
									bl conecta_K_buscar_alineamiento_arm
									add r0, r0, #1
									
									pop {r4-r11, pc} ; Recuperamos registros

retorno_0							mov r0, #0
									pop {r4-r11,pc} ; Recuperamos registros
	
	END