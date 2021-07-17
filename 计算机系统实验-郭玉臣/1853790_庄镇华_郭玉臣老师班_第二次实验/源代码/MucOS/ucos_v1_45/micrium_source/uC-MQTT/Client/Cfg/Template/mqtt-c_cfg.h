/*
*********************************************************************************************************
*                                               uC/MQTTc
*                                Message Queue Telemetry Transport Client
*
*                         (c) Copyright 2014-2017; Micrium, Inc.; Weston, FL
*
*                  All rights reserved.  Protected by international copyright laws.
*
*                  uC/MQTTc is provided in source form to registered licensees ONLY.  It is
*                  illegal to distribute this source code to any third party unless you receive
*                  written permission by an authorized Micrium representative.  Knowledge of
*                  the source code may NOT be used to develop a similar product.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available.  Your honesty is greatly appreciated.
*
*                  You can find our product's user manual, API reference, release notes and
*                  more information at: https://doc.micrium.com
*
*                  You can contact us at: http://www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                    MQTT CLIENT CONFIGURATION FILE
*
*                                               TEMPLATE
*
* Filename      : mqtt-c_cfg.h
* Version       : V1.01.00
* Programmer(s) : OD
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <lib_def.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         CONFIGURATION DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            ARG CHK DEFINE
*********************************************************************************************************
*/
                                                                /* Enable to add extra checks on ext vars.              */
#define  MQTTc_CFG_ARG_CHK_EXT_EN               DEF_DISABLED


/*
*********************************************************************************************************
*                                              DBG DEFINES
*********************************************************************************************************
*/

                                                                /* ------------------- TRACE DEFINES ------------------ */
                                                                /* Set trace to function to output trace data.          */
#define  MQTTc_CFG_DBG_TRACE                    /*printf*/
                                                                /* Set trace level to higher than OFF, to obtain data.  */
#define  MQTTc_CFG_DBG_TRACE_LEVEL              TRACE_LEVEL_OFF

                                                                /* -------------- GLOBAL DBG BUF DEFINES -------------- */
                                                                /* Enables dbg buf where data is copied at checkpoints. */
#define  MQTTc_CFG_DBG_GLOBAL_BUF_EN            DEF_DISABLED
                                                                /* Size of dbg buf.                                     */
#define  MQTTc_CFG_DBG_GLOBAL_BUF_LEN                   512u

