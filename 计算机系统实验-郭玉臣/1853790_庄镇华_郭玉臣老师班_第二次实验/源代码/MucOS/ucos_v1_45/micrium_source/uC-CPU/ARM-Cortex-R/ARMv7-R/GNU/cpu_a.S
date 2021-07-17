@********************************************************************************************************
@                                                uC/CPU
@                                    CPU CONFIGURATION & PORT LAYER
@
@                         (c) Copyright 2004-2019; Silicon Laboratories Inc.,
@                                400 W. Cesar Chavez, Austin, TX 78701
@
@                   All rights reserved. Protected by international copyright laws.
@
@                  Your use of this software is subject to your acceptance of the terms
@                  of a Silicon Labs Micrium software license, which can be obtained by
@                  contacting info@micrium.com. If you do not agree to the terms of this
@                  license, you may not use this software.
@
@                  Please help us continue to provide the Embedded community with the finest
@                  software available. Your honesty is greatly appreciated.
@
@                    You can find our product's documentation at: doc.micrium.com
@
@                          For more information visit us at: www.micrium.com
@********************************************************************************************************

@********************************************************************************************************
@
@                                            CPU PORT FILE
@
@                                               ARMv7-R
@                                           GNU C Compiler
@
@ Filename : cpu_a.S
@ Version  : V1.31.05
@********************************************************************************************************


@********************************************************************************************************
@                                           .global FUNCTIONS
@********************************************************************************************************

    .global  CPU_SR_Save
    .global  CPU_SR_Restore

    .global  CPU_IntDis
    .global  CPU_IntEn
    
    .global  CPU_WaitForInt
    .global  CPU_WaitForEvent

    .global  CPU_CntLeadZeros
    .global  CPU_CntTrailZeros


@********************************************************************************************************
@                                      CODE GENERATION DIRECTIVES
@********************************************************************************************************

     .code 32


@********************************************************************************************************
@                                     ENABLE & DISABLE INTERRUPTS
@
@ Description : Disable/Enable IRQs & FIQs.
@
@ Prototypes  : void  CPU_IntEn (void)@
@               void  CPU_IntDis(void)@
@********************************************************************************************************

    .type   CPU_IntDis, %function
CPU_IntDis:
        CPSID   IF
        DSB
        BX      LR


    .type   CPU_IntEn, %function
CPU_IntEn:
        DSB
        CPSIE   IF
        BX      LR


@********************************************************************************************************
@                                      CRITICAL SECTION FUNCTIONS
@
@ Description : Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking, the
@               state of the interrupt disable flag is stored in the local variable 'cpu_sr' & interrupts
@               are then disabled ('cpu_sr' is allocated in all functions that need to disable interrupts).
@               The previous interrupt state is restored by copying 'cpu_sr' into the CPU's status register.
@
@ Prototypes  : CPU_SR  CPU_SR_Save   (void)@
@               void    CPU_SR_Restore(CPU_SR  cpu_sr)@
@
@ Note(s)     : (1) These functions are used in general like this :
@
@                       void  Task (void  *p_arg)
@                       {
@                           CPU_SR_ALLOC()@                     /* Allocate storage for CPU status register */
@                               :
@                               :
@                           CPU_CRITICAL_ENTER()@               /* cpu_sr = CPU_SR_Save()@                  */
@                               :
@                               :
@                           CPU_CRITICAL_EXIT()@                /* CPU_SR_Restore(cpu_sr)@                  */
@                               :
@                       }
@********************************************************************************************************

    .type   CPU_SR_Save, %function
CPU_SR_Save:
        MRS     R0, CPSR
        CPSID   IF                                              @ Set IRQ & FIQ bits in CPSR to DISABLE all interrupts
        DSB
        BX      LR                                              @ DISABLED, return the original CPSR contents in R0

    .type   CPU_SR_Restore, %function
CPU_SR_Restore:
        DSB
        MSR     CPSR_c, R0
        BX      LR


@********************************************************************************************************
@                                         WAIT FOR INTERRUPT
@
@ Description : Enters sleep state, which will be exited when an interrupt is received.
@
@ Prototypes  : void  CPU_WaitForInt (void)
@
@ Argument(s) : none.
@********************************************************************************************************

    .type   CPU_WaitForInt, %function
CPU_WaitForInt:
        DSB
        WFI                                     @ Wait for interrupt
        BX      LR



@********************************************************************************************************
@                                         WAIT FOR EXCEPTION
@
@ Description : Enters sleep state, which will be exited when an exception is received.
@
@ Prototypes  : void  CPU_WaitForExcept (void)
@
@ Argument(s) : none.
@********************************************************************************************************

    .type   CPU_WaitForEvent, %function
CPU_WaitForEvent:
        DSB
        WFE                                     @ Wait for exception
        BX      LR


@********************************************************************************************************
@                                         CPU_CntLeadZeros()
@                                        COUNT LEADING ZEROS
@
@ Description : Counts the number of contiguous, most-significant, leading zero bits before the first
@               binary one bit in a data value.
@
@ Prototype   : CPU_DATA  CPU_CntLeadZeros(CPU_DATA  val);
@
@ Argument(s) : val         Data value to count leading zero bits.
@
@ Return(s)   : Number of contiguous, most-significant, leading zero bits in 'val'.
@
@ Note(s)     : (1) If the argument is zero, the value 32 is returned.
@
@               (2) MUST be implemented in cpu_a.asm if and only if CPU_CFG_LEAD_ZEROS_ASM_PRESENT is
@                   #define'd in 'cpu_cfg.h' or 'cpu.h'.
@********************************************************************************************************

    .type   CPU_CntLeadZeros, %function
CPU_CntLeadZeros:
        CLZ     R0, R0                                          @ Count leading zeros
        BX      LR


@********************************************************************************************************
@                                         CPU_CntTrailZeros()
@                                        COUNT TRAILING ZEROS
@
@ Description : Counts the number of contiguous, least-significant, trailing zero bits before the
@                   first binary one bit in a data value.
@
@ Prototype   : CPU_DATA  CPU_CntTrailZeros(CPU_DATA  val);
@
@ Argument(s) : val         Data value to count trailing zero bits.
@
@ Return(s)   : Number of contiguous, least-significant, trailing zero bits in 'val'.
@
@ Note(s)     : (1) (a) Supports 32-bit data value size as configured by 'CPU_DATA' (see 'cpu.h
@                       CPU WORD CONFIGURATION  Note #1').
@
@                   (b) For 32-bit values :
@
@                             b31  b30  b29  b28  b27  ...  b02  b01  b00    # Trailing Zeros
@                             ---  ---  ---  ---  ---       ---  ---  ---    ----------------
@                              x    x    x    x    x         x    x    1            0
@                              x    x    x    x    x         x    1    0            1
@                              x    x    x    x    x         1    0    0            2
@                              :    :    :    :    :         :    :    :            :
@                              :    :    :    :    :         :    :    :            :
@                              x    x    x    x    1         0    0    0           27
@                              x    x    x    1    0         0    0    0           28
@                              x    x    1    0    0         0    0    0           29
@                              x    1    0    0    0         0    0    0           30
@                              1    0    0    0    0         0    0    0           31
@                              0    0    0    0    0         0    0    0           32
@
@
@               (2) MUST be defined in 'cpu_a.asm' (or 'cpu_c.c') if CPU_CFG_TRAIL_ZEROS_ASM_PRESENT is
@                   #define'd in 'cpu_cfg.h' or 'cpu.h'.
@********************************************************************************************************

    .type   CPU_CntTrailZeros, %function
CPU_CntTrailZeros:
        RBIT    R0, R0                          @ Reverse bits
        CLZ     R0, R0                          @ Count trailing zeros
        BX      LR


@********************************************************************************************************
@                                     CPU ASSEMBLY PORT FILE END
@********************************************************************************************************

    .end

