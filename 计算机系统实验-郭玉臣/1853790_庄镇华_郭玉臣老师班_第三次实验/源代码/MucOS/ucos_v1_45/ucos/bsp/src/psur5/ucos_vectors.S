/*
*********************************************************************************************************
* RTOS EXCEPTION VECTORS
*********************************************************************************************************
* File      : ucos_vectors.S
* For       : Cortex R5
* Toolchain : GNU
*********************************************************************************************************
*/
.org 0
.text
.global _install_ucos_vector_table
.global _ucos_vector_table

.extern _vector_table
.extern OS_CPU_ARM_ExceptUndefInstrHndlr
.extern OS_CPU_ARM_ExceptSwiHndlr
.extern OS_CPU_ARM_ExceptPrefetchAbortHndlr
.extern OS_CPU_ARM_ExceptDataAbortHndlr
.extern OS_CPU_ARM_ExceptIrqHndlr
.extern OS_CPU_ARM_ExceptFiqHndlr


/****************/
/* VECTOR TABLE */
/****************/
.section .vectors, "a"
_ucos_vector_table:
    LDR     PC, =_Dyn_Boot
    LDR     PC, =_Dyn_UndefInstr
    LDR     PC, =_Dyn_SVC
    LDR     PC, =_Dyn_PrefetchAbort
    LDR     PC, =_Dyn_DataAbort
    LDR     PC, =_Dyn_Unused
    LDR     PC, =_Dyn_Irq
    LDR     PC, =_Dyn_Fiq


/**********************/
/* EXCEPTION HANDLERS */
/**********************/
.data
UseRTOSVects:
    .word 0

/* Reset Exception */
_Dyn_Boot:
    LDR     PC, =(_vector_table + 0x00)

/* Undefined Instruction Exception */
_Dyn_UndefInstr:
    STMFD   SP!, {R0, R1}
    MRS     R1, SPSR

    LDR     R0, =UseRTOSVects
    LDR     R0, [R0]
    CMP     R0, #1
    BEQ     _Dyn_UndefInstr_RTOS

    MSR     SPSR, R1
    LDMFD   SP!, {R0, R1}
    LDR     PC, =(_vector_table + 0x04)

_Dyn_UndefInstr_RTOS:
    MSR     SPSR, R1
    LDMFD   SP!, {R0, R1}
    B       OS_CPU_ARM_ExceptUndefInstrHndlr

/* Supervisor Call Exception */
_Dyn_SVC:
    STMFD   SP!, {R0, R1}
    MRS     R1, SPSR

    LDR     R0, =UseRTOSVects
    LDR     R0, [R0]
    CMP     R0, #1
    BEQ     _Dyn_SVC_RTOS

    MSR     SPSR, R1
    LDMFD   SP!, {R0, R1}
    LDR     PC, =(_vector_table + 0x08)

_Dyn_SVC_RTOS:
    MSR     SPSR, R1
    LDMFD   SP!, {R0, R1}
    B       OS_CPU_ARM_ExceptSwiHndlr

/* Prefetch Abort Exception */
_Dyn_PrefetchAbort:
    STMFD   SP!, {R0, R1}
    MRS     R1, SPSR

    LDR     R0, =UseRTOSVects
    LDR     R0, [R0]
    CMP     R0, #1
    BEQ     _Dyn_PrefetchAbort_RTOS

    MSR     SPSR, R1
    LDMFD   SP!, {R0, R1}
    LDR     PC, =(_vector_table + 0x0C)

_Dyn_PrefetchAbort_RTOS:
    MSR     SPSR, R1
    LDMFD   SP!, {R0, R1}
    B       OS_CPU_ARM_ExceptPrefetchAbortHndlr

/* Data Abort Exception */
_Dyn_DataAbort:
    STMFD   SP!, {R0, R1}
    MRS     R1, SPSR

    LDR     R0, =UseRTOSVects
    LDR     R0, [R0]
    CMP     R0, #1
    BEQ     _Dyn_DataAbort_RTOS

    MSR     SPSR, R1
    LDMFD   SP!, {R0, R1}
    LDR     PC, =(_vector_table + 0x10)

_Dyn_DataAbort_RTOS:
    MSR     SPSR, R1
    LDMFD   SP!, {R0, R1}
    B       OS_CPU_ARM_ExceptDataAbortHndlr

/* UNUSED */
_Dyn_Unused:
    LDR     PC, =(_vector_table + 0x14)

/* IRQ Exception */
_Dyn_Irq:
    STMFD   SP!, {R0, R1}
    MRS     R1, SPSR

    LDR     R0, =UseRTOSVects
    LDR     R0, [R0]
    CMP     R0, #1
    BEQ     _Dyn_Irq_RTOS

    MSR     SPSR, R1
    LDMFD   SP!, {R0, R1}
    LDR     PC, =(_vector_table + 0x18)

_Dyn_Irq_RTOS:
    MSR     SPSR, R1
    LDMFD   SP!, {R0, R1}
    B       OS_CPU_ARM_ExceptIrqHndlr

/* FIQ Exception */
_Dyn_Fiq:
    STMFD   SP!, {R0, R1}
    MRS     R1, SPSR

    LDR     R0, =UseRTOSVects
    LDR     R0, [R0]
    CMP     R0, #1
    BEQ     _Dyn_Fiq_RTOS

    MSR     SPSR, R1
    LDMFD   SP!, {R0, R1}
    LDR     PC, =(_vector_table + 0x1C)

_Dyn_Fiq_RTOS:
    MSR     SPSR, R1
    LDMFD   SP!, {R0, R1}
    B       OS_CPU_ARM_ExceptFiqHndlr


/* INSTALL RTOS VECTORS */
.text
_install_ucos_vector_table:
    MRS     R0, CPSR

    CPSID   IF

    LDR     R1, =UseRTOSVects
    MOV     R2, #1
    STR     R2, [R1]

    DSB
    ISB

    MSR     CPSR, R0
    BX      LR

.end
