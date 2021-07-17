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
*                                USB COMMUNICATIONS DEVICE CLASS (CDC)
*                                   ETHERNET EMULATION MODEL (EEM)
*
* Filename : usbd_cdc_eem.h
* Version  : V4.05.06
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  USBD_CDC_EEM_MODULE_PRESENT
#define  USBD_CDC_EEM_MODULE_PRESENT


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

#ifdef   USBD_CDC_EEM_MODULE
#define  USBD_CDC_EEM_EXT
#else
#define  USBD_CDC_EEM_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  USBD_CDC_EEM_HDR_LEN                              2u   /* CDC EEM data header length.                          */


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                           CDC EEM CLASS INSTANCE CONFIGURATION STRUCTURE
*********************************************************************************************************
*/

typedef  struct  usbd_cdc_eem_cfg {
    CPU_INT08U  RxBufQSize;                                     /* Size of rx buffer Q.                                 */
    CPU_INT08U  TxBufQSize;                                     /* Size of tx buffer Q.                                 */
} USBD_CDC_EEM_CFG;


/*
*********************************************************************************************************
*                                           CDC EEM DRIVER
*********************************************************************************************************
*/

typedef  const  struct  usbd_cdc_eem_drv {
                                                                /* Retrieve a Rx buffer.                                */
    CPU_INT08U  *(*RxBufGet)   (CPU_INT08U   class_nbr,
                                void        *p_arg,
                                CPU_INT16U  *p_buf_len);

                                                                /* Signal that a rx buffer is ready.                    */
    void         (*RxBufRdy)   (CPU_INT08U   class_nbr,
                                void        *p_arg);

                                                                /* Free a tx buffer.                                    */
    void         (*TxBufFree)  (CPU_INT08U   class_nbr,
                                void        *p_arg,
                                CPU_INT08U  *p_buf,
                                CPU_INT16U   buf_len);
} USBD_CDC_EEM_DRV;


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
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

void          USBD_CDC_EEM_Init           (       USBD_ERR          *p_err);

CPU_INT08U    USBD_CDC_EEM_Add            (       USBD_ERR          *p_err);

void          USBD_CDC_EEM_CfgAdd         (       CPU_INT08U         class_nbr,
                                                  CPU_INT08U         dev_nbr,
                                                  CPU_INT08U         cfg_nbr,
                                           const  CPU_CHAR          *p_if_name,
                                                  USBD_ERR          *p_err);

CPU_BOOLEAN   USBD_CDC_EEM_IsConn         (       CPU_INT08U         class_nbr);

void          USBD_CDC_EEM_InstanceInit   (       CPU_INT08U         class_nbr,
                                                  USBD_CDC_EEM_CFG  *p_cfg,
                                                  USBD_CDC_EEM_DRV  *p_cdc_eem_drv,
                                                  void              *p_arg,
                                                  USBD_ERR          *p_err);

void          USBD_CDC_EEM_Start          (       CPU_INT08U         class_nbr,
                                                  USBD_ERR          *p_err);

void          USBD_CDC_EEM_Stop           (       CPU_INT08U         class_nbr,
                                                  USBD_ERR          *p_err);

CPU_INT08U    USBD_CDC_EEM_DevNbrGet      (       CPU_INT08U         class_nbr,
                                                  USBD_ERR          *p_err);

CPU_INT08U   *USBD_CDC_EEM_RxDataPktGet   (       CPU_INT08U         class_nbr,
                                                  CPU_INT16U        *p_rx_len,
                                                  CPU_BOOLEAN       *p_crc_computed,
                                                  USBD_ERR          *p_err);

void          USBD_CDC_EEM_TxDataPktSubmit(       CPU_INT08U         class_nbr,
                                                  CPU_INT08U        *p_buf,
                                                  CPU_INT32U         buf_len,
                                                  CPU_BOOLEAN        crc_computed,
                                                  USBD_ERR          *p_err);


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

#ifndef  USBD_CDC_EEM_CFG_MAX_NBR_DEV
#error  "USBD_CDC_EEM_CFG_MAX_NBR_DEV          not #define'd in 'usbd_cfg.h'"
#error  "                                [MUST be  >= 1]                    "
#endif

#if     (USBD_CDC_EEM_CFG_MAX_NBR_DEV < 1u)
#error  "USBD_CDC_EEM_CFG_MAX_NBR_DEV    illegally #define'd in 'usbd_cfg.h'"
#error  "                                [MUST be  >= 1]                    "
#endif

#ifndef  USBD_CDC_EEM_CFG_MAX_NBR_CFG
#error  "USBD_CDC_EEM_CFG_MAX_NBR_CFG          not #define'd in 'usbd_cfg.h'"
#error  "                                [MUST be  >= 1]                    "
#endif

#if     (USBD_CDC_EEM_CFG_MAX_NBR_CFG < 1u)
#error  "USBD_CDC_EEM_CFG_MAX_NBR_CFG    illegally #define'd in 'usbd_cfg.h'"
#error  "                                [MUST be  >= 1]                    "
#endif

#ifdef USBD_CDC_EEM_CFG_RX_BUF_QTY_PER_DEV
#if ((USBD_CDC_EEM_CFG_RX_BUF_QTY_PER_DEV - 1u) > USBD_CFG_MAX_NBR_URB_EXTRA)
#error  "USBD_CDC_EEM_CFG_RX_BUF_QTY_PER_DEV    illegally #define'd in 'usbd_cfg.h'"
#error  "                                       [MUST be   <= USBD_CFG_MAX_NBR_URB_EXTRA + 1]                    "
#endif
#endif

#ifndef  USBD_CDC_EEM_CFG_ECHO_BUF_LEN
#error  "USBD_CDC_EEM_CFG_ECHO_BUF_LEN         not #define'd in 'usbd_cfg.h'"
#error  "                                [MUST be  >= 2u]                   "
#endif

#if     (USBD_CDC_EEM_CFG_ECHO_BUF_LEN < 2u)
#error  "USBD_CDC_EEM_CFG_ECHO_BUF_LEN   illegally #define'd in 'usbd_cfg.h'"
#error  "                                [MUST be  >= 2]                    "
#endif

#ifndef  USBD_CDC_EEM_CFG_RX_BUF_LEN
#error  "USBD_CDC_EEM_CFG_RX_BUF_LEN           not #define'd in 'usbd_cfg.h'"
#error  "                                [MUST be  >= 2u]                   "
#endif

#if     (USBD_CDC_EEM_CFG_RX_BUF_LEN < 2u)
#error  "USBD_CDC_EEM_CFG_RX_BUF_LEN     illegally #define'd in 'usbd_cfg.h'"
#error  "                                [MUST be  >= 2]                    "
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif
