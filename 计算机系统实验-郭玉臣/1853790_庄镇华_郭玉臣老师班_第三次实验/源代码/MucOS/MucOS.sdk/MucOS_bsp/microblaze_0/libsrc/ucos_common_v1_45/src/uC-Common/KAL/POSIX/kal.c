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
*                                            POSIX Threads
*
* Filename : kal.c
* Version  : V1.01.02
*********************************************************************************************************
* Notes    : (1) Requires a Single UNIX Specification, Version 3 compliant operating environment.
*                On Linux _XOPEN_SOURCE must be defined to at least 600, generally by passing the
*                -D_XOPEN_SOURCE=600 command line option to GCC.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define   MICRIUM_SOURCE
#define   KAL_MODULE

#include  "../kal.h"

#include  <lib_def.h>

#include  <pthread.h>
#include  <semaphore.h>


/*
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*/

#define KAL_CFG_ARG_CHK_EXT_EN  DEF_ENABLED

/*
*********************************************************************************************************
*                                            LOCAL CONSTANTS
*********************************************************************************************************
*/

KAL_CPP_EXT  const  KAL_TASK_HANDLE      KAL_TaskHandleNull    = {DEF_NULL};
KAL_CPP_EXT  const  KAL_LOCK_HANDLE      KAL_LockHandleNull    = {DEF_NULL};
KAL_CPP_EXT  const  KAL_SEM_HANDLE       KAL_SemHandleNull     = {DEF_NULL};
KAL_CPP_EXT  const  KAL_Q_HANDLE         KAL_QHandleNull       = {DEF_NULL};
KAL_CPP_EXT  const  KAL_TMR_HANDLE       KAL_TmrHandleNull     = {DEF_NULL};
KAL_CPP_EXT  const  KAL_TASK_REG_HANDLE  KAL_TaskRegHandleNull = {DEF_NULL};


/*
*********************************************************************************************************
*                                            LOCAL DATA TYPES
*********************************************************************************************************
*/

typedef  struct  kal_data {
    MEM_SEG       *MemSegPtr;

    MEM_DYN_POOL   TaskPool;
    MEM_DYN_POOL   LockPool;
    MEM_DYN_POOL   SemPool;
} KAL_DATA;

typedef  struct  kal_lock {
    pthread_mutex_t      Mutex;
    pthread_mutexattr_t  MutexAttr;
} KAL_LOCK;


typedef  struct  kal_sem {
    sem_t      Sem;
} KAL_SEM;

typedef  struct  kal_task {
    pthread_t       Thread;
    pthread_attr_t  ThreadAttr;
} KAL_TASK;

typedef  struct  kal_task_fnct_info {
    void  (*Fnct)(void  *p_arg);
    void   *ArgPtr;
    sem_t   SemInit;
} KAL_TASK_FNCT_INFO;


/*
*********************************************************************************************************
*                                              LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  KAL_DATA  *KAL_DataPtr = DEF_NULL;


/*
*********************************************************************************************************
*                                       LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  *KAL_TaskFnctWrapper(void  *p_arg);


/*
*********************************************************************************************************
*                                       LOCAL CONFIGURATION ERRORS
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
*                                             KAL_Init()
*
* Description : Initialize the Kernel Abstraction Layer.
*
* Note(s)     : (1) This function must be called prior to any product initialization function if the
*                   application needs to specify a memory segment
*********************************************************************************************************
*/

void  KAL_Init (KAL_CFG  *p_cfg,
                KAL_ERR  *p_err)
{
    LIB_ERR   err_lib;
    MEM_SEG  *p_seg ;


#if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                     /* ---------------- VALIDATE ARGUMENTS ---------------- */
    if (p_err == DEF_NULL) {                                    /* Validate err ptr.                                    */
        CPU_SW_EXCEPTION(DEF_NULL);
    }
#endif                                                          /* ----- (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED) ------ */

    if (KAL_DataPtr != (KAL_DATA *)0) {                         /* Chk if KAL already init. See note (1).               */
       *p_err = KAL_ERR_NONE;
        return;
    }

                                                                /* ------------------ ALLOC KAL DATA ------------------ */
    p_seg = DEF_NULL;
    if (p_cfg != DEF_NULL) {                                    /* Load cfg if given.                                   */
        KAL_DataPtr->MemSegPtr = p_cfg->MemSegPtr;
    }

    KAL_DataPtr = (KAL_DATA *)Mem_SegAlloc("KAL internal data",
                                            p_seg,
                                            sizeof(KAL_DATA),
                                           &err_lib);
    if (err_lib != LIB_MEM_ERR_NONE) {
       *p_err = KAL_ERR_MEM_ALLOC;
        return;
    }

    KAL_DataPtr->MemSegPtr = p_seg;

                                                                /* ----------------- CREATE TASK POOL ----------------- */
    Mem_DynPoolCreate("KAL task pool",
                     &KAL_DataPtr->TaskPool,
                      KAL_DataPtr->MemSegPtr,
                      sizeof(KAL_TASK),
                      sizeof(CPU_ALIGN),
                      0u,
                      LIB_MEM_BLK_QTY_UNLIMITED,
                     &err_lib);
    if (err_lib != LIB_MEM_ERR_NONE) {
       *p_err = KAL_ERR_POOL_INIT;
        return;
    }

                                                                /* ----------------- CREATE LOCK POOL ----------------- */
    Mem_DynPoolCreate("KAL lock pool",
                     &KAL_DataPtr->LockPool,
                      KAL_DataPtr->MemSegPtr,
                      sizeof(KAL_LOCK),
                      sizeof(CPU_ALIGN),
                      0u,
                      LIB_MEM_BLK_QTY_UNLIMITED,
                     &err_lib);
    if (err_lib != LIB_MEM_ERR_NONE) {
       *p_err = KAL_ERR_POOL_INIT;
        return;
    }

                                                                /* ----------------- CREATE LOCK POOL ----------------- */
    Mem_DynPoolCreate("KAL sem pool",
                     &KAL_DataPtr->SemPool,
                      KAL_DataPtr->MemSegPtr,
                      sizeof(KAL_SEM),
                      sizeof(CPU_ALIGN),
                      0u,
                      LIB_MEM_BLK_QTY_UNLIMITED,
                      &err_lib);
    if (err_lib != LIB_MEM_ERR_NONE) {
        *p_err = KAL_ERR_POOL_INIT;
        return;
    }


   *p_err = KAL_ERR_NONE;
    return;
}


/*
*********************************************************************************************************
*                                         KAL_FeatureQuery()
*
* Description : Query a feature and make sure it is available.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  KAL_FeatureQuery (KAL_FEATURE  feature,
                               KAL_OPT      opt)
{
    switch (feature) {
        case KAL_FEATURE_LOCK_CREATE:
        case KAL_FEATURE_LOCK_DEL:
             return (DEF_YES);


        default:
             return (DEF_NO);
    }
}


/*
*********************************************************************************************************
*                                         TASK API FUNCTIONS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           KAL_TaskAlloc()
*
* Description : Allocate a task object and its stack.
*
* Note(s)     : None.
*********************************************************************************************************
*/

KAL_TASK_HANDLE  KAL_TaskAlloc (const  CPU_CHAR          *p_name,
                                       void              *p_stk_base,
                                       CPU_SIZE_T         stk_size_word,
                                       KAL_TASK_EXT_CFG  *p_cfg,
                                       KAL_ERR           *p_err)
{
    KAL_TASK_HANDLE   handle = {DEF_NULL};
    KAL_TASK         *p_task_data;
    LIB_ERR           lib_err;
    int               pthread_err;

#if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                     /* ---------------- VALIDATE ARGUMENTS ---------------- */
    if (p_err == DEF_NULL) {                                    /* Validate err ptr.                                    */
        CPU_SW_EXCEPTION(handle);
    }

    if (p_cfg != DEF_NULL) {                                    /* Chk for unimpl cfg specified.                        */
       *p_err = KAL_ERR_UNIMPLEMENTED;
        return (handle);
    }

    if (p_stk_base != DEF_NULL) {                               /* Chk for unimpl user-specified stk base.              */
       *p_err = KAL_ERR_UNIMPLEMENTED;
        return (handle);
    }
#endif

    p_task_data = (KAL_TASK *)Mem_DynPoolBlkGet(&KAL_DataPtr->TaskPool,
                                                     &lib_err);
    if (lib_err != LIB_MEM_ERR_NONE) {
       *p_err = KAL_ERR_MEM_ALLOC;
        return (handle);
    }

                                                                /* ------------------ SET MUTEX ATTR ------------------ */
    pthread_err = pthread_attr_init(&p_task_data->ThreadAttr);
    if (pthread_err != 0) {
       *p_err = KAL_ERR_CREATE;
        return (handle);
    }


                                                                /* Turn off sched param inheritance.                    */
    pthread_err = pthread_attr_setinheritsched(&p_task_data->ThreadAttr, PTHREAD_EXPLICIT_SCHED);
    if (pthread_err != 0) {
       *p_err = KAL_ERR_CREATE;
        return (handle);
    }

                                                                /* Sel round-robin sched policy for equal-prio tasks    */
    pthread_err = pthread_attr_setschedpolicy(&p_task_data->ThreadAttr, SCHED_RR);
    if (pthread_err != 0) {
       *p_err = KAL_ERR_CREATE;
        return (handle);
    }

                                                                /* Set stk size.                                        */
    pthread_err = pthread_attr_setstacksize(&p_task_data->ThreadAttr, stk_size_word * sizeof(CPU_DATA));
    if (pthread_err != 0) {
       *p_err = KAL_ERR_CREATE;
        return (handle);
    }

    handle.TaskObjPtr = (void *)p_task_data;
   *p_err = KAL_ERR_NONE;
    return (handle);
}


/*
*********************************************************************************************************
*                                          KAL_TaskSizeGet()
*
* Description : Get the size in memory of a task.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_SIZE_T  KAL_TaskSizeGet (CPU_SIZE_T         stk_size_word,
                             KAL_TASK_EXT_CFG  *p_cfg,
                             KAL_ERR           *p_err)
{
   *p_err = KAL_ERR_UNIMPLEMENTED;
    return (0);
}


/*
*********************************************************************************************************
*                                          KAL_TaskCreate()
*
* Description : Create a task.
*
* Note(s)     : (1) The task must be allocated prior to this call using KAL_TaskAlloc().
*********************************************************************************************************
*/

void  KAL_TaskCreate (KAL_TASK_HANDLE    task_handle,
                      void             (*p_fnct)(void *),
                      void              *p_arg,
                      CPU_INT08U         prio,
                      KAL_TASK_EXT_CFG  *p_cfg,
                      KAL_ERR           *p_err)
{
    KAL_TASK        *p_task_data;
    KAL_TASK_FNCT_INFO    fnct_info;
    int                   pthread_err;
    struct  sched_param   param;


#if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                     /* ---------------- VALIDATE ARGUMENTS ---------------- */
    if (p_err == DEF_NULL) {                                    /* Validate err ptr.                                    */
        CPU_SW_EXCEPTION(DEF_NULL);
    }

    if (task_handle.TaskObjPtr == DEF_NULL) {                   /* Chk for NULL obj ptr.                                */
       *p_err = KAL_ERR_NULL_PTR;
        return;
    }

    if (p_cfg != DEF_NULL) {                                    /* Chk for invalid opt flag.                            */
       *p_err = KAL_ERR_UNIMPLEMENTED;
        return;
    }
#endif

    p_task_data = (KAL_TASK *)task_handle.TaskObjPtr;

                                                                /* ----------------- SET THREAD ATTRS ----------------- */
    param.sched_priority = 99 - prio;
    pthread_err = pthread_attr_setschedparam(&p_task_data->ThreadAttr, &param);
    if (pthread_err != 0) {
       *p_err = KAL_ERR_CREATE;
        return;
    }


                                                                /* ------------------ CREATE THREAD ------------------- */
    fnct_info.Fnct  = p_fnct;
    fnct_info.ArgPtr = p_arg;

    pthread_err = sem_init(&fnct_info.SemInit, 0u, 0u);
    if (pthread_err != 0) {
       *p_err = KAL_ERR_CREATE;
        return;
    }

    pthread_err = pthread_create(&p_task_data->Thread,
                                 &p_task_data->ThreadAttr,
                                  KAL_TaskFnctWrapper,
                         (void *)&fnct_info);
    if (pthread_err != 0) {
       *p_err = KAL_ERR_CREATE;
        return;
    }

    pthread_err = sem_wait(&fnct_info.SemInit);
    if (pthread_err != 0) {
       *p_err = KAL_ERR_CREATE;
        return;
    }

   *p_err = KAL_ERR_NONE;
}


/*
*********************************************************************************************************
*                                            KAL_TaskDel()
*
* Description : Delete a task.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  KAL_TaskDel (KAL_TASK_HANDLE   task_handle,
                   KAL_ERR          *p_err)
{
   *p_err = KAL_ERR_UNIMPLEMENTED;
}


/*
*********************************************************************************************************
*                                         LOCK API FUNCTIONS
*********************************************************************************************************
*/

KAL_LOCK_HANDLE  KAL_LockCreate (const  CPU_CHAR          *p_name,
                                        KAL_LOCK_EXT_CFG  *p_cfg,
                                        KAL_ERR           *p_err)
{
    KAL_LOCK_HANDLE   handle = KAL_LockHandleNull;
    KAL_LOCK    *p_lock_data;
    LIB_ERR           lib_err;
    int               pthread_err;


#if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                     /* ---------------- VALIDATE ARGUMENTS ---------------- */
    if (p_err == DEF_NULL) {                                    /* Validate err ptr.                                    */
        CPU_SW_EXCEPTION(handle);
    }

    if ((p_cfg != DEF_NULL) &&                                  /* Chk for invalid opt flags.                           */
        ((p_cfg->Opt & ~(KAL_OPT_CREATE_REENTRANT)) != 0u)) {
       *p_err = KAL_ERR_INVALID_ARG;
        return (handle);
    }
#endif

    p_lock_data = (KAL_LOCK *)Mem_DynPoolBlkGet(&KAL_DataPtr->LockPool,
                                                     &lib_err);
    if (lib_err != LIB_MEM_ERR_NONE) {
       *p_err = KAL_ERR_MEM_ALLOC;
        return (handle);
    }

                                                                /* ------------------ SET MUTEX ATTR ------------------ */
    pthread_err = pthread_mutexattr_init(&p_lock_data->MutexAttr);
    if (pthread_err != 0) {
       *p_err = KAL_ERR_CREATE;
        goto fail_release_blk;
    }

    if (p_cfg != DEF_NULL) {                                    /* Set attr according to cfg.                           */
        if (DEF_BIT_IS_SET(p_cfg->Opt, KAL_OPT_CREATE_REENTRANT)) {
            pthread_err = pthread_mutexattr_settype(&p_lock_data->MutexAttr, PTHREAD_MUTEX_RECURSIVE);
            if (pthread_err != 0) {
               *p_err = KAL_ERR_CREATE;
                goto fail_release_attr;
            }
        }
    }

                                                                /* -------------------- INIT MUTEX -------------------- */
    pthread_err = pthread_mutex_init(&p_lock_data->Mutex, &p_lock_data->MutexAttr);
    if (pthread_err != 0) {
       *p_err = KAL_ERR_CREATE;
        goto fail_release_attr;
    }

    handle.LockObjPtr = p_lock_data;

   *p_err = KAL_ERR_NONE;
    return (handle);


fail_release_attr:
    pthread_err = pthread_mutexattr_destroy(&p_lock_data->MutexAttr);
    if (pthread_err != 0) {
        CPU_SW_EXCEPTION(handle);
    }

fail_release_blk:
    Mem_DynPoolBlkFree(&KAL_DataPtr->LockPool, (void *)p_lock_data, &lib_err);
    if (lib_err != LIB_MEM_ERR_NONE) {
        CPU_SW_EXCEPTION(handle);
    }

    return (handle);
}


CPU_SIZE_T  KAL_LockSizeGet (KAL_LOCK_EXT_CFG  *p_cfg,
                             KAL_ERR           *p_err)
{
   *p_err = KAL_ERR_NONE;
    return (sizeof(KAL_LOCK));
}


void  KAL_LockAcquire (KAL_LOCK_HANDLE   lock_handle,
                       KAL_OPT           opt,
                       CPU_INT32U        timeout,
                       KAL_ERR          *p_err)
{
    KAL_LOCK  *p_lock_data;
    int             pthread_err;


#if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                     /* ---------------- VALIDATE ARGUMENTS ---------------- */
    if (p_err == DEF_NULL) {                                    /* Validate err ptr.                                    */
        CPU_SW_EXCEPTION(DEF_NULL);
    }

    if (lock_handle.LockObjPtr == DEF_NULL) {                   /* Chk for NULL obj ptr.                                */
       *p_err = KAL_ERR_NULL_PTR;
        return;
    }

    if (opt != 0u) {                                            /* Chk for invalid opt flag.                            */
       *p_err = KAL_ERR_INVALID_ARG;
        return;
    }
#endif

    p_lock_data = (KAL_LOCK *)lock_handle.LockObjPtr;

    if (timeout == 0u) {
        pthread_err = pthread_mutex_lock(&p_lock_data->Mutex);
        if (pthread_err != 0) {
           *p_err = KAL_ERR_OS;
            return;
        }
    } else {
        struct  timespec  ts_timeout;


        ts_timeout.tv_sec  =  timeout / 1000u;
        ts_timeout.tv_nsec = (timeout % 1000u) * 1000u;

        pthread_err = pthread_mutex_timedlock(&p_lock_data->Mutex, &ts_timeout);
        if (pthread_err != 0) {
           *p_err = KAL_ERR_OS;
            return;
        }
    }

   *p_err = KAL_ERR_NONE;
}


void  KAL_LockRelease (KAL_LOCK_HANDLE   lock_handle,
                       KAL_ERR          *p_err)
{
    KAL_LOCK  *p_lock_data;
    int             pthread_err;


#if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                     /* ---------------- VALIDATE ARGUMENTS ---------------- */
    if (p_err == DEF_NULL) {                                    /* Validate err ptr.                                    */
        CPU_SW_EXCEPTION(DEF_NULL);
    }

    if (lock_handle.LockObjPtr == DEF_NULL) {                   /* Chk for NULL obj ptr.                                */
       *p_err = KAL_ERR_NULL_PTR;
        return;
    }
#endif

    p_lock_data = (KAL_LOCK *)lock_handle.LockObjPtr;

    pthread_err = pthread_mutex_unlock(&p_lock_data->Mutex);
    if (pthread_err != 0) {
       *p_err = KAL_ERR_OS;
        return;
    }

   *p_err = KAL_ERR_NONE;
}


void  KAL_LockDel (KAL_LOCK_HANDLE   lock_handle,
                   KAL_ERR          *p_err)
{
    KAL_LOCK  *p_lock_data;
    int             pthread_err;
    LIB_ERR         lib_err;


    p_lock_data = (KAL_LOCK *)lock_handle.LockObjPtr;
    if (p_lock_data == DEF_NULL) {
       *p_err = KAL_ERR_NULL_PTR;
        return;
    }

    pthread_err = pthread_mutexattr_destroy(&p_lock_data->MutexAttr);
    if (pthread_err != 0) {
        CPU_SW_EXCEPTION(;);
    }

    pthread_err = pthread_mutex_destroy(&p_lock_data->Mutex);
    if (pthread_err != 0) {
        CPU_SW_EXCEPTION(;);
    }

    Mem_DynPoolBlkFree(&KAL_DataPtr->LockPool, (void *)p_lock_data, &lib_err);
    if (lib_err != LIB_MEM_ERR_NONE) {
        CPU_SW_EXCEPTION(;);
    }

   *p_err = KAL_ERR_NONE;
}

/*
*********************************************************************************************************
*                                          SEM API FUNCTIONS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          KAL_SemCreate()
*
* Description : Create a semaphore.
*
* Note(s)     : None.
*********************************************************************************************************
*/

KAL_SEM_HANDLE  KAL_SemCreate (const  CPU_CHAR         *p_name,
                                      KAL_SEM_EXT_CFG  *p_cfg,
                                      KAL_ERR          *p_err)
{
    KAL_SEM_HANDLE   handle = KAL_SemHandleNull;
    KAL_SEM    *p_sem_data;
    LIB_ERR          lib_err;
    int              sem_err;


#if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                     /* ---------------- VALIDATE ARGUMENTS ---------------- */
    if (p_err == DEF_NULL) {                                    /* Validate err ptr.                                    */
        CPU_SW_EXCEPTION(handle);
    }

    if (p_cfg != DEF_NULL) {                                    /* Chk for invalid opt flags.                           */
       *p_err = KAL_ERR_INVALID_ARG;
        return (handle);
    }
#endif

    p_sem_data = (KAL_SEM *)Mem_DynPoolBlkGet(&KAL_DataPtr->SemPool,
                                                   &lib_err);
    if (lib_err != LIB_MEM_ERR_NONE) {
       *p_err = KAL_ERR_MEM_ALLOC;
        return (handle);
    }

                                                                /* --------------------- INIT SEM --------------------- */
    sem_err = sem_init(&p_sem_data->Sem, 0u, 0u);
    if (sem_err != 0) {
      *p_err = KAL_ERR_CREATE;
       Mem_DynPoolBlkFree(&KAL_DataPtr->SemPool, (void *)p_sem_data, &lib_err);
       if (lib_err != LIB_MEM_ERR_NONE) {
           CPU_SW_EXCEPTION(handle);
       }
       return (handle);
    }

    handle.SemObjPtr = p_sem_data;

   *p_err = KAL_ERR_NONE;
    return (handle);
}


/*
*********************************************************************************************************
*                                          KAL_SemSizeGet()
*
* Description : Get the size of a semaphore.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_SIZE_T  KAL_SemSizeGet (KAL_SEM_EXT_CFG  *p_cfg,
                            KAL_ERR          *p_err)
{
    *p_err = KAL_ERR_NONE;
     return (sizeof(KAL_SEM));
}


/*
*********************************************************************************************************
*                                            KAL_SemPend()
*
* Description : Pend on a semaphore.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  KAL_SemPend (KAL_SEM_HANDLE   sem_handle,
                   KAL_OPT          opt,
                   CPU_INT32U       timeout,
                   KAL_ERR         *p_err)
{
    KAL_SEM  *p_sem_data;
    int            sem_err;


#if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                     /* ---------------- VALIDATE ARGUMENTS ---------------- */
    if (p_err == DEF_NULL) {                                    /* Validate err ptr.                                    */
        CPU_SW_EXCEPTION(DEF_NULL);
    }

    if (sem_handle.SemObjPtr == DEF_NULL) {                     /* Chk for NULL obj ptr.                                */
       *p_err = KAL_ERR_NULL_PTR;
        return;
    }

    if (opt != 0u) {                                            /* Chk for invalid opt flag.                            */
       *p_err = KAL_ERR_INVALID_ARG;
        return;
    }
#endif

    p_sem_data = (KAL_SEM *)sem_handle.SemObjPtr;

    if (timeout == 0u) {
        sem_err = sem_wait(&p_sem_data->Sem);
        if (sem_err != 0) {
           *p_err = KAL_ERR_OS;
            return;
        }
    } else {
        struct  timespec  ts_timeout;


        ts_timeout.tv_sec  =  timeout / 1000u;
        ts_timeout.tv_nsec = (timeout % 1000u) * 1000u;

        sem_err = sem_timedwait(&p_sem_data->Sem, &ts_timeout);
        if (sem_err != 0) {
           *p_err = KAL_ERR_OS;
            return;
        }
    }

   *p_err = KAL_ERR_NONE;
}


/*
*********************************************************************************************************
*                                            KAL_SemPost()
*
* Description : Post a semaphore.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  KAL_SemPost (KAL_SEM_HANDLE   sem_handle,
                   KAL_OPT          opt,
                   KAL_ERR         *p_err)
{
    KAL_SEM  *p_sem_data;
    int            sem_err;


#if (KAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                     /* ---------------- VALIDATE ARGUMENTS ---------------- */
    if (p_err == DEF_NULL) {                                    /* Validate err ptr.                                    */
        CPU_SW_EXCEPTION(DEF_NULL);
    }

    if (sem_handle.SemObjPtr == DEF_NULL) {                     /* Chk for NULL obj ptr.                                */
       *p_err = KAL_ERR_NULL_PTR;
        return;
    }

    if (opt != KAL_OPT_NONE) {                                  /* Chk for invalid opt flags.                           */
       *p_err = KAL_ERR_INVALID_ARG;
        return;
    }
#endif

    p_sem_data = (KAL_SEM *)sem_handle.SemObjPtr;

    sem_err = sem_post(&p_sem_data->Sem);
    if (sem_err != 0) {
       *p_err = KAL_ERR_OS;
        return;
    }

   *p_err = KAL_ERR_NONE;
}


/*
*********************************************************************************************************
*                                         KAL_SemPendAbort()
*
* Description : Abort given semaphore and resume all the tasks pending on it.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  KAL_SemPendAbort (KAL_SEM_HANDLE   sem_handle,
                        KAL_ERR         *p_err)
{
   *p_err = KAL_ERR_UNIMPLEMENTED;
}


/*
*********************************************************************************************************
*                                            KAL_SemSet()
*
* Description : Set count of given semaphore.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  KAL_SemSet (KAL_SEM_HANDLE   sem_handle,
                  CPU_INT16U       count,
                  KAL_ERR         *p_err)
{
   *p_err = KAL_ERR_UNIMPLEMENTED;
}


/*
*********************************************************************************************************
*                                            KAL_SemDel()
*
* Description : Delete given semaphore.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  KAL_SemDel (KAL_SEM_HANDLE   sem_handle,
                  KAL_ERR         *p_err)
{
    KAL_SEM  *p_sem_data;
    int            sem_err;
    LIB_ERR        err_lib;


    p_sem_data = (KAL_SEM *)sem_handle.SemObjPtr;

    sem_err = sem_destroy(&p_sem_data->Sem);
    if (sem_err != 0) {
        CPU_SW_EXCEPTION(;);
    }

    Mem_DynPoolBlkFree(&KAL_DataPtr->SemPool, (void *)p_sem_data, &err_lib);
    if (err_lib != LIB_MEM_ERR_NONE) {
        CPU_SW_EXCEPTION(;);
    }

   *p_err = KAL_ERR_NONE;
}


/*
*********************************************************************************************************
*                                          TMR API FUNCTIONS
*********************************************************************************************************
*/

KAL_TMR_HANDLE  KAL_TmrCreate (const  CPU_CHAR         *p_name,
                                      void            (*p_callback)(void *),
                                      void             *p_callback_arg,
                                      CPU_INT32U        period_ms,
                                      KAL_TMR_EXT_CFG  *p_cfg,
                                      KAL_ERR          *p_err)
{
    KAL_TMR_HANDLE  handle = {DEF_NULL};


   *p_err = KAL_ERR_UNIMPLEMENTED;
    return (handle);
}


CPU_SIZE_T  KAL_TmrSizeGet (void            (*p_callback)(void *),
                            void             *p_callback_arg,
                            CPU_INT32U        delay_ms,
                            KAL_TMR_EXT_CFG  *p_cfg,
                            KAL_ERR          *p_err)
{
   *p_err = KAL_ERR_UNIMPLEMENTED;
    return (0);
}


void  KAL_TmrStart (KAL_TMR_HANDLE   tmr,
                    KAL_ERR         *p_err)
{
   *p_err = KAL_ERR_UNIMPLEMENTED;
}


/*
*********************************************************************************************************
*                                           Q API FUNCTIONS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            KAL_QCreate()
*
* Description : Create a message queue.
*
* Note(s)     : None.
*********************************************************************************************************
*/

KAL_Q_HANDLE  KAL_QCreate (const  CPU_CHAR       *p_name,
                                  KAL_MSG_QTY     max_msg_qty,
                                  KAL_Q_EXT_CFG  *p_cfg,
                                  KAL_ERR        *p_err)
{
    KAL_Q_HANDLE  handle = {DEF_NULL};

   *p_err = KAL_ERR_UNIMPLEMENTED;
    return (handle);
}


/*
*********************************************************************************************************
*                                           KAL_QSizeGet()
*
* Description : Get message queue size.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_SIZE_T  KAL_QSizeGet (KAL_MSG_QTY     max_msg_qty,
                          KAL_Q_EXT_CFG  *p_cfg,
                          KAL_ERR        *p_err)
{
   *p_err = KAL_ERR_UNIMPLEMENTED;
    return (0);
}


/*
*********************************************************************************************************
*                                             KAL_QPost()
*
* Description : Post message on queue.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  KAL_QPost (KAL_Q_HANDLE   q_handle,
                 void          *p_msg,
                 KAL_OPT        opt,
                 KAL_ERR       *p_err)
{
   *p_err = KAL_ERR_UNIMPLEMENTED;
}


/*
*********************************************************************************************************
*                                             KAL_QPend()
*
* Description : Pend/get first message of queue.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  *KAL_QPend (KAL_Q_HANDLE   q_handle,
                  KAL_OPT        opt,
                  CPU_INT32U     timeout,
                  KAL_ERR       *p_err)
{
   *p_err = KAL_ERR_UNIMPLEMENTED;
    return (DEF_NULL);
}


/*
*********************************************************************************************************
*                                             KAL_QDel()
*
* Description : Delay current task.
*
* Note(s)     : If someone ever wants to implement this function, the KAL_QCreate() function will have
*               to be modified in order to allocate the Qs from a dynamic memory pool.
*********************************************************************************************************
*/

void  KAL_QDel (KAL_Q_HANDLE   q_handle,
                KAL_ERR       *p_err)
{
   *p_err = KAL_ERR_UNIMPLEMENTED;
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
* Description : Delay current task.
*
* Notes(s)    : None.
*********************************************************************************************************
*/

void  KAL_Dly (CPU_INT32U  dly_ms)
{
    struct timespec time;


    time.tv_sec  = dly_ms/1000;
    dly_ms      %= 1000u;
    time.tv_nsec = dly_ms * 1000 * 1000;

    if (nanosleep(&time, DEF_NULL) != 0) {
        CPU_SW_EXCEPTION(;);
    }
}


/*
*********************************************************************************************************
*                                       TASK REG API FUNCTIONS
*********************************************************************************************************
*/

KAL_TASK_REG_HANDLE  KAL_TaskRegCreate (KAL_TASK_REG_EXT_CFG  *p_cfg,
                                        KAL_ERR               *p_err)
{
    KAL_TASK_REG_HANDLE  handle = {DEF_NULL};

   *p_err = KAL_ERR_UNIMPLEMENTED;
    return (handle);
}


CPU_SIZE_T  KAL_TaskRegSizeGet (KAL_TASK_REG_EXT_CFG  *p_cfg,
                                KAL_ERR               *p_err)
{
   *p_err = KAL_ERR_UNIMPLEMENTED;
    return (0);
}


CPU_INT32U  KAL_TaskRegGet (KAL_TASK_HANDLE       task_handle,
                            KAL_TASK_REG_HANDLE   task_reg,
                            KAL_ERR              *p_err)
{
   *p_err = KAL_ERR_UNIMPLEMENTED;
    return ((CPU_INT32U)-1);
}


void  KAL_TaskRegSet (KAL_TASK_HANDLE       task_handle,
                      KAL_TASK_REG_HANDLE   task_reg_handle,
                      CPU_INT32U            value,
                      KAL_ERR              *p_err)
{
   *p_err = KAL_ERR_UNIMPLEMENTED;
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

void  *KAL_TaskFnctWrapper(void  *p_arg)
{
    KAL_TASK_FNCT_INFO  *p_fnct_info;
    void (*fnct)(void *);
    void  *p_fnct_arg;


    p_fnct_info = (KAL_TASK_FNCT_INFO *)p_arg;
    fnct        = p_fnct_info->Fnct;
    p_fnct_arg  = p_fnct_info->ArgPtr;

    sem_post(&p_fnct_info->SemInit);

    fnct(p_fnct_arg);

    return (DEF_NULL);
}


