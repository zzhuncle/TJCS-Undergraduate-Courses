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
* File : ucos_bsp.h
*********************************************************************************************************
*/

#ifndef  UCOS_BSP_PRESENT
#define  UCOS_BSP_PRESENT

#ifdef __cplusplus
extern "C" {
#endif

#include  <lib_def.h>
#include  <ucos_impl.h>

#if (APP_TCPIP_ENABLED == DEF_ENABLED)
#include  <IF/net_if_ether.h>
#endif

/* Possible CPU types. */
#define UCOS_CPU_TYPE_NONE 0
#define UCOS_CPU_TYPE_MB 1
#define UCOS_CPU_TYPE_PS7 2
#define UCOS_CPU_TYPE_PSUA53 3
#define UCOS_CPU_TYPE_PSUR5 4

/* Possible stdin/out providers. */
#define UCOS_UART_NONE 0
#define UCOS_UART_PS7_UART 1
#define UCOS_UART_AXI_UART_LITE 2
#define UCOS_UART_AXI_UART_16550 3

/* Possible systick providers. */
#define UCOS_SYSTICK_NONE 0
#define UCOS_SYSTICK_AXITIMER 1

/* Possible ts providers. */
#define UCOS_TS_NONE 0
#define UCOS_TS_AXITIMER 1

/* MicroBlaze interrupt handling types. */
#define UCOS_MB_INTC_TYPE_NONE 0
#define UCOS_MB_INTC_TYPE_RAW 1
#define UCOS_MB_INTC_TYPE_AXIINTC 2

/* Ethernet interface providers. */
#define UCOS_ETHERNET_NONE 0
#define UCOS_ETHERNET_EMACPS 1
#define UCOS_ETHERNET_AXIETHLITE 2
#define UCOS_ETHERNET_AXIETH 3

#define  UCOS_NET_PHY_SPD_0 0
#define  UCOS_NET_PHY_SPD_10 10
#define  UCOS_NET_PHY_SPD_100 100
#define  UCOS_NET_PHY_SPD_1000 1000
#define  UCOS_NET_PHY_SPD_AUTO 0xFFFF

#define  UCOS_NET_PHY_DUPLEX_UNKNOWN 0
#define  UCOS_NET_PHY_DUPLEX_HALF 1
#define  UCOS_NET_PHY_DUPLEX_FULL 2
#define  UCOS_NET_PHY_DUPLEX_AUTO 3

#define  UCOS_NET_PHY_BUS_MODE_MII 0
#define  UCOS_NET_PHY_BUS_MODE_RMII 1
#define  UCOS_NET_PHY_BUS_MODE_SMII 2
#define  UCOS_NET_PHY_BUS_MODE_GMII 3
#define  UCOS_NET_PHY_BUS_MODE_AUTO 255

#define  UCOS_NET_PHY_TYPE_INT 0
#define  UCOS_NET_PHY_TYPE_EXT 1
#define  UCOS_NET_PHY_ADDR_AUTO 0xFF

/* USB interface providers */
#define UCOS_USB_NONE 0
#define UCOS_USB_USBPS 1

/* USB IF Type */
#define UCOS_USB_TYPE_NONE 0
#define UCOS_USB_TYPE_DEVICE 1
#define UCOS_USB_TYPE_HOST 2

/* USB storage drivers. */
#define UCOS_USBD_STORAGE_DRV_RAM 0
#define UCOS_USBD_STORAGE_DRV_FS 1

#define UCOS_INT_SOURCE_NONE DEF_INT_32U_MAX_VAL

void UCOS_LowLevelInit (void);
void UCOS_StartupInit (void);
void UCOS_TmrTickInit (CPU_INT32U tick_rate);

CPU_BOOLEAN UCOSStartup (CPU_FNCT_PTR initial_func);
CPU_BOOLEAN UCOS_Shell_Init (void);
CPU_BOOLEAN UCOS_FS_Init (void);
CPU_BOOLEAN UCOS_DHCPc_Init (void);
CPU_BOOLEAN UCOS_DHCPc_IF_Init (CPU_INT32U if_nbr);
CPU_BOOLEAN UCOS_DNSc_Init (void);
CPU_BOOLEAN UCOS_HTTPc_Init (void);
CPU_BOOLEAN UCOS_MQTTc_Init (void);
CPU_BOOLEAN UCOS_TELNETs_Init (void);
CPU_BOOLEAN UCOS_IPerf_Init (void);
CPU_BOOLEAN UCOS_USBD_Init (void);
CPU_BOOLEAN UCOS_USBH_Init (void);
CPU_BOOLEAN UCOS_USBH_Start (void);

#if (APP_TCPIP_ENABLED == DEF_ENABLED)
CPU_BOOLEAN UCOS_TCPIP_Init (void);
CPU_BOOLEAN UCOS_TCPIP_IfCfgInit (NET_DEV_CFG_ETHER *Cfg);
CPU_BOOLEAN UCOS_TCPIP_PhyCfgInit (NET_PHY_CFG_ETHER *Cfg);
#endif

void UCOS_StdInOutInit (void);
void UCOS_PrintfInit (void);
void UCOS_Print (const CPU_CHAR *p_string);
void UCOS_Printf (const CPU_CHAR *ctrl1, ...);
CPU_INT32U UCOS_Read (CPU_CHAR *buf, CPU_INT32U cnt);


#endif /* #ifndef  UCOS_BSP_PRESENT */

#ifdef __cplusplus
}
#endif