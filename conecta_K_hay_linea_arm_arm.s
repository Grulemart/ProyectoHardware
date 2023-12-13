
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
;	r9 direcion deltas_fila
;	r10 direccion deltas_columna

	PRESERVE8
	AREA   Datos, DATA, READWRITE

K_SIZE EQU 4
N_DELTAS EQU 4
TRUE EQU 1
FALSE EQU 0

deltas_fila DCD 0,-1,-1,1
deltas_columna DCD -1,0,-1,-1



	AREA   Codigo, CODE, READONLY 
	IMPORT conecta_K_buscar_alineamiento_arm
	
	EXPORT conecta_K_hay_linea_arm_arm
		
	

conecta_K_hay_linea_arm_arm	push{r4-r10,lr} ; Guardamos los registros preservados

							mov r6, #0 ; i = 0
							mov r7, #FALSE ; linea = false
							ldr r9, =deltas_fila
							ldr r10,= deltas_columna
						
for							cmp r7, #FALSE ; linea == FALSE
							bne found
							cmp r6, #N_DELTAS ; i < N_DELTAS
							bge not_found
							
							ldr r4, [r9, r6, lsl #2] ; r4 = deltas_fila[i]	
							ldr r5, [r10, r6, lsl #2] ; r5 = deltas_columna[i]
							
							push{r0-r3}	; Guardamos registros no preservados
							push{r4,r5} ; Pasamos los argumentos restantes por la pila
							
							bl conecta_K_buscar_alineamiento_arm

							mov r8, r0	; long_linea = conecta_k_buscar_alimento_c(...)
							pop{r4, r5} ; Recuperamos deltas_fila y deltas_columna
							pop{r0-r3}	; Recuperamos los registros no preservados
							cmp r8, #K_SIZE	; long_linea >= K_SIZE
							bge found
							
							
							
							sub r4, r4, r4, lsl #1	; r9 = -r4
							sub r5, r5, r5, lsl #1	; r10 = -r5
							push {r0-r3}	; Guardamos argumentos no preservados
							push {r4, r5}	; Pasamos los argumentos por la pila
							
							add r1,r1,r4	; r1 = fila-deltas_fila[i]
							add r2,r2,r5	; r2 = columna-deltas_columna[i]
							
							bl conecta_K_buscar_alineamiento_arm
							
							add r8, r8, r0	; long_linea + conecta_k_buscar_alimento_c(...)
							add SP, SP, #8	; Quitamos de la pila r4,r5 que se pasaron como argumentos
											; No hacemos pop por que no se necesitan
							pop {r0-r3} ; Recuperamos los registros no preservados
							cmp r8, #K_SIZE ; long_linea >= K_SIZE
							bge found

							add r6, r6, #1	; ++i
							b for
			
not_found					mov r0, #FALSE
							b done

found		 				mov r0, #TRUE

done 						pop {r4-r10,pc}
						
			END
	