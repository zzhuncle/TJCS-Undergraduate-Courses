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
*                                    MICRIUM BOARD SUPPORT PACKAGE
*                                                SYSTICK
*
* File : ucos_osii_bsp.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <lib_def.h>
#include  <cpu.h>

#include  <xparameters.h>
#include  <xscutimer.h>

#include  "ucos_osii_bsp.h"
#include  <ucos_int.h>
#include  <os.h>

#if (XPAR_CPU_ID == 0)
#define SCUTMR_CLK_FREQ  (XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2)
#else
#define SCUTMR_CLK_FREQ  (XPAR_PS7_CORTEXA9_1_CPU_CLK_FREQ_HZ / 2)
#endif

static  XScuTimer  OSTickTmr;


/*
*********************************************************************************************************
*                                       BSP_OS_TmrTickHandler()
*
* Description : Interrupt handler for the tick timer
*
* Argument(s) : cpu_id     Source core id
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  UCOS_TmrTickHandler(void *p_int_arg)
{
    XScuTimer_ClearInterruptStatus((XScuTimer *)p_int_arg);

    OSTimeTick();
}


/*
 *********************************************************************************************************
 *                                            BSP_OS_TmrTickInit()
 *
 * Description : Initialize uC/OS-III's tick source
 *
 * Argument(s) : ticks_per_sec              Number of ticks per second.
 *
 * Return(s)   : none.
 *
 * Caller(s)   : Application.
 *
 * Note(s)     : none.
 *********************************************************************************************************
 */

void UCOS_TmrTickInit(CPU_INT32U tick_rate)
{
    s32 Status;
    XScuTimer_Config *tmr_config;
    CPU_BOOLEAN isOk;


    tmr_config = XScuTimer_LookupConfig(XPAR_XSCUTIMER_0_DEVICE_ID);

    Status = XScuTimer_CfgInitialize(&OSTickTmr,
                                      tmr_config,
                                      tmr_config->BaseAddr);

    if (Status != XST_SUCCESS) {
        return;
    }

    XScuTimer_SetPrescaler(&OSTickTmr, 0u);
    XScuTimer_LoadTimer(&OSTickTmr, (SCUTMR_CLK_FREQ / tick_rate));
    XScuTimer_EnableAutoReload(&OSTickTmr);

    isOk = UCOS_IntVectSet (29u,
                            0u,
                            0u,
                            UCOS_TmrTickHandler,
                            &OSTickTmr);

    if (isOk != DEF_OK) {
        return;
    }

    isOk = UCOS_IntSrcEn (29u);

    if (isOk != DEF_OK) {
        return;
    }

    XScuTimer_EnableInterrupt(&OSTickTmr);
    XScuTimer_Start(&OSTickTmr);
}

