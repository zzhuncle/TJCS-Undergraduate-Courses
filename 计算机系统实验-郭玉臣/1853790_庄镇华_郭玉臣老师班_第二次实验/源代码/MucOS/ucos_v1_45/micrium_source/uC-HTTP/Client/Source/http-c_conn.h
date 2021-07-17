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
*                                    HTTP CLIENT CONNECTION MODULE
*
* Filename : http-c_conn.h
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

#ifndef  HTTPc_CONN_MODULE_PRESENT                              /* See Note #1.                                         */
#define  HTTPc_CONN_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
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

void          HTTPcConn_Process         (HTTPc_CONN  *p_conn);

void          HTTPcConn_Connect         (HTTPc_CONN  *p_conn,
                                         HTTPc_ERR   *p_err);

void          HTTPcConn_Close           (HTTPc_CONN  *p_conn,
                                         HTTPc_ERR   *p_err);

void          HTTPcConn_TransProcess    (HTTPc_CONN  *p_conn);

void          HTTPcConn_TransParamReset (HTTPc_CONN  *p_conn);

CPU_BOOLEAN   HTTPcConn_TransComplete   (HTTPc_CONN  *p_conn);

void          HTTPcConn_Add             (HTTPc_CONN  *p_conn);

void          HTTPcConn_Remove          (HTTPc_CONN  *p_conn);

void          HTTPcConn_ReqAdd          (HTTPc_REQ   *p_req,
                                         HTTPc_ERR   *p_err);

void          HTTPcConn_ReqRemove       (HTTPc_CONN  *p_conn);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif  /* HTTPc_CONN_MODULE_PRESENT  */
