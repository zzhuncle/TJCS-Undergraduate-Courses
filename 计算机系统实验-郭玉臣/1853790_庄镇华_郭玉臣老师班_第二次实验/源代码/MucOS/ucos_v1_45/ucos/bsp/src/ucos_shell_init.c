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
*                                         uC/Shell INIT CODE
*
* File : ucos_dhcp-c_init.c
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

#if (APP_SHELL_ENABLED == DEF_ENABLED)
#include <Source/shell.h>
#endif

#if (APP_SHELL_ENABLED == DEF_ENABLED)
CPU_BOOLEAN UCOS_Shell_Init (void)
{
    CPU_BOOLEAN err_shell;


#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
    UCOS_Print("UCOS - Initializing uC/Shell.\r\n");
#endif

    err_shell = Shell_Init();

    if (err_shell != DEF_OK) {
        UCOS_Printf("UCOS - Error initializing uC/Shell.\r\n");
        return (DEF_FAIL);
    }

    return (DEF_OK);
}

#endif /* #if (APP_SHELL_ENABLED == DEF_ENABLED) */
