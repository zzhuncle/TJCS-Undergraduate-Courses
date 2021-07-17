/*
*********************************************************************************************************
*                                               uC/DNSc
*                                     Domain Name Server (client)
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
*                                    DNS CLIENT CONFIGURATION FILE
*
*                                              TEMPLATE
*
* Filename : dns-c_cfg.c
* Version  : V2.01.02
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
*                   DNSc_OS_CFG_INSTANCE_TASK_PRIO
*                   DNSc_OS_CFG_INSTANCE_TASK_STK_SIZE
*
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <app_cfg.h>                                           /* See Note #1.                                         */
#include  <lib_def.h>
#include  <Source/dns-c.h>
#include  <dns-c_cfg.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                 DNS CLIENT CONFIGURATION STRUCTURE
*********************************************************************************************************
*********************************************************************************************************
*/

const  DNSc_CFG  DNSc_Cfg = {
                                /* Configure segment where allocate the memory for          */
                                /* DNS objects:                                             */
    DEF_NULL,
                                /* [DEF_NULL] to allocate from uC/LIB Memory HEAP.          */
                                /* [Pointer] to the memory segment.                         */


                                /* Configure default DNS server to use.                     */
#if    defined(NET_IPv4_MODULE_EN)
    "8.8.8.8",
#elif  defined(NET_IPv6_MODULE_EN)
    "2001:4860:4860::8888",
#endif
                                /* [Pointer] to a string that contains the IP address.      */


                                /* Configure the maximum host name length                   */
    DNSc_DFLT_HOST_NAME_LEN,
                                /* [MUST be >= 2]                                           */


                                /* Configure number of entry the cache can contains:        */
    2,                          /* [MUST be >= 1]                                           */


                                /* Configure how many IP addresses can be stored by host:   */
    1,                          /* Number of IPv4 Addresses.                                */
    1,                          /* Number of IPv6 Addresses.                                */
                                /* [MUST be >= 1]                                           */



                                /* Configure task delay in integer milliseconds :           */
    DNSc_DFLT_TASK_DLY_MS,
                                /* Default value: [DNSc_DFLT_TASK_DLY_MS] = 50ms            */
                                /* [MUST be >= 1]                                           */


                                /* Configure maximum of request resolution retry :          */
    DNSc_DFLT_REQ_RETRY_NBR_MAX,
                                /* Default value: [DNSc_DFLT_REQ_RETRY_NBR_MAX] = 2         */
                                /* [MUST be >= 1]                                           */

                                /* Configure timeout before a request resolution retry :    */
    DNSc_DFLT_REQ_RETRY_TIMEOUT_MS,
                                /* Default value: [DNSc_DFLT_REQ_RETRY_TIMEOUT_MS] = 1000ms */
                                /* [MUST be >= 100]                                         */
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                               DNS CLIENT TASK CONFIGURATION STRUCTURE
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#if (DNSc_CFG_MODE_ASYNC_EN == DEF_ENABLED)
#ifndef  DNSc_OS_CFG_INSTANCE_TASK_PRIO
#define  DNSc_OS_CFG_INSTANCE_TASK_PRIO                  30
#endif

#ifndef  DNSc_OS_CFG_INSTANCE_TASK_STK_SIZE
#define  DNSc_OS_CFG_INSTANCE_TASK_STK_SIZE             512
#endif

const  DNSc_CFG_TASK  DNSc_CfgTask = {
                                         DNSc_OS_CFG_INSTANCE_TASK_PRIO,
                                         DNSc_OS_CFG_INSTANCE_TASK_STK_SIZE,
                                         DEF_NULL
                                     };
#endif
