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
*                                     HTTP CLIENT MEMORY LIBRARY
*
* Filename : http-c_mem.h
* Version  : V3.00.08
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This main network protocol suite header file is protected from multiple pre-processor
*               inclusion through use of the HTTP memory module present pre-processor macro definition.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  HTTPc_MEM_PRESENT                                          /* See Note #1.                                         */
#define  HTTPc_MEM_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  "http-c.h"
#ifdef  HTTPc_WEBSOCK_MODULE_EN
#include  "http-c_websock.h"
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/

typedef enum {
    HTTPc_MSG_TYPE_CONN_OPEN,
    HTTPc_MSG_TYPE_CONN_CLOSE,
    HTTPc_MSG_TYPE_REQ,
    HTTPc_MSG_TYPE_WEBSOCK_MSG,
} HTTPc_MSG_TYPE;

typedef struct httpc_task_msg {
    HTTPc_MSG_TYPE  Type;
    void           *DataPtr;
} HTTPc_TASK_MSG;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

void               HTTPc_Mem_TaskMsgPoolInit    (const  HTTPc_CFG           *p_cfg,
                                                        MEM_SEG             *p_seg,
                                                        HTTPc_ERR           *p_err);

HTTPc_TASK_MSG    *HTTPc_Mem_TaskMsgGet         (void);

void               HTTPc_Mem_TaskMsgRelease     (HTTPc_TASK_MSG      *p_msg);


#ifdef  HTTPc_WEBSOCK_MODULE_EN
void               HTTPc_Mem_WebSockReqPoolInit (const HTTPc_CFG           *p_cfg,
                                                       MEM_SEG             *p_seg,
                                                       HTTPc_ERR           *p_err);

HTTPc_WEBSOCK_REQ *HTTPc_Mem_WebSockReqGet      (void);

void               HTTPc_Mem_WebSockReqRelease  (HTTPc_WEBSOCK_REQ   *p_ws_req);
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif                                                          /* End of HTTPc memory module include.                  */

