/*
*********************************************************************************************************
*                                              uC/TCP-IP
*                                      The Embedded TCP/IP Suite
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
*                                       NETWORK UTILITY LIBRARY
*
* Filename : net_base64.h
* Version  : V3.05.04
*********************************************************************************************************
* Note(s)  : (1) N O compiler-supplied standard library functions are used by the network protocol suite.
*                'net_util.*' implements ALL network-specific library functions.
*
*                See also 'net.h  NETWORK INCLUDE FILES  Note #3'.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  NET_BASE64_MODULE_PRESENT
#define  NET_BASE64_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <lib_mem.h>
#include  <Source/net.h>


/*
*********************************************************************************************************
*                                       BASE 64 ENCODER DEFINES
*
* Note(s) : (1) The size of the output buffer the base 64 encoder produces is typically bigger than the
*               input buffer by a factor of (4 x 3).  However, when padding is necessary, up to 3
*               additional characters could by appended.  Finally, one more character is used to NULL
*               terminate the buffer.
*********************************************************************************************************
*/

#define  NET_BASE64_ENCODER_OCTETS_IN_GRP                3
#define  NET_BASE64_ENCODER_OCTETS_OUT_GRP               4

#define  NET_BASE64_DECODER_OCTETS_IN_GRP                4
#define  NET_BASE64_DECODER_OCTETS_OUT_GRP               3

#define  NET_BASE64_ENCODER_PAD_CHAR                    '='
                                                                /* See Note #1.                                         */
#define  NET_BASE64_ENCODER_OUT_MAX_LEN(length)         (((length / 3) * 4) + ((length % 3) == 0 ? 0 : 4) + 1)



void        NetBase64_Encode (CPU_CHAR               *pin_buf,
                              CPU_INT16U              in_len,
                              CPU_CHAR               *pout_buf,
                              CPU_INT16U              out_len,
                              NET_ERR                *p_err);

void        NetBase64_Decode (CPU_CHAR               *pin_buf,
                              CPU_INT16U              in_len,
                              CPU_CHAR               *pout_buf,
                              CPU_INT16U              out_len,
                              NET_ERR                *p_err);

#endif
