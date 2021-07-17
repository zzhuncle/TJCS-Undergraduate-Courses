/*
*********************************************************************************************************
*                                               uC/Shell
*                                            Shell utility
*
*                           (c) Copyright 2007-2013; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                              TERMINAL
*
* Filename      : terminal.h
* Version       : V1.03.01
* Programmer(s) : BAN
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               TERMINAL present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  TERMINAL_PRESENT                                       /* See Note #1.                                         */
#define  TERMINAL_PRESENT


/*
*********************************************************************************************************
*                                       TERMINAL VERSION NUMBER
*
* Note(s) : (1) (a) The Terminal software version is denoted as follows :
*
*                       Vx.yy
*
*                           where
*                                  V     denotes 'Version' label
*                                  x     denotes major software version revision number
*                                  yy    denotes minor software version revision number
*
*               (b) The Terminal software version label #define is formatted as follows :
*
*                       ver = x.yy * 100
*
*                           where
*                                  ver   denotes software version number scaled as
*                                        an integer value
*                                  x.yy  denotes software version number
*********************************************************************************************************
*/

#define  TERMINAL_VERSION          100u                         /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   TERMINAL_MODULE
#define  TERMINAL_EXT
#else
#define  TERMINAL_EXT  extern
#endif


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <cpu_core.h>

#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_mem.h>
#include  <lib_str.h>

#include  <terminal_cfg.h>
#include  <shell.h>


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  TERMINAL_ESC_TYPE_NONE                            0u
#define  TERMINAL_ESC_TYPE_UP                              1u
#define  TERMINAL_ESC_TYPE_DOWN                            2u
#define  TERMINAL_ESC_TYPE_INS                             3u


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


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

CPU_BOOLEAN  Terminal_Init        (void);

void         Terminal_Task        (void         *p_arg);

void         Terminal_WrStr       (CPU_CHAR     *pbuf,
                                   CPU_SIZE_T    buf_len);

void         Terminal_WrChar      (CPU_CHAR      c);

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                   DEFINED in OS's 'terminal_os.c'
*********************************************************************************************************
*/

CPU_BOOLEAN  Terminal_OS_Init     (void         *p_arg);

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                               DEFINED in SERIAL's 'terminal_serial.c'
*********************************************************************************************************
*/

CPU_BOOLEAN  TerminalSerial_Init  (void);

void         TerminalSerial_Exit  (void);

CPU_INT08U   TerminalSerial_RdByte(void);

CPU_INT16S   TerminalSerial_Wr    (void         *pbuf,
                                   CPU_SIZE_T    buf_len);

void         TerminalSerial_WrByte(CPU_INT08U    c);

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                 DEFINED in MODE's 'terminal_mode.c'
*********************************************************************************************************
*/

CPU_INT08U   TerminalMode_RdLine  (CPU_CHAR     *pstr,
                                   CPU_SIZE_T    len_max,
                                   CPU_SIZE_T   *pcursor_pos,
                                   CPU_BOOLEAN   ins);

void         TerminalMode_Clr     (CPU_SIZE_T    nbr_char,
                                   CPU_SIZE_T    cursor_pos);

void         TerminalMode_NewLine (void);

void         TerminalMode_Prompt  (void);


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

                                                                /* Task priority.                                       */
#ifndef  TERMINAL_OS_CFG_TASK_PRIO
#error  "TERMINAL_OS_CFG_TASK_PRIO                    not #define'd in 'terminal_cfg.h'"
#endif



                                                                /* Task stack size.                                     */
#ifndef  TERMINAL_OS_CFG_TASK_STK_SIZE
#error  "TERMINAL_OS_CFG_TASK_STK_SIZE                not #define'd in 'terminal_cfg.h'"
#endif



                                                                /* Maximum working directory path length.               */
#ifndef  TERMINAL_CFG_MAX_PATH_LEN
#error  "TERMINAL_CFG_MAX_PATH_LEN                    not #define'd in 'terminal_cfg.h'"
#error  "                                       [MUST be >=     1]                "
#error  "                                       [     && <= 65535]                "

#elif  ((TERMINAL_CFG_MAX_PATH_LEN           <                   1) || \
        (TERMINAL_CFG_MAX_PATH_LEN           > DEF_INT_16U_MAX_VAL))
#error  "TERMINAL_CFG_MAX_PATH_LEN              illegally #define'd in 'terminal_cfg.h'"
#error  "                                       [MUST be >=     1]                "
#error  "                                       [     && <= 65535]                "
#endif



                                                                /* Maximum input line length length.                    */
#ifndef  TERMINAL_CFG_MAX_CMD_LEN
#error  "TERMINAL_CFG_MAX_CMD_LEN                     not #define'd in 'terminal_cfg.h'"
#error  "                                       [MUST be >=     1]                "
#error  "                                       [     && <= 65535]                "


#elif  ((TERMINAL_CFG_MAX_CMD_LEN            <                   1) || \
        (TERMINAL_CFG_MAX_CMD_LEN            > DEF_INT_16U_MAX_VAL))
#error  "TERMINAL_CFG_MAX_CMD_LEN               illegally #define'd in 'terminal_cfg.h'"
#error  "                                       [MUST be >=     1]                "
#error  "                                       [     && <= 65535]                "
#endif




                                                                /* History enable.                                      */
#ifndef  TERMINAL_CFG_HISTORY_EN
#error  "TERMINAL_CFG_HISTORY_EN                      not #define'd in 'terminal_cfg.h'"
#error  "                                       [MUST be    DEF_ENABLED ]         "
#error  "                                       [     && || DEF_DISABLED]         "



#elif  ((TERMINAL_CFG_HISTORY_EN             != DEF_ENABLED) && \
        (TERMINAL_CFG_HISTORY_EN             != DEF_DISABLED))
#error  "TERMINAL_CFG_HISTORY_EN                illegally #define'd in 'terminal_cfg.h'"
#error  "                                       [MUST be    DEF_ENABLED ]         "
#error  "                                       [     && || DEF_DISABLED]         "



#elif   (TERMINAL_CFG_HISTORY_EN             == DEF_ENABLED)
                                                                /* Number of items to keep in history.                  */
#ifndef  TERMINAL_CFG_HISTORY_ITEMS_NBR
#error  "TERMINAL_CFG_HISTORY_ITEMS_NBR               not #define'd in 'terminal_cfg.h'"
#error  "                                       [MUST be >=     4]                "
#error  "                                       [     && <= 65535]                "

#elif  ((TERMINAL_CFG_HISTORY_ITEMS_NBR      <                   4) || \
        (TERMINAL_CFG_HISTORY_ITEMS_NBR      > DEF_INT_16U_MAX_VAL))
#error  "TERMINAL_CFG_HISTORY_ITEMS_NBR         illegally #define'd in 'terminal_cfg.h'"
#error  "                                       [MUST be >=     4]                "
#error  "                                       [     && <= 65535]                "
#endif



                                                                /* Maximum length of items in history.                  */
#ifndef  TERMINAL_CFG_HISTORY_ITEM_LEN
#error  "TERMINAL_CFG_HISTORY_ITEM_LEN                not #define'd in 'terminal_cfg.h'"
#error  "                    [MUST be >=                        1]                "
#error  "                    [     && <= TERMINAL_CFG_MAX_CMD_LEN]                "

#elif  ((TERMINAL_CFG_HISTORY_ITEM_LEN <                        1) || \
        (TERMINAL_CFG_HISTORY_ITEM_LEN > TERMINAL_CFG_MAX_CMD_LEN))
#error  "TERMINAL_CFG_HISTORY_ITEM_LEN          illegally #define'd in 'terminal_cfg.h'"
#error  "                    [MUST be >=                        1]                "
#error  "                    [     && <= TERMINAL_CFG_MAX_CMD_LEN]                "
#endif

#endif


/*
*********************************************************************************************************
*                                              MODULE END
*
* Note(s) : See 'MODULE  Note #1'.
*********************************************************************************************************
*/

#endif                                                          /* End of TERMINAL module include (see Note #1).        */
