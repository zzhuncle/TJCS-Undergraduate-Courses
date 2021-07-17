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
* File : usbd_storage.h
*********************************************************************************************************
*/

#include <xparameters.h>
#include <ucos_bsp.h>

#if (UCOS_USBD_STORAGE_DRV == UCOS_USBD_STORAGE_DRV_FS)
#include "Class/MSC/Storage/uC-FS/V4/usbd_storage.h"
#else
#include "Class/MSC/Storage/RAMDisk/usbd_storage.h"
#endif