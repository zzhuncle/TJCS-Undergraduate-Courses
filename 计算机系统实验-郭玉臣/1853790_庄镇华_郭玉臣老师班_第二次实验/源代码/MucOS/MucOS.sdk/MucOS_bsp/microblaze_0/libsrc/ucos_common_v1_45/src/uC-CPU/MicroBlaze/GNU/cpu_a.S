/*
*********************************************************************************************************
*                                                uC/CPU
*                                    CPU CONFIGURATION & PORT LAYER
*
*                         (c) Copyright 2004-2019; Silicon Laboratories Inc.,
*                                400 W. Cesar Chavez, Austin, TX 78701
*
*                   All rights reserved. Protected by international copyright laws.
*
*                  Your use of this software is subject to your acceptance of the terms
*                  of a Silicon Labs Micrium software license, which can be obtained by
*                  contacting info@micrium.com. If you do not agree to the terms of this
*                  license, you may not use this software.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available. Your honesty is greatly appreciated.
*
*                    You can find our product's documentation at: doc.micrium.com
*
*                          For more information visit us at: www.micrium.com
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*
*                                            CPU PORT FILE
*
*                                              MicroBlaze
*                                            GNU C Compiler
*
* Filename : cpu_a.S
* Version  : V1.31.05
*********************************************************************************************************
*/

#define _ASMLANGUAGE

#include  <cpu_cfg.h>

/*
*********************************************************************************************************
*                                            PUBLIC FUNCTIONS
*********************************************************************************************************
*/

        .globl      CPU_SR_Save
        .globl      CPU_SR_Restore

/*
*********************************************************************************************************
*                                              EQUATES
*********************************************************************************************************
*/

.equ    CPU_IE_BIT,         0x02

.text


/*
*********************************************************************************************************
*                                         DISABLE INTERRUPTS
*                                      CPU_SR  CPU_SR_Save(void);
*
* Description : Disables the interrupts and returns the RMSR contents.  This allows the IE state to be
*               restored at a subsequent time.
*
*               The variable in the calling routine which the return is set to MUST be declared 'volatile'
*               for proper operation.  There is no guarantee that the proper register will be scheduled
*               for the subsequent 'CPU_SR_Save()' function call if the variable is not declared
*               'volatile'.
*
* Arguments   : None
*
* Returns     : Current RMSR contents in R3
*
* Note(s)     : This function uses the MSRCLR instruction, which is not, by default, recognized by
*               MicroBlaze processors.  Setting the paramter C_USE_MSR_INSTR to '1' in the MHS file
*               allows the processor to recognize this instruction.
*********************************************************************************************************
*/

#ifdef CPU_CFG_MB_HAS_MSR_INST
CPU_SR_Save:
    RTSD    r15, 8
    MSRCLR  r3,  CPU_IE_BIT        /* Save MSR in r3 and disable interrupts              */
#else
CPU_SR_Save:
    ADDIK   r1, r1, -4             /* Save R4 since it's used as a scratchpad register   */
    SW      r4, r1, r0

    MFS     r3, RMSR               /* Read the MSR.  r3 is used as the return value      */
    ANDNI   r4, r3, CPU_IE_BIT     /* Mask off the IE bit                                */
    MTS     RMSR, r4               /* Store the MSR                                      */

    LW      r4, r1, r0             /* Restore R4                                         */
    ADDIK   r1, r1, 4

    AND     r0, r0, r0             /* NO-OP - pipeline flush                             */
    AND     r0, r0, r0             /* NO-OP - pipeline flush                             */
    AND     r0, r0, r0             /* NO-OP - pipeline flush                             */

    RTSD    r15, 8                 /* Return to caller with R3 containing original RMSR  */
    AND     r0, r0, r0             /* NO-OP                                              */
#endif


/*
*********************************************************************************************************
*                                         ENABLE INTERRUPTS
*                                    void CPU_SR_Restore(CPU_SR sr);
*
* Description: Enables the interrupts using the provided data. If the IE bit is set in the argument, the
*              RTID opcode is used to return.  If the IE bis is clear, the standard RTSD is used leaving
*              the interrupts disabled.
*
*              The argument from the calling routine MUST be declared 'volatile' for proper operation.
*              There is no guarantee that the proper register will be scheduled for the 'CPU_SR_Restore()'
*              function call if the variable is not declared 'volatile'.
*
* Arguments   : Saved RMSR contents in R5
*
* Returns     : None
*
* Note(s)     : None
*********************************************************************************************************
*/

CPU_SR_Restore:
    RTSD    r15, 8
    MTS     rMSR, r5                         /* Move the saved status from r5 into rMSR                */


/*
********************************************************************************************************
*                                         CPU_CntLeadZeros()
*                                        COUNT LEADING ZEROS
*
* Description : Counts the number of contiguous, most-significant, leading zero bits before the first
*               binary one bit in a data value.
*
* Prototype   : CPU_DATA  CPU_CntLeadZeros(CPU_DATA  val);
*
* Argument(s) : val         Data value to count leading zero bits.
*
* Return(s)   : Number of contiguous, most-significant, leading zero bits in 'val'.
*
* Note(s)     : (1) If the argument is zero, the value 32 is returned.
*
*               (2) MUST be implemented in cpu_a.asm if and only if CPU_CFG_LEAD_ZEROS_ASM_PRESENT is
*                   #define'd in 'cpu_cfg.h' or 'cpu.h'.
********************************************************************************************************
*/

#ifdef CPU_CFG_LEAD_ZEROS_ASM_PRESENT
.globl CPU_CntLeadZeros
CPU_CntLeadZeros:
    RTSD    r15, 8
    CLZ     r3, r5
#endif


/*
*********************************************************************************************************
*                                     CPU ASSEMBLY PORT FILE END
*********************************************************************************************************
*/

