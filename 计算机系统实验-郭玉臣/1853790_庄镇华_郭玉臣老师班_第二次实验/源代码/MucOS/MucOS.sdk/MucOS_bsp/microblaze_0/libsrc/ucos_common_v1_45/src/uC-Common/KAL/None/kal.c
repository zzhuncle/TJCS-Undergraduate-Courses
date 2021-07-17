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
*                                         No Operating System
*
* Filename : kal.c
* Version  : V1.01.02
*********************************************************************************************************
* Note(s)  : (1) Since no operating system is present, most of the present features are implemented
*                directly in the KAL.
*
*            (2) Since no operating system is present, active wait is used and may considerably
*                reduce performance.
*
*            (3) Since no operating system is present, it is assumed that there is no multi-tasking.
*                Therefore, the locking mechanism can be reduced to nothing, since the execution
*                flow can only be pre-empted by an interrupt and it is impossible to pend on a lock
*                in an interrupt context.
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
#include  <lib_mem.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#define  KAL_CFG_ARG_CHK_EXT_EN                 DEF_ENABLED


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            LOCAL CONSTANTS
*********************************************************************************************************
*********************************************************************************************************
*/

KAL_CPP_EXT  const  CPU_INT32U           KAL_Version           =  10000u;
KAL_CPP_EXT  const  KAL_TICK_RATE_HZ     KAL_TickRate          =  0u;

KAL_CPP_EXT  const  KAL_TASK_HANDLE      KAL_TaskHandleNull    = {DEF_NULL};
KAL_CPP_EXT  const  KAL_LOCK_HANDLE      KAL_LockHandleNull    = {DEF_NULL};
KAL_CPP_EXT  const  KAL_SEM_HANDLE       KAL_SemHandleNull     = {DEF_NULL};
KAL_CPP_EXT  const  KAL_TMR_HANDLE       KAL_TmrHandleNull     = {DEF_NULL};
KAL_CPP_EXT  const  KAL_Q_HANDLE         KAL_QHandleNull       = {DEF_NULL};
KAL_CPP_EXT  const  KAL_TASK_REG_HANDLE  KAL_TaskRegHandleNull = {DEF_NULL};

             const  KAL_LOCK_HANDLE      KAL_LockDfltHandle    = {(void *)1u};


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/

                                                                /* ------------------- KAL SEM TYPE ------------------- */
typedef  struct  kal_sem {
    volatile  CPU_INT32U  Cnt;                                  /* Cnt used by sem.                                     */
} KAL_SEM;


                                                                /* ----------------- KAL INTERNAL DATA ---------------- */
typedef  struct  kal_data {
    MEM_SEG       *MemSegPtr;                                   /* Mem Seg to alloc from.                               */
    MEM_DYN_POOL   SemPool;                                     /* Pool for sems.                                       */
} KAL_DATA;


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

static  KAL_DATA  *KAL_DataPtr = DEF_NULL;


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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_ALREADY_INIT       Already initialized and p_cfg is not NULL.
*                                   RTOS_ERR_ALLOC              Memory segment allocation failed.
*                                   RTOS_ERR_INIT               Memory pool initialization failed.
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
    MEM_SEG  *p_seg;
    LIB_ERR   err_lib;
    CPU_SR_ALLOC();


    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }
    #endif

    CPU_CRITICAL_ENTER();
    if (KAL_DataPtr != DEF_NULL) {                              /* Chk if KAL already init. See note #1.                */
        CPU_CRITICAL_EXIT();
        if (p_cfg == DEF_NULL) {
           *p_err = RTOS_ERR_NONE;                              /* KAL_Init() can be called many times if no cfg.       */
        } else {
           *p_err = RTOS_ERR_ALREADY_INIT;                      /* If a cfg is given and KAL is already init, set err.  */
        }
        return;
    }

    p_seg = DEF_NULL;
    if (p_cfg != DEF_NULL) {                                    /* Load cfg if given.                                   */
        p_seg = p_cfg->MemSegPtr;
    }

    KAL_DataPtr = (KAL_DATA *)Mem_SegAlloc("KAL internal data",
                                            p_seg,
                                            sizeof(KAL_DATA),
                                           &err_lib);
    CPU_CRITICAL_EXIT();
    if (err_lib != LIB_MEM_ERR_NONE) {
       *p_err = RTOS_ERR_ALLOC;
        return;
    }

    KAL_DataPtr->MemSegPtr = p_seg;

    Mem_DynPoolCreate("KAL sem pool",
                      &KAL_DataPtr->SemPool,
                       KAL_DataPtr->MemSegPtr,
                       sizeof(KAL_SEM),
                       sizeof(CPU_ALIGN),
                       0u,
                       LIB_MEM_BLK_QTY_UNLIMITED,
                      &err_lib);
    if (err_lib != LIB_MEM_ERR_NONE) {
       *p_err = RTOS_ERR_INIT;
        return;
    }

   *p_err = RTOS_ERR_NONE;

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
             is_en = DEF_YES;
             break;


        case KAL_FEATURE_SEM_CREATE:                            /* ----------------------- SEMS ----------------------- */
        case KAL_FEATURE_SEM_DEL:
             is_en = DEF_YES;
             break;


        case KAL_FEATURE_SEM_PEND:
             if (DEF_BIT_IS_SET(opt, KAL_OPT_PEND_NON_BLOCKING) == DEF_YES) {
                 is_en = DEF_YES;
             } else {
                 #if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
                     is_en = DEF_YES;
                 #endif
             }
             break;


        case KAL_FEATURE_SEM_POST:
             is_en = DEF_YES;
             break;


        case KAL_FEATURE_SEM_ABORT:
        case KAL_FEATURE_SEM_SET:
             break;


        case KAL_FEATURE_Q_CREATE:                              /* ---------------------- QUEUES ---------------------- */
        case KAL_FEATURE_Q_POST:
        case KAL_FEATURE_Q_PEND:
             break;


        case KAL_FEATURE_DLY:                                   /* ----------------------- DLYS ----------------------- */
             #if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
                 is_en = DEF_YES;
             #endif
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
*                                   RTOS_ERR_NOT_AVAIL          Feature not available without an OS.
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


                                                                /* Task creation is not avail.                          */
    (void)p_name;
    (void)p_stk_base;
    (void)stk_size_bytes;
    (void)p_cfg;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(handle);
        }
    #endif

   *p_err = RTOS_ERR_NOT_AVAIL;

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
*                                   RTOS_ERR_NOT_AVAIL          Feature not available without an OS.
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
                                                                /* Task creation is not avail.                          */
    (void)task_handle;
    (void)(*p_fnct);
    (void)p_task_arg;
    (void)prio;
    (void)p_cfg;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }
    #endif

   *p_err = RTOS_ERR_NOT_AVAIL;

    return;
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
*                                   RTOS_ERR_NOT_AVAIL          Feature not available without an OS.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_TaskDel (KAL_TASK_HANDLE   task_handle,
                   RTOS_ERR         *p_err)
{
                                                                /* Task del is not avail.                               */
    (void)task_handle;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }
    #endif

   *p_err = RTOS_ERR_NOT_AVAIL;

    return;
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
*                                   RTOS_ERR_NONE               Function always successful.
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


    (void)p_name;
    (void)p_cfg;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(handle);
        }
    #endif

   *p_err = RTOS_ERR_NONE;

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
*                                   RTOS_ERR_NONE               Function always successful.
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
    (void)lock_handle;
    (void)opt;
    (void)timeout_ms;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }
    #endif

   *p_err = RTOS_ERR_NONE;

    return;
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
*                                   RTOS_ERR_NONE               Function always successful.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_LockRelease (KAL_LOCK_HANDLE   lock_handle,
                       RTOS_ERR         *p_err)
{
    (void)lock_handle;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }
    #endif

   *p_err = RTOS_ERR_NONE;

    return;
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
*                                   RTOS_ERR_NONE               Function always successful.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_LockDel (KAL_LOCK_HANDLE   lock_handle,
                   RTOS_ERR         *p_err)
{
    (void)lock_handle;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }
    #endif

   *p_err = RTOS_ERR_NONE;

    return;
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_SUPPORTED      'p_cfg' parameter was not NULL.
*                                   RTOS_ERR_ALLOC              Unable to allocate memory for semaphore.
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
    KAL_SEM_HANDLE   handle = KAL_SemHandleNull;
    KAL_SEM         *p_sem;
    LIB_ERR          err_lib;


    (void)p_name;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(handle);
        }

        if (p_cfg != DEF_NULL) {                                /* Make sure no unsupported cfg recv.                   */
           *p_err = RTOS_ERR_NOT_SUPPORTED;
            return (handle);
        }
    #else
        (void)p_cfg;
    #endif

    p_sem = (KAL_SEM *)Mem_DynPoolBlkGet(&KAL_DataPtr->SemPool,
                                         &err_lib);
    if (err_lib != LIB_MEM_ERR_NONE) {
       *p_err = RTOS_ERR_ALLOC;
        return (handle);
    }

    p_sem->Cnt       =  0u;                                     /* Created sem is unvavail at creation.                 */
    handle.SemObjPtr = (void *)p_sem;
   *p_err            =  RTOS_ERR_NONE;

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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NULL_PTR           Handle contains a NULL/invalid pointer.
*                                   RTOS_ERR_INVALID_ARG        Handle or options specified is invalid.
*                                   RTOS_ERR_WOULD_BLOCK        KAL_OPT_PEND_NON_BLOCKING opt specified and
*                                                               semaphore is not available.
*                                   RTOS_ERR_TIMEOUT            Operation timed-out.
*                                   RTOS_ERR_OS                 Generic OS error.
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
    KAL_SEM    *p_sem;
    CPU_REG32   sem_cnt = 0u;
    CPU_SR_ALLOC();


    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }

        if (KAL_SEM_HANDLE_IS_NULL(sem_handle) == DEF_YES) {
           *p_err = RTOS_ERR_NULL_PTR;
            return;
        }

        if (DEF_BIT_IS_SET_ANY(opt, ~(KAL_OPT_PEND_NONE | KAL_OPT_PEND_NON_BLOCKING)) == DEF_YES) {
           *p_err = RTOS_ERR_INVALID_ARG;
            return;
        }

        if (dly_ts > (DEF_GET_U_MAX_VAL(CPU_TS_TMR) - 1u)) {
           *p_err = RTOS_ERR_INVALID_ARG;                       /* Dly too long for size of tmr used.                   */
            return;
        }
    #endif

    p_sem = (KAL_SEM *)sem_handle.SemObjPtr;

    if (DEF_BIT_IS_CLR(opt, KAL_OPT_PEND_NON_BLOCKING) == DEF_YES) {
                                                                /* Blocking call.                                       */
        if (timeout_ms == 0u) {
            while (sem_cnt == 0u) {
                CPU_CRITICAL_ENTER();
                sem_cnt = p_sem->Cnt;
                if (sem_cnt != 0u) {
                   p_sem->Cnt--;                                /* Dec sem cnt.                                         */
                }
                CPU_CRITICAL_EXIT();
            }
           *p_err = RTOS_ERR_NONE;
        } else {
            #if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
            {
                CPU_TS_TMR       ts;
                CPU_TS_TMR       ts_prev;
                CPU_INT32U       ts_delta;
                CPU_INT32U       timeout_ts;
                CPU_TS_TMR_FREQ  tmr_freq;
                CPU_INT08U       tmr_size = sizeof(CPU_TS_TMR);
                CPU_ERR          err_cpu;


                tmr_freq = CPU_TS_TmrFreqGet(&err_cpu);
                if (err_cpu != CPU_ERR_NONE) {
                   *p_err = RTOS_ERR_OS;
                    return;
                }

                timeout_ts = timeout_ms * (tmr_freq / 1000u);   /* Calc timeout ts val, depending on tmr freq.          */
                ts_delta   = 0u;
                ts_prev    = CPU_TS_TmrRd();

                while (ts_delta < timeout_ts) {                 /* Loop until sem is avail or timeout expires.          */
                    CPU_CRITICAL_ENTER();
                    sem_cnt = p_sem->Cnt;
                    if (sem_cnt != 0u) {
                        p_sem->Cnt--;                           /* Dec sem cnt.                                         */
                        CPU_CRITICAL_EXIT();
                       *p_err = RTOS_ERR_NONE;
                        return;
                    }
                    CPU_CRITICAL_EXIT();

                    ts = CPU_TS_TmrRd();
                    if (ts >= ts_prev) {                        /* Update ts_delta val.                                 */
                        ts_delta += ts - ts_prev;
                    } else {                                    /* Wraparound.                                          */
                        ts_delta += DEF_GET_U_MAX_VAL(CPU_TS_TMR) - ts_prev + ts;
                    }
                    ts_prev = ts;
                }
               *p_err = RTOS_ERR_TIMEOUT;
            }
            #else
               *p_err = RTOS_ERR_NOT_AVAIL;
            #endif
        }
    } else {                                                    /* Non-blocking call.                                   */
        CPU_CRITICAL_ENTER();
        if (p_sem->Cnt != 0u) {
            p_sem->Cnt--;
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = RTOS_ERR_WOULD_BLOCK;
        }
        CPU_CRITICAL_EXIT();
    }

    return;
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NULL_PTR           Handle contains a NULL/invalid pointer.
*                                   RTOS_ERR_INVALID_ARG        Handle or options specified is invalid.
*                                   RTOS_ERR_WOULD_OVF          Semaphore would overflow.
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
    KAL_SEM  *p_sem;
    CPU_SR_ALLOC();


    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }

        if (KAL_SEM_HANDLE_IS_NULL(sem_handle) == DEF_YES) {
           *p_err = RTOS_ERR_NULL_PTR;
            return;
        }

        if (opt != KAL_OPT_POST_NONE) {
           *p_err = RTOS_ERR_INVALID_ARG;
            return;
        }
    #endif

    p_sem = (KAL_SEM *)sem_handle.SemObjPtr;

    CPU_CRITICAL_ENTER();
    p_sem->Cnt++;
    if (p_sem->Cnt != 0u) {
       *p_err = RTOS_ERR_NONE;
    } else {
       *p_err = RTOS_ERR_WOULD_OVF;
        p_sem->Cnt--;
    }
    CPU_CRITICAL_EXIT();

    return;
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
*                                   RTOS_ERR_NOT_SUPPORTED      Feature not implemented.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_SemPendAbort (KAL_SEM_HANDLE   sem_handle,
                        RTOS_ERR        *p_err)
{
    (void)sem_handle;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }
    #endif

   *p_err = RTOS_ERR_NOT_SUPPORTED;

    return;
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
*                                   RTOS_ERR_NOT_SUPPORTED      Feature not implemented.
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
    (void)sem_handle;
    (void)cnt;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }
    #endif

   *p_err = RTOS_ERR_NOT_SUPPORTED;

    return;
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NULL_PTR           Handle contains a NULL/invalid pointer.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_SemDel (KAL_SEM_HANDLE   sem_handle,
                  RTOS_ERR        *p_err)
{
    KAL_SEM  *p_sem;
    LIB_ERR   err_lib;


    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }

        if (KAL_SEM_HANDLE_IS_NULL(sem_handle) == DEF_YES) {
           *p_err = RTOS_ERR_NULL_PTR;
            return;
        }
    #endif

    p_sem = (KAL_SEM *)sem_handle.SemObjPtr;

    Mem_DynPoolBlkFree(       &KAL_DataPtr->SemPool,
                       (void *)p_sem,
                              &err_lib);
    (void)err_lib;

   *p_err = RTOS_ERR_NONE;

    return;
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
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
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


    (void)p_name;
    (void)p_callback;
    (void)p_callback_arg;
    (void)interval_ms;
    (void)p_cfg;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(handle);
        }
    #endif

   *p_err = RTOS_ERR_NOT_AVAIL;

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
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_TmrStart (KAL_TMR_HANDLE   tmr_handle,
                    RTOS_ERR        *p_err)
{
    (void)tmr_handle;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }
    #endif

   *p_err = RTOS_ERR_NOT_AVAIL;

    return;
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
*                                   RTOS_ERR_NOT_AVAIL          Feature not available without an OS.
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


                                                                /* Qs are not avail.                                    */
    (void)p_name;
    (void)max_msg_qty;
    (void)p_cfg;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(handle);
        }
    #endif

   *p_err = RTOS_ERR_NOT_AVAIL;

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
*                                   RTOS_ERR_NOT_AVAIL          Feature not available without an OS.
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
                                                                /* Qs are not avail.                                    */
    (void)q_handle;
    (void)opt;
    (void)timeout_ms;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(DEF_NULL);
        }
    #endif

   *p_err = RTOS_ERR_NOT_AVAIL;

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
*                                   RTOS_ERR_NOT_AVAIL          Feature not available without an OS.
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
                                                                /* Qs are not avail.                                    */
    (void)q_handle;
    (void)p_msg;
    (void)opt;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }
    #endif

   *p_err = RTOS_ERR_NOT_AVAIL;

    return;
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
* Note(s)     : (1) This function is only available if time stamp is enabled. Otherwise, without an OS,
*                   there is no other available way to measure time.
*********************************************************************************************************
*/

void  KAL_Dly (CPU_INT32U  dly_ms)
{
    #if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
        CPU_TS_TMR       ts;
        CPU_TS_TMR       ts_prev;
        CPU_INT32U       ts_delta;
        CPU_INT32U       dly_ts;
        CPU_TS_TMR_FREQ  tmr_freq;
        CPU_INT08U       tmr_size = sizeof(CPU_TS_TMR);
        CPU_ERR          err_cpu;


        #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)             /* ---------------- VALIDATE ARGUMENTS ---------------- */
            if (dly_ts > (DEF_GET_U_MAX_VAL(CPU_TS_TMR) - 1u)) {
                CPU_SW_EXCEPTION(;);                            /* Fail call. Dly too long for size of tmr used.        */
            }
        #endif

        tmr_freq = CPU_TS_TmrFreqGet(&err_cpu);
        if (err_cpu != CPU_ERR_NONE) {
            CPU_SW_EXCEPTION(;);                                /* Fail call.                                           */
        }

        dly_ts   = dly_ms * (tmr_freq / 1000u);                 /* Calc dly ts val, depending on tmr freq.              */
        ts_delta = 0u;
        ts_prev  = CPU_TS_TmrRd();

        while (ts_delta < dly_ts) {                             /* Loop until dly expires.                              */
            ts = CPU_TS_TmrRd();
            if (ts > ts_prev) {                                 /* Update ts_delta val.                                 */
                ts_delta += ts - ts_prev;
            } else {                                            /* Wraparound.                                          */
                ts_delta += DEF_GET_U_MAX_VAL(CPU_TS_TMR) - ts_prev + ts;
            }
            ts_prev = ts;
        }

        return;
    #else
        (void)dly_ms;

        CPU_SW_EXCEPTION(;);                                    /* Fail call. See note #1.                              */
    #endif
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
* Note(s)     : (1) This function can only be called if an OS is present, since the notion of 'tick' is
*                   not present if no OS is available.
*********************************************************************************************************
*/

void  KAL_DlyTick (KAL_TICK  dly_ticks,
                   KAL_OPT   opt)
{
    (void)dly_ticks;
    (void)opt;

    CPU_SW_EXCEPTION(;);                                        /* Fail call. See note #1.                              */
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
*                                   RTOS_ERR_NOT_AVAIL          Feature not available without an OS.
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


    (void)p_cfg;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(handle);
        }
    #endif

   *p_err = RTOS_ERR_NOT_AVAIL;

    return (handle);
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
*                                   RTOS_ERR_NOT_AVAIL          Feature not available without an OS.
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
                                                                /* Task reg is not avail.                               */
    (void)task_handle;
    (void)task_reg_handle;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(0u);
        }
    #endif

   *p_err = RTOS_ERR_NOT_AVAIL;

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
*                                   RTOS_ERR_NOT_AVAIL          Feature not available without an OS.
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
                                                                /* Task reg is not avail.                               */
    (void)task_handle;
    (void)task_reg_handle;
    (void)val;

    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }
    #endif

   *p_err = RTOS_ERR_NOT_AVAIL;

    return;
}


/*
*********************************************************************************************************
*                                             KAL_TickGet()
*
* Description : Get value of OS' tick counter.
*
* Argument(s) : p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NOT_AVAIL          Feature not available without an OS.
*
* Return(s)   : OS tick counter's value.
*
* Note(s)     : none.
*********************************************************************************************************
*/

KAL_TICK  KAL_TickGet (RTOS_ERR  *p_err)
{
    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(0u);
        }
    #endif

   *p_err = RTOS_ERR_NOT_AVAIL;

    return (0u);
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

