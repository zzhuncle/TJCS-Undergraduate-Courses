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
* Filename      : terminal.c
* Version       : V1.03.01
* Programmer(s) : BAN
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    TERMINAL_MODULE
#include  <terminal.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


#define  TERMINAL_NEW_LINE              (CPU_CHAR *)"\r\n"
#define  TERMINAL_BLANK_LINE            (CPU_CHAR *)"\r\n\r\n"
#define  TERMINAL_STR_HELP              (CPU_CHAR *)"-h"

/*
*********************************************************************************************************
*                                       ARGUMENT ERROR MESSAGES
*********************************************************************************************************
*/

#define  TERMINAL_ARG_ERR_FC            (CPU_CHAR *)"Term_fc: usage: Term_fc"

/*
*********************************************************************************************************
*                                    COMMAND EXPLANATION MESSAGES
*********************************************************************************************************
*/

#define  TERMINAL_CMD_EXP_FC            (CPU_CHAR *)"                List terminal history items."


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

#if (TERMINAL_CFG_HISTORY_EN == DEF_ENABLED)
static  CPU_CHAR     Terminal_History[TERMINAL_CFG_HISTORY_ITEMS_NBR][TERMINAL_CFG_HISTORY_ITEM_LEN];
static  CPU_INT16U   Terminal_HistoryIxFirst;
static  CPU_INT16U   Terminal_HistoryIxLast;
static  CPU_INT16U   Terminal_HistoryIxShown;
static  CPU_BOOLEAN  Terminal_HistoryShown;
static  CPU_BOOLEAN  Terminal_HistoryEmpty;
static  CPU_INT16U   Terminal_HistoryCnt;
#endif


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  CPU_INT16S  Terminal_OutFnct       (CPU_CHAR         *pbuf,
                                            CPU_INT16U        buf_len,
                                            void             *popt);

static  CPU_INT16S  Terminal_Help          (SHELL_OUT_FNCT    out_fnct,
                                            SHELL_CMD_PARAM  *pcmd_param);

#if (TERMINAL_CFG_HISTORY_EN == DEF_ENABLED)
static  void        Terminal_HistoryInit   (void);

static  void        Terminal_HistoryNextGet(CPU_CHAR         *pstr);

static  void        Terminal_HistoryPrevGet(CPU_CHAR         *pstr);

static  void        Terminal_HistoryPut    (CPU_CHAR         *pstr);

static  CPU_INT16S  Terminal_fc            (CPU_INT16U        argc,
                                            CPU_CHAR         *argv[],
                                            SHELL_OUT_FNCT    out_fnct,
                                            SHELL_CMD_PARAM  *pcmd_param);
#endif

/*
*********************************************************************************************************
*                                         SHELL COMMAND TABLE
*********************************************************************************************************
*/

#if (TERMINAL_CFG_HISTORY_EN == DEF_ENABLED)
static  SHELL_CMD  Terminal_CmdTbl [] = {
    {"Term_fc", Terminal_fc},
    {0,         0          }
};
#endif


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           Terminal_Init()
*
* Description : Initialize terminal.
*
* Argument(s) : none.
*
* Return(s)   : DEF_OK,   if terminal was initialized.
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  Terminal_Init (void)
{
    CPU_BOOLEAN  ok;


                                                                /* ------------------- INIT SERIAL IF ----------------- */
    ok = TerminalSerial_Init();
    if (ok == DEF_OK) {



                                                                /* ------------------ INIT OS SERVICES ---------------- */
        ok = Terminal_OS_Init((void *)0);
        if (ok != DEF_OK) {                                     /* If OS not init'd ...                                 */
            TerminalSerial_Exit();                              /* ... exit serial if.                                  */
        }
    }

    return (ok);
}


/*
*********************************************************************************************************
*                                           Terminal_Task()
*
* Description : Terminal task.
*
* Argument(s) : p_arg       Argument passed to the task (ignored).
*
* Return(s)   : none.
*
* Caller(s)   : Terminal OS port.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  Terminal_Task (void *p_arg)
{
    CPU_CHAR         cmd[TERMINAL_CFG_MAX_CMD_LEN + 1u];
    CPU_SIZE_T       cmd_len;
    SHELL_CMD_PARAM  cmd_param;
    CPU_INT16S       cmp_val;
    CPU_SIZE_T       cursor_pos;
    CPU_CHAR         cwd_path[TERMINAL_CFG_MAX_PATH_LEN + 1u];
    SHELL_ERR        err;
    CPU_INT08U       esc_type;
    CPU_BOOLEAN      ins;


    (void)p_arg;

                                                                /* --------------------- INIT VARS -------------------- */
    Mem_Set((void     *)&cwd_path[0],                           /* Clr cur working dir path.                            */
            (CPU_INT08U) 0x00u,
            (CPU_SIZE_T) TERMINAL_CFG_MAX_PATH_LEN);

    Str_Copy(cwd_path, (CPU_CHAR *)"\\");

    Mem_Set((void     *)&cmd[0],                                /* Clr cur line.                                        */
            (CPU_INT08U) 0x00u,
            (CPU_SIZE_T) TERMINAL_CFG_MAX_CMD_LEN);

#if (TERMINAL_CFG_HISTORY_EN == DEF_ENABLED)
    Terminal_HistoryInit();
    Shell_CmdTblAdd((CPU_CHAR *)"Term", Terminal_CmdTbl, &err);
#endif

    cursor_pos                 = 0u;
    cmd_len                    = 0u;
    ins                        = DEF_NO;

    cmd_param.pcur_working_dir = (void *)cwd_path;
    cmd_param.pout_opt         = (void *)0;

    TerminalMode_Prompt();                                      /* Show first prompt.                                   */




    while (DEF_TRUE) {
                                                                /* -------------------- RD NEW LINE ------------------- */
        esc_type = TerminalMode_RdLine(&cmd[0],
                                        TERMINAL_CFG_MAX_CMD_LEN,
                                       &cursor_pos,
                                        ins);
        cmd_len  = Str_Len(&cmd[0]);


        switch (esc_type) {
#if (TERMINAL_CFG_HISTORY_EN == DEF_ENABLED)
            case TERMINAL_ESC_TYPE_UP:                          /* ------------- MOVE TO PREV HISTORY ITEM ------------ */
                 TerminalMode_Clr(cmd_len, cursor_pos);         /* Clr terminal line.                                   */
                 Terminal_HistoryPrevGet(cmd);                  /* Get prev history item.                               */
                 cmd_len    = Str_Len(cmd);
                 cursor_pos = cmd_len;                          /* Cursor at end of line.                               */
                 Terminal_WrStr(cmd, cmd_len);                  /* Wr  prev history item to terminal.                   */
                 break;



            case TERMINAL_ESC_TYPE_DOWN:                        /* ------------- MOVE TO NEXT HISTORY ITEM ------------ */
                 TerminalMode_Clr(cmd_len, cursor_pos);         /* Clr terminal line.                                   */
                 Terminal_HistoryNextGet(cmd);                  /* Get next history item.                               */
                 cmd_len    = Str_Len(cmd);
                 cursor_pos = cmd_len;                          /* Cursor at end of line.                               */
                 Terminal_WrStr(cmd, cmd_len);                  /* Wr  next history item to terminal.                   */
                 break;
#else




            case TERMINAL_ESC_TYPE_UP:                          /* ---------------- UNSUPPORTED UP/DOWN --------------- */
            case TERMINAL_ESC_TYPE_DOWN:
                 TerminalMode_Clr(cmd_len, cursor_pos);         /* Clear line.                                          */
                 Str_Copy(cmd, (CPU_CHAR *)"");
                 break;
#endif



            case TERMINAL_ESC_TYPE_INS:                         /* ---------------- TOGGLE INSERT MODE ---------------- */
                 if (ins == DEF_YES) {
                     ins = DEF_NO;
                 } else {
                     ins = DEF_YES;
                 }
                 break;



            case TERMINAL_ESC_TYPE_NONE:                        /* --------------------- EXEC CMD --------------------- */
            default:
#if (TERMINAL_CFG_HISTORY_EN == DEF_ENABLED)
                 Terminal_HistoryPut(cmd);                      /* Put line into history.                               */
#endif

                 cmp_val = Str_Cmp(cmd, (CPU_CHAR *)"");
                 if (cmp_val != 0) {
                     TerminalMode_NewLine();                    /* Move to new line.                                    */

                     cmp_val = Str_Cmp(cmd, (CPU_CHAR *)"?");
                     if (cmp_val == 0) {
                        (void)Terminal_Help( Terminal_OutFnct,  /* List all cmds ...                                    */
                                            &cmd_param);


                     } else {
                        (void)Shell_Exec( cmd,                  /* ... OR exec cmd.                                     */
                                          Terminal_OutFnct,
                                         &cmd_param,
                                         &err);

                         switch (err) {
                             case SHELL_ERR_CMD_NOT_FOUND:
                             case SHELL_ERR_CMD_SEARCH:
                             case SHELL_ERR_ARG_TBL_FULL:
                                  Terminal_WrStr((CPU_CHAR *)"Command not found\r\n", 19);
                                  break;

                             case SHELL_ERR_NONE:
                             case SHELL_ERR_NULL_PTR:
                             case SHELL_ERR_CMD_EXEC:
                             default:
                                  break;
                         }
                     }
                 }



                                                                /* ------------------ DISP NEW PROMPT ----------------- */
                 TerminalMode_Prompt();                         /* Show new prompt.                                     */
                 Str_Copy(cmd, (CPU_CHAR *)"");                 /* Clear cmd.                                           */
                 cursor_pos = 0u;                               /* Cursor pos'd at beginning of line.                   */
                 break;
        }
    }
}


/*
*********************************************************************************************************
*                                          Terminal_WrStr()
*
* Description : Write string to terminal.
*
* Argument(s) : pbuf        Pointer to the buffer to transmit.
*
*               buf_len     Number of bytes in the buffer.
*
* Return(s)   : none.
*
* Caller(s)   : various.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  Terminal_WrStr (CPU_CHAR    *pbuf,
                      CPU_SIZE_T   buf_len)
{
    TerminalSerial_Wr((void     *)pbuf,
                      (CPU_SIZE_T)buf_len);
}


/*
*********************************************************************************************************
*                                          Terminal_WrChar()
*
* Description : Write character to terminal.
*
* Argument(s) : c           Character to transmit.
*
* Return(s)   : none.
*
* Caller(s)   : various.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  Terminal_WrChar (CPU_CHAR  c)
{
    TerminalSerial_WrByte((CPU_INT08U)c);
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          Terminal_OutFnct()
*
* Description : Out function used by Shell.
*
* Argument(s) : pbuf        Pointer to the buffer contianing data to send.
*
*               buf_len     Length of buffer.
*
*               popt        Pointer to options (unused).
*
* Return(s)   : Number of positive data octets transmitted.
*
* Caller(s)   : Shell, as a result of command execution in Terminal_Task().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_INT16S  Terminal_OutFnct (CPU_CHAR    *pbuf,
                                      CPU_INT16U   buf_len,
                                      void        *popt)
{
    (void)popt;

    TerminalSerial_Wr((void     *)pbuf,
                      (CPU_SIZE_T)buf_len);

    return ((CPU_INT16S)buf_len);
}


/*
*********************************************************************************************************
*                                           Terminal_Help()
*
* Description : List all commands.
*
* Argument(s) : out_fnct    The output function.
*
*               pcmd_param  Pointer to the command parameters.
*
* Return(s)   : SHELL_EXEC_ERR, if an error is encountered.
*               SHELL_ERR_NONE, otherwise.
*
* Caller(s)   : Shell, in response to command execution.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_INT16S  Terminal_Help (SHELL_OUT_FNCT    out_fnct,
                                   SHELL_CMD_PARAM  *pcmd_param)
{
    SHELL_CMD         *pcmd;
    SHELL_MODULE_CMD  *pmodule_cmd;


    pmodule_cmd = Shell_ModuleCmdUsedPoolPtr;
    while (pmodule_cmd != (SHELL_MODULE_CMD *)0) {
        pcmd = pmodule_cmd->CmdTblPtr;
        if (pcmd != (SHELL_CMD *)0) {
            while (pcmd->Fnct != (SHELL_CMD_FNCT)0) {
                (void)out_fnct((CPU_CHAR *)pcmd->Name,
                               (CPU_INT16U)Str_Len(pcmd->Name),
                                           pcmd_param->pout_opt);
                (void)out_fnct(TERMINAL_NEW_LINE, 2,                               pcmd_param->pout_opt);
                pcmd++;
            }
        }
        pmodule_cmd = pmodule_cmd->NextModuleCmdPtr;
    }

    return (SHELL_ERR_NONE);
}


/*
*********************************************************************************************************
*                                       Terminal_HistoryInit()
*
* Description : Initialize terminal history.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Terminal_Task().
*
* Note(s)     : (1) The history is implemented as a circular buffer of strings.  Three indices and two
*                   flags are used to track the state of the history buffer :
*
*                   (a) (1) If 'Terminal_HistoryShown' is DEF_YES, then the terminal displays a
*                           (potentially modified) item from the history.  This is the case after the up
*                           key is pressed once the history buffer is no longer empty, until a command
*                           is executed.
*
*                       (2) If 'Terminal_HistoryShown' is DEF_NO, then the terminal displays a bare
*                           terminal, or text that the user typed on a bare terminal.  This is the case
*                           upon terminal startup or after execution of a command until the up key is
*                           pressed.
*
*                   (b) If 'Terminal_HistoryEmpty' is DEF_YES, then the terminal history is empty.
*                       This is used in two ways :
*
*                       (1) The 'up' key will not produce a change in the history state when the history
*                           is empty.
*
*                       (2) The values of 'Terminal_HistoryIxFirst' and 'Terminal_HistoryIxLast' are
*                           ignored when the first item is put into the buffer.  Only when the terminal
*                           history is non-empty are these valid indices.
*
*                   (c) 'Terminal_HistoryIxFirst' is the index of the first item in the history.  The
*                       index of the first item is ONLY changed once the user has logged more history
*                       items than can be held in the history buffer, whereupon the index is incremented
*                       to point to the next item in the buffer (since the old first item will have been
*                       overwritten with the new last element).
*
*                   (d) 'Terminal_HistoryIxLast' is the index of the last item in the history.  The
*                       index of the last item is ONLY changed when elements are added to the history
*                       buffer, whereupon the index is incremented to point to the new last item in the
*                       buffer.
*
*                   (e) 'Terminal_HistoryIxShown' is the index of the history item shown on the terminal
*                       (if 'Terminal_HistoryShown' is DEF_YES).
*********************************************************************************************************
*/

#if (TERMINAL_CFG_HISTORY_EN == DEF_ENABLED)
static  void  Terminal_HistoryInit (void)
{
    CPU_INT16U  history_ix;


                                                                /* Clr history lines.                                   */
    for (history_ix = 0u; history_ix < TERMINAL_CFG_HISTORY_ITEMS_NBR; history_ix++) {
        Str_Copy(Terminal_History[history_ix], (CPU_CHAR *)"");
    }

    Terminal_HistoryIxFirst = 0u;                               /* See Notes #1b2.                                      */
    Terminal_HistoryIxLast  = 0u;                               /* See Notes #1b2.                                      */
    Terminal_HistoryIxShown = 0u;
    Terminal_HistoryEmpty   = DEF_YES;                          /* History empty (see Notes #1b).                       */
    Terminal_HistoryShown   = DEF_NO;                           /* History item NOT shown (see Notes #1a).              */

    Terminal_HistoryCnt     = 0u;
}
#endif


/*
*********************************************************************************************************
*                                      Terminal_HistoryPrevGet()
*
* Description : Copy previous history line into buffer.
*
* Argument(s) : pstr        String buffer.
*
* Return(s)   : none.
*
* Caller(s)   : Terminal_Task().
*
* Note(s)     : See 'Terminal_HistoryInit()  Note #1'.
*********************************************************************************************************
*/

#if (TERMINAL_CFG_HISTORY_EN == DEF_ENABLED)
static  void  Terminal_HistoryPrevGet (CPU_CHAR  *pstr)
{
    if (Terminal_HistoryShown == DEF_YES) {                     /* ---------- HISTORY ITEM DISP'D ON TERMINAL --------- */
                                                                /* If first item NOT shown ... show prev item.          */
        if (Terminal_HistoryIxShown != Terminal_HistoryIxFirst) {
            if (Terminal_HistoryIxShown == 0u) {
                Terminal_HistoryIxShown = TERMINAL_CFG_HISTORY_ITEMS_NBR - 1u;
            } else {
                Terminal_HistoryIxShown--;
            }
        }
        Str_Copy(pstr, Terminal_History[Terminal_HistoryIxShown]);




    } else {                                                    /* -------- HISTORY ITEM NOT DISP'D ON TERMINAL ------- */
        if (Terminal_HistoryEmpty == DEF_NO) {                  /* If history buf NOT empty ... disp last item.         */
            Terminal_HistoryShown   = DEF_YES;
            Terminal_HistoryIxShown = Terminal_HistoryIxLast;
            Str_Copy(pstr, Terminal_History[Terminal_HistoryIxShown]);
        } else {                                                /* If history buf empty ... clr str buf.                */
            Str_Copy(pstr, (CPU_CHAR *)"");
        }
    }
}
#endif


/*
*********************************************************************************************************
*                                      Terminal_HistoryNextGet()
*
* Description : Copy next history line into buffer.
*
* Argument(s) : pstr        String buffer.
*
* Return(s)   : none.
*
* Caller(s)   : Terminal_Task().
*
* Note(s)     : See 'Terminal_HistoryInit()  Note #1'.
*********************************************************************************************************
*/

#if (TERMINAL_CFG_HISTORY_EN == DEF_ENABLED)
static  void  Terminal_HistoryNextGet (CPU_CHAR  *pstr)
{
    if (Terminal_HistoryShown == DEF_YES) {                     /* ---------- HISTORY ITEM DISP'D ON TERMINAL --------- */
                                                                /* If last item is NOT being shown ... use next.        */
        if (Terminal_HistoryIxShown != Terminal_HistoryIxLast) {
            if (Terminal_HistoryIxShown == TERMINAL_CFG_HISTORY_ITEMS_NBR - 1u) {
                Terminal_HistoryIxShown = 0u;
            } else {
                Terminal_HistoryIxShown++;
            }
        }
        Str_Copy(pstr, Terminal_History[Terminal_HistoryIxShown]);
    }
}
#endif


/*
*********************************************************************************************************
*                                        Terminal_HistoryPut()
*
* Description : Copy buffer into history.
*
* Argument(s) : pstr        String buffer.
*
* Return(s)   : none.
*
* Caller(s)   : Terminal_Task().
*
* Note(s)     : See 'Terminal_HistoryInit()  Note #1'.
*********************************************************************************************************
*/

#if (TERMINAL_CFG_HISTORY_EN == DEF_ENABLED)
static  void  Terminal_HistoryPut (CPU_CHAR  *pstr)
{
    CPU_INT16U  cmp_val;


    Terminal_HistoryShown   = DEF_NO;
    Terminal_HistoryIxShown = 0u;

    cmp_val = Str_Cmp(pstr, (CPU_CHAR *)"");
    if (cmp_val != 0) {
        Terminal_HistoryCnt++;

        if (Terminal_HistoryEmpty == DEF_YES) {
            Str_Copy_N(Terminal_History[0], pstr, TERMINAL_CFG_HISTORY_ITEM_LEN - 1u);
            Terminal_History[0][TERMINAL_CFG_HISTORY_ITEM_LEN - 1u] = (CPU_CHAR)0;
            Terminal_HistoryEmpty = DEF_NO;
        } else {
                                                                /* Update ix of last item in history.                   */
            if (Terminal_HistoryIxLast == TERMINAL_CFG_HISTORY_ITEMS_NBR - 1) {
                Terminal_HistoryIxLast =  0u;
            } else {
                Terminal_HistoryIxLast++;
            }

                                                                /* Copy line into history.                              */
            Str_Copy_N(Terminal_History[Terminal_HistoryIxLast], pstr, TERMINAL_CFG_HISTORY_ITEM_LEN - 1u);
            Terminal_History[Terminal_HistoryIxLast][TERMINAL_CFG_HISTORY_ITEM_LEN - 1u] = (CPU_CHAR)0;

                                                                /* If last now first ... item overwr'n.                 */
            if (Terminal_HistoryIxFirst == Terminal_HistoryIxLast) {
                if (Terminal_HistoryIxFirst == TERMINAL_CFG_HISTORY_ITEMS_NBR - 1u) {
                    Terminal_HistoryIxFirst =  0u;
                } else {
                    Terminal_HistoryIxFirst++;
                }
            }
        }
    }
}
#endif


/*
*********************************************************************************************************
*                                            Terminal_fc()
*
* Description : Process the command history list.
*
* Argument(s) : argc        The number of arguments.
*
*               argv        Array of arguments.
*
*               out_fnct    The output function.
*
*               pcmd_param  Pointer to the command parameters.
*
* Return(s)   : SHELL_EXEC_ERR, if an error is encountered.
*               SHELL_ERR_NONE, otherwise.
*
* Caller(s)   : Shell, in response to command execution.
*
* Note(s)     : (1) (a) Usage(s)    : Term_fc
*
*                   (b) Argument(s) : none.
*
*                   (c) Output      : List of terminal history items.
*********************************************************************************************************
*/

#if (TERMINAL_CFG_HISTORY_EN == DEF_ENABLED)
static  CPU_INT16S  Terminal_fc (CPU_INT16U        argc,
                                 CPU_CHAR         *argv[],
                                 SHELL_OUT_FNCT    out_fnct,
                                 SHELL_CMD_PARAM  *pcmd_param)
{
    CPU_INT16U  history_cnt;
    CPU_INT16U  history_qty;
    CPU_INT16U  history_item_len;
    CPU_INT16U  history_ix;
    CPU_INT16U  i;
    CPU_CHAR    nbr_str[8];


                                                                /* ---------------- RESPOND TO HELP CMD --------------- */
    if (argc == 2u) {
        if (Str_Cmp(argv[1], TERMINAL_STR_HELP) == 0) {
            (void)out_fnct(TERMINAL_ARG_ERR_FC, (CPU_INT16U)Str_Len(TERMINAL_ARG_ERR_FC), pcmd_param->pout_opt);
            (void)out_fnct(TERMINAL_NEW_LINE,   2u,                                       pcmd_param->pout_opt);
            (void)out_fnct(TERMINAL_CMD_EXP_FC, (CPU_INT16U)Str_Len(TERMINAL_CMD_EXP_FC), pcmd_param->pout_opt);
            (void)out_fnct(TERMINAL_NEW_LINE,   2u,                                       pcmd_param->pout_opt);
            return (SHELL_ERR_NONE);
        }
    }



                                                                /* ----------------- HANDLE ARG QTY ERR --------------- */
    if (argc != 1u) {
        (void)out_fnct(TERMINAL_ARG_ERR_FC, (CPU_INT16U)Str_Len(TERMINAL_ARG_ERR_FC), pcmd_param->pout_opt);
        (void)out_fnct(TERMINAL_NEW_LINE,   2u,                                       pcmd_param->pout_opt);
        return (SHELL_EXEC_ERR);
    }



                                                                /* --------------- LIST TERMINAL HISTORY -------------- */
    if (Terminal_HistoryEmpty == DEF_YES) {                     /* If history empty ... rtn.                            */
        return (SHELL_ERR_NONE);
    }

                                                                /* Calc nbr of items in history.                        */
    if (Terminal_HistoryIxLast >= Terminal_HistoryIxFirst) {
        history_qty = Terminal_HistoryIxLast - Terminal_HistoryIxFirst + 1u;
    } else {
        history_qty = TERMINAL_CFG_HISTORY_ITEMS_NBR;
    }

    history_cnt = Terminal_HistoryCnt - history_qty;
    history_ix  = Terminal_HistoryIxFirst;
    for (i = 0u; i < history_qty; i++) {                        /* List each history item & item cnt.                   */
        (void)Str_FmtNbr_Int32U(history_cnt,
                                7u,
                                DEF_NBR_BASE_DEC,
                                ASCII_CHAR_SPACE,
                                DEF_NO,
                                DEF_YES,
                                nbr_str);

        history_item_len = (CPU_INT16U)Str_Len(Terminal_History[history_ix]);
        (void)out_fnct(nbr_str,                      7u,               pcmd_param->pout_opt);
        (void)out_fnct((CPU_CHAR *)"     ",          5u,               pcmd_param->pout_opt);
        (void)out_fnct(Terminal_History[history_ix], history_item_len, pcmd_param->pout_opt);
        (void)out_fnct(TERMINAL_NEW_LINE,            2u,               pcmd_param->pout_opt);

        history_cnt++;
        history_ix++;
        if (history_ix == TERMINAL_CFG_HISTORY_ITEMS_NBR) {
            history_ix =  0u;
        }
    }

    return (SHELL_ERR_NONE);
}
#endif
