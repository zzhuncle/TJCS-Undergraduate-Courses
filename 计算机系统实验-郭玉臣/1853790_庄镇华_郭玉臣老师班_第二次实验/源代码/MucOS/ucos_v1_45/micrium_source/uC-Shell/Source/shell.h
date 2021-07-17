/*
*********************************************************************************************************
*                                              uC/Shell
*                                            Shell Utility
*
*                          (c) Copyright 2007-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/Shell is provided in source form to registered licensees ONLY.  It is
*               illegal to distribute this source code to any third party unless you receive
*               written permission by an authorized Micrium representative.  Knowledge of
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            SHELL UTILITY
*
* Filename      : shell.h
* Version       : V1.03.01
* Programmer(s) : SR
*                 FBJ
*********************************************************************************************************
* 
*
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               SHELL present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  SHELL_PRESENT                                          /* See Note #1.                                         */
#define  SHELL_PRESENT


/*
*********************************************************************************************************
*                                        SHELL VERSION NUMBER
*
* Note(s) : (1) (a) The Shell module software version is denoted as follows :
*
*                       Vx.yy.zz
*
*                           where
*                                   V               denotes 'Version' label
*                                   x               denotes     major software version revision number
*                                   yy              denotes     minor software version revision number
*                                   zz              denotes sub-minor software version revision number
*
*               (b) The Shell software version label #define is formatted as follows :
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

#define  SHELL_VERSION                                 10301u   /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   SHELL_MODULE
#define  SHELL_EXT
#else
#define  SHELL_EXT  extern
#endif


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*
* Note(s) : (1) The Shell module files are located in the following directories :
*
*               (a) \<Your Product Application>\shell_cfg.h
*
*               (b) \<Shell>\Source\shell.h
*                                  \shell.c
*
*                       where
*                               <Your Product Application>      directory path for Your Product's Application
*                               <Shell>                         directory path for Shell module
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
*               (b) '\<Shell>\' directory                                               See Note #1b
*
*               (c) (1) '\<CPU-Compiler Directory>\'                  directory         See Note #2a
*                   (2) '\<CPU-Compiler Directory>\<cpu>\<compiler>\' directory         See Note #2b
*
*               (d) '\<Custom Library Directory>\' directory                            See Note #3a
*********************************************************************************************************
*/

#include  <cpu.h>                                               /* CPU Configuration              (see Note #2b)        */
#include  <cpu_core.h>                                          /* CPU Core Library               (see Note #2a)        */

#include  <lib_def.h>                                           /* Standard        Defines        (see Note #3a)        */
#include  <lib_str.h>                                           /* Standard String Library        (see Note #3a)        */

#include  <shell_cfg.h>                                         /* Application Configuration File (see Note #1a)        */


/*
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*/

#define  SHELL_ASCII_SPACE                                ' '   /* ASCII value for space                                */
#define  SHELL_ASCII_QUOTE                                '\"'  /* ASCII value for quote                                */
#define  SHELL_ASCII_ARG_END                              '\0'
#define  SHELL_ASCII_CDM_NAME_DELIMITER                   '_'


/*
*********************************************************************************************************
*                                      SHELL ERROR CODES DEFINES
*
* Note(s) : (1) Command function MUST return SHELL_EXEC_ERR when an error occurred at execution time.
*               Any other return value is command specific.
*
*           (2) Output function MUST return :
*
*               (a) The number of positive data octets transmitted, if NO errors
*
*               (b) SHELL_OUT_RTN_CODE_CONN_CLOSED,                 if implemented connection closed
*
*               (c) SHELL_OUT_ERR,                                  otherwise 
*********************************************************************************************************
*/

#define  SHELL_EXEC_ERR                                   -1    /* See Note #1.                                         */

#define  SHELL_OUT_RTN_CODE_CONN_CLOSED                    0    /* See Note #2.                                         */
#define  SHELL_OUT_ERR                                    -1

#define  SHELL_ERR_NONE                                    0
#define  SHELL_ERR_NULL_PTR                                1
#define  SHELL_ERR_MODULE_CMD_EMPTY                        2
#define  SHELL_ERR_MODULE_CMD_NAME_TOO_LONG                3
#define  SHELL_ERR_MODULE_CMD_NAME_NONE                    4
#define  SHELL_ERR_MODULE_CMD_NAME_COPY                    5
#define  SHELL_ERR_MODULE_CMD_NONE_AVAIL                   6
#define  SHELL_ERR_MODULE_CMD_ALREADY_IN                   7
#define  SHELL_ERR_MODULE_CMD_NOT_FOUND                    8
#define  SHELL_ERR_CMD_EXEC                                9
#define  SHELL_ERR_CMD_NOT_FOUND                          10
#define  SHELL_ERR_ARG_TBL_FULL                           11
#define  SHELL_ERR_CMD_SEARCH                             12


/*
*********************************************************************************************************
*                                        ERROR CODES DATA TYPE
*********************************************************************************************************
*/

typedef  CPU_INT16U  SHELL_ERR;


/*
*********************************************************************************************************
*                                        SHELL COMMAND PARAMETER
*
* Note(s) : (1) This structure is used to pass additional parameters to the command.  Future implementation
*               of this module could add fields to that structure.
*
*           (2) This variable is used to let the shell commands to control the session status.  For
*               instance, a command interacting with uC/TELNETs could used this variable to terminate
*               the current session.
*********************************************************************************************************
*/

typedef  struct  shell_cmd_param {
    void         *pcur_working_dir;                             /* Cur working dir ptr.                                 */
    void         *pout_opt;                                     /* Output opt      ptr.                                 */
    CPU_BOOLEAN  *psession_active;                              /* Session status flag (see Note #2).                   */
} SHELL_CMD_PARAM;


/*
*********************************************************************************************************
*                                SHELL COMMAND FUNCTION POINTER DATA TYPE
*
* Note(s) : (1) (a) 'SHELL_OUT_FNCT' data type defined to replace the commonly-used function pointer
*                   to the output facility.
*
*               (b)     CPU_INT16U        ret_val;
*                       SHELL_CMD_FNCT    FnctName;
*                       CPU_CHAR         *pbuf;
*                       CPU_INT16U        buf_len;
*                       void             *popt;
*
*                       ret_val = FnctName(pbuf, buf_len, popt);
*
*               (c) Shell output function MUST return : 
*
*                   (1) The number of positive data octets transmitted, if NO error
*
*                   (2) SHELL_OUT_RTN_CODE_CONN_CLOSED,                 if link connection closed
*
*                   (3) SHELL_OUT_ERR,                                  otherwise 
*
*           (2) (a) 'SHELL_CMD_FNCT' data type defined to replace the commonly-used function pointer
*                   to a shell command.  The last parameter is a pointer to an 'out' function having
*                   the prototype specified in #1.
*
*               (b) Example function pointer usage :
*
*                       CPU_INT16U        ret_val
*                       SHELL_CMD_FNCT    FnctName;
*                       CPU_INT16U        argc;
*                       CPU_CHAR         *argv[];
*                       SHELL_OUT_FNCT    pout_fnct;
*                       SHELL_CMD_PARAM  *pcmd_param
*
*                       ret_val = FnctName(argc, argv, pout_fnct, pcmd_param);
*
*               (c) Shell commands MUST return SHELL_EXEC_ERR when an error occured at execution
*                   time.  Any other return value is command specific.
*********************************************************************************************************
*/

                                                                /* See Note #1.                                         */
typedef  CPU_INT16S  (*SHELL_OUT_FNCT)(CPU_CHAR          *p_buf,
                                       CPU_INT16U         buf_len,
                                       void              *p_opt);

                                                                /* See Note #2.                                         */
typedef  CPU_INT16S  (*SHELL_CMD_FNCT)(CPU_INT16U         argc,
                                       CPU_CHAR          *argv[],
                                       SHELL_OUT_FNCT     out_fnct,
                                       SHELL_CMD_PARAM   *p_cmd_param);


/*
*********************************************************************************************************
*                                       SHELL COMMAND DATA TYPE
*
* Note(s) : This structure is used to store a command (function pointer) along with a character string
*           representing its name.
*********************************************************************************************************
*/

typedef  struct  shell_cmd {
    const  CPU_CHAR  *Name;                                     /* Ptr to cmd name.                                     */
    SHELL_CMD_FNCT    Fnct;                                     /* Ptr to cmd fnct.                                     */
} SHELL_CMD;


/*
*********************************************************************************************************
*                                    SHELL MODULE COMMAND DATA TYPE
*
* Note(s) : 'Name' is a NULL terminated character string representing the name of the module command.
*           See Shell_CmdTblAdd(), Note #2, for more details.
*********************************************************************************************************
*/

typedef  struct  shell_module_cmd  SHELL_MODULE_CMD;

struct  shell_module_cmd {
    CPU_CHAR           Name[SHELL_CFG_MODULE_CMD_NAME_LEN_MAX]; /* Name (prefix) of module cmd (see Note #1).           */
    SHELL_MODULE_CMD  *PrevModuleCmdPtr;                        /* Ptr to PREV      module cmd.                         */
    SHELL_MODULE_CMD  *NextModuleCmdPtr;                        /* Ptr to NEXT      module cmd.                         */
    SHELL_CMD         *CmdTblPtr;                               /* Ptr to cmd tbl.                                      */
} ;


/*
*********************************************************************************************************
*                                           GLOBAL VARIABLES
*********************************************************************************************************
*/

SHELL_EXT  SHELL_MODULE_CMD   Shell_ModuleCmdTbl[SHELL_CFG_CMD_TBL_SIZE];

SHELL_EXT  SHELL_MODULE_CMD  *Shell_ModuleCmdUsedPoolPtr;       /* Ptr to pool of used module cmd.                      */
SHELL_EXT  SHELL_MODULE_CMD  *Shell_ModuleCmdFreePoolPtr;       /* Ptr to pool of free module cmd.                      */


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

CPU_BOOLEAN  Shell_Init     (void);

CPU_INT16S   Shell_Exec     (CPU_CHAR         *in,
                             SHELL_OUT_FNCT    out_fnct,
                             SHELL_CMD_PARAM  *pcmd_param,
                             SHELL_ERR        *perr);

void         Shell_CmdTblAdd(CPU_CHAR         *cmd_tbl_name,
                             SHELL_CMD         cmd_tbl[],
                             SHELL_ERR        *perr);

void         Shell_CmdTblRem(CPU_CHAR         *cmd_tbl_name,
                             SHELL_ERR        *perr);


/*
*********************************************************************************************************
*                                              TRACING
*********************************************************************************************************
*/

                                                                /* Trace level, default to TRACE_LEVEL_OFF              */
#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                                 0
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                                1
#endif

#ifndef  TRACE_LEVEL_DEBUG
#define  TRACE_LEVEL_DEBUG                               2
#endif

#ifndef  SHELL_TRACE_LEVEL
#define  SHELL_TRACE_LEVEL                 TRACE_LEVEL_OFF
#endif

#ifndef  SHELL_TRACE
#define  SHELL_TRACE                                printf
#endif

#define  SHELL_TRACE_INFO(x)                  ((SHELL_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(SHELL_TRACE x) : (void)0)
#define  SHELL_TRACE_DEBUG(x)                 ((SHELL_TRACE_LEVEL >= TRACE_LEVEL_DEBUG) ? (void)(SHELL_TRACE x) : (void)0)


/*
*********************************************************************************************************
*                                         CONFIGURATION ERRORS
*********************************************************************************************************
*/

#ifndef  SHELL_CFG_CMD_TBL_SIZE
#error  "SHELL_CFG_CMD_TBL_SIZE                   not #define'd in 'shell_cfg.h'"

#elif  ((SHELL_CFG_CMD_TBL_SIZE <                   1) || \
        (SHELL_CFG_CMD_TBL_SIZE > DEF_INT_16U_MAX_VAL))
#error  "SHELL_CFG_CMD_TBL_SIZE             illegally #define'd in 'shell_cfg.h'"
#error  "                                   [MUST be >=     1]                "
#error  "                                   [     && <= 65535]                "
#endif

#ifndef  SHELL_CFG_CMD_ARG_NBR_MAX
#error  "SHELL_CFG_CMD_ARG_NBR_MAX                not #define'd in 'shell_cfg.h'"

#elif  ((SHELL_CFG_CMD_ARG_NBR_MAX <                   1) || \
        (SHELL_CFG_CMD_ARG_NBR_MAX > DEF_INT_16U_MAX_VAL))
#error  "SHELL_CFG_CMD_ARG_NBR_MAX          illegally #define'd in 'shell_cfg.h'"
#error  "                                   [MUST be >=     1]                "
#error  "                                   [     && <= 65535]                "
#endif

#ifndef  SHELL_CFG_MODULE_CMD_NAME_LEN_MAX
#error  "SHELL_CFG_MODULE_CMD_NAME_LEN_MAX        not #define'd in 'shell_cfg.h'"

#elif  ((SHELL_CFG_MODULE_CMD_NAME_LEN_MAX <                   1) || \
        (SHELL_CFG_MODULE_CMD_NAME_LEN_MAX > DEF_INT_16U_MAX_VAL))
#error  "SHELL_CFG_MODULE_CMD_NAME_LEN_MAX  illegally #define'd in 'shell_cfg.h'"
#error  "                                   [MUST be >=     1]                "
#error  "                                   [     && <= 65535]                "
#endif


/*
*********************************************************************************************************
*                                              MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of SHELL module include.                         */

