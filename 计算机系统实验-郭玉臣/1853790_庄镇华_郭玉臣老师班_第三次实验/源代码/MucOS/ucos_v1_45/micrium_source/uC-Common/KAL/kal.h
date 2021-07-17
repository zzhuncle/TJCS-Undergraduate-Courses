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
*
* Filename : kal.h
* Version  : V1.01.02
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This library header file is protected from multiple pre-processor inclusion through
*               use of the library module present pre-processor macro definition.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  KAL_MODULE_PRESENT                                     /* See Note #1.                                         */
#define  KAL_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_mem.h>
#include  "../common_err.h"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*********************************************************************************************************
*/

#ifdef   KAL_MODULE
#define  KAL_EXT
#else
#define  KAL_EXT  extern
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*********************************************************************************************************
*/

#if (LIB_VERSION < 13800)
#error  "KAL requires LIB_VERSION (defined in lib_def.h) 1.38.00 or higher to function properly."
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                GENERAL
*********************************************************************************************************
*/

#define  KAL_TIMEOUT_INFINITE                  0u

#define  KAL_OPT_NONE                           DEF_BIT_NONE


/*
*********************************************************************************************************
*                                        CREATE OPTS (LOCK ONLY)
*********************************************************************************************************
*/

#define  KAL_OPT_CREATE_NONE           (KAL_OPT)KAL_OPT_NONE

#define  KAL_OPT_CREATE_NON_REENTRANT           KAL_OPT_NONE    /* Create non-re-entrant lock.                          */
#define  KAL_OPT_CREATE_REENTRANT      (KAL_OPT)DEF_BIT_00      /* Create     re-entrant lock.                          */


/*
*********************************************************************************************************
*                                       PEND OPTS (LOCK, SEM, Q)
*********************************************************************************************************
*/

#define  KAL_OPT_PEND_NONE             (KAL_OPT)KAL_OPT_NONE

#define  KAL_OPT_PEND_BLOCKING                  KAL_OPT_NONE    /* Block until rsrc is avail.                           */
#define  KAL_OPT_PEND_NON_BLOCKING     (KAL_OPT)DEF_BIT_00      /* Don't block if rsrc is unavail.                      */


/*
*********************************************************************************************************
*                                       POST OPTS (LOCK, SEM, Q)
*********************************************************************************************************
*/

#define  KAL_OPT_POST_NONE                      KAL_OPT_NONE


/*
*********************************************************************************************************
*                                        ABORT OPTS (LOCK, SEM)
*********************************************************************************************************
*/

#define  KAL_OPT_ABORT_NONE                     KAL_OPT_NONE


/*
*********************************************************************************************************
*                                      DELETE OPTS (LOCK, SEM, Q)
*********************************************************************************************************
*/

#define  KAL_OPT_DEL_NONE                       KAL_OPT_NONE


/*
*********************************************************************************************************
*                                               TMR OPTS
*********************************************************************************************************
*/

#define  KAL_OPT_TMR_NONE              (KAL_OPT)KAL_OPT_NONE

#define  KAL_OPT_TMR_ONE_SHOT                   KAL_OPT_NONE    /* One-shot tmr,   callback called only once.           */
#define  KAL_OPT_TMR_PERIODIC          (KAL_OPT)DEF_BIT_00      /* Periodic timer, callback called periodically.        */


/*
*********************************************************************************************************
*                                               DLY OPTS
*********************************************************************************************************
*/

#define  KAL_OPT_DLY_NONE                       KAL_OPT_NONE

#define  KAL_OPT_DLY                            KAL_OPT_NONE    /* 'Normal' delay.                                      */
#define  KAL_OPT_DLY_PERIODIC                   DEF_BIT_00      /* Periodic delay.                                      */


/*
*********************************************************************************************************
*                                      LEGACY ERROR CODES
*
* Note(s) : (1) These error codes are deprecated and will be removed in a future version. Please use the
*               codes from RTOS_ERR instead, located in common_err.h
*********************************************************************************************************
*/

#define  KAL_ERR_NONE               RTOS_ERR_NONE

#define  KAL_ERR_NOT_AVAIL          RTOS_ERR_NOT_AVAIL
#define  KAL_ERR_UNIMPLEMENTED      RTOS_ERR_NOT_SUPPORTED
#define  KAL_ERR_INVALID_ARG        RTOS_ERR_INVALID_ARG
#define  KAL_ERR_NULL_PTR           RTOS_ERR_NULL_PTR
#define  KAL_ERR_OS                 RTOS_ERR_OS

#define  KAL_ERR_ALREADY_INIT       RTOS_ERR_ALREADY_INIT
#define  KAL_ERR_MEM_ALLOC          RTOS_ERR_ALLOC
#define  KAL_ERR_POOL_INIT          RTOS_ERR_INIT
#define  KAL_ERR_CREATE             RTOS_ERR_CREATE_FAIL
#define  KAL_ERR_TIMEOUT            RTOS_ERR_TIMEOUT
#define  KAL_ERR_ABORT              RTOS_ERR_ABORT
#define  KAL_ERR_WOULD_BLOCK        RTOS_ERR_WOULD_BLOCK
#define  KAL_ERR_ISR                RTOS_ERR_ISR
#define  KAL_ERR_OVF                RTOS_ERR_WOULD_OVF
#define  KAL_ERR_RSRC               RTOS_ERR_NO_MORE_RSRC

#define  KAL_ERR_LOCK_OWNER         RTOS_ERR_OWNERSHIP


/*
*********************************************************************************************************
*                                      LANGUAGE SUPPORT DEFINES
*********************************************************************************************************
*/

#ifdef  __cplusplus
#define  KAL_CPP_EXT  extern
#else
#define  KAL_CPP_EXT
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                GENERAL
*********************************************************************************************************
*/

typedef  CPU_INT32U  KAL_TICK;
typedef  CPU_INT32U  KAL_TICK_RATE_HZ;

typedef  CPU_INT16U  KAL_MSG_QTY;

typedef  CPU_INT08U  KAL_OPT;

                                                                /* !!!! This type is deprecated and will be removed ... */
typedef  RTOS_ERR    KAL_ERR;                                   /* ... a future version. Use RTOS_ERR instead.          */


/*
*********************************************************************************************************
*                                              OBJ HANDLES
*********************************************************************************************************
*/

typedef  struct  kal_task_handle {                              /* -------------------- TASK HANDLE ------------------- */
    void  *TaskObjPtr;
} KAL_TASK_HANDLE;

typedef  struct  kal_lock_handle {                              /* -------------------- LOCK HANDLE ------------------- */
    void  *LockObjPtr;
} KAL_LOCK_HANDLE;

typedef  struct  kal_sem_handle {                               /* -------------------- SEM HANDLE -------------------- */
    void  *SemObjPtr;
} KAL_SEM_HANDLE;

typedef  struct  kal_q_handle {                                 /* --------------------- Q HANDLE --------------------- */
    void  *QObjPtr;
} KAL_Q_HANDLE;

typedef  struct  kal_tmr_handle {                               /* -------------------- TMR HANDLE -------------------- */
    void  *TmrObjPtr;
} KAL_TMR_HANDLE;

typedef  struct  kal_task_reg_handle {                          /* ------------------ TASK REG HANDLE ----------------- */
    void  *TaskRegObjPtr;
} KAL_TASK_REG_HANDLE;


/*
*********************************************************************************************************
*                                              CFG STRUCTS
*********************************************************************************************************
*/

typedef  struct  kal_cfg {                                      /* ---------------------- GEN CFG --------------------- */
    MEM_SEG     *MemSegPtr;                                     /* Mem seg to use for alloc.                            */
} KAL_CFG;

typedef  struct  kal_task_ext_cfg {                             /* ------------------- TASK EXT CFG ------------------- */
    CPU_INT32U   Rsvd;                                          /* Rsvd for future use.                                 */
} KAL_TASK_EXT_CFG;

typedef  struct  kal_lock_ext_cfg {                             /* ------------------- LOCK EXT CFG ------------------- */
    KAL_OPT      Opt;                                           /* Opt passed to LockCreate() funct.                    */
} KAL_LOCK_EXT_CFG;

typedef  struct  kal_sem_ext_cfg {                              /* -------------------- SEM EXT CFG ------------------- */
    CPU_INT32U   Rsvd;                                          /* Rsvd for future use.                                 */
} KAL_SEM_EXT_CFG;

typedef  struct  kal_q_ext_cfg {                                /* --------------------- Q EXT CFG -------------------- */
    CPU_INT32U   Rsvd;                                          /* Rsvd for future use.                                 */
} KAL_Q_EXT_CFG;

typedef  struct  kal_tmr_ext_cfg {                              /* -------------------- TMR EXT CFG ------------------- */
    KAL_OPT      Opt;                                           /* Opt passed to TmrCreate() funct.                     */
} KAL_TMR_EXT_CFG;

typedef  struct  kal_task_reg_ext_cfg {                         /* ----------------- TASK REG EXT CFG ----------------- */
    CPU_INT32U   Rsvd;                                          /* Rsvd for future use.                                 */
} KAL_TASK_REG_EXT_CFG;


/*
*********************************************************************************************************
*                                            KAL FEATURES
*********************************************************************************************************
*/

typedef  enum  kal_feature {
    KAL_FEATURE_TASK_CREATE = 0u,                               /* Task creation.                                       */
    KAL_FEATURE_TASK_DEL,                                       /* Task del.                                            */

    KAL_FEATURE_LOCK_CREATE,                                    /* Lock create, acquire and release.                    */
    KAL_FEATURE_LOCK_ACQUIRE,                                   /* Lock pend.                                           */
    KAL_FEATURE_LOCK_RELEASE,                                   /* Lock post.                                           */
    KAL_FEATURE_LOCK_DEL,                                       /* Lock del.                                            */

    KAL_FEATURE_SEM_CREATE,                                     /* Sem creation.                                        */
    KAL_FEATURE_SEM_PEND,                                       /* Sem pend.                                            */
    KAL_FEATURE_SEM_POST,                                       /* Sem post.                                            */
    KAL_FEATURE_SEM_ABORT,                                      /* Sem pend abort.                                      */
    KAL_FEATURE_SEM_SET,                                        /* Sem set cnt.                                         */
    KAL_FEATURE_SEM_DEL,                                        /* Sem del.                                             */

    KAL_FEATURE_TMR,                                            /* Tmr creation and exec.                               */

    KAL_FEATURE_Q_CREATE,                                       /* Q creation.                                          */
    KAL_FEATURE_Q_POST,                                         /* Q post.                                              */
    KAL_FEATURE_Q_PEND,                                         /* Q pend.                                              */

    KAL_FEATURE_DLY,                                            /* Dly in both ms and ticks.                            */

    KAL_FEATURE_TASK_REG,                                       /* Task storage creation, get and set.                  */

    KAL_FEATURE_TICK_GET                                        /* Get OS tick val.                                     */
} KAL_FEATURE;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          GLOBAL CONSTANTS
*********************************************************************************************************
*********************************************************************************************************
*/

extern  const  CPU_INT32U           KAL_Version;                /* KAL version.                                         */
extern  const  KAL_TICK_RATE_HZ     KAL_TickRate;               /* Tick rate.                                           */

                                                                /* ------------------- NULL HANDLES ------------------- */
extern  const  KAL_TASK_HANDLE      KAL_TaskHandleNull;
extern  const  KAL_LOCK_HANDLE      KAL_LockHandleNull;
extern  const  KAL_SEM_HANDLE       KAL_SemHandleNull;
extern  const  KAL_Q_HANDLE         KAL_QHandleNull;
extern  const  KAL_TASK_REG_HANDLE  KAL_TaskRegHandleNull;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MACROS
*********************************************************************************************************
*********************************************************************************************************
*/

#define  KAL_TASK_HANDLE_IS_NULL(task_handle)            (((task_handle).TaskObjPtr        == KAL_TaskHandleNull.TaskObjPtr)       ? DEF_YES : DEF_NO)
#define  KAL_LOCK_HANDLE_IS_NULL(lock_handle)            (((lock_handle).LockObjPtr        == KAL_LockHandleNull.LockObjPtr)       ? DEF_YES : DEF_NO)
#define  KAL_SEM_HANDLE_IS_NULL(sem_handle)              (((sem_handle).SemObjPtr          == KAL_SemHandleNull.SemObjPtr)         ? DEF_YES : DEF_NO)
#define  KAL_Q_HANDLE_IS_NULL(q_handle)                  (((q_handle).QObjPtr              == KAL_QHandleNull.QObjPtr)             ? DEF_YES : DEF_NO)
#define  KAL_TMR_HANDLE_IS_NULL(tmr_handle)              (((tmr_handle).TmrObjPtr          == KAL_TmrHandleNull.TmrObjPtr)         ? DEF_YES : DEF_NO)
#define  KAL_TASK_REG_HANDLE_IS_NULL(task_reg_handle)    (((task_reg_handle).TaskRegObjPtr == KAL_TaskRegHandleNull.TaskRegObjPtr) ? DEF_YES : DEF_NO)


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 CORE
*********************************************************************************************************
*/

void                  KAL_Init         (      KAL_CFG                *p_cfg,
                                              RTOS_ERR               *p_err);

CPU_BOOLEAN           KAL_FeatureQuery (      KAL_FEATURE             feature,
                                              KAL_OPT                 opt);


/*
*********************************************************************************************************
*                                                 TASKS
*********************************************************************************************************
*/

KAL_TASK_HANDLE       KAL_TaskAlloc    (const CPU_CHAR               *p_name,
                                              void                   *p_stk_base,
                                              CPU_SIZE_T              stk_size_bytes,
                                              KAL_TASK_EXT_CFG       *p_cfg,
                                              RTOS_ERR               *p_err);

void                  KAL_TaskCreate   (      KAL_TASK_HANDLE         task_handle,
                                              void                  (*p_fnct)(void  *p_arg),
                                              void                   *p_task_arg,
                                              CPU_INT08U              prio,
                                              KAL_TASK_EXT_CFG       *p_cfg,
                                              RTOS_ERR               *p_err);

void                  KAL_TaskDel      (      KAL_TASK_HANDLE         task_handle,
                                              RTOS_ERR               *p_err);


/*
*********************************************************************************************************
*                                                 LOCKS
*********************************************************************************************************
*/

KAL_LOCK_HANDLE       KAL_LockCreate   (const CPU_CHAR               *p_name,
                                              KAL_LOCK_EXT_CFG       *p_cfg,
                                              RTOS_ERR               *p_err);

void                  KAL_LockAcquire  (      KAL_LOCK_HANDLE         lock_handle,
                                              KAL_OPT                 opt,
                                              CPU_INT32U              timeout,
                                              RTOS_ERR               *p_err);

void                  KAL_LockRelease  (      KAL_LOCK_HANDLE         lock_handle,
                                              RTOS_ERR               *p_err);

void                  KAL_LockDel      (      KAL_LOCK_HANDLE         lock_handle,
                                              RTOS_ERR               *p_err);


/*
*********************************************************************************************************
*                                                 SEMS
*********************************************************************************************************
*/

KAL_SEM_HANDLE        KAL_SemCreate    (const CPU_CHAR               *p_name,
                                              KAL_SEM_EXT_CFG        *p_cfg,
                                              RTOS_ERR               *p_err);

void                  KAL_SemPend      (      KAL_SEM_HANDLE          sem_handle,
                                              KAL_OPT                 opt,
                                              CPU_INT32U              timeout,
                                              RTOS_ERR               *p_err);

void                  KAL_SemPost      (      KAL_SEM_HANDLE          sem_handle,
                                              KAL_OPT                 opt,
                                              RTOS_ERR               *p_err);

void                  KAL_SemPendAbort (      KAL_SEM_HANDLE          sem_handle,
                                              RTOS_ERR               *p_err);

void                  KAL_SemSet       (      KAL_SEM_HANDLE          sem_handle,
                                              CPU_INT16U              cnt,
                                              RTOS_ERR               *p_err);

void                  KAL_SemDel       (      KAL_SEM_HANDLE          sem_handle,
                                              RTOS_ERR               *p_err);


/*
*********************************************************************************************************
*                                                 TMRS
*********************************************************************************************************
*/

KAL_TMR_HANDLE        KAL_TmrCreate    (const CPU_CHAR               *p_name,
                                              void                  (*p_callback)(void  *p_arg),
                                              void                   *p_callback_arg,
                                              CPU_INT32U              interval_ms,
                                              KAL_TMR_EXT_CFG        *p_cfg,
                                              RTOS_ERR               *p_err);

void                  KAL_TmrStart     (      KAL_TMR_HANDLE          tmr_handle,
                                              RTOS_ERR               *p_err);


/*
*********************************************************************************************************
*                                                  QS
*********************************************************************************************************
*/

KAL_Q_HANDLE          KAL_QCreate      (const CPU_CHAR               *p_name,
                                              KAL_MSG_QTY             max_msg_qty,
                                              KAL_Q_EXT_CFG          *p_cfg,
                                              RTOS_ERR               *p_err);

void                 *KAL_QPend        (      KAL_Q_HANDLE            q_handle,
                                              KAL_OPT                 opt,
                                              CPU_INT32U              timeout,
                                              RTOS_ERR               *p_err);

void                  KAL_QPost        (      KAL_Q_HANDLE            q_handle,
                                              void                   *p_msg,
                                              KAL_OPT                 opt,
                                              RTOS_ERR               *p_err);


/*
*********************************************************************************************************
*                                                 DLYS
*********************************************************************************************************
*/

void                  KAL_Dly          (      CPU_INT32U              dly_ms);

void                  KAL_DlyTick      (      KAL_TICK                dly_tick,
                                              KAL_OPT                 opt);


/*
*********************************************************************************************************
*                                               TASK REGS
*********************************************************************************************************
*/

KAL_TASK_REG_HANDLE   KAL_TaskRegCreate(      KAL_TASK_REG_EXT_CFG   *p_cfg,
                                              RTOS_ERR               *p_err);

CPU_INT32U            KAL_TaskRegGet   (      KAL_TASK_HANDLE         task_handle,
                                              KAL_TASK_REG_HANDLE     task_reg,
                                              RTOS_ERR               *p_err);

void                  KAL_TaskRegSet   (      KAL_TASK_HANDLE         task_handle,
                                              KAL_TASK_REG_HANDLE     task_reg_handle,
                                              CPU_INT32U              val,
                                              RTOS_ERR               *p_err);


/*
*********************************************************************************************************
*                                               TICK CTR
*********************************************************************************************************
*/

KAL_TICK              KAL_TickGet      (      RTOS_ERR               *p_err);

#endif /* KAL_MODULE_PRESENT */

