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
*                              AXI INTERRUPT CONTROLLER FOR THE MICROBLAZE
*
* File : ucos_int_impl.h
*********************************************************************************************************
*/

#ifndef  UCOS_INT_IMPL_PRESENT
#define  UCOS_INT_IMPL_PRESENT

#ifdef __cplusplus
extern "C" {
#endif

#include  <lib_def.h>
#include  <cpu.h>
#include  <xil_types.h>

typedef  XInterruptHandler  UCOS_INT_FNCT_PTR;
typedef  CPU_INT32U  UCOS_INT_TYPE;

#endif /* UCOS_INT_IMPL_PRESENT */

#ifdef __cplusplus
}
#endif
