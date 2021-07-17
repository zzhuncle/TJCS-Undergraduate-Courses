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
*                                          USB DEVICE MSC SCSI
*
* Filename : usbd_scsi.h
* Version  : V4.05.06
*********************************************************************************************************
*/

/*
**********************************************************************************************************
*                                                 MODULE
**********************************************************************************************************
*/

#ifndef  USBD_SCSI_H
#define  USBD_SCSI_H


/*
**********************************************************************************************************
*                                            INCLUDE FILES
**********************************************************************************************************
*/

#include  "../../Source/usbd_core.h"
#include  "usbd_msc.h"


/*
**********************************************************************************************************
*                                                 EXTERNS
**********************************************************************************************************
*/

#ifdef   USBD_SCSI_MODULE
#define  USBD_SCSI_EXT
#else
#define  USBD_SCSI_EXT  extern
#endif


/*
**********************************************************************************************************
*                                                 DEFINES
**********************************************************************************************************
*/


/*
**********************************************************************************************************
*                                               DATA TYPES
**********************************************************************************************************
*/

/*
**********************************************************************************************************
*                                          STORAGE UNIT CONTROL
**********************************************************************************************************
*/

typedef  struct  usbd_storage_lun {
    CPU_INT08U    LunNbr;                                       /* Logical Unit Number.                                 */
    CPU_CHAR     *VolStrPtr;                                    /* String uniquely identifying a logical unit.          */
    CPU_BOOLEAN   MediumPresent;                                /* Flag indicating presence of logical unit.            */
    CPU_BOOLEAN   LockFlag;                                     /* Flag indicating logical unit locked or not.          */
    CPU_BOOLEAN   EjectFlag;                                    /* Flag indicating logical unit ejected by host or not. */
} USBD_STORAGE_LUN;


/*
**********************************************************************************************************
*                                        LOGICAL UNIT CHARACTERISTICS
**********************************************************************************************************
*/

typedef  struct  usbd_lun_info {
    CPU_INT08U   VendorId[USBD_MSC_DEV_MAX_VEND_ID_LEN];        /* Dev vendor info.                                     */
    CPU_INT08U   ProdId[USBD_MSC_DEV_MAX_PROD_ID_LEN];          /* Dev prod ID.                                         */
    CPU_INT32U   ProdRevisionLevel;                             /* Revision level of product.                           */
    CPU_BOOLEAN  ReadOnly;                                      /* Wr protected or not.                                 */
} USBD_LUN_INFO;


/*
**********************************************************************************************************
*                                            LOGICAL UNIT CONTROL
**********************************************************************************************************
*/

typedef  struct  usbd_msc_lun_ctrl {
    CPU_INT08U       LunNbr;                                    /* LUN given by MSC IF.                                 */
    USBD_LUN_INFO    LunInfo;                                   /* Logical unit info.                                   */
    CPU_INT64U       NbrBlocks;                                 /* Nbr of blks supported by logical unit.               */
    CPU_INT32U       BlockSize;                                 /* Blk size supported by logical unit.                  */
    void            *LunArgPtr;                                 /* Ptr to the LUN specific argument.                    */
} USBD_MSC_LUN_CTRL;


/*
**********************************************************************************************************
*                                            GLOBAL VARIABLES
**********************************************************************************************************
*/


/*
**********************************************************************************************************
*                                                 MACRO'S
**********************************************************************************************************
*/


/*
**********************************************************************************************************
*                                           FUNCTION PROTOTYPES
**********************************************************************************************************
*/

void  USBD_SCSI_Init      (      USBD_ERR          *p_err);

void  USBD_SCSI_LunAdd    (      CPU_INT08U         lun_nbr,
                                 CPU_CHAR          *p_vol_str,
                                 USBD_ERR          *p_err);

void  USBD_SCSI_CmdProcess(      USBD_MSC_LUN_CTRL  *p_lun,
                           const CPU_INT08U         *p_cbwcb,
                                 CPU_INT32U         *p_resp_len,
                                 CPU_INT08U         *p_data_dir,
                                 USBD_ERR           *p_err);

void  USBD_SCSI_DataRd    (const USBD_MSC_LUN_CTRL  *p_lun,
                                 CPU_INT08U          scsi_cmd,
                                 CPU_INT08U         *p_data_buf,
                                 CPU_INT32U          data_len,
                                 CPU_INT32U         *p_ret_len,
                                 USBD_ERR           *p_err);

void  USBD_SCSI_DataWr    (const USBD_MSC_LUN_CTRL  *p_lun,
                                 CPU_INT08U          scsi_cmd,
                                 void               *p_data_buf,
                                 CPU_INT32U          data_len,
                                 USBD_ERR           *p_err);

void  USBD_SCSI_Reset     (      void);

void  USBD_SCSI_Conn      (const USBD_MSC_LUN_CTRL  *p_lun);

void  USBD_SCSI_Unlock    (const USBD_MSC_LUN_CTRL  *p_lun,
                                 USBD_ERR           *p_err);


/*
**********************************************************************************************************
*                                          CONFIGURATION ERRORS
**********************************************************************************************************
*/


/*
**********************************************************************************************************
*                                              MODULE END
**********************************************************************************************************
*/
#endif
