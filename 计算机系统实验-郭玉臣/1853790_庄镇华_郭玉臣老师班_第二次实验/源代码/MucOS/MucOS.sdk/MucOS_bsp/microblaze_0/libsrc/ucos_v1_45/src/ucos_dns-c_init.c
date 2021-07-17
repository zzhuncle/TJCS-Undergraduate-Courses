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
*                                    MICRIUM BOARD SUPPORT PACKAGE
*                                         uC/DNS-C INIT CODE
*
* File : ucos_dns-c_init.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <app_cfg.h>
#include <cpu.h>
#include <ucos_bsp.h>
#include <ucos_int.h>
#include <xparameters.h>
#include <KAL/kal.h>

#if (APP_TCPIP_ENABLED == DEF_ENABLED)
#include <IP/IPv4/net_ipv4.h>
#include <Source/net_type.h>
#include <Source/net_ip.h>

#if (APP_DNSC_ENABLED == DEF_ENABLED)
#include <Source/dns-c.h>
#endif

#if (APP_DNSC_ENABLED == DEF_ENABLED)
CPU_BOOLEAN UCOS_DNSc_Init (void)
{
    DNSc_ERR err_dns;


#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
    UCOS_Print("UCOS - Initializing uC/DNSc.\r\n");
#endif

#if (DNSc_CFG_MODE_ASYNC_EN == DEF_DISABLED)
    DNSc_Init(&DNSc_Cfg, DEF_NULL, &err_dns);

#else
    DNSc_Init(&DNSc_Cfg, &DNSc_CfgTask, &err_dns);
#endif

    if (err_dns != DNSc_ERR_NONE) {
        UCOS_Printf("UCOS - Error initializing uC/DNSc. Error code %d\r\n", err_dns);
        return (DEF_FAIL);
    }

    return (DEF_OK);
}

#endif /* #if (APP_TCPIP_ENABLED == DEF_ENABLED) */
#endif /* #if (APP_DNSC_ENABLED == DEF_ENABLED) */
