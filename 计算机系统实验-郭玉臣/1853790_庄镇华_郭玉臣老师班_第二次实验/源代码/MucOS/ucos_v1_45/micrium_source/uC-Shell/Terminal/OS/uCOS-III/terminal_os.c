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
*                                         uC/OS-II RTOS PORT
*
* Filename      : terminal_os.c
* Version       : V1.03.01
* Programmer(s) : BAN
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <terminal.h>
#include  <os.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  CPU_STK  Terminal_OS_TaskStk[TERMINAL_OS_CFG_TASK_STK_SIZE];
static  OS_TCB   Terminal_OS_TaskTCB;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  Terminal_OS_Task(void  *p_arg);


/*
*********************************************************************************************************
*                                         Terminal_OS_Task()
*
* Description : RTOS interface for terminal main loop.
*
* Argument(s) : p_arg       Argument to pass to the task.
*
* Return(s)   : none.
*
* Caller(s)   : RTOS.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  Terminal_OS_Task (void  *p_arg)
{
    Terminal_Task(p_arg);
}


/*
*********************************************************************************************************
*                                         Terminal_OS_Init()
*
* Description : Initialize the terminal task.
*
* Argument(s) : p_arg       Argument to pass to the task.
*
* Return(s)   : DEF_FAIL    Initialize task failed.
*               DEF_OK      Initialize task successful.
*
* Caller(s)   : Terminal_Init()
*
* Note(s)     : The RTOS needs to create Terminal_OS_Task().
*********************************************************************************************************
*/

CPU_BOOLEAN  Terminal_OS_Init (void *p_arg)
{
    OS_ERR  os_err;


    OSTaskCreate((OS_TCB     *)&Terminal_OS_TaskTCB,
                 (CPU_CHAR   *)"Terminal",
				 (OS_TASK_PTR )Terminal_OS_Task,
				 (void       *)0,
				 (OS_PRIO     )TERMINAL_OS_CFG_TASK_PRIO,
				 (CPU_STK    *)&Terminal_OS_TaskStk[0], 
				 (CPU_STK_SIZE)TERMINAL_OS_CFG_TASK_STK_SIZE / 10u,
				 (CPU_STK_SIZE)TERMINAL_OS_CFG_TASK_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&os_err);

    if (os_err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}
