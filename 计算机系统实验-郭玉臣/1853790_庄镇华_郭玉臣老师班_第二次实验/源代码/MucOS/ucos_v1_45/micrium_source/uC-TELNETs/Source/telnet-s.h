/*
*********************************************************************************************************
*                                             uC/TELNETs
*                                           Telnet (server)
*
*                         (c) Copyright 2004-2018; Silicon Laboratories Inc.,
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
*                                            TELNET SERVER
*
* Filename : telnet-s.h
* Version  : V1.05.03
*********************************************************************************************************
*
*
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               TELNETs present pre-processor macro definition.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  TELNETs_PRESENT                                        /* See Note #1.                                         */
#define  TELNETs_PRESENT


/*
*********************************************************************************************************
*                                       TELNETs VERSION NUMBER
*
* Note(s) : (1) (a) The TELNETs module software version is denoted as follows :
*
*                       Vx.yy.zz
*
*                           where
*                                   V               denotes 'Version' label
*                                   x               denotes     major software version revision number
*                                   yy              denotes     minor software version revision number
*                                   zz              denotes sub-minor software version revision number
*
*               (b) The TELNETs software version label #define is formatted as follows :
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
*/

#define  TELNETs_VERSION                               10503u   /* See Note #1.                                         */


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*********************************************************************************************************
*/

#ifdef   TELNETs_MODULE
#define  TELNETs_EXT
#else
#define  TELNETs_EXT  extern
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*
* Note(s) : (1) The TELNETs module files are located in the following directories :
*
*               (a) \<Your Product Application>\telnet-s_cfg.h
*
*               (b) (1) \<Network Protocol Suite>\Source\net_*.*
*
*                   (2) If network security manager is to be used:
*
*                       (A) \<Network Protocol Suite>\Secure\net_secure_mgr.*
*
*                       (B) \<Network Protocol Suite>\Secure\<Network Security Suite>\net_secure.*
*
*               (c) (1) \<TELNETs>\Source\telnet-s.h
*                                        \telnet-s.c
*
*                   (2) \<TELNETs>\OS\<os>\telnet-s_os.*
*
*                       where
*                               <Your Product Application>      directory path for Your Product's Application
*                               <Network Protocol Suite>        directory path for network protocol suite
*                               <TELNETs>                       directory path for TELNETs module
*                               <os>                            directory name for specific operating system (OS)
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
*                   (2) (A) '\<Network Protocol Suite>\Secure\'                             See Note #1b2A
*                       (B) '\<Network Protocol Suite>\Secure\<Network Security Suite>\'    See Note #1b2B
*
*               (c) '\<TELNETs>\' directories                                               See Note #1c
*
*               (d) (1) '\<CPU-Compiler Directory>\'                                        See Note #2a
*                   (2) '\<CPU-Compiler Directory>\<cpu>\<compiler>\'                       See Note #2b
*
*               (e) '\<Custom Library Directory>\'                                          See Note #3a
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <cpu.h>                                               /* CPU Configuration              (see Note #2b)        */
#include  <cpu_core.h>                                          /* CPU Core Library               (see Note #2a)        */

#include  <lib_def.h>                                           /* Standard        Defines        (see Note #3a)        */
#include  <lib_str.h>                                           /* Standard String Library        (see Note #3a)        */

#include  <telnet-s_cfg.h>                                      /* Telnet Configuration File      (see Note #1a)        */

#include  <Source/net.h>                                        /* Network Protocol Suite         (see Note #1b)        */
#include  <Source/net_sock.h>

#if 1
#include  <stdio.h>
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#define  TELNETs_SHELL_ERR_TX                             -1


/*
*********************************************************************************************************
*                                      TELNETs ERROR CODES DEFINES
*
* Note(s) : (1) The TELNETs_CmdProcess() function MUST return TELNETs_CMDPROCESS_ERR when an error occurred.
*               Any other return value means the processing of the command was successful.
*********************************************************************************************************
*/

#define  TELNETs_CMDPROCESS_ERR_NONE                       0
#define  TELNETs_CMDPROCESS_ERR                           -1    /* See Note #1.                                         */

typedef enum {
    TELNETs_ERR_NONE                        =  0,
    TELNETs_ERR_NONE_EOL_RX,
    TELNETs_ERR_NONE_OPT_STATUS_NOT_CHANGED,
    TELNETs_ERR_SOCK,
    TELNETs_ERR_CONN_CLOSED,
    TELNETs_ERR_RX_TIMEOUT,
    TELNETs_ERR_RX,
    TELNETs_ERR_TX,
    TELNETs_ERR_OPT_NOT_SUPPORTED,
    TELNETs_ERR_OPT_CODE_UNKNOWN,
    TELNETs_ERR_OPT_STATUS_UNKNOWN,
    TELNETs_ERR_NULL_PTR,
    TELNETs_ERR_LEN_ZERO,
    TELNETs_ERR_BUF_TOO_SMALL,
    TELNETs_ERR_CMD_PROCESS,
    TELNETs_ERR_CMD_EXEC
} TELNETs_ERR;


/*
*********************************************************************************************************
*                                 TELNETs SESSION AND CONNECTION DEFINES
*
* Note(s) : (1) This defines the maximum number of session task(s) supported.
*
*               (a) #### This current version of TELNETs only supports 1 client at a time.  Therefore,
*                   this define should always be set to '1'.
*********************************************************************************************************
*/

#define  TELNETs_SESSION_TASKS_MAX                         1    /* See Note #1.                                         */

#define  TELNETs_CONN_Q_SIZE                               3

#define  TELNETs_FAILED_LOGIN_DLY_MS                     200    /* Dly between login tries.                             */


/*
*********************************************************************************************************
*                                      TELNET USASCII GRAPHIC CODES
*
* Note(s) : From RFC #1938, section 'The NVT printer and keyboard', "The NVT can produce representations
*           of all 95 USASCII graphics (codes 32 through 126)".  The lower and higher codes are here
*           defined to help determine if they should be sent to the printer or not.
*********************************************************************************************************
*/

#define  TELNET_ASCII_GRAPHIC_LOWER                       32    /* Lower  graphic code displayed by the NVT.            */
#define  TELNET_ASCII_GRAPHIC_HIGHER                     126    /* Higher graphic code displayed by the NVT.            */


/*
*********************************************************************************************************
*                                        TELNET COMMAND DEFINES
*
* Note(s) : (1) According to RFC #854, 'TELNET PROTOCOL SPECIFICATION', This should always be accompanied
*               by a TCP Urgent notification.
*
*           (2) The 'SB' command indicates that what follows is sub-negotiation of the indicated option.
*
*           (3) From RFC #854, 'TELNET PROTOCOL SPECIFICATION', section 'General Considerations', "The
*               principle of negotiated options takes cognizance of the fact that many hosts will wish
*               to provide additional services over and above those available within an NVT [and these
*               options] may be used with the DO, DON'T, WILL, and WON'T structure to allow a user and
*               server to agree to use a more elaborate set of conventions for their TELNET connection".
*
*               WILL XXX is sent to indicate a party's desire (offer) to begin performing option XXX.
*                        DO    XXX    positive response
*                        DON'T XXX    negative response
*
*               DO   XXX is sent to indicate a desire (request) that the other party begin performing
*               option XXX
*                        WILL  XXX    positive acknowledgment
*                        WON'T XXX    negative acknowledgment
*
*               Since a basic NVT is what is left when no options are enabled, responding with DON'T and
*               WON'T guarantees to leave the connection in a state which both hosts can handle.
*
*           (4) When calling function TELNETs_TxCmd() with a command not related to any option, this
*               define should be passed as the third parameter.
*********************************************************************************************************
*/



typedef enum {                                                  /* See Note #3.                                         */
         TELNETs_OPT_STATUS_CMD_SE   =                   240,   /* End of subnegotiation parameters.                    */
         TELNETs_OPT_STATUS_CMD_NOP  =                   241,   /* No operation.                                        */
         TELNETs_OPT_STATUS_CMD_DM   =                   242,   /* Data stream portion of a Synch (see Note #1).        */
         TELNETs_OPT_STATUS_CMD_BRK  =                   243,   /* NVT character BRK (Break).                           */
         TELNETs_OPT_STATUS_CMD_IP   =                   244,   /* The function IP   (Interrupt Process).               */
         TELNETs_OPT_STATUS_CMD_AO   =                   245,   /* The function AO   (Abord Output).                    */
         TELNETs_OPT_STATUS_CMD_AYT  =                   246,   /* The function AYT  (Are You There).                   */
         TELNETs_OPT_STATUS_CMD_EC   =                   247,   /* The function EC   (Erase Character).                 */
         TELNETs_OPT_STATUS_CMD_EL   =                   248,   /* The function EL   (Erase Line).                      */
         TELNETs_OPT_STATUS_CMD_GA   =                   249,   /* The GA signal     (Go Ahead).                        */
         TELNETs_OPT_STATUS_CMD_SB   =                   250,   /* Beginning of subnegotiation (see Note #2).           */
         TELNETs_OPT_STATUS_CMD_WILL =                   251,
         TELNETs_OPT_STATUS_CMD_WONT =                   252,
         TELNETs_OPT_STATUS_CMD_DO   =                   253,
         TELNETs_OPT_STATUS_CMD_DONT =                   254,
         TELNETs_OPT_STATUS_CMD_IAC  =                   255    /* The IAC command   (Indicate As Command).             */
}  TELNETs_OPT_STATUS_CMD;




#define  TELNET_NO_OPT                                    -1    /* See Note #4.                                         */


#define  TELNETs_CMD_MAX_BUF_LEN                           3    /* Cmd buf defines.                                     */
#define  TELNETs_CMD_BUF_LEN_NO_OPT                        2
#define  TELNETs_CMD_BUF_LEN_WITH_OPT                      3

#define  TELNETs_CMD_IAC_OFFSET                            0
#define  TELNETs_CMD_CMD_OFFSET                            1
#define  TELNETs_CMD_OPT_OFFSET                            2


/*
*********************************************************************************************************
*                                        TELNET OPTION DEFINES
*
* Note(s) : (1) This section defines some telnet option codes.  This list is not intended to be exhaustive,
*               and the listed options codes are not necessarily supported and implemented.  Indeed, the
*               list of supported options can be found in the file telnet-s.c, under the "INITIALIZED DATA"
*               section.
*********************************************************************************************************
*/

#define  TELNET_OPT_TX_BINARY                              0
#define  TELNET_OPT_ECHO                                   1
#define  TELNET_OPT_SUPPRESS_GA                            3
#define  TELNET_OPT_STATUS                                 5
#define  TELNET_OPT_TIMING_MARK                            6
#define  TELNET_OPT_EXT_OPT_LIST                         255


/*
*********************************************************************************************************
*                                    TELNET SUPPORTED OPTIONS DEFINE
*
* Note(s) : (1) This defines the number of supported options in this current implementation.  The particular
*                options are defined in the TELNETs_SupportedOptTbl table in the implementation file.  This
*                define MUST match the number of options appearing in previously mentioned table.
*********************************************************************************************************
*/

#define  TELNET_NBR_OPT_SUPPORTED                          2    /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                 TELNETs STRING AND CHARACTER DEFINES
*********************************************************************************************************
*/

#define  TELNETs_SYS_MSG_STR                       "\x0D\x0A"  \
                                                   "Micrium Telnet Server\x0D\x0A\x0D\x0A"

#define  TELNETs_NO_SERVICE_STR                    "Service not available, try again later\x0D\x0A"

#define  TELNETs_PROMPT_STR                        ">"

#define  TELNETs_LOGIN_STR                         "login: "
#define  TELNETs_PW_STR                            "password: "
#define  TELNETs_LOGIN_FAILURE_STR                 "Login incorrect\x0D\x0A"

#define  TELNETs_CMD_PROCESS_ERR_STR               "Command not recognized\x0D\x0A"

#define  TELNETs_EOL_STR                           "\x0D\x0A"
#define  TELNETs_EOL_STR_LEN                               2


/*
*********************************************************************************************************
*                                  TELNETs INTERNAL COMMANDS DEFINES
*********************************************************************************************************
*/

#define  TELNETs_INT_CMD_LOGOUT                    "logout"


/*
*********************************************************************************************************
*                                       TELNET NVT PARSE STATES
*
* Note(s) : (1) The TELNETs_NVTPrint() function relies on a state machine in to parse incoming data in
*               order to treat them as telnet command or as regular USASCII graphic characters.  The
*               TELNETs_NVT_STATE enum defines the various states the machine may be in.
*********************************************************************************************************
*/

typedef enum {
    TELNETs_NVT_STATE_GRAPHIC  = 1,
    TELNETs_NVT_STATE_CR       = 2,
    TELNETs_NVT_STATE_IAC      = 3,
    TELNETs_NVT_STATE_CODE     = 4,
    TELNETs_NVT_STATE_OPTION   = 5
} TELNETs_NVT_STATE;


/*
*********************************************************************************************************
*                                     TELNETs OPTION STATUS DATA TYPES
*
* Note(s) : (1) This structure contains the option status for both sides of the connection (server and
*               client), as well as a flag indicating whether or not we have sent a request for a change
*               in option status not yet acknowledged for.
*********************************************************************************************************
*/

typedef struct  TELNETs_Opt {
    CPU_INT08U              code;                               /* Num code   for the opt.                              */
    TELNETs_OPT_STATUS_CMD  server_status;                      /* Status     for the opt (server side).                */
    TELNETs_OPT_STATUS_CMD  client_status;                      /* Status     for the opt (client side).                */
    CPU_BOOLEAN             server_status_req_tx;               /* Server status change req tx'd.                       */
    CPU_BOOLEAN             client_status_req_tx;               /* Client status change req tx'd.                       */
}  TELNETs_OPT;


/*
*********************************************************************************************************
*                                      TELNETs SESSION DATA TYPE
*
* Note(s) : (1) This structure is used by the session tasks to maintain their connection information.
*
*           (2) The reception buffer has its size set to a configurable length.
*
*           (3) The NVT printer buffer is used to simulate the presence of a console.  What this buffer
*               contains is the last line (current one) of the terminal.  The character in this buffer
*               should match the ones on the client's terminal.
*
*           (4) The list of the options is set when initializing the TELNETs_SESSION structure instance
*                in the TELNETs_NVTInit() function.  When option are supported, the NVT will accept request
*                from the client to change their status, and will allow the server to issue such a request.
*********************************************************************************************************
*/

typedef struct  TELNETs_Session {
    CPU_BOOLEAN             session_active;                     /* Whether the session is active or not.                */
    NET_SOCK_ID             sock_id;                            /* Sock id for this session.                            */

    void                   *pcur_working_dir;                   /* Cur working dir ptr.                                 */

    CPU_CHAR                rx_buf[TELNETs_CFG_RX_BUF_LEN];     /* Rx buf          (see Note #2).                       */
    CPU_INT32U              rx_buf_len;                         /* Len of valid data in rx buf.                         */

                                                                /* NVT printer buf (see Note #3).                       */
    CPU_CHAR                nvt_buf[TELNETs_CFG_NVT_BUF_LEN + TELNETs_EOL_STR_LEN + 1];
    CPU_INT32U              nvt_buf_len;                        /* Len of valid data in nvt buf.                        */

    CPU_INT08U              rx_opt_code;                        /* Last rx'd opt code.                                  */
    TELNETs_OPT_STATUS_CMD  rx_opt_status_cmd;                  /* Last rx'd opt status cmd.                            */

    TELNETs_NVT_STATE       nvt_state;                          /* Last NVT state.                                      */

    TELNETs_OPT             opt[TELNET_NBR_OPT_SUPPORTED];      /* Supported opt   (See Note #4).                       */
}  TELNETs_SESSION;


/*
*********************************************************************************************************
*                              TELNET OUTPUT FUNCTION POINTER DATA TYPE
*********************************************************************************************************
*/

typedef  CPU_INT16S  (*TELNET_OUT_FNCT)(CPU_CHAR   *,
                                        CPU_INT16U  ,
                                        void       *);


/*
*********************************************************************************************************
*                            TELNETs SECURE SESSION CONFIGURATION DATA TYPE
*********************************************************************************************************
*/

typedef  struct  TELNETs_SecureCfg {
    CPU_CHAR                      *CertPtr;
    CPU_INT32U                     CertLen;
    CPU_CHAR                      *KeyPtr;
    CPU_INT32U                     KeyLen;
    NET_SOCK_SECURE_CERT_KEY_FMT   Fmt;
    CPU_BOOLEAN                    CertChain;
} TELNETs_SECURE_CFG;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           GLOBAL VARIABLES
*
* Note(s) : (1) The current implementation of this TELNET server only support one session (client) at the
*               time.  When future release gets rid of this limitation, the TELNETs_ActiveSession global
*               variable will have to be changed to an array of structure, and an allocation mechanism
*               will have to be included in the server task.
*********************************************************************************************************
*********************************************************************************************************
*/

TELNETs_EXT  CPU_INT32U       TELNETs_NbrActiveSessionTask;     /* Nbr of active session tasks.                         */
TELNETs_EXT  TELNETs_SESSION  TELNETs_ActiveSession;            /* See Note #1.                                         */


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

CPU_BOOLEAN  TELNETs_Init       (       NET_SOCK_ADDR_FAMILY   family,
                                 const  TELNETs_SECURE_CFG    *p_secure_cfg);   /* TELNET server startup function.      */

void         TELNETs_ServerTask (       void                  *p_arg);          /* TELNET server  main loop.            */

void         TELNETs_SessionTask(       void                  *p_arg);          /* TELNET session main loop.            */


/*
*********************************************************************************************************
*                                APPLICATION CALLBACK FUNCTION PROTOTYPES
*                                      (see user application app.c)
*********************************************************************************************************
*/

                                                                /* Authenticate usr.                                    */
CPU_BOOLEAN  TELNETs_AuthUser     (CPU_CHAR         *user_name,
                                   CPU_CHAR         *pw);

                                                                /* Process ext cmd.                                     */
CPU_INT16S   TELNETs_CmdHandlerExt(CPU_CHAR         *pcmd_line,
                                   void             *pcwd,
                                   CPU_BOOLEAN      *psession_active,
                                   void             *pout_opt,
                                   TELNET_OUT_FNCT   pout_fnct,
                                   TELNETs_ERR      *perr);


/*
*********************************************************************************************************
*                                       RTOS INTERFACE FUNCTIONS
*                                          (see telnet-s_os.c)
*********************************************************************************************************
*/

CPU_BOOLEAN  TELNETs_OS_ServerTaskInit (void        *p_arg);    /* Perform TELNET OS init.                              */

CPU_BOOLEAN  TELNETs_OS_SessionTaskInit(void        *p_arg);    /* Create session task.                                 */

void         TELNETs_OS_TaskSuspend    (void);                  /* Suspend   current task.                              */
void         TELNETs_OS_TaskDelete     (void);                  /* Terminate current task.                              */

                                                                /* Delay     current task.                              */
CPU_BOOLEAN  TELNETs_OS_TimeDly        (CPU_INT08U   hours,
                                        CPU_INT08U   minutes,
                                        CPU_INT08U   seconds,
                                        CPU_INT08U   milli);


/*
*********************************************************************************************************
*                                              TRACING
*********************************************************************************************************
*/
                                                                /* Trace level, default to TRACE_LEVEL_OFF.             */
#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                                   0
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                                  1
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                                   2
#endif

#ifndef  TELNETs_TRACE_LEVEL
#define  TELNETs_TRACE_LEVEL                               TRACE_LEVEL_OFF
#endif

#ifndef  TELNETs_TRACE
#define  TELNETs_TRACE                                     printf
#endif

#if ((defined(TELNETs_TRACE))       && \
(defined(TELNETs_TRACE_LEVEL)) && \
(TELNETs_TRACE_LEVEL >= TRACE_LEVEL_INFO))

#if (TELNETs_TRACE_LEVEL >= TRACE_LEVEL_LOG)
#define  TELNETs_TRACE_LOG(msg)     TELNETs_TRACE  msg
#else
#define  TELNETs_TRACE_LOG(msg)
#endif


#if (TELNETs_TRACE_LEVEL >= TRACE_LEVEL_DBG)
#define  TELNETs_TRACE_DBG(msg)     TELNETs_TRACE  msg
#else
#define  TELNETs_TRACE_DBG(msg)
#endif

#define  TELNETs_TRACE_INFO(msg)        TELNETs_TRACE  msg

#else
#define  TELNETs_TRACE_LOG(msg)
#define  TELNETs_TRACE_DBG(msg)
#define  TELNETs_TRACE_INFO(msg)
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         CONFIGURATION ERRORS
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  TELNETs_CFG_PORT_SERVER
#error  "TELNETs_CFG_PORT_SERVER not #define'd in 'telnet-s_cfg.h' see template file in package named 'telnet-s_cfg.h'"
#endif


#ifndef  TELNETs_CFG_PORT_SERVER_SECURE
#error  "TELNETs_CFG_PORT_SERVER_SECURE  not #define'd in 'telnet-s_cfg.h' see template file in package named 'telnet-s_cfg.h'"
#endif


#ifndef  TELNETs_CFG_INACTIVITY_TIMEOUT_S
#error  "TELNETs_CFG_INACTIVITY_TIMEOUT_S not #define'd in 'telnet-s_cfg.h' see template file in package named 'telnet-s_cfg.h'"

#elif  ((TELNETs_CFG_INACTIVITY_TIMEOUT_S <                   1) || \
        (TELNETs_CFG_INACTIVITY_TIMEOUT_S > DEF_INT_16U_MAX_VAL))
#error  "TELNETs_CFG_INACTIVITY_TIMEOUT_S illegally #define'd in 'telnet-s_cfg.h' [MUST be >= 1 && <= 65535]"
#endif


#ifndef  TELNETs_CFG_MAX_TX_TRIES
#error  "TELNETs_CFG_MAX_TX_TRIES not #define'd in 'telnet-s_cfg.h' see template file in package named 'telnet-s_cfg.h'"

#elif  ((TELNETs_CFG_MAX_TX_TRIES <                   1) || \
        (TELNETs_CFG_MAX_TX_TRIES > DEF_INT_08U_MAX_VAL))
#error  "TELNETs_CFG_MAX_TX_TRIES illegally #define'd in 'telnet-s_cfg.h'[MUST be >= 1 && <= 255]"
#endif


#ifndef  TELNETs_CFG_RX_BUF_LEN
#error  "TELNETs_CFG_RX_BUF_LEN not #define'd in 'telnet-s_cfg.h' see template file in package named 'telnet-s_cfg.h'"

#elif  ( TELNETs_CFG_RX_BUF_LEN < 1)
#error  "TELNETs_CFG_RX_BUF_LEN illegally #define'd in 'telnet-s_cfg.h' [MUST be >=   1]"
#endif


#ifndef  TELNETs_CFG_NVT_BUF_LEN
#error  "TELNETs_CFG_NVT_BUF_LEN not #define'd in 'telnet-s_cfg.h' see template file in package named 'telnet-s_cfg.h'"

#elif  ( TELNETs_CFG_NVT_BUF_LEN < 1)
#error  "TELNETs_CFG_NVT_BUF_LEN illegally #define'd in 'telnet-s_cfg.h' [MUST be >= 1]"
#endif


#ifndef  TELNETs_CFG_MAX_LOGIN_TRIES
#error  "TELNETs_CFG_MAX_LOGIN_TRIES not #define'd in 'telnet-s_cfg.h' see template file in package named 'telnet-s_cfg.h'"

#elif  ((TELNETs_CFG_MAX_LOGIN_TRIES <                   0) || \
        (TELNETs_CFG_MAX_LOGIN_TRIES > DEF_INT_08U_MAX_VAL))
#error  "TELNETs_CFG_MAX_LOGIN_TRIES illegally #define'd in 'telnet-s_cfg.h' [MUST be >= 0 && <= 255]"
#endif


#ifndef  TELNETs_CFG_MAX_USR_NAME_LEN
#error  "TELNETs_CFG_MAX_USR_NAME_LEN not #define'd in 'telnet-s_cfg.h' see template file in package named 'telnet-s_cfg.h'"

#elif  ((TELNETs_CFG_MAX_USR_NAME_LEN <                   1) || \
        (TELNETs_CFG_MAX_USR_NAME_LEN > DEF_INT_08U_MAX_VAL))
#error  "TELNETs_CFG_MAX_USR_NAME_LEN illegally #define'd in 'telnet-s_cfg.h' [MUST be >= 1 && <= 255] "
#endif


#ifndef  TELNETs_CFG_MAX_PW_LEN
#error  "TELNETs_CFG_MAX_PW_LEN not #define'd in 'telnet-s_cfg.h' see template file in package named 'telnet-s_cfg.h'"

#elif  ((TELNETs_CFG_MAX_PW_LEN <                   1) || \
        (TELNETs_CFG_MAX_PW_LEN > DEF_INT_08U_MAX_VAL))
#error  "TELNETs_CFG_MAX_PW_LEN illegally #define'd in 'telnet-s_cfg.h' [MUST be >= 1 && <= 255]"
#endif


#ifndef  TELNETs_CFG_FS_EN
#error  "TELNETs_CFG_FS_EN not #define'd in 'telnet-s_cfg.h' see template file in package named 'telnet-s_cfg.h'"
#endif


#ifndef  TELNETs_CFG_WELCOME_MSG_STR
#error  "TELNETs_CFG_WELCOME_MSG_STR not #define'd in 'telnet-s_cfg.h' see template file in package named 'telnet-s_cfg.h'"
#endif


/*
*********************************************************************************************************
*                                    NETWORK CONFIGURATION ERRORS
*********************************************************************************************************
*/

#if     (NET_TCP_CFG_EN != DEF_ENABLED)
#error  "NET_TCP_CFG_EN illegally #define'd in 'net_cfg.h' [MUST be  DEF_ENABLED]"
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif                                                          /* End of TELNETs module include.                       */

