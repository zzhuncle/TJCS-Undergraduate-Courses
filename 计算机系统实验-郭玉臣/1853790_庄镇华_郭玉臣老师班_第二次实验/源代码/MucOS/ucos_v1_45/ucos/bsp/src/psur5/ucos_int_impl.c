/*
*********************************************************************************************************
*                                             uCOS XSDK BSP
*
*                      (c) Copyright 2014-2020; Silicon Laboratories Inc.,
*                             400 W. Cesar Chavez, Austin, TX 78701
*
*                All rights reserved. Protected by international copyright laws.
*
*               Your use of this software is subject to your acceptance of the terms
*               of a Silicon Labs Micrium software license, which can be obtained by
*               contacting info@micrium.com. If you do not agree to the terms of this
*               license, you may not use this software.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available. Your honesty is greatly appreciated.
*
*                 You can find our product's documentation at: doc.micrium.com
*
*                       For more information visit us at: www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                    Cortex-R5 Interrupt Management
*
* File : ucos_int_impl.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <xparameters.h>
#include  <xil_exception.h>
#include  <xscugic.h>
#include  <vectors.h>
#include  <cpu.h>
#include  <ucos_bsp.h>
#include  <ucos_int.h>
#include  <os_cpu.h>


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  XScuGic  UCOS_Intc;                                     /* SCUGIC Instance for the UCOS_Int API.                */


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            UCOS_IntInit()
*
* Description : Initialize the SCU interrupt controller and register its handler as the IRQ exception.
*
* Argument(s) : none.
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : This should only be called from the UCOS BSP.
*
*********************************************************************************************************
*/

CPU_BOOLEAN UCOS_IntInit (void)
{
    s32              Status;
    XScuGic_Config  *IntcConfig;


    /*
     * Initialize the SCUGIC controller.
     */
    IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_0_DEVICE_ID);
    Status     = XScuGic_CfgInitialize(&UCOS_Intc,
                                        IntcConfig,
                                        IntcConfig->CpuBaseAddress);

    if (Status != XST_SUCCESS) {
        return DEF_FAIL;
    }

    /*
     * Initialize the exception table and register the interrupt
     * controller handler with the exception table
     */
    Xil_ExceptionDisable();

    Xil_ExceptionInit();

    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, &UCOS_Intc);

    Xil_ExceptionEnable();

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                            UCOS_IntSrcEn()
*
* Description : Enable an interrupt source.
*
* Argument(s) : int_id     ID of the interrupt to enable.
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN UCOS_IntSrcEn (CPU_INT32U int_id)
{
    if(int_id >= XSCUGIC_MAX_NUM_INTR_INPUTS) {
        return (DEF_FAIL);
    }

    XScuGic_Enable(&UCOS_Intc, int_id);

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                            UCOS_IntSrcDis()
*
* Description : Disable an interrupt source.
*
* Argument(s) : int_id     ID of the interrupt to disable.
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : UCOS_IntSrcDis() won't clear a pending interrupt.
*
*********************************************************************************************************
*/

CPU_BOOLEAN UCOS_IntSrcDis (CPU_INT32U int_id)
{
    if(int_id >= XSCUGIC_MAX_NUM_INTR_INPUTS) {
        return (DEF_FAIL);
    }

    XScuGic_Disable(&UCOS_Intc, int_id);

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                            UCOS_IntPrioSet()
*
* Description : Configure the priority of an interrupt source.
*
* Argument(s) : int_id     ID of the interrupt to configure.
*               int_prio   Interrupt priority.
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN UCOS_IntPrioSet (CPU_INT32U int_id, CPU_INT32U int_prio)
{
    u8 xilprio;
    u8 xiltype;


    if(int_id >= XSCUGIC_MAX_NUM_INTR_INPUTS) {
        return (DEF_FAIL);
    }

    if (int_prio > XSCUGIC_INTR_PRIO_MASK) {
        return (DEF_FAIL);
    }

    Xil_ExceptionDisable();

    XScuGic_GetPriorityTriggerType(&UCOS_Intc, int_id, &xilprio, &xiltype);

    xilprio = (u8)int_prio;
    XScuGic_SetPriorityTriggerType(&UCOS_Intc, int_id, xilprio, xiltype);

    Xil_ExceptionEnable();

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                            UCOS_IntTargetSet()
*
* Description : Configure the target list of an interrupt source.
*
* Argument(s) : int_id     ID of the interrupt to configure.
*               int_target_list  List of CPU that can be interrupted or 0 for the current CPU.
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN UCOS_IntTargetSet (CPU_INT32U int_id, CPU_INT32U int_target_list)
{
    u8 cpuid;
    u8 isSet;


    if(int_id >= XSCUGIC_MAX_NUM_INTR_INPUTS) {
        return (DEF_FAIL);
    }

    if (int_target_list > 0xFF) {
        return (DEF_FAIL);
    }

    Xil_ExceptionDisable();

    if (int_target_list == 0u) {
        XScuGic_InterruptMaptoCpu(&UCOS_Intc, XPAR_CPU_ID, int_id);
    } else {
        for (cpuid = 0; cpuid < 8; cpuid++) {
            isSet = int_target_list & (1 << cpuid);
            if (isSet) {
                XScuGic_InterruptMaptoCpu(&UCOS_Intc, cpuid, int_id);
            }
        }
    }

    Xil_ExceptionEnable();

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                            UCOS_IntTypeSet()
*
* Description : Configure the trigger type on an interrupt source.
*
* Argument(s) : int_id     ID of the interrupt to configure.
*               type       Interrupt type.
*                   UCOS_INT_TYPE_LEVEL - Level sensitivity
*                   UCOS_INT_TYPE_EDGE  - Edge sensitivity
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN UCOS_IntTypeSet (CPU_INT32U int_id, UCOS_INT_TYPE type)
{
    u8 xilprio;
    u8 xiltype;

    if(int_id >= XSCUGIC_MAX_NUM_INTR_INPUTS) {
        return (DEF_FAIL);
    }

    Xil_ExceptionDisable();

    XScuGic_GetPriorityTriggerType(&UCOS_Intc, int_id, &xilprio, &xiltype);

    xiltype = (type == UCOS_INT_TYPE_LEVEL) ? (0x01): (0x03);
    XScuGic_SetPriorityTriggerType(&UCOS_Intc, int_id, xilprio, xiltype);
    Xil_ExceptionEnable();


    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                            UCOS_IntVectSet()
*
* Description : Register an interrupt handler.
*
* Argument(s) : int_id           ID of the interrupt to register.
*               int_prio         Interrupt priority.
*               int_target_list  List of CPU that can be interrupted or 0 for the current CPU.
*               int_fnct         Handler to register.
*               p_int_arg        Argument given to the handler.
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : An interrupt handler has the following signature :
*
*               void Handler (void *data)
*               {
*               }
*
*********************************************************************************************************
*/

CPU_BOOLEAN UCOS_IntVectSet (CPU_INT32U          int_id,
                             CPU_INT32U          int_prio,
                             CPU_INT08U          int_target_list,
                             UCOS_INT_FNCT_PTR   int_fnct,
                             void               *p_int_arg)
{
    s32 Status;
    u8  cpuid;
    u8  isSet;
    u8  xilprio;
    u8  xiltype;


    if(int_id >= XSCUGIC_MAX_NUM_INTR_INPUTS) {
        return (DEF_FAIL);
    }

    if (int_prio > XSCUGIC_INTR_PRIO_MASK) {
        return (DEF_FAIL);
    }


    Xil_ExceptionDisable();

    if (int_target_list == 0u) {
        XScuGic_InterruptMaptoCpu(&UCOS_Intc, XPAR_CPU_ID, int_id);
    } else {
        for (cpuid = 0; cpuid < 8; cpuid++) {
            isSet = int_target_list & (1 << cpuid);
            if (isSet) {
                XScuGic_InterruptMaptoCpu(&UCOS_Intc, cpuid, int_id);
            }
        }
    }

    XScuGic_GetPriorityTriggerType(&UCOS_Intc, int_id, &xilprio, &xiltype);

    xilprio = (u8)int_prio;
    XScuGic_SetPriorityTriggerType(&UCOS_Intc, int_id, xilprio, xiltype);

    Status = XScuGic_Connect(&UCOS_Intc, int_id, int_fnct, p_int_arg);

    Xil_ExceptionEnable();

    if (Status != XST_SUCCESS) {
        return DEF_FAIL;
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                          OS_CPU_ExceptHndlr()
*
* Description : Handle any exceptions.
*
* Argument(s) : except_id     ARM exception type:
*
*                                  OS_CPU_ARM_EXCEPT_RESET             0x00
*                                  OS_CPU_ARM_EXCEPT_UNDEF_INSTR       0x01
*                                  OS_CPU_ARM_EXCEPT_SWI               0x02
*                                  OS_CPU_ARM_EXCEPT_PREFETCH_ABORT    0x03
*                                  OS_CPU_ARM_EXCEPT_DATA_ABORT        0x04
*                                  OS_CPU_ARM_EXCEPT_ADDR_ABORT        0x05
*                                  OS_CPU_ARM_EXCEPT_IRQ               0x06
*                                  OS_CPU_ARM_EXCEPT_FIQ               0x07
*
* Return(s)   : none.
*
* Caller(s)   : OS_CPU_ARM_EXCEPT_HANDLER(), which is declared in os_cpu_a.s.
*
* Note(s)     : (1) We invoke the exception handlers set through the Xil_Exception mechanism.
*********************************************************************************************************
*/

void OS_CPU_ExceptHndlr(CPU_INT32U except_id) {

    switch (except_id) {
    case OS_CPU_ARM_EXCEPT_FIQ:
        FIQInterrupt();
        break;

    case OS_CPU_ARM_EXCEPT_IRQ:
        IRQInterrupt();
        break;

    case OS_CPU_ARM_EXCEPT_UNDEF_INSTR:
        UndefinedException();
        break;

    case OS_CPU_ARM_EXCEPT_SWI:
        SWInterrupt();
        break;

    case OS_CPU_ARM_EXCEPT_PREFETCH_ABORT:
        PrefetchAbortInterrupt();
        break;

    case OS_CPU_ARM_EXCEPT_DATA_ABORT:
        DataAbortInterrupt();
        break;

    case OS_CPU_ARM_EXCEPT_ADDR_ABORT:  /* No handler defined by vector.c */
    case OS_CPU_ARM_EXCEPT_RESET:       /* Not handled by the OS port. */
    default:
        while (DEF_TRUE) {
            CPU_WaitForEvent();
        }
    }
}

