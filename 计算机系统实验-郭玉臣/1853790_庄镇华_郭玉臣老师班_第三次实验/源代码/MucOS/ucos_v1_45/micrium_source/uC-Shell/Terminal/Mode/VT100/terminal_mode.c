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
*                                           VT100 MODE PORT
*
* Filename      : terminal_mode.c
* Version       : V1.03.01
* Programmer(s) : BAN
*********************************************************************************************************
* Note(s)       : (1) ECMA-48 'Control Functions for Coded Character Sets' (5th edition), standardizes
*                     a set of terminal emulation commands.  The ISO/IEC and ANSI issued corresponding
*                     standards, ISO/IEC 6429 and ANSI X3.64, respectively.  The DEC VT100 video terminal
*                     implemented a command set which conformed to ANSI X3.64, so this type of terminal
*                     emulation is often known as VT100.
*
*                 (2) To use this terminal mode, the terminal program employed (Hyperterminal, puTTY,
*                     etc.) should be setup in VT100 mode and character echo should be disabled.
*                     Depending on the command set used, it may be necessary to generate a CR ('\r') on
*                     each LF ('\n').
*
*                 (3) Only the several ECMA-48 commands relevant to terminal operation are supported.
*                     These are :
*
*                     (a) Cursor Up.
*                     (b) Cursor Down.
*                     (c) Cursor Left.
*                     (d) Cursor Right.
*
*                     In addition, several editing keys are supported :
*
*                     (a) Insert.
*                     (b) Delete.
*                     (c) End.
*                     (d) Home.
*
*                     ($$$$ Where is this functionality specified?)
*
*                 (4) Only 7-bit mode is supported.
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <terminal.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  TERMINAL_VT100_ESC_CHAR                        0x1Bu

/*
*********************************************************************************************************
*                                           C0 COMMAND SET
*********************************************************************************************************
*/

#define  TERMINAL_VT100_C0_BS                           0x08u   /* Backspace.                                           */
#define  TERMINAL_VT100_C0_LF                           0x0Au   /* Line feed.                                           */
#define  TERMINAL_VT100_C0_CR                           0x0Du   /* Carriage return.                                     */

#define  TERMINAL_VT100_C0_ESC                          0x1Bu   /* Escape.                                              */

/*
*********************************************************************************************************
*                                           C1 COMMAND SET
*********************************************************************************************************
*/

#define  TERMINAL_VT100_C1_CSI                          0x5Bu   /* Control sequence  introducer.                        */

/*
*********************************************************************************************************
*                                 CONTROL SEQUENCE BYTE VALUE LIMITS
*********************************************************************************************************
*/

#define  TERMINAL_VT100_PB_MIN                          0x30
#define  TERMINAL_VT100_PB_MAX                          0x3F
#define  TERMINAL_VT100_IB_MIN                          0x20
#define  TERMINAL_VT100_IB_MAX                          0x2F
#define  TERMINAL_VT100_FB_MIN                          0x40
#define  TERMINAL_VT100_FB_MAX                          0x7E

/*
*********************************************************************************************************
*                              NO PARAMETER CONTROL SEQUENCE FINAL BYTE
*********************************************************************************************************
*/

#define  TERMINAL_VT100_NP_CUU                          0x41u   /* Cursor up.                                           */
#define  TERMINAL_VT100_NP_CUD                          0x42u   /* Cursor down.                                         */
#define  TERMINAL_VT100_NP_CUF                          0x43u   /* Cursor right.                                        */
#define  TERMINAL_VT100_NP_CUB                          0x44u   /* Cursor left.                                         */

/*
*********************************************************************************************************
*                                    CONTROL & FUNCTION KEY VALUES
*********************************************************************************************************
*/

#define  TERMINAL_VT100_KEY_HOME                        0x31u
#define  TERMINAL_VT100_KEY_INSERT                      0x32u
#define  TERMINAL_VT100_KEY_DELETE                      0x33u
#define  TERMINAL_VT100_KEY_END                         0x34u
#define  TERMINAL_VT100_KEY_PAGEDOWN                    0x35u
#define  TERMINAL_VT100_KEY_PAGEUP                      0x36u

#define  TERMINAL_VT100_P1_KEY                          0x7Eu


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/

static  CPU_INT08U  Terminal_VT100_EscStrLeft[]  = {TERMINAL_VT100_ESC_CHAR,
                                                    TERMINAL_VT100_C1_CSI,
                                                    TERMINAL_VT100_NP_CUB};

static  CPU_INT08U  Terminal_VT100_EscStrRight[] = {TERMINAL_VT100_ESC_CHAR,
                                                    TERMINAL_VT100_C1_CSI,
                                                    TERMINAL_VT100_NP_CUF};


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL MACRO'S
*********************************************************************************************************
*/

#define  TERMINAL_VT100_IS_PARAMETER_BYTE(b)        ((((b) >= TERMINAL_VT100_PB_MIN) && ((b) <= TERMINAL_VT100_PB_MAX)) ? DEF_YES : DEF_NO)

#define  TERMINAL_VT100_IS_INTERMEDIATE_BYTE(b)     ((((b) >= TERMINAL_VT100_IB_MIN) && ((b) <= TERMINAL_VT100_IB_MAX)) ? DEF_YES : DEF_NO)

#define  TERMINAL_VT100_IS_FINAL_BYTE(b)            ((((b) >= TERMINAL_VT100_FB_MIN) && ((b) <= TERMINAL_VT100_FB_MAX)) ? DEF_YES : DEF_NO)


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                        TerminalMode_RdLine()
*
* Description : Read a line from the serial interface.
*
* Argument(s) : pstr            Pointer to a buffer at which the string can be stored.
*
*               len_max         Size of the string that will be read.
*
*               pcursor_pos     Pointer to variable that specifies    the current cursor position
*                                   AND
*                               Pointer to variable that will receive the final   cursor position.
*
*               ins             Indicates insertion mode :
*
*                                   DEF_YES,  insert on.
*                                   DEF_NO, insert off.
*
* Return(s)   : Type of escape sequence encountered :
*
*                   TERMINAL_ESC_TYPE_NONE  No escape sequence.
*                   TERMINAL_ESC_TYPE_UP    'Up'   arrow key sequence (move to previous history element).
*                   TERMINAL_ESC_TYPE_DOWN  'Down' arrow key sequence (move to next history element).
*                   TERMINAL_ESC_TYPE_INS   Insert mode toggled.
*
* Caller(s)   : Terminal_Task().
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT08U  TerminalMode_RdLine (CPU_CHAR     *pstr,
                                 CPU_SIZE_T    len_max,
                                 CPU_SIZE_T   *pcursor_pos,
                                 CPU_BOOLEAN   ins)
{
    CPU_CHAR    cmd[10];
    CPU_CHAR    cmd_end;
    CPU_SIZE_T  cmd_ix;
    CPU_CHAR    cursor_char;
    CPU_SIZE_T  cursor_pos;
    CPU_CHAR    in_char;
    CPU_INT08U  rtn_val;
    CPU_SIZE_T  str_len;
    CPU_SIZE_T  str_ix;


    rtn_val    =  TERMINAL_ESC_TYPE_NONE;
    cursor_pos = *pcursor_pos;

    while (DEF_TRUE) {
        in_char = TerminalSerial_RdByte();

        switch (in_char) {
            case TERMINAL_VT100_C0_CR:                          /* ------------------- NEW LINE CHAR ------------------ */
            case TERMINAL_VT100_C0_LF:
                 str_len       = Str_Len(pstr);
                 pstr[str_len] = ASCII_CHAR_NULL;
                *pcursor_pos   = str_len;
                 rtn_val       = TERMINAL_ESC_TYPE_NONE;
                 return (rtn_val);



            case TERMINAL_VT100_C0_BS:                          /* ------------------ BACKSPACE CHAR ------------------ */
                 if (cursor_pos > 0u) {
                     if (ins == DEF_FALSE) {                    /* Clr prev char.                                       */
                         Terminal_WrStr((CPU_CHAR *)"\b \b", 3u);
                         if (pstr[cursor_pos] == ASCII_CHAR_NULL) {
                             pstr[cursor_pos - 1u] = ASCII_CHAR_NULL;
                         } else {
                             pstr[cursor_pos - 1u] = ASCII_CHAR_SPACE;
                         }
                         cursor_pos--;
                     } else {                                   /* Del prev char.                                       */
                         str_len = Str_Len(pstr);
                         Terminal_WrChar(TERMINAL_VT100_C0_BS);
                         if (str_len > cursor_pos) {
                             Terminal_WrStr(&pstr[cursor_pos], str_len - cursor_pos);
                         }
                         Terminal_WrChar(ASCII_CHAR_SPACE);
                         for (str_ix = cursor_pos; str_ix <= str_len; str_ix++) {
                             Terminal_WrChar(TERMINAL_VT100_C0_BS);
                             pstr[str_ix - 1u] = pstr[str_ix];
                         }
                         cursor_pos--;
                     }
                 }
                 break;



            case TERMINAL_VT100_C0_ESC:                         /* -------------------- ESCAPE CHAR ------------------- */
                 in_char = TerminalSerial_RdByte();             /* Rd esc'd char.                                       */
                 if (in_char == TERMINAL_VT100_C1_CSI) {        /* Cmd seq intro.                                       */
                     cmd_ix = 0u;
                                                                /* Rd param byte's.                                     */
                     in_char  = TerminalSerial_RdByte();
                     while ((TERMINAL_VT100_IS_PARAMETER_BYTE(in_char) == DEF_YES) && (cmd_ix < sizeof(cmd))) {
                         cmd[cmd_ix] = in_char;
                         cmd_ix++;
                         in_char     = TerminalSerial_RdByte();
                     }
                                                                /* Rd intermediate byte's.                              */
                     while ((TERMINAL_VT100_IS_INTERMEDIATE_BYTE(in_char) == DEF_YES) && (cmd_ix < sizeof(cmd))) {
                         cmd[cmd_ix] = in_char;
                         cmd_ix++;
                         in_char     = TerminalSerial_RdByte();
                     }

                     cmd_end = in_char;                         /* Cmd end byte.                                        */



                     switch (cmd_end) {
                         case TERMINAL_VT100_NP_CUU:            /* --------------------- CURSOR UP -------------------- */
                             *pcursor_pos = cursor_pos;
                              rtn_val     = TERMINAL_ESC_TYPE_UP;
                              return (rtn_val);                 /* Terminal history will handle cmd.                    */



                         case TERMINAL_VT100_NP_CUD:            /* -------------------- CURSOR DOWN ------------------- */
                             *pcursor_pos = cursor_pos;
                              rtn_val     = TERMINAL_ESC_TYPE_DOWN;
                              return (rtn_val);                 /* Terminal history will handle cmd.                    */



                         case TERMINAL_VT100_NP_CUF:            /* ------------------- CURSOR RIGHT ------------------- */
                              if (cursor_pos < Str_Len(pstr)) { /* Move cursor right one pos.                           */
                                  cursor_pos++;
                                  Terminal_WrStr((CPU_CHAR *)Terminal_VT100_EscStrRight, 3u);
                              }
                              break;



                         case TERMINAL_VT100_NP_CUB:            /* ------------------- CURSOR LEFT -------------------- */
                              if (cursor_pos > 0u) {            /* Move cursor left one pos.                            */
                                  cursor_pos--;
                                  Terminal_WrStr((CPU_CHAR *)Terminal_VT100_EscStrLeft, 3u);
                              }
                              break;

                         case TERMINAL_VT100_P1_KEY:
                              if (cmd_ix == 1u) {
                                  switch (cmd[0]) {
                                      case TERMINAL_VT100_KEY_INSERT:   /* ------------------ INSERT ------------------ */
                                          *pcursor_pos = cursor_pos;
                                           rtn_val     = TERMINAL_ESC_TYPE_INS;
                                           return (rtn_val);            /* Terminal will handle cmd.                    */



                                      case TERMINAL_VT100_KEY_HOME:     /* ------------------- HOME ------------------- */
                                           while (cursor_pos > 0u) {    /* Move cursor to first char on line.           */
                                               Terminal_WrStr((CPU_CHAR *)Terminal_VT100_EscStrLeft, 3u);
                                               cursor_pos--;
                                           }
                                           break;



                                      case TERMINAL_VT100_KEY_DELETE:   /* ------------------ DELETE ------------------ */
                                           str_len = Str_Len(pstr);     /* Delete char at cursor.                       */
                                           if (str_len > cursor_pos) {
                                               Terminal_WrStr(&pstr[cursor_pos + 1u], str_len - cursor_pos);
                                               Terminal_WrStr((CPU_CHAR *)" \b", 2u);
                                               for (str_ix = cursor_pos + 1; str_ix < str_len; str_ix++) {
                                                  pstr[str_ix - 1u] = pstr[str_ix];
                                                  Terminal_WrStr((CPU_CHAR *)Terminal_VT100_EscStrLeft, 3u);
                                               }
                                               pstr[str_len - 1u] = (CPU_CHAR)0;
                                               if (str_len == cursor_pos) {
                                                   cursor_pos--;
                                               }
                                           }
                                           break;



                                      case TERMINAL_VT100_KEY_END:      /* -------------------- END ------------------- */
                                           str_len = Str_Len(pstr);     /* Move cursor to last char on line.            */
                                           while (cursor_pos < str_len) {
                                               Terminal_WrStr((CPU_CHAR *)Terminal_VT100_EscStrRight, 3u);
                                               cursor_pos++;
                                           }
                                           break;

                                      default:
                                           break;
                                  }
                              }
                              break;

                         default:
                              break;
                     }
                 }
                 break;


            default:                                            /* -------------------- OTHER CHAR -------------------- */
                 if (ASCII_IsPrint(in_char) == DEF_YES){        /* Print printable char.                                */
                                                                /* Ovwr char at cursor's pos.                           */
                     if (cursor_pos < len_max) {
                         if (ins == DEF_NO) {
                             Terminal_WrChar(in_char);
                             cursor_char      = pstr[cursor_pos];
                             pstr[cursor_pos] = in_char;
                             if (cursor_char == ASCII_CHAR_NULL) {  /* If char at cursor was NULL, wr new NULL.         */
                                 pstr[cursor_pos + 1u] = ASCII_CHAR_NULL;
                             }
                             cursor_pos++;

                         } else {                                   /* Ins char at cursor's pos.                        */
                             str_len = Str_Len(pstr);               /* Shift str right.                                 */
                             if (str_len == len_max) {              /* Handle buf ovf.                                  */
                                 str_len =  len_max - 1u;
                             }
                             for (str_ix = str_len; str_ix > cursor_pos; str_ix--) {
                                 pstr[str_ix] = pstr[str_ix - 1u];
                             }
                             pstr[cursor_pos]   = in_char;          /* Ins new char.                                    */
                             pstr[str_len + 1u] = ASCII_CHAR_NULL;

                             if (str_len > cursor_pos) {            /* Wr str to terminal.                              */
                                 Terminal_WrStr(&pstr[cursor_pos], str_len - cursor_pos);
                             }
                                                                    /* Place cursor at old pos.                         */
                             for (str_ix = cursor_pos; str_ix < str_len; str_ix++)  {
                                 Terminal_WrChar(ASCII_CHAR_BACKSPACE);
                             }
                             cursor_pos++;
                         }
                     }
                 }
                 break;
        }
    }
}


/*
*********************************************************************************************************
*                                         TerminalMode_Clr()
*
* Description : Clear the terminal line.
*
* Argument(s) : nbr_char    Number of characters on line.
*
*               cursor_pos  Current cursor position.
*
* Return(s)   : none.
*
* Caller(s)   : Terminal_Task().
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  TerminalMode_Clr (CPU_SIZE_T  nbr_char,
                        CPU_SIZE_T  cursor_pos)
{
    CPU_SIZE_T  ix;


    for (ix = cursor_pos; ix < nbr_char; ix++) {
        Terminal_WrStr((CPU_CHAR *)" ", 1u);
    }
    for (ix = 0; ix < nbr_char; ix++) {
        Terminal_WrStr((CPU_CHAR *)"\b \b", 3u);
    }
}


/*
*********************************************************************************************************
*                                       TerminalMode_NewLine()
*
* Description : Move terminal to new line.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Terminal_Task().
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  TerminalMode_NewLine (void)
{
    Terminal_WrStr((CPU_CHAR *)"\r\n", 2u);
}


/*
*********************************************************************************************************
*                                        TerminalMode_Prompt()
*
* Description : Show prompt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Terminal_Task().
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  TerminalMode_Prompt (void)
{
    Terminal_WrStr((CPU_CHAR *)"\r\n> ", 4u);
}
