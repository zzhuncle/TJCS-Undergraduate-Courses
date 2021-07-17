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
*                                         uC/TELNET-S INIT CODE
*
* File : ucos_telnet-s_init.c
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

#if (APP_TELNETS_ENABLED == DEF_ENABLED)
#include <Source/telnet-s.h>
#include <Cmd/telnet-s_shell.h>
#endif

#if (APP_TELNETS_ENABLED == DEF_ENABLED)
CPU_BOOLEAN UCOS_TELNETs_Init (void)
{
    NET_SOCK_ADDR_FAMILY ip_type = AF_INET;
    CPU_BOOLEAN ok;


#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
    UCOS_Print("UCOS - Initializing uC/TELNETs.\r\n");
#endif

    TELNETsShell_Init("DUT", "micrium");

    ok = TELNETs_Init(NET_SOCK_ADDR_FAMILY_IP_V4, DEF_NULL);

    if (ok == DEF_OK)
        return (DEF_OK);
    else
        return (DEF_FAIL);
}

#endif /* #if (APP_TELNETS_ENABLED == DEF_ENABLED) */
