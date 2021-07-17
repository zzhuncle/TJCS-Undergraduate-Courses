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
#include  <xtime_l.h>

#include  "ucos_osii_bsp.h"
#include  <ucos_int.h>
#include  <os.h>


static CPU_INT64U TmrStepSize;

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
    CPU_INT64U tmr_cnt;


    (void)p_int_arg;

#if (UCOS_ZYNQ_MPSOC_A53_TICK_SECURE_EL3 == DEF_ENABLED)
    tmr_cnt = mfcp(CNTPS_CVAL_EL1);
    tmr_cnt += TmrStepSize;
    mtcp(CNTPS_CVAL_EL1, tmr_cnt);
#else
    tmr_cnt = mfcp(CNTP_CVAL_EL0);
    tmr_cnt += TmrStepSize;
    mtcp(CNTP_CVAL_EL0, tmr_cnt);
#endif

    OSTimeTick();
}


/*
 *********************************************************************************************************
 *                                            BSP_OS_TmrTickInit()
 *
 * Description : Initialize uC/OS-II's tick source
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

    TmrStepSize = COUNTS_PER_SECOND / tick_rate;

#if (UCOS_ZYNQ_MPSOC_A53_TICK_SECURE_EL3 == DEF_ENABLED)
    UCOS_IntVectSet (29u,
                     0u,
                     0u,
                     UCOS_TmrTickHandler,
                     DEF_NULL);

    UCOS_IntSrcEn (29u);

    mtcp(CNTPS_TVAL_EL1, TmrStepSize);
    mtcp(CNTPS_CTL_EL1, 0x1u);
#else
    UCOS_IntVectSet (30u,
                     0u,
                     0u,
                     UCOS_TmrTickHandler,
                     DEF_NULL);

    UCOS_IntSrcEn (30u);

    mtcp(CNTP_TVAL_EL0, TmrStepSize);
    mtcp(CNTP_CTL_EL0, 0x1u);
#endif
}
