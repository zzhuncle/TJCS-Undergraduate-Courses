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
*                                                HTTP
*
* Filename : http.h
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
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  HTTP_MODULE_PRESENT                                   /* See Note #1.                                         */
#define  HTTP_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         HTTP INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <cpu_core.h>

#include  <lib_def.h>
#include  <lib_str.h>
#include  <lib_ascii.h>

#include  <FS/net_fs.h>
#ifdef  HTTPs_MODULE
#include  <http-s_cfg.h>
#endif
#include  <Source/net.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*********************************************************************************************************
*/

#if ((defined(HTTP_MODULE)) && \
     (defined(HTTP_GLOBALS_EXT)))
#define  HTTP_EXT
#else
#define  HTTP_EXT  extern
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
*                                       HTTP DEFAUT PORT DEFINES
*********************************************************************************************************
*/

#define  HTTP_DFLT_PORT_NBR                    80
#define  HTTP_DFLT_PORT_NBR_SECURE            443


/*
*********************************************************************************************************
*                                         URL ENCODING DEFINES
*********************************************************************************************************
*/

#define  HTTP_URL_ENCODING_LEN                  3
#define  HTTP_URL_ENCODING_JUMP                 2


/*
*********************************************************************************************************
*                                     HEXADECIMAL STRING DEFINE
*********************************************************************************************************
*/

#define  HTTP_INT_16U_HEX_STR_LEN_MAX         4u


/*
*********************************************************************************************************
*********************************************************************************************************
*                                              DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                     TASK CONFIGURATION DATA TYPE
*
* Note(s): (1) When the Stack pointer is defined as null pointer (DEF_NULL), the task's stack should be
*              automatically allowed on the heap of uC/LIB.
*********************************************************************************************************
*/

typedef  struct  http_task_cfg {
    CPU_INT32U   Prio;                                          /* Task priority.                                       */
    CPU_INT32U   StkSizeBytes;                                  /* Size of the stack.                                   */
    void        *StkPtr;                                        /* Pointer to base of the stack (see Note #1).          */
} HTTP_TASK_CFG;


/*
*********************************************************************************************************
*                                         METHODS ENUMARATION
*********************************************************************************************************
*/

typedef enum http_method {
    HTTP_METHOD_GET,
    HTTP_METHOD_POST,
    HTTP_METHOD_HEAD,
    HTTP_METHOD_PUT,
    HTTP_METHOD_DELETE,
    HTTP_METHOD_OPTIONS,
    HTTP_METHOD_TRACE,
    HTTP_METHOD_CONNECT,
    HTTP_METHOD_UNKNOWN
} HTTP_METHOD;


/*
*********************************************************************************************************
*                                      STATUS CODES ENUMARATION
*********************************************************************************************************
*/

typedef enum http_status_code {
    HTTP_STATUS_CONTINUE,
    HTTP_STATUS_SWITCHING_PROTOCOLS,
    HTTP_STATUS_PROCESSING,
    HTTP_STATUS_EARLY_HINTS,
    HTTP_STATUS_OK,
    HTTP_STATUS_CREATED,
    HTTP_STATUS_ACCEPTED,
    HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION,
    HTTP_STATUS_NO_CONTENT,
    HTTP_STATUS_RESET_CONTENT,
    HTTP_STATUS_PARTIAL_CONTENT,
    HTTP_STATUS_MULTI_STATUS,
    HTTP_STATUS_ALREADY_REPORTED,
    HTTP_STATUS_IM_USED,
    HTTP_STATUS_MULTIPLE_CHOICES,
    HTTP_STATUS_MOVED_PERMANENTLY,
    HTTP_STATUS_FOUND,
    HTTP_STATUS_SEE_OTHER,
    HTTP_STATUS_NOT_MODIFIED,
    HTTP_STATUS_USE_PROXY,
    HTTP_STATUS_SWITCH_PROXY,
    HTTP_STATUS_TEMPORARY_REDIRECT,
    HTTP_STATUS_PERMANENT_REDIRECT,
    HTTP_STATUS_BAD_REQUEST,
    HTTP_STATUS_UNAUTHORIZED,
    HTTP_STATUS_FORBIDDEN,
    HTTP_STATUS_NOT_FOUND,
    HTTP_STATUS_METHOD_NOT_ALLOWED,
    HTTP_STATUS_NOT_ACCEPTABLE,             /* With the Accept Req Hdr */
    HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED,
    HTTP_STATUS_REQUEST_TIMEOUT,
    HTTP_STATUS_CONFLICT,
    HTTP_STATUS_GONE,
    HTTP_STATUS_LENGTH_REQUIRED,
    HTTP_STATUS_PRECONDITION_FAILED,
    HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE,
    HTTP_STATUS_REQUEST_URI_TOO_LONG,
    HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE,
    HTTP_STATUS_REQUESTED_RANGE_NOT_SATISFIABLE,
    HTTP_STATUS_EXPECTATION_FAILED,
    HTTP_STATUS_MISDIRECTED_REQUEST,
    HTTP_STATUS_UNPROCESSABLE_ENTITY,
    HTTP_STATUS_LOCKED,
    HTTP_STATUS_FAILED_DEPENDENCY,
    HTTP_STATUS_UPGRADE_REQUIRED,
    HTTP_STATUS_PRECONDITION_REQUIRED,
    HTTP_STATUS_TOO_MANY_REQUESTS,
    HTTP_STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE,
    HTTP_STATUS_UNAVAILABLE_FOR_LEGAL_REASONS,
    HTTP_STATUS_INTERNAL_SERVER_ERR,
    HTTP_STATUS_NOT_IMPLEMENTED,
    HTTP_STATUS_BAD_GATEWAY,
    HTTP_STATUS_SERVICE_UNAVAILABLE,
    HTTP_STATUS_GATEWAY_TIMEOUT,
    HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED,
    HTTP_STATUS_VARIANT_ALSO_NEGOTIATES,
    HTTP_STATUS_INSUFFICIENT_STORAGE,
    HTTP_STATUS_LOOP_DETECTED,
    HTTP_STATUS_NOT_EXTENDED,
    HTTP_STATUS_NETWORK_AUTHENTICATION_REQUIRED,
    HTTP_STATUS_UNKNOWN
} HTTP_STATUS_CODE;


/*
*********************************************************************************************************
*                                     HTTPS HEADER ENUMARATION
*********************************************************************************************************
*/

typedef  enum http_hdr_field {
    HTTP_HDR_FIELD_UNKNOWN,
    HTTP_HDR_FIELD_CONTENT_TYPE,
    HTTP_HDR_FIELD_CONTENT_LEN,
    HTTP_HDR_FIELD_CONTENT_DISPOSITION,
    HTTP_HDR_FIELD_HOST,
    HTTP_HDR_FIELD_LOCATION,
    HTTP_HDR_FIELD_CONN,
    HTTP_HDR_FIELD_TRANSFER_ENCODING,
    HTTP_HDR_FIELD_ACCEPT,
    HTTP_HDR_FIELD_ACCEPT_CHARSET,
    HTTP_HDR_FIELD_ACCEPT_ENCODING,
    HTTP_HDR_FIELD_ACCEPT_LANGUAGE,
    HTTP_HDR_FIELD_ACCEPT_RANGES,
    HTTP_HDR_FIELD_AGE,
    HTTP_HDR_FIELD_ALLOW,
    HTTP_HDR_FIELD_AUTHORIZATION,
    HTTP_HDR_FIELD_CONTENT_ENCODING,
    HTTP_HDR_FIELD_CONTENT_LANGUAGE,
    HTTP_HDR_FIELD_CONTENT_LOCATION,
    HTTP_HDR_FIELD_CONTENT_MD5,
    HTTP_HDR_FIELD_CONTENT_RANGE,
    HTTP_HDR_FIELD_COOKIE,
    HTTP_HDR_FIELD_COOKIE2,
    HTTP_HDR_FIELD_DATE,
    HTTP_HDR_FIELD_ETAG,
    HTTP_HDR_FIELD_EXPECT,
    HTTP_HDR_FIELD_EXPIRES,
    HTTP_HDR_FIELD_FROM,
    HTTP_HDR_FIELD_IF_MODIFIED_SINCE,
    HTTP_HDR_FIELD_IF_MATCH,
    HTTP_HDR_FIELD_IF_NONE_MATCH,
    HTTP_HDR_FIELD_IF_RANGE,
    HTTP_HDR_FIELD_IF_UNMODIFIED_SINCE,
    HTTP_HDR_FIELD_LAST_MODIFIED,
    HTTP_HDR_FIELD_RANGE,
    HTTP_HDR_FIELD_REFERER,
    HTTP_HDR_FIELD_RETRY_AFTER,
    HTTP_HDR_FIELD_SERVER,
    HTTP_HDR_FIELD_SET_COOKIE,
    HTTP_HDR_FIELD_SET_COOKIE2,
    HTTP_HDR_FIELD_TE,
    HTTP_HDR_FIELD_TRAILER,
    HTTP_HDR_FIELD_UPGRADE,
    HTTP_HDR_FIELD_USER_AGENT,
    HTTP_HDR_FIELD_VARY,
    HTTP_HDR_FIELD_VIA,
    HTTP_HDR_FIELD_WARNING,
    HTTP_HDR_FIELD_WWW_AUTHENTICATE,
    HTTP_HDR_FIELD_WEBSOCKET_KEY,
    HTTP_HDR_FIELD_WEBSOCKET_ACCEPT,
    HTTP_HDR_FIELD_WEBSOCKET_VERSION,
    HTTP_HDR_FIELD_WEBSOCKET_PROTOCOL,
    HTTP_HDR_FIELD_WEBSOCKET_EXTENSIONS
} HTTP_HDR_FIELD;


/*
*********************************************************************************************************
*                                     HTTPS VERSIONS ENUMARATION
*********************************************************************************************************
*/

typedef  enum  http_protocol_ver {
    HTTP_PROTOCOL_VER_0_9,
    HTTP_PROTOCOL_VER_1_0,
    HTTP_PROTOCOL_VER_1_1,
    HTTP_PROTOCOL_VER_UNKNOWN
} HTTP_PROTOCOL_VER;


/*
*********************************************************************************************************
*                                      CONTENT TYPES ENUMARATION
*********************************************************************************************************
*/

typedef enum http_content_type {
    HTTP_CONTENT_TYPE_UNKNOWN,
    HTTP_CONTENT_TYPE_NONE,
    HTTP_CONTENT_TYPE_HTML,
    HTTP_CONTENT_TYPE_OCTET_STREAM,
    HTTP_CONTENT_TYPE_PDF,
    HTTP_CONTENT_TYPE_ZIP,
    HTTP_CONTENT_TYPE_GIF,
    HTTP_CONTENT_TYPE_JPEG,
    HTTP_CONTENT_TYPE_PNG,
    HTTP_CONTENT_TYPE_JS,
    HTTP_CONTENT_TYPE_PLAIN,
    HTTP_CONTENT_TYPE_CSS,
    HTTP_CONTENT_TYPE_JSON,
    HTTP_CONTENT_TYPE_APP_FORM,
    HTTP_CONTENT_TYPE_MULTIPART_FORM,
    HTTP_CONTENT_TYPE_WEBSOCK_TXT_DATA,
    HTTP_CONTENT_TYPE_WEBSOCK_BIN_DATA
} HTTP_CONTENT_TYPE;


/*
*********************************************************************************************************
*                             CONTENT DISPOSITION TYPE VALUES ENUMARATION
*********************************************************************************************************
*/

typedef enum http_content_disposition {
    HTTP_CONTENT_DISPOSITION_FORM_DATA
} HTTP_CONTENT_DISPOSITION;


/*
*********************************************************************************************************
*                             HEADER FIELD CONNECTION VALUES ENUMARATION
*********************************************************************************************************
*/

typedef enum http_hdr_field_conn_val {
    HTTP_HDR_FIELD_CONN_CLOSE,
    HTTP_HDR_FIELD_CONN_PERSISTENT,
    HTTP_HDR_FIELD_CONN_UPGRADE,
    HTTP_HDR_FIELD_CONN_UNKNOWN
} HTTP_HDR_FIELD_CONN_VAL;


/*
*********************************************************************************************************
*                             HEADER FIELD TRANSFER ENCODING VALUES ENUMARATION
*********************************************************************************************************
*/

typedef enum http_hdr_field_transfer_type {
    HTTP_HDR_FIELD_TRANSFER_TYPE_NONE,
    HTTP_HDR_FIELD_TRANSFER_TYPE_CHUNKED
} HTTP_HDR_FIELD_TRANSFER_TYPE;

/*
*********************************************************************************************************
*                                  HEADER FIELD UPGRADE VALUES ENUMARATION
*********************************************************************************************************
*/

typedef enum http_hdr_field_upgrade_val {
    HTTP_HDR_FIELD_UPGRADE_WEBSOCKET
} HTTP_HDR_FIELD_UPGRADE_VAL;


/*
*********************************************************************************************************
*                                       FILE TYPES ENUMARATION
*********************************************************************************************************
*/

typedef  enum http_file_type {
    HTTP_FILE_TYPE_FS,
    HTTP_FILE_TYPE_STATIC_DATA
} HTTP_FILE_TYPE;


/*
*********************************************************************************************************
*                                       BODY TYPES ENUMARATION
*********************************************************************************************************
*/

typedef  enum http_body_type {
    HTTP_BODY_TYPE_FS,
    HTTP_BODY_TYPE_DATA,
    HTTP_BODY_TYPE_BUF
} HTTP_BODY_TYPE;


/*
*********************************************************************************************************
*                                     FORM DATA TYPES ENUMARATION
*********************************************************************************************************
*/

typedef enum  http_form_data_type {
    HTTP_FORM_DATA_TYPE_CTRL_VAL_QUERY,
    HTTP_FORM_DATA_TYPE_CTRL_VAL_FORM,
    HTTP_FORM_DATA_TYPE_FILE
} HTTP_FORM_DATA_TYPE;


/*
*********************************************************************************************************
*                                            FORM DATA TYPE
*********************************************************************************************************
*/

typedef  struct  http_form_data  HTTP_FORM_DATA;

struct  http_form_data {
    HTTP_FORM_DATA_TYPE   DataType;
    CPU_CHAR             *CtrlNamePtr;
    CPU_INT16U            CtrlNameLen;
    CPU_CHAR             *ValPtr;
    CPU_INT16U            ValLen;
    HTTP_FORM_DATA       *DataNextPtr;
};


/*
*********************************************************************************************************
*                                 HTTP HDR FIELD VALUE TYPE DATA TYPE
*********************************************************************************************************
*/

typedef  enum  http_hdr_val_type {
    HTTP_HDR_VAL_TYPE_NONE,
    HTTP_HDR_VAL_TYPE_STR_CONST,
    HTTP_HDR_VAL_TYPE_STR_DYN
} HTTP_HDR_VAL_TYPE;


/*
*********************************************************************************************************
*                                     HTTP HDR FIELD BLK DATA TYPE
*********************************************************************************************************
*/

typedef  struct  http_hdr_blk  HTTP_HDR_BLK;

struct  http_hdr_blk {
    HTTP_HDR_FIELD      HdrField;
    HTTP_HDR_VAL_TYPE   ValType;
    void               *ValPtr;
    CPU_INT32U          ValLen;
    HTTP_HDR_BLK       *NextPtr;
    HTTP_HDR_BLK       *PrevPtr;
};


/*
*********************************************************************************************************
*                              HTTP FORM MULTIPART CONTENT FIELD DATA TYPE
*********************************************************************************************************
*/

typedef enum http_multipart_field {
    HTTP_MULTIPART_FIELD_NAME,
    HTTP_MULTIPART_FIELD_FILE_NAME,
    HTTP_MULTIPART_FIELD_UNKNOWN
} HTTP_MULTIPART_FIELD;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                                MACROS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        HTTP COUNTER MACRO'S
*
* Description : Functionality to set and increment statistic and error counter
*
* Argument(s) : Various HTTP counter variable(s) & values.
*
* Return(s)   : none.
*
* Caller(s)   : various.
*
*               These macro's are INTERNAL HTTP suite macro's & SHOULD NOT be called by
*               application function(s).
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if ((HTTPs_CFG_CTR_STAT_EN == DEF_ENABLED) || \
     (HTTPc_CFG_CTR_STAT_EN == DEF_ENABLED))
#define  HTTP_SET_PTR_STATS(p_ctr_stats, p_instance)   {                                           \
                                                           p_ctr_stats = &p_instance->StatsCtr;    \
                                                       }

#define  HTTP_STATS_INC(p_ctr)                         {                                           \
                                                            p_ctr++;                               \
                                                       }

#define  HTTP_STATS_OCTET_INC(p_ctr, octet)            {                                           \
                                                            p_ctr += octet;                        \
                                                       }

#else
                                                                /* Prevent 'variable unused' compiler warning.          */
#define  HTTP_SET_PTR_STATS(p_ctr_stats, p_instance)   {                                           \
                                                          (void)p_ctr_stats;                       \
                                                       }

#define  HTTP_STATS_INC(p_ctr)

#define  HTTP_STATS_OCTET_INC(p_ctr, octet)

#endif



#if ((HTTPs_CFG_CTR_ERR_EN == DEF_ENABLED) || \
     (HTTPc_CFG_CTR_ERR_EN == DEF_ENABLED))

#define  HTTP_SET_PTR_ERRS(p_ctr_err, p_instance)      {                                           \
                                                            p_ctr_err = &p_instance->ErrsCtr;      \
                                                       }

#define  HTTP_ERR_INC(p_ctr)                           {                                           \
                                                            p_ctr++;                               \
                                                       }

#else
                                                                 /* Prevent 'variable unused' compiler warning.          */
#define  HTTP_SET_PTR_ERRS(p_ctr_err, p_instance)      {                                           \
                                                          (void)p_ctr_err;                         \
                                                       }

#define  HTTP_ERR_INC(p_ctr)

#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

CPU_CHAR           *HTTP_HdrParseFieldValueGet     (       CPU_CHAR          *p_field,
                                                           CPU_INT16U         field_len,
                                                           CPU_CHAR          *p_field_end,
                                                           CPU_INT16U        *p_len_rem);

CPU_CHAR           *HTTP_StrGraphSrchFirst         (       CPU_CHAR          *p_str,
                                                           CPU_INT32U         str_len);

CPU_BOOLEAN         HTTP_URL_EncodeStr             (const  CPU_CHAR          *p_str_src,
                                                           CPU_CHAR          *p_str_dest,
                                                           CPU_SIZE_T        *p_str_len,
                                                           CPU_SIZE_T         str_len_max);

CPU_BOOLEAN         HTTP_URL_DecodeStr             (const  CPU_CHAR          *p_str_src,
                                                           CPU_CHAR          *p_str_dest,
                                                           CPU_SIZE_T        *p_str_lenn);

CPU_BOOLEAN         HTTP_URL_DecodeReplaceStr      (       CPU_CHAR          *p_str,
                                                           CPU_INT16U        *p_str_len);

CPU_INT16U          HTTP_URL_CharEncodeNbr         (const  CPU_CHAR          *p_str_src,
                                                           CPU_SIZE_T         str_len);

CPU_CHAR           *HTTP_ChunkTransferWrSize       (       CPU_CHAR          *p_buf_wr,
                                                           CPU_SIZE_T         buf_len,
                                                           CPU_SIZE_T         nbr_dig_max,
                                                           CPU_INT16U         size);

CPU_INT08U          HTTP_StrSizeHexDigReq          (       CPU_INT32U         nbr);

HTTP_CONTENT_TYPE   HTTP_GetContentTypeFromFileExt (       CPU_CHAR          *p_file_path,
                                                           CPU_SIZE_T         path_len_max);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif  /* HTTP_MODULE_PRESENT  */
