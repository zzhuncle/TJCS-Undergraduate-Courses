/*
*********************************************************************************************************
*                                             uC/TELNETs
*                                           Telnet (server)
*
*                         (c) Copyright 2004-2018; Silicon Laboratories Inc.,
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
*
*                                  TELNET SERVER CONFIGURATION FILE
*
*                                              TEMPLATE
*
* Filename : telnet-s_cfg.h
* Version  : V1.05.03
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           TASKS PRIORITIES
*********************************************************************************************************
*/

#define  TELNETs_OS_CFG_SERVER_TASK_PRIO                  15
#define  TELNETs_OS_CFG_SESSION_TASK_PRIO                 16


/*
*********************************************************************************************************
*                                              STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  TELNETs_OS_CFG_SERVER_TASK_STK_SIZE             512
#define  TELNETs_OS_CFG_SESSION_TASK_STK_SIZE            512


/*
*********************************************************************************************************
*                                                TELNETs
*
* Note(s) : (1) Default server port for TELNET server is 23.
*
*           (2) Default secure port for TELNET server is 992.
*
*           (3) The inactivity timeout defines the maximum amount of time the server waits for a client
*               to transmit data.  Once this time has expired, the server closes the connection and waits
*               for other client to connect.
*
*           (4) The receive buffer is used to hold the data as well as the incoming option requests and
*               replies.  Its size should hence be slightly larger than the NVT buffer.  This later should
*               be defined to be as large as the longest line of data transmitted by the client.
*
*           (5) Configure TELNETs_CFG_FS_EN to enable/disable file system in this module.  If enabled,
*               make sure a file system in included in your product application BEFORE including
*               uC/TELNETs.
*
*           (6) The maximum path name configuration define should be set in accordance with the file
*               system being used on your target, and only applies if a file system is configured with
*               this module.  This default setting is the appropriate one for uC/FS V3 & V4.
*********************************************************************************************************
*/

#define  TELNETs_CFG_PORT_SERVER                          23    /* Cfg TELNETs server port. Dflt is  23.                */
#define  TELNETs_CFG_PORT_SERVER_SECURE                  992    /* Cfg TELNETs secure port. Dflt is 992.                */

#define  TELNETs_CFG_INACTIVITY_TIMEOUT_S                 30    /* Cfg TELNET inactivity time (see Note #3).            */
#define  TELNETs_CFG_MAX_TX_TRIES                          3    /* Cfg max number of tries on tx.                       */

#define  TELNETs_CFG_RX_BUF_LEN                          100    /* Cfg TELNET rx  buf len     (see Note #4).            */
#define  TELNETs_CFG_NVT_BUF_LEN                          80    /* Cfg TELNET NVT buf len     (see Note #4).            */

#define  TELNETs_CFG_MAX_USR_NAME_LEN                     32    /* Cfg max len for usr name.                            */
#define  TELNETs_CFG_MAX_PW_LEN                           32    /* Cfg max len for pw.                                  */

#define  TELNETs_CFG_MAX_LOGIN_TRIES                       3    /* Cfg max number of tries on login.                    */

                                                                /* Cfg FS presence            (see Note #5) :           */
#define  TELNETs_CFG_FS_EN                       DEF_DISABLED
                                                                /*   DEF_DISABLED  FS DISABLED for this module.         */
                                                                /*   DEF_ENABLED   FS ENABLED  for this module.         */

                                                                /* Cfg Echo.                                            */
#define  TELNETs_CFG_ECHO_EN                     DEF_ENABLED
                                                                /*   DEF_DISABLED  Echo DISABLED.                       */
                                                                /*   DEF_ENABLED   Echo ENABLED.                        */

                                                                /* Cfg max path name len      (see Note #6).            */
#define  TELNETs_CFG_FS_MAX_PATH_NAME_LEN               256u

                                                                /* Cfg TELNET welcome msg.                              */
#define  TELNETs_CFG_WELCOME_MSG_STR              "\x0D\x0A\x0D\x0A"  \
                                                  "Welcome to Micrium Telnet Server\x0D\x0A\x0D\x0A\x0D\x0A"


