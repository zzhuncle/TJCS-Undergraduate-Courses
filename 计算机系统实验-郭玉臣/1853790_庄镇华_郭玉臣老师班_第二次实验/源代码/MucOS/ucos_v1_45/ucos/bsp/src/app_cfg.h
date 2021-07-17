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
*
* File : app_cfg.h
*********************************************************************************************************
*/

#ifndef  UCOS_APP_CFG_PRESENT
#define  UCOS_APP_CFG_PRESENT

#include  <lib_def.h>

#include  <xparameters.h>

#define  APP_CPU_ENABLED DEF_DISABLED

#define  APP_LIB_ENABLED DEF_DISABLED

#define  APP_COMMON_ENABLED DEF_DISABLED

#define  APP_SHELL_ENABLED DEF_DISABLED

#define  APP_CLK_ENABLED DEF_DISABLED

#define  APP_OSIII_ENABLED DEF_DISABLED

#define  APP_OSII_ENABLED  DEF_DISABLED

#define  APP_TCPIP_ENABLED  DEF_DISABLED

#define  APP_TCPIP_EXP_ENABLED  DEF_DISABLED

#define  APP_DHCPC_ENABLED  DEF_DISABLED

#define  APP_DNSC_ENABLED  DEF_DISABLED

#define  APP_HTTPC_ENABLED  DEF_DISABLED

#define  APP_MQTTC_ENABLED  DEF_DISABLED

#define  APP_TELNETS_ENABLED  DEF_DISABLED

#define  APP_IPERF_ENABLED  DEF_DISABLED

#define  APP_FS_ENABLED  DEF_DISABLED

#define  APP_USBD_ENABLED  DEF_DISABLED

#define  APP_USBH_ENABLED  DEF_DISABLED

#define  APP_OPENAMP_ENABLED  DEF_DISABLED

#define  OS_TASK_TMR_PRIO                 (OS_LOWEST_PRIO - 2u)

#define  CLK_OS_CFG_TASK_STK_SIZE      1024
#define  CLK_OS_CFG_TASK_PRIO            20

#endif /* #ifndef  UCOS_APP_CFG_PRESENT */

