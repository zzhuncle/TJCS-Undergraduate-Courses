/*
*********************************************************************************************************
*                                            uC/USB-Device
*                                    The Embedded USB Device Stack
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
*                                    USB DEVICE CONFIGURATION FILE
*
*                                              TEMPLATE
*
* Filename : usbd_dev_cfg.c
* Version  : V4.05.06
*********************************************************************************************************
*/

#include  <Source/usbd_core.h>
#include  <usbd_dev_cfg.h>
#include  <bsp_usbd_template.h>


/*
*********************************************************************************************************
*                                      USB DEVICE CONFIGURATION
*********************************************************************************************************
*/

const USBD_DEV_CFG  USBD_DevCfg_Template = {
    0xFFFE,                                                     /* Vendor  ID.                                          */
    0x1234,                                                     /* Product ID.                                          */
    0x0100,                                                     /* Device release number.                               */
   "MICRIUM MANUFACTURER",                                      /* Manufacturer  string.                                */
   "MICRIUM PRODUCT",                                           /* Product       string.                                */
   "1234567890ABCDEF",                                          /* Serial number string.                                */
    USBD_LANG_ID_ENGLISH_US                                     /* String language ID.                                  */
};


/*
*********************************************************************************************************
*                                   USB DEVICE DRIVER CONFIGURATION
*********************************************************************************************************
*/

const USBD_DRV_CFG  USBD_DrvCfg_Template = {
    0x00000000,                                                 /* Base addr   of device controller hw registers.       */
    0x00000000,                                                 /* Base addr   of device controller dedicated mem.      */
             0u,                                                /* Size        of device controller dedicated mem.      */

    USBD_DEV_SPD_FULL,                                          /* Speed       of device controller.                    */

    USBD_DrvEP_InfoTbl_Template                                 /* EP Info tbl of device controller.                    */
};
