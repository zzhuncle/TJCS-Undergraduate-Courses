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
*                                     HTTP CLIENT REQUEST MODULE
*
* Filename : http-c_req.h
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

#ifndef  HTTPc_REQ_MODULE_PRESENT                          /* See Note #1.                                         */
#define  HTTPc_REQ_MODULE_PRESENT


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

#include  <Source/net.h>
#include  <Source/net_cfg_net.h>

#include  <http-c_cfg.h>
#include  "../../Common/http.h"
#include  "http-c.h"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#define  HTTPc_STR_BOUNDARY                  "rgifovj80325n"

#define  HTTPc_STR_BOUNDARY_LEN              (sizeof(HTTPc_STR_BOUNDARY) -1)

#define  HTTPc_STR_BOUNDARY_START            "--" HTTPc_STR_BOUNDARY

#define  HTTPc_STR_BOUNDARY_END              "--" HTTPc_STR_BOUNDARY "--"

#define  HTTPc_STR_BOUNDARY_START_LEN        (sizeof(HTTPc_STR_BOUNDARY_START) - 1)

#define  HTTPc_STR_BOUNDARY_END_LEN          (sizeof(HTTPc_STR_BOUNDARY_END) - 1)


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

void          HTTPcReq_Prepare      (       HTTPc_CONN      *p_conn,
                                            HTTPc_ERR       *p_err);

CPU_BOOLEAN   HTTPcReq              (       HTTPc_CONN      *p_conn,
                                            HTTPc_ERR       *p_err);

CPU_CHAR     *HTTPcReq_HdrCopyToBuf (       CPU_CHAR        *p_buf,         // TODO put function in HTTP common files ?
                                            CPU_INT16U       buf_len,
                                            CPU_SIZE_T       buf_len_rem,
                                            HTTP_HDR_FIELD   hdr_type,
                                     const  CPU_CHAR        *p_val,
                                            CPU_SIZE_T       val_len,
                                            CPU_BOOLEAN      add_CRLF,
                                            HTTPc_ERR       *p_err);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif  /* HTTPc_REQ_MODULE_PRESENT  */
