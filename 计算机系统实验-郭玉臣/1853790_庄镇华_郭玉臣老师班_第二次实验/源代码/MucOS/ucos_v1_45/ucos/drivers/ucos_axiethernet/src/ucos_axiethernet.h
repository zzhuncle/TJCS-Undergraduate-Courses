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
*                                             AXI ETHERNET
*
* File : ucos_axiethernet.h
*********************************************************************************************************
*/

#ifndef  UCOS_AXIETHERNET_PRESENT
#define  UCOS_AXIETHERNET_PRESENT


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <lib_def.h>
#include <cpu.h>

/*
*********************************************************************************************************
*                                        STRUCTURE DEFINITIONS
*********************************************************************************************************
*/

typedef struct {
    CPU_INT32U  DeviceId;
    CPU_INT32U  BaseAddress;
    CPU_INT32U  EmacType;
    CPU_INT32U  TxCSum;
    CPU_INT32U  RxCSum;
    CPU_INT32U  PhyType;
    CPU_INT32U  TXVlanTran;
    CPU_INT32U  RXVlanTran;
    CPU_INT32U  TXVlanTag;
    CPU_INT32U  RXVlanTag;
    CPU_INT32U  TXVlanStrip;
    CPU_INT32U  RXVlanStrip;
    CPU_INT32U  MCastExt;
    CPU_INT32U  Stats;
    CPU_INT32U  AVB;
    CPU_INT32U  SGMIIOverLVDS;
    CPU_INT32U  Intr;
    CPU_INT32U  ConnType;
    CPU_INT32U  ConnAddr;
    CPU_INT32U  PhyAddr;
    CPU_INT32U  RXIntr;
    CPU_INT32U  TCIntr;
} UCOS_AXIETHERNET_Config;


#endif /* UCOS_AXIETHERNET_PRESENT */
