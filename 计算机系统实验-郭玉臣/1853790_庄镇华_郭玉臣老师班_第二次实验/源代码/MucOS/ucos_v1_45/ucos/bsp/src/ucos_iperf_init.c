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
*                                         uC/IPerf INIT CODE
*
* File : ucos_iperf_init.c
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

#if (APP_IPERF_ENABLED == DEF_ENABLED)
#include <Source/iperf.h>
#endif

#if (APP_IPERF_ENABLED == DEF_ENABLED)
CPU_BOOLEAN UCOS_IPerf_Init (void)
{
    NET_SOCK_ADDR_FAMILY ip_type = AF_INET;
    CPU_BOOLEAN ok;
    IPERF_ERR err_iperf;


#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
    UCOS_Print("UCOS - Initializing uC/IPerf.\r\n");
#endif

    IPerf_Init(&err_iperf);

    if (err_iperf != IPERF_ERR_NONE) {
        UCOS_Printf("UCOS - Error initializing uC/IPerf. Error code %d.\r\n", err_iperf);
        return (DEF_FAIL);
    }

    if (err_iperf == IPERF_ERR_NONE) {
        IPerfShell_Init(&err_iperf);
    }

    if (err_iperf != IPERF_ERR_NONE) {
        UCOS_Printf("UCOS - Error registering uC/IPerf shell commands. Error code %d.\r\n", err_iperf);
        return (DEF_FAIL);
    }

    return (DEF_OK);
}

#endif /* #if (APP_IPERF_ENABLED == DEF_ENABLED) */
