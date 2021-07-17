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
*                                    USB DEVICE CORE INTERNAL FUNCTIONS
*
* Filename : usbd_internal.h
* Version  : V4.05.06
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  USBD_INTERNAL_MODULE_PRESENT
#define  USBD_INTERNAL_MODULE_PRESENT

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "usbd_core.h"


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
*                                    INTERNAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
                                                                /* ------------ DEVICE INTERNAL FUNCTIONS  ------------ */
USBD_DRV  *USBD_DrvRefGet          (CPU_INT08U   dev_nbr);

void       USBD_CoreTaskHandler    (void);

void       USBD_DbgTaskHandler     (void);

                                                                /* ------------ ENDPOINT INTERNAL FUNCTIONS ----------- */
void       USBD_EP_Init            (void);

void       USBD_EventEP            (USBD_DRV    *p_drv,
                                    CPU_INT08U   ep_addr,
                                    USBD_ERR     err);

void       USBD_CtrlOpen           (CPU_INT08U   dev_nbr,
                                    CPU_INT16U   max_pkt_size,
                                    USBD_ERR    *p_err);

void       USBD_CtrlClose          (CPU_INT08U   dev_nbr,
                                    USBD_ERR    *p_err);

void       USBD_CtrlStall          (CPU_INT08U   dev_nbr,
                                    USBD_ERR    *p_err);

void       USBD_CtrlRxStatus       (CPU_INT08U   dev_nbr,
                                    CPU_INT16U   timeout_ms,
                                    USBD_ERR    *p_err);

void       USBD_CtrlTxStatus       (CPU_INT08U   dev_nbr,
                                    CPU_INT16U   timeout_ms,
                                    USBD_ERR    *p_err);

void       USBD_EP_Open            (USBD_DRV    *p_drv,
                                    CPU_INT08U   ep_addr,
                                    CPU_INT16U   max_pkt_size,
                                    CPU_INT08U   attrib,
                                    CPU_INT08U   interval,
                                    USBD_ERR    *p_err);

void       USBD_EP_Close           (USBD_DRV    *p_drv,
                                    CPU_INT08U   ep_addr,
                                    USBD_ERR    *p_err);

void       USBD_EP_XferAsyncProcess(USBD_DRV    *p_drv,
                                    CPU_INT08U   ep_addr,
                                    USBD_ERR     xfer_err);


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                      DEFINED IN OS'S usbd_os.c
*********************************************************************************************************
*/

void   USBD_OS_Init           (USBD_ERR    *p_err);

void   USBD_OS_EP_SignalCreate(CPU_INT08U   dev_nbr,
                               CPU_INT08U   ep_ix,
                               USBD_ERR    *p_err);

void   USBD_OS_EP_SignalDel   (CPU_INT08U   dev_nbr,
                               CPU_INT08U   ep_ix);

void   USBD_OS_EP_SignalPend  (CPU_INT08U   dev_nbr,
                               CPU_INT08U   ep_ix,
                               CPU_INT16U   timeout_ms,
                               USBD_ERR    *p_err);

void   USBD_OS_EP_SignalAbort (CPU_INT08U   dev_nbr,
                               CPU_INT08U   ep_ix,
                               USBD_ERR    *p_err);

void   USBD_OS_EP_SignalPost  (CPU_INT08U   dev_nbr,
                               CPU_INT08U   ep_ix,
                               USBD_ERR    *p_err);

void   USBD_OS_EP_LockCreate  (CPU_INT08U   dev_nbr,
                               CPU_INT08U   ep_ix,
                               USBD_ERR    *p_err);

void   USBD_OS_EP_LockDel     (CPU_INT08U   dev_nbr,
                               CPU_INT08U   ep_ix);

void   USBD_OS_EP_LockAcquire (CPU_INT08U   dev_nbr,
                               CPU_INT08U   ep_ix,
                               CPU_INT16U   timeout_ms,
                               USBD_ERR    *p_err);

void   USBD_OS_EP_LockRelease (CPU_INT08U   dev_nbr,
                               CPU_INT08U   ep_ix);

void   USBD_OS_DbgEventRdy    (void);
void   USBD_OS_DbgEventWait   (void);

void  *USBD_OS_CoreEventGet   (CPU_INT32U   timeout_ms,
                               USBD_ERR    *p_err);

void   USBD_OS_CoreEventPut   (void        *p_event);


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif

