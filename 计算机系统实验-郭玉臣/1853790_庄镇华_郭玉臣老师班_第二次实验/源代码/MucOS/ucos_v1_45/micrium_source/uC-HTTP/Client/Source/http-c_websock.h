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
*                                    HTTP CLIENT WEBSOCKET MODULE
*
* Filename : http-c_websock.h
* Version  : V3.00.08
*********************************************************************************************************
*/

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

#include  <Source/net.h>
#include  <Modules/Common/net_base64.h>
#include  <Modules/Common/net_sha1.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This main network protocol suite header file is protected from multiple pre-processor
*               inclusion through use of the HTTPc module present pre-processor macro definition.
*
*           (2) The following HTTPc-WebSocket-module configuration value MUST be pre-#define'd in
*               'http-c.h' PRIOR to all other HTTPc modules that require the WebSocket Layer configuration:
*
*                   HTTPc_WEBSOCK_MODULE_EN
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  HTTPc_WEBSOCK_MODULE_PRESENT                           /* See Note #1.                                         */
#define  HTTPc_WEBSOCK_MODULE_PRESENT

#ifdef   HTTPc_WEBSOCK_MODULE_EN                                /* See Note #2.                                         */


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                     WEBSOCKET HANDSHAKE DEFINES
*********************************************************************************************************
*/


#define  HTTPc_WEBSOCK_GUID_STRING                  "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#define  HTTPc_WEBSOCK_GUID_STRING_LEN               sizeof(HTTPc_WEBSOCK_GUID_STRING)

#define  HTTPc_WEBSOCK_PROTOCOL_VERSION_13           13u
#define  HTTPc_WEBSOCK_PROTOCOL_VERSION_13_STR      "13"
#define  HTTPc_WEBSOCK_PROTOCOL_VERSION_13_STR_LEN   sizeof(HTTPc_WEBSOCK_PROTOCOL_VERSION_13_STR) - 1

#define  HTTPc_WEBSOCK_KEY_LEN                       16
#define  HTTPc_WEBSOCK_KEY_ENCODED_LEN               NET_BASE64_ENCODER_OUT_MAX_LEN(HTTPc_WEBSOCK_KEY_LEN)
#define  HTTPc_WEBSOCK_KEY_PRE_HASH_LEN              (HTTPc_WEBSOCK_KEY_ENCODED_LEN - 1) + (HTTPc_WEBSOCK_GUID_STRING_LEN - 1)
#define  HTTPc_WEBSOCK_KEY_HASH_LEN                  NET_SHA1_HASH_SIZE
#define  HTTPc_WEBSOCK_KEY_HASH_ENCODED_LEN          NET_BASE64_ENCODER_OUT_MAX_LEN(NET_SHA1_HASH_SIZE)


/*
*********************************************************************************************************
*                                   WEBSOCKET FRAME LEN DEFINITION
*********************************************************************************************************
*/

#define  HTTPc_WEBSOCK_SMALL_FRAME_MIN_LEN            0u
#define  HTTPc_WEBSOCK_SMALL_FRAME_MAX_LEN            125u

#define  HTTPc_WEBSOCK_NORMAL_FRAME_MIN_LEN           126u
#define  HTTPc_WEBSOCK_NORMAL_FRAME_MAX_LEN           DEF_INT_16U_MAX_VAL
#define  HTTPc_WEBSOCK_NORMAL_FRAME_CODE              126u

#define  HTTPc_WEBSOCK_LONG_FRAME_MIN_LEN             DEF_INT_16U_MAX_VAL + 1
#define  HTTPc_WEBSOCK_LONG_FRAME_MAX_LEN             DEF_INT_32U_MAX_VAL
#define  HTTPc_WEBSOCK_LONG_FRAME_CODE                127u

#define  HTTPc_WEBSOCK_MAX_CTRL_FRAME_LEN             HTTPc_WEBSOCK_SMALL_FRAME_MAX_LEN

#define  HTTPc_WEBSOCK_CLOSE_CODE_LEN                 sizeof(CPU_INT16U)
#define  HTTPc_WEBSOCK_MAX_CLOSE_REASON_LEN           HTTPc_WEBSOCK_MAX_CTRL_FRAME_LEN - HTTPc_WEBSOCK_CLOSE_CODE_LEN


/*
*********************************************************************************************************
*                                       WEBSOCKET REQUEST FLAGS
*********************************************************************************************************
*/

#define  HTTPc_FLAG_WEBSOCK_REQ_CONN_UPGRADE                   DEF_BIT_00
#define  HTTPc_FLAG_WEBSOCK_REQ_UPGRADE_WEBSOCKET              DEF_BIT_01
#define  HTTPc_FLAG_WEBSOCK_REQ_ACCEPT_VALIDATED               DEF_BIT_02
#define  HTTPc_FLAG_WEBSOCK_REQ_VERSION_VALIDATED              DEF_BIT_03
#define  HTTPc_FLAG_WEBSOCK_REQ_IN_USE                         DEF_BIT_04


#define  HTTPc_FLAG_WEBSOCK_ALL       (HTTPc_FLAG_WEBSOCK_REQ_CONN_UPGRADE      | \
                                       HTTPc_FLAG_WEBSOCK_REQ_UPGRADE_WEBSOCKET | \
                                       HTTPc_FLAG_WEBSOCK_REQ_ACCEPT_VALIDATED)


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                  WEBSOCKET CLOSE REASON STRUCTURE
*********************************************************************************************************
*/

typedef struct httpc_websock_close_reason {
    CPU_CHAR     *DataPtr;
    CPU_INT08U    Len;
}HTTPc_WEBSOCK_CLOSE_REASON;


/*
*********************************************************************************************************
*                                            WEBSOCKET FLAGS
*********************************************************************************************************
*/

typedef struct httpc_flag_websock_msg {
    CPU_INT08U  IsUsed          : 1;
    CPU_INT08U  IsNoBlock       : 1;
    CPU_INT08U  IsFin           : 1;
    CPU_INT08U  IsMasked        : 1;
    CPU_INT08U  IsHdrSet        : 1;
    CPU_INT08U  IsCompleted     : 1;
} HTTPc_FLAG_WEBSOCK_MSG;

typedef struct httpc_flag_websock {
    CPU_INT08U  IsTxMsgCtrlUsed : 1;
    CPU_INT08U  IsRxDataCached  : 1;
    CPU_INT08U  IsWebsockUsed   : 1;
    CPU_INT08U  IsCloseStarted  : 1;
    CPU_INT08U  IsPongStarted   : 1;

} HTTPc_FLAG_WEBSOCK;

typedef struct httpc_flag_websock_rx  {
    CPU_INT08U  IsFin    : 1;
    CPU_INT08U  IsMasked : 1;
} HTTPc_FLAG_WEBSOCK_RX;


/*
*********************************************************************************************************
*                                        WEBSOCKET INTERNAL STATES
*********************************************************************************************************
*/

typedef enum {
    HTTPc_WEBSOCK_TX_STATE_MSG_INIT,
    HTTPc_WEBSOCK_TX_STATE_SET_BUF,
    HTTPc_WEBSOCK_TX_STATE_SEND_BUF,
    HTTPc_WEBSOCK_TX_STATE_COMPLETE,
} HTTPc_WEBSOCK_TX_STATE;


typedef enum {
    HTTPc_WEBSOCK_RX_STATE_WAIT,
    HTTPc_WEBSOCK_RX_STATE_HDR,
    HTTPc_WEBSOCK_RX_STATE_PREPARE,
    HTTPc_WEBSOCK_RX_STATE_PAYLOAD,
    HTTPc_WEBSOCK_RX_STATE_COMPLETE,
} HTTPc_WEBSOCK_RX_STATE;


/*
*********************************************************************************************************
*                                  HOOK & CALLBACK FUNCTIONS DATA TYPE
*********************************************************************************************************
*/

typedef  void         (*HTTPc_WEBSOCK_ON_OPEN)             (HTTPc_CONN_OBJ                *p_conn);

typedef  void         (*HTTPc_WEBSOCK_ON_CLOSE)            (HTTPc_CONN_OBJ                *p_conn,
                                                            HTTPc_WEBSOCK_CLOSE_CODE       close_code,
                                                            HTTPc_WEBSOCK_CLOSE_REASON    *p_reason);

typedef  void         (*HTTPc_WEBSOCK_ON_RX_INIT)          (HTTPc_CONN_OBJ                *p_conn,
                                                            HTTPc_WEBSOCK_MSG_TYPE         msg_type,
                                                            CPU_INT32U                     msg_len,
                                                            void                         **p_data);

typedef  CPU_INT32U   (*HTTPc_WEBSOCK_ON_RX_DATA)          (HTTPc_CONN_OBJ                *p_conn,
                                                            HTTPc_WEBSOCK_MSG_TYPE         msg_type,
                                                            void                          *p_data,
                                                            CPU_INT16U                     data_len);

typedef  void         (*HTTPc_WEBSOCK_ON_RX_COMPLETE)      (HTTPc_CONN_OBJ                *p_conn);


typedef  CPU_INT32U   (*HTTPc_WEBSOCK_ON_TX_INIT)          (HTTPc_CONN_OBJ                *p_conn,
                                                            HTTPc_WEBSOCK_MSG_OBJ         *p_ws_msg);


typedef  CPU_INT32U   (*HTTPc_WEBSOCK_ON_TX_DATA)          (HTTPc_CONN_OBJ                *p_conn,
                                                            HTTPc_WEBSOCK_MSG_OBJ         *p_websock_msg,
                                                            CPU_CHAR                      *p_buf,
                                                            CPU_INT32U                     buf_len);

typedef  void         (*HTTPc_WEBSOCK_ON_TX_COMPLETE)      (HTTPc_CONN_OBJ                *p_conn,
                                                            HTTPc_WEBSOCK_MSG_OBJ         *p_websock_msg,
                                                            CPU_BOOLEAN                    status);

typedef  void         (*HTTPc_WEBSOCK_ON_ERR)              (HTTPc_CONN_OBJ                *p_conn,
                                                            HTTPc_ERR                      err);

typedef  void         (*HTTPc_WEBSOCK_ON_PONG)             (HTTPc_CONN_OBJ                *p_conn,
                                                            CPU_CHAR                      *p_data,
                                                            CPU_INT16U                     data_len);


/*
*********************************************************************************************************
*                                WEBSOCKET MSG TASK CALLBACK FIELDS MACRO
*********************************************************************************************************
*/

#define  TASK_WEBSOCK_MSG_CALLBACK_FIELDS_DEF(prefix, suffix)  FIELD_DEF(prefix,  HTTPc_WEBSOCK_ON_TX_INIT,      OnMsgTxInit,       suffix) \
                                                               FIELD_DEF(prefix,  HTTPc_WEBSOCK_ON_TX_DATA,      OnMsgTxData,       suffix) \
                                                               FIELD_DEF(prefix,  HTTPc_WEBSOCK_ON_TX_COMPLETE,  OnMsgTxComplete,   suffix)


/*
*********************************************************************************************************
*                                WEBSOCKET TASK CALLBACK FIELDS MACRO
*********************************************************************************************************
*/

#define  TASK_WEBSOCK_CALLBACK_FIELDS_DEF(prefix, suffix)  FIELD_DEF(prefix,  HTTPc_WEBSOCK_ON_OPEN,         OnOpen,            suffix) \
                                                           FIELD_DEF(prefix,  HTTPc_WEBSOCK_ON_CLOSE,        OnClose,           suffix) \
                                                           FIELD_DEF(prefix,  HTTPc_WEBSOCK_ON_ERR,          OnErr,             suffix) \
                                                           FIELD_DEF(prefix,  HTTPc_WEBSOCK_ON_RX_INIT,      OnMsgRxInit,       suffix) \
                                                           FIELD_DEF(prefix,  HTTPc_WEBSOCK_ON_RX_DATA,      OnMsgRxData,       suffix) \
                                                           FIELD_DEF(prefix,  HTTPc_WEBSOCK_ON_RX_COMPLETE,  OnMsgRxComplete,   suffix) \
                                                           FIELD_DEF(prefix,  HTTPc_WEBSOCK_ON_PONG,         OnPong,            suffix)


/*
*********************************************************************************************************
*                                     WEBSOCKET REQUEST STRUCTURE
*********************************************************************************************************
*/

struct httpc_websock_req {
    CPU_CHAR       Key[HTTPc_WEBSOCK_KEY_ENCODED_LEN];
    CPU_CHAR       Accept[HTTPc_WEBSOCK_KEY_HASH_ENCODED_LEN];
    CPU_INT08U     Flags;
    HTTPc_WEBSOCK *WebSockObjPtr;
};


/*
*********************************************************************************************************
*                                  WEBSOCKET MESSAGE STRUCTURE MACRO
*********************************************************************************************************
*/

#define  STRUCT_WS_MSG_INIT(prefix, suffix)  \
    FIELD_DEF(prefix,  HTTPc_FLAG_WEBSOCK_MSG,     Flags,            suffix) \
    FIELD_DEF(prefix,  HTTPc_WEBSOCK_OPCODE,       OpCode,           suffix) \
    FIELD_DEF(prefix,  CPU_INT32U,                 Len,              suffix) \
    FIELD_DEF(prefix,  CPU_INT32U,                 DataLen,          suffix) \
    FIELD_DEF(prefix,  CPU_INT32U,                 LenSent,          suffix) \
    FIELD_DEF(prefix,  CPU_INT32U,                 MskKey,           suffix) \
    FIELD_DEF(prefix,  void,                      *DataPtr,          suffix) \
    FIELD_DEF(prefix,  HTTPc_CONN,                *ConnPtr,          suffix) \
    FIELD_DEF(prefix,  HTTPc_WEBSOCK_MSG,         *NextPtr,          suffix) \
    TASK_WEBSOCK_MSG_CALLBACK_FIELDS_DEF(prefix, suffix)                     \
    USER_DATA_FIELD_DEF(,)

struct httpc_websock_msg_obj {
    STRUCT_WS_MSG_INIT(const, _reserved)
};

struct httpc_websock_msg {
    STRUCT_WS_MSG_INIT(,)
};


/*
*********************************************************************************************************
*                                      WEBSOCKET STRUCTURE MACRO
*********************************************************************************************************
*/

#define  STRUCT_WS_INIT(prefix, suffix)  \
    FIELD_DEF(prefix,  HTTPc_FLAG_WEBSOCK,         Flags,            suffix) \
    FIELD_DEF(prefix,  HTTPc_WEBSOCK_MSG,         *TxMsgListHeadPtr, suffix) \
    FIELD_DEF(prefix,  HTTPc_WEBSOCK_MSG,         *TxMsgListEndPtr,  suffix) \
    FIELD_DEF(prefix,  HTTPc_WEBSOCK_MSG,          TxMsgCtrl,        suffix) \
    FIELD_DEF(prefix,  HTTPc_WEBSOCK_OPCODE,       RxMsgOpCode,      suffix) \
    FIELD_DEF(prefix,  HTTPc_FLAG_WEBSOCK_RX,      RxMsgFlags,       suffix) \
    FIELD_DEF(prefix,  CPU_INT32U,                 RxMsgLen,         suffix) \
    FIELD_DEF(prefix,  CPU_INT32U,                 RxMsgLenRead,     suffix) \
    FIELD_DEF(prefix,  CPU_INT32U,                 RxMsgMskKey,      suffix) \
    FIELD_DEF(prefix,  CPU_CHAR,                  *RxMsgDataPtr,     suffix) \
    FIELD_DEF(prefix,  HTTPc_WEBSOCK_TX_STATE,     TxState,          suffix) \
    FIELD_DEF(prefix,  HTTPc_WEBSOCK_RX_STATE,     RxState,          suffix) \
    FIELD_DEF(prefix,  HTTPc_WEBSOCK_CLOSE_CODE,   CloseCode,        suffix) \
    FIELD_DEF(prefix,  HTTPc_WEBSOCK_CLOSE_REASON, CloseReason,      suffix) \
    FIELD_DEF(prefix,  CPU_INT32U,                 TotalMsgLen,      suffix) \
    FIELD_DEF(prefix,  HTTPc_WEBSOCK_OPCODE,       OrigOpCode,       suffix) \
    TASK_WEBSOCK_CALLBACK_FIELDS_DEF(prefix, suffix)                         \
    USER_DATA_FIELD_DEF(,)

struct httpc_websock_obj {
    STRUCT_WS_INIT(const, _reserved)
};

struct httpc_websock {
    STRUCT_WS_INIT(,)
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

void               HTTPcWebSock_Process        (HTTPc_CONN         *p_conn);

HTTPc_WEBSOCK_REQ *HTTPcWebSock_InitReqObj     (HTTPc_ERR          *p_err);

void               HTTPcWebSock_TxMsgAdd       (HTTPc_WEBSOCK_MSG  *p_msg,
                                                HTTPc_ERR          *p_err);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/
#endif  /* HTTPc_WEBSOCK_MODULE_EN       */
#endif  /* HTTPc_WEBSOCK_MODULE_PRESENT  */
