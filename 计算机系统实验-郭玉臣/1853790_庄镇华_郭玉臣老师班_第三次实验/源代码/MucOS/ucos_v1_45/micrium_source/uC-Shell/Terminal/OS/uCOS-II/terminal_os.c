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
#include  <ucos_ii.h>


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

static  OS_STK  Terminal_OS_TaskStk[TERMINAL_OS_CFG_TASK_STK_SIZE];


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
    CPU_INT08U  err;


#if (OS_TASK_CREATE_EXT_EN > 0)
    #if (OS_STK_GROWTH == 1)
    err = OSTaskCreateExt( Terminal_OS_Task,
                           p_arg,
                                                                /* Set Top-Of-Stack.                                    */
                          &Terminal_OS_TaskStk[TERMINAL_OS_CFG_TASK_STK_SIZE - 1],
                           TERMINAL_OS_CFG_TASK_PRIO,
                           TERMINAL_OS_CFG_TASK_PRIO,
                          &Terminal_OS_TaskStk[0],              /* Set Bottom-Of-Stack.                                 */
                           TERMINAL_OS_CFG_TASK_STK_SIZE,
                          (void *)0,                            /* No TCB extension.                                    */
                                                                /* Enable stack checking + clear stack.                 */
                           OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
    #else
    err = OSTaskCreateExt( Terminal_OS_Task,
                           p_arg,
                          &Terminal_OS_TaskStk[0],              /* Set Top-Of-Stack.                                    */
                           TERMINAL_OS_CFG_TASK_PRIO,
                           TERMINAL_OS_CFG_TASK_PRIO,
                                                                /* Set Bottom-Of-Stack.                                 */
                          &Terminal_OS_TaskStk[TERMINAL_OS_CFG_TASK_STK_SIZE - 1],
                           TERMINAL_OS_CFG_TASK_STK_SIZE,
                          (void *)0,                            /* No TCB extension.                                    */
                                                                /* Enable stack checking + clear stack.                 */
                           OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
    #endif
#else
    #if (OS_STK_GROWTH == 1)
    err = OSTaskCreate( Terminal_OS_Task,
                        p_arg,
                       &Terminal_OS_TaskStk[TERMINAL_OS_CFG_TASK_STK_SIZE - 1],
                        TERMINAL_OS_CFG_TASK_PRIO);
    #else
    err = OSTaskCreate( Terminal_OS_Task,
                        p_arg,
                       &Terminal_OS_TaskStk[0],
                        TERMINAL_OS_CFG_TASK_PRIO);
    #endif
#endif

    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(TERMINAL_OS_CFG_TASK_PRIO, (INT8U *)"Terminal", &err);
#endif

    return (DEF_OK);
}
