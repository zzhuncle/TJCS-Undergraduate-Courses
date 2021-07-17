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
*                                        INTERRUPT MANAGEMENT
*
* File : ucos_int.h
*********************************************************************************************************
*/

#ifndef  UCOS_INT_PRESENT
#define  UCOS_INT_PRESENT

#ifdef __cplusplus
extern "C" {
#endif

#include  <lib_def.h>
#include  <cpu.h>
#include  <ucos_int_impl.h>
#include  <xil_exception.h>

/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  UCOS_INT_TYPE_LEVEL 0
#define  UCOS_INT_TYPE_EDGE 1


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

CPU_BOOLEAN UCOS_IntInit (void);

CPU_BOOLEAN UCOS_IntSrcEn (CPU_INT32U int_id);

CPU_BOOLEAN UCOS_IntSrcDis (CPU_INT32U int_id);

CPU_BOOLEAN UCOS_IntPrioSet (CPU_INT32U int_id, CPU_INT32U int_prio);

CPU_BOOLEAN UCOS_IntTargetSet (CPU_INT32U int_id, CPU_INT32U int_target_list);

CPU_BOOLEAN UCOS_IntTypeSet (CPU_INT32U int_id, UCOS_INT_TYPE type);

CPU_BOOLEAN UCOS_IntVectSet (CPU_INT32U         int_id,
                             CPU_INT32U         int_prio,
                             CPU_INT08U         int_target_list,
                             UCOS_INT_FNCT_PTR  int_fnct,
                             void              *p_int_arg);

#endif /* UCOS_INT_PRESENT */

#ifdef __cplusplus
}
#endif