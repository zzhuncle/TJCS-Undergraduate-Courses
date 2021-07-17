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
*                               HTTP INSTANCE SERVER CONFIGURATION FILE
*
*                                              TEMPLATE
*
* Filename : http-s_instance_cfg.c
* Version  : V3.00.08
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                             INCLUDE FILES
*
* Note(s) : (1) All values that are used in this file and are defined in other header files should be
*               included in this file. Some values could be located in the same file such as task priority
*               and stack size. This template file assume that the following values are defined in app_cfg.h:
*
*                   HTTPs_OS_CFG_INSTANCE_TASK_PRIO
*                   HTTPs_OS_CFG_INSTANCE_TASK_STK_SIZE
*
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <app_cfg.h>                                           /* See Note #1.                                         */

#include  "http-s_instance_cfg.h"


/*
*********************************************************************************************************
*********************************************************************************************************
*                             HTTP SERVER INSTANCE TASK CONFIGURATION
*
* Note(s) : (1) We recommend you configure the Network Protocol Stack task priorities & HTTP server Instances'
*               task priorities as follows:
*
*               NET_OS_CFG_IF_TX_DEALLOC_TASK_PRIO  (highest priority)
*
*               HTTPs_OS_CFG_INSTANCE_TASK_PRIO
*
*               NET_OS_CFG_TMR_TASK_PRIO
*               NET_OS_CFG_IF_RX_TASK_PRIO          (lowest  priority)
*
*               We recommend that the uC/TCP-IP Timer task and network interface Receive task be lower
*               priority than almost all other application tasks; but we recommend that the network
*               interface Transmit De-allocation task be higher  priority than all application tasks that use
*               uC/TCP-IP network services.
*
*               However better performance can be observed when the web server instance is set with the lowest priority.
*               Some experimentation could be required to identify the best task priority configuration.
*
*           (2) In general, the size of the uC/HTTP-server task stack will depend on the CPU architecture and compiler
*               used but also the application due to the multiple hook functions available.
*               The easiest and best method for calculating the maximum stack usage for any task/function should be
*               performed statically by the compiler or by a static analysis tool since these can calculate function/task
*               maximum stack usage based on the compiler s actual code generation and optimization settings. So for optimal
*               task stack configuration, we recommend to invest in a task stack calculator tool compatible with your build
*               toolchain.
*               From experience, a stack size of 4KB SHOULD be enough. Certainly, the stack size may be examined and
*               reduced accordingly once the run-time behavior of the device has been analyzed and additional stack space
*               deemed to be unnecessary.
*
*           (3) If DEF_NULL is passed as the stack pointer start, the stack will be allocated on the
*               HEAP memory.
*********************************************************************************************************
*********************************************************************************************************
*/

const  NET_TASK_CFG  HTTPs_TaskCfgInstance = {

    /* CPU_INT32U    Prio                                                                                               */
                                                                /* Configure Instance Task priority (See Note #1):      */
    HTTPs_OS_CFG_INSTANCE_TASK_PRIO,
                                                                /* MUST be >= Minimum OS Priority                       */

    /* CPU_INT32U    StkSizeBytes                                                                                       */
                                                                /* Configure instance task size:                        */
    HTTPs_OS_CFG_INSTANCE_TASK_STK_SIZE,
                                                                /* MUST be >= Minimum stack size allowed by OS.         */

    /* void    *StkPtr                                                                                                  */
                                                                /* Configure the pointer to base of the stack.          */
    DEF_NULL
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                              HTTP SERVER FILE SYSTEM CONFIGURATION
*
* Note(s) : (1) Three types of File System are accepted by the uC/HTTP-server and each has its own
*               configuration structure.
*                        FS TYPE                FS CFG OBJ
*                   --------------------    -------------------
*                   HTTPs_FS_TYPE_NONE   -> HTTPs_CFG_FS_NONE
*                   HTTPs_FS_TYPE_STATIC -> HTTPS_CFG_FS_STATIC
*                   HTTPS_FS_TYPE_DYN    -> HTTPS_CFG_FS_DYN
*
*               Below are examples for the three File System type.
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                    NO FILE SYSTEM CONFIGURATION
*
* Note(s) : (1) When no File System is present, the uC/HTTP-server needs to know the maximum length a path
*               to a resource can have so the server can store adequately the URL received in an HTTP
*               request.
*********************************************************************************************************
*/

#if 1
const  HTTPs_CFG_FS_NONE  HTTPs_CfgFS_None = {

    /* CPU_INT32U  PathLenMax                                                                                           */
                                                                /* Configure maximum path length (see note #1):         */
    256,
                                                                /* MUST be >= 1                                         */
};
#endif


/*
*********************************************************************************************************
*                              HTTP SERVER FILE SYSTEM CONFIGURATION
*
* Note(s) : (1) The pointer to the File System Network API MUST be defined here. See the 'net_fs.h' file
*               for the FS API structure required for Network Applications.
*               If HTTP Static File System is used as File System, the port is already available in
*               uC-HTTP/Server/FS folder and the API structure is defined as 'HTTPs_FS_API_Static'.
*********************************************************************************************************
*/

#if 0
const  HTTPs_CFG_FS_STATIC  HTTPs_CfgFS_Static = {

    /* const  NET_FS_API  *FS_API_Ptr                                                                                   */
                                                                /* Configure instance FS API pointer (see note #1):     */
   &HTTPs_FS_API_Static,
                                                                /* MUST NOT be a NULL                                   */
};
#endif


/*
*********************************************************************************************************
*                              HTTP SERVER FILE SYSTEM CONFIGURATION
*
* Note(s) : (1) The pointer to the File System Network API MUST be defined here. See the 'net_fs.h' file
*               for the FS API structure required for Network Applications.
*               If uC/FS is used as File System, the port is already available in uC-TCPIP/FS folder and the
*               API structure is defined as 'NetFS_API_FS_V4'.
*
*           (2) Web server instance uses a working folder where files and sub-folders are located.
*
*               (a) It can be set as a null pointer (DEF_NULL), if the file system doesn't support
*                   'set working folder' functionality but HTML documents and files must be located in the
*                   default path used by the file system.
*********************************************************************************************************
*/

#if 0
const  HTTPs_CFG_FS_DYN  HTTPs_CfgFS_Dyn = {

    /* const  NET_FS_API  *FS_API_Ptr                                                                                   */
                                                                /* Configure instance FS API pointer (see note #1):     */
   &NetFS_API_FS_V4,
                                                                /* MUST NOT be a NULL                                   */

    /* CPU_CHAR    *WorkingFolderPtr                                                                                    */
                                                                /* Configure instance working folder (see note #2):     */
    HTTPs_CFG_INSTANCE_STR_FOLDER_ROOT
                                                                /* SHOULD be a string pointer                           */
};
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                             HTTP SERVER INSTANCE HEADER RX CONFIGURATION
*
* Note(s) : (1) (a) The uC-HTTP Server core process, by default, the following header fields :
*                       Content-Type
*                       Content-Length
*                       Transfer-Encoding
*                       Location
*                       Connection
*
*               (b) Enabling the header feature allows the processing of additional header fields other than the
*                   the default ones.
*
*                   By configuring the right callback functions, the upper application can :
*
*                       (1) choose which header field(s) received in an http request message to keep in memory blocks for
*                           later processing in hook function associated with request received.
*
*                       (2) add header field(s) to memory blocks that will be included in http response message by the uC-HTTP
*                           server core.
*
*               (c) To allow the processing of additional header fields in reception, 'HTTPs_CFG_HDR_RX_EN' must be set as 'DEF_ENABLED'.
*                   See http-s_cfg.h section 'HTTP HEADER FIELD FEATURE' for further informations.
*
*           (2) The total number of request header field blocks represents the memory blocks pool available
*               for all the connections.
*
*           (3) Each connection has a maximum number of request header field blocks it can used.
*
*               The maximum MUST be equal or less than the total number of request header field blocks (see Note #1).
*
*               If no more request header field blocks are available when a connection solicits one, the server will retry at
*               the next occasion. Therefore, the maximum request header field blocks per connection and the total number
*               of request header field blocks must be set carefully to optimize performance.
*
*           (4) Request header field data length MUST be configured to handle the longest data value the upper application
*               is expected to receive in an header field.
*
*               Only the maximum data length will be kept from a received header field with data longer than the maximum.
*********************************************************************************************************
*********************************************************************************************************
*/

const  HTTPs_HDR_RX_CFG  HTTPs_HdrRxCfg = {

    /* CPU_INT16U    NbrPerConnMax                                                                                      */
                                                                /* Configure number of request header field blocks ...  */
                                                                /* ... (see note #2):                                   */
    LIB_MEM_BLK_QTY_UNLIMITED,
                                                                /* SHOULD be >= 0                                       */

    /* CPU_INT16U    DataLenMax                                                                                         */
                                                                /* Configure maximum of request header field data  ...  */
                                                                /* length (see note #4):                                */
    128,
                                                                /* SHOULD be >= 0                                       */
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                             HTTP SERVER INSTANCE HEADER TX CONFIGURATION
*
* Note(s) : (1) (a) The uC-HTTP Server core process, by default, the following header fields :
*                       Content-Type
*                       Content-Length
*                       Transfer-Encoding
*                       Location
*                       Connection
*
*               (b) Enabling the header feature allows the processing of additional header fields other than the
*                   the default ones.
*
*                   By configuring the right hook functions, the upper application can :
*
*                       (1) choose which header field(s) received in an http request message to keep in memory blocks for
*                           later processing in hook function associated with request received.
*
*                       (2) add header field(s) to memory blocks that will be included in http response message by the uC-HTTP
*                           server core.
*
*               (c) To allow the addition of header fields in transmission, 'HTTPs_CFG_HDR_TX_EN' must be set as 'DEF_ENABLED'.
*                   See http-s_cfg.h section 'HTTP HEADER FIELD FEATURE' for further informations.
*
*           (2) The total number of response header field blocks represents the memory block pools available for all the
*               connections.
*
*           (3) Each connection has a maximum of response header field blocks it can used.
*
*               The maximum MUST be equal or less than the total number of response header field blocks (see Note #1).
*
*               If no more response header field block is available when a connection solicit one, the server will retry at
*               the next occasion. Therefore, the maximum of response header field blocks per connection and the total number
*               of response header field blocks must be set careful to optimize performance.
*
*           (4) Response header field data length MUST be configured to handle the longest data value the upper application
*               is ready to send in an header field.
*********************************************************************************************************
*********************************************************************************************************
*/

const  HTTPs_HDR_TX_CFG  HTTPs_HdrTxCfg = {

    /* CPU_INT16U    NbrPerConnMax                                                                                      */
                                                                /* Configure number of response header field blocks ... */
                                                                /* ...  (see note #2):                                  */
    LIB_MEM_BLK_QTY_UNLIMITED,
                                                                /* SHOULD be >= 0                                       */

    /* CPU_INT16U    DataLenMax                                                                                         */
                                                                /* Configure maximum string length (see note #4):       */
    128,
                                                                /* SHOULD be >= 0                                       */
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                           HTTP SERVER INSTANCE QUERY STRING CONFIGURATION
*
* Note(s) : (1) A Query String is a set of key-value pairs that comes just after the URL and can be used
*               by a HTTP client to pass additional information relative to the request.
*
*               For the uC/HTTP-server to parse a Query String received, the compile-time configuration
*               HTTPs_CFG_QUERY_STR_EN must be enabled and the Query String configuration object pointer
*               (HTTPs_QUERY_STR_CFG) must be set in the Instance runtime configuration (HTTPs_CFG).
*
*               When enabled, the server will parse the received Query String and saved each field in a
*               key-value list accessible in the HTTPs_CONN object.
*
*               The server also supports fields that are single value (no key-value pair). The value will
*               still be saved in a key-value pair block, but only the value parameter will be set.
*
*           (2) The first parameter 'NbrPerConnMax' of the Query String configuration structure is the number of
*               fields in the Query String accepted for one HTTP transaction (connection).
*
*           (3) The maximum length of the key part of the field must be defined in the second parameter
*               'KeyLenMax'.
*               This is the maximum string length the server can accept for a key.
*
*           (4) The maximum length of the value part of the field must be defined in the third parameter
*               'ValLenMax'.
*               This is the maximum string length the server can accept for a value.
*********************************************************************************************************
*********************************************************************************************************
*/

const  HTTPs_QUERY_STR_CFG  HTTPs_QueryStrCfg = {
    /* CPU_INT16U   NbrPerConnMax                                                                                       */
                                                                /* Configure number of key value pairs ...              */
                                                                /* ...  (see note #2):                                  */
    LIB_MEM_BLK_QTY_UNLIMITED,
                                                                /* SHOULD be >= 1                                       */

    /* CPU_INT16U   KeyLenMax                                                                                           */
                                                                /* Configure maximum key length ...                     */
                                                                /* ... (see note #3):                                   */
    15,
                                                                /* SHOULD be > 1                                        */

    /* CPU_INT16U   ValLenMax                                                                                           */
                                                                /* Configure maximum value length ...                   */
                                                                /* ... (see note #4):                                   */
    20,
                                                                /* SHOULD be > 1                                        */
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                             HTTP SERVER INSTANCE FORM CONFIGURATION
*
* Note(s) : (1) (a) Forms in HTML documents are allowed. When the form is posted, the web server will process the POST
*                   action and will invoke the callback with a list of key-value pairs transmitted.
*
*                   Assuming we have an HTML page that look like this:
*
*                       <html>
*                           <body>
*                               <form action="form_return_page.htm" method="post">
*                                   Text Box 1: <input type="text" name="textbox1" /><br>
*                                   Text Box 2: <input type="text" name="textbox1" /><br>
*                                   <input type="submit" name="submit" value="Submit"></input>
*                               </form>
*                           </body>
*                       </html>
*
*
*                   When the client sends the request, the web server should call the callback function with the following
*                   key pair value items:
*
*                       Key-Name: "textbox1",  Key-Value: "Text Box 1 value"
*                       Key-Name: "textbox2",  Key-Value: "Text Box 2 value"
*                       Key-Name: "submit",    Key-Value: "Submit"*
*
*                   For more information see User Manual section 'Form Submissions'.
*
*               (b) Only form method 'POST' action is supported.
*
*               (c) 'HTTPs_CFG_FORM_EN' must be set as 'DEF_ENABLED' to enable the web server instance token replacement.
*                   See http-s_cfg.h section 'HTTP FORM CONFIGURATION' for further information.
*
*
*           (2) (a) Number of control key value pairs must be greater than or equal to the maximum number of inputs
*                   which can be transmitted by one of your forms contained in your HTML documents.
*
*               (b) If the feature is not enabled, this value is not used.
*
*
*           (3) (a) Control name length MUST be configured to handle the longest Key-Name contained in your html documents.
*
*               (b) Control value length MUST be configured to handle the longest Key-Value which can be entered by the user
*                   in your html documents.
*
*
*           (4) (a) Multipart forms MUST be used to transmit large messages such as a file or e-mails message body. HTML pages
*                   that contain mutlipart forms look like this:
*
*                       <html>
*                           <body>
*                               <form action="form_return_page.htm" method="post" enctype="multipart/form-data">
*                                   Browse file: <input type="file" name="fileUploaded"/><br>
*                                   <input type="submit" name="submit" value="Submit"></input>
*                               </form>
*                           </body>
*                       </html>
*
*               (b) If File upload feature is enabled, the web server will store the file received. If the feature is
*                   not enabled and a file is received the file will be simply dropped.
*
*                   (1) File upload is not yet possible with the Static File System.
*
*                   (2) File overwrite must be enabled to allow a file to be received if the file already exists in the folder.
*
*                   (3) A folder name need to be specified to indicate where the uploaded files will be saved.
*
*                       If you wish to save uploaded files directly in the root web directory, the name folder needs
*                       to be set as "\\".
*
*                       If uploaded files need to be saved inside a subfolder of the root web directory, the folder MUST
*                       already exist when the HTTP server tries to access it.
*********************************************************************************************************
*********************************************************************************************************
*/

const  HTTPs_FORM_CFG  HTTPs_FormCfg = {

    /* CPU_INT16U   NbrPerConnMax                                                                                       */
                                                                /* Configure number of control key value pairs ...      */
                                                                /* ...  (see note #2):                                  */
    LIB_MEM_BLK_QTY_UNLIMITED,
                                                                /* SHOULD be >= 1                                       */

    /* CPU_INT16U   KeyLenMax                                                                                           */
                                                                /* Configure maximum key length ...                     */
                                                                /* ... (see note #3a):                                  */
    15,
                                                                /* SHOULD be > 1                                        */

    /* CPU_INT16U   ValLenMax                                                                                           */
                                                                /* Configure maximum value length ...                   */
                                                                /* ... (see note #3b):                                  */
    48,
                                                                /* SHOULD be > 1                                        */

    /* CPU_BOOLEAN  MultipartEn                                                                                         */
                                                                /* Configure instance Multipart form feature ...        */
                                                                /* ... (see note #4a):                                  */
    DEF_ENABLED,
                                                                /*   DEF_DISABLED     multipart form DISABLED           */
                                                                /*   DEF_ENABLED      multipart form ENABLED            */

    /* CPU_BOOLEAN  MultipartFileUploadEn                                                                               */
                                                                /* Configure instance file upload feature ...           */
                                                                /* ... (see note #4b):                                  */
    DEF_ENABLED,
                                                                /*   DEF_DISABLED     File upload DISABLED              */
                                                                /*   DEF_ENABLED      File upload ENABLED               */

    /*CPU_BOOLEAN   MultipartFileUploadOverWrEn                                                                         */
                                                                /* Configure instance file overwrite feature ...        */
                                                                /* ... (see note #4b2):                                 */
    DEF_ENABLED,
                                                                /*   DEF_DISABLED     File overwrite DISABLED           */
                                                                /*   DEF_ENABLED      File overwrite ENABLED            */

    /* CPU_CHAR     *MultipartFileUploadFolderPtr                                                                       */
                                                                /* Configure instance upload folder (see note #4b3):    */
    HTTPs_CFG_INSTANCE_STR_FOLDER_UPLOAD,
                                                                /* SHOULD be a string pointer.                          */
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                             HTTP SERVER INSTANCE TOKEN CONFIGURATION
*
* Note(s) : (1) (a) Dynamic content can be inserted in HTML web pages (files having the htm or html suffix) by using
*                   special tokens being substituted when the page is actually sent to the web browser. Those tokens
*                   are represented in an HTML document as:
*
*                       ${TOKEN_NAME}
*
*                   Assuming we have an HTML page that look like this:
*
*                       <html>
*                           <body>
*                               This system's IP address is ${My_IP_Address}
*                           </body>
*                       </html>
*
*
*                   When a web client requests this file, the web server will parse the file, find the ${My_IP_Address}
*                   token, and pass the string " My_IP_Address " into the callback function. That function will then
*                   substitute the token for its value, sending the following HTML file to the client:
*
*                       <html>
*                           <body>
*                               This system's IP address is 135.17.115.215
*                           </body>
*                       </html>
*
*               (b) 'HTTPs_CFG_TOKEN_PARSE_EN' must be set as 'DEF_ENABLED' to enable the web server instance token
*                   replacement. See http-s_cfg.h section 'HTTP DYNAMIC TOKEN REPLACEMENT CONFIGURATION' for further
*                   information.
*
*           (2) (a) Each connection that transmits an HTML document requires only one token. So to optimize performance, numbers
*                   of tokens SHOULD be equally configured to the maximum number of HTML documents that can be transmitted
*                   simultaneously.
*
*               (b) If dynamic token replacement feature is enabled, number of tokens must be greater than or equal to
*                   one.
*
*           (3) (a) The web server reserves a value buffer for each token which is passed to the callback function to be
*                   filled with the replacement value. Therefore, the length of the token value must be configured to handle the
*                   longest value.
*
*               (b) If the dynamic token replacement feature is enabled, token value length must be greater than or equal
*                   to one.
*********************************************************************************************************
*********************************************************************************************************
*/

const  HTTPs_TOKEN_CFG  HTTPs_TokenCfg = {

    /* CPU_INT16U   NbrPerConnMax                                                                                       */
                                                                /* Configure instance number of token (see note #2):    */
    LIB_MEM_BLK_QTY_UNLIMITED,
                                                                /* SHOULD be >= 1                                       */

    /* CPU_INT16U   ValLenMax                                                                                           */
                                                                /* Configure instance token value length (see note #3): */
    12,
                                                                /* SHOULD be >= 1                                       */
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                               HTTP SERVER INSTANCE CONFIGURATION
*********************************************************************************************************
*********************************************************************************************************
*/

const  HTTPs_CFG  HTTPs_CfgInstance = {

/*
*--------------------------------------------------------------------------------------------------------
*                                     INSTANCE TASK CONFIGURATION
*
* Note(s) : (1) The web server can delay this task periodically to allow other tasks with lower priority to run.
*--------------------------------------------------------------------------------------------------------
*/

    /* CPU_INT32U    OS_TaskDly_ms                                                                                      */
                                                                /* Configure instance task delay ...                    */
    1,
                                                                /* ... in integer milliseconds (see Note #1).           */


/*
*--------------------------------------------------------------------------------------------------------
*                                INSTANCE LISTEN SOCKET CONFIGURATION
*
* Note(s) : (1) Configure socket type. Select which kind of IP addresses can be accepted by the web server instance.
*
*                   (a) HTTPs_SOCK_SEL_IPv4:      Accept Only IPv4
*                   (b) HTTPs_SOCK_SEL_IPv6:      Accept Only IPv6
*                   (c) HTTPs_SOCK_SEL_IPv4_IPv6: Accept IPv4 and IPv6
*
*           (2) (a) 'Secure' field is used to enabled or disable the Secure Sockets Layer (SSL):
*
*                           DEF_NULL,                                  the web server instance is not secure and doesn't
*                                                                      use SSL.
*
*                           Point to a secure configuration structure, the web server is secure and uses SSL.
*
*               (b) The secure web server can be enabled ONLY if the application project contains a secure module
*                   supported by uC/TCPIP-V2 such as:
*
*                   (i)  NanoSSL provided by Mocana.
*                   (ii) CyaSSL  provided by YaSSL.
*
*           (3) (a) Default HTTP port used by all web browsers is 80. The default port number is defined by the following
*                   value:
*
*                        HTTPs_CFG_DFLT_PORT
*
*
*                   When default port is used the web server instance can be accessed using the IP address of the target
*                   from any web browser:
*
*                       http://<target IP address>
*
*                   If the web server instance is configured with the non default port, the instance server should be accessed
*                   via this kind of address:
*
*                       http://<target IP address>:<port number>
*
*                       Where
*                           <target IP address> must be replaced by the ip address of the target.
*                           <port number>       must be replaced by the configured port number.
*
*               (b) Default secure port used by all browsers is 443. The default secure port number is defined by the
*                   following value:
*
*                       HTTPs_CFG_DFLT_PORT_SECURE
*
*                   When default port is used the web server instance can be accessed using the IP address of the target
*                   from any web browser:
*
*                       https://<target IP address>
*
*                   If the web server instance is configured with the non default port, the instance server should be accessed
*                   via this kind of address:
*
*                       https://<target IP address>:<port number>
*
*                       Where
*                           <target IP address> must be replaced by the ip address of the target.
*                           <port number>       must be replaced by the configured port number.
*
*               (c) Port number must be unique, i.e. it's not possible to start two instances with the same
*                   port number.
*--------------------------------------------------------------------------------------------------------
*/

    /* HTTPs_SOCK_SEL    SockSel                                                                                        */
                                                                /* Configure socket type (see note #1) :                */
    HTTPs_SOCK_SEL_IPv4_IPv6,
                                                                /* HTTPs_SOCK_SEL_IPv4       Accept Only IPv4.          */
                                                                /* HTTPs_SOCK_SEL_IPv6       Accept Only IPv6.          */
                                                                /* HTTPs_SOCK_SEL_IPv4_IPv6  Accept Only Ipv4 & IPv6.   */

    /* HTTPs_SECURE_CFG    *SecurePtr                                                                                   */
                                                                /* Configure instance secure configuration (SSL) ...    */
                                                                /* structure (see note #2):                             */
    DEF_NULL,
                                                                /* DEF_NULL for a non-secure web server.                */
                                                                /* Pointer to the secure configuration to be used.      */

    /* CPU_INT16U    Port                                                                                               */
                                                                /* Configure instance server port (See note #3) :       */
    HTTPs_CFG_DFLT_PORT,
                                                                /* HTTPs_CFG_DFLT_PORT         Default HTTP     port.   */
                                                                /* HTTPs_CFG_DFLT_PORT_SECURE  Default HTTP SSL port.   */


/*
*--------------------------------------------------------------------------------------------------------
*                                  INSTANCE CONNECTION CONFIGURATION
*
* Note(s) : (1) (a) 'ConnNbrMax' is used to configure maximum number of connections that the web server will be able to serve
*                   simultaneously.
*
*               (b) Maximum number of connections must be configured following your requirements about the memory usage and
*                   the number of connections:
*
*                   (1) Each connection requires memory space which is reserved at the instance start up. The memory
*                       required by the web server is greatly affected by the number of connections configured.
*
*                   (2) When a client downloads an items such as an html document, image, css file, javascript file, it
*                       should open a new connection for each of these items. Also, most common web servers can open up to
*                       15 simultaneous connections. For example, 1 html document which includes 2 images + 1 css file,
*                       should have 4 connections simultaneously opened.
*
*               (c) The number of connections and uC/TCPIP configurations must be set accordingly. Each connection
*                   requires 1 socket and 1 TCP connection, so the following configuration values located in 'net_cfg.h'
*                   MUST be correctly configured:
*
*                       NET_SOCK_CFG_SOCK_NBR_TCP
*
*           (2) For each connection, when the inactivity timeout occurs, the connection is automatically closed with
*               whatever the last connection state was.
*
*           (3) Each connection has a buffer to receive or transmit data and to read files. If the memory is limited the buffer
*               size can be reduced, but the performance might be impacted.
*
*           (4) (a) Enabling the persistent connection feature allows the connection to stay open for multiple HTTP transactions
*                   before closing. This allows to reduce the traffic because the 3-way-handshake and the closing are done
*                   only once instead of after each HTTP transactions.
*
*               (b) To allow the connection to stay open after HTTP transactions, 'HTTPs_CFG_PERSISTENT_CONN_EN' must be
*                   set as 'DEF_ENABLED'.
*                   See http-s_cfg.h section 'HTTP SERVER PERSISTENT CONNECTION CONFIGURATION' for further informations.
*--------------------------------------------------------------------------------------------------------
*/

    /* CPU_INT08U    ConnNbrMax                                                                                         */
                                                                /* Configure maximum number of simultaneous ...         */
                                                                /* connections (see Note #1):                           */
    15,
                                                                /* MUST be >= 1                                         */

    /* CPU_INT16U    ConnInactivityTimeout_s                                                                            */
                                                                /* Configure connection maximum inactivity timeout ...  */
                                                                /* ... in integer seconds (see Note #2).                */
    10,
                                                                /* SHOULD be >= 1                                       */

    /* CPU_INT16U    BufLen                                                                                             */
                                                                /* Configure connection buffer length (see note #3):    */
    1460,
                                                                /* MUST be >= 512                                       */

    /* CPU_BOOLEAN    ConnPersistentEn                                                                                  */
                                                                /* Configure persistent conn feature (see note #4):     */
    DEF_ENABLED,
                                                                /*   DEF_DISABLED     Persistent Conn support DISABLED  */
                                                                /*   DEF_ENABLED      Persistent Conn support ENABLED   */


/*
*--------------------------------------------------------------------------------------------------------
*                                  INSTANCE FILE SYSTEM CONFIGURATION
*
* Note(s) : (1) The parameter 'FS_Type' is used to indicate which type of File System must be used with the HTTP server.
*               The server supports three type of File System :
*
*                   HTTPs_FS_TYPE_NONE   : No File System is present.
*                   HTTPs_FS_TYPE_STATIC : The Static File System offered with the uC/HTTP-server must be used.
*                   HTTPs_FS_TYPE_DYN    : A real File System, like uC/FS, must be used.
*
*           (2) Each type of File System (see note #1) has it's own File System configuration object.
*
*                   HTTPs_CFG_FS_NONE
*                   HTTPs_CFG_FS_STATIC
*                   HTTPs_CFG_FS_DYN
*
*               See section 'HTTP SERVER FILE SYSTEM CONFIGURATION' for examples for each configuration object type.
*
*           (3) The default resource is returned when no resource is specified in the request of the client,
*               i.e. accessing with only the web server address. Most of the time this resource should be the file
*               "index.html".
*--------------------------------------------------------------------------------------------------------
*/

    /* HTTPs_FS_TYPE   FS_Type                                                                                          */
                                                                /* Configure instance FS type (see note #1):            */
    HTTPs_FS_TYPE_NONE,


    /* const  void  *FS_CfgPtr                                                                                          */
                                                                /* Configure FS configuration pointer (see note #2):    */
   &HTTPs_CfgFS_None,

    /* CPU_CHAR    *DfltResourceNamePtr                                                                                 */
                                                                /* Configure instance default page (see note #3):       */
    HTTPs_CFG_INSTANCE_STR_FILE_DEFAULT,
                                                                /* MUST be a string pointer                             */


/*
*--------------------------------------------------------------------------------------------------------
*                                    INSTANCE PROXY CONFIGURATION
*
* Note(s) : (1) (a) When an HTTP Server is behind an HTTP Proxy, the HTTP client must send its requests with an
*                    absolute Uniform Resource Identifier (URI).
*                   For example,
*                       GET http://example.com/index.html HTTP/1.1
*
*                   When the absolute URI feature is enabled, the HTTP server will support absolute URI in the first line
*                   of the http request messages (see example just above).
*
*                   The server will also look for the 'Host' header field in the received request messages and save it in
*                   the 'HostPtr' field of the HTTPs_CONN structure.
*
*               (b) 'HTTPs_CFG_ABSOLUTE_URI_EN' must be set as 'DEF_ENABLED' to enable the web server support of
*                    absolute URI.
*                    See the http-s_cfg.h section of 'HTTP PROXY CONFIGURATION' for further information.
*
*               (c) The maximum host name length is the maximum length the server will allow for the received host name
*                   in a request message.
*--------------------------------------------------------------------------------------------------------
*/

   /* CPU_INT16U   HostNameLenMax                                                                                      */
                                                               /* Configure maximum host name length (see note #1c):   */
   128,
                                                               /* SHOULD be > 1                                        */

/*
*--------------------------------------------------------------------------------------------------------
*                                           HOOKS CONFIGURATION
*
* Note(s) : (1) Multiple hook functions are offered by the uC/HTTP-server stack at different points of the HTTP
*               transaction processing to personalize the server behavior. See Example Init for all the details on the
*               hook functions available or online documentation at https://doc.micrium.com/display/httpdoc.
*               If the application is using some hook functions, a HTTPs_HOOK_CFG object must be created that will
*               contained all the pointer to the hook functions.
*               The pointer to the HTTPs_HOOK_CFG object must after be passed to the HTTP-server configuration here.
*               The parameter can be set to DEF_NULL if no hook functions are necessary for the upper application.
*
*           (2) Additional data or configuration specific to the upper application can be necessary inside hook
*               functions. Therefore a void pointer is offered as second parameter for the hooks configuration.
*               The parameter can be set to DEF_NULL if not useful.
*--------------------------------------------------------------------------------------------------------
*/

    /* const  HTTPs_HOOK_CFG    *HooksPtr                                                                               */
                                                                /* Configure Pointer to Hooks' Object (see note #1):    */
    DEF_NULL,

    /* const  void  *Hooks_CfgPtr                                                                                       */
                                                                /* Configure Pointer to Application Data Hook.          */
    DEF_NULL,


/*
*--------------------------------------------------------------------------------------------------------
*                                     HEADER FIELD CONFIGURATION
*
* Note(s) : (1) To enable the Header feature supports in HTTP request, set the pointer to the Request Header configuration object.
*               Set to DEF_NULL, if the Request Header feature is not used.
*
*               See HTTPs_HdrRxCfg Declaration for more details.
*
*           (2) To enable the Header feature supports in HTTP response, set the pointer to the Response Header configuration object.
*               Set to DEF_NULL, if the Response Header feature is not used.
*
*               See HTTPs_HdrTxCfg Declaration for more details.
*--------------------------------------------------------------------------------------------------------
*/

    /* HTTPs_HDR_RX_CFG  *HdrRxCfgPtr                                                                                   */
                                                     /* Pointer to Request header Configuration Object (see note #1):   */
   &HTTPs_HdrRxCfg,

    /* HTTPs_HDR_TX_CFG  *HdrTxCfgPtr                                                                                   */
                                                    /* Pointer to Response header Configuration Object (see note #2):   */
   &HTTPs_HdrTxCfg,


/*
*--------------------------------------------------------------------------------------------------------
*                                  INSTANCE QUERY STRING CONFIGURATION
*
* Note(s) : (1) To enable the Query String feature supports, set the pointer to the Query String configuration object.
*               Set to DEF_NULL, if Query String feature is not used.
*
*               See HTTPs_QueryStrCfg Declaration for more details.
*--------------------------------------------------------------------------------------------------------
*/

    /* HTTPs_QUERY_STR_CFG  *QueryStrCfgPtr                                                                             */
                                                        /* Pointer to Query String Configuration Object (see note #1):  */
   &HTTPs_QueryStrCfg,


/*
*--------------------------------------------------------------------------------------------------------
*                                    INSTANCE FORM CONFIGURATION
*
* Note(s) : (1) To enable the Form feature supports, set the pointer to the Form configuration object.
*               Set to DEF_NULL, if Form feature is not used.
*
*               See HTTPs_Form_Cfg Declaration for more details.
*--------------------------------------------------------------------------------------------------------
*/

    /* HTTPs_FORM_CFG  *FormCfgPtr                                                                                      */
                                                                /* Pointer to Form Configuration Object (see note #1):  */
   &HTTPs_FormCfg,

/*
*--------------------------------------------------------------------------------------------------------
*                               DYNAMIC TOKEN REPLACEMENT CONFIGURATION
*
* Note(s) : (1) To enable the Token feature supports, set the pointer to the Token configuration object.
*               Set to DEF_NULL, if the Token feature is not used.
*
*               See HTTPs_TokenCfg Declaration for more details.
*--------------------------------------------------------------------------------------------------------
*/

    /* HTTPs_TOKEN_CFG  *Token_CfgPtr                                                                                   */
                                                              /* Pointer to Token Configuration Object (see note #1):   */
   &HTTPs_TokenCfg,

};                                                              /* End of configuration structure.                      */

