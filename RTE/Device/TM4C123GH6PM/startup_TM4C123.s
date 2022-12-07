;/**************************************************************************//**
; * @file     startup_TM4C123.s
; * @brief    CMSIS Cortex-M4 Core Device Startup File for
; *           TI Tiva TM4C123 Blizzard Class Device
; * @version  V1.00
; * @date     15. May 2013
; *
; * @note
; * Copyright (C) 2011 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB
				
				IMPORT ADC0_Handler

; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              		; Top of Stack
                DCD     Reset_Handler             		; Reset Handler
                DCD     NMI_Handler               		; NMI Handler
                DCD     HardFault_Handler         		; Hard Fault Handler
                DCD     IntDefaultHandler         		; MPU Fault Handler
                DCD     IntDefaultHandler          		; Bus Fault Handler
                DCD     IntDefaultHandler        		; Usage Fault Handler
                DCD     0                         		; Reserved
                DCD     0                         		; Reserved
                DCD     0                         		; Reserved
                DCD     0                         		; Reserved
                DCD     IntDefaultHandler               ; SVCall Handler
                DCD     IntDefaultHandler          		; Debug Monitor Handler
                DCD     0                         		; Reserved
                DCD     IntDefaultHandler            	; PendSV Handler
                DCD     IntDefaultHandler           	; SysTick Handler

                ; External Interrupts

                DCD     IntDefaultHandler             	;   0: GPIO Port A
                DCD     IntDefaultHandler             	;   1: GPIO Port B
                DCD     IntDefaultHandler             	;   2: GPIO Port C
                DCD     IntDefaultHandler				;   3: GPIO Port D
                DCD     IntDefaultHandler             	;   4: GPIO Port E
                DCD     IntDefaultHandler             	;   5: UART0 Rx and Tx
                DCD     IntDefaultHandler             	;   6: UART1 Rx and Tx
                DCD     IntDefaultHandler              	;   7: SSI0 Rx and Tx
                DCD     IntDefaultHandler              	;   8: I2C0 Master and Slave
                DCD     IntDefaultHandler        		;   9: PWM Fault
                DCD     IntDefaultHandler            	;  10: PWM Generator 0
                DCD     IntDefaultHandler            	;  11: PWM Generator 1
                DCD     IntDefaultHandler            	;  12: PWM Generator 2
                DCD     IntDefaultHandler              	;  13: Quadrature Encoder 0
                DCD     ADC0_Handler          		    ;  14: ADC Sequence 0
                DCD     IntDefaultHandler           	;  15: ADC Sequence 1
                DCD     IntDefaultHandler           	;  16: ADC Sequence 2
                DCD     IntDefaultHandler           	;  17: ADC Sequence 3
                DCD     IntDefaultHandler              	;  18: Watchdog timer
                DCD     IntDefaultHandler              	;  19: Timer 0 subtimer A
                DCD     IntDefaultHandler           	;  20: Timer 0 subtimer B
                DCD     IntDefaultHandler           	;  21: Timer 1 subtimer A
                DCD     IntDefaultHandler           	;  22: Timer 1 subtimer B
                DCD     IntDefaultHandler           	;  23: Timer 2 subtimer A
                DCD     IntDefaultHandler           	;  24: Timer 2 subtimer B
                DCD     IntDefaultHandler             	;  25: Analog Comparator 0
                DCD     IntDefaultHandler             	;  26: Analog Comparator 1
                DCD     IntDefaultHandler             	;  27: Analog Comparator 2
                DCD     IntDefaultHandler            	;  28: System Control (PLL, OSC, BO)
                DCD     IntDefaultHandler             	;  29: FLASH Control
                DCD     IntDefaultHandler             	;  30: GPIO Port F
                DCD     IntDefaultHandler             	;  31: GPIO Port G
                DCD     IntDefaultHandler             	;  32: GPIO Port H
                DCD     IntDefaultHandler             	;  33: UART2 Rx and Tx
                DCD     IntDefaultHandler              	;  34: SSI1 Rx and Tx
                DCD     IntDefaultHandler           	;  35: Timer 3 subtimer A
                DCD     IntDefaultHandler           	;  36: Timer 3 subtimer B
                DCD     IntDefaultHandler              	;  37: I2C1 Master and Slave
                DCD     IntDefaultHandler              	;  38: Quadrature Encoder 1
                DCD     IntDefaultHandler              	;  39: CAN0
                DCD     IntDefaultHandler              	;  40: CAN1
                DCD     IntDefaultHandler              	;  41: CAN2
                DCD     0                         		;  42: Reserved
                DCD     IntDefaultHandler               ;  43: Hibernate
                DCD     IntDefaultHandler              	;  44: USB0
                DCD     IntDefaultHandler            	;  45: PWM Generator 3
                DCD     IntDefaultHandler              	;  46: uDMA Software Transfer
                DCD     IntDefaultHandler           	;  47: uDMA Error
                DCD     IntDefaultHandler           	;  48: ADC1 Sequence 0
                DCD     IntDefaultHandler           	;  49: ADC1 Sequence 1
                DCD     IntDefaultHandler           	;  50: ADC1 Sequence 2
                DCD     IntDefaultHandler           	;  51: ADC1 Sequence 3
                DCD     0                         		;  52: Reserved
                DCD     0                         		;  53: Reserved
                DCD     IntDefaultHandler             	;  54: GPIO Port J
                DCD     IntDefaultHandler             	;  55: GPIO Port K
                DCD     IntDefaultHandler             	;  56: GPIO Port L
                DCD     IntDefaultHandler              	;  57: SSI2 Rx and Tx
                DCD     IntDefaultHandler              	;  58: SSI3 Rx and Tx
                DCD     IntDefaultHandler             	;  59: UART3 Rx and Tx
                DCD     IntDefaultHandler             	;  60: UART4 Rx and Tx
                DCD     IntDefaultHandler             	;  61: UART5 Rx and Tx
                DCD     IntDefaultHandler             	;  62: UART6 Rx and Tx
                DCD     IntDefaultHandler             	;  63: UART7 Rx and Tx
                DCD     0                         		;  64: Reserved
                DCD     0                         		;  65: Reserved
                DCD     0                         		;  66: Reserved
                DCD     0                         		;  67: Reserved
                DCD     IntDefaultHandler              	;  68: I2C2 Master and Slave
                DCD     IntDefaultHandler              	;  69: I2C3 Master and Slave
                DCD     IntDefaultHandler           	;  70: Timer 4 subtimer A
                DCD     IntDefaultHandler           	;  71: Timer 4 subtimer B
                DCD     0                         		;  72: Reserved
                DCD     0                         		;  73: Reserved
                DCD     0                         		;  74: Reserved
                DCD     0                         		;  75: Reserved
                DCD     0                         		;  76: Reserved
                DCD     0                         		;  77: Reserved
                DCD     0                         		;  78: Reserved
                DCD     0                         		;  79: Reserved
                DCD     0                         		;  80: Reserved
                DCD     0                         		;  81: Reserved
                DCD     0                         		;  82: Reserved
                DCD     0                         		;  83: Reserved
                DCD     0                         		;  84: Reserved
                DCD     0                         		;  85: Reserved
                DCD     0                         		;  86: Reserved
                DCD     0                         		;  87: Reserved
                DCD     0                         		;  88: Reserved
                DCD     0                         		;  89: Reserved
                DCD     0                         		;  90: Reserved
                DCD     0                         		;  91: Reserved
                DCD     IntDefaultHandler           	;  92: Timer 5 subtimer A
                DCD     IntDefaultHandler           	;  93: Timer 5 subtimer B
                DCD     IntDefaultHandler          		;  94: Wide Timer 0 subtimer A
                DCD     IntDefaultHandler          		;  95: Wide Timer 0 subtimer B
                DCD     IntDefaultHandler          		;  96: Wide Timer 1 subtimer A
                DCD     IntDefaultHandler          		;  97: Wide Timer 1 subtimer B
                DCD     IntDefaultHandler          		;  98: Wide Timer 2 subtimer A
                DCD     IntDefaultHandler          		;  99: Wide Timer 2 subtimer B
                DCD     IntDefaultHandler          		; 100: Wide Timer 3 subtimer A
                DCD     IntDefaultHandler          		; 101: Wide Timer 3 subtimer B
                DCD     IntDefaultHandler          		; 102: Wide Timer 4 subtimer A
                DCD     IntDefaultHandler          		; 103: Wide Timer 4 subtimer B
                DCD     IntDefaultHandler          		; 104: Wide Timer 5 subtimer A
                DCD     IntDefaultHandler          		; 105: Wide Timer 5 subtimer B
                DCD     IntDefaultHandler               ; 106: FPU
                DCD     0                         		; 107: Reserved
                DCD     0                         		; 108: Reserved
                DCD     IntDefaultHandler              	; 109: I2C4 Master and Slave
                DCD     IntDefaultHandler              	; 110: I2C5 Master and Slave
                DCD     IntDefaultHandler             	; 111: GPIO Port M
                DCD     IntDefaultHandler             	; 112: GPIO Port N
                DCD     IntDefaultHandler              	; 113: Quadrature Encoder 2
                DCD     0                         		; 114: Reserved
                DCD     0                         		; 115: Reserved
                DCD     IntDefaultHandler            	; 116: GPIO Port P (Summary or P0)
                DCD     IntDefaultHandler            	; 117: GPIO Port P1
                DCD     IntDefaultHandler            	; 118: GPIO Port P2
                DCD     IntDefaultHandler            	; 119: GPIO Port P3
                DCD     IntDefaultHandler            	; 120: GPIO Port P4
                DCD     IntDefaultHandler            	; 121: GPIO Port P5
                DCD     IntDefaultHandler            	; 122: GPIO Port P6
                DCD     IntDefaultHandler            	; 123: GPIO Port P7
                DCD     IntDefaultHandler            	; 124: GPIO Port Q (Summary or Q0)
                DCD     IntDefaultHandler            	; 125: GPIO Port Q1
                DCD     IntDefaultHandler            	; 126: GPIO Port Q2
                DCD     IntDefaultHandler            	; 127: GPIO Port Q3
                DCD     IntDefaultHandler            	; 128: GPIO Port Q4
                DCD     IntDefaultHandler            	; 129: GPIO Port Q5
                DCD     IntDefaultHandler            	; 130: GPIO Port Q6
                DCD     IntDefaultHandler            	; 131: GPIO Port Q7
                DCD     IntDefaultHandler             	; 132: GPIO Port R
                DCD     IntDefaultHandler             	; 133: GPIO Port S
                DCD     IntDefaultHandler            	; 134: PWM 1 Generator 0
                DCD     IntDefaultHandler            	; 135: PWM 1 Generator 1
                DCD     IntDefaultHandler            	; 136: PWM 1 Generator 2
                DCD     IntDefaultHandler            	; 137: PWM 1 Generator 3
                DCD     IntDefaultHandler        		; 138: PWM 1 Fault

__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
					
IntDefaultHandler
				B       IntDefaultHandler
									
                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
