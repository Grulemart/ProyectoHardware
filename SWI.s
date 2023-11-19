;/*****************************************************************************/
;/* SWI.S: SWI Handler                                                        */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2006 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/

; Standard definitions of Interrupt (I & F) flags and thumb bit in PSRs

I_Bit           EQU     0x80            ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40            ; when F bit is set, FIQ is disabled
T_Bit           EQU     0x20

                PRESERVE8                      ; 8-Byte aligned Stack
                AREA    SWI_Area, CODE, READONLY
                ARM

                EXPORT  SWI_Handler
SWI_Handler   

                STMFD   SP!, {R12, LR}         ; Store R12, LR
                MRS     R12, SPSR              ; Get SPSR
                STMFD   SP!, {R8, R12}         ; Store R8, SPSR
                TST     R12, #T_Bit            ; Check Thumb Bit
                LDRNEH  R12, [LR,#-2]          ; Thumb: Load Halfword
                BICNE   R12, R12, #0xFF00      ; Extract SWI Number
                LDREQ   R12, [LR,#-4]          ; ARM:   Load Word
                BICEQ   R12, R12, #0xFF000000  ; Extract SWI Number

                CMP     R12, #0xFF
                BEQ     __disable_fiq
                CMP     R12, #0xFE
                BEQ     __enable_irq
                CMP     R12, #0xFD
                BEQ     __disable_irq

                LDR     R8, SWI_Count
                CMP     R12, R8
                BHS     SWI_Dead               ; Overflow
                ADR     R8, SWI_Table
                LDR     R12, [R8,R12,LSL #2]   ; Load SWI Function Address
                MOV     LR, PC                 ; Return Address
                BX      R12                    ; Call SWI Function 

                LDMFD   SP!, {R8, R12}         ; Load R8, SPSR
                MSR     SPSR_cxsf, R12         ; Set SPSR
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return

SWI_Dead        B       SWI_Dead               ; None Existing SWI

SWI_Cnt         EQU    (SWI_End-SWI_Table)/4
SWI_Count       DCD     SWI_Cnt

                IMPORT  __SWI_0
__SWI_1         EQU     read_IRQ_bit

SWI_Table
                DCD     __SWI_0                ; SWI 0 Function Entry
                DCD     __SWI_1                ; SWI 1 Function Entry
;                DCD     __SWI_2                ; SWI 2 Function Entry
;                DCD     __SWI_3                ; SWI 3 Function Entry

SWI_End

;/**
; * @brief Lee el bit IRQ del registro de estado para saber si las interrupciones
; *        están habilitadas o deshabilitadas
; */
read_IRQ_bit
  LDMFD   SP!, {R8, R12}         ; Load R8, SPSR
  AND     R0, R12, #I_Bit
  MOV     R0, R0, LSR #7
  MSR     SPSR_cxsf, R12         ; Set SPSR
  LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return

;/**
; * @brief Desactiva sólo las interrupciones fiq en el registro de estado.
; */
__disable_fiq
  LDMFD   SP!, {R8, R12}         ; Load R8, SPSR
  ORR     R12, R12, #F_Bit       ; f bit = 1
  MSR     SPSR_cxsf, R12         ; Set SPSR
  LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return

;/**
; * @brief Activa sólo las interrupciones irq en el registro de estado.
; */
__enable_irq
  LDMFD   SP!, {R8, R12}            ; Load R8, SPSR
  BIC     R12, R12, #I_Bit          ; i bit = 0
  MSR     SPSR_cxsf, R12            ; Set SPSR
  LDMFD   SP!, {R12, PC}^           ; Restore R12 and Return

;/**
; * @brief Desactiva sólo las interrupciones irq en el registro de estado.
; */
__disable_irq
  LDMFD   SP!, {R8, R12}            ; Load R8, SPSR
  ORR     R12, R12, #I_Bit          ; i bit = 1
  MSR     SPSR_cxsf, R12            ; Set SPSR
  LDMFD   SP!, {R12, PC}^           ; Restore R12 and Return

                END