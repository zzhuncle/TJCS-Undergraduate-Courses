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
*                                 USB DEVICE MSC CLASS STORAGE DRIVER
*
*                                               RAMDISK
*
* Filename : usbd_storage.h
* Version  : V4.05.06
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  USBF_STORAGE_H
#define  USBF_STORAGE_H


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "../../../../Source/usbd_core.h"
#include  "../../usbd_scsi.h"


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  USBD_StorageInit       (USBD_ERR          *p_err);

void  USBD_StorageAdd        (USBD_STORAGE_LUN  *p_storage_lun,
                              USBD_ERR          *p_err);

void  USBD_StorageCapacityGet(USBD_STORAGE_LUN  *p_storage_lun,
                              CPU_INT64U        *p_nbr_blks,
                              CPU_INT32U        *p_blk_size,
                              USBD_ERR          *p_err);

void  USBD_StorageRd         (USBD_STORAGE_LUN  *p_storage_lun,
                              CPU_INT64U         blk_addr,
                              CPU_INT32U         nbr_blks,
                              CPU_INT08U        *p_data_buf,
                              USBD_ERR          *p_err);

void  USBD_StorageWr         (USBD_STORAGE_LUN  *p_storage_lun,
                              CPU_INT64U         blk_addr,
                              CPU_INT32U         nbr_blks,
                              CPU_INT08U        *p_data_buf,
                              USBD_ERR          *p_err);

void  USBD_StorageStatusGet  (USBD_STORAGE_LUN  *p_storage_lun,
                              USBD_ERR          *p_err);

void  USBD_StorageLock       (USBD_STORAGE_LUN  *p_storage_lun,
                              CPU_INT32U         timeout_ms,
                              USBD_ERR          *p_err);

void  USBD_StorageUnlock     (USBD_STORAGE_LUN  *p_storage_lun,
                              USBD_ERR          *p_err);


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

#ifndef  USBD_RAMDISK_CFG_BLK_SIZE
#error  "USBD_RAMDISK_CFG_BLK_SIZE not #defined'd in 'usbd_cfg.h' [MUST be > 0]"
#elif   (USBD_RAMDISK_CFG_BLK_SIZE < 1u)
#error  "USBD_RAMDISK_CFG_BLK_SIZE illegally #define'd in 'usbd_cfg.h' [MUST be > 0]"
#endif

#ifndef  USBD_RAMDISK_CFG_NBR_BLKS
#error  "USBD_RAMDISK_CFG_NBR_BLKS not #defined'd in 'usbd_cfg.h' [MUST be > 0]"
#elif   (USBD_RAMDISK_CFG_NBR_BLKS < 1u)
#error  "USBD_RAMDISK_CFG_NBR_BLKS illegally #define'd in 'usbd_cfg.h' [MUST be > 0]"
#endif

#ifndef  USBD_RAMDISK_CFG_NBR_UNITS
#error  "USBD_RAMDISK_CFG_NBR_UNITS not #defined'd in 'usbd_cfg.h' [MUST be > 0]"
#elif   (USBD_RAMDISK_CFG_NBR_UNITS < 1u)
#error  "USBD_RAMDISK_CFG_NBR_UNITS illegally #define'd in 'usbd_cfg.h' [MUST be > 0]"
#endif

#ifndef  USBD_RAMDISK_CFG_BASE_ADDR
#error  "USBD_RAMDISK_CFG_BASE_ADDR not #defined'd in 'usbd_cfg.h' [MUST be >= 0]"
#elif   (USBD_RAMDISK_CFG_BASE_ADDR < 0u)
#error  "USBD_RAMDISK_CFG_BASE_ADDR illegally #define'd in 'usbd_cfg.h' [MUST be >= 0]"
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif
