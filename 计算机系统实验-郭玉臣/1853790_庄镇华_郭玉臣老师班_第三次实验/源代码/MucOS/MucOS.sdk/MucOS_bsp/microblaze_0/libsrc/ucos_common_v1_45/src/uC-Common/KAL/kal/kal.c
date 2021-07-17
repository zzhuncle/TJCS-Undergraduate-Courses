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
*                                              uCOS-III
*
* Filename : kal.c
* Version  : V1.01.02
*********************************************************************************************************
* Note(s)  : (1) 'goto' statements were used in this software module. Their usage
*                is restricted to cleanup purposes in exceptional program flow (e.g.
*                error handling), in compliance with CERT MEM12-C and MISRA C:2012
*                rules 15.2, 15.3 and 15.4.
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
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <Source/os.h>
#include  <os_cfg.h>
#include  <os_cfg_app.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#define  KAL_CFG_ARG_CHK_EXT_EN                 OS_CFG_ARG_CHK_EN

#define  KAL_CFG_TASK_STK_SIZE_PCT_FULL       90u

#define  KAL_INIT_STATUS_NONE                   0u
#define  KAL_INIT_STATUS_OK                     1u
#define  KAL_INIT_STATUS_FAIL                   2u


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            LOCAL CONSTANTS
*********************************************************************************************************
*********************************************************************************************************
*/

KAL_CPP_EXT  const  CPU_INT32U           KAL_Version           =  10000u;
KAL_CPP_EXT  const  KAL_TICK_RATE_HZ     KAL_TickRate          =  OS_CFG_TICK_RATE_HZ;

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

                                                                /* ------------------ KAL TASK TYPE ------------------- */
typedef  struct  kal_task {
           OS_TCB       TCB;                                    /* TCB for OS-III.                                      */

           CPU_STK     *StkBasePtr;                             /* Task stack base ptr.                                 */
           CPU_INT32U   StkSizeBytes;                           /* Task stack size (in bytes).                          */

    const  CPU_CHAR    *NamePtr;                                /* Task name string.                                    */
} KAL_TASK;

                                                                /* ------------------- KAL LOCK TYPE ------------------ */
#if (OS_CFG_MUTEX_EN == DEF_ENABLED)
typedef  struct  kal_lock {
    OS_MUTEX    Mutex;                                          /* OS-III mutex obj.                                    */
    CPU_INT08U  OptFlags;                                       /* Opt flags associated with this lock.                 */
} KAL_LOCK;
#endif

                                                                /* ------------------- KAL TMR TYPE ------------------- */
#if (OS_CFG_TMR_EN == DEF_ENABLED)
typedef  struct  kal_tmr {
    OS_TMR    Tmr;                                              /* OS-III tmr obj.                                      */

    void    (*CallbackFnct)(void  *p_arg);                      /* Tmr registered callback fnct.                        */
    void     *CallbackArg;                                      /* Arg to pass to callback fnct.                        */
} KAL_TMR;
#endif

                                                                /* ---------------- KAL TASK REG TYPE ----------------- */
#if (OS_CFG_TASK_REG_TBL_SIZE > 0u)
typedef  struct  kal_task_reg {
    OS_REG_ID  Id;                                              /* Id of the task reg.                                  */
} KAL_TASK_REG;
#endif

                                                                /* -------------- KAL INTERNAL DATA TYPE -------------- */
typedef  struct  kal_data {
    MEM_SEG       *MemSegPtr;                                   /* Mem Seg to alloc from.                               */

#if (OS_CFG_MUTEX_EN == DEF_ENABLED)
    MEM_DYN_POOL   MutexPool;                                   /* Dyn mem pool used to alloc mutex.                    */
#endif

#if (OS_CFG_SEM_EN == DEF_ENABLED)
    MEM_DYN_POOL   SemPool;                                     /* Dyn mem pool used to alloc sems.                     */
#endif

#if (OS_CFG_TMR_EN == DEF_ENABLED)
    MEM_DYN_POOL   TmrPool;                                     /* Dyn mem pool used to alloc tmrs.                     */
#endif

#if (OS_CFG_TASK_REG_TBL_SIZE > 0u)
    MEM_DYN_POOL   TaskRegPool;                                 /* Dyn mem pool used to alloc task regs.                */
#endif
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

static            KAL_DATA     *KAL_DataPtr    = DEF_NULL;

static            CPU_BOOLEAN   KAL_IsInit     = DEF_NO;
static  volatile  CPU_INT08U    KAL_InitStatus = KAL_INIT_STATUS_NONE;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                       LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

#if (OS_CFG_TMR_EN == DEF_ENABLED)
static  void      KAL_TmrFnctWrapper(void        *p_tmr_os,
                                     void        *p_arg);
#endif

static  KAL_TICK  KAL_msToTicks     (CPU_INT32U   ms);

static  RTOS_ERR  KAL_ErrConvert    (OS_ERR       err_os);


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
*                                   RTOS_ERR_OS                 Concurrent initialization failed.
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
    OS_ERR    err_os;
    LIB_ERR   err_lib;
    CPU_SR_ALLOC();


    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }
    #endif

    CPU_CRITICAL_ENTER();
    if (KAL_IsInit == DEF_YES) {                                /* Chk if KAL already init. See note #1.                */
        CPU_INT08U  init_status = KAL_InitStatus;


        CPU_CRITICAL_EXIT();

        while (init_status == KAL_INIT_STATUS_NONE) {           /* Wait until init is done before returning.            */
            OSTimeDly(100u,
                      OS_OPT_TIME_DLY,
                     &err_os);
            (void)err_os;
            CPU_CRITICAL_ENTER();
            init_status = KAL_InitStatus;
            CPU_CRITICAL_EXIT();
        }
        if (init_status == KAL_INIT_STATUS_OK) {                /* Check resulting init status.                         */
            if (p_cfg == DEF_NULL) {
               *p_err = RTOS_ERR_NONE;                          /* KAL_Init() can be called many times if no cfg.       */
            } else {
               *p_err = RTOS_ERR_ALREADY_INIT;                  /* If a cfg is given and KAL is already init, set err.  */
            }
        } else {
           *p_err = RTOS_ERR_OS;                                /* Concurrent init failed.                              */
        }
        return;
    }

    KAL_IsInit = DEF_YES;                                       /* Prevent another init, even in case of err.           */
    CPU_CRITICAL_EXIT();

    p_seg = DEF_NULL;
    if (p_cfg != DEF_NULL) {                                    /* Load cfg if given.                                   */
        p_seg = p_cfg->MemSegPtr;
    }

    KAL_DataPtr = (KAL_DATA *)Mem_SegAlloc("KAL internal data",
                                            p_seg,
                                            sizeof(KAL_DATA),
                                           &err_lib);
    if (err_lib != LIB_MEM_ERR_NONE) {
       *p_err = RTOS_ERR_ALLOC;
        goto end_err;
    }

    KAL_DataPtr->MemSegPtr = p_seg;

    #if (OS_CFG_MUTEX_EN == DEF_ENABLED)
        Mem_DynPoolCreate("KAL mutex pool",
                          &KAL_DataPtr->MutexPool,
                           KAL_DataPtr->MemSegPtr,
                           sizeof(KAL_LOCK),
                           sizeof(CPU_ALIGN),
                           0u,
                           LIB_MEM_BLK_QTY_UNLIMITED,
                          &err_lib);
        if (err_lib != LIB_MEM_ERR_NONE) {
           *p_err = RTOS_ERR_INIT;
            goto end_err;
        }
    #endif

    #if (OS_CFG_SEM_EN == DEF_ENABLED)
        Mem_DynPoolCreate("KAL sem pool",
                          &KAL_DataPtr->SemPool,
                           KAL_DataPtr->MemSegPtr,
                           sizeof(OS_SEM),
                           sizeof(CPU_ALIGN),
                           0u,
                           LIB_MEM_BLK_QTY_UNLIMITED,
                          &err_lib);
        if (err_lib != LIB_MEM_ERR_NONE) {
           *p_err = RTOS_ERR_INIT;
            goto end_err;
        }
    #endif

    #if (OS_CFG_TMR_EN == DEF_ENABLED)
        Mem_DynPoolCreate("KAL tmr pool",
                          &KAL_DataPtr->TmrPool,
                           KAL_DataPtr->MemSegPtr,
                           sizeof(KAL_TMR),
                           sizeof(CPU_ALIGN),
                           0u,
                           LIB_MEM_BLK_QTY_UNLIMITED,
                          &err_lib);
        if (err_lib != LIB_MEM_ERR_NONE) {
           *p_err = RTOS_ERR_INIT;
            goto end_err;
        }
    #endif

    #if (OS_CFG_TASK_REG_TBL_SIZE > 0u)
        Mem_DynPoolCreate("KAL task reg pool",
                          &KAL_DataPtr->TaskRegPool,
                           KAL_DataPtr->MemSegPtr,
                           sizeof(KAL_TASK_REG),
                           sizeof(CPU_ALIGN),
                           0u,
                           LIB_MEM_BLK_QTY_UNLIMITED,
                          &err_lib);
        if (err_lib != LIB_MEM_ERR_NONE) {
           *p_err = RTOS_ERR_INIT;
            goto end_err;
        }
    #endif

    CPU_CRITICAL_ENTER();
    KAL_InitStatus = KAL_INIT_STATUS_OK;
    CPU_CRITICAL_EXIT();

   *p_err = RTOS_ERR_NONE;

    return;

end_err:                                                        /* Init failed. Handle vars for other concurrent init.  */
    CPU_CRITICAL_ENTER();
    KAL_InitStatus = KAL_INIT_STATUS_FAIL;                      /* Indicate potential concurrent init that it failed.   */
    CPU_CRITICAL_EXIT();

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
             is_en = DEF_YES;
             break;


        case KAL_FEATURE_TASK_DEL:
             #if (OS_CFG_TASK_DEL_EN == DEF_ENABLED)
                 is_en = DEF_YES;
             #endif
             break;


#if (OS_CFG_MUTEX_EN == DEF_ENABLED)                            /* ----------------------- LOCKS ---------------------- */
        case KAL_FEATURE_LOCK_CREATE:
        case KAL_FEATURE_LOCK_ACQUIRE:
        case KAL_FEATURE_LOCK_RELEASE:
             is_en = DEF_YES;
             break;


        case KAL_FEATURE_LOCK_DEL:
             #if (OS_CFG_MUTEX_DEL_EN == DEF_ENABLED)
                 is_en = DEF_YES;
             #endif
             break;
#endif /* OS_CFG_MUTEX_EN */


#if (OS_CFG_SEM_EN == DEF_ENABLED)                              /* ----------------------- SEMS ----------------------- */
        case KAL_FEATURE_SEM_CREATE:
        case KAL_FEATURE_SEM_PEND:
        case KAL_FEATURE_SEM_POST:
             is_en = DEF_YES;
             break;


        case KAL_FEATURE_SEM_ABORT:
             #if (OS_CFG_SEM_PEND_ABORT_EN == DEF_ENABLED)
                 is_en = DEF_YES;
             #endif
             break;


        case KAL_FEATURE_SEM_SET:
             #if (OS_CFG_SEM_SET_EN == DEF_ENABLED)
                 is_en = DEF_YES;
             #endif
             break;


        case KAL_FEATURE_SEM_DEL:
             #if (OS_CFG_SEM_DEL_EN == DEF_ENABLED)
                 is_en = DEF_YES;
             #endif
             break;
#endif /* OS_CFG_SEM_EN */


        case KAL_FEATURE_TMR:                                   /* ----------------------- TMRS ----------------------- */
             #if (OS_CFG_TMR_EN == DEF_ENABLED)
                 is_en = DEF_YES;
             #endif
             break;


        case KAL_FEATURE_Q_CREATE:                              /* ---------------------- QUEUES ---------------------- */
        case KAL_FEATURE_Q_PEND:
        case KAL_FEATURE_Q_POST:
             #if (OS_CFG_Q_EN == DEF_ENABLED)
                 is_en = DEF_YES;
             #endif
             break;


        case KAL_FEATURE_DLY:                                   /* ----------------------- DLYS ----------------------- */
             is_en = DEF_YES;
             break;


        case KAL_FEATURE_TASK_REG:                              /* ------------------- TASK STORAGE ------------------- */
             #if (OS_CFG_TASK_REG_TBL_SIZE > 0u)
                 is_en = DEF_YES;
             #endif
             break;


        case KAL_FEATURE_TICK_GET:                              /* ------------------- TICK CTR INFO ------------------ */
             is_en = DEF_YES;
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_INVALID_ARG        Specified stack size is invalid.
*                                   RTOS_ERR_NOT_SUPPORTED      'p_cfg' parameter was not NULL.
*                                   RTOS_ERR_ALLOC              Task stack or control block allocation failed.
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
    KAL_TASK_HANDLE   handle = KAL_TaskHandleNull;
    KAL_TASK         *p_task;
    CPU_ADDR          stk_addr;
    CPU_ADDR          stk_addr_aligned;
    CPU_SIZE_T        actual_stk_size_bytes;
    LIB_ERR           err_lib;


    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(handle);
        }

        if (stk_size_bytes < (OS_CFG_STK_SIZE_MIN * sizeof(CPU_STK))) {
           *p_err = RTOS_ERR_INVALID_ARG;
            return (handle);
        }

        if (p_cfg != DEF_NULL) {                                /* Make sure no unsupported cfg recv.                   */
           *p_err = RTOS_ERR_NOT_SUPPORTED;
            return (handle);
        }
    #else
        (void)p_cfg;
    #endif

    if (p_stk_base == DEF_NULL) {                               /* Must alloc task stk on mem seg.                      */
        stk_addr_aligned = (CPU_ADDR)Mem_SegAllocExt("KAL task stk",
                                                      KAL_DataPtr->MemSegPtr,
                                                      stk_size_bytes,
                                                      CPU_CFG_STK_ALIGN_BYTES,
                                                      DEF_NULL,
                                                     &err_lib);
        if (err_lib != LIB_MEM_ERR_NONE) {
           *p_err = RTOS_ERR_ALLOC;
            return (handle);
        }
        actual_stk_size_bytes = stk_size_bytes;
    } else {
                                                                /* Align stk ptr, if needed.                            */
        stk_addr              = (CPU_ADDR)p_stk_base;
        stk_addr_aligned      =  MATH_ROUND_INC_UP_PWR2(stk_addr, CPU_CFG_STK_ALIGN_BYTES);
        actual_stk_size_bytes =  stk_size_bytes - (stk_addr_aligned - stk_addr);
    }

    p_task = (KAL_TASK *)Mem_SegAlloc("KAL task",
                                       KAL_DataPtr->MemSegPtr,
                                       sizeof(KAL_TASK),
                                      &err_lib);
    if (err_lib != LIB_MEM_ERR_NONE) {
       *p_err = RTOS_ERR_ALLOC;
        return (handle);
    }

    p_task->StkBasePtr   = (CPU_STK *)stk_addr_aligned;
    p_task->StkSizeBytes =  actual_stk_size_bytes;
    p_task->NamePtr      =  p_name;
    handle.TaskObjPtr    = (void *)p_task;

   *p_err = RTOS_ERR_NONE;

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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_SUPPORTED      'p_cfg' parameter was not NULL.
*                                   RTOS_ERR_NULL_PTR           Handle contains a NULL/invalid pointer.
*                                   RTOS_ERR_INVALID_ARG        Invalid argument passed to function.
*                                   RTOS_ERR_ISR                Function called from an ISR context.
*
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
    KAL_TASK      *p_task;
    CPU_STK_SIZE   stk_size_words;
    CPU_STK_SIZE   stk_limit;
    OS_ERR         err_os;


    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }

        if (KAL_TASK_HANDLE_IS_NULL(task_handle) == DEF_YES) {
           *p_err = RTOS_ERR_NULL_PTR;
            return;
        }

        if (p_cfg != DEF_NULL) {                                /* Make sure no unsupported cfg recv.                   */
           *p_err = RTOS_ERR_NOT_SUPPORTED;
            return;
        }
    #else
        (void)p_cfg;
    #endif

    p_task         = (KAL_TASK *)task_handle.TaskObjPtr;
    stk_size_words =  p_task->StkSizeBytes / sizeof(CPU_STK);
    stk_limit      = (stk_size_words * (100u - KAL_CFG_TASK_STK_SIZE_PCT_FULL)) / 100u;

    OSTaskCreate(&p_task->TCB,
      (CPU_CHAR *)p_task->NamePtr,
                  p_fnct,
                  p_task_arg,
                  prio,
                  p_task->StkBasePtr,
                  stk_limit,
                  stk_size_words,
                  0u,
                  0u,
                  DEF_NULL,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &err_os);
    if (err_os == OS_ERR_NONE) {
       *p_err = RTOS_ERR_NONE;
    } else {
       *p_err = KAL_ErrConvert(err_os);
    }

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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NULL_PTR           Handle contains a NULL/invalid pointer.
*                                   RTOS_ERR_INVALID_ARG        Invalid argument passed to function.
*                                   RTOS_ERR_ISR                Function called from an ISR context.
*                                   RTOS_ERR_OS                 Generic OS error.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_TaskDel (KAL_TASK_HANDLE   task_handle,
                   RTOS_ERR         *p_err)
{
    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }

        if (KAL_TASK_HANDLE_IS_NULL(task_handle) == DEF_YES) {
           *p_err = RTOS_ERR_NULL_PTR;
            return;
        }
    #endif

    #if (OS_CFG_TASK_DEL_EN == DEF_ENABLED)
    {
        OS_TCB  *p_tcb;
        OS_ERR   err_os;


        p_tcb = &((KAL_TASK *)task_handle.TaskObjPtr)->TCB;     /* Get TCB from task handle provided.                   */

        OSTaskDel(p_tcb,
                 &err_os);
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return;
    }
    #else
        (void)task_handle;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return;
    #endif
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_ALLOC              Unable to allocate memory for lock.
*                                   RTOS_ERR_ISR                Function called from an ISR context.
*                                   RTOS_ERR_INVALID_ARG        Invalid argument passed to function.
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


    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(handle);
        }
    #endif

    #if (OS_CFG_MUTEX_EN == DEF_ENABLED)
    {
        KAL_LOCK    *p_kal_lock;
        CPU_INT08U   opt_flags;
        LIB_ERR      err_lib;
        OS_ERR       err_os;


        p_kal_lock = (KAL_LOCK *)Mem_DynPoolBlkGet(&KAL_DataPtr->MutexPool,
                                                   &err_lib);
        if (err_lib != LIB_MEM_ERR_NONE) {
           *p_err = RTOS_ERR_ALLOC;
            return (handle);
        }

        OSMutexCreate(           &p_kal_lock->Mutex,
                      (CPU_CHAR *)p_name,
                                 &err_os);
        if (err_os == OS_ERR_NONE) {
            if ((p_cfg                                                != DEF_NULL) &&
                (DEF_BIT_IS_SET(p_cfg->Opt, KAL_OPT_CREATE_REENTRANT) == DEF_YES)) {
                opt_flags = KAL_OPT_CREATE_REENTRANT;
            } else {
                opt_flags = KAL_OPT_CREATE_NON_REENTRANT;
            }
            p_kal_lock->OptFlags =  opt_flags;
            handle.LockObjPtr    = (void *)p_kal_lock;
           *p_err                =  RTOS_ERR_NONE;
        } else {
            Mem_DynPoolBlkFree(       &KAL_DataPtr->MutexPool,
                               (void *)p_kal_lock,
                                      &err_lib);
            (void)err_lib;                                     /* Err ignored.                                         */
           *p_err = KAL_ErrConvert(err_os);
        }

        return (handle);
    }
    #else
        (void)p_name;
        (void)p_cfg;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return (handle);
    #endif
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NULL_PTR           Handle contains a NULL/invalid pointer.
*                                   RTOS_ERR_INVALID_ARG        Handle or options specified is invalid.
*                                   RTOS_ERR_ISR                Function called from an ISR context.
*                                   RTOS_ERR_ABORT              Pend operation was aborted.
*                                   RTOS_ERR_TIMEOUT            Operation timed-out.
*                                   RTOS_ERR_OWNERSHIP          Calling task already owns the lock.
*                                   RTOS_ERR_WOULD_BLOCK        KAL_OPT_PEND_NON_BLOCKING opt specified and
*                                                               lock is not available.
*                                   RTOS_ERR_OS                 Generic OS error.
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
    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }

        if (KAL_LOCK_HANDLE_IS_NULL(lock_handle) == DEF_YES) {
           *p_err = RTOS_ERR_NULL_PTR;
            return;
        }

        if (DEF_BIT_IS_SET_ANY(opt, ~(KAL_OPT_PEND_NONE | KAL_OPT_PEND_NON_BLOCKING)) == DEF_YES) {
           *p_err = RTOS_ERR_INVALID_ARG;
            return;
        }
    #endif

    #if (OS_CFG_MUTEX_EN == DEF_ENABLED)
    {
        KAL_LOCK    *p_kal_lock;
        CPU_INT32U   timeout_ticks;
        OS_OPT       opt_os;
        OS_ERR       err_os;


        if (timeout_ms != KAL_TIMEOUT_INFINITE) {
            timeout_ticks = KAL_msToTicks(timeout_ms);
        } else {
            timeout_ticks = 0u;
        }

        opt_os = OS_OPT_NONE;
        if (DEF_BIT_IS_CLR(opt, KAL_OPT_PEND_NON_BLOCKING) == DEF_YES) {
            opt_os |= OS_OPT_PEND_BLOCKING;
        } else {
            opt_os |= OS_OPT_PEND_NON_BLOCKING;
        }

        p_kal_lock = (KAL_LOCK *)lock_handle.LockObjPtr;
        OSMutexPend(&p_kal_lock->Mutex,
                     timeout_ticks,
                     opt_os,
                     DEF_NULL,
                    &err_os);
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else if (err_os != OS_ERR_MUTEX_OWNER) {
           *p_err = KAL_ErrConvert(err_os);
        } else {
            if (DEF_BIT_IS_SET(p_kal_lock->OptFlags, KAL_OPT_CREATE_REENTRANT) == DEF_YES) {
               *p_err = RTOS_ERR_NONE;
            } else {
                OSMutexPost(&p_kal_lock->Mutex,                 /* Post mutex to decrement nesting ctr.                 */
                             OS_OPT_POST_NONE,
                            &err_os);
                (void)err_os;
               *p_err = RTOS_ERR_OWNERSHIP;
            }
        }

        return;
    }
    #else
        (void)lock_handle;
        (void)opt;
        (void)timeout_ms;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return;
    #endif
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NULL_PTR           Handle contains a NULL/invalid pointer.
*                                   RTOS_ERR_INVALID_ARG        Task releasing lock does not own lock.
*                                   RTOS_ERR_ISR                Function called from an ISR context.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_LockRelease (KAL_LOCK_HANDLE   lock_handle,
                       RTOS_ERR         *p_err)
{
    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }

        if (KAL_LOCK_HANDLE_IS_NULL(lock_handle) == DEF_YES) {
           *p_err = RTOS_ERR_NULL_PTR;
            return;
        }
    #endif

    #if (OS_CFG_MUTEX_EN == DEF_ENABLED)
    {
        KAL_LOCK  *p_kal_lock;
        OS_ERR     err_os;


        p_kal_lock = (KAL_LOCK *)lock_handle.LockObjPtr;
        OSMutexPost(&p_kal_lock->Mutex,
                     OS_OPT_POST_NONE,
                    &err_os);
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else if (err_os != OS_ERR_MUTEX_NESTING) {
           *p_err = KAL_ErrConvert(err_os);
        } else {
            if (DEF_BIT_IS_SET(p_kal_lock->OptFlags, KAL_OPT_CREATE_REENTRANT) == DEF_YES) {
               *p_err = RTOS_ERR_NONE;
            } else {
               *p_err = RTOS_ERR_OS;
            }
        }

        return;
    }
    #else
        (void)lock_handle;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return;
    #endif
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NULL_PTR           Handle contains a NULL/invalid pointer.
*                                   RTOS_ERR_INVALID_ARG        Handle or options specified is invalid.
*                                   RTOS_ERR_ISR                Function was called from an ISR.
*                                   RTOS_ERR_OS                 Generic OS error.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_LockDel (KAL_LOCK_HANDLE   lock_handle,
                   RTOS_ERR         *p_err)
{
    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }

        if (KAL_LOCK_HANDLE_IS_NULL(lock_handle) == DEF_YES) {
           *p_err = RTOS_ERR_NULL_PTR;
            return;
        }
    #endif

    #if ((OS_CFG_MUTEX_EN     == DEF_ENABLED) && \
         (OS_CFG_MUTEX_DEL_EN == DEF_ENABLED))                  /* Mutex and mutex del are avail.                       */
    {
        KAL_LOCK  *p_kal_lock;
        OS_ERR     err_os;


        p_kal_lock = (KAL_LOCK *)lock_handle.LockObjPtr;

        (void)OSMutexDel(&p_kal_lock->Mutex,
                          OS_OPT_DEL_ALWAYS,
                         &err_os);
        if (err_os == OS_ERR_NONE) {
            LIB_ERR  err_lib;


            Mem_DynPoolBlkFree(       &KAL_DataPtr->MutexPool,
                               (void *)p_kal_lock,
                                      &err_lib);
            (void)err_lib;
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return;
    }
    #else                                                       /* Mutex or mutex del is not avail.                     */
        (void)lock_handle;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return;
    #endif
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
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NOT_SUPPORTED      'p_cfg' parameter was not NULL.
*                                   RTOS_ERR_ALLOC              Unable to allocate memory for semaphore.
*                                   RTOS_ERR_INVALID_ARG        Invalid argument passed to function.
*                                   RTOS_ERR_ISR                Function called from an ISR context.
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

    #if (OS_CFG_SEM_EN == DEF_ENABLED)
    {
        OS_SEM   *p_sem;
        LIB_ERR   err_lib;
        OS_ERR    err_os;


        p_sem = (OS_SEM *)Mem_DynPoolBlkGet(&KAL_DataPtr->SemPool,
                                            &err_lib);
        if (err_lib != LIB_MEM_ERR_NONE) {
           *p_err = RTOS_ERR_ALLOC;
            return (handle);
        }

        OSSemCreate(            p_sem,
                    (CPU_CHAR *)p_name,
                                0u,
                               &err_os);
        if (err_os == OS_ERR_NONE) {
            handle.SemObjPtr = (void *)p_sem;
           *p_err            =  RTOS_ERR_NONE;
        } else {
            Mem_DynPoolBlkFree(       &KAL_DataPtr->SemPool,
                               (void *)p_sem,
                                      &err_lib);
            (void)err_lib;
           *p_err = KAL_ErrConvert(err_os);
        }

        return (handle);
    }
    #else
        (void)p_name;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return (handle);
    #endif
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
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NULL_PTR           Handle contains a NULL/invalid pointer.
*                                   RTOS_ERR_INVALID_ARG        Handle or options specified is invalid.
*                                   RTOS_ERR_ABORT              Pend operation was aborted.
*                                   RTOS_ERR_TIMEOUT            Operation timed-out.
*                                   RTOS_ERR_ISR                Function was called from an ISR.
*                                   RTOS_ERR_WOULD_BLOCK        KAL_OPT_PEND_NON_BLOCKING opt specified and
*                                                               semaphore is not available.
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
    #endif

    #if (OS_CFG_SEM_EN == DEF_ENABLED)
    {
        CPU_INT32U  timeout_ticks;
        OS_OPT      opt_os;
        OS_ERR      err_os;


        if (timeout_ms != KAL_TIMEOUT_INFINITE) {
            timeout_ticks = KAL_msToTicks(timeout_ms);
        } else {
            timeout_ticks = 0u;
        }

        opt_os = OS_OPT_NONE;
        if (DEF_BIT_IS_CLR(opt, KAL_OPT_PEND_NON_BLOCKING) == DEF_YES) {
            opt_os |= OS_OPT_PEND_BLOCKING;
        } else {
            opt_os |= OS_OPT_PEND_NON_BLOCKING;
        }

        OSSemPend((OS_SEM *)sem_handle.SemObjPtr,
                            timeout_ticks,
                            opt_os,
                            DEF_NULL,
                           &err_os);
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return;
    }
    #else
        (void)sem_handle;
        (void)opt;
        (void)timeout_ms;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return;
    #endif
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
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
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

    #if (OS_CFG_SEM_EN == DEF_ENABLED)
    {
        OS_ERR  err_os;


        OSSemPost((OS_SEM *)sem_handle.SemObjPtr,
                            OS_OPT_POST_1,
                           &err_os);
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return;
    }
    #else
        (void)sem_handle;
        (void)opt;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return;
    #endif
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NULL_PTR           Handle contains a NULL/invalid pointer.
*                                   RTOS_ERR_INVALID_ARG        Handle specified is invalid.
*                                   RTOS_ERR_ISR                Function called from an ISR context.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_SemPendAbort (KAL_SEM_HANDLE   sem_handle,
                        RTOS_ERR        *p_err)
{
    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }

        if (KAL_SEM_HANDLE_IS_NULL(sem_handle) == DEF_YES) {
           *p_err = RTOS_ERR_NULL_PTR;
            return;
        }
    #endif

    #if ((OS_CFG_SEM_EN            == DEF_ENABLED) && \
         (OS_CFG_SEM_PEND_ABORT_EN == DEF_ENABLED))
    {
        OS_ERR  err_os;


        (void)OSSemPendAbort((OS_SEM *)sem_handle.SemObjPtr,
                                       OS_OPT_PEND_ABORT_ALL,
                                      &err_os);
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return;
    }
    #else
        (void)sem_handle;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return;
    #endif
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NULL_PTR           Handle contains a NULL/invalid pointer.
*                                   RTOS_ERR_INVALID_ARG        Handle specified is invalid.
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
    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ------------------ VALIDATE ARGS ------------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }

        if (KAL_SEM_HANDLE_IS_NULL(sem_handle) == DEF_YES) {    /* Validate handle.                                     */
           *p_err = RTOS_ERR_NULL_PTR;
            return;
        }
    #endif

    #if ((OS_CFG_SEM_EN      == DEF_ENABLED) && \
         (OS_CFG_SEM_SET_EN  == DEF_ENABLED))
    {
        OS_ERR  err_os;


        OSSemSet((OS_SEM *)sem_handle.SemObjPtr,
                           cnt,
                          &err_os);
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return;
    }
    #else
        (void)sem_handle;
        (void)cnt;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return;
    #endif
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
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NULL_PTR           Handle contains a NULL/invalid pointer.
*                                   RTOS_ERR_INVALID_ARG        Handle specified is invalid.
*                                   RTOS_ERR_ISR                Function was called from an ISR.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_SemDel (KAL_SEM_HANDLE   sem_handle,
                  RTOS_ERR        *p_err)
{
    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }

        if (KAL_SEM_HANDLE_IS_NULL(sem_handle) == DEF_YES) {
           *p_err = RTOS_ERR_NULL_PTR;
            return;
        }
    #endif

    #if ((OS_CFG_SEM_EN     == DEF_ENABLED) && \
         (OS_CFG_SEM_DEL_EN == DEF_ENABLED))
    {
        OS_SEM  *p_sem;
        OS_ERR   err_os;


        p_sem = (OS_SEM *)sem_handle.SemObjPtr;

        (void)OSSemDel(p_sem,
                       OS_OPT_DEL_ALWAYS,
                      &err_os);
        if (err_os == OS_ERR_NONE) {
            LIB_ERR  err_lib;


            Mem_DynPoolBlkFree(       &KAL_DataPtr->SemPool,
                               (void *)p_sem,
                                      &err_lib);
            (void)err_lib;
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return;
    }
    #else
        (void)sem_handle;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return;
    #endif
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NULL_PTR           Null pointer passed to function.
*                                   RTOS_ERR_ALLOC              Unable to allocate memory for semaphore.
*                                   RTOS_ERR_INVALID_ARG        Invalid argument passed to function.
*                                   RTOS_ERR_ISR                Function called from an ISR context.
*                                   RTOS_ERR_OS                 Generic OS error.
*
* Return(s)   : Created timer handle.
*
* Note(s)     : none.
*********************************************************************************************************
*/

KAL_TMR_HANDLE  KAL_TmrCreate (const  CPU_CHAR          *p_name,
                                      void             (*p_callback)(void  *p_arg),
                                      void              *p_callback_arg,
                                      CPU_INT32U         interval_ms,
                                      KAL_TMR_EXT_CFG   *p_cfg,
                                      RTOS_ERR          *p_err)
{
    KAL_TMR_HANDLE  handle = KAL_TmrHandleNull;


    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(handle);
        }

        if (p_callback == DEF_NULL) {                           /* Validate callback fnct ptr.                          */
           *p_err = RTOS_ERR_NULL_PTR;
            return (handle);
        }

        if (interval_ms == 0u) {                                /* Validate time.                                       */
           *p_err = RTOS_ERR_INVALID_ARG;
            return (handle);
        }

        if (p_cfg != DEF_NULL) {
            if (DEF_BIT_IS_SET_ANY(p_cfg->Opt, ~(KAL_OPT_TMR_NONE | KAL_OPT_TMR_PERIODIC)) == DEF_YES) {
               *p_err = RTOS_ERR_INVALID_ARG;
                return (handle);
            }
        }
    #endif

    #if (OS_CFG_TMR_EN == DEF_ENABLED)
    {
        KAL_TMR     *p_tmr;
        CPU_INT32U   tmr_dly;
        CPU_INT32U   tmr_period;
        OS_OPT       opt_os;
        KAL_TICK     ticks;
        OS_ERR       err_os;
        LIB_ERR      err_lib;
        #if  ((OS_CFG_TMR_TASK_RATE_HZ         >= 1000u) && \
              (OS_CFG_TMR_TASK_RATE_HZ % 1000u ==    0u))
        const  CPU_INT08U   mult = OS_CFG_TMR_TASK_RATE_HZ / 1000u;
        #endif


        p_tmr  = (KAL_TMR *)Mem_DynPoolBlkGet(&KAL_DataPtr->TmrPool,
                                              &err_lib);
        if (err_lib != LIB_MEM_ERR_NONE) {
           *p_err = RTOS_ERR_ALLOC;
            return (handle);
        }

        p_tmr->CallbackFnct = p_callback;
        p_tmr->CallbackArg  = p_callback_arg;
                                                                /* Calc nbr of tmr ticks (rounded up nearest int).      */
        #if  ((OS_CFG_TMR_TASK_RATE_HZ         >= 1000u) && \
              (OS_CFG_TMR_TASK_RATE_HZ % 1000u ==    0u))       /* Optimize calc if possible for often used vals.       */
            ticks =    interval_ms * mult;
        #elif (OS_CFG_TMR_TASK_RATE_HZ ==  100u)
            ticks =  ((interval_ms +  9u) /  10u);
        #elif (OS_CFG_TMR_TASK_RATE_HZ ==   10u)
            ticks =  ((interval_ms + 99u) / 100u);
        #else                                                   /* General formula.                                     */
            ticks = (((interval_ms * OS_CFG_TMR_TASK_RATE_HZ)  + 1000u - 1u) / 1000u);
        #endif
                                                                /* Tmr is 'periodic'.                                   */
        if ((p_cfg                                            != DEF_NULL) &&
            (DEF_BIT_IS_SET(p_cfg->Opt, KAL_OPT_TMR_PERIODIC) == DEF_YES)) {
            opt_os     = OS_OPT_TMR_PERIODIC;
            tmr_dly    = 0u;
            tmr_period = ticks;
        } else {
            opt_os     = OS_OPT_TMR_ONE_SHOT;                   /* Tmr is 'one-shot'.                                   */
            tmr_dly    = ticks;
            tmr_period = 0u;
        }

        OSTmrCreate(           &p_tmr->Tmr,                     /* Create tmr obj.                                      */
                    (CPU_CHAR *)p_name,
                                tmr_dly,
                                tmr_period,
                                opt_os,
                                KAL_TmrFnctWrapper,
                    (void     *)p_tmr,
                               &err_os);
        if (err_os == OS_ERR_NONE) {
            handle.TmrObjPtr = p_tmr;
           *p_err            = RTOS_ERR_NONE;
        } else {
            Mem_DynPoolBlkFree(&KAL_DataPtr->TmrPool,
                                p_tmr,
                               &err_lib);
            (void)err_lib;
           *p_err = KAL_ErrConvert(err_os);
        }

        return (handle);
    }
    #else
        (void)p_name;
        (void)p_callback;
        (void)p_callback_arg;
        (void)interval_ms;
        (void)p_cfg;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return (handle);
    #endif
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NULL_PTR           Null timer handle passed to function.
*                                   RTOS_ERR_ISR                Function called from an ISR context.
*                                   RTOS_ERR_OS                 Generic OS error.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_TmrStart (KAL_TMR_HANDLE   tmr_handle,
                    RTOS_ERR        *p_err)
{
    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }

        if (KAL_TMR_HANDLE_IS_NULL(tmr_handle) == DEF_YES) {
           *p_err = RTOS_ERR_NULL_PTR;
            return;
        }
    #endif

    #if (OS_CFG_TMR_EN == DEF_ENABLED)
    {
        KAL_TMR  *p_tmr;
        OS_ERR    err_os;


        p_tmr = (KAL_TMR *)tmr_handle.TmrObjPtr;

        OSTmrStart(&p_tmr->Tmr,
                   &err_os);
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return;
    }
    #else
        (void)tmr_handle;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return;
    #endif
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NOT_SUPPORTED      'p_cfg' parameter was not NULL.
*                                   RTOS_ERR_ALLOC              Unable to allocate memory for queue.
*                                   RTOS_ERR_ISR                Function called from an ISR context.
*                                   RTOS_ERR_INVALID_ARG        Argument passed to function is invalid.
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

    #if (OS_CFG_Q_EN == DEF_ENABLED)
    {
        OS_Q     *p_q;
        LIB_ERR   err_lib;
        OS_ERR    err_os;


        p_q = (OS_Q *)Mem_SegAlloc("KAL Q",
                                    KAL_DataPtr->MemSegPtr,
                                    sizeof(OS_Q),
                                   &err_lib);
        if (err_lib != LIB_MEM_ERR_NONE) {
           *p_err = RTOS_ERR_ALLOC;
            return (handle);
        }

        OSQCreate(            p_q,
                  (CPU_CHAR *)p_name,
                              max_msg_qty,
                             &err_os);
        if (err_os == OS_ERR_NONE) {
            handle.QObjPtr = (void *)p_q;
           *p_err          =  RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return (handle);
    }
    #else
        (void)p_name;
        (void)max_msg_qty;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return (handle);
    #endif
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NULL_PTR           Handle contains a NULL/invalid pointer.
*                                   RTOS_ERR_INVALID_ARG        Handle or options specified is invalid.
*                                   RTOS_ERR_ABORT              Pend operation was aborted.
*                                   RTOS_ERR_TIMEOUT            Operation timed-out.
*                                   RTOS_ERR_ISR                Function was called from an ISR.
*                                   RTOS_ERR_WOULD_BLOCK        KAL_OPT_PEND_NON_BLOCKING opt specified and no
*                                                               message is available.
*                                   RTOS_ERR_OS                 Generic OS error.
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
    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(DEF_NULL);
        }

        if (KAL_Q_HANDLE_IS_NULL(q_handle) == DEF_YES) {
           *p_err = RTOS_ERR_NULL_PTR;
            return (DEF_NULL);
        }

        if (DEF_BIT_IS_SET_ANY(opt, ~(KAL_OPT_PEND_NONE | KAL_OPT_PEND_NON_BLOCKING)) == DEF_YES) {
           *p_err = RTOS_ERR_INVALID_ARG;
            return (DEF_NULL);
        }
    #endif

    #if (OS_CFG_Q_EN == DEF_ENABLED)
    {
        void         *p_msg;
        CPU_INT32U    timeout_ticks;
        OS_MSG_SIZE   msg_size;
        OS_OPT        opt_os;
        OS_ERR        err_os;


        if (timeout_ms != KAL_TIMEOUT_INFINITE) {
            timeout_ticks = KAL_msToTicks(timeout_ms);
        } else {
            timeout_ticks = 0u;
        }

        opt_os = OS_OPT_NONE;
        if (DEF_BIT_IS_CLR(opt, KAL_OPT_PEND_NON_BLOCKING) == DEF_YES) {
            opt_os |= OS_OPT_PEND_BLOCKING;
        } else {
            opt_os |= OS_OPT_PEND_NON_BLOCKING;
        }

        p_msg = OSQPend((OS_Q *)q_handle.QObjPtr,
                                timeout_ticks,
                                opt_os,
                               &msg_size,
                                DEF_NULL,
                               &err_os);
        (void)msg_size;
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return (p_msg);
    }
    #else
        (void)q_handle;
        (void)opt;
        (void)timeout_ms;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return (DEF_NULL);
    #endif
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NULL_PTR           Handle contains a NULL/invalid pointer.
*                                   RTOS_ERR_INVALID_ARG        Handle or options specified is invalid.
*                                   RTOS_ERR_NO_MORE_RSRC       Queue cannot contain any more message,
*                                                               no more message available.
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
    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }

        if (KAL_Q_HANDLE_IS_NULL(q_handle) == DEF_YES) {
           *p_err = RTOS_ERR_NULL_PTR;
            return;
        }

        if (opt != KAL_OPT_POST_NONE) {
           *p_err = RTOS_ERR_INVALID_ARG;
            return;
        }
    #else
        (void)opt;
    #endif

    #if (OS_CFG_Q_EN == DEF_ENABLED)                            /* Qs are available.                                    */
    {
        OS_ERR  err_os;


        OSQPost((OS_Q *)q_handle.QObjPtr,
                        p_msg,
                        0u,
                        OS_OPT_POST_1,
                       &err_os);
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return;
    }
    #else
        (void)q_handle;
        (void)p_msg;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return;
    #endif
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
    CPU_INT32U  dly_ticks;
    OS_ERR      err_os;


    dly_ticks = KAL_msToTicks(dly_ms);

    OSTimeDly(dly_ticks,
              OS_OPT_TIME_DLY,
             &err_os);
    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)
        switch (err_os) {
            case OS_ERR_NONE:
            case OS_ERR_TIME_ZERO_DLY:
                 break;

            case OS_ERR_OPT_INVALID:
            case OS_ERR_SCHED_LOCKED:
            case OS_ERR_TIME_DLY_ISR:
            default:
                 CPU_SW_EXCEPTION(;);
                 break;
        }
    #else
        (void)err_os;                                          /* Ignore err from OSTimeDly().                         */
    #endif

    return;
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
    OS_OPT  opt_os;
    OS_ERR  err_os;


    if (DEF_BIT_IS_SET(opt, KAL_OPT_DLY_PERIODIC) == DEF_YES) {
        opt_os = OS_OPT_TIME_PERIODIC;
    } else {
        opt_os = OS_OPT_TIME_DLY;
    }

    OSTimeDly(dly_ticks,
              opt_os,
             &err_os);
    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)
        switch (err_os) {
            case OS_ERR_NONE:
            case OS_ERR_TIME_ZERO_DLY:
                 break;

            case OS_ERR_OPT_INVALID:
            case OS_ERR_SCHED_LOCKED:
            case OS_ERR_TIME_DLY_ISR:
            default:
                 CPU_SW_EXCEPTION(;);
                 break;
        }
    #else
        (void)err_os;                                          /* Ignore err from OSTimeDly().                         */
    #endif

    return;
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NOT_SUPPORTED      'p_cfg' parameter was not NULL.
*                                   RTOS_ERR_ALLOC              Unable to allocate memory for task register.
*                                   RTOS_ERR_NO_MORE_RSRC       No more task register available.
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

    #if (OS_CFG_TASK_REG_TBL_SIZE > 0u)
    {
        KAL_TASK_REG  *p_task_reg;
        LIB_ERR        err_lib;
        OS_ERR         err_os;


        p_task_reg = (KAL_TASK_REG *)Mem_DynPoolBlkGet(&KAL_DataPtr->TaskRegPool,
                                                       &err_lib);
        if (err_lib != LIB_MEM_ERR_NONE) {
           *p_err = RTOS_ERR_ALLOC;
            return (handle);
        }

        p_task_reg->Id = OSTaskRegGetID(&err_os);
        if (err_os == OS_ERR_NONE) {
            handle.TaskRegObjPtr = (void *)p_task_reg;
           *p_err                =  RTOS_ERR_NONE;
        } else {
                                                                /* Free rsrc to pool.                                   */
            Mem_DynPoolBlkFree(       &KAL_DataPtr->TaskRegPool,
                               (void *)p_task_reg,
                                      &err_lib);
            (void)err_lib;                                     /* Err ignored.                                         */
           *p_err = KAL_ErrConvert(err_os);
        }

        return (handle);
    }
    #else
       *p_err = RTOS_ERR_NOT_AVAIL;

        return (handle);
    #endif
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NULL_PTR           Task register handle is NULL.
*                                   RTOS_ERR_INVALID_ARG        Handle specified is invalid.
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
    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(0u);
        }

        if (KAL_TASK_REG_HANDLE_IS_NULL(task_reg_handle) == DEF_YES) {
           *p_err = RTOS_ERR_NULL_PTR;
            return (0u);
        }
    #endif

    #if (OS_CFG_TASK_REG_TBL_SIZE > 0u)
    {
        KAL_TASK_REG  *p_task_reg;
        OS_TCB        *p_task_tcb;
        CPU_INT32U     ret_val;
        OS_ERR         err_os;


        p_task_reg = (KAL_TASK_REG *)task_reg_handle.TaskRegObjPtr;

        if (KAL_TASK_HANDLE_IS_NULL(task_handle) == DEF_YES) {
            p_task_tcb = OSTCBCurPtr;                           /* Use cur task if no task handle is provided.          */
        } else {
                                                                /* Get TCB from task handle provided.                   */
            p_task_tcb = &((KAL_TASK *)task_handle.TaskObjPtr)->TCB;
        }

        ret_val = OSTaskRegGet(p_task_tcb,
                               p_task_reg->Id,
                              &err_os);
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return (ret_val);
    }
    #else
        (void)task_handle;
        (void)task_reg_handle;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return (0u);
    #endif
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
*                                   RTOS_ERR_NONE               No error.
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
*                                   RTOS_ERR_NULL_PTR           Task register handle is NULL.
*                                   RTOS_ERR_INVALID_ARG        Handle specified is invalid.
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
    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }

        if (KAL_TASK_REG_HANDLE_IS_NULL(task_reg_handle) == DEF_YES) {
           *p_err = RTOS_ERR_NULL_PTR;
            return;
        }
    #endif

    #if (OS_CFG_TASK_REG_TBL_SIZE > 0u)
    {
        KAL_TASK_REG  *p_task_reg;
        OS_TCB        *p_task_tcb;
        OS_ERR         err_os;


        p_task_reg = (KAL_TASK_REG *)task_reg_handle.TaskRegObjPtr;

        if (KAL_TASK_HANDLE_IS_NULL(task_handle) == DEF_YES) {
            p_task_tcb = OSTCBCurPtr;                           /* Use cur task if no task handle is provided.          */
        } else {
                                                                /* Get TCB from task handle provided.                   */
            p_task_tcb = &((KAL_TASK *)task_handle.TaskObjPtr)->TCB;
        }

        OSTaskRegSet(       p_task_tcb,
                            p_task_reg->Id,
                    (OS_REG)val,
                           &err_os);
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return;
    }
    #else
        (void)task_handle;
        (void)task_reg_handle;
        (void)val;

       *p_err = RTOS_ERR_NOT_AVAIL;

        return;
    #endif
}


/*
*********************************************************************************************************
*                                             KAL_TickGet()
*
* Description : Get value of OS' tick counter.
*
* Argument(s) : p_err           Pointer to variable that will receive the return error code from this function:
*
*                                   RTOS_ERR_NONE               No error.
*
* Return(s)   : OS tick counter's value.
*
* Note(s)     : none.
*********************************************************************************************************
*/

KAL_TICK  KAL_TickGet (RTOS_ERR  *p_err)
{
    KAL_TICK  tick_cnt;
    OS_ERR    err_os;


    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(0u);
        }
    #endif

    tick_cnt = OSTimeGet(&err_os);
    if (err_os == OS_ERR_NONE) {
       *p_err = RTOS_ERR_NONE;
    } else {
       *p_err = KAL_ErrConvert(err_os);
    }

    return (tick_cnt);
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         KAL_TmrFnctWrapper()
*
* Description : Wrapper function for timer callback.
*
* Argument(s) : p_tmr_os        Pointer to OS timer object.
*
*               p_arg           Pointer to KAL timer object.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (OS_CFG_TMR_EN == DEF_ENABLED)
static  void  KAL_TmrFnctWrapper (void  *p_tmr_os,
                                  void  *p_arg)
{
    KAL_TMR  *p_tmr;


    (void)p_tmr_os;

    p_tmr = (KAL_TMR *)p_arg;
    p_tmr->CallbackFnct(p_tmr->CallbackArg);
}
#endif


/*
*********************************************************************************************************
*                                            KAL_msToTicks()
*
* Description : Convert milliseconds value in tick value.
*
* Argument(s) : ms              Millisecond value to convert.
*
* Return(s)   : Number of ticks corresponding to the millisecond value, rounded up, if needed.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  KAL_TICK  KAL_msToTicks (CPU_INT32U  ms)
{
           KAL_TICK    ticks;
#if  ((OS_CFG_TICK_RATE_HZ         >= 1000u) && \
      (OS_CFG_TICK_RATE_HZ % 1000u ==    0u))
    const  CPU_INT08U  mult = OS_CFG_TICK_RATE_HZ / 1000u;
#endif


    #if  ((OS_CFG_TICK_RATE_HZ         >= 1000u) && \
          (OS_CFG_TICK_RATE_HZ % 1000u ==    0u))               /* Optimize calc if possible for often used vals.       */
        ticks =    ms * mult;
    #elif (OS_CFG_TICK_RATE_HZ ==  100u)
        ticks =  ((ms +  9u) /  10u);
    #elif (OS_CFG_TICK_RATE_HZ ==   10u)
        ticks =  ((ms + 99u) / 100u);
    #else                                                       /* General formula.                                     */
        ticks = (((ms * OS_CFG_TICK_RATE_HZ)  + 1000u - 1u) / 1000u);
    #endif

    return (ticks);
}


/*
*********************************************************************************************************
*                                           KAL_ErrConvert()
*
* Description : Convert OS errors in KAL errors.
*
* Argument(s) : err_os          Error value used by the OS.
*
* Return(s)   : KAL error.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  RTOS_ERR  KAL_ErrConvert (OS_ERR  err_os)
{
    RTOS_ERR  err_rtos;


    switch (err_os) {
        case OS_ERR_NONE:
        case OS_ERR_PEND_ABORT_NONE:
        case OS_ERR_TIME_ZERO_DLY:
             err_rtos = RTOS_ERR_NONE;
             break;


        case OS_ERR_MUTEX_NOT_OWNER:
        case OS_ERR_NAME:
        case OS_ERR_OBJ_TYPE:
        case OS_ERR_OPT_INVALID:
        case OS_ERR_PRIO_INVALID:
        case OS_ERR_Q_SIZE:
        case OS_ERR_REG_ID_INVALID:
        case OS_ERR_STK_INVALID:
        case OS_ERR_STK_SIZE_INVALID:
        case OS_ERR_STK_LIMIT_INVALID:
        case OS_ERR_TASK_DEL_IDLE:
        case OS_ERR_TASK_INVALID:
        case OS_ERR_TCB_INVALID:
        case OS_ERR_TMR_INVALID_PERIOD:
             err_rtos = RTOS_ERR_INVALID_ARG;
             break;


        case OS_ERR_OBJ_PTR_NULL:
        case OS_ERR_PTR_INVALID:
             err_rtos = RTOS_ERR_NULL_PTR;
             break;


        case OS_ERR_ILLEGAL_CREATE_RUN_TIME:
        case OS_ERR_MUTEX_NESTING:
        case OS_ERR_OBJ_DEL:
        case OS_ERR_SCHED_LOCKED:
        case OS_ERR_STATE_INVALID:
        case OS_ERR_STATUS_INVALID:
        case OS_ERR_TASK_WAITING:
        case OS_ERR_TMR_INACTIVE:
        case OS_ERR_TMR_INVALID_DLY:
        case OS_ERR_TMR_INVALID_STATE:
        case OS_ERR_TMR_INVALID:
             err_rtos = RTOS_ERR_OS;
             break;


        case OS_ERR_TIMEOUT:
             err_rtos = RTOS_ERR_TIMEOUT;
             break;


        case OS_ERR_PEND_ABORT:
             err_rtos = RTOS_ERR_ABORT;
             break;


        case OS_ERR_PEND_WOULD_BLOCK:
             err_rtos = RTOS_ERR_WOULD_BLOCK;
             break;


        case OS_ERR_CREATE_ISR:
        case OS_ERR_DEL_ISR:
        case OS_ERR_PEND_ISR:
        case OS_ERR_PEND_ABORT_ISR:
        case OS_ERR_POST_ISR:
        case OS_ERR_SET_ISR:
        case OS_ERR_TASK_CREATE_ISR:
        case OS_ERR_TASK_DEL_ISR:
        case OS_ERR_TIME_DLY_ISR:
        case OS_ERR_TMR_ISR:
             err_rtos = RTOS_ERR_ISR;
             break;


        case OS_ERR_SEM_OVF:
             err_rtos = RTOS_ERR_WOULD_OVF;
             break;


        case OS_ERR_MSG_POOL_EMPTY:
        case OS_ERR_NO_MORE_ID_AVAIL:
        case OS_ERR_Q_MAX:
             err_rtos = RTOS_ERR_NO_MORE_RSRC;
             break;


        case OS_ERR_MUTEX_OWNER:
             err_rtos = RTOS_ERR_OWNERSHIP;
             break;


        default:
             err_rtos = RTOS_ERR_OS;
             break;
    }

    return (err_rtos);
}
