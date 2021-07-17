/*
*********************************************************************************************************
*                                             uCOS XSDK BSP
*
*                      (c) Copyright 2014-2020; Silicon Laboratories Inc.,
*                             400 W. Cesar Chavez, Austin, TX 78701
*
*                All rights reserved. Protected by international copyright laws.
*
*               Your use of this software is subject to your acceptance of the terms
*               of a Silicon Labs Micrium software license, which can be obtained by
*               contacting info@micrium.com. If you do not agree to the terms of this
*               license, you may not use this software.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available. Your honesty is greatly appreciated.
*
*                 You can find our product's documentation at: doc.micrium.com
*
*                       For more information visit us at: www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                 HTTP CLIENT CONFIGURATION FILE
*
* File : http-c_cfg.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             INCLUDE FILES
*
* Note(s) : (1) All values that are used in this file and are defined in other header files should be
*               included in this file. Some values could be located in the same file such as task priority
*               and stack size. This template file assume that the following values are defined in app_cfg.h:
*
*                   HTTPc_OS_CFG_INSTANCE_TASK_PRIO
*                   HTTPc_OS_CFG_INSTANCE_TASK_STK_SIZE
*
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <app_cfg.h>                                           /* See Note #1.                                         */
#include  <lib_def.h>

#include  <xparameters.h>
#include  "http-c_cfg.h"

/*
*********************************************************************************************************
*********************************************************************************************************
*                                             DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                        SECURE CONFIGURATION
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                              HTTP CLIENT SUITE CONFIGURATION OBJECT
*
* Note(s): (1) For additional information on the HTTP Client Configuration fields, refer to the
*              Micrium Documentation online at : https://doc.micrium.com/display/HTTPc/Run-Time Configuration
*********************************************************************************************************
*********************************************************************************************************
*/

const  HTTPc_CFG  HTTPc_Cfg = {

/*
*--------------------------------------------------------------------------------------------------------
*                                       TASK CONFIGURATION
*--------------------------------------------------------------------------------------------------------
*/

         UCOS_HTTPc_OS_CFG_TASK_DELAY,                          /* .TaskDly_ms (HTTPc Task delay in milliseconds)       */
         UCOS_HTTPc_OS_CFG_MSG_Q_SIZE,                          /* .MsqQ_Size  (Message Task Queue size)                */

/*
*--------------------------------------------------------------------------------------------------------
*                                    CONNECTION CONFIGURATION
*--------------------------------------------------------------------------------------------------------
*/

         UCOS_HTTPc_OS_CFG_TIMEOUT,                             /* .ConnConnectTimeout_ms   (Connect timeout in ms)     */
         UCOS_HTTPc_OS_CFG_INACTIVITY_TIMEOUT,                  /* .ConnInactivityTimeout_s (Inactivity timeout in s)   */

};


/*
*********************************************************************************************************
*********************************************************************************************************
*                               HTTP CLIENT TASK CONFIGURATION OBJECT
*
* Note(s): (1) We recommend to configure the Network Protocol Stack task priorities & HTTP client task
*              priority as follows:
*
*                   NET_OS_CFG_IF_TX_DEALLOC_TASK_PRIO (Highest)
*
*                   HTTPc_OS_CFG_TASK_PRIO             (  ...  )
*
*                   NET_OS_CFG_TMR_TASK_PRIO           (  ...  )
*
*                   NET_OS_CFG_IF_RX_TASK_PRIO         (Lowest )
*
*              We recommend that the uC/TCP-IP Timer task and network interface Receive task be lower
*              priority than almost all other application tasks; but we recommend that the network
*              interface Transmit De-allocation task be higher priority than all application tasks that
*              use uC/TCP-IP network services.
*
*              However better performance can be observed when the HTTP Client is set with the lowest
*              priority. So some experimentation could be required to identify the better task priority
*              configuration.
*
*          (2) TODO note on the HTTP Client stack's task size.
*
*          (3) When the Stack pointer is defined as null pointer (DEF_NULL), the task's stack should be
*              automatically allowed on the heap of uC/LIB.
*********************************************************************************************************
*********************************************************************************************************
*/

#if (HTTPc_CFG_MODE_ASYNC_TASK_EN == DEF_ENABLED)

#ifndef  HTTPc_OS_CFG_TASK_PRIO
#define  HTTPc_OS_CFG_TASK_PRIO                  20
#endif

#ifndef  HTTPc_OS_CFG_TASK_STK_SIZE
#define  HTTPc_OS_CFG_TASK_STK_SIZE            1024
#endif

const  HTTP_TASK_CFG  HTTPc_TaskCfg = {
        HTTPc_OS_CFG_TASK_PRIO,                                 /* HTTPc task priority              (See Note #1).      */
        HTTPc_OS_CFG_TASK_STK_SIZE,                             /* HTTPc task stack size in bytes   (See Note #2).      */
        DEF_NULL,                                               /* HTTPc task stack pointer         (See Note #3).      */
};
#endif
