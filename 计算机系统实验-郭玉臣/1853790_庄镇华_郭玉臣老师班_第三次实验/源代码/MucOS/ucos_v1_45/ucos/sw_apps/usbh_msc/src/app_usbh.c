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
*                                 USB HOST APPLICATION INITIALIZATION
*
* File : app_usbh.c
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define   APP_USBH_MODULE

#include  <ucos_bsp.h>
#include  "app_usbh.h"

/*
*********************************************************************************************************
*                                              LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                        LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            App_USBH_Init()
*
* Description : Initialize USB Host Stack and additional demos.
*
* Argument(s) : None.
*
* Return(s)   : DEF_OK      if successfull.
*               DEF_FAIL    otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  App_USBH_Init (void)
{
    USBH_ERR    err;

    UCOS_Printf("... Initiliazing HOST Mass Storage class ...\r\n");
    err = App_USBH_MSC_Init();\

    if (err != USBH_ERR_NONE) {
    	UCOS_Printf("...could not initialize HOST Mass Storage Class w/err = %d\r\n\r\n", err);
        return (DEF_FAIL);
    }


    return (DEF_OK);
}
