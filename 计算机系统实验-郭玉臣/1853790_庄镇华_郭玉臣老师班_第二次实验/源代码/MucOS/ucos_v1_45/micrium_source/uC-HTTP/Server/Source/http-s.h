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
*                                             HTTP SERVER
*
* Filename : http-s.h
* Version  : V3.00.08
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This main network protocol suite header file is protected from multiple pre-processor
*               inclusion through use of the HTTP module present pre-processor macro definition.
*
*               See also 'HTTPs INCLUDE FILES  Note #5'.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  HTTPs_MODULE_PRESENT                                   /* See Note #1.                                         */
#define  HTTPs_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                        HTTPs VERSION NUMBER
*
* Note(s) : (1) (a) The HTTPs module software version is denoted as follows :
*
*                       Vx.yy.zz
*
*                           where
*                                   V               denotes 'Version' label
*                                   x               denotes     major software version revision number
*                                   yy              denotes     minor software version revision number
*                                   zz              denotes sub-minor software version revision number
*
*               (b) The HTTPs software version label #define is formatted as follows :
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

#define  HTTPs_VERSION                                 30008u   /* See Note #1.                                         */


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         HTTPs INCLUDE FILES
*
* Note(s) : (1) The HTTPs module files are located in the following directories :
*
*               (a) (1) \<Your Product Application>\http-s_cfg.h
*                   (2)                            \http-s_instance_cfg.*
*
*               (b) (1) \<Network Protocol Suite>\Source\net_*.*
*
*                   (2) If a dynamic file system is used:
*
*                       (A) \<Network Protocol Suite>\FS\<fs>\net_fs_<fs>.*
*
*                   (3) If network security manager is to be used:
*
*                       (A) \<Network Protocol Suite>\Secure\<Network Security Suite>\net_secure.*
*
*               (c) (1) \<HTTPs>\Source\http-s.*
*                                      \http-s_mem.*
*
*                   (2) If a dynamic file system is not used:
*
*                       (A) \<HTTPs>\FS\Static\http-s_fs_static.*
*
*               where
*                   <Your Product Application>      directory path for Your Product's Application
*                   <Network Protocol Suite>        directory path for network protocol suite
*                   <HTTPs>                         directory path for HTTPs module
*
*           (2) CPU-configuration software files are located in the following directories :
*
*               (a) \<CPU-Compiler Directory>\cpu_*.*
*               (b) \<CPU-Compiler Directory>\<cpu>\<compiler>\cpu*.*
*
*                       where
*                               <CPU-Compiler Directory>        directory path for common CPU-compiler software
*                               <cpu>                           directory name for specific processor (CPU)
*                               <compiler>                      directory name for specific compiler
*
*           (3) NO compiler-supplied standard library functions SHOULD be used.
*
*               (a) Standard library functions are implemented in the custom library module(s) :
*
*                       \<Custom Library Directory>\lib_*.*
*
*                           where
*                                   <Custom Library Directory>      directory path for custom library software
*
*           (4) Compiler MUST be configured to include as additional include path directories :
*
*               (a) '\<Your Product Application>\'                                          See Note #1a
*
*               (b) (1) '\<Network Protocol Suite>\                                         See Note #1b1
*
*                   (2) '\<Network Protocol Suite>\Secure\<Network Security Suite>\'        See Note #1b2A
*
*               (c) '\<HTTPs>\' directories                                                 See Note #1c
*
*               (d) (1) '\<CPU-Compiler Directory>\'                                        See Note #2a
*                   (2) '\<CPU-Compiler Directory>\<cpu>\<compiler>\'                       See Note #2b
*
*               (e) '\<Custom Library Directory>\'                                          See Note #3a
*
*           (5) An application pre-processor MUST include ONLY this main HTTPs server header file, 'net.h'.
*               All other network protocol suite files are included via this main HTTP server header file.
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <cpu.h>                                               /* CPU Configuration              (see Note #2b)        */
#include  <cpu_core.h>                                          /* CPU Core Library               (see Note #2a)        */

#include  <lib_def.h>                                           /* Standard        Defines        (see Note #3a)        */
#include  <lib_str.h>                                           /* Standard String Library        (see Note #3a)        */
#include  <lib_ascii.h>                                         /* Standard ASCII  Library        (see Note #3a)        */

#include  <http-s_cfg.h>
#include  <FS/net_fs.h>                                         /* File System Interface          (see Note #1c3)       */

#include  <Source/net.h>                                        /* Network Protocol Suite         (see Note #1b)        */
#include  <Source/net_app.h>
#include  <Source/net_sock.h>
#include  <Source/net_tcp.h>
#include  <Source/net_tmr.h>

#include  "../../Common/http.h"
#include  "../../Common/http_dict.h"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*********************************************************************************************************
*/

#ifdef HTTPs_MODULE
#define  HTTPs_EXT
#else
#define  HTTPs_EXT  extern
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                       PRE CONFIGURATION ERROR
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  HTTPs_CFG_ARG_CHK_EXT_EN
    #error  "HTTPs_CFG_ARG_CHK_EXT_EN not #define'd in 'http-s_cfg.h'"
#elif  ((HTTPs_CFG_ARG_CHK_EXT_EN != DEF_ENABLED ) && \
        (HTTPs_CFG_ARG_CHK_EXT_EN != DEF_DISABLED))
    #error  "HTTPs_CFG_ARG_CHK_EXT_EN illegally #define'd in 'http-s_cfg.h'. MUST be DEF_DISABLED or DEF_ENABLED"
#endif


#ifndef  HTTPs_CFG_CTR_STAT_EN
    #error  "HTTPs_CFG_CTR_STAT_EN not #define'd in 'http-s_cfg.h'"
#elif  ((HTTPs_CFG_CTR_STAT_EN != DEF_ENABLED ) && \
        (HTTPs_CFG_CTR_STAT_EN != DEF_DISABLED))
    #error  "HTTPs_CFG_CTR_STAT_EN illegally #define'd in 'http-s_cfg.h'. MUST be  DEF_DISABLED or DEF_ENABLED"
#endif


#ifndef  HTTPs_CFG_CTR_ERR_EN
    #error  "HTTPs_CFG_CTR_ERR_EN not #define'd in 'http-s_cfg.h'"
#elif  ((HTTPs_CFG_CTR_ERR_EN != DEF_ENABLED ) && \
        (HTTPs_CFG_CTR_ERR_EN != DEF_DISABLED))
    #error  "HTTPs_CFG_CTR_ERR_EN illegally #define'd in 'http-s_cfg.h'. MUST be  DEF_DISABLED or DEF_ENABLED"
#endif


#ifndef  HTTPs_CFG_PERSISTENT_CONN_EN
    #error  "HTTPs_CFG_PERSISTENT_CONN_EN not #define'd in 'http-s_cfg.h'"
#elif  ((HTTPs_CFG_PERSISTENT_CONN_EN != DEF_ENABLED ) && \
        (HTTPs_CFG_PERSISTENT_CONN_EN != DEF_DISABLED))
    #error  "HTTPs_CFG_PERSISTENT_CONN_EN illegally #define'd in 'http-s_cfg.h'. MUST be  DEF_DISABLED or DEF_ENABLED"
#endif


#ifndef  HTTPs_CFG_TOKEN_PARSE_EN
    #error  "HTTPs_CFG_TOKEN_PARSE_EN not #define'd in 'http-s_cfg.h'"
#elif  ((HTTPs_CFG_TOKEN_PARSE_EN != DEF_ENABLED ) && \
        (HTTPs_CFG_TOKEN_PARSE_EN != DEF_DISABLED))
    #error  "HTTPs_CFG_TOKEN_PARSE_EN illegally #define'd in 'http-s_cfg.h'. MUST be  DEF_DISABLED or DEF_ENABLED"
#endif


#ifndef  HTTPs_CFG_ABSOLUTE_URI_EN
    #error  "HTTPs_CFG_ABSOLUTE_URI_EN not #define'd in 'http-s_cfg.h'"
#elif  ((HTTPs_CFG_ABSOLUTE_URI_EN != DEF_ENABLED ) && \
        (HTTPs_CFG_ABSOLUTE_URI_EN != DEF_DISABLED))
    #error  "HTTPs_CFG_ABSOLUTE_URI_EN illegally #define'd in 'http-s_cfg.h'. MUST be DEF_DISABLED or DEF_ENABLED"
#endif


#ifndef  HTTPs_CFG_QUERY_STR_EN
    #error  "HTTPs_CFG_QUERY_STR_EN not #define'd in 'http-s_cfg.h'"
#elif  ((HTTPs_CFG_QUERY_STR_EN != DEF_ENABLED ) && \
        (HTTPs_CFG_QUERY_STR_EN != DEF_DISABLED))
    #error  "HTTPs_CFG_QUERY_STR_EN illegally #define'd in 'http-s_cfg.h'. MUST be DEF_DISABLED or DEF_ENABLED"
#endif


#ifndef  HTTPs_CFG_FORM_EN
    #error  "HTTPs_CFG_FORM_EN not #define'd in 'http-s_cfg.h'"
#elif ((HTTPs_CFG_FORM_EN != DEF_ENABLED ) && \
       (HTTPs_CFG_FORM_EN != DEF_DISABLED))
    #error  "HTTPs_CFG_FORM_EN illegally #define'd in 'http-s_cfg.h'. MUST be DEF_DISABLED or DEF_ENABLED"

#elif (HTTPs_CFG_FORM_EN == DEF_ENABLED)
    #ifndef  HTTPs_CFG_FORM_MULTIPART_EN
        #error  "HTTPs_CFG_FORM_MULTIPART_EN not #define'd in 'http-s_cfg.h'"
    #elif  ((HTTPs_CFG_FORM_MULTIPART_EN != DEF_ENABLED ) && \
            (HTTPs_CFG_FORM_MULTIPART_EN != DEF_DISABLED))
        #error  "HTTPs_CFG_FORM_MULTIPART_EN illegally #define'd in 'http-s_cfg.h'. MUST be DEF_DISABLED or DEF_ENABLED"
    #endif
#endif


#ifndef HTTPs_CFG_HDR_RX_EN
    #error  "HTTPs_CFG_RX_HDR_EN not #define'd in 'http-s_cfg.h'"
#elif  ((HTTPs_CFG_HDR_RX_EN != DEF_ENABLED ) && \
        (HTTPs_CFG_HDR_RX_EN != DEF_DISABLED))
    #error  "HTTPs_CFG_HDR_RX_EN illegally #define'd in 'http-s_cfg.h'. MUST be DEF_DISABLED or DEF_ENABLED"
#endif


#ifndef HTTPs_CFG_HDR_TX_EN
    #error  "HTTPs_CFG_TX_HDR_EN not #define'd in 'http-s_cfg.h'"
#elif  ((HTTPs_CFG_HDR_TX_EN != DEF_ENABLED ) && \
        (HTTPs_CFG_HDR_TX_EN != DEF_DISABLED))
    #error  "HTTPs_CFG_HDR_TX_EN illegally #define'd in 'http-s_cfg.h'. MUST be DEF_DISABLED or DEF_ENABLED"
#endif


#ifndef  HTTPs_CFG_HTML_DFLT_ERR_PAGE
    #error  "HTTPs_CFG_HTML_DFLT_ERR_PAGE not #define'd in 'http-s_cfg.h'. MUST be defined as a string"
#endif


/*
*********************************************************************************************************
*                                    NETWORK CONFIGURATION ERRORS
*********************************************************************************************************
*/

#if     (NET_VERSION < 30300u)
#error  "NET_VERSION: Invalid network protocol suite version. MUST be >= 3.03.00"
#endif


#if     (NET_SOCK_CFG_SEL_EN != DEF_ENABLED)
#error  "NET_SOCK_CFG_SEL_EN: illegally #define'd in 'net_cfg.h'. MUST be DEF_ENABLED"
#endif


/*
*********************************************************************************************************
*                                      LIB CONFIGURATION ERRORS
*********************************************************************************************************
*/

#if     (LIB_VERSION < 13701u)
#error  "LIB_VERSION: Invalid library suite version. MUST be >= 1.37.01"
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      GENERAL HTTPs DEFINES
*********************************************************************************************************
*/

#define  HTTPs_CFG_DFLT_PORT                                 80u
#define  HTTPs_CFG_DFLT_PORT_SECURE                         443u

#define  HTTPs_BUF_LEN_MIN                                  256u

#define  HTTPs_PATH_SEP_CHAR_DFLT                           '/'


/*
*********************************************************************************************************
*                                     STATIC ERR FILE LEN DEFINES
*********************************************************************************************************
*/

#define  HTTPs_HTML_DFLT_ERR_LEN                            (sizeof(HTTPs_CFG_HTML_DFLT_ERR_PAGE) - 1u)
#define  HTTPs_HTML_DFLT_ERR_STR_NAME                       "default.html"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            ENUMERATIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         ERRORS ENUMERATIONS
*********************************************************************************************************
*/

typedef enum https_err {
    HTTPs_ERR_NONE = 1,

    HTTPs_ERR_NULL_PTR,

    HTTPs_ERR_INIT,

    HTTPs_ERR_MEM_NO_SPACE,

    HTTPs_ERR_INIT_POOL_MEM_NO_SPACE,
    HTTPs_ERR_INIT_POOL_INSTANCE,
    HTTPs_ERR_INIT_POOL_CONN,
    HTTPs_ERR_INIT_POOL_BUF,
    HTTPs_ERR_INIT_POOL_PATH,
    HTTPs_ERR_INIT_POOL_HOST,
    HTTPs_ERR_INIT_POOL_TOKEN,
    HTTPs_ERR_INIT_POOL_TOKEN_VAL,
    HTTPs_ERR_INIT_POOL_KEY_VAL,
    HTTPs_ERR_INIT_POOL_QUERY_STR_KEY,
    HTTPs_ERR_INIT_POOL_QUERY_STR_VAL,
    HTTPs_ERR_INIT_POOL_FORM_KEY,
    HTTPs_ERR_INIT_POOL_FORM_VAL,
    HTTPs_ERR_INIT_POOL_FORM_BOUNDARY,
    HTTPs_ERR_INIT_POOL_REQ_HDR,
    HTTPs_ERR_INIT_POOL_REQ_HDR_STR,
    HTTPs_ERR_INIT_POOL_RESP_HDR,
    HTTPs_ERR_INIT_POOL_RESP_HDR_STR,
    HTTPs_ERR_INIT_INSTANCE_HOOK_FAULT,

    HTTPs_ERR_POOL_BLK_FREE_FAULT,

    HTTPs_ERR_INSTANCE_LIST_NULL_ITEM,

    HTTPs_ERR_SOCK_OPEN,
    HTTPs_ERR_SOCK_BIND,
    HTTPs_ERR_SOCK_LISTEN,
    HTTPs_ERR_SOCK_SET_OPT_BLOCK,
    HTTPs_ERR_SOCK_SET_OPT_SECURE,

    HTTPs_ERR_CFG_INVALID_NBR_CONN,
    HTTPs_ERR_CFG_INVALID_DFLT_RESOURCE,
    HTTPs_ERR_CFG_INVALID_DFLT_FILE,
    HTTPs_ERR_CFG_INVALID_SOCK_SEL,
    HTTPs_ERR_CFG_INVALID_SECURE_EN,
    HTTPs_ERR_CFG_INVALID_SECURE_CERT_INVALID,
    HTTPs_ERR_CFG_INVALID_SECURE_KEY_INVALID,
    HTTPs_ERR_CFG_INVALID_HOST_LEN,
    HTTPs_ERR_CFG_INVALID_PATH_LEN,
    HTTPs_ERR_CFG_INVALID_FS_TYPE,
    HTTPs_ERR_CFG_INVALID_FS_EN,
    HTTPs_ERR_CFG_INVALID_FS_API,
    HTTPs_ERR_CFG_INVALID_FS_PATH_LEN,
    HTTPs_ERR_CFG_INVALID_FS_SEP_CHAR,
    HTTPs_ERR_CFG_INVALID_FS_WORKING_FOLDER,
    HTTPs_ERR_CFG_INVALID_HDR_EN,
    HTTPs_ERR_CFG_INVALID_HDR_PARAM,
    HTTPs_ERR_CFG_INVALID_TOKEN_EN,
    HTTPs_ERR_CFG_INVALID_TOKEN_PARAM,
    HTTPs_ERR_CFG_INVALID_QUERY_STR_EN,
    HTTPs_ERR_CFG_INVALID_QUERY_STR_PARAM,
    HTTPs_ERR_CFG_INVALID_FORM_EN,
    HTTPs_ERR_CFG_INVALID_FORM_MULTIPART_EN,
    HTTPs_ERR_CFG_INVALID_FORM_PARAM,
    HTTPs_ERR_CFG_INVALID_BUF_LEN,
    HTTPs_ERR_CFG_INVALID_ERR_PAGE_LEN,

    HTTPs_ERR_CFG_NULL_PTR_FS,
    HTTPs_ERR_CFG_NULL_PTR_REQ_HDR,
    HTTPs_ERR_CFG_NULL_PTR_RESP_HDR,
    HTTPs_ERR_CFG_NULL_PTR_TOKEN,
    HTTPs_ERR_CFG_NULL_PTR_QUERY_STR,
    HTTPs_ERR_CFG_NULL_PTR_FORM,

    HTTPs_ERR_TASK_INIT_REM_MEM,
    HTTPs_ERR_TASK_OBJ_CREATE,
    HTTPs_ERR_TASK_CREATE,
    HTTPs_ERR_TASK_LOCK_CREATE,
    HTTPs_ERR_TASK_LOCK_ACQUIRE,
    HTTPs_ERR_TASK_SEM_CREATE,
    HTTPs_ERR_TASK_STOP_REQ_SIGNAL,
    HTTPs_ERR_TASK_STOP_COMPLETED_PEND,

    HTTPs_ERR_STATE_UNKNOWN,

    HTTPs_ERR_REQ_MORE_DATA_REQUIRED,
    HTTPs_ERR_REQ_FORMAT_INVALID,
    HTTPs_ERR_REQ_METHOD_NOT_SUPPORTED,
    HTTPs_ERR_REQ_PROTOCOL_VER_NOT_SUPPORTED,
    HTTPs_ERR_REQ_URI_LEN,
    HTTPs_ERR_REQ_HDR_BLK_GET_FAULT,
    HTTPS_ERR_REQ_HDR_INVALID_VAL_LEN,
    HTTPs_ERR_REQ_HDR_OVERFLOW,
    HTTPs_ERR_REQ_HDR_POOL_EMPTY,
    HTTPs_ERR_REQ_HDR_POOL_LIB_FAULT,
    HTTPs_ERR_REQ_HDR_DATA_TYPE_UNKNOWN,
    HTTPs_ERR_REQ_BODY_FAULT,

    HTTPs_ERR_KEY_VAL_CFG_POOL_SIZE_INV,

    HTTPs_ERR_QUERY_STR_POOL_LIB_FAULT,
    HTTPs_ERR_QUERY_STR_PARSE_FAULT,

    HTTPs_ERR_FORM_POOL_LIB_FAULT,
    HTTPs_ERR_FORM_APP_PARSE_FAULT,
    HTTPs_ERR_FORM_FILE_UPLOAD_OPEN,
    HTTPs_ERR_FORM_FORMAT_INV,

    HTTPs_ERR_TOKEN_POOL_EMPTY,
    HTTPs_ERR_TOKEN_POOL_LIB_FAULT,
    HTTPs_ERR_TOKEN_NO_TOKEN_FOUND,
    HTTPs_ERR_TOKEN_MORE_DATA_REQ,

    HTTPs_ERR_HDR_FIELD_TYPE_UNKNOWN,
    HTTPs_ERR_HDR_FIELD_VAL_UNKNOWN,

    HTTPs_ERR_FILE_NOT_FOUND,
    HTTPs_ERR_FILE_WR_FAULT,

    HTTPs_ERR_RESP_CONTENT_TYPE_INVALID,
    HTTPs_ERR_RESP_BUF_NO_MORE_SPACE,
    HTTPs_ERR_RESP_STATUS_LINE,
    HTTPs_ERR_RESP_HDR_OVERFLOW,
    HTTPs_ERR_RESP_HDR_POOL_EMPTY,
    HTTPs_ERR_RESP_HDR_POOL_LIB_FAULT,
    HTTPs_ERR_RESP_HDR_DATA_TYPE_UNKNOWN,
    HTTPs_ERR_RESP_PATH_PTR_INVALID,
    HTTPs_ERR_RESP_BODY_DATA_TYPE_UNKNOWN,
    HTTPs_ERR_RESP_BODY_DATA_LEN_INVALID,
    HTTPs_ERR_RESP_BODY_PTR_INVALID,
    HTTPs_ERR_RESP_DATA_CHUNKED_HOOK_UNDEFINED,
    HTTPs_ERR_RESP_DATA_CHUNKED_LENGTH_INVALID
} HTTPs_ERR;


/*
*********************************************************************************************************
*                                         IP TYPE ENUMERATION
*********************************************************************************************************
*/

typedef enum https_sock_sel {
    HTTPs_SOCK_SEL_IPv4,
    HTTPs_SOCK_SEL_IPv6,
    HTTPs_SOCK_SEL_IPv4_IPv6
} HTTPs_SOCK_SEL;


/*
*********************************************************************************************************
*                                    CONNECTION STATES ENUMERATION
*********************************************************************************************************
*/

typedef enum https_conn_state {
    HTTPs_CONN_STATE_UNKNOWN,

    HTTPs_CONN_STATE_REQ_INIT,
    HTTPs_CONN_STATE_REQ_PARSE_METHOD,
    HTTPs_CONN_STATE_REQ_PARSE_URI,
    HTTPs_CONN_STATE_REQ_PARSE_QUERY_STRING,
    HTTPs_CONN_STATE_REQ_PARSE_PROTOCOL_VERSION,
    HTTPs_CONN_STATE_REQ_PARSE_HDR,
    HTTPs_CONN_STATE_REQ_LINE_HDR_HOOK,
    HTTPs_CONN_STATE_REQ_BODY_INIT,
    HTTPs_CONN_STATE_REQ_BODY_FLUSH_DATA,
    HTTPs_CONN_STATE_REQ_BODY_DATA,
    HTTPs_CONN_STATE_REQ_BODY_FORM_APP_PARSE,
    HTTPs_CONN_STATE_REQ_BODY_FORM_MULTIPART_INIT,
    HTTPs_CONN_STATE_REQ_BODY_FORM_MULTIPART_PARSE,
    HTTPs_CONN_STATE_REQ_BODY_FORM_MULTIPART_FILE_OPEN,
    HTTPs_CONN_STATE_REQ_BODY_FORM_MULTIPART_FILE_WR,
    HTTPs_CONN_STATE_REQ_READY_SIGNAL,
    HTTPs_CONN_STATE_REQ_READY_POLL,

    HTTPs_CONN_STATE_RESP_PREPARE,
    HTTPs_CONN_STATE_RESP_INIT,
    HTTPs_CONN_STATE_RESP_TOKEN,
    HTTPs_CONN_STATE_RESP_STATUS_LINE,
    HTTPs_CONN_STATE_RESP_HDR,
    HTTPs_CONN_STATE_RESP_HDR_CONTENT_TYPE,
    HTTPs_CONN_STATE_RESP_HDR_FILE_TRANSFER,
    HTTPs_CONN_STATE_RESP_HDR_LOCATION,
    HTTPs_CONN_STATE_RESP_HDR_CONN,
    HTTPs_CONN_STATE_RESP_HDR_LIST,
    HTTPs_CONN_STATE_RESP_HDR_TX,
    HTTPs_CONN_STATE_RESP_HDR_END,
    HTTPs_CONN_STATE_RESP_FILE_STD,
    HTTPs_CONN_STATE_RESP_DATA_CHUNKED,
    HTTPs_CONN_STATE_RESP_DATA_CHUNKED_TX_TOKEN,
    HTTPs_CONN_STATE_RESP_DATA_CHUNKED_TX_LAST_CHUNK,
    HTTPs_CONN_STATE_RESP_DATA_CHUNKED_HOOK,
    HTTPs_CONN_STATE_RESP_DATA_CHUNKED_FINALIZE,
    HTTPs_CONN_STATE_RESP_COMPLETED,

    HTTPs_CONN_STATE_COMPLETED,

    HTTPs_CONN_STATE_ERR_INTERNAL,
    HTTPs_CONN_STATE_ERR_FATAL
} HTTPs_CONN_STATE;


/*
*********************************************************************************************************
*                                      SOCKET STATES ENUMERATION
*********************************************************************************************************
*/

typedef enum https_sock_state {
    HTTPs_SOCK_STATE_NONE,
    HTTPs_SOCK_STATE_RX,
    HTTPs_SOCK_STATE_TX,
    HTTPs_SOCK_STATE_CLOSE,
    HTTPs_SOCK_STATE_ERR
} HTTPs_SOCK_STATE;


/*
*********************************************************************************************************
*                                       FILE TYPES ENUMARATION
*********************************************************************************************************
*/

typedef  enum https_body_data_type {
    HTTPs_BODY_DATA_TYPE_NONE,
    HTTPs_BODY_DATA_TYPE_FILE,
    HTTPs_BODY_DATA_TYPE_STATIC_DATA
} HTTPs_BODY_DATA_TYPE;


/*
*********************************************************************************************************
*                                     TOKEN TYPES ENUMARATION
*********************************************************************************************************
*/

typedef enum  HTTPs_token_type {
    HTTPs_TOKEN_TYPE_EXTERNAL,
    HTTPs_TOKEN_TYPE_INTERNAL,
    HTTPs_TOKEN_TYPE_NONE
} HTTPs_TOKEN_TYPE;


/*
*********************************************************************************************************
*                                 KEY-VALUE PAIR DATA TYPES ENUMARATION
*********************************************************************************************************
*/

typedef enum  https_key_val_type {
    HTTPs_KEY_VAL_TYPE_PAIR,
    HTTPs_KEY_VAL_TYPE_VAL,
    HTTPs_KEY_VAL_TYPE_FILE
} HTTPs_KEY_VAL_TYPE;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                              DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                    HTTP SERVER FLAGS DATA TYPE
*********************************************************************************************************
*/

typedef  CPU_INT08U  HTTPs_FLAGS;

#define  HTTPs_FLAG_NONE                            DEF_BIT_NONE

#define  HTTPs_FLAG_INIT                           (DEF_BIT_NONE | HTTPs_FLAG_RESP_CHUNKED)

                                                                /* SOCKET FLAGS             */
#define  HTTPs_FLAG_SOCK_RDY_RD                     DEF_BIT_00
#define  HTTPs_FLAG_SOCK_RDY_WR                     DEF_BIT_01
#define  HTTPs_FLAG_SOCK_RDY_ERR                    DEF_BIT_02

                                                                /* CONN & TRANSACTION FLAGS */
#define  HTTPs_FLAG_CONN_PERSISTENT                 DEF_BIT_00  /* Flag indicating if Connection is persistent.         */
#define  HTTPs_FLAG_REQ_FLUSH                       DEF_BIT_01
#define  HTTPs_FLAG_RESP_BODY_PRESENT               DEF_BIT_02
#define  HTTPs_FLAG_RESP_LOCATION                   DEF_BIT_03  /* Flag indicating Location hdr requirement in resp.    */
#define  HTTPs_FLAG_RESP_CHUNKED                    DEF_BIT_04
#define  HTTPs_FLAG_RESP_CHUNKED_HOOK               DEF_BIT_05


/*
*********************************************************************************************************
*                                     HTTP SERVER TASK DATA TYPE
*********************************************************************************************************
*/

typedef  struct  https_os_task_obj {
    KAL_TASK_HANDLE  TaskHandle;
    KAL_SEM_HANDLE   SemStopReq;
    KAL_SEM_HANDLE   SemStopCompleted;
} HTTPs_OS_TASK_OBJ;


/*
*********************************************************************************************************
*                                           SECURE DATA TYPE
*********************************************************************************************************
*/

typedef  struct  HTTPs_SecureCfg {
    CPU_CHAR                      *CertPtr;
    CPU_INT32U                     CertLen;
    CPU_CHAR                      *KeyPtr;
    CPU_INT32U                     KeyLen;
    NET_SOCK_SECURE_CERT_KEY_FMT   Fmt;
    CPU_BOOLEAN                    CertChain;
} HTTPs_SECURE_CFG;


/*
*********************************************************************************************************
*                                           TOKEN DATA TYPE
*********************************************************************************************************
*/

typedef  struct  https_token_ctrl {
    CPU_CHAR    *ValPtr;
    CPU_INT16U   ValLen;
    CPU_CHAR    *TxPtr;
    CPU_INT16U   TxLen;
} HTTPs_TOKEN_CTRL;

/*
*********************************************************************************************************
*                                      HTTP INTERNAL TOKEN DATA TYPE
*********************************************************************************************************
*/

#define  HTTPs_STR_TOKEN_INTERNAL_STATUS_CODE                "STATUS_CODE"
#define  HTTPs_STR_TOKEN_INTERNAL_REASON_PHRASE              "REASON_PHRASE"

typedef  enum  https_token_int {
    HTTPs_TOKEN_INTERNAL_STATUS_CODE,
    HTTPs_TOKEN_INTERNAL_REASON_PHRASE
} HTTPs_TOKEN_INTERNAL;


/*
*********************************************************************************************************
*                                            FORM DATA TYPE
*********************************************************************************************************
*/

typedef  struct  https_key_val  HTTPs_KEY_VAL;

struct  https_key_val {
    HTTPs_KEY_VAL_TYPE   DataType;
    CPU_CHAR            *KeyPtr;
    CPU_INT16U           KeyLen;
    CPU_CHAR            *ValPtr;
    CPU_INT16U           ValLen;
    HTTPs_KEY_VAL       *NextPtr;
};


/*
*********************************************************************************************************
*                                       HTTP RESP/REQ HDR FIELD BLK
*********************************************************************************************************
*/

typedef  enum  HTTPs_hdr_type {
    HTTPs_HDR_TYPE_NONE,
    HTTPs_HDR_TYPE_REQ,
    HTTPs_HDR_TYPE_RESP
} HTTPs_HDR_TYPE;


/*
*********************************************************************************************************
*                                        CONNECTIONS DATA TYPE
*********************************************************************************************************
*/

typedef  struct  HTTPs_Conn  HTTPs_CONN;

struct  HTTPs_Conn {

    NET_SOCK_ID               SockID;                           /* Connection's Socket ID.                              */
    HTTPs_SOCK_STATE          SockState;                        /* Connection's Socket State.                           */
    HTTPs_FLAGS               SockFlags;                        /* Connection's Socket Ready flags.                     */

    NET_SOCK_ADDR             ClientAddr;                       /* Client socket information.                           */

    HTTPs_ERR                 ErrCode;                          /* Error code when internal error occurs.               */

    HTTPs_CONN_STATE          State;                            /* Connection State.                                    */

    HTTPs_FLAGS               Flags;                            /* Connection and Transaction flags.                    */

    HTTP_METHOD               Method;                           /* HTTP method received in request message.             */
    HTTP_CONTENT_TYPE         ReqContentType;                   /* Content-Type received in request message.            */
    CPU_INT32U                ReqContentLen;                    /* Content-Length received in request message.          */
    CPU_INT32U                ReqContentLenRxd;                 /* Length of Content-Length read.                       */

    HTTP_PROTOCOL_VER         ProtocolVer;                      /* HTTP version received in request message.            */

    CPU_CHAR                 *PathPtr;                          /* Pointer to the URI requested by client               */
    CPU_SIZE_T                PathLenMax;                       /* Maximum file name length.                            */

    void                     *DataPtr;                          /* Pointer to file rx or data to send.                  */
    CPU_INT32U                DataLen;                          /* Data length.                                         */
    CPU_INT32U                DataTxdLen;                       /* The current Length of data sent.                     */
    CPU_INT32U                DataFixPosCur;                    /* Current position in the fixed data.                  */

    HTTP_STATUS_CODE          StatusCode;                       /* Status code of the resp after parsing of the req.    */
    HTTP_CONTENT_TYPE         RespContentType;                  /* Content-Type of file to send                         */
    HTTPs_BODY_DATA_TYPE      RespBodyDataType;                 /* Type of the data of the body (FS, Static, None).     */

#if ((HTTPs_CFG_HDR_RX_EN == DEF_ENABLED) || \
     (HTTPs_CFG_HDR_TX_EN == DEF_ENABLED))
    HTTP_HDR_BLK            *HdrListPtr;                        /* Pointer to list of additional headers.               */
    CPU_INT08U                HdrCtr;                           /* Number of headers in list.                           */
    HTTPs_HDR_TYPE            HdrType;                          /* Type of headers in list : req or resp.               */
#endif

#if (HTTPs_CFG_ABSOLUTE_URI_EN == DEF_ENABLED)
    CPU_CHAR                 *HostPtr;                          /* Ptr to host name received in the request.            */
#endif

#if (HTTPs_CFG_TOKEN_PARSE_EN == DEF_ENABLED)
    HTTPs_TOKEN_CTRL         *TokenCtrlPtr;                     /* Ptr to the ctrl token structure.                     */
    CPU_CHAR                 *TokenPtr;                         /* Ptr to the token name.                               */
    CPU_INT16U                TokenLen;                         /* Length of the token name.                            */
    CPU_INT16U                TokenBufRemLen;                   /* Remaining data len in the buf.                       */
#endif

#if (HTTPs_CFG_QUERY_STR_EN == DEF_ENABLED)
    CPU_INT16U                QueryStrBlkAcquiredCtr;           /* Counter for Query String Key-Val block acquired.     */
    HTTPs_KEY_VAL            *QueryStrListPtr;                  /* Ptr to the list of key pair-value rxd in query str.  */
#endif

#if (HTTPs_CFG_FORM_EN == DEF_ENABLED)
    CPU_INT16U                FormBlkAcquiredCtr;               /* Counter for Form Key-Val block acquired.             */
    HTTPs_KEY_VAL            *FormDataListPtr;                  /* Ptr to list of Form key pair-value rxd in POST.      */

#if (HTTPs_CFG_FORM_MULTIPART_EN == DEF_ENABLED)
    CPU_CHAR                 *FormBoundaryPtr;                  /* Ptr to the boundary for Multipart Form parsing.      */
    CPU_INT08U                FormBoundaryLen;
#endif
#endif

    CPU_CHAR                 *BufPtr;                           /* Ptr to conn buf.                                     */
    CPU_INT16U                BufLen;                           /* Conn buf len.                                        */

    CPU_CHAR                 *RxBufPtr;                         /* Ptr where to receive buf.                            */
    CPU_INT16U                RxBufLenRem;                      /* Rem data in the rx buffer.                           */
    CPU_INT32U                RxDataLen;                        /* Len of data rxd.                                     */

    CPU_CHAR                 *TxBufPtr;                         /* Ptr to buf to tx.                                    */
    CPU_INT16U                TxDataLen;                        /* Data len to tx.                                      */

    void                     *ConnDataPtr;                      /* Ptr for user conn data.                              */
    CPU_INT32U                ConnDataType;                     /* Type for user conn data.                             */

    HTTPs_CONN               *ConnPrevPtr;                      /* Pointer to previous connection.                      */
    HTTPs_CONN               *ConnNextPtr;                      /* Pointer to next connection.                          */
};


/*
*********************************************************************************************************
*                                     HOOK CONFIGURATION DATA TYPE
*********************************************************************************************************
*/

typedef  struct  HTTPs_Instance     HTTPs_INSTANCE;


typedef  CPU_BOOLEAN     (*HTTPs_INSTANCE_INIT_HOOK)      (const  HTTPs_INSTANCE         *p_instance,
                                                           const  void                   *p_hook_cfg);

typedef  CPU_BOOLEAN     (*HTTPs_REQ_HDR_RX_HOOK)         (const  HTTPs_INSTANCE         *p_instance,
                                                           const  HTTPs_CONN             *p_conn,
                                                           const  void                   *p_hook_cfg,
                                                                  HTTP_HDR_FIELD          hdr_field);

typedef  CPU_BOOLEAN     (*HTTPs_REQ_HOOK)                (const  HTTPs_INSTANCE         *p_instance,
                                                                  HTTPs_CONN             *p_conn,
                                                           const  void                   *p_hook_cfg);

typedef  CPU_BOOLEAN     (*HTTPs_REQ_BODY_RX_HOOK)        (const  HTTPs_INSTANCE         *p_instance,
                                                                  HTTPs_CONN             *p_conn,
                                                           const  void                   *p_hook_cfg,
                                                                  void                   *p_buf,
                                                           const  CPU_SIZE_T              buf_size,
                                                                  CPU_SIZE_T             *p_buf_size_used);

typedef  CPU_BOOLEAN     (*HTTPs_REQ_RDY_SIGNAL_HOOK)     (const  HTTPs_INSTANCE         *p_instance,
                                                                  HTTPs_CONN             *p_conn,
                                                           const  void                   *p_hook_cfg,
                                                           const  HTTPs_KEY_VAL          *p_data);

typedef  CPU_BOOLEAN     (*HTTPs_REQ_RDY_POLL_HOOK)       (const  HTTPs_INSTANCE         *p_instance,
                                                                  HTTPs_CONN             *p_conn,
                                                           const  void                   *p_hook_cfg);

typedef  CPU_BOOLEAN     (*HTTPs_RESP_HDR_TX_HOOK)        (const  HTTPs_INSTANCE         *p_instance,
                                                                  HTTPs_CONN             *p_conn,
                                                           const  void                   *p_hook_cfg);

typedef  CPU_BOOLEAN     (*HTTPs_RESP_TOKEN_HOOK)         (const  HTTPs_INSTANCE         *p_instance,
                                                                  HTTPs_CONN             *p_conn,
                                                           const  void                   *p_hook_cfg,
                                                           const  CPU_CHAR               *p_token,
                                                                  CPU_INT16U              token_len,
                                                                  CPU_CHAR               *p_val,
                                                                  CPU_INT16U              val_len_max);

typedef  CPU_BOOLEAN     (*HTTPs_RESP_CHUNK_HOOK)         (const  HTTPs_INSTANCE         *p_instance,
                                                                  HTTPs_CONN             *p_conn,
                                                           const  void                   *p_hook_cfg,
                                                                  void                   *p_buf,
                                                                  CPU_SIZE_T              buf_len_max,
                                                                  CPU_SIZE_T             *p_tx_len);

typedef  void            (*HTTPs_TRANS_COMPLETE_HOOK)     (const  HTTPs_INSTANCE         *p_instance,
                                                                  HTTPs_CONN             *p_conn,
                                                           const  void                   *p_hook_cfg);

typedef  void            (*HTTPs_ERR_HOOK)                (const  HTTPs_INSTANCE         *p_instance,
                                                                  HTTPs_CONN             *p_conn,
                                                           const  void                   *p_hook_cfg,
                                                                  HTTPs_ERR               err);

typedef  void            (*HTTPs_ERR_FILE_GET_HOOK)       (const  void                   *p_hook_cfg,
                                                                  HTTP_STATUS_CODE        status_code,
                                                                  CPU_CHAR               *p_file_str,
                                                                  CPU_INT32U              file_len_max,
                                                                  HTTPs_BODY_DATA_TYPE   *p_file_type,
                                                                  HTTP_CONTENT_TYPE      *p_content_type,
                                                                  void                  **p_data,
                                                                  CPU_INT32U             *p_date_len);

typedef  void            (*HTTPs_CONN_CLOSE_HOOK)         (const  HTTPs_INSTANCE         *p_instance,
                                                                  HTTPs_CONN             *p_conn,
                                                           const  void                   *p_hook_cfg);


typedef  struct  https_Hook_Cfg {
    HTTPs_INSTANCE_INIT_HOOK    OnInstanceInitHook;
    HTTPs_REQ_HDR_RX_HOOK       OnReqHdrRxHook;
    HTTPs_REQ_HOOK              OnReqHook;
    HTTPs_REQ_BODY_RX_HOOK      OnReqBodyRxHook;
    HTTPs_REQ_RDY_SIGNAL_HOOK   OnReqRdySignalHook;
    HTTPs_REQ_RDY_POLL_HOOK     OnReqRdyPollHook;
    HTTPs_RESP_HDR_TX_HOOK      OnRespHdrTxHook;
    HTTPs_RESP_TOKEN_HOOK       OnRespTokenHook;
    HTTPs_RESP_CHUNK_HOOK       OnRespChunkHook;
    HTTPs_TRANS_COMPLETE_HOOK   OnTransCompleteHook;
    HTTPs_ERR_HOOK              OnErrHook;
    HTTPs_ERR_FILE_GET_HOOK     OnErrFileGetHook;
    HTTPs_CONN_CLOSE_HOOK       OnConnCloseHook;
} HTTPs_HOOK_CFG;


/*
*********************************************************************************************************
*                                     HEADER CONFIGURATION DATA TYPE
*********************************************************************************************************
*/

typedef  struct  https_hdr_rx_cfg {
    CPU_INT16U    NbrPerConnMax;
    CPU_INT16U    DataLenMax;
} HTTPs_HDR_RX_CFG;


typedef  struct  https_hdr_tx_cfg {
    CPU_INT16U    NbrPerConnMax;
    CPU_INT16U    DataLenMax;
} HTTPs_HDR_TX_CFG;


/*
*********************************************************************************************************
*                                     QUERY STRING CONFIGURATION DATA TYPE
*********************************************************************************************************
*/

typedef  struct  https_query_str_cfg {
    CPU_INT16U    NbrPerConnMax;
    CPU_INT16U    KeyLenMax;
    CPU_INT16U    ValLenMax;
} HTTPs_QUERY_STR_CFG;


/*
*********************************************************************************************************
*                                     FORM CONFIGURATION DATA TYPE
*********************************************************************************************************
*/

typedef  struct  https_form_cfg {
    CPU_INT16U    NbrPerConnMax;
    CPU_INT16U    KeyLenMax;
    CPU_INT16U    ValLenMax;

    CPU_BOOLEAN   MultipartEn;
    CPU_BOOLEAN   MultipartFileUploadEn;
    CPU_BOOLEAN   MultipartFileUploadOverWrEn;
    CPU_CHAR     *MultipartFileUploadFolderPtr;
} HTTPs_FORM_CFG;


/*
*********************************************************************************************************
*                                     TOKEN CONFIGURATION DATA TYPE
*********************************************************************************************************
*/

typedef  struct  https_token_cfg {
    CPU_INT16U    NbrPerConnMax;
    CPU_INT16U    ValLenMax;
} HTTPs_TOKEN_CFG;


/*
*********************************************************************************************************
*                                        FS CONFIGURATION DATA TYPE
*********************************************************************************************************
*/

typedef  enum  https_fs_type {
    HTTPs_FS_TYPE_NONE,
    HTTPs_FS_TYPE_STATIC,
    HTTPs_FS_TYPE_DYN
} HTTPs_FS_TYPE;

typedef  struct  https_cfg_fs_none {
    CPU_INT32U   PathLenMax;
} HTTPs_CFG_FS_NONE;

typedef  struct  https_cfg_fs_static {
    const  NET_FS_API  *FS_API_Ptr;
} HTTPs_CFG_FS_STATIC;

typedef  struct  https_cfg_fs_dyn {
    const  NET_FS_API  *FS_API_Ptr;
           CPU_CHAR    *WorkingFolderNamePtr;
} HTTPs_CFG_FS_DYN;


/*
*********************************************************************************************************
*                                     INSTANCE CONFIGURATION DATA TYPE
*********************************************************************************************************
*/

typedef  struct  https_cfg {

/*
*--------------------------------------------------------------------------------------------------------
*                                      INSTANCE OS CONFIGURATION
*--------------------------------------------------------------------------------------------------------
*/

           CPU_INT32U            OS_TaskDly_ms;


/*
*--------------------------------------------------------------------------------------------------------
*                                INSTANCE LISTEN SOCKET CONFIGURATION
*--------------------------------------------------------------------------------------------------------
*/

           HTTPs_SOCK_SEL        SockSel;
           HTTPs_SECURE_CFG     *SecurePtr;
           CPU_INT16U            Port;

/*
*--------------------------------------------------------------------------------------------------------
*                                  INSTANCE CONNECTION CONFIGURATION
*--------------------------------------------------------------------------------------------------------
*/

           CPU_INT08U            ConnNbrMax;
           CPU_INT16U            ConnInactivityTimeout_s;
           CPU_INT16U            BufLen;
           CPU_BOOLEAN           ConnPersistentEn;

/*
*--------------------------------------------------------------------------------------------------------
*                                  INSTANCE FILE SYSTEM CONFIGURATION
*--------------------------------------------------------------------------------------------------------
*/
           HTTPs_FS_TYPE         FS_Type;
    const  void                 *FS_CfgPtr;
           CPU_CHAR             *DfltResourceNamePtr;


/*
*--------------------------------------------------------------------------------------------------------
*                                    INSTANCE PROXY CONFIGURATION
*--------------------------------------------------------------------------------------------------------
*/

           CPU_INT16U            HostNameLenMax;


/*
*--------------------------------------------------------------------------------------------------------
*                                         HOOK CONFIGURATION
*--------------------------------------------------------------------------------------------------------
*/

    const  HTTPs_HOOK_CFG       *HooksPtr;
    const  void                 *Hooks_CfgPtr;

/*
*--------------------------------------------------------------------------------------------------------
*                                     HEADER FIELD CONFIGURATION
*--------------------------------------------------------------------------------------------------------
*/

    const  HTTPs_HDR_RX_CFG     *HdrRxCfgPtr;
    const  HTTPs_HDR_TX_CFG     *HdrTxCfgPtr;


/*
*--------------------------------------------------------------------------------------------------------
*                                     QUERY STRING CONFIGURATION
*--------------------------------------------------------------------------------------------------------
*/

    const  HTTPs_QUERY_STR_CFG  *QueryStrCfgPtr;


/*
*--------------------------------------------------------------------------------------------------------
*                                    INSTANCE FORM CONFIGURATION
*--------------------------------------------------------------------------------------------------------
*/

    const  HTTPs_FORM_CFG       *FormCfgPtr;


/*
*--------------------------------------------------------------------------------------------------------
*                               DYNAMIC TOKEN REPLACEMENT CONFIGURATION
*--------------------------------------------------------------------------------------------------------
*/

    const  HTTPs_TOKEN_CFG      *TokenCfgPtr;


} HTTPs_CFG;                                                    /* End of configuration structure.                      */


/*
*********************************************************************************************************
*                                INSTANCE STATISTIC COUNTERS DATA TYPE
*********************************************************************************************************
*/

typedef  struct  HTTPs_InstanceStats {
    CPU_INT32U  Conn_StatAcceptedCtr;
    CPU_INT32U  Conn_StatClosedCtr;
    CPU_INT32U  Conn_StatAcquiredCtr;
    CPU_INT32U  Conn_StatReleasedCtr;

    CPU_INT32U  Sock_StatListenCloseCtr;
    CPU_INT32U  Sock_StatOctetRxdCtr;
    CPU_INT32U  Sock_StatOctetTxdCtr;

    CPU_INT32U  FS_StatOpenedCtr;
    CPU_INT32U  FS_StatClosedCtr;

#if (HTTPs_CFG_HDR_RX_EN == DEF_ENABLED)
    CPU_INT32U  Req_StatHdrAcquiredCtr;
    CPU_INT32U  Req_StatHdrReleaseCtr;
#endif
#if (HTTPs_CFG_HDR_TX_EN == DEF_ENABLED)
    CPU_INT32U  Resp_StatHdrAcquiredCtr;
    CPU_INT32U  Resp_StatHdrReleaseCtr;
#endif

#if (HTTPs_CFG_TOKEN_PARSE_EN == DEF_ENABLED)
    CPU_INT32U  Resp_StatTokenAcquiredCtr;
    CPU_INT32U  Resp_StatTokenReleaseCtr;
    CPU_INT32U  Resp_StatTokenFoundCtr;
#endif

#if ((HTTPs_CFG_FORM_EN      == DEF_ENABLED) || \
     (HTTPs_CFG_QUERY_STR_EN == DEF_ENABLED))
    CPU_INT32U  Req_StatKeyValAcquiredCtr;
    CPU_INT32U  Req_StatKeyValReleaseCtr;
#endif

#if ((HTTPs_CFG_FORM_EN            == DEF_ENABLED) && \
     (HTTPs_CFG_FORM_MULTIPART_EN  == DEF_ENABLED))
    CPU_INT32U  Req_StatFormFileUploadOpenedCtr;
    CPU_INT32U  Req_StatFormFileUploadClosedCtr;
    CPU_INT32U  Req_StatFormFileUploadNoFS_Ctr;
#endif

    CPU_INT32U  Req_StatContentTypeFormAppRxdCtr;
    CPU_INT32U  Req_StatContentTypeFormMultipartRxdCtr;
    CPU_INT32U  Req_StatContentTypeOtherRxdCtr;
    CPU_INT32U  Req_StatContentTypeUnknownRxdCtr;

    CPU_INT32U  Req_StatRxdCtr;
    CPU_INT32U  Req_StatProcessedCtr;
    CPU_INT32U  Req_StatMethodGetRxdCtr;
    CPU_INT32U  Req_StatMethodHeadRxdCtr;
    CPU_INT32U  Req_StatMethodPostRxdCtr;
    CPU_INT32U  Req_StatMethodDeleteRxdCtr;
    CPU_INT32U  Req_StatMethodPutRxdCtr;
    CPU_INT32U  Req_StatMethodUnsupportedRxdCtr;
    CPU_INT32U  Req_StatProtocolVer0_9Ctr;
    CPU_INT32U  Req_StatProtocolVer1_0Ctr;
    CPU_INT32U  Req_StatProtocolVer1_1Ctr;
    CPU_INT32U  Req_StatProtocolVerUnsupportedCtr;

    CPU_INT32U  Req_StatMethodGetProcessedCtr;
    CPU_INT32U  Req_StatMethodHeadProcessedCtr;
    CPU_INT32U  Req_StatMethodPostProcessedCtr;
    CPU_INT32U  Req_StatMethodDeleteProcessedCtr;
    CPU_INT32U  Req_StatMethodPutProcessedCtr;
    CPU_INT32U  Req_StatMethodUnsupportedProcessedCtr;

    CPU_INT32U  Resp_StatTxdCtr;
    CPU_INT32U  Resp_StatBodyTxdCtr;

    CPU_INT32U  Resp_StatStatusCodeOKCtr;
    CPU_INT32U  Resp_StatStatusCodeMovedPermanentlyCtr;
    CPU_INT32U  Resp_StatStatusCodeFoundCtr;
    CPU_INT32U  Resp_StatStatusCodeSeeOtherCtr;
    CPU_INT32U  Resp_StatStatusCodeNotModifiedCtr;
    CPU_INT32U  Resp_StatStatusCodeUseProxyCtr;
    CPU_INT32U  Resp_StatStatusCodeTemporaryredirectCtr;
    CPU_INT32U  Resp_StatStatusCodeCreatedCtr;
    CPU_INT32U  Resp_StatStatusCodeAcceptedCtr;
    CPU_INT32U  Resp_StatStatusCodeNoContentCtr;
    CPU_INT32U  Resp_StatStatusCodeResetContentCtr;
    CPU_INT32U  Resp_StatStatusCodeBadRequestCtr;
    CPU_INT32U  Resp_StatStatusCodeUnauthorizedCtr;
    CPU_INT32U  Resp_StatStatusCodeForbiddenCtr;
    CPU_INT32U  Resp_StatStatusCodeNotFoundCtr;
    CPU_INT32U  Resp_StatStatusCodeMethodNotAllowedCtr;
    CPU_INT32U  Resp_StatStatusCodeNotAcceptableCtr;
    CPU_INT32U  Resp_StatStatusCodeRequestTimeoutCtr;
    CPU_INT32U  Resp_StatStatusCodeConflitCtr;
    CPU_INT32U  Resp_StatStatusCodeGoneCtr;
    CPU_INT32U  Resp_StatStatusCodeLenRequiredCtr;
    CPU_INT32U  Resp_StatStatusCodeConditionFailedCtr;
    CPU_INT32U  Resp_StatStatusCodeEntityTooLongCtr;
    CPU_INT32U  Resp_StatStatusCodeURI_TooLongCtr;
    CPU_INT32U  Resp_StatStatusCodeUnsupportedTypeCtr;
    CPU_INT32U  Resp_StatStatusCodeNotSatisfiableCtr;
    CPU_INT32U  Resp_StatStatusCodeExpectationFailedCtr;
    CPU_INT32U  Resp_StatStatusCodeInternalServerErrCtr;
    CPU_INT32U  Resp_StatStatusCodeNotImplementedCtr;
    CPU_INT32U  Resp_StatStatusCodeSerUnavailableCtr;
    CPU_INT32U  Resp_StatStatusCodeVerNotSupportedCtr;
    CPU_INT32U  Resp_StatStatusCodeUnknownCtr;
} HTTPs_INSTANCE_STATS;


/*
*********************************************************************************************************
*                                  INSTANCE ERROR COUNTERS DATA TYPE
*********************************************************************************************************
*/

typedef  struct  HTTPs_InstanceErrs {
    CPU_INT32U  Conn_ErrFreePtrNullCtr;

    CPU_INT32U  Conn_ErrPoolMemSpaceCtr;
    CPU_INT32U  Conn_ErrPoolEmptyCtr;
    CPU_INT32U  Conn_ErrPoolLibGetCtr;
    CPU_INT32U  Conn_ErrPoolLibFreeCtr;

    CPU_INT32U  Conn_ErrBufPoolMemSpaceCtr;
    CPU_INT32U  Conn_ErrBufPoolEmptyCtr;
    CPU_INT32U  Conn_ErrBufPoolLibGetCtr;
    CPU_INT32U  Conn_ErrBufPoolLibFreeCtr;

    CPU_INT32U  Conn_ErrPathPoolMemSpaceCtr;
    CPU_INT32U  Conn_ErrPathPoolEmptyCtr;
    CPU_INT32U  Conn_ErrPathPoolLibGetCtr;
    CPU_INT32U  Conn_ErrPathPoolLibFreeCtr;

    CPU_INT32U  Conn_ErrHdrTypeInvalidCtr;

    CPU_INT32U  Conn_ErrNoneAvailCtr;
    CPU_INT32U  Conn_ErrTmrStartCtr;

    CPU_INT32U  Sock_ErrListenCloseCtr;
    CPU_INT32U  Sock_ErrAcceptCtr;
    CPU_INT32U  Sock_ErrSelCtr;
    CPU_INT32U  Sock_ErrCloseCtr;
    CPU_INT32U  Sock_ErrRxCtr;
    CPU_INT32U  Sock_ErrRxConnClosedCtr;
    CPU_INT32U  Sock_ErrRxFaultCtr;
    CPU_INT32U  Sock_ErrTxConnClosedCtr;
    CPU_INT32U  Sock_ErrTxFaultCtr;

#if (HTTPs_CFG_ABSOLUTE_URI_EN == DEF_ENABLED)
    CPU_INT32U  Host_ErrPoolMemSpaceCtr;
    CPU_INT32U  Host_ErrPoolEmptyCtr;
    CPU_INT32U  Host_ErrPoolLibGetCtr;
    CPU_INT32U  Host_ErrPoolLibFreeCtr;
#endif

    CPU_INT32U  Req_ErrInvalidCtr;
    CPU_INT32U  Req_ErrStateUnkownCtr;
    CPU_INT32U  Req_ErrBodyStateUnknownCtr;
    CPU_INT32U  Req_ErrBodyPostFormCtr;
    CPU_INT32U  Req_ErrBodyFormNotEn;
    CPU_INT32U  Req_ErrBodyFormMultipartNotEn;

#if ((HTTPs_CFG_QUERY_STR_EN == DEF_ENABLED) || \
     (HTTPs_CFG_FORM_EN      == DEF_ENABLED))
    CPU_INT32U  Req_ErrKeyValPoolMemSpaceCtr;
    CPU_INT32U  Req_ErrKeyValPoolLibGetCtr;
    CPU_INT32U  Req_ErrKeyValPoolLibFreeCtr;
#endif

#if (HTTPs_CFG_QUERY_STR_EN == DEF_ENABLED)
    CPU_INT32U  Req_ErrQueryStrKeyPoolMemSpaceCtr;
    CPU_INT32U  Req_ErrQueryStrKeyPoolLibGetCtr;
    CPU_INT32U  Req_ErrQueryStrKeyPoolLibFreeCtr;
    CPU_INT32U  Req_ErrQueryStrValPoolMemSpaceCtr;
    CPU_INT32U  Req_ErrQueryStrValPoolLibGetCtr;
    CPU_INT32U  Req_ErrQueryStrValPoolLibFreeCtr;
#endif

#if (HTTPs_CFG_FORM_EN == DEF_ENABLED)
    CPU_INT32U  Req_ErrFormKeyPoolMemSpaceCtr;
    CPU_INT32U  Req_ErrFormKeyPoolLibGetCtr;
    CPU_INT32U  Req_ErrFormKeyPoolLibFreeCtr;
    CPU_INT32U  Req_ErrFormValPoolMemSpaceCtr;
    CPU_INT32U  Req_ErrFormValPoolLibGetCtr;
    CPU_INT32U  Req_ErrFormValPoolLibFreeCtr;

#if (HTTPs_CFG_FORM_MULTIPART_EN == DEF_ENABLED)
    CPU_INT32U  Req_ErrFormBoundaryPoolMemSpaceCtr;
    CPU_INT32U  Req_ErrFormBoundaryPoolEmptyCtr;
    CPU_INT32U  Req_ErrFormBoundaryPoolLibGetCtr;
    CPU_INT32U  Req_ErrFormBoundaryPoolLibFreeCtr;
#endif
#endif

#if (HTTPs_CFG_HDR_RX_EN == DEF_ENABLED)
    CPU_INT32U  Req_ErrHdrPtrNullCtr;

    CPU_INT32U  Req_ErrHdrPoolMemSpaceCtr;
    CPU_INT32U  Req_ErrHdrPoolEmptyCtr;
    CPU_INT32U  Req_ErrHdrPoolLibGetCtr;
    CPU_INT32U  Req_ErrHdrPoolLibFreeCtr;

    CPU_INT32U  Req_ErrHdrBufPoolMemSpaceCtr;
    CPU_INT32U  Req_ErrHdrBufPoolEmptyCtr;
    CPU_INT32U  Req_ErrHdrBufPoolLibGetCtr;
    CPU_INT32U  Req_ErrHdrBufPoolLibFreeCtr;

    CPU_INT32U  Req_ErrHdrValTypeUnknown;

    CPU_INT32U  Req_ErrHdrDataLenInv;
#endif

#if (HTTPs_CFG_HDR_TX_EN == DEF_ENABLED)
    CPU_INT32U  Resp_ErrHdrPtrNullCtr;

    CPU_INT32U  Resp_ErrHdrPoolMemSpaceCtr;
    CPU_INT32U  Resp_ErrHdrPoolEmptyCtr;
    CPU_INT32U  Resp_ErrHdrPoolLibGetCtr;
    CPU_INT32U  Resp_ErrHdrPoolLibFreeCtr;

    CPU_INT32U  Resp_ErrHdrBufPoolMemSpaceCtr;
    CPU_INT32U  Resp_ErrHdrBufPoolEmptyCtr;
    CPU_INT32U  Resp_ErrHdrBufPoolLibGetCtr;
    CPU_INT32U  Resp_ErrHdrBufPoolLibFreeCtr;

    CPU_INT32U  Resp_ErrHdrValTypeUnknown;

    CPU_INT32U  Resp_ErrHdrCloseNotEmptyCtr;
#endif

#if (HTTPs_CFG_TOKEN_PARSE_EN == DEF_ENABLED)
    CPU_INT32U  Resp_ErrTokenPtrNullCtr;

    CPU_INT32U  Resp_ErrTokenPoolMemSpaceCtr;
    CPU_INT32U  Resp_ErrTokenPoolEmptyCtr;
    CPU_INT32U  Resp_ErrTokenPoolLibGetCtr;
    CPU_INT32U  Resp_ErrTokenPoolLibFreeCtr;

    CPU_INT32U  Resp_ErrTokenValPoolMemSpaceCtr;
    CPU_INT32U  Resp_ErrTokenValPoolEmptyCtr;
    CPU_INT32U  Resp_ErrTokenValPoolLibGetCtr;
    CPU_INT32U  Resp_ErrTokenValPoolLibFreeCtr;

    CPU_INT32U  Resp_ErrTokenCloseNotEmptyCtr;
    CPU_INT32U  Resp_ErrTokenClrNotEmptyCtr;

    CPU_INT32U  Resp_ErrTokenInternalInvalidCtr;
    CPU_INT32U  Resp_ErrTokenInternalStatusCodeInvalidCtr;
    CPU_INT32U  Resp_ErrTokenInternalReasonPhraseInvalidCtr;

    CPU_INT32U  Resp_ErrTokenTypeInvalidCtr;
#endif

    CPU_INT32U  Resp_ErrBodyTypeInvalidCtr;
    CPU_INT32U  Resp_ErrPathInvalidCtr;
    CPU_INT32U  Resp_ErrContentTypeInvalidCtr;
    CPU_INT32U  Resp_ErrPrepareCtr;
    CPU_INT32U  Resp_ErrPrepareErrPageCtr;
    CPU_INT32U  Resp_ErrStatusLineCtr;
    CPU_INT32U  Resp_ErrHdrCtr;
    CPU_INT32U  Resp_ErrTransferStdCtr;
    CPU_INT32U  Resp_ErrTransferChunkedCtr;
    CPU_INT32U  Resp_ErrTransferChunkedStateInvCtr;
    CPU_INT32U  Resp_ErrTransferChunkedHookCtr;
    CPU_INT32U  Resp_ErrTransferChunkedHookStateInvCtr;
    CPU_INT32U  Resp_ErrStateUnknownCtr;

    CPU_INT32U  FS_ErrNoEnCtr;
    CPU_INT32U  FS_ErrTypeInvalidCtr;
    CPU_INT32U  FS_ErrWorkingFolderInvalidCtr;

    CPU_INT32U  File_ErrOpenNoFS_Ctr;
    CPU_INT32U  File_ErrCloseNoFS_Ctr;
    CPU_INT32U  File_ErrRdNoFS_Ctr;
    CPU_INT32U  File_ErrSetPosNoFS_Ctr;

    CPU_INT32U  ErrInternal_ReqMethodNotSupported;
    CPU_INT32U  ErrInternal_ReqFormatInvalid;
    CPU_INT32U  ErrInternal_ReqURI_Len;
    CPU_INT32U  ErrInternal_ReqProtocolNotSupported;
    CPU_INT32U  ErrInternal_ReqMoreDataRequired;
    CPU_INT32U  ErrInternal_ReqHdrOverflow;

    CPU_INT32U  ErrInternal_ReqBodyFormFormatInvalid;
    CPU_INT32U  ErrInternal_ReqBodyFormFileUploadOpen;

    CPU_INT32U  ErrInternal_ReqKeyValPoolSizeInvalid;

    CPU_INT32U  ErrInternal_StateUnknown;
    CPU_INT32U  ErrInternal_Unknown;

} HTTPs_INSTANCE_ERRS;


/*
*********************************************************************************************************
*                                         INSTANCE DATA TYPE
*********************************************************************************************************
*/

typedef  CPU_INT08U                 HTTPs_INSTANCE_ID;
#define  HTTPs_INSTANCE_ID_NONE     0

struct  HTTPs_Instance {
           HTTPs_INSTANCE_ID      ID;
           HTTPs_OS_TASK_OBJ     *OS_TaskObjPtr;
           KAL_LOCK_HANDLE        OS_LockObj;
           KAL_LOCK_HANDLE        ConnSelAbortLockObj;
           CPU_BOOLEAN            Started;

    const  HTTPs_CFG             *CfgPtr;

    const  NET_TASK_CFG          *TaskCfgPtr;

           void                  *DataPtr;

#ifdef   NET_IPv4_MODULE_EN
           NET_SOCK_ID            SockListenID_IPv4;
#endif

#ifdef   NET_IPv6_MODULE_EN
           NET_SOCK_ID            SockListenID_IPv6;
#endif

#if (HTTPs_CFG_FS_PRESENT_EN == DEF_ENABLED)
           CPU_INT32U             FS_PathLenMax;
           CPU_CHAR               FS_PathSepChar;
#endif

           MEM_DYN_POOL           PoolConn;
           MEM_DYN_POOL           PoolBuf;
           MEM_DYN_POOL           PoolPath;

#if (HTTPs_CFG_ABSOLUTE_URI_EN == DEF_ENABLED)
           MEM_DYN_POOL           PoolHost;
#endif

#if (HTTPs_CFG_TOKEN_PARSE_EN == DEF_ENABLED)
           MEM_DYN_POOL           PoolTokenCtrl;
           MEM_DYN_POOL           PoolTokenVal;
#endif


#if ((HTTPs_CFG_FORM_EN      == DEF_ENABLED) || \
     (HTTPs_CFG_QUERY_STR_EN == DEF_ENABLED))
           MEM_DYN_POOL           PoolKeyVal;
#endif

#if (HTTPs_CFG_FORM_EN == DEF_ENABLED)
           MEM_DYN_POOL           PoolFormKeyStr;
           MEM_DYN_POOL           PoolFormValStr;
#endif

#if (HTTPs_CFG_QUERY_STR_EN == DEF_ENABLED)
           MEM_DYN_POOL           PoolQueryStrKeyStr;
           MEM_DYN_POOL           PoolQueryStrValStr;
#endif

#if ((HTTPs_CFG_FORM_EN           == DEF_ENABLED) || \
     (HTTPs_CFG_FORM_MULTIPART_EN == DEF_ENABLED))
           MEM_DYN_POOL           PoolFormBoundary;
#endif


#if (HTTPs_CFG_HDR_RX_EN == DEF_ENABLED)
           MEM_DYN_POOL           PoolReqHdr;
           MEM_DYN_POOL           PoolReqHdrStr;
#endif

#if (HTTPs_CFG_HDR_TX_EN == DEF_ENABLED)
           MEM_DYN_POOL           PoolRespHdr;
           MEM_DYN_POOL           PoolRespHdrStr;
#endif

           HTTPs_CONN            *ConnFirstPtr;
           HTTPs_CONN            *ConnLastPtr;

           CPU_INT08U             ConnActiveCtr;

#if (HTTPs_CFG_DBG_INFO_EN == DEF_ENABLED)
           HTTPs_INSTANCE        *InstancePrevPtr;
           HTTPs_INSTANCE        *InstanceNextPtr;
#endif

#if (HTTPs_CFG_CTR_STAT_EN == DEF_ENABLED)
           HTTPs_INSTANCE_STATS   StatsCtr;
#endif

#if (HTTPs_CFG_CTR_ERR_EN  == DEF_ENABLED)
           HTTPs_INSTANCE_ERRS    ErrsCtr;
#endif
           CPU_BOOLEAN            SelAbortReq;
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           GLOBAL VARIABLES
*********************************************************************************************************
*********************************************************************************************************
*/

HTTPs_EXT  CPU_INT16U  HTTPs_InstanceRunningNbr;
HTTPs_EXT  CPU_INT16U  HTTPs_InstanceInitializedNbr;

extern  const  HTTP_DICT   HTTPs_DictionaryTokenInternal[];
extern         CPU_SIZE_T  HTTPs_DictionarySizeTokenInternal;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            HOOK MACRO
*********************************************************************************************************
*/

#define  HTTPs_HOOK_DEFINED(hookCfg, hookName)              ((hookCfg != DEF_NULL) && (hookCfg->hookName != DEF_NULL))


/*
*********************************************************************************************************
*                                        HTTPs COUNTER MACRO'S
*
* Description : Functionality to set and increment statistic and error counter
*
* Argument(s) : Various HTTP server counter variable(s) & values.
*
* Return(s)   : none.
*
* Caller(s)   : various.
*
*               These macro's are INTERNAL HTTP server suite macro's & SHOULD NOT be called by
*               application function(s).
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (HTTPs_CFG_CTR_STAT_EN == DEF_ENABLED)
    #define  HTTPs_SET_PTR_STATS(p_ctr_stats, p_instance)   {                                           \
                                                                p_ctr_stats = &p_instance->StatsCtr;    \
                                                            }

    #define  HTTPs_STATS_INC(p_ctr)                         {                                           \
                                                                 p_ctr++;                               \
                                                            }

    #define  HTTPs_STATS_OCTET_INC(p_ctr, octet)            {                                           \
                                                                 p_ctr += octet;                        \
                                                            }

#else
                                                                /* Prevent 'variable unused' compiler warning.          */
    #define  HTTPs_SET_PTR_STATS(p_ctr_stats, p_instance)   {                                           \
                                                               (void)p_ctr_stats;                       \
                                                            }

    #define  HTTPs_STATS_INC(p_ctr)

    #define  HTTPs_STATS_OCTET_INC(p_ctr, octet)
#endif



#if (HTTPs_CFG_CTR_ERR_EN == DEF_ENABLED)
    #define  HTTPs_SET_PTR_ERRS(p_ctr_err, p_instance)      {                                           \
                                                                 p_ctr_err = &p_instance->ErrsCtr;      \
                                                            }

    #define  HTTPs_ERR_INC(p_ctr)                           {                                           \
                                                                 p_ctr++;                               \
                                                            }

#else
                                                                 /* Prevent 'variable unused' compiler warning.          */
    #define  HTTPs_SET_PTR_ERRS(p_ctr_err, p_instance)      {                                           \
                                                               (void)p_ctr_err;                         \
                                                            }

    #define  HTTPs_ERR_INC(p_ctr)
#endif



/*
*********************************************************************************************************
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

void             HTTPs_Init                       (       MEM_SEG               *p_mem_seg,
                                                          HTTPs_ERR             *p_err);

HTTPs_INSTANCE  *HTTPs_InstanceInit               (const  HTTPs_CFG             *p_cfg,
                                                   const  NET_TASK_CFG          *p_task_cfg,
                                                          HTTPs_ERR             *p_err);

void             HTTPs_InstanceStart              (       HTTPs_INSTANCE        *p_instance,
                                                          HTTPs_ERR             *p_err);

void             HTTPs_InstanceStop               (       HTTPs_INSTANCE        *p_instance,
                                                          CPU_BOOLEAN            graceful_stop_en,
                                                          HTTPs_ERR             *p_err);


HTTP_HDR_BLK    *HTTPs_RespHdrGet                 (const  HTTPs_INSTANCE        *p_instance,
                                                          HTTPs_CONN            *p_conn,
                                                          HTTP_HDR_FIELD         hdr_field,
                                                          HTTP_HDR_VAL_TYPE      val_type,
                                                          HTTPs_ERR             *p_err);

void             HTTPs_RespBodySetParamFile       (const  HTTPs_INSTANCE        *p_instance,
                                                          HTTPs_CONN            *p_conn,
                                                          CPU_CHAR              *p_path,
                                                          HTTP_CONTENT_TYPE      content_type,
                                                          CPU_BOOLEAN            token_en,
                                                          HTTPs_ERR             *p_err);

void             HTTPs_RespBodySetParamStaticData (const  HTTPs_INSTANCE        *p_instance,
                                                          HTTPs_CONN            *p_conn,
                                                          HTTP_CONTENT_TYPE      content_type,
                                                          void                  *p_data,
                                                          CPU_INT32U             data_len,
                                                          CPU_BOOLEAN            token_en,
                                                          HTTPs_ERR             *p_err);

void             HTTPs_RespBodySetParamNoBody     (const  HTTPs_INSTANCE        *p_instance,
                                                          HTTPs_CONN            *p_conn,
                                                          HTTPs_ERR             *p_err);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif                                                          /* End of HTTPs module include.                         */
