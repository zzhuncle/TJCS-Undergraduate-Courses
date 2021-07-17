/*
*********************************************************************************************************
*                                               uC/CAN
*                                       The Embedded CAN suite
*
*                         (c) Copyright 2003-2019; Silicon Laboratories Inc.,
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
* Filename : can_os.h
* Version  : V2.42.01
*********************************************************************************************************
*/

#ifndef _CAN_OS_H_
#define _CAN_OS_H_


#ifdef __cplusplus
extern "C" {
#endif


/*
*********************************************************************************************************
*                                              INCLUDES
*********************************************************************************************************
*/

#include "can_frm.h"                                  /* CAN Frame handling                            */
#include "can_cfg.h"                                  /* CAN Configuration defines                     */


/*-----------------------------------------------------------------------------------------------------*/
/*! \brief                                OS: NO ERROR
*
*            This errorcode indicates 'no error detected'.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANOS_NO_ERR      (CPU_INT08U)0


/*
*********************************************************************************************************
*                                       FUNCTION PROTOTYPES
*********************************************************************************************************
*/

CPU_INT16S  CANOS_Init       (void);

CPU_INT08U  CANOS_PendRxFrame(CPU_INT16U  timeout, 
                              CPU_INT16S  busId);

void        CANOS_PostRxFrame(CPU_INT16S  busId);

void        CANOS_ResetRx    (CPU_INT16S  busId);

CPU_INT08U  CANOS_PendTxFrame(CPU_INT16U  timeout,
                              CPU_INT16S  busId);

void        CANOS_PostTxFrame(CPU_INT16S  busId);

void        CANOS_ResetTx    (CPU_INT16S  busId);

CPU_INT32U  CANOS_GetTime    (void);

#ifdef __cplusplus
}
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                /* #ifndef _CAN_OS_H                             */
