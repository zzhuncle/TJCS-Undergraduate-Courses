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
#include  <xttcps.h>

#include  "ucos_osii_bsp.h"
#include  <ucos_int.h>
#include  <os.h>


XTtcPs  OSTickTmr;

#if (XPAR_CPU_ID == 0)
#define  TTC_TIMER_TICK           (XPAR_XTTCPS_0_DEVICE_ID)
#define  TTC_TIMER_INT            (XPAR_PSU_TTC_0_INTR)
#else
#define  TTC_TIMER_TICK           (XPAR_XTTCPS_1_DEVICE_ID)
#define  TTC_TIMER_INT            (XPAR_PSU_TTC_1_INTR)
#endif


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
    XTtcPs_ClearInterruptStatus((XTtcPs *)p_int_arg, XTTCPS_IXR_INTERVAL_MASK);

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

void UCOS_TmrTickInit (CPU_INT32U tick_rate)
{
    s32 Status;
    XTtcPs_Config  *tmr_cfg;
    CPU_BOOLEAN isOk;


    tmr_cfg = XTtcPs_LookupConfig(TTC_TIMER_TICK);
    Status  = XTtcPs_CfgInitialize(&OSTickTmr,
                                    tmr_cfg,
                                    tmr_cfg->BaseAddress);

    if (Status != XST_SUCCESS) {
        return;
    }

    XTtcPs_SetPrescaler(&OSTickTmr, XTTCPS_CLK_CNTRL_PS_DISABLE);
    XTtcPs_SetInterval(&OSTickTmr, tmr_cfg->InputClockHz / tick_rate);
    Status = XTtcPs_SetOptions(&OSTickTmr, XTTCPS_OPTION_INTERVAL_MODE);

    if (Status != XST_SUCCESS) {
        return;
    }

    isOk = UCOS_IntVectSet ( TTC_TIMER_INT,
                             0u,
                             0u,
                             UCOS_TmrTickHandler,
                            &OSTickTmr);

    if (isOk != DEF_OK) {
        return;
    }

    isOk = UCOS_IntSrcEn (TTC_TIMER_INT);

    if (isOk != DEF_OK) {
        return;
    }

    XTtcPs_EnableInterrupts (&OSTickTmr, XTTCPS_IXR_INTERVAL_MASK);
    XTtcPs_Start(&OSTickTmr);
}

