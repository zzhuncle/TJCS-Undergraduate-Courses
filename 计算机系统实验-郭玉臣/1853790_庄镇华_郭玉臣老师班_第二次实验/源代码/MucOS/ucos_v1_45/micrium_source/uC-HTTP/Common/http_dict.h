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
*                                           HTTP DICTIONARY
*
* Filename : http_dict.h
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
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  HTTP_DICT_MODULE_PRESENT                               /* See Note #1.                                         */
#define  HTTP_DICT_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         HTTP INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  "http.h"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                              DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             HTTP METHOD
*********************************************************************************************************
*/

#define  HTTP_STR_METHOD_GET                               "GET"
#define  HTTP_STR_METHOD_POST                              "POST"
#define  HTTP_STR_METHOD_HEAD                              "HEAD"
#define  HTTP_STR_METHOD_PUT                               "PUT"
#define  HTTP_STR_METHOD_DELETE                            "DELETE"
#define  HTTP_STR_METHOD_TRACE                             "TRACE"
#define  HTTP_STR_METHOD_CONNECT                           "CONNECT"


/*
*********************************************************************************************************
*                                            HTTP VERSION
*********************************************************************************************************
*/

#define  HTTP_STR_PROTOCOL_VER_0_9                     "HTTP/0.9"
#define  HTTP_STR_PROTOCOL_VER_1_0                     "HTTP/1.0"
#define  HTTP_STR_PROTOCOL_VER_1_1                     "HTTP/1.1"


/*
*********************************************************************************************************
*                                           HTTP STATUS CODE
*********************************************************************************************************
*/

#define  HTTP_STR_STATUS_CODE_CONTINUE                           "100"
#define  HTTP_STR_STATUS_CODE_SWITCHING_PROTOCOLS                "101"
#define  HTTP_STR_STATUS_CODE_PROCESSING                         "102"
#define  HTTP_STR_STATUS_CODE_EARLY_HINTS                        "103"
#define  HTTP_STR_STATUS_CODE_OK                                 "200"
#define  HTTP_STR_STATUS_CODE_CREATED                            "201"
#define  HTTP_STR_STATUS_CODE_ACCEPTED                           "202"
#define  HTTP_STR_STATUS_CODE_NON_AUTHORITATIVE_INFORMATION      "203"
#define  HTTP_STR_STATUS_CODE_NO_CONTENT                         "204"
#define  HTTP_STR_STATUS_CODE_RESET_CONTENT                      "205"
#define  HTTP_STR_STATUS_CODE_PARTIAL_CONTENT                    "206"
#define  HTTP_STR_STATUS_CODE_MULTI_STATUS                       "207"
#define  HTTP_STR_STATUS_CODE_ALREADY_REPORTED                   "208"
#define  HTTP_STR_STATUS_CODE_IM_USED                            "226"
#define  HTTP_STR_STATUS_CODE_MULTIPLE_CHOICES                   "300"
#define  HTTP_STR_STATUS_CODE_MOVED_PERMANENTLY                  "301"
#define  HTTP_STR_STATUS_CODE_FOUND                              "302"
#define  HTTP_STR_STATUS_CODE_SEE_OTHER                          "303"
#define  HTTP_STR_STATUS_CODE_NOT_MODIFIED                       "304"
#define  HTTP_STR_STATUS_CODE_USE_PROXY                          "305"
#define  HTTP_STR_STATUS_CODE_SWITCH_PROXY                       "306"
#define  HTTP_STR_STATUS_CODE_TEMPORARY_REDIRECT                 "307"
#define  HTTP_STR_STATUS_CODE_PERMANENT_REDIRECT                 "308"
#define  HTTP_STR_STATUS_CODE_BAD_REQUEST                        "400"
#define  HTTP_STR_STATUS_CODE_UNAUTHORIZED                       "401"
#define  HTTP_STR_STATUS_CODE_FORBIDDEN                          "403"
#define  HTTP_STR_STATUS_CODE_NOT_FOUND                          "404"
#define  HTTP_STR_STATUS_CODE_METHOD_NOT_ALLOWED                 "405"
#define  HTTP_STR_STATUS_CODE_NOT_ACCEPTABLE                     "406"
#define  HTTP_STR_STATUS_CODE_PROXY_AUTHENTICATION_REQUIRED      "407"
#define  HTTP_STR_STATUS_CODE_REQUEST_TIMEOUT                    "408"
#define  HTTP_STR_STATUS_CODE_CONFLICT                           "409"
#define  HTTP_STR_STATUS_CODE_GONE                               "410"
#define  HTTP_STR_STATUS_CODE_LENGTH_REQUIRED                    "411"
#define  HTTP_STR_STATUS_CODE_PRECONDITION_FAILED                "412"
#define  HTTP_STR_STATUS_CODE_REQUEST_ENTITY_TOO_LARGE           "413"
#define  HTTP_STR_STATUS_CODE_REQUEST_URI_TOO_LONG               "414"
#define  HTTP_STR_STATUS_CODE_UNSUPPORTED_MEDIA_TYPE             "415"
#define  HTTP_STR_STATUS_CODE_REQUESTED_RANGE_NOT_SATISFIABLE    "416"
#define  HTTP_STR_STATUS_CODE_EXPECTATION_FAILED                 "417"
#define  HTTP_STR_STATUS_CODE_MISDIRECTED_REQUEST                "421"
#define  HTTP_STR_STATUS_CODE_UNPROCESSABLE_ENTITY               "422"
#define  HTTP_STR_STATUS_CODE_LOCKED                             "423"
#define  HTTP_STR_STATUS_CODE_FAILED_DEPENDENCY                  "424"
#define  HTTP_STR_STATUS_CODE_UPGRADE_REQUIRED                   "426"
#define  HTTP_STR_STATUS_CODE_PRECONDITION_REQUIRED              "428"
#define  HTTP_STR_STATUS_CODE_TOO_MANY_REQUESTS                  "429"
#define  HTTP_STR_STATUS_CODE_REQUEST_HEADER_FIELDS_TOO_LARGE    "431"
#define  HTTP_STR_STATUS_CODE_UNAVAILABLE_FOR_LEGAL_REASONS      "451"
#define  HTTP_STR_STATUS_CODE_INTERNAL_SERVER_ERR                "500"
#define  HTTP_STR_STATUS_CODE_NOT_IMPLEMENTED                    "501"
#define  HTTP_STR_STATUS_CODE_BAD_GATEWAY                        "502"
#define  HTTP_STR_STATUS_CODE_SERVICE_UNAVAILABLE                "503"
#define  HTTP_STR_STATUS_CODE_GATEWAY_TIMEOUT                    "504"
#define  HTTP_STR_STATUS_CODE_HTTP_VERSION_NOT_SUPPORTED         "505"
#define  HTTP_STR_STATUS_CODE_VARIANT_ALSO_NEGOTIATES            "506"
#define  HTTP_STR_STATUS_CODE_INSUFFICIENT_STORAGE               "507"
#define  HTTP_STR_STATUS_CODE_LOOP_DETECTED                      "508"
#define  HTTP_STR_STATUS_CODE_NOT_EXTENDED                       "510"
#define  HTTP_STR_STATUS_CODE_NETWORK_AUTHENTICATION_REQUIRED    "511"


/*
*********************************************************************************************************
*                                          HTTP REASON PHRASE
*********************************************************************************************************
*/

#define  HTTP_STR_REASON_PHRASE_CONTINUE                         "Continue"
#define  HTTP_STR_REASON_PHRASE_SWITCHING_PROTOCOLS              "Switching Protocols"
#define  HTTP_STR_REASON_PHRASE_PROCESSING                       "Processing"
#define  HTTP_STR_REASON_PHRASE_EARLY_HINTS                      "Early Hints"
#define  HTTP_STR_REASON_PHRASE_OK                               "OK"
#define  HTTP_STR_REASON_PHRASE_CREATED                          "Created"
#define  HTTP_STR_REASON_PHRASE_ACCEPTED                         "Accepted"
#define  HTTP_STR_REASON_PHRASE_NON_AUTHORITATIVE_INFORMATION    "No Authoritative Information"
#define  HTTP_STR_REASON_PHRASE_NO_CONTENT                       "No Content"
#define  HTTP_STR_REASON_PHRASE_RESET_CONTENT                    "Reset Content"
#define  HTTP_STR_REASON_PHRASE_PARTIAL_CONTENT                  "Partial Content"
#define  HTTP_STR_REASON_PHRASE_MULTI_STATUS                     "Multi Status"
#define  HTTP_STR_REASON_PHRASE_ALREADY_REPORTED                 "Already Reported"
#define  HTTP_STR_REASON_PHRASE_IM_USED                          "IM Used"
#define  HTTP_STR_REASON_PHRASE_MULTIPLE_CHOICES                 "Multiple Choices"
#define  HTTP_STR_REASON_PHRASE_MOVED_PERMANENTLY                "Moved Permanently"
#define  HTTP_STR_REASON_PHRASE_FOUND                            "Found"
#define  HTTP_STR_REASON_PHRASE_SEE_OTHER                        "See Other"
#define  HTTP_STR_REASON_PHRASE_NOT_MODIFIED                     "Not Modified"
#define  HTTP_STR_REASON_PHRASE_USE_PROXY                        "Use Proxy"
#define  HTTP_STR_REASON_PHRASE_SWITCH_PROXY                     "Switch Proxy"
#define  HTTP_STR_REASON_PHRASE_TEMPORARY_REDIRECT               "Temporary Redirect"
#define  HTTP_STR_REASON_PHRASE_PERMANENT_REDIRECT               "Permanent Redirect"
#define  HTTP_STR_REASON_PHRASE_BAD_REQUEST                      "Bad Request"
#define  HTTP_STR_REASON_PHRASE_UNAUTHORIZED                     "Unauthorized"
#define  HTTP_STR_REASON_PHRASE_FORBIDDEN                        "Forbidden"
#define  HTTP_STR_REASON_PHRASE_NOT_FOUND                        "Not Found"
#define  HTTP_STR_REASON_PHRASE_METHOD_NOT_ALLOWED               "Method Not Allowed"
#define  HTTP_STR_REASON_PHRASE_NOT_ACCEPTABLE                   "Not Acceptable"
#define  HTTP_STR_REASON_PHRASE_PROXY_AUTHENTICATION_REQUIRED    "Proxy Authentication Required"
#define  HTTP_STR_REASON_PHRASE_REQUEST_TIMEOUT                  "Request Timeout"
#define  HTTP_STR_REASON_PHRASE_CONFLICT                         "Conflict"
#define  HTTP_STR_REASON_PHRASE_GONE                             "Gone"
#define  HTTP_STR_REASON_PHRASE_LENGTH_REQUIRED                  "Length Required"
#define  HTTP_STR_REASON_PHRASE_PRECONDITION_FAILED              "Precondition Failed"
#define  HTTP_STR_REASON_PHRASE_REQUEST_ENTITY_TOO_LARGE         "Request Entity Too Large"
#define  HTTP_STR_REASON_PHRASE_REQUEST_URI_TOO_LONG             "Request URI Too Long"
#define  HTTP_STR_REASON_PHRASE_UNSUPPORTED_MEDIA_TYPE           "Unsupported Media Type"
#define  HTTP_STR_REASON_PHRASE_REQUESTED_RANGE_NOT_SATISFIABLE  "Requested Range Not Satisfiable"
#define  HTTP_STR_REASON_PHRASE_EXPECTATION_FAILED               "Expectation Failed"
#define  HTTP_STR_REASON_PHRASE_MISDIRECTED_REQUEST              "Misdirected Request"
#define  HTTP_STR_REASON_PHRASE_UNPROCESSABLE_ENTITY             "Unprocessable Entity"
#define  HTTP_STR_REASON_PHRASE_LOCKED                           "Locked"
#define  HTTP_STR_REASON_PHRASE_FAILED_DEPENDENCY                "Failed Dependency"
#define  HTTP_STR_REASON_PHRASE_UPGRADE_REQUIRED                 "Upgrade Required"
#define  HTTP_STR_REASON_PHRASE_PRECONDITION_REQUIRED            "Precondition Required"
#define  HTTP_STR_REASON_PHRASE_TOO_MANY_REQUESTS                "Too Many Requetsts"
#define  HTTP_STR_REASON_PHRASE_REQUEST_HEADER_FIELDS_TOO_LARGE  "Request Header Fields Too Large"
#define  HTTP_STR_REASON_PHRASE_UNAVAILABLE_FOR_LEGAL_REASONS    "Unavailable For Legal Reasons"
#define  HTTP_STR_REASON_PHRASE_INTERNAL_SERVER_ERR              "Internal Server Error"
#define  HTTP_STR_REASON_PHRASE_NOT_IMPLEMENTED                  "Not Implemented"
#define  HTTP_STR_REASON_PHRASE_BAD_GATEWAY                      "Bad Gateway"
#define  HTTP_STR_REASON_PHRASE_SERVICE_UNAVAILABLE              "Service Unavailable"
#define  HTTP_STR_REASON_PHRASE_GATEWAY_TIMEOUT                  "Gateway Timeout"
#define  HTTP_STR_REASON_PHRASE_HTTP_VERSION_NOT_SUPPORTED       "HTTP Version Not Supported"
#define  HTTP_STR_REASON_PHRASE_VARIANT_ALSO_NEGOTIATES          "Variant Also Negotiates"
#define  HTTP_STR_REASON_PHRASE_INSUFFICIENT_STORAGE             "Insufficient Storage"
#define  HTTP_STR_REASON_PHRASE_LOOP_DETECTED                    "Loop Detected"
#define  HTTP_STR_REASON_PHRASE_NOT_EXTENDED                     "Not Extended"
#define  HTTP_STR_REASON_PHRASE_NETWORK_AUTHENTICATION_REQUIRED  "Network Authentication Required"


/*
*********************************************************************************************************
*                                          HTTP FILE EXTENSION
*********************************************************************************************************
*/

#define  HTTP_STR_FILE_EXT_HTM                             "htm"
#define  HTTP_STR_FILE_EXT_HTML                            "html"
#define  HTTP_STR_FILE_EXT_GIF                             "gif"
#define  HTTP_STR_FILE_EXT_JPEG                            "jpeg"
#define  HTTP_STR_FILE_EXT_JPG                             "jpg"
#define  HTTP_STR_FILE_EXT_PNG                             "png"
#define  HTTP_STR_FILE_EXT_JS                              "js"
#define  HTTP_STR_FILE_EXT_TXT                             "txt"
#define  HTTP_STR_FILE_EXT_CSS                             "css"
#define  HTTP_STR_FILE_EXT_PDF                             "pdf"
#define  HTTP_STR_FILE_EXT_ZIP                             "zip"
#define  HTTP_STR_FILE_EXT_ASTERISK                        "*"
#define  HTTP_STR_FILE_EXT_CLASS                           "class"


/*
*********************************************************************************************************
*                                         HTTP MIME CONTENT TYPE
*********************************************************************************************************
*/

#define  HTTP_STR_CONTENT_TYPE_HTML                        "text/html"
#define  HTTP_STR_CONTENT_TYPE_GIF                         "image/gif"
#define  HTTP_STR_CONTENT_TYPE_JPEG                        "image/jpeg"
#define  HTTP_STR_CONTENT_TYPE_PNG                         "image/png"
#define  HTTP_STR_CONTENT_TYPE_JS                          "text/javascript"
#define  HTTP_STR_CONTENT_TYPE_PLAIN                       "text/plain"
#define  HTTP_STR_CONTENT_TYPE_CSS                         "text/css"
#define  HTTP_STR_CONTENT_TYPE_OCTET_STREAM                "application/octet-stream"
#define  HTTP_STR_CONTENT_TYPE_PDF                         "application/pdf"
#define  HTTP_STR_CONTENT_TYPE_ZIP                         "application/zip"
#define  HTTP_STR_CONTENT_TYPE_JSON                        "application/json"
#define  HTTP_STR_CONTENT_TYPE_APP_FORM                    "application/x-www-form-urlencoded"
#define  HTTP_STR_CONTENT_TYPE_MULTIPART_FORM              "multipart/form-data"


/*
*********************************************************************************************************
*                                          MULTIPART BOUNDARY
*********************************************************************************************************
*/

#define  HTTP_STR_MULTIPART_BOUNDARY                    "boundary"
#define  HTTP_STR_MULTIPART_BOUNDARY_LEN                (sizeof(HTTP_STR_MULTIPART_BOUNDARY) - 1)


/*
*********************************************************************************************************
*                                          HTTP HEADER FIELD
*********************************************************************************************************
*/

#define  HTTP_STR_HDR_FIELD_CONN                           "Connection"
#define  HTTP_STR_HDR_FIELD_HOST                           "Host"
#define  HTTP_STR_HDR_FIELD_LOCATION                       "Location"
#define  HTTP_STR_HDR_FIELD_CONTENT_TYPE                   "Content-Type"
#define  HTTP_STR_HDR_FIELD_CONTENT_LEN                    "Content-Length"
#define  HTTP_STR_HDR_FIELD_CONTENT_DISPOSITION            "Content-Disposition"
#define  HTTP_STR_HDR_FIELD_TRANSFER_ENCODING              "Transfer-Encoding"
#define  HTTP_STR_HDR_FIELD_ACCEPT                         "Accept"
#define  HTTP_STR_HDR_FIELD_ACCEPT_CHARSET                 "Accept-Charset"
#define  HTTP_STR_HDR_FIELD_ACCEPT_ENCODING                "Accept-Encoding"
#define  HTTP_STR_HDR_FIELD_ACCEPT_LANGUAGE                "Accept-Language"
#define  HTTP_STR_HDR_FIELD_ACCEPT_RANGES                  "Accept-Ranges"
#define  HTTP_STR_HDR_FIELD_AGE                            "Age"
#define  HTTP_STR_HDR_FIELD_ALLOW                          "Allow"
#define  HTTP_STR_HDR_FIELD_AUTHORIZATION                  "Authorization"
#define  HTTP_STR_HDR_FIELD_CONTENT_ENCODING               "Content-Encoding"
#define  HTTP_STR_HDR_FIELD_CONTENT_LANGUAGE               "Content-Language"
#define  HTTP_STR_HDR_FIELD_CONTENT_LOCATION               "Content-Location"
#define  HTTP_STR_HDR_FIELD_CONTENT_MD5                    "Content-MD5"
#define  HTTP_STR_HDR_FIELD_CONTENT_RANGE                  "Content-Range"
#define  HTTP_STR_HDR_FIELD_COOKIE                         "Cookie"
#define  HTTP_STR_HDR_FIELD_COOKIE2                        "Cookie2"
#define  HTTP_STR_HDR_FIELD_DATE                           "Date"
#define  HTTP_STR_HDR_FIELD_ETAG                           "ETag"
#define  HTTP_STR_HDR_FIELD_EXPECT                         "Expect"
#define  HTTP_STR_HDR_FIELD_EXPIRES                        "Expires"
#define  HTTP_STR_HDR_FIELD_FROM                           "From"
#define  HTTP_STR_HDR_FIELD_IF_MODIFIED_SINCE              "If-Modified-Since"
#define  HTTP_STR_HDR_FIELD_IF_MATCH                       "If-Match"
#define  HTTP_STR_HDR_FIELD_IF_NONE_MATCH                  "If-None-Match"
#define  HTTP_STR_HDR_FIELD_IF_RANGE                       "If-Range"
#define  HTTP_STR_HDR_FIELD_IF_UNMODIFIED_SINCE            "If-Unmodified-Since"
#define  HTTP_STR_HDR_FIELD_LAST_MODIFIED                  "Last-Modified"
#define  HTTP_STR_HDR_FIELD_RANGE                          "Range"
#define  HTTP_STR_HDR_FIELD_REFERER                        "Referer"
#define  HTTP_STR_HDR_FIELD_RETRY_AFTER                    "Retry-After"
#define  HTTP_STR_HDR_FIELD_SERVER                         "Server"
#define  HTTP_STR_HDR_FIELD_SET_COOKIE                     "Set-Cookie"
#define  HTTP_STR_HDR_FIELD_SET_COOKIE2                    "Set-Cookie2"
#define  HTTP_STR_HDR_FIELD_TE                             "TE"
#define  HTTP_STR_HDR_FIELD_TRAILER                        "Trailer"
#define  HTTP_STR_HDR_FIELD_UPGRADE                        "Upgrade"
#define  HTTP_STR_HDR_FIELD_USER_AGENT                     "User-Agent"
#define  HTTP_STR_HDR_FIELD_VARY                           "Vary"
#define  HTTP_STR_HDR_FIELD_VIA                            "Via"
#define  HTTP_STR_HDR_FIELD_WARNING                        "Warning"
#define  HTTP_STR_HDR_FIELD_WWW_AUTHENTICATE               "WWW-Authenticate"
#define  HTTP_STR_HDR_FIELD_WEBSOCKET_KEY                  "Sec-WebSocket-Key"
#define  HTTP_STR_HDR_FIELD_WEBSOCKET_ACCEPT               "Sec-WebSocket-Accept"
#define  HTTP_STR_HDR_FIELD_WEBSOCKET_VERSION              "Sec-WebSocket-Version"
#define  HTTP_STR_HDR_FIELD_WEBSOCKET_PROTOCOL             "Sec-WebSocket-Protocol"
#define  HTTP_STR_HDR_FIELD_WEBSOCKET_EXTENSIONS           "Sec-WebSocket-Extensions"

                                                                /* Len of Hdr Field string names in Request messages     */
#define  HTTP_STR_HDR_FIELD_CONN_LEN                       (sizeof(HTTP_STR_HDR_FIELD_CONN)                - 1)
#define  HTTP_STR_HDR_FIELD_HOST_LEN                       (sizeof(HTTP_STR_HDR_FIELD_HOST)                - 1)
#define  HTTP_STR_HDR_FIELD_LOCATION_LEN                   (sizeof(HTTP_STR_HDR_FIELD_LOCATION)            - 1)
#define  HTTP_STR_HDR_FIELD_CONTENT_TYPE_LEN               (sizeof(HTTP_STR_HDR_FIELD_CONTENT_TYPE)        - 1)
#define  HTTP_STR_HDR_FIELD_CONTENT_LEN_LEN                (sizeof(HTTP_STR_HDR_FIELD_CONTENT_LEN)         - 1)
#define  HTTP_STR_HDR_FIELD_CONTENT_DISPOSITION_LEN        (sizeof(HTTP_STR_HDR_FIELD_CONTENT_DISPOSITION) - 1)
#define  HTTP_STR_HDR_FIELD_TRANSFER_ENCODING_LEN          (sizeof(HTTP_STR_HDR_FIELD_TRANSFER_ENCODING)   - 1)
#define  HTTP_STR_HDR_FIELD_ACCEPT_LEN                     (sizeof(HTTP_STR_HDR_FIELD_ACCEPT)              - 1)
#define  HTTP_STR_HDR_FIELD_ACCEPT_CHARSET_LEN             (sizeof(HTTP_STR_HDR_FIELD_ACCEPT_CHARSET)      - 1)
#define  HTTP_STR_HDR_FIELD_ACCEPT_ENCODING_LEN            (sizeof(HTTP_STR_HDR_FIELD_ACCEPT_ENCODING)     - 1)
#define  HTTP_STR_HDR_FIELD_ACCEPT_LANGUAGE_LEN            (sizeof(HTTP_STR_HDR_FIELD_ACCEPT_LANGUAGE)     - 1)
#define  HTTP_STR_HDR_FIELD_AUTHORIZATION_LEN              (sizeof(HTTP_STR_HDR_FIELD_AUTHORIZATION)       - 1)
#define  HTTP_STR_HDR_FIELD_CLIENT_IP_LEN                  (sizeof(HTTP_STR_HDR_FIELD_CLIENT_IP)           - 1)
#define  HTTP_STR_HDR_FIELD_COOKIE_LEN                     (sizeof(HTTP_STR_HDR_FIELD_COOKIE)              - 1)
#define  HTTP_STR_HDR_FIELD_COOKIE2_LEN                    (sizeof(HTTP_STR_HDR_FIELD_COOKIE2)             - 1)
#define  HTTP_STR_HDR_FIELD_DATE_LEN                       (sizeof(HTTP_STR_HDR_FIELD_DATE)                - 1)
#define  HTTP_STR_HDR_FIELD_EXPECT_LEN                     (sizeof(HTTP_STR_HDR_FIELD_EXPECT)              - 1)
#define  HTTP_STR_HDR_FIELD_FROM_LEN                       (sizeof(HTTP_STR_HDR_FIELD_FROM)                - 1)
#define  HTTP_STR_HDR_FIELD_IF_MODIFIED_SINCE_LEN          (sizeof(HTTP_STR_HDR_FIELD_IF_MODIFIED_SINCE)   - 1)
#define  HTTP_STR_HDR_FIELD_IF_MATCH_LEN                   (sizeof(HTTP_STR_HDR_FIELD_IF_MATCH)            - 1)
#define  HTTP_STR_HDR_FIELD_IF_NONE_MATCH_LEN              (sizeof(HTTP_STR_HDR_FIELD_IF_NONE_MATCH)       - 1)
#define  HTTP_STR_HDR_FIELD_IF_RANGE_LEN                   (sizeof(HTTP_STR_HDR_FIELD_IF_RANGE)            - 1)
#define  HTTP_STR_HDR_FIELD_IF_UNMODIFIED_SINCE_LEN        (sizeof(HTTP_STR_HDR_FIELD_IF_UNMODIFIED_SINCE) - 1)
#define  HTTP_STR_HDR_FIELD_RANGE_LEN                      (sizeof(HTTP_STR_HDR_FIELD_RANGE)               - 1)
#define  HTTP_STR_HDR_FIELD_REFERER_LEN                    (sizeof(HTTP_STR_HDR_FIELD_REFERER)             - 1)
#define  HTTP_STR_HDR_FIELD_TE_LEN                         (sizeof(HTTP_STR_HDR_FIELD_TE)                  - 1)
#define  HTTP_STR_HDR_FIELD_UPGRADE_LEN                    (sizeof(HTTP_STR_HDR_FIELD_UPGRADE)             - 1)
#define  HTTP_STR_HDR_FIELD_USER_AGENT_LEN                 (sizeof(HTTP_STR_HDR_FIELD_USER_AGENT)          - 1)
#define  HTTP_STR_HDR_FIELD_VIA_LEN                        (sizeof(HTTP_STR_HDR_FIELD_VIA)                 - 1)
#define  HTTP_STR_HDR_FIELD_WARNING_LEN                    (sizeof(HTTP_STR_HDR_FIELD_WARNING)             - 1)
#define  HTTP_STR_HDR_FIELD_WWW_AUTHENTICATE_LEN           (sizeof(HTTP_STR_HDR_FIELD_WWW_AUTHENTICATE)    - 1)
#define  HTTP_STR_HDR_FIELD_WEBSOCKET_KEY_LEN              (sizeof(HTTP_STR_HDR_FIELD_WEBSOCKET_KEY)       - 1)
#define  HTTP_STR_HDR_FIELD_WEBSOCKET_ACCEPT_LEN           (sizeof(HTTP_STR_HDR_FIELD_WEBSOCKET_ACCEPT)    - 1)
#define  HTTP_STR_HDR_FIELD_WEBSOCKET_VERSION_LEN          (sizeof(HTTP_STR_HDR_FIELD_WEBSOCKET_VERSION)   - 1)
#define  HTTP_STR_HDR_FIELD_WEBSOCKET_PROTOCOL_LEN         (sizeof(HTTP_STR_HDR_FIELD_WEBSOCKET_PROTOCOL)  - 1)
#define  HTTP_STR_HDR_FIELD_WEBSOCKET_EXTENSIONS_LEN       (sizeof(HTTP_STR_HDR_FIELD_WEBSOCKET_EXTENSIONS)- 1)


/*
*********************************************************************************************************
*                                      HTTP CONTENT DISPOSITION VALUE
*********************************************************************************************************
*/

#define  HTTP_STR_CONTENT_DISPOSITION_FORM_DATA        "form-data"
#define  HTTP_STR_CONTENT_DISPOSITION_FORM_DATA_LEN     (sizeof(HTTP_STR_CONTENT_DISPOSITION_FORM_DATA) - 1)


/*
*********************************************************************************************************
*                                        HTTP CONNECTION VALUE
*********************************************************************************************************
*/

#define  HTTP_STR_HDR_FIELD_CONN_CLOSE                 "close"
#define  HTTP_STR_HDR_FIELD_CONN_KEEP_ALIVE            "keep-alive"
#define  HTTP_STR_HDR_FIELD_CONN_UPGRADE               "Upgrade"


/*
*********************************************************************************************************
*                                   HTTP TRANSFER ENCODING VALUE
*********************************************************************************************************
*/

#define  HTTP_STR_HDR_FIELD_CHUNKED                     "chunked"
#define  HTTP_STR_BUF_TOP_SPACE_REQ_MIN                        6
#define  HTTP_STR_BUF_END_SPACE_REQ_MIN                        2


/*
*********************************************************************************************************
*                                  HTTP FORM MULTIPART CONTENT FIELD
*********************************************************************************************************
*/

#define  HTTP_STR_MULTIPART_FIELD_NAME                     "name"
#define  HTTP_STR_MULTIPART_FIELD_FILE_NAME                "filename"

#define  HTTP_STR_MULTIPART_FIELD_NAME_LEN                  (sizeof(HTTP_STR_MULTIPART_FIELD_NAME)      - 1)
#define  HTTP_STR_MULTIPART_FIELD_FILE_NAME_LEN             (sizeof(HTTP_STR_MULTIPART_FIELD_FILE_NAME) - 1)


/*
*********************************************************************************************************
*                                        HTTP UPGRADE VALUE
*********************************************************************************************************
*/

#define  HTTP_STR_HDR_FIELD_UPGRADE_WEBSOCKET              "websocket"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                              DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/

typedef  CPU_INT32U  HTTP_DICT_KEY;

#define  HTTP_DICT_KEY_INVALID                       DEF_INT_32U_MAX_VAL


typedef  struct  HTTP_dict {
    const  HTTP_DICT_KEY   Key;
    const  CPU_CHAR       *StrPtr;
    const  CPU_INT32U      StrLen;
} HTTP_DICT;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           GLOBAL VARIABLES
*********************************************************************************************************
*********************************************************************************************************
*/

extern  const  HTTP_DICT     HTTP_Dict_ReqMethod[];
extern  const  HTTP_DICT     HTTP_Dict_ProtocolVer[];
extern  const  HTTP_DICT     HTTP_Dict_StatusCode[];
extern  const  HTTP_DICT     HTTP_Dict_ReasonPhrase[];
extern  const  HTTP_DICT     HTTP_Dict_FileExt[];
extern  const  HTTP_DICT     HTTP_Dict_ContentType[];
extern  const  HTTP_DICT     HTTP_Dict_HdrField[];
extern  const  HTTP_DICT     HTTP_DictContentDispositionVal[];
extern  const  HTTP_DICT     HTTP_Dict_HdrFieldConnVal[];
extern  const  HTTP_DICT     HTTP_Dict_HdrFieldTransferTypeVal[];
extern  const  HTTP_DICT     HTTP_Dict_MultipartField[];
extern  const  HTTP_DICT     HTTP_Dict_HdrFieldUpgradeVal[];

extern         CPU_SIZE_T    HTTP_Dict_ReqMethodSize;
extern         CPU_SIZE_T    HTTP_Dict_ProtocolVerSize;
extern         CPU_SIZE_T    HTTP_Dict_StatusCodeSize;
extern         CPU_SIZE_T    HTTP_Dict_ReasonPhraseSize;
extern         CPU_SIZE_T    HTTP_Dict_FileExtSize;
extern         CPU_SIZE_T    HTTP_Dict_ContentTypeSize;
extern         CPU_SIZE_T    HTTP_Dict_HdrFieldSize;
extern         CPU_SIZE_T    HTTP_Dict_ContentDispositionValSize;
extern         CPU_SIZE_T    HTTP_Dict_HdrFieldConnValSize;
extern         CPU_SIZE_T    HTTP_Dict_HdrFieldTransferTypeValSize;
extern         CPU_SIZE_T    HTTP_Dict_MultipartFieldSize;
extern         CPU_SIZE_T    HTTP_Dict_HdrFieldUpgradeValSize;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

CPU_INT32U   HTTP_Dict_KeyGet      (const  HTTP_DICT   *p_dict_tbl,
                                           CPU_INT32U   dict_size,
                                    const  CPU_CHAR    *p_str_cmp,
                                           CPU_BOOLEAN  case_sensitive,
                                           CPU_INT32U   str_len);

HTTP_DICT   *HTTP_Dict_EntryGet    (const  HTTP_DICT   *p_dict_tbl,
                                           CPU_INT32U   dict_size,
                                           CPU_INT32U   key);

CPU_CHAR    *HTTP_Dict_ValCopy     (const  HTTP_DICT   *p_dict_tbl,
                                           CPU_INT32U   dict_size,
                                           CPU_INT32U   key,
                                           CPU_CHAR    *p_buf,
                                           CPU_SIZE_T   buf_len);

CPU_CHAR    *HTTP_Dict_StrKeySrch  (const  HTTP_DICT   *p_dict_tbl,
                                           CPU_INT32U   dict_size,
                                           CPU_INT32U   key,
                                    const  CPU_CHAR    *p_str,
                                           CPU_SIZE_T   str_len);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif  /* HTTP_DICT_MODULE_PRESENT  */
