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
*                                        USB DEVICE MSC CLASS
*
* Filename : usbd_msc.h
* Version  : V4.05.06
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  USBD_MSC_MODULE_PRESENT
#define  USBD_MSC_MODULE_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "../../Source/usbd_core.h"


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   USBD_MSC_MODULE
#define  USBD_MSC_EXT
#else
#define  USBD_MSC_EXT  extern
#endif


/*
*********************************************************************************************************
*                                                DEFINES
*
* Note(s) : (1) The T10 VENDOR IDENTIFICATION field contains 8 bytes of left-aligned ASCII data
*               identifying the vendor of the product. The T10 vendor identification shall be one
*               assigned by INCITS.
*               The PRODUCT IDENTIFICATION field contains 16 bytes of left-aligned ASCII data
*               defined by the vendor.
*               See 'SCSI Primary Commands - 3 (SPC-3)', section 6.4.2 for more details about
*               Standard INQUIRY data format.
*********************************************************************************************************
*/

                                                                /* ---- STANDARD INQUIRY DATA FORMAT (see Note #1) ---- */
#define  USBD_MSC_DEV_MAX_VEND_ID_LEN                      8u
#define  USBD_MSC_DEV_MAX_PROD_ID_LEN                     16u


/*
**********************************************************************************************************
*                                             DATA TYPES
**********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACROS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/


void         USBD_MSC_Init       (       USBD_ERR    *p_err);

CPU_INT08U   USBD_MSC_Add        (       USBD_ERR    *p_err);

CPU_BOOLEAN  USBD_MSC_CfgAdd     (       CPU_INT08U   class_nbr,
                                         CPU_INT08U   dev_nbr,
                                         CPU_INT08U   cfg_nbr,
                                         USBD_ERR    *p_err);

void         USBD_MSC_LunAdd     (const  CPU_CHAR    *p_store_name,
                                         CPU_INT08U   class_nbr,
                                         CPU_CHAR    *p_vend_id,
                                         CPU_CHAR    *p_prod_id,
                                         CPU_INT32U   prod_rev_level,
                                         CPU_BOOLEAN  rd_only,
                                         USBD_ERR    *p_err);

CPU_BOOLEAN  USBD_MSC_IsConn     (       CPU_INT08U   class_nbr);

void         USBD_MSC_TaskHandler(       CPU_INT08U   class_nbr);


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

#ifndef  USBD_MSC_CFG_MAX_NBR_DEV
#error  "USBD_MSC_CFG_MAX_NBR_DEV not #define'd in 'usbd_cfg.h' [MUST be >= 1]"
#endif

#if     (USBD_MSC_CFG_MAX_NBR_DEV < 1u)
#error  "USBD_MSC_CFG_MAX_NBR_DEV illegally #define'd in 'usbd_cfg.h' [MUST be >= 1]"
#endif

#ifndef  USBD_MSC_CFG_MAX_NBR_CFG
#error  "USBD_MSC_CFG_MAX_NBR_CFG not #define'd in 'usbd_cfg.h' [MUST be >= 1]"
#endif

#if     (USBD_MSC_CFG_MAX_NBR_CFG < 1u)
#error  "USBD_MSC_CFG_MAX_NBR_CFG illegally #define'd in 'usbd_cfg.h' [MUST be >= 1]"
#endif

#ifndef  USBD_MSC_CFG_MAX_LUN
#error  "USBD_MSC_CFG_MAX_LUN not #define'd in 'usbd_cfg.h' [MUST be >= 1]"
#endif

#if     (USBD_MSC_CFG_MAX_LUN < 1u)
#error  "USBD_MSC_CFG_MAX_LUN illegally #define'd in 'usbd_cfg.h' [MUST be >= 1]"
#endif

#ifndef  USBD_MSC_CFG_DATA_LEN
#error  "USBD_MSC_CFG_DATA_LEN not #define'd in 'usbd_cfg.h' [MUST be >= 1]"
#endif

#if     (USBD_MSC_CFG_DATA_LEN < 1u)
#error  "USBD_MSC_CFG_DATA_LEN illegally #define'd in 'usbd_cfg.h' [MUST be >= 1]"
#endif

#ifndef  USBD_MSC_CFG_MICRIUM_FS
#error  "USBD_MSC_CFG_MICRIUM_FS not #define'd in 'usbd_cfg.h' [MUST be DEF_ENABLED or DEF_DISABLED]"
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif
