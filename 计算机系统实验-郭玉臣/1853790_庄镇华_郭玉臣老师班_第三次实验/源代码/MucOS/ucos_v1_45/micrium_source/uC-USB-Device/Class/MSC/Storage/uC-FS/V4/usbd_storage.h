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
*                                              uC/FS V4
*
* Filename : usbd_storage.h
* Version  : V4.05.06
*********************************************************************************************************
*/

/*
**********************************************************************************************************
*                                                  MODULE
**********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "../../../../../Source/usbd_core.h"
#include  "../../../usbd_scsi.h"
#include  <fs_cfg.h>


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   STORAGE_MODULE
#define  STORAGE_EXT
#else
#define  STORAGE_EXT  extern
#endif


/*
*********************************************************************************************************
*                                                 DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  USBD_StorageInit              (USBD_ERR          *p_err);

void  USBD_StorageAdd               (USBD_STORAGE_LUN  *p_storage_lun,
                                     USBD_ERR          *p_err);

void  USBD_StorageCapacityGet       (USBD_STORAGE_LUN  *p_storage_lun,
                                     CPU_INT64U        *p_nbr_blks,
                                     CPU_INT32U        *p_blk_size,
                                     USBD_ERR          *p_err);

void  USBD_StorageRd                (USBD_STORAGE_LUN  *p_storage_lun,
                                     CPU_INT64U         blk_addr,
                                     CPU_INT32U         nbr_blks,
                                     CPU_INT08U        *p_data_buf,
                                     USBD_ERR          *p_err);

void  USBD_StorageWr                (USBD_STORAGE_LUN  *p_storage_lun,
                                     CPU_INT64U         blk_addr,
                                     CPU_INT32U         nbr_blks,
                                     CPU_INT08U        *p_data_buf,
                                     USBD_ERR          *p_err);

void  USBD_StorageStatusGet         (USBD_STORAGE_LUN  *p_storage_lun,
                                     USBD_ERR          *p_err);

void  USBD_StorageLock              (USBD_STORAGE_LUN  *p_storage_lun,
                                     CPU_INT32U         timeout_ms,
                                     USBD_ERR          *p_err);

void  USBD_StorageUnlock            (USBD_STORAGE_LUN  *p_storage_lun,
                                     USBD_ERR          *p_err);

#if (USBD_MSC_CFG_FS_REFRESH_TASK_EN == DEF_ENABLED)
void  USBD_StorageRefreshTaskHandler(void *p_arg);
#endif


/*
*********************************************************************************************************
*                                          CONFIGURATION ERRORS
*********************************************************************************************************
*/

#ifndef  USBD_MSC_CFG_FS_REFRESH_TASK_EN
#error  "USBD_MSC_CFG_FS_REFRESH_TASK_EN not #defined'd in 'usbd_cfg.h' [MUST be DEF_ENABLED or DEF_DISABLED]"
#endif


/*
**********************************************************************************************************
*                                           MODULE END
**********************************************************************************************************
*/




