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
*                                          CONFIGURATION FILE
*
* Filename : os_cfg.h
* Version  : V3.07.05
*********************************************************************************************************
*/

#ifndef OS_CFG_H
#define OS_CFG_H

                                                                /* --------------------------- MISCELLANEOUS --------------------------- */
#define OS_CFG_APP_HOOKS_EN                        1
#define OS_CFG_ARG_CHK_EN                          1
#define OS_CFG_CALLED_FROM_ISR_CHK_EN              1
#define OS_CFG_DBG_EN                              1
#define OS_CFG_TICK_EN                             1
#define OS_CFG_DYN_TICK_EN                         0
#define OS_CFG_INVALID_OS_CALLS_CHK_EN             0
#define OS_CFG_OBJ_TYPE_CHK_EN                     1
#define OS_CFG_TS_EN                               0

#define OS_CFG_PRIO_MAX                           64

#define OS_CFG_SCHED_LOCK_TIME_MEAS_EN             0
#define OS_CFG_SCHED_ROUND_ROBIN_EN                0

#define OS_CFG_STK_SIZE_MIN                       64


                                                                /* --------------------------- EVENT FLAGS ----------------------------- */
#define OS_CFG_FLAG_EN                             1
#define OS_CFG_FLAG_DEL_EN                         1
#define OS_CFG_FLAG_MODE_CLR_EN                    1
#define OS_CFG_FLAG_PEND_ABORT_EN                  1


                                                                /* ------------------------ MEMORY MANAGEMENT -------------------------  */
#define OS_CFG_MEM_EN                              1


                                                                /* ------------------- MUTUAL EXCLUSION SEMAPHORES --------------------  */
#define OS_CFG_MUTEX_EN                            1
#define OS_CFG_MUTEX_DEL_EN                        1
#define OS_CFG_MUTEX_PEND_ABORT_EN                 1


                                                                /* -------------------------- MESSAGE QUEUES --------------------------  */
#define OS_CFG_Q_EN                                1
#define OS_CFG_Q_DEL_EN                            1
#define OS_CFG_Q_FLUSH_EN                          1
#define OS_CFG_Q_PEND_ABORT_EN                     1


                                                                /* ---------------------------- SEMAPHORES ----------------------------- */
#define OS_CFG_SEM_EN                              1
#define OS_CFG_SEM_DEL_EN                          1
#define OS_CFG_SEM_PEND_ABORT_EN                   1
#define OS_CFG_SEM_SET_EN                          1


                                                                /* -------------------------- TASK MANAGEMENT -------------------------- */
#define OS_CFG_STAT_TASK_EN                        0
#define OS_CFG_STAT_TASK_STK_CHK_EN                0

#define OS_CFG_TASK_CHANGE_PRIO_EN                 1
#define OS_CFG_TASK_DEL_EN                         1
#define OS_CFG_TASK_IDLE_EN                        1
#define OS_CFG_TASK_PROFILE_EN                     0
#define OS_CFG_TASK_Q_EN                           1
#define OS_CFG_TASK_Q_PEND_ABORT_EN                1
#define OS_CFG_TASK_REG_TBL_SIZE                   2

#define OS_CFG_TASK_STK_REDZONE_EN                 0
#define OS_CFG_TASK_STK_REDZONE_DEPTH              8

#define OS_CFG_TASK_SEM_PEND_ABORT_EN              1
#define OS_CFG_TASK_SUSPEND_EN                     1


                                                                /* ------------------ TASK LOCAL STORAGE MANAGEMENT -------------------  */
#define OS_CFG_TLS_TBL_SIZE                        0


                                                                /* ------------------------- TIME MANAGEMENT --------------------------  */
#define OS_CFG_TIME_DLY_HMSM_EN                    1
#define OS_CFG_TIME_DLY_RESUME_EN                  1


                                                                /* ------------------------- TIMER MANAGEMENT -------------------------- */
#define OS_CFG_TMR_EN                              1
#define OS_CFG_TMR_DEL_EN                          1


                                                                /* ------------------------- TRACE RECORDER ---------------------------- */
#define OS_CFG_TRACE_EN                            0
#define OS_CFG_TRACE_API_ENTER_EN                  0
#define OS_CFG_TRACE_API_EXIT_EN                   0

#endif
