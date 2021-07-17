/*
*********************************************************************************************************
*                                               uC/HTTP
*                                     Hypertext Transfer Protocol
*
*                         (c) Copyright 2004-2019; Silicon Laboratories Inc.,
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
*                                      HTTP CLIENT SOCKET MODULE
*
* Filename : http-c_sock.h
* Version  : V3.00.08
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This main network protocol suite header file is protected from multiple pre-processor
*               inclusion through use of the HTTPc module present pre-processor macro definition.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  HTTPc_SOCK_MODULE_PRESENT                          /* See Note #1.                                         */
#define  HTTPc_SOCK_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         HTTPc INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <cpu_core.h>

#include  <lib_def.h>
#include  <lib_str.h>
#include  <lib_ascii.h>

#include  <http-c_cfg.h>
#include  "../../Common/http.h"
#include  "http-c.h"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

void          HTTPcSock_Connect              (HTTPc_CONN  *p_conn,
                                              HTTPc_ERR   *p_err);

CPU_BOOLEAN   HTTPcSock_ConnDataRx           (HTTPc_CONN  *p_conn,
                                              CPU_INT32U   max_len,
                                              HTTPc_ERR   *p_err);

CPU_BOOLEAN   HTTPcSock_ConnDataTx           (HTTPc_CONN  *p_conn,
                                              HTTPc_ERR   *p_err);

void          HTTPcSock_Close                (HTTPc_CONN  *p_conn,
                                              HTTPc_ERR   *p_err);

void          HTTPcSock_Sel                  (HTTPc_CONN  *p_conn,
                                              HTTPc_ERR   *p_err);

CPU_BOOLEAN   HTTPcSock_IsRxClosed           (HTTPc_CONN  *p_conn,
                                              HTTPc_ERR   *p_err);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif  /* HTTPc_SOCK_MODULE_PRESENT  */
