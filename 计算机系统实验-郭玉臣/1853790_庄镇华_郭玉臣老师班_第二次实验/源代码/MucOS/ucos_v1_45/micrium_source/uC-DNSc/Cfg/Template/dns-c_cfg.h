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
* Filename : dns-c_cfg.h
* Version  : V2.01.02
*********************************************************************************************************
*/

#ifndef DNSc_CFG_MODULE_PRESENT
#define DNSc_CFG_MODULE_PRESENT

#include  <Source/dns-c_type.h>


/*
*********************************************************************************************************
*                                  DNSc ARGUMENT CHECK CONFIGURATION
*
* Note(s) : (1) Configure DNSc_CFG_ARG_CHK_EXT_EN to enable/disable the DNS client external argument
*               check feature :
*
*               (a) When ENABLED,  ALL arguments received from any port interface provided by the developer
*                   are checked/validated.
*
*               (b) When DISABLED, NO  arguments received from any port interface provided by the developer
*                   are checked/validated.
*********************************************************************************************************
*/
                                                                /* Configure external argument check feature ...        */
                                                                /* See Note 1.                                          */
#define  DNSc_CFG_ARG_CHK_EXT_EN                    DEF_DISABLED
                                                                /* DEF_DISABLED     External argument check DISABLED    */
                                                                /* DEF_ENABLED      External argument check ENABLED     */

/*
*********************************************************************************************************
*                                     DNSc FEATURES CONFIGURATION
*
* Note(s) : (1) Configure DNSc_CFG_MODE_ASYNC_EN to enable/disable the DNS client asynchronous communication mode:
*
*               (a) When ENABLED,  A dedicated task will handle all host resolution request. It will be possible to
*                   call DNS API to get remote host address without blocking.
*
*               (b) When DISABLED, The API to get remote host will always block until the resolution is completed.
*
*           (2) Configure DNSc_CFG_MODE_BLOCK_EN to enable/disable the blocking option when the asynchronous
*               communication is enabled.
*
*               (a) When ENABLED,  It will be possible to block when calling the DNS API to get remote host until the
*                   resolution is completed (via a flag option).
*
*               (b) When DISABLED, The API to get remote host will always be non-blocking, must poll DNS client to
*                   know when the resolution is completed.
*********************************************************************************************************
*/

                                                                /* Configure asynchronous mode feature, See Note #1 ... */
#define  DNSc_CFG_MODE_ASYNC_EN                     DEF_DISABLED
                                                                /* DEF_DISABLED     Asynchronous mode DISABLED          */
                                                                /* DEF_ENABLED      Asynchronous mode ENABLED           */


                                                                /* Configure blocking option feature, See Note #2 ... */
#define  DNSc_CFG_MODE_BLOCK_EN                     DEF_DISABLED
                                                                /* DEF_DISABLED     Blocking option DISABLED            */
                                                                /* DEF_ENABLED      Blocking option ENABLED             */

/*
*********************************************************************************************************
*                                DNSc RUN-TIME STRUCTURE CONFIGURATION
*
* Note(s) : (1) These structures should be defined into a 'C' file.
*********************************************************************************************************
*/

extern  const  DNSc_CFG       DNSc_Cfg;                         /* Must always be defined.                              */

#if (DNSc_CFG_MODE_ASYNC_EN == DEF_ENABLED)
extern  const  DNSc_CFG_TASK  DNSc_CfgTask;                     /* Not required when Asynchronous mode is disabled.     */
#endif

#endif
