/*
*********************************************************************************************************
*                                             uC/USB-Host
*                                     The Embedded USB Host Stack
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
*                                       USB HOST OS OPERATIONS
*
* Filename : usb_os.h
* Version  : V3.41.09
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/


#ifndef  USBH_OS_H
#define  USBH_OS_H

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/


#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_mem.h>
#include  "usbh_err.h"


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/


#ifdef   USBH_OS_MODULE
#define  USBH_OS_EXT
#else
#define  USBH_OS_EXT  extern
#endif

/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

typedef  CPU_INT32U  USBH_HSEM;                                 /* Handle on semaphores.                                */
typedef  CPU_INT32U  USBH_HMUTEX;                               /* Handle on mutex.                                     */
typedef  CPU_INT32U  USBH_HTASK;                                /* Handle on tasks.                                     */
typedef  CPU_INT32U  USBH_HQUEUE;                               /* Handle on queues.                                    */
typedef  CPU_INT32U  USBH_HTMR;                                 /* Handle on timers.                                    */

typedef  void        (*USBH_TASK_FNCT)(void  *data);            /* Task function.                                       */

/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

USBH_ERR      USBH_OS_LayerInit      (void);
                                                                /* --------------- DELAY TASK FUNCTIONS --------------- */
void          USBH_OS_DlyMS          (CPU_INT32U        dly);

void          USBH_OS_DlyUS          (CPU_INT32U        dly);

                                                                /* ----------------- MUTEX FUNCTIONS ------------------ */
USBH_ERR      USBH_OS_MutexCreate    (USBH_HMUTEX      *p_mutex);

USBH_ERR      USBH_OS_MutexLock      (USBH_HMUTEX       mutex);

USBH_ERR      USBH_OS_MutexUnlock    (USBH_HMUTEX       mutex);

USBH_ERR      USBH_OS_MutexDestroy   (USBH_HMUTEX       mutex);

                                                                /* --------------- SEMAPHORE FUNCTIONS ---------------- */
USBH_ERR      USBH_OS_SemCreate      (USBH_HSEM        *p_sem,
                                      CPU_INT32U        cnt);

USBH_ERR      USBH_OS_SemWait        (USBH_HSEM         sem,
                                      CPU_INT32U        timeout);

USBH_ERR      USBH_OS_SemWaitAbort   (USBH_HSEM         sem);

USBH_ERR      USBH_OS_SemPost        (USBH_HSEM         sem);

USBH_ERR      USBH_OS_SemDestroy     (USBH_HSEM         sem);

                                                                /* ------------------ TASK FUNCTIONS ------------------ */
USBH_ERR      USBH_OS_TaskCreate     (CPU_CHAR         *p_name,
                                      CPU_INT32U        prio,
                                      USBH_TASK_FNCT    task_fnct,
                                      void             *p_data,
                                      CPU_INT32U       *p_stk,
                                      CPU_INT32U        stk_size,
                                      USBH_HTASK       *p_task);

                                                                /* --------------- MSG QUEUE FUNCTIONS ---------------- */
USBH_HQUEUE   USBH_OS_MsgQueueCreate (void            **p_start,
                                      CPU_INT16U        size,
                                      USBH_ERR         *p_err);

USBH_ERR      USBH_OS_MsgQueuePut    (USBH_HQUEUE       msg_q,
                                      void             *p_msg);

void         *USBH_OS_MsgQueueGet    (USBH_HQUEUE       msg_q,
                                      CPU_INT32U        timeout,
                                      USBH_ERR         *p_err);

                                                                /* ----------- INTERNAL USE TIMER FUNCTIONS ----------- */
USBH_HTMR     USBH_OS_TmrCreate      (CPU_CHAR         *p_name,
                                      CPU_INT32U        interval_ms,
                                      void            (*p_callback)(void *p_tmr, void *p_arg),
                                      void             *p_callback_arg,
                                      USBH_ERR         *p_err);

USBH_ERR      USBH_OS_TmrStart       (USBH_HTMR         tmr);

USBH_ERR      USBH_OS_TmrDel         (USBH_HTMR         tmr);

                                                                /* ------------------- MISCELLANEOUS ------------------ */
void         *USBH_OS_VirToBus       (void             *x);

void         *USBH_OS_BusToVir       (void             *x);


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/
#endif
