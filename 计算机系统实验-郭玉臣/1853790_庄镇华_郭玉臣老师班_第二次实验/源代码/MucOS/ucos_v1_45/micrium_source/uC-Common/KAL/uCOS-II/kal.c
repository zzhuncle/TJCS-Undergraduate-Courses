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
*                                               uCOS-II
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
#include  <os_cfg.h>
#include  <Source/ucos_ii.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#define  KAL_CFG_ARG_CHK_EXT_EN                 OS_ARG_CHK_EN

#define  KAL_LOCK_OWNER_PRIO_NONE              (OS_LOWEST_PRIO + 1u)

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
KAL_CPP_EXT  const  KAL_TICK_RATE_HZ     KAL_TickRate          =  OS_TICKS_PER_SEC;

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
           CPU_STK     *StkBasePtr;                             /* Task stack base ptr.                                 */
           CPU_INT32U   StkSizeBytes;                           /* Task stack size (in bytes).                          */

           CPU_INT08U   Prio;                                   /* Task prio.                                           */
#if (OS_TASK_NAME_EN == DEF_ENABLED)                            /* Check if task name is en.                            */
    const  CPU_CHAR    *NamePtr;                                /* Task name string.                                    */
#endif
} KAL_TASK;

                                                                /* ------------------- KAL LOCK TYPE ------------------ */
typedef  struct  kal_lock {
    OS_EVENT    *SemEventPtr;                                   /* Pointer to an OS_EVENT sem.                          */
    CPU_INT08U   OptFlags;                                      /* Opt flags passed at creation.                        */

    CPU_INT08U   OwnerPrio;                                     /* Prio of curr lock owner. Used only when re-entrant.  */
    CPU_INT08U   NestingCtr;                                    /* Curr cnt of nesting calls to acquire re-entrant lock.*/
} KAL_LOCK;

                                                                /* ------------------- KAL TMR TYPE ------------------- */
#if (OS_TMR_EN == DEF_ENABLED)
typedef  struct  kal_tmr {
    OS_TMR   *TmrPtr;                                           /* Ptr to an OS-II tmr obj.                             */

    void    (*CallbackFnct)(void  *p_arg);                      /* Tmr registered callback fnct.                        */
    void     *CallbackArg;                                      /* Arg to pass to callback fnct.                        */
} KAL_TMR;
#endif

                                                                /* ---------------- KAL TASK REG TYPE ----------------- */
#if (OS_TASK_REG_TBL_SIZE > 0u)
typedef  struct  kal_task_reg {
    CPU_INT08U  Id;                                             /* Id of the task reg.                                  */
} KAL_TASK_REG;
#endif

                                                                /* -------------- KAL INTERNAL DATA TYPE -------------- */
typedef  struct  kal_data {
    MEM_SEG       *MemSegPtr;                                   /* Mem Seg to alloc from.                               */

#if (OS_SEM_EN == DEF_ENABLED)
    MEM_DYN_POOL   LockPool;                                    /* Dyn mem pool used to alloc locks.                    */
#endif

#if (OS_TMR_EN == DEF_ENABLED)
    MEM_DYN_POOL   TmrPool;                                     /* Dyn mem pool used to alloc tmrs.                     */
#endif

#if (OS_TASK_REG_TBL_SIZE > 0u)
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

#if (OS_TMR_EN == DEF_ENABLED)
static  void      KAL_TmrFnctWrapper(void        *p_tmr_os,
                                     void        *p_arg);
#endif

static  KAL_TICK  KAL_msToTicks     (CPU_INT32U   ms);

static  RTOS_ERR  KAL_ErrConvert    (CPU_INT08U   err_os);


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
            OSTimeDly(100u);
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

    #if (OS_SEM_EN == DEF_ENABLED)
        Mem_DynPoolCreate("KAL lock pool",
                          &KAL_DataPtr->LockPool,
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

    #if (OS_TMR_EN == DEF_ENABLED)
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

    #if (OS_TASK_REG_TBL_SIZE > 0u)
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


    is_en = DEF_NO;

    switch (feature) {
        case KAL_FEATURE_TASK_CREATE:                           /* ---------------------- TASKS ----------------------- */
             is_en = DEF_YES;
             break;


        case KAL_FEATURE_TASK_DEL:
             #if (OS_TASK_DEL_EN == DEF_ENABLED)
                 is_en = DEF_YES;
             #endif
             break;


#if (OS_SEM_EN == DEF_ENABLED)                                  /* ------------------- LOCKS & SEMS ------------------- */
        case KAL_FEATURE_LOCK_CREATE:
        case KAL_FEATURE_LOCK_RELEASE:
        case KAL_FEATURE_SEM_CREATE:
             is_en = DEF_YES;
             break;


        case KAL_FEATURE_LOCK_ACQUIRE:
             if (DEF_BIT_IS_CLR(opt, KAL_OPT_CREATE_REENTRANT) == DEF_YES) {
                                                                /* Re-entrant locks not supported in OS-II right now.   */
                 if (DEF_BIT_IS_CLR(opt, KAL_OPT_PEND_NON_BLOCKING) == DEF_YES) {
                     is_en = DEF_YES;
                 } else {
                     #if (OS_SEM_ACCEPT_EN == DEF_ENABLED)
                         is_en = DEF_YES;                       /* Non-blocking supported only if OSSemAccept() is en.  */
                     #endif
                 }
             }
             break;


        case KAL_FEATURE_SEM_PEND:
             if (DEF_BIT_IS_CLR(opt, KAL_OPT_PEND_NON_BLOCKING) == DEF_YES) {
                 is_en = DEF_YES;
             } else {
                 #if (OS_SEM_ACCEPT_EN == DEF_ENABLED)
                     is_en = DEF_YES;                           /* Non-blocking supported only if OSSemAccept() is en.  */
                 #endif
             }
             break;


        case KAL_FEATURE_SEM_POST:
             is_en = DEF_YES;
             break;


        case KAL_FEATURE_SEM_ABORT:
             #if (OS_SEM_PEND_ABORT_EN == DEF_ENABLED)
                 is_en = DEF_YES;
             #endif
             break;


        case KAL_FEATURE_SEM_SET:
             #if (OS_SEM_SET_EN == DEF_ENABLED)
                 is_en = DEF_YES;
             #endif
             break;


#if (OS_SEM_DEL_EN == DEF_ENABLED)
        case KAL_FEATURE_LOCK_DEL:
             if (DEF_BIT_IS_CLR(opt, KAL_OPT_CREATE_REENTRANT) == DEF_YES) {
                 is_en = DEF_YES;
             }
             break;


        case KAL_FEATURE_SEM_DEL:
             is_en = DEF_YES;
             break;
#endif
#endif /* OS_SEM_EN */


        case KAL_FEATURE_TMR:                                   /* ----------------------- TMRS ----------------------- */
             #if (OS_TMR_EN == DEF_ENABLED)
                 is_en = DEF_YES;
             #endif
             break;


#if (OS_Q_EN == DEF_ENABLED)                                    /* ---------------------- QUEUES ---------------------- */
        case KAL_FEATURE_Q_CREATE:
             #if ((OS_Q_POST_EN     == DEF_ENABLED) || \
                  (OS_Q_POST_OPT_EN == DEF_ENABLED))
                 is_en = DEF_YES;                               /* Qs and at least one of the Q post fnct are avail.    */
             #endif
             break;


        case KAL_FEATURE_Q_PEND:
             if (DEF_BIT_IS_CLR(opt, KAL_OPT_PEND_NON_BLOCKING) == DEF_YES) {
                 is_en = DEF_YES;
             } else {
                 #if (OS_Q_ACCEPT_EN == DEF_ENABLED)
                     is_en = DEF_YES;                           /* Non-blocking supported only if OSQAccept() is en.    */
                 #endif
             }
             break;


        case KAL_FEATURE_Q_POST:
             #if ((OS_Q_POST_OPT_EN == DEF_ENABLED) || \
                  (OS_Q_POST_EN == DEF_ENABLED))
                 is_en = DEF_YES;
             #endif
             break;
#endif  /* OS_Q_EN */


        case KAL_FEATURE_DLY:                                   /* ----------------------- DLYS ----------------------- */
             if (DEF_BIT_IS_CLR(opt, KAL_OPT_DLY_PERIODIC) == DEF_YES) {
                 is_en = DEF_YES;
             }
             break;


        case KAL_FEATURE_TASK_REG:                              /* ------------------- TASK STORAGE ------------------- */
             #if (OS_TASK_REG_TBL_SIZE > 0u)
                 is_en = DEF_YES;
             #endif
             break;


        case KAL_FEATURE_TICK_GET:                              /* ------------------- TICK CTR INFO ------------------ */
             #if (OS_TIME_GET_SET_EN == DEF_ENABLED)
                 is_en = DEF_YES;
             #endif
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
    p_task->Prio         =  0u;
    #if (OS_TASK_NAME_EN == DEF_ENABLED)                        /* Check if task name is en.                            */
        p_task->NamePtr  =  p_name;
    #else
        (void)p_name;
    #endif
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
    CPU_INT32U     stk_top_offset;
    CPU_INT08U     err_os;


    #if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                 /* ---------------- VALIDATE ARGUMENTS ---------------- */
        if (p_err == DEF_NULL) {                                /* Validate err ptr.                                    */
            CPU_SW_EXCEPTION(;);
        }

        if (p_fnct == DEF_NULL) {
           *p_err = RTOS_ERR_NULL_PTR;
            return;
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

    #if (OS_STK_GROWTH == DEF_ENABLED)                          /* Set stk top offset, based on OS_STK_GROWTH cfg.      */
        stk_top_offset = stk_size_words - 1u;
    #else
        stk_top_offset = 0u;
    #endif

    #if (OS_TASK_CREATE_EXT_EN == DEF_ENABLED)
    {
        CPU_INT32U  stk_bottom_offset;


        #if (OS_STK_GROWTH == DEF_ENABLED)                      /* Set stk bottom offset, based on OS_STK_GROWTH cfg.   */
            stk_bottom_offset = 0u;
        #else
            stk_bottom_offset = stk_size_words - 1u;
        #endif

        err_os = OSTaskCreateExt(p_fnct,
                                 p_task_arg,
                                &p_task->StkBasePtr[stk_top_offset],
                                 prio,
                                 prio,
                                &p_task->StkBasePtr[stk_bottom_offset],
                                 stk_size_words,
                                 DEF_NULL,
                                 OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK);
    }
    #else
        err_os = OSTaskCreate(p_fnct,
                              p_task_arg,
                             &p_task->StkBasePtr[stk_top_offset],
                              prio);
    #endif
    if (err_os != OS_ERR_NONE) {
       *p_err = KAL_ErrConvert(err_os);
        return;
    }

   *p_err        = RTOS_ERR_NONE;
    p_task->Prio = prio;

    #if (OS_TASK_NAME_EN == DEF_ENABLED)                        /* Set task name if names en.                           */
        if (p_task->NamePtr != DEF_NULL) {                      /* Do not try to set name if name is NULL.              */
            OSTaskNameSet(prio, (INT8U *)p_task->NamePtr, &err_os);
            if (err_os != OS_ERR_NONE) {
               *p_err = KAL_ErrConvert(err_os);
            }
        }
    #endif

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
*                                   RTOS_ERR_INVALID_ARG        Task handle is invalid.
*                                   RTOS_ERR_ISR                If trying to delete task from an ISR.
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

    #if (OS_TASK_DEL_EN == DEF_ENABLED)
    {
        KAL_TASK    *p_task;
        CPU_INT08U   err_os;


        p_task = (KAL_TASK *)task_handle.TaskObjPtr;

        err_os =  OSTaskDel(p_task->Prio);
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
*                                   RTOS_ERR_CREATE_FAIL        Lock creation failed.
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

        if (p_cfg != DEF_NULL) {
            if (DEF_BIT_IS_SET_ANY(p_cfg->Opt, ~(KAL_OPT_CREATE_NONE | KAL_OPT_CREATE_REENTRANT)) == DEF_YES) {
               *p_err = RTOS_ERR_INVALID_ARG;
                return (handle);
            }
        }
    #endif

    #if (OS_SEM_EN == DEF_ENABLED)
    {
        KAL_LOCK  *p_kal_lock;
        OS_EVENT  *p_sem;
        LIB_ERR    err_lib;


        p_kal_lock = (KAL_LOCK *)Mem_DynPoolBlkGet(&KAL_DataPtr->LockPool,
                                                   &err_lib);
        if (err_lib != LIB_MEM_ERR_NONE) {
           *p_err = RTOS_ERR_ALLOC;
            return (handle);
        }

        p_kal_lock->OptFlags = DEF_BIT_NONE;

        if ((p_cfg                                                != DEF_NULL) &&
            (DEF_BIT_IS_SET(p_cfg->Opt, KAL_OPT_CREATE_REENTRANT) == DEF_YES)) {
                                                                /* Created lock is re-entrant.                          */
            DEF_BIT_SET(p_kal_lock->OptFlags, KAL_OPT_CREATE_REENTRANT);
            p_kal_lock->OwnerPrio  = KAL_LOCK_OWNER_PRIO_NONE;
            p_kal_lock->NestingCtr = 0u;
        }

       *p_err = RTOS_ERR_NONE;

        p_sem = OSSemCreate(1u);
        if (p_sem == (OS_EVENT *)0) {
            Mem_DynPoolBlkFree(       &KAL_DataPtr->LockPool,   /* Free rsrc to pool.                                   */
                               (void *)p_kal_lock,
                                      &err_lib);
            (void)err_lib;                                     /* Err ignored.                                         */

           *p_err = RTOS_ERR_CREATE_FAIL;
            return (handle);
        }

        #if (OS_EVENT_NAME_EN == DEF_ENABLED)                   /* Set name only if cfg is en.                          */
            if (p_name != DEF_NULL) {
                CPU_INT08U  err_os;


                OSEventNameSet(         p_sem,
                               (INT8U *)p_name,
                                       &err_os);
                (void)err_os;                                  /* Err ignored, no err can occur if OSSemCreate() is OK.*/
            }
        #else
            (void)p_name;
        #endif

        p_kal_lock->SemEventPtr =  p_sem;
        handle.LockObjPtr       = (void *)p_kal_lock;

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
*                                   RTOS_ERR_TIMEOUT            Operation timed-out.
*                                   RTOS_ERR_ISR                Function was called from an ISR.
*                                   RTOS_ERR_WOULD_BLOCK        KAL_OPT_PEND_NON_BLOCKING opt specified and
*                                                               lock is not available.
*                                   RTOS_ERR_WOULD_OVF          Nesting count would overflow.
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

    #if (OS_SEM_EN == DEF_ENABLED)
    {
        KAL_LOCK    *p_kal_lock;
        OS_EVENT    *p_sem;
        CPU_INT32U   timeout_ticks;
        CPU_INT08U   err_os;
        CPU_SR_ALLOC();


        p_kal_lock = (KAL_LOCK *)lock_handle.LockObjPtr;
        p_sem      =  p_kal_lock->SemEventPtr;

        if (timeout_ms != KAL_TIMEOUT_INFINITE) {
            timeout_ticks = KAL_msToTicks(timeout_ms);
        } else {
            timeout_ticks = 0u;
        }

        if (DEF_BIT_IS_SET(p_kal_lock->OptFlags, KAL_OPT_CREATE_REENTRANT) == DEF_YES) {
            CPU_CRITICAL_ENTER();
            if (p_kal_lock->OwnerPrio == OSPrioCur) {
                p_kal_lock->NestingCtr++;
                if (p_kal_lock->NestingCtr != 0u) {
                   *p_err = RTOS_ERR_NONE;
                } else {
                   *p_err = RTOS_ERR_WOULD_OVF;
                    p_kal_lock->NestingCtr--;
                }
                CPU_CRITICAL_EXIT();
                return;
            }
            CPU_CRITICAL_EXIT();
        }

        if (DEF_BIT_IS_CLR(opt, KAL_OPT_PEND_NON_BLOCKING) == DEF_YES) {
            OSSemPend(p_sem,
                      timeout_ticks,
                     &err_os);
            if (err_os == OS_ERR_NONE) {
               *p_err = RTOS_ERR_NONE;
            } else {
               *p_err = KAL_ErrConvert(err_os);
                return;
            }
        } else {
            #if (OS_SEM_ACCEPT_EN == DEF_ENABLED)               /* If OSSemAccept() is en.                              */
                CPU_INT16U  ret_val;


                ret_val = OSSemAccept(p_sem);
                if (ret_val != 0u) {
                   *p_err = RTOS_ERR_NONE;
                } else {
                   *p_err = RTOS_ERR_WOULD_BLOCK;
                    return;
                }
            #else                                               /* If OSSemAccept() is dis, cannot exec operation.      */
               *p_err = RTOS_ERR_NOT_AVAIL;
                return;
            #endif
        }

        if (DEF_BIT_IS_SET(p_kal_lock->OptFlags, KAL_OPT_CREATE_REENTRANT) == DEF_YES) {
            CPU_CRITICAL_ENTER();
            p_kal_lock->OwnerPrio  = OSPrioCur;                 /* If lock is re-entrant, set prio to cur.              */
            p_kal_lock->NestingCtr = 0u;
            CPU_CRITICAL_EXIT();
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
*                                   RTOS_ERR_INVALID_ARG        Handle or options specified is invalid.
*                                   RTOS_ERR_WOULD_OVF          Semaphore would overflow.
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

    #if (OS_SEM_EN == DEF_ENABLED)
    {
        KAL_LOCK    *p_kal_lock;
        OS_EVENT    *p_sem;
        CPU_INT08U   err_os;
        CPU_SR_ALLOC();


        p_kal_lock = (KAL_LOCK *)lock_handle.LockObjPtr;
        p_sem      =  p_kal_lock->SemEventPtr;

        if (DEF_BIT_IS_SET(p_kal_lock->OptFlags, KAL_OPT_CREATE_REENTRANT) == DEF_YES) {
                                                                /* Re-entrant lock.                                     */
            CPU_CRITICAL_ENTER();
            if (p_kal_lock->NestingCtr != 0u) {
                p_kal_lock->NestingCtr--;
                CPU_CRITICAL_EXIT();
               *p_err = RTOS_ERR_NONE;
                return;
            }
                                                                /* If lock is re-entrant and is not nested, set ...     */
            p_kal_lock->OwnerPrio = KAL_LOCK_OWNER_PRIO_NONE;   /* ... OwnerPrio to 'none'.                             */
            CPU_CRITICAL_EXIT();
        }

        err_os = OSSemPost(p_sem);
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
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
*                                   RTOS_ERR_NOT_SUPPORTED      Re-entrant locks cannot be deleted.
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

    #if ((OS_SEM_EN     == DEF_ENABLED) && \
         (OS_SEM_DEL_EN == DEF_ENABLED))                        /* Sems and sems del are avail.                         */
    {
        KAL_LOCK    *p_kal_lock;
        CPU_INT08U   err_os;
        LIB_ERR      err_lib;
        CPU_SR_ALLOC();


        p_kal_lock = (KAL_LOCK *)lock_handle.LockObjPtr;

        if (DEF_BIT_IS_SET(p_kal_lock->OptFlags, KAL_OPT_CREATE_REENTRANT) == DEF_YES) {
            CPU_CRITICAL_ENTER();
            p_kal_lock->OwnerPrio  = KAL_LOCK_OWNER_PRIO_NONE;  /* If lock is re-entrant, set OwnerPrio to 'none'.      */
            p_kal_lock->NestingCtr = 0u;
            CPU_CRITICAL_EXIT();
        }
        (void)OSSemDel(p_kal_lock->SemEventPtr,
                       OS_DEL_ALWAYS,
                      &err_os);
        if (err_os == OS_ERR_NONE) {
            Mem_DynPoolBlkFree(       &KAL_DataPtr->LockPool,   /* Free rsrc to pool.                                   */
                               (void *)p_kal_lock,
                                      &err_lib);
            (void)err_lib;                                     /* Err ignored.                                         */
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return;
    }
    #else                                                       /* Sems or sems del is not avail.                       */
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
*                                   RTOS_ERR_CREATE_FAIL        Semaphore creation failed.
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

    #if (OS_SEM_EN == DEF_ENABLED)
    {
        OS_EVENT  *p_sem;


       *p_err = RTOS_ERR_NONE;

        p_sem = OSSemCreate(0u);
        if (p_sem == (OS_EVENT *)0) {
           *p_err = RTOS_ERR_CREATE_FAIL;
            return (handle);
        }

        #if (OS_EVENT_NAME_EN == DEF_ENABLED)
            if (p_name != DEF_NULL) {
                CPU_INT08U  err_os;


                OSEventNameSet(         p_sem,
                               (INT8U *)p_name,
                                       &err_os);
                (void)err_os;                                  /* Err ignored, no err can occur if OSSemCreate() is OK.*/
            }
        #else
            (void)p_name;
        #endif

        handle.SemObjPtr = (void *)p_sem;

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

    #if (OS_SEM_EN == DEF_ENABLED)
    {
        OS_EVENT    *p_sem;
        CPU_INT32U   timeout_ticks;


        p_sem = (OS_EVENT *)sem_handle.SemObjPtr;

        if (timeout_ms != KAL_TIMEOUT_INFINITE) {
            timeout_ticks = KAL_msToTicks(timeout_ms);
        } else {
            timeout_ticks = 0u;
        }

        if (DEF_BIT_IS_CLR(opt, KAL_OPT_PEND_NON_BLOCKING) == DEF_YES) {
            CPU_INT08U  err_os;


            OSSemPend(p_sem,
                      timeout_ticks,
                     &err_os);
            if (err_os == OS_ERR_NONE) {
               *p_err = RTOS_ERR_NONE;
            } else {
               *p_err = KAL_ErrConvert(err_os);
            }
        } else {
            #if (OS_SEM_ACCEPT_EN == DEF_ENABLED)               /* If OSSemAccept() is en.                              */
            {
                CPU_INT16U  ret_val;


                ret_val = OSSemAccept(p_sem);
                if (ret_val == 0u) {
                   *p_err = RTOS_ERR_WOULD_BLOCK;
                }
            }
            #else                                               /* If OSSemAccept() is dis, cannot exec operation.      */
               *p_err = RTOS_ERR_NOT_AVAIL;
            #endif
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

    #if (OS_SEM_EN == DEF_ENABLED)
    {
        OS_EVENT    *p_sem;
        CPU_INT08U   err_os;


        p_sem  = (OS_EVENT *)sem_handle.SemObjPtr;

        err_os =  OSSemPost(p_sem);
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
*                                   RTOS_ERR_INVALID_ARG        Handle or options specified is invalid.
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

    #if ((OS_SEM_EN            == DEF_ENABLED) && \
         (OS_SEM_PEND_ABORT_EN == DEF_ENABLED))                 /* Sems and sems pend abort are avail.                  */
    {
        OS_EVENT    *p_sem;
        CPU_INT08U   err_os;


        p_sem = (OS_EVENT *)sem_handle.SemObjPtr;

        (void)OSSemPendAbort(p_sem,
                             OS_PEND_OPT_BROADCAST,
                            &err_os);
        if ((err_os == OS_ERR_NONE) ||
            (err_os != OS_ERR_PEND_ABORT)) {
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return;
    }
    #else                                                       /* Sems or sems pend abort is not avail.                */
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
*                                   RTOS_ERR_INVALID_ARG        Handle or options specified is invalid.
*                                   RTOS_ERR_OS                 Tasks are waiting on semaphore. Cannot set value.
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

    #if ((OS_SEM_EN     == DEF_ENABLED) && \
         (OS_SEM_SET_EN == DEF_ENABLED))                        /* Sems and sems set are avail.                         */
    {
        CPU_INT08U  err_os;


        OSSemSet((OS_EVENT *)sem_handle.SemObjPtr,
                             cnt,
                            &err_os);
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return;
    }
    #else                                                       /* Sems or sems set is not avail.                       */
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
*                                   RTOS_ERR_INVALID_ARG        Handle or options specified is invalid.
*                                   RTOS_ERR_ISR                Function was called from an ISR.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  KAL_SemDel (KAL_SEM_HANDLE   sem_handle,
                  RTOS_ERR         *p_err)
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

    #if ((OS_SEM_EN     == DEF_ENABLED) && \
         (OS_SEM_DEL_EN == DEF_ENABLED))                        /* Sems and sems del are avail.                         */
    {
        CPU_INT08U  err_os;


        (void)OSSemDel((OS_EVENT *)sem_handle.SemObjPtr,
                                   OS_DEL_ALWAYS,
                                  &err_os);
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return;
    }
    #else                                                       /* Sems or sems del is not avail.                       */
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
*                                   RTOS_ERR_ALLOC              Unable to allocate memory for resource.
*                                   RTOS_ERR_INVALID_ARG        Invalid argument passed to function.
*                                   RTOS_ERR_ISR                Function called from an ISR context.
*                                   RTOS_ERR_NO_MORE_RSRC       OS cannot allocate timer resource.
*                                   RTOS_ERR_OS                 Generic OS error.
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

    #if (OS_TMR_EN == DEF_ENABLED)
    {
        KAL_TMR     *p_tmr;
        CPU_INT32U   tmr_dly;
        CPU_INT32U   tmr_period;
        CPU_INT08U   opt_os;
        CPU_INT08U   err_os;
        LIB_ERR      err_lib;


        p_tmr  = (KAL_TMR *)Mem_DynPoolBlkGet(&KAL_DataPtr->TmrPool,
                                              &err_lib);
        if (err_lib != LIB_MEM_ERR_NONE) {
           *p_err = RTOS_ERR_ALLOC;
            return (handle);
        }

        p_tmr->CallbackFnct = p_callback;
        p_tmr->CallbackArg  = p_callback_arg;
                                                                /* Tmr is 'periodic'.                                   */
        if ((p_cfg                                            != DEF_NULL) &&
            (DEF_BIT_IS_SET(p_cfg->Opt, KAL_OPT_TMR_PERIODIC) == DEF_YES)) {
            opt_os     = OS_TMR_OPT_PERIODIC;
            tmr_dly    = 0u;
            tmr_period = KAL_msToTicks(interval_ms);
        } else {
            opt_os     = OS_TMR_OPT_ONE_SHOT;                   /* Tmr is 'one-shot'.                                   */
            tmr_dly    = KAL_msToTicks(interval_ms);
            tmr_period = 0u;
        }

        p_tmr->TmrPtr = OSTmrCreate(              tmr_dly,      /* Create tmr obj.                                      */
                                                  tmr_period,
                                                  opt_os,
                                                  KAL_TmrFnctWrapper,
                                    (void       *)p_tmr,
                                    (CPU_INT08U *)p_name,
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
*                                   RTOS_ERR_INVALID_ARG        Invalid handle passed to function.
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

    #if (OS_TMR_EN == DEF_ENABLED)
    {
        KAL_TMR     *p_tmr;
        CPU_INT08U   err_os;


        p_tmr = (KAL_TMR *)tmr_handle.TmrObjPtr;

        (void)OSTmrStart(p_tmr->TmrPtr,
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
*                                   RTOS_ERR_ALLOC              Unable to allocate memory for queue.
*                                   RTOS_ERR_CREATE_FAIL        Queue creation failed.
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

    #if ((OS_Q_EN          == DEF_ENABLED) && \
        ((OS_Q_POST_EN     == DEF_ENABLED) || \
         (OS_Q_POST_OPT_EN == DEF_ENABLED)))                    /* Qs and at least one of the Q post fnct are avail.    */
    {
        void      **p_q_start;
        OS_EVENT   *p_q_event;
        LIB_ERR     err_lib;


        p_q_start = (void **)Mem_SegAlloc("KAL Q",
                                           KAL_DataPtr->MemSegPtr,
                                           sizeof(void *) * max_msg_qty,
                                          &err_lib);
        if (err_lib != LIB_MEM_ERR_NONE) {
           *p_err = RTOS_ERR_ALLOC;
            return (handle);
        }

        p_q_event = OSQCreate(p_q_start,
                              max_msg_qty);
        if (p_q_event == (OS_EVENT *)0) {
           *p_err = RTOS_ERR_CREATE_FAIL;
            return (handle);
        }

        #if (OS_EVENT_NAME_EN == DEF_ENABLED)
            if (p_name != DEF_NULL) {
                CPU_INT08U  err_os;


                OSEventNameSet(              p_q_event,
                               (CPU_INT08U *)p_name,
                                            &err_os);
                (void)err_os;                                  /* Err ignored, no err can occur if OSQCreate() is OK.  */
            }
        #else
            (void)p_name;
        #endif

        handle.QObjPtr = (void *)p_q_event;
       *p_err          =  RTOS_ERR_NONE;

        return (handle);
    }
    #else                                                       /* Qs or Q post are not avail.                          */
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
*                                   RTOS_ERR_TIMEOUT            Operation timed-out.
*                                   RTOS_ERR_ISR                Function was called from an ISR.
*                                   RTOS_ERR_WOULD_BLOCK        KAL_OPT_PEND_NON_BLOCKING opt specified and no
*                                                               message is available.
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

    #if ((OS_Q_EN          == DEF_ENABLED) && \
        ((OS_Q_POST_EN     == DEF_ENABLED) || \
         (OS_Q_POST_OPT_EN == DEF_ENABLED)))                    /* Qs and at least one of the Q post fnct are avail.    */
    {
        OS_EVENT    *p_q_event;
        void        *p_msg;
        CPU_INT08U   err_os;


        p_q_event = (OS_EVENT *)q_handle.QObjPtr;

        if (DEF_BIT_IS_CLR(opt, KAL_OPT_PEND_NON_BLOCKING) == DEF_YES) {
            CPU_INT32U  timeout_ticks;


                                                                /* Blocking call.                                       */
            if (timeout_ms != KAL_TIMEOUT_INFINITE) {
                timeout_ticks = KAL_msToTicks(timeout_ms);
            } else {
                timeout_ticks = 0u;
            }

            p_msg = OSQPend(p_q_event,
                            timeout_ticks,
                           &err_os);
        } else {                                                /* Non-blocking call.                                   */
            #if (OS_Q_ACCEPT_EN == DEF_ENABLED)
                p_msg = OSQAccept(p_q_event,
                                 &err_os);
            #else
               *p_err = RTOS_ERR_NOT_AVAIL;

                return (DEF_NULL);
            #endif
        }
        if (err_os == OS_ERR_NONE) {
           *p_err = RTOS_ERR_NONE;
        } else {
           *p_err = KAL_ErrConvert(err_os);
        }

        return (p_msg);
    }
    #else                                                       /* Qs or Q post are not avail.                          */
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
*                                   RTOS_ERR_NO_MORE_RSRC       Queue cannot contain any more message.
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

    #if ((OS_Q_EN          == DEF_ENABLED) && \
        ((OS_Q_POST_EN     == DEF_ENABLED) || \
         (OS_Q_POST_OPT_EN == DEF_ENABLED)))                    /* Qs and at least one of the Q post fnct are avail.    */
    {
        OS_EVENT    *p_q_event;
        CPU_INT08U   err_os;


        p_q_event = (OS_EVENT *)q_handle.QObjPtr;

        #if (OS_Q_POST_OPT_EN == DEF_ENABLED)
            err_os = OSQPostOpt(p_q_event,
                                p_msg,
                                OS_POST_OPT_NONE);
            if (err_os == OS_ERR_NONE) {
               *p_err = RTOS_ERR_NONE;
            } else {
               *p_err = KAL_ErrConvert(err_os);
            }
        #elif (OS_Q_POST_EN == DEF_ENABLED)
            err_os = OSQPost(p_q_event,
                             p_msg);
            if (err_os == OS_ERR_NONE) {
               *p_err = RTOS_ERR_NONE;
            } else {
               *p_err = KAL_ErrConvert(err_os);
            }
        #endif

        return;
    }
    #else                                                       /* Qs or Q post are not avail.                          */
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


    if (dly_ms != 0u) {
        dly_ticks = KAL_msToTicks(dly_ms);
    } else {
        dly_ticks = 0u;
    }

    OSTimeDly(dly_ticks);

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
* Note(s)     : (1) KAL_OPT_DLY_PERIODIC is not supported. Using KAL_OPT_DLY instead.
*********************************************************************************************************
*/

void  KAL_DlyTick (KAL_TICK  dly_ticks,
                   KAL_OPT   opt)
{
    (void)opt;

    OSTimeDly(dly_ticks);

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

    #if (OS_TASK_REG_TBL_SIZE > 0u)
    {
        KAL_TASK_REG  *p_task_reg;
        CPU_INT08U     err_os;
        LIB_ERR        err_lib;


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

    #if (OS_TASK_REG_TBL_SIZE > 0u)
    {
        KAL_TASK_REG  *p_task_reg;
        CPU_INT08U     task_prio;
        CPU_INT32U     ret_val;
        CPU_INT08U     err_os;


        p_task_reg = (KAL_TASK_REG *)task_reg_handle.TaskRegObjPtr;

        if (KAL_TASK_HANDLE_IS_NULL(task_handle) == DEF_YES) {
            task_prio = OS_PRIO_SELF;                           /* Use cur task if no task handle is provided.          */
        } else {
                                                                /* Get prio from task handle provided.                  */
            task_prio = ((KAL_TASK *)task_handle.TaskObjPtr)->Prio;
        }

        ret_val = OSTaskRegGet(task_prio,
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

    #if (OS_TASK_REG_TBL_SIZE > 0u)
    {
        KAL_TASK_REG  *p_task_reg;
        CPU_INT08U     task_prio;
        CPU_INT08U     err_os;


        p_task_reg = (KAL_TASK_REG *)task_reg_handle.TaskRegObjPtr;

        if (KAL_TASK_HANDLE_IS_NULL(task_handle) == DEF_YES) {
            task_prio = OS_PRIO_SELF;                           /* Use cur task if no task handle is provided.          */
        } else {
                                                                /* Get prio from task handle provided.                  */
            task_prio = ((KAL_TASK *)task_handle.TaskObjPtr)->Prio;
        }

        OSTaskRegSet(task_prio,
                     p_task_reg->Id,
                     val,
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
*                                   RTOS_ERR_NOT_AVAIL          Configuration does not allow operation.
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

    #if (OS_TIME_GET_SET_EN == DEF_ENABLED)
    {
        KAL_TICK  tick_cnt;


        tick_cnt = OSTimeGet();

       *p_err = RTOS_ERR_NONE;

        return (tick_cnt);
    }
    #else
       *p_err = RTOS_ERR_NOT_AVAIL;

        return (0u);
    #endif
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

#if (OS_TMR_EN == DEF_ENABLED)
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
#if  ((OS_TICKS_PER_SEC          >= 1000u) && \
      (OS_TICKS_PER_SEC %  1000u ==    0u))
    const  CPU_INT08U  mult = OS_TICKS_PER_SEC / 1000u;
#endif


    #if  ((OS_TICKS_PER_SEC          >= 1000u) && \
          (OS_TICKS_PER_SEC %  1000u ==    0u))                 /* Optimize calc if possible for often used vals.       */
        ticks =    ms * mult;
    #elif (OS_TICKS_PER_SEC ==  100u)
        ticks =  ((ms +  9u) /  10u);
    #elif (OS_TICKS_PER_SEC ==   10u)
        ticks =  ((ms + 99u) / 100u);
    #else                                                       /* General formula.                                     */
        ticks = (((ms * OS_TICKS_PER_SEC)  + 1000u - 1u) / 1000u);
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

static  RTOS_ERR  KAL_ErrConvert (CPU_INT08U  err_os)
{
    RTOS_ERR  err_rtos;


    switch (err_os) {
        case OS_ERR_NONE:
             err_rtos = RTOS_ERR_NONE;
             break;


        case OS_ERR_EVENT_TYPE:
        case OS_ERR_INVALID_OPT:
        case OS_ERR_ID_INVALID:
        case OS_ERR_PNAME_NULL:
        case OS_ERR_PRIO_EXIST:
        case OS_ERR_PRIO_INVALID:
        case OS_ERR_TASK_DEL:
        case OS_ERR_TASK_DEL_IDLE:
        case OS_ERR_TASK_NOT_EXIST:
        case OS_ERR_TMR_INACTIVE:
        case OS_ERR_TMR_INVALID_DLY:
        case OS_ERR_TMR_INVALID_TYPE:
        case OS_ERR_TMR_INVALID:
             err_rtos = RTOS_ERR_INVALID_ARG;
             break;


        case OS_ERR_PEVENT_NULL:
             err_rtos = RTOS_ERR_NULL_PTR;
             break;


        case OS_ERR_PEND_LOCKED:
        case OS_ERR_TASK_WAITING:
        case OS_ERR_TMR_INVALID_PERIOD:
        case OS_ERR_TMR_INVALID_OPT:
        case OS_ERR_TMR_INVALID_STATE:
             err_rtos = RTOS_ERR_OS;
             break;


        case OS_ERR_TIMEOUT:
             err_rtos = RTOS_ERR_TIMEOUT;
             break;


        case OS_ERR_PEND_ABORT:
             err_rtos = RTOS_ERR_ABORT;
             break;


        case OS_ERR_Q_EMPTY:
             err_rtos = RTOS_ERR_WOULD_BLOCK;
             break;


        case OS_ERR_PEND_ISR:
        case OS_ERR_DEL_ISR:
        case OS_ERR_NAME_SET_ISR:
        case OS_ERR_TASK_CREATE_ISR:
        case OS_ERR_TASK_DEL_ISR:
        case OS_ERR_TMR_ISR:
             err_rtos = RTOS_ERR_ISR;
             break;


        case OS_ERR_SEM_OVF:
             err_rtos = RTOS_ERR_WOULD_OVF;
             break;


        case OS_ERR_Q_FULL:
        case OS_ERR_NO_MORE_ID_AVAIL:
        case OS_ERR_TMR_NON_AVAIL:
             err_rtos = RTOS_ERR_NO_MORE_RSRC;
             break;


        default:
             err_rtos = RTOS_ERR_OS;
             break;
    }

    return (err_rtos);
}
