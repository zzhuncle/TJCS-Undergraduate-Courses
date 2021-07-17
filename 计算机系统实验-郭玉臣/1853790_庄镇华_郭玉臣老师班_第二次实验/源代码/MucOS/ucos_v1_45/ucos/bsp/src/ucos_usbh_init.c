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
*                                         uC/USB-Host INIT CODE
*
* File : ucos_usbh_init.c
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

#if ((APP_USBH_ENABLED == DEF_ENABLED) && (UCOS_USB_TYPE == UCOS_USB_TYPE_HOST))
#include <usbh_hc_cfg.h>

#include <usbh_bsp_zynq_ehci.h>

#if (UCOS_USB_DRIVER == UCOS_USB_USBPS)
#include <ucos_usbps.h>
extern UCOS_USBPS_Config UCOS_USBPS_ConfigTable[];
#endif

static  CPU_STK  USBH_AsyncTaskStk[USBH_TASK_CFG_ASYNC_STACK_SIZE];
static  CPU_STK  USBH_HubTaskStk[USBH_TASK_CFG_HUB_STACK_SIZE];

static USBH_KERNEL_TASK_INFO  AsyncTaskInfo = {                 /* ---------------- INFO ON ASYNC TASK ---------------- */
    USBH_TASK_CFG_ASYNC_PRIO,                                   /* Async task priority.                                 */
    USBH_AsyncTaskStk,                                          /* Ptr to async task stack.                             */
    USBH_TASK_CFG_ASYNC_STACK_SIZE                              /* Size of async task stack.                            */
};

static USBH_KERNEL_TASK_INFO  HubTaskInfo = {                   /* ----------------- INFO ON HUB TASK ----------------- */
    USBH_TASK_CFG_HUB_PRIO,                                     /* Hub task priority.                                   */
    USBH_HubTaskStk,                                            /* Ptr to hub task stack.                               */
    USBH_TASK_CFG_HUB_STACK_SIZE                                /* Size of hub task stack.                              */
};

struct  usbh_hc_cfg  USBH_HC_Cfg_EHCI_USB = {
    (CPU_ADDR)0x00000000u,                                      /* Base addr of host controller hw registers.           */
    (CPU_ADDR)0x00000000u,                                      /* Base addr of host controller dedicated mem.          */
              0u,                                               /* Size      of host controller dedicated mem.          */
              DEF_ENABLED,                                      /* Does HC can access sys mem?                          */
              0u,                                               /* Data buf max len.                                    */
              4u,                                               /* Max nbr opened bulk EP.                              */
              4u,                                               /* Max nbr opened intr EP.                              */
              0u                                                /* Max nbr opened isoc EP.                              */
};


CPU_BOOLEAN UCOS_USBH_Init (void)
{
    USBH_ERR    usbh_err;

#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
    UCOS_Print("UCOS - Initializing uC/USB-Host.\r\n");
#endif

    usbh_err = USBH_Init(&AsyncTaskInfo, &HubTaskInfo);
    if (usbh_err != USBH_ERR_NONE) {
#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
        UCOS_Printf("UCOS - Error initializing uC/USB-Host. Error code %d\r\n", usbh_err);
#endif
        return (DEF_FAIL);
    }

    return (DEF_OK);
}

#if (UCOS_USB_DRIVER == UCOS_USB_USBPS)
CPU_BOOLEAN UCOS_USBH_Start (void)
{
    USBH_ERR    usbh_err;
    CPU_INT08U  hc_nbr;

#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
    UCOS_Print("UCOS - Starting uC/USB-Host.\r\n");
#endif

    USBH_HC_Cfg_EHCI_USB.BaseAddr = UCOS_USBPS_ConfigTable[UCOS_USB_DEVICE_ID].BaseAddress + 0x100u;

    hc_nbr = USBH_HC_Add(&USBH_HC_Cfg_EHCI_USB,
                         &EHCI_DrvAPI_Synopsys,
                         &EHCI_RH_API,
                         &USBH_HC_BSP_API_ZYNQ_EHCI_USB,
                         &usbh_err);
    if (usbh_err != USBH_ERR_NONE) {
#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
        UCOS_Printf("UCOS - Error adding USB host controller. Error code %d\r\n", usbh_err);
#endif
        return (DEF_FAIL);
    }

    usbh_err = USBH_HC_Start(hc_nbr);
    if (usbh_err != USBH_ERR_NONE) {
#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
        UCOS_Printf("UCOS - Error starting USB host controller. Error code %d\r\n", usbh_err);
#endif
        return (DEF_FAIL);
    }
}
#endif /* #if (UCOS_USB_DRIVER == UCOS_ETHERNET_USBPS) */

#endif /* #if (APP_USBH_ENABLED == DEF_ENABLED) */
