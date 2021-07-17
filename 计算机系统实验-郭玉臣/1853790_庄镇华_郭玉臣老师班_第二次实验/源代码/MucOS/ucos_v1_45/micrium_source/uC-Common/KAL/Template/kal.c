/*
*********************************************************************************************************
*                                              uC/Common
*                                 Common Features for Micrium Stacks
*
*                         (c) Copyright 2013-2019; Silicon Laboratories Inc.,
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
*                             uC/Common - Kernel Abstraction Layer (KAL)
*                                              Template
*
* Filename : kal.c
* Version  : V1.01.02
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#define  MICRIUM_SOURCE
#define  KAL_MODULE

#include  "../kal.h"

#include  <lib_def.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            LOCAL CONSTANTS
*********************************************************************************************************
*********************************************************************************************************
*/

KAL_CPP_EXT  const  CPU_INT32U           KAL_Version           =  10000u;
                                                                /* &&&& OS TICK RATE                            */
KAL_CPP_EXT  const  KAL_TICK_RATE_HZ     KAL_TickRate          =  0u;

KAL_CPP_EXT  const  KAL_TASK_HANDLE      KAL_TaskHandleNull    = {DEF_NULL};
KAL_CPP_EXT  const  KAL_LOCK_HANDLE      KAL_LockHandleNull    = {DEF_NULL};
KAL_CPP_EXT  const  KAL_SEM_HANDLE       KAL_SemHandleNull     = {DEF_NULL};
KAL_CPP_EXT  const  KAL_TMR_HANDLE       KAL_TmrHandleNull     = {DEF_NULL};
KAL_CPP_EXT  const  KAL_Q_HANDLE         KAL_QHandleNull       = {DEF_NULL};
KAL_CPP_EXT  const  KAL_TASK_REG_HANDLE  KAL_TaskRegHandleNull = {DEF_NULL};


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                              LOCAL TABLES
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                       LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                       LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         KAL CORE API FUNCTIONS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              KAL_Init()
*
* Description : Initialize the Kernel Abstraction Layer.
*
* Argument(s) : p_cfg       Pointer to KAL configuration structure.
*
*               p_err       Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : none.
*
* Note(s)     : (1) This function must be called prior to any product initialization function if the
*                   application needs to specify a memory segment.
*********************************************************************************************************
*/

void  KAL_Init (KAL_CFG   *p_cfg,
                RTOS_ERR  *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
    return;
}


/*
*********************************************************************************************************
*                                          KAL_FeatureQuery()
*
* Description : Check if specified feature is available.
*
* Argument(s) : feature     Feature to query.
*
*               opt         Option associated with the feature requested.
*
* Return(s)   : DEF_YES, if feature is available.
*
*               DEF_NO,  otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  KAL_FeatureQuery (KAL_FEATURE  feature,
                               KAL_OPT      opt)
{
    CPU_BOOLEAN  is_en;


    (void)opt;

    is_en = DEF_NO;

    switch (feature) {
        case KAL_FEATURE_TASK_CREATE:                           /* ---------------------- TASKS ----------------------- */
        case KAL_FEATURE_TASK_DEL:
             break;


        case KAL_FEATURE_LOCK_CREATE:                           /* ---------------------- LOCKS ----------------------- */
        case KAL_FEATURE_LOCK_ACQUIRE:
        case KAL_FEATURE_LOCK_RELEASE:
        case KAL_FEATURE_LOCK_DEL:
             break;


        case KAL_FEATURE_SEM_CREATE:                            /* ----------------------- SEMS ----------------------- */
        case KAL_FEATURE_SEM_DEL:
        case KAL_FEATURE_SEM_PEND:
        case KAL_FEATURE_SEM_POST:
        case KAL_FEATURE_SEM_ABORT:
        case KAL_FEATURE_SEM_SET:
             break;


        case KAL_FEATURE_Q_CREATE:                              /* ---------------------- QUEUES ---------------------- */
        case KAL_FEATURE_Q_POST:
        case KAL_FEATURE_Q_PEND:
             break;


        case KAL_FEATURE_DLY:                                   /* ----------------------- DLYS ----------------------- */
             break;


        case KAL_FEATURE_TASK_REG:                              /* --------------------- TASK REGS -------------------- */
             break;


        case KAL_FEATURE_TICK_GET:                              /* ------------------- TICK CTR INFO ------------------ */
             break;


        default:
             break;
    }

    return (is_en);
}


/*
*********************************************************************************************************
*                                         TASK API FUNCTIONS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            KAL_TaskAlloc()
*
* Description : Allocate a task object and its stack.
*
* Argument(s) : p_name          Pointer to name of the task.
*
*               p_stk_base      Pointer to start of task stack. If NULL, the stack will be allocated from
*                               the KAL memory segment.
*
*               stk_size_bytes  Size (in bytes) of the task stack.
*
*               p_cfg           Pointer to KAL task configuration structure.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : Allocated task's handle.
*
* Note(s)     : none.
*********************************************************************************************************
*/

KAL_TASK_HANDLE  KAL_TaskAlloc (const  CPU_CHAR          *p_name,
                                       void              *p_stk_base,
                                       CPU_SIZE_T         stk_size_bytes,
                                       KAL_TASK_EXT_CFG  *p_cfg,
                                       RTOS_ERR          *p_err)
{
    KAL_TASK_HANDLE  handle = KAL_TaskHandleNull;


   *p_err = RTOS_ERR_NOT_SUPPORTED;
    return (handle);
}


/*
*********************************************************************************************************
*                                           KAL_TaskCreate()
*
* Description : Create and start a task.
*
* Argument(s) : task_handle     Handle of the task to create.
*
*               p_fnct          Pointer to task function.
*
*               p_task_arg      Pointer to argument that will be passed to task function (can be DEF_NULL).
*
*               prio            Task priority.
*
*               p_cfg           Pointer to KAL task configuration structure.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
* Return(s)   : none.
*
* Note(s)     : (1) The task must be allocated prior to this call using KAL_TaskAlloc().
*********************************************************************************************************
*/

void  KAL_TaskCreate (KAL_TASK_HANDLE     task_handle,
                      void              (*p_fnct)(void  *p_arg),
                      void               *p_task_arg,
                      CPU_INT08U          prio,
                      KAL_TASK_EXT_CFG   *p_cfg,
                      RTOS_ERR           *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
}


/*
*********************************************************************************************************
*                                             KAL_TaskDel()
*
* Description : Delete a task.
*
* Argument(s) : task_handle     Handle of the task to delete.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_TaskDel (KAL_TASK_HANDLE   task_handle,
                   RTOS_ERR         *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
}


/*
*********************************************************************************************************
*                                         LOCK API FUNCTIONS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           KAL_LockCreate()
*
* Description : Create a lock, which is unlocked by default.
*
* Argument(s) : p_name          Pointer to name of the lock.
*
*               p_cfg           Pointer to KAL lock configuration structure.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : Created lock handle.
*
* Note(s)     : none.
*********************************************************************************************************
*/

KAL_LOCK_HANDLE  KAL_LockCreate (const  CPU_CHAR          *p_name,
                                        KAL_LOCK_EXT_CFG  *p_cfg,
                                        RTOS_ERR          *p_err)
{
    KAL_LOCK_HANDLE  handle = KAL_LockHandleNull;


   *p_err = RTOS_ERR_NOT_SUPPORTED;
    return (handle);
}


/*
*********************************************************************************************************
*                                           KAL_LockAcquire()
*
* Description : Acquire a lock.
*
* Argument(s) : lock_handle     Handle of the lock to acquire.
*
*               opt             Options available:
*                                   KAL_OPT_PEND_NONE:          block until timeout expires or lock is available.
*                                   KAL_OPT_PEND_BLOCKING:      block until timeout expires or lock is available.
*                                   KAL_OPT_PEND_NON_BLOCKING:  return immediately even if lock is not available.
*
*               timeout_ms      Timeout, in milliseconds. A value of 0 will never timeout.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_LockAcquire (KAL_LOCK_HANDLE   lock_handle,
                       KAL_OPT           opt,
                       CPU_INT32U        timeout_ms,
                       RTOS_ERR         *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
}


/*
*********************************************************************************************************
*                                           KAL_LockRelease()
*
* Description : Release a lock.
*
* Argument(s) : lock_handle     Handle of the lock to release.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_LockRelease (KAL_LOCK_HANDLE   lock_handle,
                       RTOS_ERR         *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
}


/*
*********************************************************************************************************
*                                             KAL_LockDel()
*
* Description : Delete a lock.
*
* Argument(s) : lock_handle     Handle of the lock to delete.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_LockDel (KAL_LOCK_HANDLE   lock_handle,
                   RTOS_ERR         *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
}


/*
*********************************************************************************************************
*                                          SEM API FUNCTIONS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            KAL_SemCreate()
*
* Description : Create a semaphore, with a count of 0.
*
* Argument(s) : p_name          Pointer to name of the semaphore.
*
*               p_cfg           Pointer to KAL semaphore configuration structure.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : Created semaphore's handle.
*
* Note(s)     : none.
*********************************************************************************************************
*/

KAL_SEM_HANDLE  KAL_SemCreate (const  CPU_CHAR         *p_name,
                                      KAL_SEM_EXT_CFG  *p_cfg,
                                      RTOS_ERR         *p_err)
{
    KAL_SEM_HANDLE  handle = KAL_SemHandleNull;


   *p_err = RTOS_ERR_NOT_SUPPORTED;
    return (handle);
}


/*
*********************************************************************************************************
*                                             KAL_SemPend()
*
* Description : Pend on a semaphore.
*
* Argument(s) : sem_handle      Handle of the semaphore to pend on.
*
*               opt             Options available:
*                                   KAL_OPT_PEND_NONE:          block until timeout expires or semaphore is available.
*                                   KAL_OPT_PEND_BLOCKING:      block until timeout expires or semaphore is available.
*                                   KAL_OPT_PEND_NON_BLOCKING:  return immediately even if semaphore is not available.
*
*               timeout_ms      Timeout, in milliseconds. A value of 0 will never timeout.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_SemPend (KAL_SEM_HANDLE   sem_handle,
                   KAL_OPT          opt,
                   CPU_INT32U       timeout_ms,
                   RTOS_ERR        *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
}


/*
*********************************************************************************************************
*                                             KAL_SemPost()
*
* Description : Post a semaphore.
*
* Argument(s) : sem_handle      Handle of the semaphore to post.
*
*               opt             Options available:
*                                   KAL_OPT_POST_NONE:     wake only the highest priority task pending on semaphore.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_SemPost (KAL_SEM_HANDLE   sem_handle,
                   KAL_OPT          opt,
                   RTOS_ERR        *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
}


/*
*********************************************************************************************************
*                                          KAL_SemPendAbort()
*
* Description : Abort given semaphore and resume all the tasks pending on it.
*
* Argument(s) : sem_handle      Handle of the sempahore to abort.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_SemPendAbort (KAL_SEM_HANDLE   sem_handle,
                        RTOS_ERR        *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
}


/*
*********************************************************************************************************
*                                             KAL_SemSet()
*
* Description : Set value of semaphore.
*
* Argument(s) : sem_handle      Handle of the semaphore to set.
*
*               cnt             Count value to set semaphore.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_SemSet (KAL_SEM_HANDLE   sem_handle,
                  CPU_INT16U       cnt,
                  RTOS_ERR        *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
}


/*
*********************************************************************************************************
*                                             KAL_SemDel()
*
* Description : Delete a semaphore.
*
* Argument(s) : sem_handle      Handle of the semaphore to delete.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_SemDel (KAL_SEM_HANDLE   sem_handle,
                  RTOS_ERR        *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
}


/*
*********************************************************************************************************
*                                          TMR API FUNCTIONS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            KAL_TmrCreate()
*
* Description : Create a single-shot timer.
*
* Argument(s) : p_name          Pointer to name of the timer.
*
*               p_callback      Pointer to the callback function that will be called on completion of timer.
*
*               p_callback_arg  Argument passed to callback function.
*
*               interval_ms     If timer is 'one-shot', delay  used by the timer, in milliseconds.
*                               If timer is 'periodic', period used by the timer, in milliseconds.
*
*               p_cfg           Pointer to KAL timer configuration structure.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : Created timer handle.
*
* Note(s)     : none.
*********************************************************************************************************
*/

KAL_TMR_HANDLE  KAL_TmrCreate (const  CPU_CHAR   *p_name,
                               void             (*p_callback)(void  *p_arg),
                               void              *p_callback_arg,
                               CPU_INT32U         interval_ms,
                               KAL_TMR_EXT_CFG   *p_cfg,
                               RTOS_ERR          *p_err)
{
    KAL_TMR_HANDLE  handle = KAL_TmrHandleNull;


   *p_err = RTOS_ERR_NOT_SUPPORTED;
    return (handle);
}


/*
*********************************************************************************************************
*                                            KAL_TmrStart()
*
* Description : Start timer.
*
* Argument(s) : tmr_handle      Handle of timer to start.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_TmrStart (KAL_TMR_HANDLE   tmr_handle,
                    RTOS_ERR        *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
}


/*
*********************************************************************************************************
*                                           Q API FUNCTIONS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             KAL_QCreate()
*
* Description : Create an empty queue.
*
* Argument(s) : p_name          Pointer to name of the queue.
*
*               max_msg_qty     Maximum number of message contained in the queue.
*
*               p_cfg           Pointer to KAL queue configuration structure.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : Created queue handle.
*
* Note(s)     : none.
*********************************************************************************************************
*/

KAL_Q_HANDLE  KAL_QCreate (const  CPU_CHAR       *p_name,
                                  KAL_MSG_QTY     max_msg_qty,
                                  KAL_Q_EXT_CFG  *p_cfg,
                                  RTOS_ERR       *p_err)
{
    KAL_Q_HANDLE  handle = KAL_QHandleNull;


   *p_err = RTOS_ERR_NOT_SUPPORTED;
    return (handle);
}


/*
*********************************************************************************************************
*                                              KAL_QPend()
*
* Description : Pend/get first message of queue.
*
* Argument(s) : q_handle        Handle of the queue to pend on.
*
*               opt             Options available:
*                                   KAL_OPT_PEND_NONE:          block until timeout expires or message is available.
*                                   KAL_OPT_PEND_BLOCKING:      block until timeout expires or message is available.
*                                   KAL_OPT_PEND_NON_BLOCKING:  return immediately with or without message.
*
*               timeout_ms      Timeout, in milliseconds. A value of 0 will never timeout.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : Pointer to message obtained, if any, if no error.
*
*               Null pointer,                        otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  *KAL_QPend (KAL_Q_HANDLE   q_handle,
                  KAL_OPT        opt,
                  CPU_INT32U     timeout_ms,
                  RTOS_ERR      *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
    return (DEF_NULL);
}


/*
*********************************************************************************************************
*                                              KAL_QPost()
*
* Description : Post message on queue.
*
* Argument(s) : q_handle        Handle of the queue on which to post message.
*
*               p_msg           Pointer to message to post.
*
*               opt             Options available:
*                                   KAL_OPT_POST_NONE:     wake only the highest priority task pending on queue.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_QPost (KAL_Q_HANDLE   q_handle,
                 void          *p_msg,
                 KAL_OPT        opt,
                 RTOS_ERR      *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
}


/*
*********************************************************************************************************
*                                          DLY API FUNCTIONS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               KAL_Dly()
*
* Description : Delay current task (in milliseconds).
*
* Argument(s) : dly_ms          Delay value, in milliseconds.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_Dly (CPU_INT32U  dly_ms)
{

}


/*
*********************************************************************************************************
*                                             KAL_DlyTick()
*
* Description : Delay current task (in ticks).
*
* Argument(s) : dly_ticks       Delay value, in ticks.
*
*               opt             Options available:
*                                   KAL_OPT_DLY_NONE:       apply a 'normal' delay.
*                                   KAL_OPT_DLY:            apply a 'normal' delay.
*                                   KAL_OPT_DLY_PERIODIC:   apply a periodic delay.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_DlyTick (KAL_TICK  dly_ticks,
                   KAL_OPT   opt)
{

}


/*
*********************************************************************************************************
*                                       TASK REG API FUNCTIONS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          KAL_TaskRegCreate()
*
* Description : Create a task register.
*
* Argument(s) : p_cfg           Pointer to KAL task register configuration structure.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : Created task register's handle.
*
* Note(s)     : none.
*********************************************************************************************************
*/

KAL_TASK_REG_HANDLE  KAL_TaskRegCreate (KAL_TASK_REG_EXT_CFG  *p_cfg,
                                        RTOS_ERR              *p_err)
{
    KAL_TASK_REG_HANDLE  handle = KAL_TaskRegHandleNull;


   *p_err = RTOS_ERR_NOT_SUPPORTED;
    return (handle);
}


/*
*********************************************************************************************************
*                                             KAL_TickGet()
*
* Description : Get value of OS' tick counter.
*
* Argument(s) : p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : OS tick counter's value.
*
* Note(s)     : none.
*********************************************************************************************************
*/

KAL_TICK  KAL_TickGet (RTOS_ERR *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
    return (0u);
}


/*
*********************************************************************************************************
*                                           KAL_TaskRegGet()
*
* Description : Get value from a task register.
*
* Argument(s) : task_handle     Handle of the task associated to the task register. Current task is used if NULL.
*
*               task_reg_handle Handle of the task register to read.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : Value read from the task register, if no error.
*               0,                                 otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  KAL_TaskRegGet (KAL_TASK_HANDLE       task_handle,
                            KAL_TASK_REG_HANDLE   task_reg_handle,
                            RTOS_ERR             *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
    return (0u);
}


/*
*********************************************************************************************************
*                                           KAL_TaskRegSet()
*
* Description : Set value of task register.
*
* Argument(s) : task_handle     Handle of the task associated to the task register. Current task is used if NULL.
*
*               task_reg_handle Handle of the task register to write to.
*
*               val             Value to write in the task register.
*
*               p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_SUPPORTED      Function not implemented.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_TaskRegSet (KAL_TASK_HANDLE       task_handle,
                      KAL_TASK_REG_HANDLE   task_reg_handle,
                      CPU_INT32U            val,
                      RTOS_ERR             *p_err)
{
   *p_err = RTOS_ERR_NOT_SUPPORTED;
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
