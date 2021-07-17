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
*                                      HTTP SERVER SOCKET MODULE
*
* Filename : http-s_sock.h
* Version  : V3.00.08
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This main network protocol suite header file is protected from multiple pre-processor
*               inclusion through use of the HTTPs module present pre-processor macro definition.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  HTTPs_SOCK_MODULE_PRESENT                          /* See Note #1.                                         */
#define  HTTPs_SOCK_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <cpu_core.h>

#include  "http-s.h"

/*
*********************************************************************************************************
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

NET_SOCK_ID   HTTPsSock_ListenInit   (const  HTTPs_CFG                 *p_cfg,
                                             NET_SOCK_PROTOCOL_FAMILY   family,
                                             HTTPs_ERR                 *p_err);

void          HTTPsSock_ListenClose  (       HTTPs_INSTANCE            *p_instance,
                                             NET_SOCK_ID                sock_listen_id);

NET_SOCK_QTY  HTTPsSock_ConnSel      (       HTTPs_INSTANCE            *p_instance,
                                             CPU_BOOLEAN                accept);

CPU_BOOLEAN   HTTPsSock_ConnDataRx   (       HTTPs_INSTANCE            *p_instance,
                                             HTTPs_CONN                *p_conn);

CPU_BOOLEAN   HTTPsSock_ConnDataTx   (       HTTPs_INSTANCE            *p_instance,
                                             HTTPs_CONN                *p_conn);

void          HTTPsSock_ConnClose    (       HTTPs_INSTANCE            *p_instance,
                                             HTTPs_CONN                *p_conn);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif  /* HTTPs_SOCK_MODULE_PRESENT  */
