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
*                                        uC/CAN CONFIGURATION
*
*                                         ZYNQ ZC7000 Series
*                                             CAN DRIVER
*
* File : can_cfg.h
*********************************************************************************************************
*/

#ifndef _CAN_CFG_H_
#define _CAN_CFG_H_

#include  "lib_def.h"


/*
*********************************************************************************************************
*                                    COMMON DEFINES & ENUMERATIONS
*********************************************************************************************************
*/

                                                                /* Definiton for CANSIG_GRANULARITY, Options:           */
#define  CAN_CFG_BIT                            0u              /*      BIT                                             */
#define  CAN_CFG_BYTE                           1u              /*      BYTE                                            */

#ifndef  CAN_FALSE
#define  CAN_FALSE                              0u
#endif

#ifndef  CAN_TRUE
#define  CAN_TRUE                               1u
#endif

#ifndef  NULL_PTR
#define  NULL_PTR                              (void *)0
#endif

                                                                /* ------------ APPLICATION ENUMERATIONS -------------- */
enum {
   S_NODESTATUS = 0,
   S_CPULOAD,
   S_COUNTER,
   S_MAX,
};

enum {
   M_STATUS = 0,
   M_COMMAND,
   M_MAX
};


/*
*********************************************************************************************************
*                                      MULTIPLE CAN CONTROLLERS
*********************************************************************************************************
*/

#define  CAN_MODULE_CHANNEL_0                   DEF_ENABLED
#define  CAN_MODULE_CHANNEL_1                   DEF_DISABLED


/*
*********************************************************************************************************
*                                       DRIVER SPECIFIC DEFINES
*********************************************************************************************************
*/
                                                                /* ---------------- BAUDRATE SETTINGS ----------------- */
#define  CAN_DEFAULT_BAUDRATE             1000000u              /* Default Baudrate                                     */
#define  CAN_DEFAULT_SP                       750u              /* Default Bit Sample Point in 1/10 %                   */
#define  CAN_DEFAULT_RJW                      125u              /* Default Re-Synch Jump Width in 1/10 %                */

                                                                /* ---------------- TIMEOUT SETTINGS ------------------ */
#define  CAN_TIMEOUT_ERR_VAL               100000uL             /* Timeout Value for While Loop Error Checks            */


/* ==================================  ADVANCED DRIVER CONFIGURATION: DEFAULT VALUES ================================== */
/*  By Default, the following Driver specific settings for the ZYNQ ZC7xxx Driver are set to their default values       */
/*  unless they are modified by customer needs.                                                                         */
/*                                                                                                                      */
/*  By Default, the Watermark level is configured to Maximum Watermark Value in the Driver, based on the reset value    */
/*  presented by the Reference Manual. Redefine the following define to modify the Watermark Level for the following.   */
/*      Tx FIFO Empty & Rx FIFO Full Watermark Level(s):                                                                */
/*          NOTE : The VALID range is between 1 & 63.                                                                   */
/*                                                                                                                      */
/*          #define  CAN_WATERMARK_Rx_Tx_SIZE              63u                                                          */
/*                                                                                                                      */
/*  By Default, the Operating Mode of the CAN controller is configured to "NORMAL" Mode. For diagnostic checking,       */
/*  additional operating modes have been included in the driver. Redefine the following define to modify the Operating  */
/*  Mode to either "LOOP BACK" or "SNOOP" Mode(s).                                                                      */
/*      NOTE that only one operating mode can be selected at once at Initialization. Once CAN has been Initialized it   */
/*           is possible to change between Operating Modes at run-time using the xxx_CAN_IoCtl() API Function call.     */
/*                                                                                                                      */
/*          #define  CAN_DIAGNOSTIC_OFF                     0u                                                          */
/*          #define  CAN_DIAGNOSTIC_LOOPBACK                1u                                                          */
/*          #define  CAN_DIAGNOSTIC_SNOOP                   2u                                                          */
/*                                                                                                                      */
/*          #define  CAN_DIAGNOSTIC_SELECT                  CAN_DIAGNOSTIC_LOOPBACK                                     */
/*                                                                                                                      */
/* ==================================================================================================================== */


/*
*********************************************************************************************************
*                                               CAN BUS
*********************************************************************************************************
*/

#define  CANBUS_EN                              1u              /* Enable CAN Bus Management                            */
#define  CANBUS_N                               3u              /*   Number of busses                                   */
#define  CANBUS_ARG_CHK_EN                      1u              /*   Enable runtime argument checking                   */
#define  CANBUS_TX_HANDLER_EN                   1u              /*   Enable usage of CanBusTxHandler                    */
#define  CANBUS_RX_HANDLER_EN                   1u              /*   Enable usage of CanBusRxHandler                    */
#define  CANBUS_NS_HANDLER_EN                   1u              /*   Enable usage of CanBusNsHandler                    */

#define  CANBUS_STAT_EN                         1u              /*   Enable Bus Statistics                              */
#define  CANBUS_TX_QSIZE                 (2u * CANBUS_N)        /*   Transmit Queue Size in CAN Frames for each CAN Bus */
#define  CANBUS_RX_QSIZE                 (2u * CANBUS_N)        /*   Receive Queue Size in CAN Frames for each CAN Bus  */

#define  CANBUS_HOOK_NS_EN                      1u              /*   Enable Node Status Handler Hook Function           */
#define  CANBUS_HOOK_RX_EN                      1u              /*   Enable Rx Handler Hook Function                    */
#define  CANBUS_RX_READ_ALWAYS_EN               1u              /*   If enabled the Rx Handler executes a read even..   */
                                                                /*   .. when frames can't be allocated                  */


/*
*********************************************************************************************************
*                                             CAN MESSAGE
*********************************************************************************************************
*/

#define  CANMSG_EN                              1u              /* Enable CAN Message Support                           */
#define  CANMSG_N                               2u              /*   Number of messages                                 */
#define  CANMSG_ARG_CHK_EN                      1u              /*   Enable runtime argument checking                   */


/*
*********************************************************************************************************
*                                             CAN SIGNAL
*********************************************************************************************************
*/

#define  CANSIG_EN                              1u              /* Enable CAN Signal Database                           */
#define  CANSIG_N                               3u              /*   Number of signals                                  */
#define  CANSIG_ARG_CHK_EN                      1u              /*   Enable runtime argument checking                   */
#define  CANSIG_MAX_WIDTH                       4u              /*   Maximal signal width in byte                       */
#define  CANSIG_GRANULARITY               CAN_CFG_BYTE          /*   Set signal resolution to byte                      */
#define  CANSIG_STATIC_CONFIG                   1u              /* To reduce memory usage, declare static signal table  */
#define  CANSIG_USE_DELETE                      0u              /* To reduce memory usage don't use delete functions    */
#define  CANSIG_CALLBACK_EN                     0u              /* Enable callback functions                            */


/*
*********************************************************************************************************
*                                              CAN FRAME
*********************************************************************************************************
*/

#define  CANFRM_ARG_CHK_EN                      1u              /* Enable runtime argument checking                     */


/*
*********************************************************************************************************
*                                               CAN OS
*********************************************************************************************************
*/

#define  CANOS_ARG_CHK_EN                       1u              /* Enable runtime argument checking                     */


/*
*********************************************************************************************************
*                                          CONFIGURATION END
*********************************************************************************************************
*/

#endif  /* #ifndef _CAN_CFG_H_ */
