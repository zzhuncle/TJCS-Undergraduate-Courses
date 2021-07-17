/*
*********************************************************************************************************
* RTOS EXCEPTION VECTORS
*********************************************************************************************************
* File      : ucos_vectors.S
* For       : Cortex A53
* Toolchain : GNU
*********************************************************************************************************
*/

.global _install_ucos_vector_table
.global _ucos_proc_init



/* RTOS VECTOR TABLE */
.section .vectors, "a"
.align 10
_ucos_vector_table:
.org (_ucos_vector_table + 0x200)
    b   _vector_table + 0x200

.org (_ucos_vector_table + 0x280)
    b   OS_CPU_ARM_ExceptIrqHndlr

.org (_ucos_vector_table + 0x300)
    b   _vector_table + 0x300

.org (_ucos_vector_table + 0x380)
    b   _vector_table + 0x380


.text
/* PROCESSOR INITIALIZATION FOR UCOS */
_ucos_proc_init:
    mov     x9, #0
    msr     CPTR_EL3, x9
    ret     lr


/* INSTALL RTOS VECTORS */
_install_ucos_vector_table:
    ldr     x9, =_ucos_vector_table
    mrs     x10, CurrentEL
    lsr     x10, x10, #2

    cmp     x10, #3
    b.eq    install_el3
    cmp     x10, #2
    b.eq    install_el2
    cmp     x10, #1
    b.eq    install_el1

install_el3:
    msr   vbar_el3, x9
    b     sync

install_el2:
    msr   vbar_el2, x9
    b     sync

install_el1:
    msr   vbar_el1, x9
    b     sync

sync:
    dsb     sy
    isb     sy

    ret     lr

.end
