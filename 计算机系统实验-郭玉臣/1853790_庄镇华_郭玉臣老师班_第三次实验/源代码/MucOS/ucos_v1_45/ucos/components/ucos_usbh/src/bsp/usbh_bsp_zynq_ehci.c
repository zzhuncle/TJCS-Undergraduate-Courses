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
*                                           XILINX ZYNQ-7000
*                             		 EXAMPLE BSP ON THE AVNET ZEBOARD
*
* File : usbh_bsp_ehci.h
*********************************************************************************************************
*/

#define  USBH_EHCI_ZYNQ7000_MODULE


/*
**************************************************************************************************************
*                                            INCLUDE FILES
**************************************************************************************************************
*/

#define   MICRIUM_SOURCE
#include  <cpu.h>
#include  <lib_def.h>
#include  <usbh_cfg.h>

#include  "usbh_bsp_zynq_ehci.h"

#include  <xparameters.h>

#if (XPAR_UCOS_USBPS_NUM_INSTANCES > 0)
#include  <ucos_int.h>
#include  <ucos_usbps.h>

#ifdef UCOS_USB_DEVICE_ID

/*
**************************************************************************************************************
*                                            LOCAL DEFINES
**************************************************************************************************************
*/

extern UCOS_USBPS_Config UCOS_USBPS_ConfigTable[];

/*
**************************************************************************************************************
*                                           LOCAL CONSTANTS
**************************************************************************************************************
*/


/*
**************************************************************************************************************
*                                          LOCAL DATA TYPES
**************************************************************************************************************
*/

typedef struct zynq7000_ehci_bsp_isr_info {
	CPU_FNCT_PTR   ISRFnct;       								/* HC Drv ISR ptr.										*/
	void          *ISRArg;        								/* HC Drv ISR arg.										*/
} ZYNQ7000_EHCI_BSP_ISR_INFO;


/*
**************************************************************************************************************
*                                            LOCAL TABLES
**************************************************************************************************************
*/


/*
**************************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
**************************************************************************************************************
*/

static  void  ZYNQ7000_EHCI_BSP_InitUSB0     (USBH_HC_DRV   *p_hc_drv,
                                              USBH_ERR      *p_err);

static  void  ZYNQ7000_EHCI_BSP_ISR_RegUSB0  (CPU_FNCT_PTR   isr_fnct,
                                              USBH_ERR      *p_err);

static  void  ZYNQ7000_EHCI_BSP_ISR_UnregUSB0(USBH_ERR      *p_err);

static  void  ZYNQ7000_EHCI_BSP_ISR          (void          *p_int_arg);


/*
**************************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
**************************************************************************************************************
*/

USBH_HC_BSP_API  USBH_HC_BSP_API_ZYNQ_EHCI_USB =
{
    ZYNQ7000_EHCI_BSP_InitUSB0,
    ZYNQ7000_EHCI_BSP_ISR_RegUSB0,
    ZYNQ7000_EHCI_BSP_ISR_UnregUSB0
};

static  ZYNQ7000_EHCI_BSP_ISR_INFO  ZYNQ7000_EHCI_BSP_ISR_InfoTbl[2u];


/*
**************************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
**************************************************************************************************************
*/


/*
**************************************************************************************************************
*                   INITIALIZED GLOBAL VARIABLES ACCESSES BY OTHER MODULES/OBJECTS
**************************************************************************************************************
*/


/*
**************************************************************************************************************
**************************************************************************************************************
*                                           GLOBAL FUNCTION
**************************************************************************************************************
**************************************************************************************************************
*/


/*
**************************************************************************************************************
**************************************************************************************************************
*                                           LOCAL FUNCTION
**************************************************************************************************************
**************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                     ZYNQ7000_EHCI_BSP_InitUSB0()
*
* Description : Performs board specific initialization for USB host controller.
*
* Argument(s) : p_hc_drv       Pointer to host controller driver structure
*
*               p_err          Pointer to variable that will receive the return error code from this function
*
*                              USBH_ERR_NONE    Initialization successful.
*
* Return(s)   : None.
*
* Caller(s)   : HCD Init() function via p_bsp_api->Init().
*
* Note(s)     : (1) The vast majority of the controller logic is driven by the 60 MHz clock from the
* 					ULPI PHY. The controller's interconnect is driven by the AHB/APB interface CPU_1x
* 					clock which is generated by the PS clock subsystem.
* 					The CPU_1x is configured through some registers from the System Level Control
* 					Registers (SLCR) module.
* 					The initialization code generated by Vivado for the ZedBoard already configures
* 					the required clock. The HTML file ps7_init.html contained in the Board Hardware
* 					Platform project summarizes the USB0 clock settings initialized in ps7_init.c.
*
* 					See section '15.15.1 Clocks' of 'Zynq-7000 AP SoC Technical Reference Manual,
* 					UG585 (v1.5) March 7, 2013' for more details about the USB0 I/O configuration.
*
* 				(2) I/O configuration: USB controller has multiple I/O interfaces including the main
* 					ULPI that interfaces via MIO to the external PHY and the port indicator and power
* 					signals via EMIO. The routing of the ULPI through the MIO must be programmed. The
* 					routing of the signals through the EMIO is always available to logic in the PL that
* 					can route these signals to the SelectIO pins.
* 					The ULPI I/O interface is configured through some registers from the System Level
* 					Control Registers (SLCR) module.
* 					The initialization code generated by Vivado for the ZedBoard already configures
* 					the required I/O. The HTML file ps7_init.html contained in the Board Hardware
* 					Platform project summarizes the USB0 I/O settings initialized in ps7_init.c.
*
* 					See section '15.16 I/O Interfaces' of 'Zynq-7000 AP SoC Technical Reference Manual,
* 					UG585 (v1.5) March 7, 2013' for more details about the USB0 I/O configuration.
*
* 				(3) On ZedBoard, Vbus 5V Enable is controlled directly on the board via the jumper JP2.
* 					This jumper must be shorted. No need to enable Vbus through a I/O.
*
* 					See section '4 Jumper Settings' of 'ZedBoard Hardware User's Guide' available at
* 					http://zedboard.org/sites/default/files/documentations/ZedBoard_HW_UG_v2_2.pdf
*********************************************************************************************************
*/

static  void  ZYNQ7000_EHCI_BSP_InitUSB0 (USBH_HC_DRV  *p_hc_drv,
                                          USBH_ERR     *p_err)
{
																/* Clock Cfg (see Note #1).								*/
																/* I/O Cfg (see Note #2).								*/
																/* Vbus 5V Enable (see Note #3).						*/

	ZYNQ7000_EHCI_BSP_ISR_InfoTbl[0u].ISRArg = (void *)p_hc_drv;

   *p_err = USBH_ERR_NONE;
}


/*
*********************************************************************************************************
*                                    ZYNQ7000_EHCI_BSP_ISR_RegUSB0()
*
* Description : This function registers the Interrupt Service Routine.
*
* Argument(s) : isr_fnct     Host controller ISR address.
*
*               p_err        Pointer to variable that will receive the return error code from this function
*
*                           USBH_ERR_NONE     Initialization successful.
*
* Return(s)   : None.
*
* Caller(s)   : HCD Start() function via p_bsp_api->ISR_Reg().
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  ZYNQ7000_EHCI_BSP_ISR_RegUSB0 (CPU_FNCT_PTR   isr_fnct,
                                             USBH_ERR      *p_err)
{
    CPU_INT32U irq_id;


	ZYNQ7000_EHCI_BSP_ISR_InfoTbl[0u].ISRFnct = isr_fnct;

	irq_id = UCOS_USBPS_ConfigTable[UCOS_USB_DEVICE_ID].IntSource;

    UCOS_IntVectSet(irq_id,
                    0u,
                    0u,
                    ZYNQ7000_EHCI_BSP_ISR,
                    (void *)&ZYNQ7000_EHCI_BSP_ISR_InfoTbl[0u]);

    UCOS_IntSrcEn(irq_id);

   *p_err = USBH_ERR_NONE;
}


/*
*********************************************************************************************************
*                                   ZYNQ7000_EHCI_BSP_ISR_UnregUSB0()
*
* Description : This function disables the main USB interrupt.
*
* Argument(s) : p_err   Pointer to variable that will receive the return error code from this function
*
*                       USBH_ERR_NONE       Initialization success.
*
* Return(s)   : None.
*
* Caller(s)   : HCD Stop() function via p_bsp_api->ISR_Unreg().
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  ZYNQ7000_EHCI_BSP_ISR_UnregUSB0 (USBH_ERR  *p_err)
{
    CPU_INT32U irq_id;


    irq_id = UCOS_USBPS_ConfigTable[UCOS_USB_DEVICE_ID].IntSource;

    UCOS_IntSrcDis(irq_id);

   *p_err = USBH_ERR_NONE;
}


/*
*********************************************************************************************************
*                                     ZYNQ7000_EHCI_BSP_ISR()
*
* Description : Interrupt Service Routine.
*
* Argument(s) : None.
*
* Return(s)   : None.
*
* Caller(s)   : Hardware.
*
* Note(s)     : None.
*********************************************************************************************************
*/

static void  ZYNQ7000_EHCI_BSP_ISR (void  *p_int_arg)
{
	ZYNQ7000_EHCI_BSP_ISR_INFO  *p_isr_info;

	p_isr_info = (ZYNQ7000_EHCI_BSP_ISR_INFO *)p_int_arg;

    if (p_isr_info->ISRFnct != (CPU_FNCT_PTR )0) {

    	p_isr_info->ISRFnct(p_isr_info->ISRArg);				/* Call HC drv ISR.                 					*/
    }
}


/*
**************************************************************************************************************
*                                                  END
**************************************************************************************************************
*/

#endif /* (XPAR_UCOS_USBPS_NUM_INSTANCES > 0) */

#endif /* #ifdef UCOS_USB_DEVICE_ID */
