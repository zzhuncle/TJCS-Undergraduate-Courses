/*
*********************************************************************************************************
*                                               uC/Clk
*                                          Clock / Calendar
*
*                         (c) Copyright 2005-2019; Silicon Laboratories Inc.,
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
*                               CLOCK / CALENDAR OPERATING SYSTEM LAYER
*
*                                          Micrium uC/OS-III
*
* Filename : clk.c
* Version  : V3.09.07
*********************************************************************************************************
* Note(s)  : (1) Assumes uC/OS-III V3.01.0 (or more recent version) is included in the project build.
*
*            (2) REQUIREs the following uC/OS-III feature to be ENABLED :
*
*                ------ FEATURE ------     ------ MINIMUM CONFIGURATION FOR Clock/OS PORT ------
*
*                    (a) Semaphores
*                        (1) OS_CFG_SEM_EN         Enabled
*                                                      required only if CLK_CFG_SIGNAL_EN is DEF_ENABLED
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "../../Source/clk.h"
#include  <app_cfg.h>
#include  <os.h>                                                /* See this 'clk_os.c  Note #1'.                        */


/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) See 'clk.h  MODULE CONFIGURATION'.
*********************************************************************************************************
*/

#define  MICRIUM_SOURCE
#ifdef   CLK_OS_MODULE_PRESENT


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     OS TASK/OBJECT NAME DEFINES
*********************************************************************************************************
*/

                                                                /* -------------------- TASK NAME --------------------- */
                                          /*           1         2 */
                                          /* 012345678901234567890 */
#define  CLK_OS_TASK_NAME                   "RealTime Clk"

                                                                /* --------------------- OBJ NAME --------------------- */
                                          /*           1         2 */
                                          /* 012345678901234567890 */
#define  CLK_OS_SIGNAL_NAME                 "Clk Signal"


/*
*********************************************************************************************************
*                                          OS OBJECT DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

                                                                /* --------------------- TASK TCB --------------------- */
static  OS_TCB    Clk_OS_TaskTCB;

                                                                /* -------------------- TASK STACK -------------------- */
static  CPU_STK   Clk_OS_TaskStk[CLK_OS_CFG_TASK_STK_SIZE];


#if (CLK_CFG_SIGNAL_EN == DEF_ENABLED)                          /* ------------------- CLOCK SIGNAL ------------------- */
static  OS_SEM    Clk_OS_SignalObj;                             /* Counting semaphore signals each second elapsed.      */
#endif


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  Clk_OS_Task(void  *p_data);                       /* Clock task.                                          */


/*
*********************************************************************************************************
*                                        DEFAULT CONFIGURATION
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

                                                                /* See this 'clk_os.c  Note #1'.                        */
#if     (OS_VERSION < 3010u)
#error  "OS_VERSION                  [SHOULD be >= V3.01.0]"
#endif



#if     (CLK_CFG_SIGNAL_EN == DEF_ENABLED)
#if     (OS_CFG_SEM_EN < 1u)                                    /* See this 'clk_os.c  Note #2a1'.                      */
#error  "OS_CFG_SEM_EN               illegally #define'd in 'os_cfg.h'           "
#error  "                            [MUST be  > 0u, (see 'clk_os.c  Note #2a1')]"
#endif
#endif



#ifndef  CLK_OS_CFG_TASK_PRIO
#error  "CLK_OS_CFG_TASK_PRIO              not #define'd in 'app_cfg.h'"
#error  "                            [MUST be  >= 0u]                  "

#elif   (CLK_OS_CFG_TASK_PRIO < 0u)
#error  "CLK_OS_CFG_TASK_PRIO        illegally #define'd in 'app_cfg.h'"
#error  "                            [MUST be  >= 0u]                  "
#endif


#ifndef  CLK_OS_CFG_TASK_STK_SIZE
#error  "CLK_OS_CFG_TASK_STK_SIZE          not #define'd in 'app_cfg.h'"
#error  "                            [MUST be  > 0u]                   "

#elif   (CLK_OS_CFG_TASK_STK_SIZE < 1u)
#error  "CLK_OS_CFG_TASK_STK_SIZE    illegally #define'd in 'app_cfg.h'"
#error  "                            [MUST be  > 0u]                   "
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           CLOCK FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            Clk_OS_Init()
*
* Description : (1) Perform Clock/OS initialization :
*
*                   (a) Implement Clock signal by creating a counting semaphore.
*
*                       (1) Initialize Clock signal to 0 tick by setting the semaphore count to 0.
*
*                   (b) Create Clock Task
*
*
* Argument(s) : p_err       Pointer to variable that will receive the return error code from this function :
*
*                               CLK_OS_ERR_NONE                 Clock OS initialization successful.
*                               CLK_OS_ERR_INIT_SIGNAL          Clock OS signal NOT successfully initialized.
*                               CLK_OS_ERR_INIT_TASK            Clock OS task   NOT successfully initialized.
*
* Return(s)   : DEF_OK,   if NO error(s).
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  Clk_OS_Init (CLK_ERR  *p_err)
{
    OS_ERR  os_err;


#if (CLK_CFG_SIGNAL_EN == DEF_ENABLED)                      /* ----------------- CREATE CLOCK SIGNAL ------------------ */
    OSSemCreate((OS_SEM   *)&Clk_OS_SignalObj,              /* Create counting semaphore to signal each elapsed second. */
                (CPU_CHAR *) CLK_OS_SIGNAL_NAME,
                (OS_SEM_CTR) 0u,
                (OS_ERR   *)&os_err);
    if (os_err != OS_ERR_NONE) {
       *p_err   = CLK_OS_ERR_INIT_SIGNAL;
        return;
    }
#endif


                                                            /* ------------------ CREATE CLOCK TASK ------------------- */
    OSTaskCreate((OS_TCB     *)&Clk_OS_TaskTCB,
                 (CPU_CHAR   *) CLK_OS_TASK_NAME,
                 (OS_TASK_PTR ) Clk_OS_Task,
                 (void       *) 0,
                 (OS_PRIO     ) CLK_OS_CFG_TASK_PRIO,
                 (CPU_STK    *)&Clk_OS_TaskStk[0],
                 (CPU_STK_SIZE)(CLK_OS_CFG_TASK_STK_SIZE / 10u),
                 (CPU_STK_SIZE) CLK_OS_CFG_TASK_STK_SIZE,
                 (OS_MSG_QTY  ) 0u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&os_err);
    if (os_err != OS_ERR_NONE) {
       *p_err   = CLK_OS_ERR_INIT_TASK;
        return;
    }


   *p_err = CLK_OS_ERR_NONE;
}


/*
*********************************************************************************************************
*                                            Clk_OS_Task()
*
* Description : OS-dependent shell task to schedule & run Clock Task handler.
*
*               (1) Shell task's primary purpose is to schedule & run Clk_TaskHandler() forever;
*                   (i.e. shell task should NEVER exit).
*
*
* Argument(s) : p_data      Pointer to task initialization data (required by uC/OS-III).
*
* Return(s)   : none.
*
* Created by  : Clk_OS_Init().
*
*               This function is an INTERNAL Clock function & MUST NOT be called by application function(s).
*
* Note(s)     : (2) To prevent deadlocking any lower priority task(s), Clock Task SHOULD delay (for a brief)
*                   time after task handler exits.
*********************************************************************************************************
*/

static  void  Clk_OS_Task (void  *p_data)
{
    OS_ERR  os_err;


   (void)p_data;                                               /* Prevent 'variable unused' compiler warning.          */

    while (DEF_ON) {
        Clk_TaskHandler();
        OSTimeDly((OS_TICK ) 1u,                                /* Dly for lower prio task(s) [see Note #2].            */
                  (OS_OPT  ) OS_OPT_TIME_DLY,
                  (OS_ERR *)&os_err);
    }
}


/*
*********************************************************************************************************
*                                            Clk_OS_Wait()
*
* Description : Wait for Clock signal to increment Clock timestamp.
*
* Argument(s) : p_err       Pointer to variable that will receive the return error code from this function :
*
*                               CLK_OS_ERR_NONE                 Signal     received.
*                               CLK_OS_ERR_WAIT                 Signal NOT received.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  Clk_OS_Wait (CLK_ERR  *p_err)
{
    OS_ERR  os_err;


#if (CLK_CFG_SIGNAL_EN == DEF_ENABLED)
    OSSemPend((OS_SEM *)&Clk_OS_SignalObj,                      /* Wait for one-sec signal ...                          */
              (OS_TICK ) 0u,                                    /* ... without timeout.                                 */
              (OS_OPT  ) OS_OPT_PEND_BLOCKING,
              (CPU_TS *) 0,
              (OS_ERR *)&os_err);
#else
    OSTimeDlyHMSM(0u,
                  0u,
                  1u,                                           /* Dly for one sec (see Note #1).                       */
                  0u,
                  OS_OPT_TIME_PERIODIC,
                 &os_err);
#endif

    if (os_err != OS_ERR_NONE) {
       *p_err   = CLK_OS_ERR_WAIT;
        return;
    }

   *p_err = CLK_OS_ERR_NONE;
}


/*
*********************************************************************************************************
*                                           Clk_OS_Signal()
*
* Description : Signal Clock module to increment timestamp.
*
* Argument(s) : p_err       Pointer to variable that will receive the return error code from this function
*
*                               CLK_OS_ERR_NONE                 Clock module     successfully signaled.
*                               CLK_OS_ERR_SIGNAL               Clock module NOT successfully signaled.
*
* Return(s)   : none.
*
* Note(s)     : (1) Clk_OS_Signal() MUST be called once per second.
*********************************************************************************************************
*/

#if (CLK_CFG_SIGNAL_EN == DEF_ENABLED)
void  Clk_OS_Signal (CLK_ERR  *p_err)
{
    OS_ERR  os_err;


    OSSemPost(&Clk_OS_SignalObj,
               OS_OPT_POST_1,
              &os_err);
    if (os_err != OS_ERR_NONE) {
       *p_err   = CLK_OS_ERR_SIGNAL;
        return;
    }

   *p_err = CLK_OS_ERR_NONE;
}
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*
* Note(s) : (1) See 'clk.h  MODULE CONFIGURATION'.
*********************************************************************************************************
*/

#endif                                                          /* End of Clk OS module include (see Note #1).          */

