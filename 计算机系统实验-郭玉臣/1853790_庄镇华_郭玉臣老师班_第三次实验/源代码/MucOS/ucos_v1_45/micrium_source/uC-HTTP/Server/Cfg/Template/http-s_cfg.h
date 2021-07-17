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
*                                   HTTP SERVER CONFIGURATION FILE
*
*                                              TEMPLATE
*
* Filename : http-s_cfg.h
* Version  : V3.00.08
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                  HTTP SERVER ARGUMENT CHECK CONFIGURATION
*
* Note(s) : (1) Configure HTTPs_CFG_ARG_CHK_EXT_EN to enable/disable the HTTP server external argument
*               check feature :
*
*               (a) When ENABLED,  ALL arguments received from any port interface provided by the developer
*                   are checked/validated.
*
*               (b) When DISABLED, NO  arguments received from any port interface provided by the developer
*                   are checked/validated.
*
*           (2) Configure HTTPs_CFG_DBG_INFO_EN to enable/disable network protocol suite debug status
*               variables.
*********************************************************************************************************
*/

                                                                /* Configure external argument check feature ...        */
#define  HTTPs_CFG_ARG_CHK_EXT_EN                 DEF_ENABLED
                                                                /*   DEF_DISABLED     External argument check DISABLED  */
                                                                /*   DEF_ENABLED      External argument check ENABLED   */


                                                                /* Configure debug information feature (see Note #2) :  */
#define  HTTPs_CFG_DBG_INFO_EN                    DEF_ENABLED
                                                                /*   DEF_DISABLED  Debug information DISABLED           */
                                                                /*   DEF_ENABLED   Debug information ENABLED            */


/*
*********************************************************************************************************
*                                HTTP SERVER COUNTER MANAGEMENT CONFIGURATION
*
* Note(s) : (1) Configure HTTPs_CFG_CTR_STAT_EN to enable/disable HTTP server suite statistics counters.
*
*           (2) Configure HTTPs_CFG_CTR_ERR_EN  to enable/disable HTTP server suite error      counters.
*********************************************************************************************************
*/

                                                                /* Configure statistics counter feature (see Note #1) : */
#define  HTTPs_CFG_CTR_STAT_EN                    DEF_ENABLED
                                                                /*   DEF_DISABLED     Stat  counters DISABLED           */
                                                                /*   DEF_ENABLED      Stat  counters ENABLED            */

                                                                /* Configure error      counter feature (see Note #2) : */
#define  HTTPs_CFG_CTR_ERR_EN                     DEF_ENABLED
                                                                /*   DEF_DISABLED     Error counters DISABLED           */
                                                                /*   DEF_ENABLED      Error counters ENABLED            */


/*
*********************************************************************************************************
*                               HTTP SERVER FILE SYSTEM CONFIGURATION
*
* Note(s) : (1) Configure HTTPs_CFG_FS_PRESENT_EN to enable when a File System is present and must be used
*               by the HTTP server. Else, if no File System need to be used with the server, the configuration
*               can be disabled to reduce memory space used by the server.
*********************************************************************************************************
*/

#define  HTTPs_CFG_FS_PRESENT_EN                  DEF_ENABLED


/*
*********************************************************************************************************
*                            HTTP SERVER PERSISTENT CONNECTION CONFIGURATION
*
* Note(s) : (1) Configure HTTPs_CFG_PERSISTENT_CONN_EN to enable/disable persistent connection feature
*               source code.
*********************************************************************************************************
*/

                                                                /* Configure Persistent Connection feature ...          */
                                                                /* ... (see Note #1):                                   */
#define  HTTPs_CFG_PERSISTENT_CONN_EN             DEF_ENABLED
                                                                /*   DEF_DISABLED   Persistent Connection DISABLED      */
                                                                /*   DEF_ENABLED    Persistent Connection ENABLED       */


/*
*********************************************************************************************************
*                                     HTTP SERVER HEADER FIELD FEATURE
*
* Note(s) : (1) Configure HTTPs_CFG_HDR_RX_EN to enable/disable header field processing in reception
*               (i.e for headers received in the HTTP request).
*
*           (2) Configure HTTPs_CFG_HDR_TX_EN to enable/disable header field processing in transmission
*               (i.e for headers to include in the HTTP response).
*********************************************************************************************************
*/

                                                                /* Configure Header fields feature in RX (see Note #1): */
#define  HTTPs_CFG_HDR_RX_EN                      DEF_ENABLED
                                                                /*   DEF_DISABLED   Header processing DISABLED          */
                                                                /*   DEF_ENABLED    Header processing ENABLED           */

                                                                /* Configure Header fields feature in TX (see Note #1): */
#define  HTTPs_CFG_HDR_TX_EN                      DEF_ENABLED
                                                                /*   DEF_DISABLED   Header processing DISABLED          */
                                                                /*   DEF_ENABLED    Header processing ENABLED           */


/*
*********************************************************************************************************
*                             HTTP SERVER QUERY STRING CONFIGURATION
*
* Note(s) : (1) Configure HTTPs_CFG_QUERY_STR_EN to enable/disable Query String feature
*               source code.
*********************************************************************************************************
*/

                                                                /* Configure Query String feature (see Note #1):        */
#define  HTTPs_CFG_QUERY_STR_EN                   DEF_ENABLED
                                                                /*   DEF_DISABLED   Query String DISABLED               */
                                                                /*   DEF_ENABLED    Query String ENABLED                */


/*
*********************************************************************************************************
*                                     HTTP SERVER FORM CONFIGURATION
*
* Note(s) : (1) Configure HTTPs_CFG_FORM_EN             to enable/disable           Form processing source code.
*
*           (2) Configure HTTPs_CFG_FORM_MULTIPART_EN   to enable/disable multipart Form processing source code.
*********************************************************************************************************
*/

                                                                /* Configure Form processing feature (see Note #1):     */
#define  HTTPs_CFG_FORM_EN                        DEF_ENABLED
                                                                /*   DEF_DISABLED   Form processing DISABLED            */
                                                                /*   DEF_ENABLED    Form processing ENABLED             */

                                                                /* Configure Multipart Form processing feature ...      */
                                                                /* ... (see Note #2):                                   */
#define  HTTPs_CFG_FORM_MULTIPART_EN              DEF_ENABLED
                                                                /*   DEF_DISABLED   Mutlipart Form processing DISABLED  */
                                                                /*   DEF_ENABLED    Mutlipart Form processing ENABLED   */


/*
*********************************************************************************************************
*                            HTTP SERVER DYNAMIC TOKEN REPLACEMENT CONFIGURATION
*
* Note(s) : (1) Configure HTTPs_CFG_TOKEN_PARSE_EN to enable/disable dynamic token replacement source code.
*********************************************************************************************************
*/

                                                                /* Configure Dynamic token replacement feature ...      */
                                                                /* ... (see Note #1):                                   */
#define  HTTPs_CFG_TOKEN_PARSE_EN                 DEF_ENABLED
                                                                /*   DEF_DISABLED   Dynamic token replacement DISABLED  */
                                                                /*   DEF_ENABLED    Dynamic token replacement ENABLED   */


/*
*********************************************************************************************************
*                                     HTTP SERVER PROXY CONFIGURATION
*
* Note(s) : (1) Configure HTTPs_CFG_ABSOLUTE_URI_EN to enable/disable support of absolute Uniform Resource
*                                                                                         Indentifier (URI).
*********************************************************************************************************
*/

                                                                /* Configure absolute URI support feature ...           */
                                                                /* ... (see Note #1):                                   */
#define  HTTPs_CFG_ABSOLUTE_URI_EN                DEF_ENABLED
                                                                /*   DEF_DISABLED   Absolute URI support DISABLED       */
                                                                /*   DEF_ENABLED    Absolute URI support ENABLED        */


/*
*********************************************************************************************************
*                           DEFAULT FATAL ERROR HTML DOCUMENT CONFIGURATION
*
* Note(s) : (1) Dynamic token replacement is also supported in default error HTML document.
*********************************************************************************************************
*/

                                                                /* Default HTML document returned error.                */
#define  HTTPs_CFG_HTML_DFLT_ERR_PAGE                                                                       \
                                                  "<HTML>\r\n"                                              \
                                                  "<BODY>\r\n"                                              \
                                                  "<HEAD><TITLE>SYSTEM ERROR</TITLE></HEAD>\r\n"            \
                                                  "<H1>#{STATUS_CODE}: #{REASON_PHRASE}</H1>\r\n"           \
                                                  "The operation cannot be completed.\r\n"                  \
                                                  "</BODY>\r\n"                                             \
                                                  "</HTML>\r\n"

