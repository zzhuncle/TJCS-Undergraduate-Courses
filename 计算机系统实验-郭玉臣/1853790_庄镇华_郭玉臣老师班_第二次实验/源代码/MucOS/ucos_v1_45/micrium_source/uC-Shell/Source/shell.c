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
* Filename      : shell.c
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
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define    SHELL_MODULE
#include  <shell.h>

#include  <lib_def.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           INITIALIZED DATA
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  CPU_INT16U      Shell_Scanner          (CPU_CHAR           *in,
                                                CPU_CHAR           *arg_tbl[],
                                                CPU_INT16U          arg_tbl_size,
                                                SHELL_ERR          *perr);

static  SHELL_CMD_FNCT  Shell_CmdSearch        (CPU_CHAR           *cmd_name,
                                                SHELL_ERR          *perr);

static  void            Shell_ModuleCmdNameGet (CPU_CHAR           *cmd_name,
                                                CPU_CHAR            module_cmd_name[],
                                                CPU_INT16U          len,
                                                SHELL_ERR          *perr);

static  void            Shell_ModuleCmdClr      (SHELL_MODULE_CMD  *pmodule_cmd);


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             Shell_Init()
*
* Description : Initialize the shell.
*
* Arguments   : none.
*
* Returns     : DEF_OK          Shell initialization successful.
*               DEF_FAIL        Shell server initialization failed.
*
* Caller(s)   : Your Product's Application.
*
*               This function is a Shell initialization function & MAY be called by
*               application/initialization function(s).
*
* Note(s)     : (1) Shell_Init() MUST be called ...
*
*                   (a) BEFORE the other Shell function are invoked.
*
*               (2) Shell_Init() MUST ONLY be called ONCE from product's application.
*
*               (3) Module command pools MUST be initialized PRIOR to initializing the pool with 
*                   pointers to module commands.
*********************************************************************************************************
*/

CPU_BOOLEAN  Shell_Init (void)
{
    CPU_INT16U         i;
    SHELL_MODULE_CMD  *pmodule_cmd;


                                                                /* ---------------- INIT SHELL CMD TBL ---------------- */
    Shell_ModuleCmdUsedPoolPtr = DEF_NULL;                      /* Init-clr module cmd pools    (see Note #3).          */
    Shell_ModuleCmdFreePoolPtr = DEF_NULL;
    
    pmodule_cmd = &Shell_ModuleCmdTbl[0];
    for (i = 0; i < SHELL_CFG_CMD_TBL_SIZE; i++) {              /* Free each module cmd to pool (see Note #3).          */
        Shell_ModuleCmdClr(pmodule_cmd);

                                                                /* Init doubly-linked list.                             */
        pmodule_cmd->PrevModuleCmdPtr = DEF_NULL;
        pmodule_cmd->NextModuleCmdPtr = Shell_ModuleCmdFreePoolPtr;

        if (Shell_ModuleCmdFreePoolPtr != DEF_NULL) {
            Shell_ModuleCmdFreePoolPtr->PrevModuleCmdPtr = pmodule_cmd;
        }

        Shell_ModuleCmdFreePoolPtr = pmodule_cmd;


        pmodule_cmd++;
    }
    
    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                             Shell_Exec()
*
* Description : (1) Parse and execute command passed in parameter :
*
*                   (a) Parse input string
*                   (b) Search  command
*                   (c) Execute command
*
*
* Argument(s) : in              Pointer to a CPU_CHAR string holding a complete command and its argument(s).
*               out_funt        Pointer to 'output' function used by command (see Note #1).
*               pcmd_param      Pointer to command additional parameters.
*               perr            Pointer to variable that will receive the return error code from this
*                               function :
*
*                               SHELL_ERR_NONE              No error. 
*                               SHELL_ERR_NULL_PTR          Argument 'in'       passed a NULL pointer.
*                               SHELL_ERR_CMD_NOT_FOUND     Command NOT found.
*                               SHELL_ERR_CMD_SEARCH        Error searching for command.
*                               SHELL_ERR_CMD_EXEC          Error executing     command.
*
*                                                                       ----- RETURNED BY Shell_Scanner() : -----
*                               SHELL_ERR_ARG_TBL_FULL      Argument table full and token still to be parsed.
*
* Return(s)   : Command specific return value.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) The command may generate some output that should be transmitted to some device (socket,
*                   RS-232 link, ...).  The caller of this function is hence responsible for the
*                   implementation of such function, if output is desired.
*********************************************************************************************************
*/

CPU_INT16S  Shell_Exec (CPU_CHAR         *in,
                        SHELL_OUT_FNCT    out_fnct,
                        SHELL_CMD_PARAM  *pcmd_param,
                        SHELL_ERR        *perr)
{
    CPU_INT16U      argc;    
    CPU_CHAR       *argv[SHELL_CFG_CMD_ARG_NBR_MAX];
    SHELL_CMD_FNCT  cmd_fnct;
    CPU_INT16S      ret_val;


                                                                /* ------------------- VALIDATE PTR ------------------- */
    if (in == DEF_NULL) {
       *perr = SHELL_ERR_NULL_PTR;
        return (0);
    }


                                                                /* ------------------ PARSE INPUT STR ----------------- */
    argc = Shell_Scanner(in,
                         argv,
                         SHELL_CFG_CMD_ARG_NBR_MAX,
                         perr);

    if (*perr != SHELL_ERR_NONE) {
        return (0);
    } else if (argc == 0) {
       *perr = SHELL_ERR_MODULE_CMD_EMPTY;
        return (0);
    }


                                                                /* -------------------- SEARCH CMD -------------------- */
    cmd_fnct = Shell_CmdSearch(argv[0], perr);
    switch (*perr) {
        case SHELL_ERR_NONE:
             break;

        case SHELL_ERR_MODULE_CMD_NOT_FOUND:
        case SHELL_ERR_CMD_NOT_FOUND:
        case SHELL_ERR_MODULE_CMD_NAME_NONE:
        case SHELL_ERR_MODULE_CMD_NAME_TOO_LONG:
            *perr = SHELL_ERR_CMD_NOT_FOUND;
             return (0);

        case SHELL_ERR_MODULE_CMD_NAME_COPY:
        default:
            *perr = SHELL_ERR_CMD_SEARCH;
             return (0);
    }


                                                                /* -------------------- EXECUTE CMD ------------------- */
    ret_val = cmd_fnct(argc, argv, out_fnct, pcmd_param);
    
    if (ret_val == SHELL_EXEC_ERR) {
       *perr = SHELL_ERR_CMD_EXEC;
    }

    return (ret_val);
}


/*
*********************************************************************************************************
*                                           Shell_CmdTblAdd()
*
* Description : (1) Allocate & initialize a module command :
*
*                   (a) Validate   module command
*                   (b) Get a free module command
*                   (c) Initialize module command
*                   (d) Add to     module command used pool.
*
*               (2) The module command pools are implemented as doubly-linked lists :
*
*                   (a) 'Shell_ModuleCmdUsedPoolPtr' and 'Shell_ModuleCmdFreePoolPtr' point to the head
*                       their module command pool.
*
*                   (b) Module command NextModuleCmdPtr's and PrevModuleCmdPtr's link each command to form
*                       the module command pool doubly-linked list.
*
*                   (c) Module command are inserted & removed at the head of the module command pool lists.
*
*
*                                    Module commands are
*                                    inserted & removed
*                                        at the head
*                                      (see Note #2c)
*
*                                             |            NextModuleCmdPtr
*                                             |             (see Note #2b)
*                                             v                    |
*                                                                  |
*                                          -------       -------   v   -------       -------
*                 Module command Pool ---->|     |------>|     |------>|     |------>|     |
*                      Pointer             |     |       |     |       |     |       |     |
*                                          |     |<------|     |<------|     |<------|     |
*                   (see Note #2a)         -------       -------       -------   ^    -------
*                                                                                |
*                                                                                |
*                                                                         PrevModuleCmdPtr
*                                                                          (see Note #2b)
*
*                                          |                                               |
*                                          |<-----Pool of Free/Used Module Commands ------>|
*                                          |                (see Note #2)                  |
*
*
*
* Argument(s) : cmd_tbl_name    Pointer to character string representing the name of the command table.
*               cmd_tbl         Command table to add.
*               perr            Pointer to variable that will receive the return error code from this
*                               function :
*
*                               SHELL_ERR_NONE                      No error.
*                               SHELL_ERR_NULL_PTR                  Argument 'cmd_tbl' passed a NULL pointer.
*                               SHELL_ERR_MODULE_CMD_EMPTY          Command table empty.
*                               SHELL_ERR_MODULE_CMD_ALREADY_IN     Command table already added, or command
*                                                                   table name already used.
*                               SHELL_ERR_MODULE_CMD_NONE_AVAIL     NO available module command to allocate.
*
*                                                                       ----- RETURNED BY Shell_ModuleCmdNameGet() : -----
*                               SHELL_ERR_MODULE_CMD_NAME_NONE      NO module command name found.
*                               SHELL_ERR_MODULE_CMD_NAME_TOO_LONG  Module command name too long.
*                               SHELL_ERR_MODULE_CMD_NAME_COPY      Copy error.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (3) The 'name' argument is the prefix of the commands in 'cmd_tbl'.  In order to speed up
*                   the command search, the shell first locate the appropriate table based on the prefix
*                   of the command.  Hence, it is mandatory that all commands in a table be named with
*                   the same prefix.  For instance, uC/TCP-IP related command displaying statistics should
*                   look like :
*
*                       Net_stats
*
*                   while a file system command listing the current directory would be :
*
*                       FS_ls
*
*                   The names of those module commands are respectively 'Net' and 'FS'.
*
*               (4) #### The 'cmd_tbl_name' parameter is not mandatory in the current implementation.
*                   Although you could pass a 'NULL' value for this parameter, it is recommended to provide
*                   the prefix of the commands in 'cmd_tbl' for future compatibility.
*
*                   However, passing NULL for this parameter will result in the first command prefix to
*                   be extracted and used as the command table name.
*
*               (5) If an empty character array is passed in the cmd_tbl_name parameter, the function
*                   will extract the first command prefix to use as the command table name.
*********************************************************************************************************
*/

void  Shell_CmdTblAdd (CPU_CHAR   *cmd_tbl_name,
                       SHELL_CMD   cmd_tbl[],
                       SHELL_ERR  *perr)
{
    SHELL_CMD         *pcmd;
    CPU_SIZE_T         name_len;
    CPU_CHAR           tbl_name[SHELL_CFG_MODULE_CMD_NAME_LEN_MAX];
    CPU_CHAR          *ptbl_name;
    SHELL_MODULE_CMD  *pmodule_cmd;
    CPU_INT16S         name_compare;


                                                                /* ------------------- VALIDATE PTR ------------------- */
    if (cmd_tbl == DEF_NULL) {
       *perr = SHELL_ERR_NULL_PTR;
        return;
    }

    if (cmd_tbl[0].Fnct == (SHELL_CMD_FNCT)0) {                 /* If cmd tbl empty ...                                 */
       *perr = SHELL_ERR_MODULE_CMD_EMPTY;                      /* ... rtn err.                                         */
        return;
    }


    ptbl_name = DEF_NULL;

    if (cmd_tbl_name != DEF_NULL) {                        /* If cmd_tbl_name not null ...                         */
        name_len = Str_Len(cmd_tbl_name);
        if (name_len >= SHELL_CFG_MODULE_CMD_NAME_LEN_MAX) {    /* ... If name too long ...                             */
           *perr = SHELL_ERR_MODULE_CMD_NAME_TOO_LONG;          /*     ... rtn err.                                     */
            return;
        
        } else if (name_len > 0) {                              /*     ... else if name greater of 0 ...                */
            ptbl_name = cmd_tbl_name;                           /*     ... use as tbl name.                             */
        }
    }

    if (ptbl_name == DEF_NULL) {                           /* If cmd tbl name not gotten from param ...            */
        pcmd = &cmd_tbl[0];
        Shell_ModuleCmdNameGet((CPU_CHAR *)pcmd->Name,          /* ... get name from first command.                     */
                                           tbl_name,
                                           SHELL_CFG_MODULE_CMD_NAME_LEN_MAX,
                                           perr);

        if (*perr != SHELL_ERR_NONE) {
            return;
        }
        ptbl_name = tbl_name;
    }


                                                                /* -------------- CHK FOR DUPLICATE ENTRY ------------- */
    pmodule_cmd = Shell_ModuleCmdUsedPoolPtr;
    while (pmodule_cmd != DEF_NULL) {
        name_compare = Str_Cmp(pmodule_cmd->Name, ptbl_name);

        if ((pmodule_cmd->CmdTblPtr == cmd_tbl) ||              /* If module name already used ...                      */
            (name_compare           == 0      )) {

           *perr = SHELL_ERR_MODULE_CMD_ALREADY_IN;             /* ... rtn err.                                         */
            return;
        } 

        pmodule_cmd = pmodule_cmd->NextModuleCmdPtr;
    }


                                                                /* ------------------ GET MODULE CMD ------------------ */
    if (Shell_ModuleCmdFreePoolPtr != DEF_NULL) {               /* If module cmd pool NOT empty ...                     */
                                                                /* ... get one from pool.                               */
        pmodule_cmd                 = Shell_ModuleCmdFreePoolPtr;
        Shell_ModuleCmdFreePoolPtr  = pmodule_cmd->NextModuleCmdPtr;

    } else {                                                    /* If none avail ...                                    */
       *perr =  SHELL_ERR_MODULE_CMD_NONE_AVAIL;                /* ... rtn err.                                         */
        return;
    }


                                                                /* ----------------- INIT MODULE CMD ------------------ */
    Str_Copy(pmodule_cmd->Name, ptbl_name);
    pmodule_cmd->CmdTblPtr = cmd_tbl;


                                                                /* ---------- ADD TO MODULE CMD TBL USED POOL --------- */
                                                                /* Update doubly-linked list.                           */
    pmodule_cmd->PrevModuleCmdPtr = DEF_NULL;
    pmodule_cmd->NextModuleCmdPtr = Shell_ModuleCmdUsedPoolPtr;

    if (Shell_ModuleCmdUsedPoolPtr != DEF_NULL) {
        Shell_ModuleCmdUsedPoolPtr->PrevModuleCmdPtr = pmodule_cmd;
    }

    Shell_ModuleCmdUsedPoolPtr = pmodule_cmd;


   *perr = SHELL_ERR_NONE;
}


/*
*********************************************************************************************************
*                                           Shell_CmdTblRem()
*
* Description : (1) Remove a module command  :
*
*                   (a) Search module command
*                   (b) Remove module command
*                   (c) Update module command pools
*
*
* Argument(s) : cmd_tbl_name    Pointer to character string representing the name of the command table.
*               perr            Pointer to variable that will receive the return error code from this
*                               function :
*
*                               SHELL_ERR_NONE                  No error.
*                               SHELL_ERR_NULL_PTR              Argument 'cmd_tbl_name' passed a NULL pointer.
*                               SHELL_ERR_MODULE_CMD_NOT_FOUND  Module command NOT found.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  Shell_CmdTblRem (CPU_CHAR   *cmd_tbl_name,
                       SHELL_ERR  *perr)
{
    SHELL_MODULE_CMD  *pmodule_cmd_list;
    SHELL_MODULE_CMD  *pmodule_cmd;
    CPU_INT16S         name_compare;


                                                                /* ------------------- VALIDATE PTR ------------------- */
    if (cmd_tbl_name == DEF_NULL) {
       *perr = SHELL_ERR_NULL_PTR;
        return;
    }

                                                                /* ----------------- SEARCH MODULE CMD ---------------- */  
    pmodule_cmd_list = Shell_ModuleCmdUsedPoolPtr;
    pmodule_cmd      = DEF_NULL;

    while (pmodule_cmd_list != DEF_NULL) {
        name_compare = Str_Cmp(cmd_tbl_name, pmodule_cmd_list->Name);
        if (name_compare == 0) {
            pmodule_cmd = pmodule_cmd_list;
            break;
        }
        pmodule_cmd_list = pmodule_cmd_list->NextModuleCmdPtr;
    }

    if (pmodule_cmd == DEF_NULL) {
       *perr = SHELL_ERR_MODULE_CMD_NOT_FOUND;
        return;
    }


                                                                /* ----------- REM MODULE CMD & UPDATE POOLS ---------- */
                                                                /* Update used module cmd pool.                         */
    if (pmodule_cmd->PrevModuleCmdPtr != DEF_NULL) {           /* If prev NOT NULL ... set prev's next to next.        */
        (pmodule_cmd->PrevModuleCmdPtr)->NextModuleCmdPtr = pmodule_cmd->NextModuleCmdPtr;
    } else {                                                    /* Else ...             set used pool ptr to next.      */
        Shell_ModuleCmdUsedPoolPtr = pmodule_cmd->NextModuleCmdPtr;
    }

    if (pmodule_cmd->NextModuleCmdPtr != DEF_NULL) {           /* If next NOT NULL ... set next's prev to prev.        */
        (pmodule_cmd->NextModuleCmdPtr)->PrevModuleCmdPtr = pmodule_cmd->PrevModuleCmdPtr;
    }

                                                                /* Update free module cmd pool.                         */
    Shell_ModuleCmdClr(pmodule_cmd);
    pmodule_cmd->NextModuleCmdPtr = Shell_ModuleCmdFreePoolPtr;

    if (Shell_ModuleCmdFreePoolPtr != DEF_NULL) {
        Shell_ModuleCmdFreePoolPtr->PrevModuleCmdPtr = pmodule_cmd;
    }

    Shell_ModuleCmdFreePoolPtr = pmodule_cmd;


   *perr = SHELL_ERR_NONE;
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
*                                           Shell_Scanner()
*
* Description : Scan and parse the command line.
*
* Argument(s) : in              Pointer to a NUL terminated string holding a complete command and its
*                               argument(s).
*               arg_tbl         Array of pointer that will receive pointers to token.
*               arg_tbl_size    Size of arg_tbl array.
*               perr            Pointer to variable that will receive the return error code from this
*                               function :
*
*                               SHELL_ERR_NONE              No error. 
*                               SHELL_ERR_ARG_TBL_FULL      Argument table full and token still to be parsed.
*
* Return(s)   : Number of token(s) (command name and argument(s)).
*
* Caller(s)   : Shell_Exec().
*
* Note(s)     : (1) The first token is always the command name itself.
*
*               (2) This function modify the 'in' arguments by replacing token's delimiter characters by
*                   termination character ('\0').
*********************************************************************************************************
*/

static  CPU_INT16U  Shell_Scanner (CPU_CHAR   *in,
                                   CPU_CHAR   *arg_tbl[],
                                   CPU_INT16U  arg_tbl_size,
                                   SHELL_ERR  *perr)
{
    CPU_CHAR    *in_rd;
    CPU_INT16U   tok_ix;
    CPU_BOOLEAN  end_tok_found;
    CPU_BOOLEAN  quote_opened;


    in_rd            = in;
    tok_ix           = 0;
    end_tok_found    = DEF_YES;
    quote_opened     = DEF_NO;    

                                                                /* ------------------ SCAN CMD LINE  ------------------ */
    while (*in_rd) {
        switch (*in_rd) {
            case SHELL_ASCII_QUOTE:                             /* Quote char found.                                    */
                 if (quote_opened == DEF_YES) {
                      quote_opened  = DEF_NO;
                     *in_rd         = (CPU_CHAR)0;
                      end_tok_found = DEF_YES;
                 } else {
                      quote_opened   = DEF_YES;
                 }
                 
                 break;

            case SHELL_ASCII_SPACE:                             /* Space char found.                                    */
                 if ((end_tok_found == DEF_NO) &&               /* If first space between tok && quote NOT opened ...   */
                     (quote_opened  == DEF_NO)) {                           
                     *in_rd         =  SHELL_ASCII_ARG_END;   /* ... put termination char.                            */    
                      end_tok_found =  DEF_YES;   
                 }

                 break;

            default:                                            /* Other char found ...                                 */
                 if (end_tok_found == DEF_YES) {
                     if (tok_ix < arg_tbl_size) {
                         arg_tbl[tok_ix] = in_rd;               /* Set arg_tbl ptr to tok location.                     */
                         tok_ix++;
                         end_tok_found = DEF_NO;
                     } else {
                        *perr = SHELL_ERR_ARG_TBL_FULL;
                         return (0);
                     }
                 }

                 break;
        }
        in_rd++;
    }


   *perr = SHELL_ERR_NONE;
    
    return (tok_ix);
}


/*
*********************************************************************************************************
*                                           Shell_CmdSearch()
*
* Description : (1) Search for specified command :
*
*                   (a) Extract module command name
*                   (b) Search  module command
*                   (c) Search  command in table
*
*
* Argument(s) : cmd_name        Pointer to command name.
*               perr            Pointer to variable that will receive the return error code from this
*                               function :
*
*                               SHELL_ERR_NONE                      No error. 
*                               SHELL_ERR_MODULE_CMD_NOT_FOUND      Module command NOT found.
*                               SHELL_ERR_CMD_NOT_FOUND             Command        NOT found.
*
*                                                                       ----- RETURNED BY Shell_ModuleCmdNameGet() : -----
*                               SHELL_ERR_MODULE_CMD_NAME_NONE      No module command name found.
*                               SHELL_ERR_MODULE_CMD_NAME_TOO_LONG  Module command name too long.
*                               SHELL_ERR_MODULE_CMD_NAME_COPY      Copy error.
*
* Return(s)   : Pointer to command function.
*
* Caller(s)   : Shell_Exec().
*
* Note(s)     : none. 
*********************************************************************************************************
*/

static  SHELL_CMD_FNCT  Shell_CmdSearch (CPU_CHAR   *cmd_name,
                                         SHELL_ERR  *perr)
{
    CPU_CHAR           module_cmd_name[SHELL_CFG_MODULE_CMD_NAME_LEN_MAX];    
    SHELL_MODULE_CMD  *pmodule_cmd_list;
    SHELL_MODULE_CMD  *pmodule_cmd;
    SHELL_CMD         *pcmd_list;
    SHELL_CMD         *pcmd;
    CPU_INT16S         name_compare;
    CPU_INT16U         i; 
    SHELL_CMD_FNCT     fnct;

                                                                /* ------------- INIT RECEIVE CMD NAME TBL ------------ */
    for (i = 0; i < SHELL_CFG_MODULE_CMD_NAME_LEN_MAX; i++) {
        module_cmd_name[i] = SHELL_ASCII_ARG_END;   
    }

                                                                /* -------------- EXTRACT MODULE CMD NAME ------------- */
    Shell_ModuleCmdNameGet(cmd_name,
                           module_cmd_name,
                           SHELL_CFG_MODULE_CMD_NAME_LEN_MAX,
                           perr);

    if (*perr !=  SHELL_ERR_NONE) {
         return ((SHELL_CMD_FNCT)0);
    }

                                                                /* ----------------- SEARCH MODULE CMD ---------------- */  
    pmodule_cmd_list = Shell_ModuleCmdUsedPoolPtr;
    pmodule_cmd      = DEF_NULL;

    while (pmodule_cmd_list != DEF_NULL) {
        name_compare = Str_Cmp(module_cmd_name, pmodule_cmd_list->Name);
        if (name_compare == 0) {
            pmodule_cmd = pmodule_cmd_list;
            break;
        }
        pmodule_cmd_list = pmodule_cmd_list->NextModuleCmdPtr;
    }

    if (pmodule_cmd == DEF_NULL) {
       *perr = SHELL_ERR_MODULE_CMD_NOT_FOUND;
        return ((SHELL_CMD_FNCT)0);
    }


                                                                /* -------------------- SEARCH CMD -------------------- */
    pcmd_list = pmodule_cmd->CmdTblPtr;
    pcmd      = DEF_NULL;

    while (pcmd_list->Fnct != (SHELL_CMD_FNCT)0) {
        name_compare = Str_Cmp(cmd_name, pcmd_list->Name);
        if (name_compare == 0) {
            pcmd  = pcmd_list;
            break;
        }
        pcmd_list++;
    }

    if (pcmd == DEF_NULL) {
       *perr = SHELL_ERR_CMD_NOT_FOUND;
        return ((SHELL_CMD_FNCT)0);
    }

                                                                /* ---------------------- RTN CMD --------------------- */
   *perr =  SHELL_ERR_NONE;
    fnct = (SHELL_CMD_FNCT)pcmd->Fnct;

    return (fnct);
}


/*
*********************************************************************************************************
*                                          Shell_ModuleCmdNameGet()
*
* Description : (1) Get the command module name (prefix) from a command string :
*
*                   (a) Search for module command name delimiter
*                   (b) Copy       module command name
*
*
* Argument(s) : cmd_str          Pointer to command string holding the command module name.
*               module_cmd_name  Pointer to a preallocated variable that will receive the module
*                                command name.
*               len              Length of the array pointed by 'module_cmd_name'.
*               perr             Pointer to variable that will receive the return error code from this
*                                function :
*
*                                SHELL_ERR_NONE                      No error. 
*                                SHELL_ERR_MODULE_CMD_NAME_NONE      No module command name found.
*                                SHELL_ERR_MODULE_CMD_NAME_TOO_LONG  Module command name too long.
*                                SHELL_ERR_MODULE_CMD_NAME_COPY      Copy error.
*
* Return(s)   : none.
*
* Caller(s)   : Shell_CmdTblAdd(),
*               Shell_CmdSearch().
*
* Note(s)     : (2) The command module name consists in the first part of a command part, that is the
*                   part preceding the underscore ('_') character. If there is no underscored character,
*                   the command name is interpreted as the commande module name.
*********************************************************************************************************
*/

static  void  Shell_ModuleCmdNameGet (CPU_CHAR    *cmd_str,
                                      CPU_CHAR     module_cmd_name[],
                                      CPU_INT16U   len,
                                      SHELL_ERR   *perr)
{
    CPU_CHAR     *pcmd;
    CPU_BOOLEAN   found;
    CPU_INT16U    name_len;
    CPU_CHAR     *copy_ret_val;


    pcmd  = cmd_str;
    found = DEF_NO;

                                                                /* --------- SEARCH MODULE CMD NAME DELIMITER --------- */
    while ((pcmd  != DEF_NULL) &&
           (found ==  DEF_NO)) {

        if (*pcmd ==  SHELL_ASCII_CDM_NAME_DELIMITER) {
             found =  DEF_YES;
             break;
        } else if (*pcmd == SHELL_ASCII_ARG_END) {
             found =  DEF_YES;
             break;
        }
        pcmd++;
    }

    if (found == DEF_NO) {
       *perr = SHELL_ERR_MODULE_CMD_NAME_NONE;
        return;
    }

 
                                                                /* --------------- COPY MODULE CMD NAME --------------- */
    name_len = (pcmd - cmd_str);
    if (name_len >= len) {                                      /* If module cmd name too long ...                      */
       *perr = SHELL_ERR_MODULE_CMD_NAME_TOO_LONG;              /* ... rtn with error.                                  */
        return;
    }

    copy_ret_val = Str_Copy_N(module_cmd_name, cmd_str, name_len);
    if (copy_ret_val == (CPU_CHAR)0) {
       *perr = SHELL_ERR_MODULE_CMD_NAME_COPY;
        return;
    }


   *perr = SHELL_ERR_NONE;
}


/*
*********************************************************************************************************
*                                            Shell_ModuleCmdClr()
*
* Description : Clear module command.
*
* Argument(s) : pmodule_cmd      Pointer to module command.
*
* Return(s)   : none.
*
* Caller(s)   : Shell_Init(),
*               Shell_CmdTblRem().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  Shell_ModuleCmdClr (SHELL_MODULE_CMD  *pmodule_cmd)
{
    Str_Copy(pmodule_cmd->Name, "");

    pmodule_cmd->PrevModuleCmdPtr = DEF_NULL;
    pmodule_cmd->NextModuleCmdPtr = DEF_NULL;

    pmodule_cmd->CmdTblPtr        = DEF_NULL;
}

