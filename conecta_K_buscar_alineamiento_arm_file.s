
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
;	r7 = offset para acceder a t->columnas[fila][col] y t->no_ceros[fila][co
;	r8 = t->columnas[fila][col] y t->no_ceros[fila][col]
;	r9 = t->no_ceros[fila][col] == color
;	r10 = Dirrecion de la fia

	PRESERVE8
	AREA datos, DATA, READWRITE
                 
ERROR EQU 1
EXITO EQU 0
	
MAX_FILA EQU 7
MAX_COLUMNA EQU 7
MAX_NO_CERO EQU 6
	
MAX_TAMANYO_T_COLUMNAS EQU 42
	
INVALIDA EQU 0xFF
	

	AREA codigo, CODE
	EXPORT conecta_K_buscar_alineamiento_arm
					 
conecta_K_buscar_alineamiento_arm	

									; Protocolo de epilogo de pila
									mov IP, SP
									push{r4-r11,lr}
									sub FP, IP, #4
									
									; Cogen los valores delta_fila y delta_columna
									ldr r4, [FP, #4]	
									ldr r5, [FP, #8]	

									; Funcion tablero_buscar_color	
									
									; !tablero_fila_valida(fila)
									cmp r1, #MAX_FILA
									bge errortag
									; !tablero_columna_valida(columna)
									cmp r2, #MAX_COLUMNA
									bge errortag
										
									;col = 0
									mov r6, #0	
									;r7 offset para acceder @[fila][0]
									mov r10, #MAX_NO_CERO	; Cada fila del array tiene MAX_NO_CERO unidades
									mul r7, r1, r10  		; Acceder a la fila del tablero
									
									; col < MAX_NO_CERO
for									cmp r6, #MAX_NO_CERO
									bge done
																		
									ldrb r8, [r0, r7]    	; r8 <= t->columnas[fila][col]
									
									cmp r8, r2				; columna != t->columnas[fila][col]
									beq done
									
									; Vamos a comprobar si es invalida, si no lo es vamos a done
									cmp r8, #INVALIDA
									beq errortag
									
									; ++col
									add r6,r6,#1
									add r7, r7,#1			; r7 <= ofsset @[fila][col]

									b for

done								; Si sale del bucle por iteraciones da error
									; No ha encontrado la celda
									cmp r6, #MAX_NO_CERO ; col == MAX_NO_CERO
									beq errortag
									
									; celda_color(t->no_ceros[fila][col] == color)
									add r7, r7, #MAX_TAMANYO_T_COLUMNAS  ; A�adir todas la unidades de t.columnas para acceder a no_ceros
									ldrb r8, [r0, r7]                    ; r7 <= t->no_ceros[fila][col]
									and r8, r8, #3 						 ; Usamos una and para coger solo los 3 bits que nos interesan
									cmp r8, r3							 ; t->no_ceros[fila][col] == color
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
									add r2, r2, r5	
									
									push{r4, r5}
									; return 1 + conecta_ K_buscar_alineamiento_arm(t, nueva_fila, nueva_columna, color, delta_fila, delta_columna)
									bl conecta_K_buscar_alineamiento_arm
									add SP, SP, #8
									add r0, r0, #1
									
									pop {r4-r11, pc} ; Recuperamos registros

retorno_0							mov r0, #0
									pop {r4-r11,pc} ; Recuperamos registros
	
	END