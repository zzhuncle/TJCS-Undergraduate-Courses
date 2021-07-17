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
*                                USB HID CLASS OPERATING SYSTEM LAYER
*
* Filename : usbd_hid_os.h
* Version  : V4.05.06
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  USBD_HID_OS_MODULE_PRESENT
#define  USBD_HID_OS_MODULE_PRESENT


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
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  USBD_HID_OS_Init               (USBD_ERR    *p_err);


void  USBD_HID_OS_InputLock          (CPU_INT08U   class_nbr,
                                      USBD_ERR    *p_err);

void  USBD_HID_OS_InputUnlock        (CPU_INT08U   class_nbr);

void  USBD_HID_OS_InputDataPend      (CPU_INT08U   class_nbr,
                                      CPU_INT16U   timeout_ms,
                                      USBD_ERR    *p_err);

void  USBD_HID_OS_InputDataPendAbort (CPU_INT08U   class_nbr);

void  USBD_HID_OS_InputDataPost      (CPU_INT08U   class_nbr);


void  USBD_HID_OS_OutputLock         (CPU_INT08U   class_nbr,
                                      USBD_ERR    *p_err);

void  USBD_HID_OS_OutputUnlock       (CPU_INT08U   class_nbr);

void  USBD_HID_OS_OutputDataPendAbort(CPU_INT08U   class_nbr);

void  USBD_HID_OS_OutputDataPend     (CPU_INT08U   class_nbr,
                                      CPU_INT16U   timeout_ms,
                                      USBD_ERR    *p_err);

void  USBD_HID_OS_OutputDataPost     (CPU_INT08U   class_nbr);


void  USBD_HID_OS_TxLock             (CPU_INT08U   class_nbr,
                                      USBD_ERR    *p_err);

void  USBD_HID_OS_TxUnlock           (CPU_INT08U   class_nbr);


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
