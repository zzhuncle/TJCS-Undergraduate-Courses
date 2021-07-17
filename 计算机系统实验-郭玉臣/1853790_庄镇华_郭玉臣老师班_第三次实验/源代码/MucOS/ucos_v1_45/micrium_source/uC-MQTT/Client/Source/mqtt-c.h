/*
*********************************************************************************************************
*                                               uC/MQTTc
*                                Message Queue Telemetry Transport Client
*
*                         (c) Copyright 2014-2017; Micrium, Inc.; Weston, FL
*
*                  All rights reserved.  Protected by international copyright laws.
*
*                  uC/MQTTc is provided in source form to registered licensees ONLY.  It is
*                  illegal to distribute this source code to any third party unless you receive
*                  written permission by an authorized Micrium representative.  Knowledge of
*                  the source code may NOT be used to develop a similar product.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available.  Your honesty is greatly appreciated.
*
*                  You can find our product's user manual, API reference, release notes and
*                  more information at: https://doc.micrium.com
*
*                  You can contact us at: http://www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                             MQTT CLIENT
*
* Filename      : mqtt-c.h
* Version       : V1.01.00
* Programmer(s) : OD
*                 MTM
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This main network protocol suite header file is protected from multiple pre-processor
*               inclusion through use of the MQTTc module present pre-processor macro definition.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  MQTTc_MODULE_PRESENT
#define  MQTTc_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                        MQTTc VERSION NUMBER
*
* Note(s) : (1) (a) The MQTTc module software version is denoted as follows :
*
*                       Vx.yy.zz
*
*                           where
*                                   V               denotes 'Version' label
*                                   x               denotes     major software version revision number
*                                   yy              denotes     minor software version revision number
*                                   zz              denotes sub-minor software version revision number
*
*               (b) The MQTTc software version label #define is formatted as follows :
*
*                       ver = x.yyzz * 100 * 100
*
*                           where
*                                   ver             denotes software version number scaled as an integer value
*                                   x.yyzz          denotes software version number, where the unscaled integer
*                                                       portion denotes the major version number & the unscaled
*                                                       fractional portion denotes the (concatenated) minor
*                                                       version numbers
*********************************************************************************************************
*********************************************************************************************************
*/

#define  MQTTc_VERSION                                 10100u   /* See Note #1.                                         */


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <mqtt-c_cfg.h>

#include  <Source/net_app.h>
#include  <Source/net_sock.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#define  MQTTc_FLAGS_NONE                       DEF_BIT_NONE    /* Reserved for future usage.                           */


/*
*********************************************************************************************************
*                                               TRACING
*********************************************************************************************************
*/

#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                                   0u
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                                  1u
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                                   2u
#endif

#ifndef  MQTTc_CFG_DBG_TRACE_LEVEL
#define  MQTTc_CFG_DBG_TRACE_LEVEL                          TRACE_LEVEL_OFF
#endif

#ifndef  MQTTc_CFG_DBG_TRACE
#define  MQTTc_CFG_DBG_TRACE                                printf
#endif

#if ((defined(MQTTc_CFG_DBG_TRACE))       && \
     (defined(MQTTc_CFG_DBG_TRACE_LEVEL)) && \
     (MQTTc_CFG_DBG_TRACE_LEVEL >= TRACE_LEVEL_INFO))

    #if (MQTTc_CFG_DBG_TRACE_LEVEL >= TRACE_LEVEL_LOG)
        #define  MQTTc_DBG_TRACE_LOG(msg)     MQTTc_CFG_DBG_TRACE  msg
    #else
        #define  MQTTc_DBG_TRACE_LOG(msg)
    #endif

    #if (MQTTc_CFG_DBG_TRACE_LEVEL >= TRACE_LEVEL_DBG)
        #define  MQTTc_DBG_TRACE_DBG(msg)     MQTTc_CFG_DBG_TRACE  msg
    #else
        #define  MQTTc_DBG_TRACE_DBG(msg)
    #endif

    #define  MQTTc_DBG_TRACE_INFO(msg)        MQTTc_CFG_DBG_TRACE  msg
#else
    #define  MQTTc_DBG_TRACE_LOG(msg)
    #define  MQTTc_DBG_TRACE_DBG(msg)
    #define  MQTTc_DBG_TRACE_INFO(msg)
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/

typedef  struct  mqttc_conn  MQTTc_CONN;                        /* Forward declaration of MQTTc_CONN.                   */
typedef  struct  mqttc_msg   MQTTc_MSG;                         /* Forward declaration of MQTTc_MSG.                    */


/*
*********************************************************************************************************
*                                           MQTTc PARAM TYPE
*********************************************************************************************************
*/

typedef  enum  mqttc_param_type {
    MQTTc_PARAM_TYPE_BROKER_IP_ADDR,                            /* Conn's broker's IP addr.                             */
    MQTTc_PARAM_TYPE_BROKER_NAME,                               /* Conn's broker's name.                                */
    MQTTc_PARAM_TYPE_BROKER_PORT_NBR,                           /* Conn's broker's port nbr.                            */
    MQTTc_PARAM_TYPE_INACTIVITY_TIMEOUT_S,                      /* Conn's inactivity timeout, in seconds.               */
    MQTTc_PARAM_TYPE_CLIENT_ID_STR,                             /* Conn's client ID str.                                */
    MQTTc_PARAM_TYPE_USERNAME_STR,                              /* Conn's client username str.                          */
    MQTTc_PARAM_TYPE_PASSWORD_STR,                              /* Conn's client password str.                          */
    MQTTc_PARAM_TYPE_KEEP_ALIVE_TMR_SEC,                        /* Conn's keep alive tmr, in seconds.                   */
    MQTTc_PARAM_TYPE_WILL_CFG_PTR,                              /* Conn's will cfg ptr, if any.                         */
    MQTTc_PARAM_TYPE_SECURE_CFG_PTR,                            /* Conn's ptr to secure cfg struct.                     */

    MQTTc_PARAM_TYPE_CALLBACK_ON_COMPL,                         /* Conn's generic on     cmpl callback.                 */
    MQTTc_PARAM_TYPE_CALLBACK_ON_CONNECT_CMPL,                  /* Conn's on connect     cmpl callback.                 */
    MQTTc_PARAM_TYPE_CALLBACK_ON_PUBLISH_CMPL,                  /* Conn's on publish     cmpl callback.                 */
    MQTTc_PARAM_TYPE_CALLBACK_ON_SUBSCRIBE_CMPL,                /* Conn's on subscribe   cmpl callback.                 */
    MQTTc_PARAM_TYPE_CALLBACK_ON_UNSUBSCRIBE_CMPL,              /* Conn's on unsubscribe cmpl callback.                 */
    MQTTc_PARAM_TYPE_CALLBACK_ON_PINGREQ_CMPL,                  /* Conn's on pingreq     cmpl callback.                 */
    MQTTc_PARAM_TYPE_CALLBACK_ON_DISCONNECT_CMPL,               /* Conn's on disconnect  cmpl callback.                 */
    MQTTc_PARAM_TYPE_CALLBACK_ON_ERR_CALLBACK,                  /* Conn's on err              callback.                 */

    MQTTc_PARAM_TYPE_CALLBACK_ON_PUBLISH_RX,                    /* Conn's on publish rx'd callback.                     */

    MQTTc_PARAM_TYPE_CALLBACK_ARG_PTR,                          /* Conn's ptr on arg passed to callback.                */

    MQTTc_PARAM_TYPE_TIMEOUT_MS,                                /* Conn's 'Open' timeout, in milliseconds.              */

    MQTTc_PARAM_TYPE_PUBLISH_RX_MSG_PTR,                        /* Conn's ptr on msg that is used to rx publish msg.    */

    MQTTc_PARAM_TYPE_MSG_BUF_PTR,                               /* Msg's buf ptr.                                       */
    MQTTc_PARAM_TYPE_MSG_BUF_LEN                                /* Msg's buf len.                                       */
} MQTTc_PARAM_TYPE;


/*
*********************************************************************************************************
*                                               MQTTc ERR
*********************************************************************************************************
*/

typedef  enum  mqttc_err {
    MQTTc_ERR_NONE,                                             /* No err.                                              */
    MQTTc_ERR_FATAL,                                            /* Fatal err. Sock must be closed.                      */
    MQTTc_ERR_FAIL,                                             /* Generic fail.                                        */
    MQTTc_ERR_NOT_INIT,                                         /* MQTTc module not init yet.                           */
    MQTTc_ERR_ALLOC,                                            /* Allocation of resource failed.                       */
    MQTTc_ERR_OS_FAIL,                                          /* OS/KAL operation failed.                             */
    MQTTc_ERR_CONN_IS_CLOSED,                                   /* Connection used is/has been closed.                  */
    MQTTc_ERR_CONNACK_FAIL,                                     /* CONNACK msg rx'd was not successful.                 */
    MQTTc_ERR_INVALID_ARG,                                      /* Invalid arg passed to function.                      */
    MQTTc_ERR_NULL_PTR,                                         /* Unexpected null ptr passed to function.              */
    MQTTc_ERR_INVALID_BUF_SIZE,                                 /* Invalid size of buf passed to function.              */
    MQTTc_ERR_BUF_OVERFLOW,                                     /* Operation would overflow buf.                        */
    MQTTc_ERR_QoS_LEVEL_NOT_GRANTED,                            /* QoS level requested was not granted by server.       */
    MQTTc_ERR_UNEXPECTED_MSG,                                   /* Rx'd unexpected message. No need to re-open conn.    */

    MQTTc_ERR_RX,                                               /* Generic Rx err.                                      */
    MQTTc_ERR_RX_BUF_EMPTY,                                     /* No more bytes can be read at the moment.             */
    MQTTc_ERR_TX,                                               /* Generic Tx err.                                      */
    MQTTc_ERR_SEL,                                              /* Generic Sel err.                                     */
    MQTTc_ERR_TIMEOUT,                                          /* Operation timed out.                                 */
    MQTTc_ERR_SOCK_FAIL,                                        /* Operation on sock failed.                            */
} MQTTc_ERR;


/*
*********************************************************************************************************
*                                            MQTTc MSG TYPE
*********************************************************************************************************
*/

typedef  enum  mqttc_msg_type {
    MQTTc_MSG_TYPE_NONE,
    MQTTc_MSG_TYPE_CONNECT,
    MQTTc_MSG_TYPE_CONNACK,
    MQTTc_MSG_TYPE_PUBLISH,
    MQTTc_MSG_TYPE_PUBACK,
    MQTTc_MSG_TYPE_PUBREC,
    MQTTc_MSG_TYPE_PUBREL,
    MQTTc_MSG_TYPE_PUBCOMP,
    MQTTc_MSG_TYPE_SUBSCRIBE,
    MQTTc_MSG_TYPE_SUBACK,
    MQTTc_MSG_TYPE_UNSUBSCRIBE,
    MQTTc_MSG_TYPE_UNSUBACK,
    MQTTc_MSG_TYPE_PINGREQ,
    MQTTc_MSG_TYPE_PINGRESP,
    MQTTc_MSG_TYPE_DISCONNECT,

    MQTTc_MSG_TYPE_REQ_CLOSE
} MQTTc_MSG_TYPE;


/*
*********************************************************************************************************
*                                            MQTTc MSG STATE
*********************************************************************************************************
*/

typedef  enum  mqttc_msg_state {
    MQTTc_MSG_STATE_NONE,                                       /* Msg is in 'Idle'/'No' state.                         */
    MQTTc_MSG_STATE_CMPL,                                       /* Msg has cmpl'd.                                      */

    MQTTc_MSG_STATE_MUST_TX,                                    /* Msg must be tx'd.                                    */
    MQTTc_MSG_STATE_WAIT_TX_CMPL,                               /* Msg is waiting for tx to cmpl.                       */

    MQTTc_MSG_STATE_WAIT_RX                                     /* Msg is waiting to rx.                                */
} MQTTc_MSG_STATE;


/*
*********************************************************************************************************
*                                         MQTTc CALLBACK TYPES
*********************************************************************************************************
*/

                                                                /* Type of callback exec'd when user-req'd oper cmpl.   */
typedef  void  (*MQTTc_CMPL_CALLBACK)           (      MQTTc_CONN    *p_conn,
                                                       MQTTc_MSG     *p_msg,
                                                       void          *p_arg,
                                                       MQTTc_ERR      err);

                                                                /* Type of callback exec'd when err occurs/conn closes. */
typedef  void  (*MQTTc_ERR_CALLBACK)            (      MQTTc_CONN    *p_conn,
                                                       void          *p_arg,
                                                       MQTTc_ERR      err);

                                                                /* Type of callback exec'd when a publish is rx'd.      */
typedef  void  (*MQTTc_PUBLISH_RX_CALLBACK)     (       MQTTc_CONN   *p_conn,
                                                 const  CPU_CHAR     *topic_name_str,
                                                        CPU_INT32U    topic_len,
                                                 const  CPU_CHAR     *p_payload,
                                                        CPU_INT32U    payload_len,
                                                        void         *p_arg,
                                                        MQTTc_ERR     err);


/*
*********************************************************************************************************
*                                            MQTTc FLAG TYPE
*********************************************************************************************************
*/

typedef  CPU_INT08U  MQTTc_FLAGS;                               /* No flags implemented, reserved for future usage.     */


/*
*********************************************************************************************************
*                                          MQTTc WILL CFG TYPE
*********************************************************************************************************
*/

typedef  struct  mqttc_will_cfg {
    CPU_CHAR     *WillTopic;                                    /* Will's topic.                                        */
    CPU_CHAR     *WillMessage;                                  /* Will's msg.                                          */
    CPU_BOOLEAN   WillRetain;                                   /* Flag indicating if will must be retained.            */
    CPU_INT08U    WillQoS;                                      /* Will's QoS level.                                    */
} MQTTc_WILL_CFG;


/*
*********************************************************************************************************
*                                            MQTTc MSG TYPE
*********************************************************************************************************
*/

struct  mqttc_msg {
    MQTTc_CONN       *ConnPtr;                                  /* Ptr to MQTTc_CONN associated.                        */
    MQTTc_MSG_TYPE    Type;                                     /* Msg's type.                                          */
    MQTTc_MSG_STATE   State;                                    /* Msg's state.                                         */
    CPU_INT08U        QoS;                                      /* Msg's QoS.                                           */

    CPU_INT16U        MsgID;                                    /* Msg ID used by msg.                                  */

                                                                /* Ptr to rx/tx buf in case of mqttc msg, ptr to sem ... */
    void             *ArgPtr;                                   /* to post, in case of 'close' msg.                     */
    CPU_INT32U        BufLen;                                   /* Avail buf len for msg.                               */
    CPU_INT32U        XferLen;                                  /* Len of xfer.                                         */

    MQTTc_ERR         Err;                                      /* Err associated to processing of msg.                 */

    MQTTc_MSG        *NextPtr;                                  /* Ptr to next msg.                                     */
};


/*
*********************************************************************************************************
*                                            MQTTc CONN TYPE
*********************************************************************************************************
*/

struct  mqttc_conn {
    NET_SOCK_ID                 SockId;                         /* Connection's socket ID.                              */
    CPU_INT08U                  SockSelFlags;                   /* Flags to identify which oper must be checked in Sel. */

    CPU_CHAR                   *BrokerNamePtr;                  /* MQTT broker's name.                                  */
    CPU_INT16U                  BrokerPortNbr;                  /* MQTT broker's port nbr.                              */
    CPU_INT16U                  InactivityTimeout_s;            /* Inactivity timeout, in seconds.                      */

    CPU_CHAR                   *ClientID_Str;                   /* Client ID str.                                       */
    CPU_CHAR                   *UsernameStr;                    /* Username str.                                        */
    CPU_CHAR                   *PasswordStr;                    /* Password str.                                        */

    CPU_INT16U                  KeepAliveTimerSec;              /* Keep alive timer duration, in seconds.               */
    MQTTc_WILL_CFG             *WillCfgPtr;                     /* Ptr to will cfg, if any.                             */

    NET_APP_SOCK_SECURE_CFG    *SecureCfgPtr;                   /* Ptr to secure will cfg, if any.                      */

                                                                /* -------------------- CALLBACKS --------------------- */
    MQTTc_CMPL_CALLBACK         OnCmpl;                         /* Generic, on cmpl callback.                           */
    MQTTc_CMPL_CALLBACK         OnConnectCmpl;                  /* On connect cmpl callback.                            */
    MQTTc_CMPL_CALLBACK         OnPublishCmpl;                  /* On publish cmpl callback.                            */
    MQTTc_CMPL_CALLBACK         OnSubscribeCmpl;                /* On subscribe cmpl callback.                          */
    MQTTc_CMPL_CALLBACK         OnUnsubscribeCmpl;              /* On unsubscribe cmpl callback.                        */
    MQTTc_CMPL_CALLBACK         OnPingReqCmpl;                  /* On ping req cmpl callback.                           */
    MQTTc_CMPL_CALLBACK         OnDisconnectCmpl;               /* On disconnect cmpl callback.                         */
    MQTTc_ERR_CALLBACK          OnErrCallback;                  /* On err or conn lost callback. Conn must be re-opened.*/
    MQTTc_PUBLISH_RX_CALLBACK   OnPublishRx;                    /* On publish rx'd cmpl callback.                       */
    void                       *ArgPtr;                         /* Ptr to arg that will be provided to callbacks.       */

    CPU_INT32U                  TimeoutMs;                      /* Timeout for 'Open' operation, in milliseconds.       */

                                                                /* ----------------- NEXT MSG VALUES ------------------ */
    CPU_INT08U                  NextMsgHeader;                  /* Header of next msg to parse.                         */
    CPU_INT32U                  NextMsgRxLen;                   /* Rx len of next msg.                                  */
    MQTTc_MSG_TYPE              NextMsgType;                    /* Next msg's type.                                     */
    CPU_INT32U                  NextMsgLen;                     /* Len remaining to rx for next msg.                    */
    CPU_BOOLEAN                 NextMsgLenIsCmpl;               /* Flag indicating if next msg's len value is rx'd.     */
    CPU_INT16U                  NextMsgMsgID;                   /* ID of next msg, if any.                              */
    CPU_BOOLEAN                 NextMsgMsgID_IsCmpl;            /* Flag indicating if next msg's ID has been rx'd.      */
    MQTTc_MSG                  *NextMsgPtr;                     /* Ptr to next msg, if known.                           */

    MQTTc_MSG                  *PublishRxMsgPtr;                /* Ptr to msg that is used to rx publish from server.   */

    MQTTc_MSG                  *TxMsgHeadPtr;                   /* Ptr to head of msg needing to tx or waiting reply.   */
    CPU_INT32U                  NextTxMsgTxLen;                 /* Len of already xfer'd data.                          */

    MQTTc_CONN                 *NextPtr;                        /* Ptr to next conn.                                    */
};


/*
*********************************************************************************************************
*                                            MQTTc CFG TYPE
*********************************************************************************************************
*/

typedef  struct  mqttc_cfg {
                                                                /* Max nbr of msgs that will need to be processed ...   */
    CPU_INT16U     MaxMsgNbr;                                   /* at any given time.                                   */
    CPU_INT16U     InactivityTimeout_s;                         /* Inactivity timeout of sock, in seconds.              */
    CPU_INT32U     TaskDly;                                     /* Optional internal task dly.                          */
} MQTTc_CFG;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                      PUBLIC FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

void  MQTTc_Init            (const  MQTTc_CFG          *p_cfg,
                             const  NET_TASK_CFG       *p_task_cfg,
                                    MEM_SEG            *p_mem_seg,
                                    MQTTc_ERR          *p_err);

void  MQTTc_ConnClr         (       MQTTc_CONN         *p_conn,
                                    MQTTc_ERR          *p_err);

void  MQTTc_ConnSetParam    (       MQTTc_CONN         *p_conn,
                                    MQTTc_PARAM_TYPE    type,
                                    void               *p_param,
                                    MQTTc_ERR          *p_err);

void  MQTTc_ConnOpen        (       MQTTc_CONN         *p_conn,
                                    MQTTc_FLAGS         flags,
                                    MQTTc_ERR          *p_err);

void  MQTTc_ConnClose       (       MQTTc_CONN         *p_conn,
                                    MQTTc_FLAGS         flags,
                                    MQTTc_ERR          *p_err);

void  MQTTc_MsgClr          (       MQTTc_MSG          *p_msg,
                                    MQTTc_ERR          *p_err);

void  MQTTc_MsgSetParam     (       MQTTc_MSG          *p_msg,
                                    MQTTc_PARAM_TYPE    type,
                                    void               *p_param,
                                    MQTTc_ERR          *p_err);

void  MQTTc_Connect         (       MQTTc_CONN         *p_conn,
                                    MQTTc_MSG          *p_msg,
                                    MQTTc_ERR          *p_err);

void  MQTTc_Publish         (       MQTTc_CONN         *p_conn,
                                    MQTTc_MSG          *p_msg,
                             const  CPU_CHAR           *topic_str,
                                    CPU_INT08U          qos_lvl,
                                    CPU_BOOLEAN         retain_flag,
                             const  CPU_CHAR           *p_payload,
                                    CPU_INT32U          payload_len,
                                    MQTTc_ERR          *p_err);

void  MQTTc_Subscribe       (       MQTTc_CONN         *p_conn,
                                    MQTTc_MSG          *p_msg,
                             const  CPU_CHAR           *topic_str,
                                    CPU_INT08U          topic_req_qos,
                                    MQTTc_ERR          *p_err);

void  MQTTc_SubscribeMult   (       MQTTc_CONN         *p_conn,
                                    MQTTc_MSG          *p_msg,
                             const  CPU_CHAR          **topic_str_tbl,
                                    CPU_INT08U         *topic_req_qos_tbl,
                                    CPU_INT08U          topic_nbr,
                                    MQTTc_ERR          *p_err);

void  MQTTc_Unsubscribe     (       MQTTc_CONN         *p_conn,
                                    MQTTc_MSG          *p_msg,
                             const  CPU_CHAR           *topic_str,
                                    MQTTc_ERR          *p_err);

void  MQTTc_UnsubscribeMult (       MQTTc_CONN         *p_conn,
                                    MQTTc_MSG          *p_msg,
                             const  CPU_CHAR          **topic_str_tbl,
                                    CPU_INT08U          topic_nbr,
                                    MQTTc_ERR          *p_err);

void  MQTTc_PingReq         (       MQTTc_CONN         *p_conn,
                                    MQTTc_MSG          *p_msg,
                                    MQTTc_ERR          *p_err);

void  MQTTc_Disconnect      (       MQTTc_CONN         *p_conn,
                                    MQTTc_MSG          *p_msg,
                                    MQTTc_ERR          *p_err);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  MQTTc_CFG_ARG_CHK_EXT_EN
#error  "MQTTc_CFG_ARG_CHK_EXT_EN not #define'd in 'mqtt-c_cfg.h'. Must be [DEF_DISABLED] or [DEF_ENABLED]."
#elif  ((MQTTc_CFG_ARG_CHK_EXT_EN != DEF_DISABLED) && \
        (MQTTc_CFG_ARG_CHK_EXT_EN != DEF_ENABLED ))
#error  "MQTTc_CFG_ARG_CHK_EXT_EN illegally #define'd in 'mqtt-c_cfg.h'. MUST be [DEF_DISABLED] or [DEF_ENABLED]."
#endif

#ifndef  MQTTc_CFG_DBG_GLOBAL_BUF_EN
#error  "MQTTc_CFG_DBG_GLOBAL_BUF_EN not #define'd in 'mqtt-c_cfg.h'. Must be [DEF_DISABLED] or [DEF_ENABLED]."
#elif  ((MQTTc_CFG_DBG_GLOBAL_BUF_EN != DEF_DISABLED) && \
        (MQTTc_CFG_DBG_GLOBAL_BUF_EN != DEF_ENABLED ))
#error  "MQTTc_CFG_DBG_GLOBAL_BUF_EN illegally #define'd in 'mqtt-c_cfg.h'. MUST be [DEF_DISABLED] or [DEF_ENABLED]."
#elif   (MQTTc_CFG_DBG_GLOBAL_BUF_EN == DEF_ENABLED)
#ifndef MQTTc_CFG_DBG_GLOBAL_BUF_LEN
#error  "MQTTc_CFG_DBG_GLOBAL_BUF_LEN not #define'd in 'mqtt-c_cfg.h'. Must be >= 0u."
#elif  (MQTTc_CFG_DBG_GLOBAL_BUF_LEN == 0u)
#error  "MQTTc_CFG_DBG_GLOBAL_BUF_LEN illegally #define'd in 'mqtt-c_cfg.h'. Must be >= 0u."
#endif
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif
