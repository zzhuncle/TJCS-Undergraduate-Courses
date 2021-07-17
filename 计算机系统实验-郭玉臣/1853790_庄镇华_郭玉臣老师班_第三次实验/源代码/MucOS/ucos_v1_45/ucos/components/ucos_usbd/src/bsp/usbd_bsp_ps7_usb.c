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
*                                  USB DEVICE DRIVER BOARD-SPECIFIC FUNCTIONS
*                                               XILINX ZYNQ-7000
*
* File : usbd_bsp_ps7_usb.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <ucos_bsp.h>
#include  <Source/usbd_core.h>

#include  <xparameters.h>

#if (UCOS_USB_DRIVER == UCOS_USB_USBPS)

#include  <ucos_usbps.h>

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*
* Note(s) : (1) The USB device controller hardware in the Zynq-7000 is configured to support
*               up to 12 endpoints.
*           (2) The USB device controller driver can enable, disable and configure endpoint
*               types up to the maximum selected during synthesis.
*           (3) Each endpoint direction is essentially independent and can be configured with
*               differing behavior in each direction.
*               For example, the driver can configure endpoint 1 IN to be a bulk endpoint and
*               endpoint 1 OUT to be an isochronous endpoint.
*********************************************************************************************************
*/

USBD_DRV_EP_INFO  USBD_DrvEP_InfoTbl_PS7_USB[] = {
    {USBD_EP_INFO_TYPE_CTRL                                                   | USBD_EP_INFO_DIR_OUT,  0u,   64u},
    {USBD_EP_INFO_TYPE_CTRL                                                   | USBD_EP_INFO_DIR_IN,   0u,   64u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_OUT,  1u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_IN,   1u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_OUT,  2u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_IN,   2u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_OUT,  3u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_IN,   3u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_OUT,  4u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_IN,   4u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_OUT,  5u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_IN,   5u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_OUT,  6u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_IN,   6u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_OUT,  7u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_IN,   7u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_OUT,  8u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_IN,   8u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_OUT,  9u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_IN,   9u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_OUT, 10u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_IN,  10u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_OUT, 11u, 1024u},
    {USBD_EP_INFO_TYPE_ISOC | USBD_EP_INFO_TYPE_BULK | USBD_EP_INFO_TYPE_INTR | USBD_EP_INFO_DIR_IN,  11u, 1024u},
    {DEF_BIT_NONE                                                                                  ,   0u,    0u}
};


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  USBD_DRV  *USBD_PS7_BSP_DrvPtr;

extern  UCOS_USBPS_Config UCOS_USBPS_ConfigTable[];


/*
*********************************************************************************************************
*                                            LOCAL MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  USBD_BSP_PS7_Init       (USBD_DRV  *p_drv);
static  void  USBD_BSP_PS7_Conn       (void            );
static  void  USBD_BSP_PS7_Disconn    (void            );
static  void  USBD_BSP_PS7_IntHandler (void      *p_arg);

/*
*********************************************************************************************************
*                                   USB DEVICE DRIVER BSP INTERFACE
*********************************************************************************************************
*/
                                                                /* Board-/device-specific BSP fnct ptrs :   */
USBD_DRV_BSP_API  USBD_DrvBSP_PS7_USB = {
    USBD_BSP_PS7_Init,
    USBD_BSP_PS7_Conn,
    USBD_BSP_PS7_Disconn,
};

/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       USBD_BSP_PS7_Init()
*
* Description : USB device controller board-specific initialization.
*
*                   (1) Enable    USB dev ctrl registers  and bus clock.
*                   (2) Configure USB dev ctrl interrupts.
*                   (3) Disable   USB dev transceiver Pull-up resistor in D+ line.
*                   (4) Disable   USB dev transceiver clock.
*
* Argument(s) : p_drv       Pointer to device driver structure.
*
* Return(s)   : none.
*
* Caller(s)   : Device controller driver init function via 'p_drv_api->Init()'
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  USBD_BSP_PS7_Init (USBD_DRV  *p_drv)
{
    CPU_INT32U  int_id;


    USBD_PS7_BSP_DrvPtr = p_drv;

    CPU_MB();

    int_id = UCOS_USBPS_ConfigTable[UCOS_USB_DEVICE_ID].IntSource;

    UCOS_IntVectSet (int_id,
                    0u,
                    0u,
                    USBD_BSP_PS7_IntHandler,
                    USBD_PS7_BSP_DrvPtr);

    UCOS_IntSrcEn(int_id);
}


/*
*********************************************************************************************************
*                                       USBD_BSP_PS7_Conn()
*
* Description : Connect pull-up on DP.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Device controller driver start function via 'p_drv_api->Conn()'
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  USBD_BSP_PS7_Conn (void)
{
    CPU_INT32U int_id;


    int_id = UCOS_USBPS_ConfigTable[UCOS_USB_DEVICE_ID].IntSource;

    UCOS_IntSrcEn(int_id);
}


/*
*********************************************************************************************************
*                                     USBD_BSP_PS7_Disconn()
*
* Description : Disconnect pull-up on DP.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Device controller driver stop function via 'p_drv_api->Disconn()'
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  USBD_BSP_PS7_Disconn (void)
{

}


/*
*********************************************************************************************************
*                                    USBD_BSP_PS7_IntHandler()
*
* Description : This is the USB ISR handler for the USB Device port(UDP) interrupt
*
* Argument(s) : p_arg       Interrupt handler argument.
*
* Return(s)   : none.
*
* Caller(s)   : This is a ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  USBD_BSP_PS7_IntHandler (void  *p_arg)
{
    USBD_DRV      *p_drv;
    USBD_DRV_API  *p_drv_api;


    p_drv     = (USBD_DRV *)p_arg;
    p_drv_api = p_drv->API_Ptr;

    p_drv_api->ISR_Handler(p_drv);
}

#endif /* #if (UCOS_USB_DRIVER == UCOS_USB_USBPS) */
