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
*                                    MICRIUM BOARD SUPPORT PACKAGE
*
* File : ucos_dev_cfg.h
*********************************************************************************************************
*/

#include  <Source/usbd_core.h>

#include  <ucos_bsp.h>
#include  <xparameters.h>

#if (UCOS_USB_DRIVER == UCOS_USB_USBPS)
#include  "usbd_bsp_ps7_usb.h"

USBD_DRV_CFG  UCOS_USBD_DrvCfg_PS7 = {
#if (UCOS_USB_DEVICE_ID == 1)
        XPAR_UCOS_USBPS_1_BASEADDR,
#else
        XPAR_UCOS_USBPS_0_BASEADDR,
#endif
        0,
        0,
        USBD_DEV_SPD_HIGH,
        USBD_DrvEP_InfoTbl_PS7_USB
    };
#endif
