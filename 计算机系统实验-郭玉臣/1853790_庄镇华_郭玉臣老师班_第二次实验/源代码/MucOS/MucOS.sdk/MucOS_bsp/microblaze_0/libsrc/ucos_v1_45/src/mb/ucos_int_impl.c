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
*                              AXI INTERRUPT CONTROLLER FOR THE MICROBLAZE
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
#include  <xintc.h>
#include  <cpu.h>
#include  <ucos_bsp.h>
#include  <ucos_int.h>
#include  <os_cpu.h>


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  XIntc  UCOS_Intc;                                       /* XIntc Instance for the UCOS_Int API.                 */


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
* Description : Initialize the AXI interrupt controller and register its handler as the IRQ exception.
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
    s32 Status;


    /*
     * Initialize the AXI Interrupt controller.
     */
    Status = XIntc_Initialize(&UCOS_Intc, XPAR_INTC_0_DEVICE_ID);

    if (Status != XST_SUCCESS) {
        return DEF_FAIL;
    }

    Status = XIntc_Start(&UCOS_Intc, XIN_REAL_MODE);
    if (Status != XST_SUCCESS) {
        return DEF_FAIL;
    }

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
    if(int_id >= XPAR_INTC_MAX_NUM_INTR_INPUTS) {
        return (DEF_FAIL);
    }

    XIntc_Enable(&UCOS_Intc, int_id);

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
    if(int_id >= XPAR_INTC_MAX_NUM_INTR_INPUTS) {
        return (DEF_FAIL);
    }

    XIntc_Enable(&UCOS_Intc, int_id);

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                            UCOS_IntPrioSet()
*
* Description : Stub Function for the AXI Interrupt Controller.
*
*********************************************************************************************************
*/

CPU_BOOLEAN UCOS_IntPrioSet (CPU_INT32U int_id, CPU_INT32U int_prio)
{
    (void)int_id;
    (void)int_prio;
    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                            UCOS_IntTargetSet()
*
* Description : Stub Function for the AXI Interrupt Controller.
*
*********************************************************************************************************
*/

CPU_BOOLEAN UCOS_IntTargetSet (CPU_INT32U int_id, CPU_INT32U int_target_list)
{
    (void)int_id;
    (void)int_target_list;
    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                            UCOS_IntTypeSet()
*
* Description : Stub Function for the AXI Interrupt Controller.
*
*********************************************************************************************************
*/

CPU_BOOLEAN UCOS_IntTypeSet (CPU_INT32U int_id, UCOS_INT_TYPE type)
{
    (void)int_id;
    (void)type;
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

CPU_BOOLEAN UCOS_IntVectSet (CPU_INT32U         int_id,
                             CPU_INT32U         int_prio,
                             CPU_INT08U         int_target_list,
                             UCOS_INT_FNCT_PTR  int_fnct,
                             void              *p_int_arg)
{
    s32 Status;

    (void)int_prio;
    (void)int_target_list;

    if(int_id >= XPAR_INTC_MAX_NUM_INTR_INPUTS) {
        return (DEF_FAIL);
    }

    Xil_ExceptionDisable();
    Status = XIntc_Connect(&UCOS_Intc, int_id, int_fnct, p_int_arg);
    Xil_ExceptionEnable();

    if (Status != XST_SUCCESS) {
        return DEF_FAIL;
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                          BSP_IntHandler()
*
* Description : Global interrupt handler.
*
* Argument(s) : int_id     ID of the interrupt to enable.
*
* Return(s)   : none.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void BSP_IntHandler (void)
{
    XIntc_DeviceInterruptHandler(XPAR_INTC_0_DEVICE_ID);
}
