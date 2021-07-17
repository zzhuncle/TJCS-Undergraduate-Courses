/*
*********************************************************************************************************
*                                               uC/HTTP
*                                     Hypertext Transfer Protocol
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
*                                       HTTP SERVER TASK MODULE
*
* Filename : http-s_task.h
* Version  : V3.00.08
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This main network protocol suite header file is protected from multiple pre-processor
*               inclusion through use of the HTTPs module present pre-processor macro definition.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  HTTPs_TASK_MODULE_PRESENT                          /* See Note #1.                                         */
#define  HTTPs_TASK_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <KAL/kal.h>
#include  <cpu.h>
#include  <cpu_core.h>

#include  "http-s.h"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

KAL_LOCK_HANDLE      HTTPsTask_LockCreate                   (HTTPs_ERR                 *p_err);

void                 HTTPsTask_LockAcquire                  (KAL_LOCK_HANDLE            os_lock_obj,
                                                             HTTPs_ERR                 *p_err);

void                 HTTPsTask_LockRelease                  (KAL_LOCK_HANDLE            os_lock_obj);

void                 HTTPsTask_InstanceObjInit              (HTTPs_INSTANCE            *p_instance,
                                                             HTTPs_ERR                 *p_err);

void                 HTTPsTask_InstanceTaskCreate           (HTTPs_INSTANCE            *p_instance,
                                                             HTTPs_ERR                 *p_err);

void                 HTTPsTask_InstanceTaskDel              (HTTPs_INSTANCE            *p_instance);

void                 HTTPsTask_InstanceStopReqSignal        (HTTPs_INSTANCE            *p_instance,
                                                             HTTPs_ERR                 *p_err);

CPU_BOOLEAN          HTTPsTask_InstanceStopReqPending       (HTTPs_INSTANCE            *p_instance);

void                 HTTPsTask_InstanceStopCompletedSignal  (HTTPs_INSTANCE            *p_instance);

void                 HTTPsTask_InstanceStopCompletedPending (HTTPs_INSTANCE            *p_instance,
                                                             HTTPs_ERR                 *p_err);

void                 HTTPsTask_TimeDly_ms                   (CPU_INT32U                 time_dly_ms);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif  /* HTTPs_TASK_MODULE_PRESENT  */
