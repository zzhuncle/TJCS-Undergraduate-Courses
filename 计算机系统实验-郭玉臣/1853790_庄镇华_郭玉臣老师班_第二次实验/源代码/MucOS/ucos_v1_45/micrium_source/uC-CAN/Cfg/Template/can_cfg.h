/*
*********************************************************************************************************
*                                               uC/CAN
*                                       The Embedded CAN suite
*
*                         (c) Copyright 2003-2019; Silicon Laboratories Inc.,
*                                400 W. Cesar Chavez, Austin, TX 78701
*
*                   All rights reserved. Protected by international copyright laws.
*
*                  Your use of this software is subject to your acceptance of the terms
*                  of a Silicon Labs Micrium software license, which can be obtained by
*                  contacting info@micrium.com. If you do not agree to the terms of this
*                  license, you may not use this software.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available. Your honesty is greatly appreciated.
*
*                    You can find our product's documentation at: doc.micrium.com
*
*                          For more information visit us at: www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        uC/CAN CONFIGURATION
*
*                                              TEMPLATE
*
* Filename : can_cfg.h
* Version  : V2.42.01
*********************************************************************************************************
*/

#ifndef _CAN_CFG_H_
#define _CAN_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif


/*
*********************************************************************************************************
*                                            CONFIGURATION
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           COMMON DEFINES
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
#define  NULL_PTR                       (void *)0
#endif


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
*                                       DRIVER SPECIFIC DEFINES
*********************************************************************************************************
*/
                                                                /* ---------------- BAUDRATE SETTINGS ----------------- */
#define  CAN_DEFAULT_BAUDRATE             1000000u              /* Default Baudrate                                     */
#define  CAN_DEFAULT_SP                       750u              /* Default Bit Sample Point in 1/10 %                   */
#define  CAN_DEFAULT_RJW                      125u              /* Default Re-Synch Jump Width in 1/10 %                */

                                                                /* ---------------- TIMEOUT SETTINGS ------------------ */
#define  CAN_TIMEOUT_ERR_VAL               100000uL             /* Timeout Value for While Loop Error Checks            */


/*
*********************************************************************************************************
*                                    APPLICATION SPECIFIC DEFINES
*********************************************************************************************************
*/
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
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/
                                                                /* ---------------------- CAN OS ---------------------- */
#if  ((CANOS_ARG_CHK_EN < 0u) || (CANOS_ARG_CHK_EN > 1u))
#error "CANOS_ARG_CHK_EN is invalid; check definition to be 0 or 1!"
#endif

                                                                /* --------------------- CAN FRAME -------------------- */
#if  ((CANFRM_ARG_CHK_EN < 0u) || (CANFRM_ARG_CHK_EN > 1u))
#error "CANFRM_ARG_CHK_EN is invalid; check definition to be 0 or 1!"
#endif

                                                                /* -------------------- CAN SIGNALS ------------------- */
#if  ((CANSIG_EN < 0u) || (CANSIG_EN > 1u))
#error "CANSIG_EN is invalid; check definition to be 0 or 1!"
#endif

#if  ((CANSIG_N < 1u) || (CANSIG_N > 32767u))
#error "CANSIG_N is invalid; check definition to be in range 1 ... 32767!"
#endif

#if  ((CANSIG_ARG_CHK_EN < 0u) || (CANSIG_ARG_CHK_EN > 1u))
#error "CANSIG_ARG_CHK_EN is invalid; check definition to be 0 or 1!"
#endif

#if  ((CANSIG_MAX_WIDTH != 1u) && (CANSIG_MAX_WIDTH != 2u) && (CANSIG_MAX_WIDTH != 4u))
#error "CANSIG_MAX_WIDTH is invalid; check definition to be 1, 2 or 4!"
#endif

#if  ((CANSIG_GRANULARITY < 0u) || (CANSIG_GRANULARITY > 1u))
#error "CANSIG_GRANULARITY is invalid; check definition to be 0 or 1!"
#endif

#if  ((CANSIG_STATIC_CONFIG < 0u) || (CANSIG_STATIC_CONFIG > 1u))
#error "CANSIG_STATIC_CONFIG is invalid; check definition to be 0 or 1!"
#endif

#if  ((CANSIG_USE_DELETE < 0u) || (CANSIG_USE_DELETE > 1u))
#error "CANSIG_USE_DELETE is invalid; check definition to be 0 or 1!"
#endif

#if  ((CANSIG_CALLBACK_EN < 0u) || (CANSIG_CALLBACK_EN > 1u))
#error "CANSIG_CALLBACK_EN is invalid; check definition to be 0 or 1!"
#endif

                                                                /* ------------------- CAN MESSAGES ------------------- */
#if  ((CANMSG_EN < 0u) || (CANMSG_EN > 1u))
#error "CANMSG_EN is invalid; check definition to be 0 or 1!"
#endif

#if  ((CANMSG_N < 1u) || (CANMSG_N > 32767u))
#error "CANMSG_N is invalid; check definition to be in range 1 ... 32767!"
#endif

#if  ((CANMSG_ARG_CHK_EN < 0u) || (CANMSG_ARG_CHK_EN > 1u))
#error "CANMSG_ARG_CHK_EN is invalid; check definition to be 0 or 1!"
#endif

                                                                /* ---------------------- CAN BUS --------------------- */
#if  ((CANBUS_EN < 0u) || (CANBUS_EN > 1u))
#error "CANBUS_EN is invalid; check definition to be 0 or 1!"
#endif

#if   (CANBUS_N < 1u)
#error "CANBUS_N is invalid; check definition to be greater than 0!"
#endif

#if  ((CANBUS_ARG_CHK_EN < 0u) || (CANBUS_ARG_CHK_EN > 1u))
#error "CANBUS_ARG_CHK_EN is invalid; check definition to be 0 or 1!"
#endif

#if  ((CANBUS_TX_HANDLER_EN < 0u) || (CANBUS_TX_HANDLER_EN > 1u))
#error "CANBUS_TX_HANDLER_EN is invalid; check definition to be 0 or 1!"
#endif

#if  ((CANBUS_RX_HANDLER_EN < 0u) || (CANBUS_RX_HANDLER_EN > 1u))
#error "CANBUS_RX_HANDLER_EN is invalid; check definition to be 0 or 1!"
#endif

#if  ((CANBUS_NS_HANDLER_EN < 0u) || (CANBUS_NS_HANDLER_EN > 1u))
#error "CANBUS_NS_HANDLER_EN is invalid; check definition to be 0 or 1!"
#endif

#if  ((CANBUS_STAT_EN < 0u) || (CANBUS_STAT_EN > 1u))
#error "CANBUS_STAT_EN is invalid; check definition to be 0 or 1!"
#endif

#if   (CANBUS_RX_QSIZE < 1u)
#error "CANBUS_RX_QSIZE is invalid; check definition to be greater than 0!"
#endif

#if   (CANBUS_TX_QSIZE < 1u)
#error "CANBUS_TX_QSIZE is invalid; check definition to be greater than 0!"
#endif

#if  ((CANBUS_HOOK_RX_EN < 0u) || (CANBUS_HOOK_RX_EN > 1u))
#error "CANBUS_HOOK_RX_EN is invalid; check definition to be 0 or 1!"
#endif

#if  ((CANBUS_HOOK_NS_EN < 0u) || (CANBUS_HOOK_NS_EN > 1u))
#error "CANBUS_HOOK_NS_EN is invalid; check definition to be 0 or 1!"
#endif

#if  ((CANBUS_RX_READ_ALWAYS_EN < 0u) || (CANBUS_RX_READ_ALWAYS_EN > 1u))
#error "CANBUS_RX_READ_ALWAYS_EN is invalid; check definition to be 0 or 1!"
#endif


/*
*********************************************************************************************************
*                                          CONFIGURATION END
*********************************************************************************************************
*/

#ifdef __cplusplus                                              /* #endif 'C' Extern.                                   */
}
#endif

#endif                                                          /* #ifndef _CAN_CFG_H_                                  */
