/*
*********************************************************************************************************
* RTOS EXCEPTION VECTORS
*********************************************************************************************************
* File      : ucos_vectors.S
* For       : Cortex A9
* Toolchain : GNU
*********************************************************************************************************
*/

.text

.global _install_ucos_vector_table

    .align 5
_ucos_vector_table:
    B   _boot
    B   OS_CPU_ARM_ExceptUndefInstrHndlr
    B   OS_CPU_ARM_ExceptSwiHndlr
    B   OS_CPU_ARM_ExceptPrefetchAbortHndlr
    B   OS_CPU_ARM_ExceptDataAbortHndlr
    NOP /* Placeholder for address exception vector*/
    B   OS_CPU_ARM_ExceptIrqHndlr
    B   OS_CPU_ARM_ExceptFiqHndlr


    .align 4
_install_ucos_vector_table:
    mrs     r1, CPSR

    CPSID   IF

    ldr     r0, =_ucos_vector_table
    mcr     p15, 0, r0, c12, c0, 0      /* Switch to the uCOS exception vector table. */

    mov     r0, #0
    mcr     p15, 0, r0, c7, c5, 0       /* Invalidate icache */
    mcr     p15, 0, r0, c7, c5, 6       /* Invalidate branch predictor array */

    isb

    msr     CPSR, r1
    bx lr

.end
