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
*                                      NETWORK CONFIGURATION FILE
*
*
* File : net_cfg.c
*********************************************************************************************************
*/

#define    MICRIUM_SOURCE
#define    NET_CFG_MODULE


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <app_cfg.h>
#include  <lib_def.h>
#include  <Source/net_type.h>
#include  <xparameters.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                     EXAMPLE TASKS CONFIGURATION
*
* Notes: (1) (a) Task priorities can be defined either in this configuration file 'net_cfg.c' or in a global
*                OS tasks priorities configuration header file which must be included in 'net_cfg.c' and
*                used within task's configuration structures:
*
*                   in app_cfg.h:
*                       #define  NET_TASK_PRIO_RX           30u
*                       #define  NET_TASK_PRIO_TX_DEALLOC    6u
*                       #define  NET_TASK_PRIO_TMR          18u
*
*                   in net_cfg.c:
*                       #include  <app_cfg.h>
*
*                       NET_TASK_CFG  NetRxTaskCfg        = {
*                                                              NET_TASK_PRIO_RX,
*                                                              2048,
*                                                              DEF_NULL,
*                                                           };
*
*                       NET_TASK_CFG  NetTxDeallocTaskCfg = {
*                                                              NET_TASK_PRIO_TX_DEALLOC,
*                                                              512,
*                                                              DEF_NULL,
*                                                           };
*
*                       NET_TASK_CFG  NetTmrTaskCfg       = {
*                                                              NET_TASK_PRIO_TMR,
*                                                              1024,
*                                                              DEF_NULL,
*                                                           };
*
*
*            (b) We recommend you configure the Network Protocol Stack task priorities & Network application
*                task priorities as follows:
*
*                   Network TX Dealloc task   (highest priority)
*
*                   Network application tasks, such as HTTPs instance.
*
*                   Network timer task
*                   Network RX task           (lowest  priority)
*
*                We recommend that the uC/TCP-IP Timer task and network interface Receive task be lower
*                priority than almost all other application tasks; but we recommend that the network
*                interface Transmit De-allocation task be higher  priority than all application tasks that use
*                uC/TCP-IP network services.
*
*                However better performance can be observed when the network application task is set with the
*                lowest priority. Some experimentation could be required to identify the best task priority
*                configuration.
*
*        (2)  The only guaranteed method of determining the required task stack sizes is to calculate the maximum
*             stack usage for each task. Obviously, the maximum stack usage for a task is the total stack usage
*             along the task's most-stack-greedy function path plus the (maximum) stack usage for interrupts.
*             Note that the most-stack-greedy function path is not necessarily the longest or deepest function path.
*             Micrium cannot provide any recommended stack size values since it's specific to each compiler and
*             processor.
*
*             Although Micrium does NOT officially recommend any specific tools to calculate task/function stack usage.
*             However Wikipedia maintains a list of static code analysis tools for various languages including C:
*
*                 http://en.wikipedia.org/wiki/List_of_tools_for_static_code_analysis
*
*        (3)  When the stack pointer is defined as null (DEF_NULL), the task's stack is allocated automatically on the
*             heap of uC/LIB. If for some reason you would like to allocate the stack somewhere else and by yourself,
*             you can just specify the memory location of the stack to use.
*********************************************************************************************************
*********************************************************************************************************
*/

#if (APP_TCPIP_EXP_ENABLED == DEF_ENABLED)
const  NET_TASK_CFG  NetTaskCfg = {
        NET_TASK_CFG_RX_PRIO,                                   /* Net task priority                    (see Note #1).  */
        NET_TASK_CFG_RX_STACK_SIZE,                             /* Net task stack size in bytes         (see Note #2).  */
        DEF_NULL,                                               /* Net task stack pointer               (See Note #3).  */
};
#else
const  NET_TASK_CFG  NetRxTaskCfg = {
        NET_TASK_CFG_RX_PRIO,                                   /* RX task priority                    (see Note #1).   */
        NET_TASK_CFG_RX_STACK_SIZE,                             /* RX task stack size in bytes         (see Note #2).   */
        DEF_NULL,                                               /* RX task stack pointer               (See Note #3).   */
};

const  NET_TASK_CFG  NetTxDeallocTaskCfg = {
        NET_TASK_CFG_TXDEALLOC_PRIO,                            /* TX Dealloc task priority            (see Note #1).   */
        NET_TASK_CFG_TXDEALLOC_STACK_SIZE,                      /* TX Dealloc task stack size in bytes (see Note #2).   */
        DEF_NULL,                                               /* TX Dealloc task stack pointer       (See Note #3).   */
};

const  NET_TASK_CFG  NetTmrTaskCfg = {
        NET_TASK_CFG_TMR_PRIO,                                  /* Timer task priority                 (see Note #1).   */
        NET_TASK_CFG_TMR_STACK_SIZE,                            /* Timer task stack size in bytes      (see Note #2).   */
        DEF_NULL,                                               /* Timer task stack pointer            (See Note #3).   */
};
#endif

