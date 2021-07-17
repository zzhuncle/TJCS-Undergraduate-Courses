/*
*********************************************************************************************************
*                                              uC/DHCPc
*                             Dynamic Host Configuration Protocol Client
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
*                                             DHCP CLIENT
*
* Filename : dhcp-c.h
* Version  : V2.10.02
*********************************************************************************************************
* Note(s)  : (1) Supports Dynamic Host Configuration Protocol as described in RFC #2131 with the
*                     following features/restrictions/constraints :
*
*                (a) Dynamic Configuration of IPv4 Link-Local Addresses       RFC #3927
*                (b) Supports both infinite & temporary address leases,
*                        with automatic renewal of lease if necessary
*
*            (2) To protect the validity & prevent the corruption of shared DHCP client resources,
*                the primary tasks of the DHCP client are prevented from running concurrently
*                through the use of a global DHCPc lock implementing protection by mutual exclusion.
*
*                (a) The mechanism of protected mutual exclusion is irrelevant but MUST be implemented
*                    in the following two functions :
*
*                        DHCPc_OS_Lock()                       acquire access to DHCP client
*                        DHCPc_OS_Unlock()                     release access to DHCP client
*
*                    implemented in
*
*                        \<DHCPc>\OS\<os>\dhcp-c_os.*
*
*                        where
*                                <DHCPc>                       directory path for DHCPc module
*                                <os>                          directory name for specific OS
*
*                (b) Since this global lock implements mutual exclusion at the DHCP client task
*                    level, critical sections are NOT required to prevent task-level concurrency in
*                    the DHCP client.
*
*            (3) Assumes the following versions (or more recent) of software modules are included in
*                the project build :
*
*                (a) uC/TCP-IP V2.13.02
*                (b) uC/CPU    V1.27
*                (c) uC/LIB    V1.35.00
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               DHCPc present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  DHCPc_PRESENT                                          /* See Note #1.                                         */
#define  DHCPc_PRESENT


/*
*********************************************************************************************************
*                                        DHCPc VERSION NUMBER
*
* Note(s) : (1) (a) The DHCPc module software version is denoted as follows :
*
*                       Vx.yy.zz
*
*                           where
*                                   V               denotes 'Version' label
*                                   x               denotes     major software version revision number
*                                   yy              denotes     minor software version revision number
*                                   zz              denotes sub-minor software version revision number
*
*               (b) The DHCPc software version label #define is formatted as follows :
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

#define  DHCPc_VERSION                                 21002u   /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   DHCPc_MODULE
#define  DHCPc_EXT
#else
#define  DHCPc_EXT  extern
#endif


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*
* Note(s) : (1) The DHCPc module files are located in the following directories :
*
*               (a) \<Your Product Application>\dhcp-c_cfg.h
*
*               (b) \<Network Protocol Suite>\Source\net_*.*
*
*               (c) (1) \<DHCPc>\Source\dhcp-c.h
*                                      \dhcp-c.c
*
*                   (2) \<DHCPc>\OS\<os>\dhcp-c_os.*
*
*                       where
*                               <Your Product Application>      directory path for Your Product's Application
*                               <Network Protocol Suite>        directory path for network protocol suite
*                               <DHCPc>                         directory path for DHCPc module
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
*               (a) '\<Your Product Application>\' directory                            See Note #1a
*
*               (b) '\<Network Protocol Suite>\'   directory                            See Note #1b
*
*               (c) '\<DHCPc>\' directory                                               See Note #1c
*
*               (d) (1) '\<CPU-Compiler Directory>\'                  directory         See Note #2a
*                   (2) '\<CPU-Compiler Directory>\<cpu>\<compiler>\' directory         See Note #2b
*
*               (e) '\<Custom Library Directory>\' directory                            See Note #3a
*********************************************************************************************************
*/

#include  <cpu_core.h>                                          /* CPU Core Library               (see Note #2a)        */

#include  <lib_def.h>                                           /* Standard        Defines        (see Note #3a)        */
#include  <lib_str.h>                                           /* Standard String Library        (see Note #3a)        */
#include  <lib_mem.h>                                           /* Standard Memory Library        (see Note #3a)        */

#include  <dhcp-c_cfg.h>                                        /* DHCPc Configuration File       (see Note #1a)        */

#include  <Source/net.h>                                        /* Network Protocol Suite         (see Note #1b)        */
#include  <Source/net_def.h>
#include  <Source/net_type.h>
#include  <net_cfg.h>

/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          DHCPc DEFAULT CFG
*
* Note(s) : (1) Configure DHCPc_CFG_BROADCAST_BIT_EN to DEF_ENABLED to instruct the contacted DHCP server
*               to use broadcast packets instead of unicast ones.  Used for TCP/IP stacks that cannot
*               receive unicast packets when not fully configured.  This is the case of the uC/TCPIP
*               stack, so this define MUST be set to DEF_ENABLED when this DHCP client is used with the
*               Micrium's uC/TCP-IP stack.
*********************************************************************************************************
*/

#ifndef  DHCPc_CFG_BROADCAST_BIT_EN
#define  DHCPc_CFG_BROADCAST_BIT_EN              DEF_ENABLED    /* Configure broadcast bit               (see Note #1) :*/
                                                                /*   DEF_DISABLED  Broadcast bit NOT set                */
                                                                /*   DEF_ENABLED   Broadcast bit     set                */
#endif


/*
*********************************************************************************************************
*                                      DHCPc ERROR CODE DEFINES
*********************************************************************************************************
*/

typedef  enum dhcp_c_err {

    DHCPc_ERR_NONE                                   =    0,

    DHCPc_ERR_NONE_NO_TMR                            =    1,
    DHCPc_ERR_NONE_LOCAL_LINK                        =    2,

    DHCPc_ERR_INIT_INCOMPLETE                        =   10,    /* DHCPc init NOT completed.                            */
    DHCPc_ERR_INIT_SOCK                              =   11,    /* Sock  init err.                                      */

    DHCPc_ERR_NULL_PTR                               =   15,    /* Ptr arg(s) passed NULL ptr(s).                       */
    DHCPc_ERR_INVALID_MSG_SIZE                       =   16,    /* Invalid DHCP msg size.                               */
    DHCPc_ERR_INVALID_HW_ADDR                        =   17,    /* Invalid/NO hw addr.                                  */
    DHCPc_ERR_INVALID_MSG                            =   18,    /* Invalid DHCP msg.                                    */
    DHCPc_ERR_OPT_BUF_SIZE                           =   19,    /* Opt val buf size too small.                          */
    DHCPc_ERR_PARAM_REQ_TBL_SIZE                     =   20,    /* Req param tbl size too small.                        */

    DHCPc_ERR_IF_CFG                                 =   21,    /* IF cfg err.                                          */
    DHCPc_ERR_IF_NOT_CFG                             =   22,    /* IF NOT yet cfg'd with DHCP.                          */
    DHCPc_ERR_IF_INVALID                             =   23,    /* IF invalid or disabled.                              */
    DHCPc_ERR_IF_NOT_MANAGED                         =   24,    /* IF NOT managed by DHCP client.                       */
    DHCPc_ERR_IF_OPT_NONE                            =   25,    /* DHCP opt NOT present on IF.                          */
    DHCPc_ERR_IF_CFG_STATE                           =   26,    /* IP cfg state err.                                    */

    DHCPc_ERR_IF_INFO_NONE_AVAIL                     =   30,    /* IF info  pool empty.                                 */
    DHCPc_ERR_IF_INFO_IF_USED                        =   31,    /* IF info  already already used for this IF.           */

    DHCPc_ERR_MSG_NONE_AVAIL                         =   40,    /* Msg      pool empty.                                 */
    DHCPc_ERR_MSG_Q                                  =   41,    /* Err posting to msg Q.                                */

    DHCPc_ERR_COMM_NONE_AVAIL                        =   50,    /* Comm obj pool empty.                                 */

    DHCPc_ERR_TMR_NONE_AVAIL                         =   60,    /* Tmr      pool empty.                                 */
    DHCPc_ERR_TMR_INVALID_MSG                        =   61,    /* Invalid tmr msg.                                     */
    DHCPc_ERR_TMR_CFG                                =   62,    /* Err cfg'ing tmr.                                     */

    DHCPc_ERR_ADDR_VALIDATE                          =   70,    /* Err validating addr.                                 */
    DHCPc_ERR_ADDR_USED                              =   71,    /* Addr in use.                                         */

    DHCPc_ERR_LOCAL_LINK                             =   75,    /* Err cfg'ing dyn link-local addr.                     */

    DHCPc_ERR_RX                                     =   80,    /* Rx err.                                              */
    DHCPc_ERR_RX_OVF                                 =   81,    /* Rx err data buffer ovf.                              */
    DHCPc_ERR_RX_NAK                                 =   82,    /* NAK msg rx'd.                                        */
    DHCPc_ERR_RX_MSG_TYPE                            =   83,    /* Rep msg type err.                                    */

    DHCPc_ERR_TX                                     =   90,    /* Tx err.                                              */

    DHCPc_ERR_IPv4_NOT_PRESENT                       =  100,    /* DHCPc requires NET_IP_CFG_IPv4_EN set to DEF_ENABLED */



    DHCPc_OS_ERR_NONE                                = 1000,

    DHCPc_OS_ERR_INIT                                = 1010,
    DHCPc_OS_ERR_CFG                                 = 1011,

    DHCPc_OS_ERR_INIT_SIGNAL                         = 1020,
    DHCPc_OS_ERR_INIT_SIGNAL_NAME                    = 1021,
    DHCPc_OS_ERR_INIT_SIGNALD                        = 1022,
    DHCPc_OS_ERR_INIT_LOCK                           = 1023,
    DHCPc_OS_ERR_INIT_LOCK_NAME                      = 1024,
    DHCPc_OS_ERR_INIT_Q                              = 1025,
    DHCPc_OS_ERR_INIT_Q_NAME                         = 1026,
    DHCPc_OS_ERR_INIT_TMR                            = 1027,
    DHCPc_OS_ERR_INIT_TMR_SIGNAL                     = 1028,
    DHCPc_OS_ERR_INIT_TMR_SIGNAL_NAME                = 1029,

    DHCPc_OS_ERR_INIT_TASK                           = 1040,
    DHCPc_OS_ERR_INIT_TASK_NAME                      = 1041,
    DHCPc_OS_ERR_INIT_TMR_TASK                       = 1042,
    DHCPc_OS_ERR_INIT_TMR_TASK_NAME                  = 1043,

    DHCPc_OS_ERR_LOCK                                = 1050,
    DHCPc_OS_ERR_MSG_Q                               = 1051,
    DHCPc_OS_ERR_TMR                                 = 1052,

} DHCPc_ERR;


/*
*********************************************************************************************************
*                                         TASK NUMBER DEFINE
*********************************************************************************************************
*/

#define  DHCPc_TASK_NBR                                     2   /* Total number of DHCPc task.                          */


/*
*********************************************************************************************************
*                                        DHCP MESSAGE DEFINES
*
* Note(s) : (1) RFC #2131, section 'Protocol Summary' states that "a DHCP client must be prepared to
*               receive a message of up to 576 octets".  Hense, the size of the DHCP message buffer is
*               fixed to this length.
*
*           (2) (a) RFC #2131, section 'Introduction' states that "the format of DHCP messages is based
*                   on the format of BOOTP messages, to capture the BOOTP relay agent behavior described
*                   as part of the BOOTP specification, and to allow interoperability of existing BOOTP
*                   clients with DHCP servers".
*
*                   Although not explicitly stated, DHCP messages should be at least 300 octets to
*                   preserve backward compatibility with BOOTP which does define this minimum size (some
*                   BOOTP relay agents have been known to drop packets of less than this length).
*
*               (b) However, NO control may be exerciced on received DHCP message, and in order to remain
*                   compatible with uncomplying DHCP server, smaller incomming DHCP message are NOT
*                   discarded, provided they are at least 244 bytes (the minimum DHCP message lenght
*                   uncluding mandatory "DHCP message type" option).
*
*               (c) Thus :
*
*                   (1) Transmitted packets will be padded so their length is at least 300 octets.
*
*                   (2) Received    packets under 244 octets will be discarded.
*
*           (3) Section 'The Client-Server Protocol' of this RFC stipulates that "the first four octets
*               of the 'options' field of the DHCP message contain the (decimal) values 99, 130, 83 and
*               99, respectively (this is the same magic cookie as is defined in RFC 1497)".
*
*           (4) See also 'DHCP MESSAGE DATA TYPE  Note #2'.
*
*           (5) DHCP operation codes are defined in RFC #2131, section 'Protocol Summary'.
*********************************************************************************************************
*/

#define  DHCP_MSG_BUF_SIZE                                576   /* Buf size                          (see Note #1).     */

#define  DHCP_MSG_TX_MIN_LEN                              300   /* Min tx'd msg len                  (see Note #2).     */
#define  DHCP_MSG_RX_MIN_LEN                              244   /* Min rx'd msg len                  (see Note #2).     */

#define  DHCP_MAGIC_COOKIE                         0x63825363   /* Magic cookie                      (see Note #3).     */
#define  DHCP_MAGIC_COOKIE_SIZE                             4   /* Size of magic cookie (in octets).                    */

#define  DHCP_FLAG_BROADCAST                         (1 << 15)  /* Broadcast flag                    (see Note #4).     */

                                                                /* Operation codes                   (see Note #5).     */
#define  DHCP_OP_REQUEST                                    1
#define  DHCP_OP_REPLY                                      2

#define  DHCP_HTYPE_ETHER                                   1   /* Ethernet hardware address type.                      */


/*
*********************************************************************************************************
*                                      DHCP MESSAGE TYPE DEFINES
*
* Note(s) : (1) DHCP message types are defined in RFC #2132, section 'DHCP Extensions, DHCP Message
*               Type'.
*
*           (2) DHCP_MSG_NONE is not defined in RFC #2132, and is intended to be used internally only.
*********************************************************************************************************
*/

#define  DHCP_MSG_NONE                                     0    /* See note #2.                                         */

#define  DHCP_MSG_DISCOVER                                 1
#define  DHCP_MSG_OFFER                                    2
#define  DHCP_MSG_REQUEST                                  3
#define  DHCP_MSG_DECLINE                                  4
#define  DHCP_MSG_ACK                                      5
#define  DHCP_MSG_NAK                                      6
#define  DHCP_MSG_RELEASE                                  7
#define  DHCP_MSG_INFORM                                   8


/*
*********************************************************************************************************
*                                 DHCP COMMUNICATION MESSAGE DEFINES
*
* Note(s) : (1) The worse case for the communication message queue is 5 messages being posted to the
*               queue waiting to be consumed; this is hence the maximum queue size per interface.
*********************************************************************************************************
*/

#define  DHCPc_COMM_MSG_NONE                               0

#define  DHCPc_COMM_MSG_START                              1
#define  DHCPc_COMM_MSG_STOP                               2
#define  DHCPc_COMM_MSG_T1_EXPIRED                         3
#define  DHCPc_COMM_MSG_T2_EXPIRED                         4
#define  DHCPc_COMM_MSG_LEASE_EXPIRED                      5


#define  DHCPc_COMM_MSG_MAX_NBR                            5    /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                      DHCP CLIENT STATE DEFINES
*
* Note(s) : (1) The DHCP states are depicted in RFC #2131, figure 5 'State-transition diagram for DHCP
*               clients'.
*
*           (2) DHCP_STATE_NONE, DHCP_STATE_LOCAL_LINK, & DHCP_STATE_STOPPING are not defined in RFC #2132,
*               and are intended to be used internally only.
*********************************************************************************************************
*/

#define  DHCP_STATE_NONE                                   0    /* See note #2.                                         */

#define  DHCP_STATE_INIT                                   1
#define  DHCP_STATE_SELECTING                              2
#define  DHCP_STATE_REQUESTING                             3
#define  DHCP_STATE_BOUND                                  4
#define  DHCP_STATE_RENEWING                               5
#define  DHCP_STATE_REBINDING                              6
#define  DHCP_STATE_REBOOTING                              7
#define  DHCP_STATE_INIT_REBOOT                            8

#define  DHCP_STATE_LOCAL_LINK                             9    /* See note #2.                                         */

#define  DHCP_STATE_STOPPING                              10    /* See note #2.                                         */


/*
*********************************************************************************************************
*                                 DHCP INTERFACE LEASE STATUS DEFINES
*********************************************************************************************************
*/

#define  DHCP_STATUS_NONE                                  0

#define  DHCP_STATUS_CFG_IN_PROGRESS                       1
#define  DHCP_STATUS_CFGD                                  2
#define  DHCP_STATUS_CFGD_NO_TMR                           3
#define  DHCP_STATUS_CFGD_LOCAL_LINK                       4

#define  DHCP_STATUS_FAIL                                  5


/*
*********************************************************************************************************
*                                         DHCP OPTION DEFINES
*
* Note(s) : (1) This section defines the DHCP Options and BOOTP Vendor Extensions, as defined in
*               RFC #2132.  This list is not necessarily exhaustive; please refer to the Internet
*               Assigned Numbers Authority (www.iana.org) for the complete list.
*********************************************************************************************************
*/

#define  DHCP_OPT_FIELD_CODE_LEN                                   1
#define  DHCP_OPT_FIELD_LEN_LEN                                    1
#define  DHCP_OPT_FIELD_HDR_LEN                     (DHCP_OPT_FIELD_CODE_LEN + DHCP_OPT_FIELD_LEN_LEN)

                                                                /* OPTION NUMBER  OPTION SIZE (payload only)            */
#define  DHCP_OPT_PAD                                              0    /*  0                                           */

                                                                /* RFC 1497 Vendor Extensions                           */
#define  DHCP_OPT_SUBNET_MASK                                      1    /*  4                                           */
#define  DHCP_OPT_TIME_OFFSET                                      2    /*  4                                           */
#define  DHCP_OPT_ROUTER                                           3    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_TIME_SERVER                                      4    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_NAME_SERVER                                      5    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_DOMAIN_NAME_SERVER                               6    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_LOG_SERVER                                       7    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_COOKIE_SERVER                                    8    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_LPR_SERVER                                       9    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_IMPRESS_SERVER                                  10    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_RESSOURCE_LOCATION_SERVER                       11    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_HOST_NAME                                       12    /*  n,     1 <= n <= 255                        */
#define  DHCP_OPT_BOOT_FILE_SIZE                                  13    /*  2                                           */
#define  DHCP_OPT_MERIT_DUMP_FILE                                 14    /*  n,     1 <= n <= 255                        */
#define  DHCP_OPT_DOMAIN_NAME                                     15    /*  n,     1 <= n <= 255                        */
#define  DHCP_OPT_SWAP_SERVER                                     16    /*  n,     1 <= n <= 255                        */
#define  DHCP_OPT_ROOT_PATH                                       17    /*  n,     1 <= n <= 255                        */
#define  DHCP_OPT_EXTENSION_PATH                                  18    /*  n,     1 <= n <= 255                        */

                                                                /* IP Layer Parameters per Host                         */
#define  DHCP_OPT_IP_FORWARDING                                   19    /*  1 (value = 0/1)                             */
#define  DHCP_OPT_NON_LOCAL_SOURCE_ROUTING                        20    /*  1 (value = 0/1)                             */
#define  DHCP_OPT_POLICY_FILTER                                   21    /*  n * 8, 1 <= n <= 255                        */
#define  DHCP_OPT_MAXIMUM_DATAGRAM_REASSEMBLY_SIZE                22    /*  2                                           */
#define  DHCP_OPT_DEFAULT_IP_TIME_TO_LIVE                         23    /*  1                                           */
#define  DHCP_OPT_PATH_MTU_AGING_TIMEOUT                          24    /*  4                                           */
#define  DHCP_OPT_PATH_MTU_PLATEAU_TABLE                          25    /*  n * 2, 1 <= n <= 255                        */

                                                                /* IP Layer Parameters per Interface                    */
#define  DHCP_OPT_INTERFACE_MTU                                   26    /*  2                                           */
#define  DHCP_OPT_ALL_SUBNETS_ARE_LOCAL                           27    /*  1 (value = 0/1)                             */
#define  DHCP_OPT_BROADCAST_ADDRESS                               28    /*  4                                           */
#define  DHCP_OPT_PERFORM_MASK_DISCOVERY                          29    /*  1 (value = 0/1)                             */
#define  DHCP_OPT_MASK_SUPPLIER                                   30    /*  1 (value = 0/1)                             */
#define  DHCP_OPT_PERFORM_ROUTER_DISCOVERY                        31    /*  1 (value = 0/1)                             */
#define  DHCP_OPT_ROUTER_SOLLICITATION_ADDRESS                    32    /*  4                                           */
#define  DHCP_OPT_STATIC_ROUTE                                    33    /*  n * 8, 1 <= n <= 255                        */

                                                                /* Link Layer Parameters per Interface                  */
#define  DHCP_OPT_TRAILER_ENCAPSULATION                           34    /*  1 (value = 0/1)                             */
#define  DHCP_OPT_ARP_CACHE_TIMEOUT                               35    /*  4                                           */
#define  DHCP_OPT_ETHERNET_ENCAPSULATION                          36    /*  1 (value = 0/1)                             */

                                                                /* TCP Parameters                                       */
#define  DHCP_OPT_TCP_DEFAULT_TTL                                 37    /*  1                                           */
#define  DHCP_OPT_TCP_KEEPALIVE_INTERVAL                          38    /*  4                                           */
#define  DHCP_OPT_TCP_KEEPALIVE_GARBAGE                           39    /*  1 (value = 0/1)                             */

                                                                /* Application and Service Parameters                   */
#define  DHCP_OPT_NETWORK_INFORMATION_SERVICE_DOMAIN              40    /*  n,     1 <= n <= 255                        */
#define  DHCP_OPT_NETWORK_INFORMATION_SERVER                      41    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_NETWORK_TIME_PROTOCOL_SERVER                    42    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_VENDOR_SPECIFIC_INFORMATION                     43    /*  n,     1 <= n <= 255                        */
#define  DHCP_OPT_NETBIOS_OVER_TCPIP_NAME_SERVER                  44    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_NETBIOS_OVER_TCPIP_DATAGRAM_DISTRIBUTION_SERVER 45    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_NETBIOS_OVER_TCPIP_NODE_TYPE                    46    /*  1                                           */
#define  DHCP_OPT_NETBIOS_OVER_TCPIP_SCOPE                        47    /*  n,     1 <= n <= 255                        */
#define  DHCP_OPT_X_WINDOW_SYSTEM_FONT_SERVER                     48    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_X_WINDOW_SYSTEM_DISPLAY_MANAGER                 49    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_NETWORK_INFORMATION_SERVICE_PLUS_DOMAIN         64    /*  n,     1 <= n <= 255                        */
#define  DHCP_OPT_NETWORK_INFORMATION_SERVICE_PLUS_SERVER         65    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_MOBILE_IP_HOME_AGENT                            68    /*  n * 4, 0 <= n <= 255                        */
#define  DHCP_OPT_SIMPLE_MAIL_TRANSPORT_PROTOCOL_SERVER           69    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_POST_OFFICE_PROTOCOL_SERVER                     70    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_NETWORK_NEWS_TRANSPORT_PROTOCOL_SERVER          71    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_DEFAULT_WORLD_WIDE_WEB_SERVER                   72    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_DEFAULT_FINGER_SERVER                           73    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_DEFAULT_INTERNET_RELAY_CHAT_SERVER              74    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_STREETTALK_SERVER                               75    /*  n * 4, 1 <= n <= 255                        */
#define  DHCP_OPT_STREETTALK_DIRECTORY_ASSISTANCE_SERVER          76    /*  n * 4, 1 <= n <= 255                        */

                                                                /* DHCP Extensions                                      */
#define  DHCP_OPT_REQUESTED_IP_ADDRESS                            50    /*  4                                           */
#define  DHCP_OPT_IP_ADDRESS_LEASE_TIME                           51    /*  4                                           */
#define  DHCP_OPT_OPTION_OVERLOAD                                 52    /*  1 (value = 1/2/3)                           */
                                                                        /*    (See 'DHCP MESSAGE TYPE DEFINES')         */
#define  DHCP_OPT_DHCP_MESSAGE_TYPE                               53    /*  1 (value = 1-9)                             */
#define  DHCP_OPT_SERVER_IDENTIFIER                               54    /*  4                                           */
#define  DHCP_OPT_PARAMETER_REQUEST_LIST                          55    /*  n,     1 <= n <= 255                        */
#define  DHCP_OPT_ERROR_MESSAGE                                   56    /*  n,     1 <= n <= 255                        */
#define  DHCP_OPT_MAXIMUM_DHCP_MESSAGE_SIZE                       57    /*  2                                           */
#define  DHCP_OPT_RENEWAL_TIME_VALUE                              58    /*  4                                           */
#define  DHCP_OPT_REBINDING_TIME_VALUE                            59    /*  4                                           */
#define  DHCP_OPT_VENDOR_CLASS_IDENTIFIER                         60    /*  n,     1 <= n <= 255                        */
#define  DHCP_OPT_CLIENT_IDENTIFIER                               61    /*  n,     2 <= n <= 255                        */
#define  DHCP_OPT_TFTP_SERVER_NAME                                66    /*  n,     1 <= n <= 255                        */
#define  DHCP_OPT_BOOTFILE_NAME                                   67    /*  n,     1 <= n <= 255                        */

#define  DHCP_OPT_END                                            255    /*  0                                           */


/*
*********************************************************************************************************
*                                          DHCP TIME DEFINES
*
* Note(s) : (1) Minimum retransmitting delay between attempt when rebinding or renewing the lease.
*
*               See also 'DHCPc_LeaseTimeUpdate()  Note #2'.
*
*           (2) Delay between retried lease negotiation.
*
*               See also 'DHCPc_InitStateHandler()  Note #4'.
*
*           (3) When a RELEASE message is transmited, it is possible that the target has to resolve
*               the destination hardware address through ARP.  We hence have to introduce a slight delay
*               to allow that address to be resolved and for the RELEASE message to be sent out before
*               the interface network layer is un-configured.
*
*               Not doing so would result in the message to never to transmited since an un-configured
*               interface does NOT resolve pending ARP caches.
*********************************************************************************************************
*/

#define  DHCP_LEASE_INFINITE                       0xFFFFFFFF   /* Permanent lease time.                                */

#define  DHCP_T1_LEASE_FRACTION                         0.5     /* T1 dflt fraction.                                    */
#define  DHCP_T2_LEASE_FRACTION                         0.875   /* T2 dflt fraction.                                    */

#define  DHCP_MIN_RETX_TIME_S                           300     /* Min re-tx time (see Note #1).                        */

#define  DHCP_INIT_DLY_MS                             10000     /* Init       dly (see Note #2).                        */

                                                                /* Release    dly (see Note #3).                        */
#define  DHCP_RELEASE_DLY_S                         NET_ARP_REQ_RETRY_TIMEOUT_DFLT_SEC


/*
*********************************************************************************************************
*                                         DHCP TIMER DEFINES
*
* Note(s) : (1) DHCP client time tick for timer(s) associated to address lease(s).
*********************************************************************************************************
*/

#define  DHCPc_TMR_PERIOD_SEC                               5   /* Period of DHCPc tmr in sec (see Note #1).            */


/*
*********************************************************************************************************
*                   DHCPc TIMER, INTERFACE INFORMATION, & MESSAGE QUANTITY DEFINES
*
* Note(s) : (1) Define the number of timers & interface information as the total number of interfaces.
*
*           (2) Define the number of message buffer as the total number of interfaces information plus
*               one.  Since the interface information structure holds the last OFFER or ACK message
*               packet, one extra buffer is needed to be able to craft a message intended to be
*               transmitted.
*
*           (3) Define the number of communication object as twice the total number of interfaces
*               configured to use DHCP.  Since communication objects are used by both DHCPc timers &
*               by the start and stop function, it is possible that DHCPc_CFG_MAX_NBR_IF number of
*               communication objects be used by timers while the application tries to stop
*               those DHCPc_CFG_MAX_NBR_IF interfaces at the same time.
*********************************************************************************************************
*/

                                                                /* See Note #1.                                         */
#define  DHCPc_NBR_TMR                      DHCPc_CFG_MAX_NBR_IF
#define  DHCPc_NBR_IF_INFO                  DHCPc_CFG_MAX_NBR_IF

                                                                /* See Note #2.                                         */
#define  DHCPc_NBR_MSG_BUF                 (DHCPc_CFG_MAX_NBR_IF + 1)

                                                                /* See Note #3.                                         */
#define  DHCPc_NBR_COMM                    (DHCPc_CFG_MAX_NBR_IF * 2)


/*
*********************************************************************************************************
*                                         DHCPc FLAG DEFINES
*********************************************************************************************************
*/

                                                                /* ------------------- DHCPc FLAGS -------------------- */
#define  DHCPc_FLAG_NONE                         DEF_BIT_NONE
#define  DHCPc_FLAG_USED                           DEF_BIT_00   /* Obj cur used; i.e. NOT in free pool.                 */


/*
*********************************************************************************************************
*                                  DHCPc TIME DELAY & RETRY DEFINES
*********************************************************************************************************
*/

#define  DHCPc_BACKOFF_DLY_INITIAL_MS                    2000   /* Initial dly                  (in ms).                */
#define  DHCPc_BACKOFF_DLY_MAX_MS                       64000   /* Max exponential back-off dly (in ms).                */
#define  DHCPc_BACKOFF_DLY_SCALAR                           2   /* Exponential back-off dly scalar.                     */

#define  DHCPc_TX_MAX_RETRY                                 3   /* Max nbr of  tx  retry   when transitory err.         */
#define  DHCPc_RX_MAX_RETRY                                 3   /* Max nbr of  rx  retry   when transitory err.         */
#define  DHCPc_TX_TIME_DLY_MS                             500   /* Dly between tx  retries when transitory err, in ms.  */
#define  DHCPc_RX_TIME_DLY_MS                             500   /* Dly between rx  retries when transitory err, in ms.  */

#define  DHCP_ADDR_VALIDATE_WAIT_TIME_MS                 3000   /* ARP reply wait time for addr validation.             */


/*
*********************************************************************************************************
*                                         LINK-LOCAL ADDRESSES DEFINES
*********************************************************************************************************
*/

#define  DHCP_LOCAL_LINK_PROBE_WAIT_S                    1
#define  DHCP_LOCAL_LINK_PROBE_NUM                       3
#define  DHCP_LOCAL_LINK_PROBE_MIN_S                     1
#define  DHCP_LOCAL_LINK_PROBE_MAX_S                     2
#define  DHCP_LOCAL_LINK_ANNOUNCE_WAIT_S                 2
#define  DHCP_LOCAL_LINK_ANNOUNCE_NUM                    2
#define  DHCP_LOCAL_LINK_ANNOUNCE_INTERVAL_S             2
#define  DHCP_LOCAL_LINK_MAX_CONFLICTS                  10
#define  DHCP_LOCAL_LINK_RATE_LIMIT_INTERVAL_S          60
#define  DHCP_LOCAL_LINK_DEFEND_INTERVAL_S              10

#define  DHCP_LOCAL_LINK_DHCP_LOOKUP_S                 300
#define  DHCP_LOCAL_LINK_ARP_CONFLICT_POOL_S            10


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     DHCPc ERROR CODES DATA TYPE
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                  DHCPc TIMER, INTERFACE INFORMATION, & MESSAGE QUANTITY DATA TYPES
*********************************************************************************************************
*/

typedef  CPU_INT16U  DHCPc_IF_INFO_QTY;                         /* Defines max qty of       IF info to support.         */
#define  DHCPc_IF_INFO_NBR_MIN                            1
#define  DHCPc_IF_INFO_NBR_MAX          DEF_INT_16U_MAX_VAL

typedef  CPU_INT16U  DHCPc_MSG_QTY;                             /* Defines max qty of       msg     to support.         */
#define  DHCPc_MSG_NBR_MIN                                1
#define  DHCPc_MSG_NBR_MAX              DEF_INT_16U_MAX_VAL

typedef  CPU_INT16U  DHCPc_COMM_QTY;                            /* Defines max qty of DHCPc comm    to support.         */
#define  DHCPc_COMM_NBR_MIN                               1
#define  DHCPc_COMM_NBR_MAX             DEF_INT_16U_MAX_VAL

typedef  CPU_INT16U  DHCPc_TMR_QTY;                             /* Defines max qty of DHCPc tmrs    to support.         */
#define  DHCPc_TMR_NBR_MIN                                1
#define  DHCPc_TMR_NBR_MAX              DEF_INT_16U_MAX_VAL


/*
*********************************************************************************************************
*                                        DHCPc TIMER DATA TYPE
*********************************************************************************************************
*/

typedef  CPU_INT32U  DHCPc_TMR_TICK;


/*
*********************************************************************************************************
*                                     DHCP MESSAGE TYPE DATA TYPE
*
* Note(s) : (1) See section 'DHCP MESSAGE TYPE DEFINES' for possible values.
*********************************************************************************************************
*/

typedef  CPU_INT08U  DHCPc_MSG_TYPE;


/*
*********************************************************************************************************
*                                DHCP COMMUNICATION MESSAGE DATA TYPE
*
* Note(s) : (1) See section 'DHCP COMMUNICATION MESSAGE DEFINES' for possible values.
*********************************************************************************************************
*/

typedef  CPU_INT08U  DHCPc_COMM_MSG;


/*
*********************************************************************************************************
*                                     DHCP CLIENT STATE DATA TYPE
*
* Note(s) : (1) See section 'DHCP CLIENT STATE DEFINES' for possible values.
*********************************************************************************************************
*/

typedef  CPU_INT08U  DHCPc_STATE;


/*
*********************************************************************************************************
*                                DHCP INTERFACE LEASE STATUS DATA TYPE
*
* Note(s) : (1) See section 'DHCP IF LEASE STATUS DEFINES' for possible values.
*********************************************************************************************************
*/

typedef  CPU_INT08U  DHCPc_STATUS;


/*
*********************************************************************************************************
*                                 DHCP MESSAGE OPTION CODE DATA TYPE
*
* Note(s) : (1) See section 'DHCP OPTION DEFINES' for possible values.
*********************************************************************************************************
*/

typedef  CPU_INT08U  DHCPc_OPT_CODE;


/*
*********************************************************************************************************
*                                       DHCP MESSAGE DATA TYPE
*
* Note(s) : (1) DHCP message format is defined in RFC #2131, section 2 'Protocol Summary'.
*
*           (2) RFC #2131, section 2 'Protocol Summary' states that "DHCP servers [...] may not be able
*               to deliver DHCP messages to clients that cannot accept hardware unicast datagrams before
*               the TCP/IP software is configured."
*
*              "To work around [that], DHCP uses the 'flags' field.  The leftmost bit is defined as the
*               BRAODCAST (B) flag. [...] The remaining bits of the flags field are reserved for future
*               use [and] they MUST be set to zero by clients."
*
*                   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
*                   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*                   |B|             MBZ             |
*                   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*
*                   B:  BROADCAST flag
*
*                   MBZ:  MUST BE ZERO (reserved for future use)
*
*           (3) The DHCP message 'header' is immediately followed by the variable length 'option' field.
*********************************************************************************************************
*/

                                                                /* ------------------- DHCP MSG HDR ------------------- */
typedef  struct dhcp_msg_hdr {
    CPU_INT08U  op;                                             /* Op code.                                             */
    CPU_INT08U  htype;                                          /* HW addr type.                                        */
    CPU_INT08U  hlen;                                           /* HW addr len.                                         */
    CPU_INT08U  hops;
    CPU_INT32U  xid;                                            /* Transaction ID.                                      */
    CPU_INT16U  secs;                                           /* Sec elapsed since beginning of addr acquisition.     */
    CPU_INT16U  flags;                                          /* See Note #2.                                         */
    CPU_INT32U  ciaddr;                                         /* Client IP addr set   by client.                      */
    CPU_INT32U  yiaddr;                                         /* Client IP addr rtn'd by server.                      */
    CPU_INT32U  siaddr;
    CPU_INT32U  giaddr;
    CPU_INT08U  chaddr[16];                                     /* Client HW addr.                                      */
    CPU_INT08U  sname[64];
    CPU_INT08U  file[128];
                                                                /* DHCP Opt's (see Note #3).                            */
} DHCP_MSG_HDR;

#define  DHCP_MSG_HDR_SIZE                    (sizeof(DHCP_MSG_HDR))
#define  DHCPc_MSG_OPT_SIZE  (DHCP_PKT_BUF_SIZE - DHCP_MSG_HDR_SIZE)


/*
*********************************************************************************************************
*                                      DHCPc INTERNAL DATA TYPES
*
* Note(s) : (1) Those type definitions are put here to make sure they are declared before being inserted
*               in one another.
*********************************************************************************************************
*/

typedef  struct  dhcpc_msg      DHCPc_MSG;
typedef  struct  dhcpc_if_info  DHCPc_IF_INFO;
typedef  struct  dhcpc_comm     DHCPc_COMM;
typedef  struct  dhcpc_tmr      DHCPc_TMR;


/*
*********************************************************************************************************
*                                       DHCPc MESSAGE DATA TYPE
*********************************************************************************************************
*/

struct dhcpc_msg {
    DHCPc_MSG      *PrevPtr;                                    /* Ptr to PREV msg.                                     */
    DHCPc_MSG      *NextPtr;                                    /* Ptr to NEXT msg.                                     */

    DHCPc_MSG_QTY   ID;                                         /* Msg id.                                              */

    CPU_INT08U      MsgBuf[DHCP_MSG_BUF_SIZE];                  /* Buf for DHCP msg.                                    */

    CPU_INT16U      MsgLen;                                     /* Tot len of msg (hdr + opt).                          */

    CPU_INT16U      Flags;                                      /* Msg flags.                                           */
};


/*
*********************************************************************************************************
*                                DHCPc INTERFACE INFORMATION DATA TYPE
*********************************************************************************************************
*/

struct dhcpc_if_info {
    DHCPc_IF_INFO      *PrevPtr;                                /* Ptr to PREV IF INFO.                                 */
    DHCPc_IF_INFO      *NextPtr;                                /* Ptr to NEXT IF INFO.                                 */

    DHCPc_IF_INFO_QTY   ID;                                     /* IF info id.                                          */
    NET_IF_NBR          IF_Nbr;                                 /* IF nbr for this IF info.                             */

    NET_IPv4_ADDR       ServerID;                               /* Server responsible for lease (in net order).         */

                                                                /* Param req tbl.                                       */
    DHCPc_OPT_CODE      ParamReqTbl[DHCPc_CFG_PARAM_REQ_TBL_SIZE];
    CPU_INT08U          ParamReqQty;                            /* Param req qty.                                       */

    DHCPc_MSG          *MsgPtr;                                 /* Ptr to DHCP msg.                                     */

    DHCPc_STATE         ClientState;                            /* DHCP client state.                                   */
    DHCPc_STATUS        LeaseStatus;                            /* Status of DHCP lease for this IF.                    */
    DHCPc_ERR           LastErr;                                /* Last DHCP error, set only when lease failed.         */

    CPU_INT32U          TransactionID;

    CPU_INT32U          NegoStartTime;                          /* Nego start     time.                                 */
    CPU_INT32U          TmrExpirationTime;                      /* Tmr expiration time.                                 */

    CPU_INT32U          LeaseTime_sec;
    CPU_INT32U          T1_Time_sec;
    CPU_INT32U          T2_Time_sec;

    DHCPc_TMR          *Tmr;                                    /* Ptr to DHCP tmr.                                     */

    CPU_INT16U          Flags;                                  /* IF info flags.                                       */
};


/*
*********************************************************************************************************
*                                    DHCPc COMMUNICATION DATA TYPE
*********************************************************************************************************
*/

struct  dhcpc_comm {
    DHCPc_COMM      *PrevPtr;                                   /* Ptr to PREV comm.                                    */
    DHCPc_COMM      *NextPtr;                                   /* Ptr to NEXT comm.                                    */

    DHCPc_COMM_QTY   ID;                                        /* Comm id.                                             */

    NET_IF_NBR       IF_Nbr;                                    /* IF nbr for this comm.                                */
    DHCPc_COMM_MSG   CommMsg;                                   /* Msg    for this comm.                                */

    CPU_INT16U       Flags;                                     /* Comm flags.                                          */
};


/*
*********************************************************************************************************
*                                        DHCPc TIMER DATA TYPE
*********************************************************************************************************
*/

struct  dhcpc_tmr {
    DHCPc_TMR       *PrevPtr;                                   /* Ptr to PREV tmr.                                     */
    DHCPc_TMR       *NextPtr;                                   /* Ptr to NEXT tmr.                                     */

    DHCPc_TMR_QTY    ID;                                        /* Tmr id.                                              */

    void            *Obj;                                       /* Ptr to obj using tmr.                                */
    DHCPc_TMR_TICK   TmrVal;                                    /* Cur tmr val (in DHCPc_TMR_TICK ticks).               */

    CPU_INT16U       Flags;                                     /* Tmr flags.                                           */
};


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

DHCPc_EXT  CPU_BOOLEAN     DHCPc_InitDone;                      /* Indicates when DHCPc initialization is complete.     */

DHCPc_EXT  DHCPc_IF_INFO   DHCPc_InfoTbl[DHCPc_NBR_IF_INFO];
DHCPc_EXT  DHCPc_IF_INFO  *DHCPc_InfoPoolPtr;                   /* Ptr to pool of free DHCPc info.                      */
DHCPc_EXT  DHCPc_IF_INFO  *DHCPc_InfoListHead;                  /* Ptr to head of Info List.                            */

DHCPc_EXT  DHCPc_MSG       DHCPc_MsgTbl[DHCPc_NBR_MSG_BUF];
DHCPc_EXT  DHCPc_MSG      *DHCPc_MsgPoolPtr;                    /* Ptr to pool of free DHCPc msg.                       */
DHCPc_EXT  DHCPc_MSG      *DHCPc_MsgListHead;                   /* Ptr to head of Msg List.                             */

DHCPc_EXT  DHCPc_COMM      DHCPc_CommTbl[DHCPc_NBR_COMM];
DHCPc_EXT  DHCPc_COMM     *DHCPc_CommPoolPtr;                   /* Ptr to pool of free DHCPc comm.                      */
DHCPc_EXT  DHCPc_COMM     *DHCPc_CommListHead;                  /* Ptr to head of Comm List.                            */

DHCPc_EXT  DHCPc_TMR       DHCPc_TmrTbl[DHCPc_NBR_TMR];
DHCPc_EXT  DHCPc_TMR      *DHCPc_TmrPoolPtr;                    /* Ptr to pool of free DHCPc tmrs.                      */
DHCPc_EXT  DHCPc_TMR      *DHCPc_TmrListHead;                   /* Ptr to head of Tmr List.                             */


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

DHCPc_ERR      DHCPc_Init         (void);                       /* DHCPc initialization function.                       */

                                                                /* Start DHCP service for specified interface.          */
void           DHCPc_Start        (NET_IF_NBR       if_nbr,
                                   DHCPc_OPT_CODE  *preq_param_tbl,
                                   CPU_INT08U       req_param_tbl_qty,
                                   DHCPc_ERR       *perr);

                                                                /* Stop  DHCP service for specified interface.          */
void           DHCPc_Stop         (NET_IF_NBR       if_nbr,
                                   DHCPc_ERR       *perr);

                                                                /* Check an interface's DHCP status & last error.       */
DHCPc_STATUS  DHCPc_ChkStatus     (NET_IF_NBR       if_nbr,
                                   DHCPc_ERR       *perr_last);

                                                                /* Get value for a given DHCP option.                   */
void          DHCPc_GetOptVal     (NET_IF_NBR       if_nbr,
                                   DHCPc_OPT_CODE   opt_code,
                                   CPU_INT08U      *pval_buf,
                                   CPU_INT16U      *pval_buf_len,
                                   DHCPc_ERR       *perr);


void          DHCPc_TmrTaskHandler(void);

void          DHCPc_TaskHandler   (void);


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                      RTOS INTERFACE FUNCTIONS
*                                  (see specific OS'S  dhcp-c_os.c)
*********************************************************************************************************
*/

void         DHCPc_OS_Init               (DHCPc_ERR   *perr);       /* Create         DHCPc Objects.                    */

void         DHCPc_OS_InitWait           (DHCPc_ERR   *perr);       /* Wait  until DHCPc initialization is complete.    */

void         DHCPc_OS_InitSignal         (DHCPc_ERR   *perr);       /* Signal that DHCPc initialization is complete.    */


void         DHCPc_OS_Lock               (DHCPc_ERR   *perr);       /* Acquire access to DHCP client.                   */

void         DHCPc_OS_Unlock             (void);                    /* Release access to DHCP client.                   */


void         DHCPc_OS_TaskInit           (DHCPc_ERR   *perr);       /* Create & start DHCPc Task.                       */


void        *DHCPc_OS_MsgWait            (DHCPc_ERR   *perr);       /* Wait for DHCP message.                           */

void         DHCPc_OS_MsgPost            (void        *pmsg,        /* Post     DHCP message.                           */
                                          DHCPc_ERR   *perr);


void         DHCPc_OS_TmrInit            (DHCPc_ERR   *perr);       /* Create      DHCPc Timer.                         */

void         DHCPc_OS_TmrStart           (DHCPc_ERR   *perr);       /* Start       DHCPc Timer.                         */

void         DHCPc_OS_TmrWait            (DHCPc_ERR   *perr);       /* Wait  until DHCPc Timer expires.                 */

void         DHCPc_OS_TmrSignal          (void);                    /* Signal that DHCPc Timer expired.                 */


CPU_INT32U   DHCPc_OS_TimeGet_tick       (void);                    /* Get       current time (in ticks).               */

CPU_INT32U   DHCPc_OS_TimeCalcElapsed_sec(CPU_INT32U   time_start,  /* Calculate elapsed time (in seconds).             */
                                          CPU_INT32U   time_stop);


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

#ifndef  DHCPc_CFG_IP_PORT_SERVER
#error  "DHCPc_CFG_IP_PORT_SERVER                not #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >= NET_PORT_NBR_MIN]       "
#error  "                                  [     &&  <= NET_PORT_NBR_MAX]       "

#elif   (DEF_CHK_VAL(DHCPc_CFG_IP_PORT_SERVER,  \
                     NET_PORT_NBR_MIN,          \
                     NET_PORT_NBR_MAX) != DEF_OK)
#error  "DHCPc_CFG_IP_PORT_SERVER          illegally #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >= NET_PORT_NBR_MIN]       "
#error  "                                  [     &&  <= NET_PORT_NBR_MAX]       "
#endif


#ifndef  DHCPc_CFG_IP_PORT_CLIENT
#error  "DHCPc_CFG_IP_PORT_CLIENT                not #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >= NET_PORT_NBR_MIN]       "
#error  "                                  [     &&  <= NET_PORT_NBR_MAX]       "

#elif   (DEF_CHK_VAL(DHCPc_CFG_IP_PORT_CLIENT,  \
                     NET_PORT_NBR_MIN,          \
                     NET_PORT_NBR_MAX) != DEF_OK)
#error  "DHCPc_CFG_IP_PORT_CLIENT          illegally #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >= NET_PORT_NBR_MIN]       "
#error  "                                  [     &&  <= NET_PORT_NBR_MAX]       "
#endif



#ifndef  DHCPc_CFG_MAX_RX_TIMEOUT_MS
#error  "DHCPc_CFG_MAX_RX_TIMEOUT_MS             not #define'd in 'dhcp-c_cfg.h' "
#error  "                                  [MUST be  >= NET_TIMEOUT_MIN_mS]      "
#error  "                                  [     &&  <= NET_TIMEOUT_MAX_mS]      "

#elif   (DEF_CHK_VAL(DHCPc_CFG_MAX_RX_TIMEOUT_MS, \
                     NET_TIMEOUT_MIN_mS,          \
                     NET_TIMEOUT_MAX_mS) != DEF_OK)
#error  "DHCPc_CFG_MAX_RX_TIMEOUT_MS       illegally #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >= NET_TIMEOUT_MIN_mS]     "
#error  "                                  [     &&  <= NET_TIMEOUT_MAX_mS]     "
#endif



#ifndef  DHCPc_CFG_BROADCAST_BIT_EN
#error  "DHCPc_CFG_BROADCAST_BIT_EN              not #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  DEF_DISABLED]              "
#error  "                                  [     ||  DEF_ENABLED ]              "

#elif  ((DHCPc_CFG_BROADCAST_BIT_EN != DEF_DISABLED) && \
        (DHCPc_CFG_BROADCAST_BIT_EN != DEF_ENABLED ))
#error  "DHCPc_CFG_BROADCAST_BIT_EN        illegally #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  DEF_DISABLED]              "
#error  "                                  [     ||  DEF_ENABLED ]              "
#endif



#ifndef  DHCPc_CFG_PARAM_REQ_TBL_SIZE
#error  "DHCPc_CFG_PARAM_REQ_TBL_SIZE            not #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >= 1  ]                    "
#error  "                                  [     &&  <= 255]                    "

#elif   (DEF_CHK_VAL(DHCPc_CFG_PARAM_REQ_TBL_SIZE, \
                     1,                            \
                     DEF_INT_08U_MAX_VAL) != DEF_OK)
#error  "DHCPc_CFG_PARAM_REQ_TBL_SIZE      illegally #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >= 1  ]                    "
#error  "                                  [     &&  <= 255]                    "
#endif



#ifndef  DHCPc_CFG_MAX_NBR_IF
#error  "DHCPc_CFG_MAX_NBR_IF                    not #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >= 1                    ]  "
#error  "                                  [     &&  <= NET_IF_CFG_MAX_NBR_IF]  "


#elif   (DEF_CHK_VAL(DHCPc_CFG_MAX_NBR_IF,           \
                     1,                              \
                     NET_IF_CFG_MAX_NBR_IF) != DEF_OK)
#error  "DHCPc_CFG_MAX_NBR_IF              illegally #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >= 1                    ]  "
#error  "                                  [     &&  <= NET_IF_CFG_MAX_NBR_IF]  "

#endif



#ifndef  DHCPc_CFG_ADDR_VALIDATE_EN
#error  "DHCPc_CFG_ADDR_VALIDATE_EN              not #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  DEF_DISABLED]              "
#error  "                                  [     ||  DEF_ENABLED ]              "

#elif  ((DHCPc_CFG_ADDR_VALIDATE_EN != DEF_ENABLED ) && \
        (DHCPc_CFG_ADDR_VALIDATE_EN != DEF_DISABLED))
#error  "DHCPc_CFG_ADDR_VALIDATE_EN        illegally #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  DEF_DISABLED]              "
#error  "                                  [     ||  DEF_ENABLED ]              "
#endif



#ifndef  DHCPc_CFG_DYN_LOCAL_LINK_ADDR_EN
#error  "DHCPc_CFG_DYN_LOCAL_LINK_ADDR_EN        not #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  DEF_DISABLED]              "
#error  "                                  [     ||  DEF_ENABLED ]              "

#elif  ((DHCPc_CFG_DYN_LOCAL_LINK_ADDR_EN != DEF_DISABLED) && \
        (DHCPc_CFG_DYN_LOCAL_LINK_ADDR_EN != DEF_ENABLED ))
#error  "DHCPc_CFG_DYN_LOCAL_LINK_ADDR_EN  illegally #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  DEF_DISABLED]              "
#error  "                                  [     ||  DEF_ENABLED ]              "
#endif


#ifndef  DHCPc_CFG_LOCAL_LINK_MAX_RETRY
#error  "DHCPc_CFG_LOCAL_LINK_MAX_RETRY          not #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >=   1]                    "
#error  "                                  [     &&  <= 255]                    "

#elif   (DEF_CHK_VAL(DHCPc_CFG_LOCAL_LINK_MAX_RETRY, \
                     1,                              \
                     DEF_INT_08U_MAX_VAL) != DEF_OK)
#error  "DHCPc_CFG_LOCAL_LINK_MAX_RETRY    illegally #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >=   1]                    "
#error  "                                  [     &&  <= 255]                    "
#endif


#ifndef  DHCPc_CFG_NEGO_RETRY_CNT
#error  "DHCPc_CFG_NEGO_RETRY_CNT                not #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >=   1]                    "
#error  "                                  [     &&  <= 255]                    "

#elif   (DEF_CHK_VAL(DHCPc_CFG_NEGO_RETRY_CNT,       \
                     1,                              \
                     DEF_INT_08U_MAX_VAL) != DEF_OK)
#error  "DHCPc_CFG_NEGO_RETRY_CNT          illegally #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >=   1]                    "
#error  "                                  [     &&  <= 255]                    "
#endif


#ifndef  DHCPc_CFG_DISCOVER_RETRY_CNT
#error  "DHCPc_CFG_DISCOVER_RETRY_CNT            not #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >=   1]                    "
#error  "                                  [     &&  <= 255]                    "

#elif   (DEF_CHK_VAL(DHCPc_CFG_DISCOVER_RETRY_CNT,   \
                     1,                              \
                     DEF_INT_08U_MAX_VAL) != DEF_OK)
#error  "DHCPc_CFG_DISCOVER_RETRY_CNT      illegally #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >=   1]                    "
#error  "                                  [     &&  <= 255]                    "
#endif


#ifndef  DHCPc_CFG_REQUEST_RETRY_CNT
#error  "DHCPc_CFG_REQUEST_RETRY_CNT             not #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >=   1]                    "
#error  "                                  [     &&  <= 255]                    "

#elif   (DEF_CHK_VAL(DHCPc_CFG_REQUEST_RETRY_CNT,    \
                     1,                              \
                     DEF_INT_08U_MAX_VAL) != DEF_OK)
#error  "DHCPc_CFG_REQUEST_RETRY_CNT       illegally #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  >=   1]                    "
#error  "                                  [     &&  <= 255]                    "
#endif


#ifndef  DHCPc_CFG_ARG_CHK_EXT_EN
#error  "DHCPc_CFG_ARG_CHK_EXT_EN                not #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  DEF_DISABLED]              "
#error  "                                  [     ||  DEF_ENABLED ]              "

#elif  ((DHCPc_CFG_ARG_CHK_EXT_EN != DEF_DISABLED) && \
        (DHCPc_CFG_ARG_CHK_EXT_EN != DEF_ENABLED ))
#error  "DHCPc_CFG_ARG_CHK_EXT_EN          illegally #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  DEF_DISABLED]              "
#error  "                                  [     ||  DEF_ENABLED ]              "
#endif


#ifndef  DHCPc_CFG_ARG_CHK_DBG_EN
#error  "DHCPc_CFG_ARG_CHK_DBG_EN                not #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  DEF_DISABLED]              "
#error  "                                  [     ||  DEF_ENABLED ]              "

#elif  ((DHCPc_CFG_ARG_CHK_DBG_EN != DEF_DISABLED) && \
        (DHCPc_CFG_ARG_CHK_DBG_EN != DEF_ENABLED ))
#error  "DHCPc_CFG_ARG_CHK_DBG_EN          illegally #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  DEF_DISABLED]              "
#error  "                                  [     ||  DEF_ENABLED ]              "
#endif


#ifndef  DHCPc_DBG_CFG_MEM_CLR_EN
#error  "DHCPc_DBG_CFG_MEM_CLR_EN                not #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  DEF_DISABLED]              "
#error  "                                  [     ||  DEF_ENABLED ]              "

#elif  ((DHCPc_DBG_CFG_MEM_CLR_EN != DEF_DISABLED) && \
        (DHCPc_DBG_CFG_MEM_CLR_EN != DEF_ENABLED ))
#error  "DHCPc_DBG_CFG_MEM_CLR_EN          illegally #define'd in 'dhcp-c_cfg.h'"
#error  "                                  [MUST be  DEF_DISABLED]              "
#error  "                                  [     ||  DEF_ENABLED ]              "
#endif



                                                                /* Correctly configured in 'dhcp-c.h'; DO NOT MODIFY.   */
#ifndef  DHCPc_NBR_TMR
#error  "DHCPc_NBR_TMR                           not #define'd in 'dhcp-c.h'"
#error  "                                  [MUST be  >= DHCPc_TMR_NBR_MIN]  "
#error  "                                  [     &&  <= DHCPc_TMR_NBR_MAX]  "

#elif   (DEF_CHK_VAL(DHCPc_NBR_TMR,              \
                     DHCPc_TMR_NBR_MIN,          \
                     DHCPc_TMR_NBR_MAX) != DEF_OK)
#error  "DHCPc_NBR_TMR                     illegally #define'd in 'dhcp-c.h'"
#error  "                                  [MUST be  >= DHCPc_TMR_NBR_MIN]  "
#error  "                                  [     &&  <= DHCPc_TMR_NBR_MAX]  "
#endif


/*
*********************************************************************************************************
*                                    NETWORK CONFIGURATION ERRORS
*********************************************************************************************************
*/

#if     (NET_IF_CFG_ETHER_EN != DEF_ENABLED)
#error  "NET_IF_CFG_ETHER_EN               illegally #define'd in 'net_cfg.h'"
#error  "                                  [MUST be  DEF_ENABLED]            "
#endif


/*
*********************************************************************************************************
*                                    LIBRARY CONFIGURATION ERRORS
*********************************************************************************************************
*/

                                                                /* See 'dhcp-c.h  Note #3a'.                            */
#if     (NET_VERSION < 30000u)
#error  "NET_VERSION       [SHOULD be >= V3.00.00]"
#endif

                                                                /* See 'dhcp-c.h  Note #3b'.                            */
#if     (CPU_CORE_VERSION < 127u)
#error  "CPU_CORE_VERSION  [SHOULD be >= V1.27]"
#endif

                                                                /* See 'dhcp-c.h  Note #3c'.                            */
#if     (LIB_VERSION < 13800u)
#error  "LIB_VERSION       [SHOULD be >= V1.38.00]"
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of DHCPc module include.                         */

