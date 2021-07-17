/*
*********************************************************************************************************
*                                               uC/OS-III
*                                          The Real-Time Kernel
*
*                         (c) Copyright 2009-2019; Silicon Laboratories Inc.,
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
*                                           APPLICATION HOOKS
*
* Filename : os_app_hooks.h
* Version  : V3.07.05
*********************************************************************************************************
*/

#ifndef  OS_APP_HOOKS_H
#define  OS_APP_HOOKS_H


#ifdef   OS_APP_HOOKS_H_GLOBALS
#define  OS_APP_HOOKS_H_EXT
#else
#define  OS_APP_HOOKS_H_EXT  extern
#endif

/*
************************************************************************************************************************
*                                                 INCLUDE HEADER FILES
************************************************************************************************************************
*/

#include <os.h>

/*
************************************************************************************************************************
*                                                 FUNCTION PROTOTYPES
************************************************************************************************************************
*/

void  App_OS_SetAllHooks   (void);
void  App_OS_ClrAllHooks   (void);


                                                                /* ---------------------- HOOKS --------------------- */
void  App_OS_IdleTaskHook  (void);

#if (OS_CFG_TASK_STK_REDZONE_EN > 0u)
void  App_OS_RedzoneHitHook(OS_TCB  *p_tcb);
#endif

void  App_OS_StatTaskHook  (void);

void  App_OS_TaskCreateHook(OS_TCB  *p_tcb);

void  App_OS_TaskDelHook   (OS_TCB  *p_tcb);

void  App_OS_TaskReturnHook(OS_TCB  *p_tcb);

void  App_OS_TaskSwHook    (void);

void  App_OS_TimeTickHook  (void);

#endif
