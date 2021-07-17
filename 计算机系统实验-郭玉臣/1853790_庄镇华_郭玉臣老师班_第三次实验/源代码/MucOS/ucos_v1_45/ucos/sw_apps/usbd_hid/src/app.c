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
*                                          SETUP INSTRUCTIONS
*
*   This demonstration project illustrates a basic uC/USB-Device project showcasing the HID class.
*   Only the USB interface of the Zynq-7000 is supported for this example.
*
*   By default some configuration steps are required to compile this example :
*
*   1. Include the require Micrium software components
*       In the BSP setting dialog in the "overview" section of the left pane the following libraries
*       should be added to the BSP :
*
*           ucos_common
*           ucos_osiii
*           ucos_standalone
*           ucos_usbd
*
*   2. USB Device interface
*       The USB interface used by the BSP can be configured from the "ucos" configuration dialog in the
*       USB INTERFACE section.
*
*   3. STDOUT configuration
*       Output from the print() and UCOS_Print() functions can be redirected to a supported UART. In
*       the "ucos_standalone" section the stdout configuration will list the available UARTs.
*
*   Troubleshooting :
*       By default the Xilinx SDK may not have selected the Micrium drivers for the timer and UART.
*       If that is the case they must be manually selected in the drivers configuration section.
*
*       Finally make sure the FPGA is programmed before debugging.
*
*
*   Remember that this example is provided for evaluation purposes only. Commercial development requires
*   a valid license from Micrium.
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdio.h>
#include  <Source/os.h>
#include  <ucos_bsp.h>

#include  <Source/usbd_core.h>
#include  <usbd_bsp_ps7_usb.h>
#include  <usbd_dev_cfg.h>
#include  "app_usb.h"

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  MainTask (void *p_arg);
void  APP_USBD_Init (void);


/*
*********************************************************************************************************
*                                               main()
*
* Description : Entry point for C code.
*
*********************************************************************************************************
*/

int main()
{

    UCOSStartup(MainTask);

    return 0;
}


/*
*********************************************************************************************************
*                                             MainTask()
*
* Description : Startup task example code.
*
* Returns     : none.
*
* Created by  : main().
*********************************************************************************************************
*/

void  MainTask (void *p_arg)
{
    OS_ERR       os_err;

    UCOS_Print ("Application start\r\n");

    APP_USBD_Init();

    while (DEF_TRUE) {
        OSTimeDlyHMSM(0, 0, 10, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);
        UCOS_Print("Periodic output every 10 seconds from the main task\r\n");
    }

}


USBD_DEV_CFG  USBD_DevCfg_Demo = {
    0xFFFD,                                                     /* Vendor  ID.                                          */
    0x1234,                                                     /* Product ID (HID).                                    */
    0x0101,                                                     /* Device release number.                               */
   "Test Manufacturer",                                         /* Manufacturer  string.                                */
   "Test Product",                                              /* Product       string.                                */
   "1234567890ABCDEF",                                          /* Serial number string.                                */
    USBD_LANG_ID_ENGLISH_US                                     /* String language ID.                                  */
};


                                                                /* ---------- USB DEVICE CALLBACKS FUNCTIONS ---------- */
static  void  App_USBD_EventReset  (CPU_INT08U   dev_nbr);
static  void  App_USBD_EventSuspend(CPU_INT08U   dev_nbr);
static  void  App_USBD_EventResume (CPU_INT08U   dev_nbr);
static  void  App_USBD_EventCfgSet (CPU_INT08U   dev_nbr, CPU_INT08U   cfg_val);
static  void  App_USBD_EventCfgClr (CPU_INT08U   dev_nbr, CPU_INT08U   cfg_val);
static  void  App_USBD_EventConn   (CPU_INT08U   dev_nbr);
static  void  App_USBD_EventDisconn(CPU_INT08U   dev_nbr);

static  USBD_BUS_FNCTS  App_USBD_BusFncts = {
    App_USBD_EventReset,
    App_USBD_EventSuspend,
    App_USBD_EventResume,
    App_USBD_EventCfgSet,
    App_USBD_EventCfgClr,
    App_USBD_EventConn,
    App_USBD_EventDisconn
};

void APP_USBD_Init (void)
{
    CPU_INT08U  dev_nbr;
    CPU_INT08U  cfg_hs_nbr;
    CPU_INT08U  cfg_fs_nbr;
    USBD_ERR    usbd_err;

    UCOS_Print("Adding USB Device interface\r\n");

    dev_nbr = USBD_DevAdd(&USBD_DevCfg_Demo,
                          &App_USBD_BusFncts,
                          &USBD_DrvAPI_Synopsys_OTG_HS,
                          &UCOS_USBD_DrvCfg_PS7,
                          &USBD_DrvBSP_PS7_USB,
                          &usbd_err);
    if (usbd_err != USBD_ERR_NONE) {
        UCOS_Printf("Error adding device. USBD_DevAdd returned error code %d\r\n", usbd_err);
    }

    USBD_DevSelfPwrSet(dev_nbr, DEF_NO, &usbd_err);              /* Device is not self-powered.                          */

    cfg_hs_nbr = USBD_CFG_NBR_NONE;
    cfg_fs_nbr = USBD_CFG_NBR_NONE;

#if (USBD_CFG_HS_EN == DEF_ENABLED)
    UCOS_Print("Adding USB Device high speed configuration\r\n");

    cfg_hs_nbr = USBD_CfgAdd( dev_nbr,                          /* Add HS configuration.                               */
                              USBD_DEV_ATTRIB_SELF_POWERED,
                              100u,
                              USBD_DEV_SPD_HIGH,
                             "HS configuration",
                             &usbd_err);
    if (usbd_err != USBD_ERR_NONE) {
        UCOS_Printf("Error adding high speed configuration. USBD_CfgAdd returned error code %d\r\n", usbd_err);
    }
#endif

    UCOS_Print("Adding USB Device full speed configuration\r\n");

    cfg_fs_nbr = USBD_CfgAdd( dev_nbr,                          /* Add FS configuration.                                */
                              USBD_DEV_ATTRIB_SELF_POWERED,
                              100u,
                              USBD_DEV_SPD_FULL,
                             "FS configuration",
                             &usbd_err);
    if (usbd_err != USBD_ERR_NONE) {
        UCOS_Printf("Error adding full speed configuration. USBD_CfgAdd returned error code %d\r\n", usbd_err);
    }

#if (USBD_CFG_HS_EN == DEF_ENABLED)
    if ((cfg_fs_nbr != USBD_CFG_NBR_NONE) &&
        (cfg_hs_nbr != USBD_CFG_NBR_NONE)) {

        USBD_CfgOtherSpeed(dev_nbr,                             /* Associate both config for other spd desc.            */
                           cfg_hs_nbr,
                           cfg_fs_nbr,
                          &usbd_err);
        if (usbd_err != USBD_ERR_NONE) {
            UCOS_Printf("Failure to associate other speed configuration. USBD_CfgOtherSpeed returned error code %d\r\n", usbd_err);
        }
    }
#endif

    App_USBD_HID_Init(dev_nbr, cfg_hs_nbr, cfg_fs_nbr);

    USBD_DevStart(dev_nbr, &usbd_err);
    if (usbd_err != USBD_ERR_NONE) {
        UCOS_Printf("Error starting USB device. USBD_DevStart returned error code %d\r\n", usbd_err);
    }

    return;
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            USB CALLBACKS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        App_USBD_EventReset()
*
* Description : Bus reset event callback function.
*
* Argument(s) : dev_nbr     Device number.
*
* Return(s)   : none.
*
* Caller(s)   : USBD_EventProcess() via 'p_bus_fnct->Reset()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_USBD_EventReset (CPU_INT08U  dev_nbr)
{
    (void)dev_nbr;
}


/*
*********************************************************************************************************
*                                       App_USBD_EventSuspend()
*
* Description : Bus suspend event callback function.
*
* Argument(s) : dev_nbr     Device number.
*
* Return(s)   : none.
*
* Caller(s)   : USBD_EventProcess() via 'p_bus_fnct->Suspend()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_USBD_EventSuspend (CPU_INT08U  dev_nbr)
{
    (void)dev_nbr;
}


/*
*********************************************************************************************************
*                                       App_USBD_EventResume()
*
* Description : Bus Resume event callback function.
*
* Argument(s) : dev_nbr     Device number.
*
* Return(s)   : none.
*
* Caller(s)   : USBD_EventProcess() via 'p_bus_fnct->Resume()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_USBD_EventResume (CPU_INT08U  dev_nbr)
{
    (void)dev_nbr;
}


/*
*********************************************************************************************************
*                                       App_USBD_EventCfgSet()
*
* Description : Set configuration callback event callback function.
*
* Argument(s) : dev_nbr     Device number.
*
*               cfg_nbr     Active device configuration.
*
* Return(s)   : none.
*
* Caller(s)   : USBD_CfgOpen() via 'p_dev->BusFnctsPtr->CfgSet()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_USBD_EventCfgSet (CPU_INT08U  dev_nbr,
                                    CPU_INT08U  cfg_val)
{
    (void)dev_nbr;
    (void)cfg_val;
}


/*
*********************************************************************************************************
*                                       App_USBD_EventCfgClr()
*
* Description : Clear configuration event callback function.
*
* Argument(s) : dev_nbr     Device number.
*
* Return(s)   : none.
*
* Caller(s)   : USBD_CfgClose via 'p_dev->BusFnctsPtr->CfgClr()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_USBD_EventCfgClr (CPU_INT08U  dev_nbr,
                                    CPU_INT08U  cfg_val)
{
    (void)dev_nbr;
    (void)cfg_val;
}


/*
*********************************************************************************************************
*                                        App_USBD_EventConn()
*
* Description : Device connection event callback function.
*
* Argument(s) : dev_nbr     Device number.
*
* Return(s)   : none.
*
* Caller(s)   : USBD_EventProcess() via 'p_bus_fnct->Conn()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_USBD_EventConn (CPU_INT08U  dev_nbr)
{
    (void)dev_nbr;
}


/*
*********************************************************************************************************
*                                       App_USBD_EventDisconn()
*
* Description : Device connection event callback function.
*
* Argument(s) : dev_nbr     USB device number.
*
* Return(s)   : none.
*
* Caller(s)   : USBD_EventProcess() via 'p_bus_fnct->Disconn()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_USBD_EventDisconn (CPU_INT08U  dev_nbr)
{
    (void)dev_nbr;
}


/*
*********************************************************************************************************
*                                            USBD_Trace()
*
* Description : Function to output or log USB trace events.
*
* Argument(s) : p_str       Pointer to string containing the trace event information.
*
* Return(s)   : none.
*
* Caller(s)   : USBD_DbgTaskHandler().
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  USBD_Trace (const  CPU_CHAR  *p_str)
{
    UCOS_Print(p_str);
}
