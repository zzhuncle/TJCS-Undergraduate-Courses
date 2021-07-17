/*
*********************************************************************************************************
*                                             uCOS XSDK BSP
*
*                      (c) Copyright 2014-2020; Silicon Laboratories Inc.,
*                             400 W. Cesar Chavez, Austin, TX 78701
*
*                All rights reserved. Protected by international copyright laws.
*
*               Your use of this software is subject to your acceptance of the terms
*               of a Silicon Labs Micrium software license, which can be obtained by
*               contacting info@micrium.com. If you do not agree to the terms of this
*               license, you may not use this software.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available. Your honesty is greatly appreciated.
*
*                 You can find our product's documentation at: doc.micrium.com
*
*                       For more information visit us at: www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                             USB DEVICE CDC ACM SERIAL TEST APPLICATION
*
* File : app_usbd_cdc.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#if (APP_CFG_USBD_CDC_EN == DEF_ENABLED)
#include  <Class/CDC/usbd_cdc.h>
#include  <Class/CDC/ACM/usbd_acm_serial.h>
#include  <lib_ascii.h>
#include  <Source/os.h>

#include  <ucos_bsp.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#ifndef OS_VERSION
#error "OS_VERSION must be #define'd."
#endif

#define  APP_USBD_CDC_BUF_LEN                             512u

#define  APP_USBD_CDC_CURSOR_START                     "\033[H"
#define  APP_USBD_CDC_CURSOR_START_SIZE                   3u

#define  APP_USBD_CDC_SCREEN_CLR                "\033[2J\033[H"
#define  APP_USBD_CDC_SCREEN_CLR_SIZE                     7u

#define  APP_USBD_CDC_SCREEN_SIZE                        80u

#define  APP_USBD_CDC_TX_TIMEOUT_mS                       0u
#define  APP_USBD_CDC_RX_TIMEOUT_mS                       0u

#define  APP_USBD_CDC_MSG                       "===== USB CDC ACM Serial Emulation Demo ======"\
                                                "\r\n"\
                                                "\r\n"\
                                                "1. Echo 1 demo.\r\n"\
                                                "2. Echo N demo.\r\n"\
                                                "Option: "
#define  APP_USBD_CDC_MSG_SIZE                           92u

#define  APP_USBD_CDC_MSG1                      "Echo 1 demo... \r\n\r\n>> "
#define  APP_USBD_CDC_MSG1_SIZE                          22u

#define  APP_USBD_CDC_MSG2                      "Echo N demo. You can send up to 512 characters at once... \r\n\r\n>> "
#define  APP_USBD_CDC_MSG2_SIZE                          65u

#define  APP_USBD_CDC_NEW_LINE                  "\n\r>> "
#define  APP_USBD_CDC_NEW_LINE_SIZE                       5u


#define  APP_CFG_USBD_CDC_SERIAL_TEST_EN  DEF_ENABLED

#define  APP_CFG_USBD_CDC_SERIAL_TASK_STK_SIZE 1024u
#define  APP_CFG_USBD_CDC_SERIAL_TASK_PRIO 5u

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
*                                   ACM SERIAL DEMO STATE DATA TYPE
*********************************************************************************************************
*/

typedef  enum  app_usbd_cdc_state {
    APP_USBD_CDC_STATE_MENU = 0u,
    APP_USBD_CDC_STATE_ECHO_1,
    APP_USBD_CDC_STATE_ECHO_N
} APP_USBD_CDC_STATE;


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

#if (APP_CFG_USBD_CDC_SERIAL_TEST_EN == DEF_ENABLED)
static  OS_TCB      App_USBD_CDC_SerialTaskTCB;

static  CPU_STK     App_USBD_CDC_SerialTaskStk[APP_CFG_USBD_CDC_SERIAL_TASK_STK_SIZE];
static  CPU_INT08U  App_USBD_CDC_SerialBuf[APP_USBD_CDC_BUF_LEN];

CPU_INT08U          App_USBD_CDC_Msg[APP_USBD_CDC_MSG_SIZE];
CPU_INT08U          App_USBD_CDC_Msg1[APP_USBD_CDC_MSG1_SIZE];
CPU_INT08U          App_USBD_CDC_Msg2[APP_USBD_CDC_MSG2_SIZE];
CPU_INT08U          App_USBD_CDC_CursorStart[APP_USBD_CDC_CURSOR_START_SIZE];
CPU_INT08U          App_USBD_CDC_ScreenClr[APP_USBD_CDC_SCREEN_CLR_SIZE];
CPU_INT08U          App_USBD_CDC_NewLine[APP_USBD_CDC_NEW_LINE_SIZE];
#endif


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#if (APP_CFG_USBD_CDC_SERIAL_TEST_EN == DEF_ENABLED)
static  void         App_USBD_CDC_SerialTask      (void                         *p_arg);
#endif

static  void         App_USBD_CDC_SerialLineCtrl  (CPU_INT08U                    subclass_nbr,
                                                   CPU_INT08U                    events,
                                                   CPU_INT08U                    events_chngd,
                                                   void                         *p_arg);

static  CPU_BOOLEAN  App_USBD_CDC_SerialLineCoding(CPU_INT08U                    subclass_nbr,
                                                   USBD_ACM_SERIAL_LINE_CODING  *p_line_coding,
                                                   void                         *p_arg);


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         App_USBD_CDC_Init()
*
* Description : Initialize CDC class.
*
* Argument(s) : dev_nbr     Device number.
*
*               cfg_hs      High speed configuration number.
*
*               cfg_fs      Full speed configuration number.
*
* Return(s)   : DEF_OK,   if CDC class initialized successfully.
*
*               DEF_FAIL, otherwise.
*
* Caller(s)   : App_USBD_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  App_USBD_CDC_Init (CPU_INT08U  dev_nbr,
                                CPU_INT08U  cfg_hs,
                                CPU_INT08U  cfg_fs)
{
    USBD_ERR    err;
    USBD_ERR    err_hs;
    USBD_ERR    err_fs;
    CPU_INT08U  subclass_nbr;
#if (APP_CFG_USBD_CDC_SERIAL_TEST_EN == DEF_ENABLED)
    OS_ERR      os_err;
#endif

    UCOS_Printf("Initializing CDC class\r\n");

    USBD_CDC_Init(&err);
    if (err != USBD_ERR_NONE) {
        UCOS_Printf("Error initializing CDC class. USBD_CDC_Init() returned error code %d\r\n", err);
        return (DEF_FAIL);
    }

    UCOS_Printf(" Initializing ACM serial subclass\r\n");

    USBD_ACM_SerialInit(&err);
    if (err != USBD_ERR_NONE) {
        UCOS_Printf("Error initializing ACM serial subclass.USBD_ACM_SerialInit returned with error code = %d\r\n", err);
        return (DEF_FAIL);
    }

    subclass_nbr = USBD_ACM_SerialAdd(64u, USBD_ACM_SERIAL_CALL_MGMT_DEV, &err);
    if (err != USBD_ERR_NONE) {
        UCOS_Printf("Error creating ACM serial subclass instance. USBD_ACM_SerialAdd() returned with error %d\r\n", err);
        return (DEF_FAIL);
    }

                                                                /* Register line coding and ctrl line change callbacks. */
    USBD_ACM_SerialLineCodingReg(        subclass_nbr,
                                         App_USBD_CDC_SerialLineCoding,
                                 (void *)0,
                                        &err);

    USBD_ACM_SerialLineCtrlReg(        subclass_nbr,
                                       App_USBD_CDC_SerialLineCtrl,
                               (void *)0,
                                      &err);


    if (cfg_hs != USBD_CFG_NBR_NONE) {
        USBD_ACM_SerialCfgAdd(subclass_nbr, dev_nbr, cfg_hs, &err_hs);
        if (err_hs != USBD_ERR_NONE) {
            UCOS_Printf("Error adding ACM serial subclass instance #%d to HS configuration. USBD_ACM_SerialCfgAdd() return with error %d\r\n", subclass_nbr, err_hs);
        }
    }

    if (cfg_fs != USBD_CFG_NBR_NONE) {
        USBD_ACM_SerialCfgAdd(subclass_nbr, dev_nbr, cfg_fs, &err_fs);
        if (err_fs != USBD_ERR_NONE) {
            UCOS_Printf("Error adding ACM serial subclass instance #%d to FS configuration. USBD_ACM_SerialCfgAdd() return with error %d\r\n", subclass_nbr, err_fs);
        }
    }

    if ((err_hs != USBD_ERR_NONE) &&                            /* If HS and FS cfg fail, stop class init.              */
        (err_fs != USBD_ERR_NONE)) {
        return (DEF_FAIL);
    }

#if (APP_CFG_USBD_CDC_SERIAL_TEST_EN == DEF_ENABLED)
    OSTaskCreate(                 &App_USBD_CDC_SerialTaskTCB,
                                  "USB Device CDC ACM Test",
                                   App_USBD_CDC_SerialTask,
                 (void *)(CPU_ADDR)subclass_nbr,
                                   APP_CFG_USBD_CDC_SERIAL_TASK_PRIO,
                                  &App_USBD_CDC_SerialTaskStk[0],
                                   APP_CFG_USBD_CDC_SERIAL_TASK_STK_SIZE / 10u,
                                   APP_CFG_USBD_CDC_SERIAL_TASK_STK_SIZE,
                                   0u,
                                   0u,
                 (void *)          0,
                                   OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                                  &os_err);
    if (os_err != OS_ERR_NONE) {
        UCOS_Printf("Error creating CDC ACM serial test task. OSTaskCreate() returned with error %d", os_err);
        return (DEF_FAIL);
    }
#endif

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                      App_USBD_CDC_SerialTask()
*
* Description : USB CDC ACM serial emulation demo task.
*
* Argument(s) : p_arg       Task argument pointer.
*
* Return(s)   : none.
*
* Created by  : App_USBD_CDC_Init().
*
* Note(s)     : None.
*********************************************************************************************************
*/

#if (APP_CFG_USBD_CDC_SERIAL_TEST_EN == DEF_ENABLED)
static  void  App_USBD_CDC_SerialTask (void  *p_arg)
{
    CPU_INT08U          ch;
    CPU_BOOLEAN         conn;
    APP_USBD_CDC_STATE  state;
    CPU_INT08U          subclass_nbr;
    CPU_INT08U          line_ctr;
    CPU_INT08U          line_state;
    CPU_INT32U          xfer_len;
    USBD_ERR            err;
    OS_ERR              os_err;


    subclass_nbr = (CPU_INT08U)(CPU_ADDR)p_arg;
    state        =  APP_USBD_CDC_STATE_MENU;
    line_ctr     =  0u;

    Mem_Copy(App_USBD_CDC_Msg,
             APP_USBD_CDC_MSG,
             APP_USBD_CDC_MSG_SIZE);

    Mem_Copy(App_USBD_CDC_Msg1,
             APP_USBD_CDC_MSG1,
             APP_USBD_CDC_MSG1_SIZE);

    Mem_Copy(App_USBD_CDC_Msg2,
             APP_USBD_CDC_MSG2,
             APP_USBD_CDC_MSG2_SIZE);

    Mem_Copy(App_USBD_CDC_CursorStart,
             APP_USBD_CDC_CURSOR_START,
             APP_USBD_CDC_CURSOR_START_SIZE);

    Mem_Copy(App_USBD_CDC_ScreenClr,
             APP_USBD_CDC_SCREEN_CLR,
             APP_USBD_CDC_SCREEN_CLR_SIZE);

    Mem_Copy(App_USBD_CDC_NewLine,
             APP_USBD_CDC_NEW_LINE,
             APP_USBD_CDC_NEW_LINE_SIZE);

    while (DEF_TRUE) {
                                                                /* Wait until device is in cfg'd state.                 */
        conn       = USBD_ACM_SerialIsConn(subclass_nbr);
        line_state = USBD_ACM_SerialLineCtrlGet(subclass_nbr, &err);

        while ((conn != DEF_YES                                     ) ||
               (DEF_BIT_IS_CLR(line_state, USBD_ACM_SERIAL_CTRL_DTR)) ||
               (err  != USBD_ERR_NONE                               )) {

            OSTimeDlyHMSM(0u, 0u, 0u, 250u, OS_OPT_TIME_HMSM_NON_STRICT, &os_err);
            (void)&os_err;
            conn       = USBD_ACM_SerialIsConn(subclass_nbr);
            line_state = USBD_ACM_SerialLineCtrlGet(subclass_nbr, &err);
        }

        switch (state) {
            case APP_USBD_CDC_STATE_MENU:
                 (void)USBD_ACM_SerialTx( subclass_nbr,
                                          App_USBD_CDC_CursorStart,
                                          APP_USBD_CDC_CURSOR_START_SIZE,
                                          APP_USBD_CDC_TX_TIMEOUT_mS,
                                         &err);

                 (void)USBD_ACM_SerialTx( subclass_nbr,
                                          App_USBD_CDC_Msg,
                                          APP_USBD_CDC_MSG_SIZE,
                                          APP_USBD_CDC_TX_TIMEOUT_mS,
                                         &err);


                 (void)USBD_ACM_SerialRx( subclass_nbr,         /* Wait for character.                                  */
                                         &ch,
                                          1u,
                                          APP_USBD_CDC_RX_TIMEOUT_mS,
                                         &err);

                 if (err != USBD_ERR_NONE) {
                     break;
                 }

                 (void)USBD_ACM_SerialTx( subclass_nbr,         /* Echo back character.                                 */
                                         &ch,
                                          1u,
                                          APP_USBD_CDC_TX_TIMEOUT_mS,
                                         &err);

                 switch (ch) {                                  /* Select demo options.                                 */
                     case '1':
                          (void)USBD_ACM_SerialTx( subclass_nbr,
                                                   App_USBD_CDC_ScreenClr,
                                                   APP_USBD_CDC_SCREEN_CLR_SIZE,
                                                   APP_USBD_CDC_TX_TIMEOUT_mS,
                                                  &err);

                          (void)USBD_ACM_SerialTx( subclass_nbr,
                                                   App_USBD_CDC_Msg1,
                                                   APP_USBD_CDC_MSG1_SIZE,
                                                   APP_USBD_CDC_TX_TIMEOUT_mS,
                                                  &err);

                          state    = APP_USBD_CDC_STATE_ECHO_1;
                          line_ctr = 0u;
                          break;


                     case '2':
                          (void)USBD_ACM_SerialTx( subclass_nbr,
                                                   App_USBD_CDC_ScreenClr,
                                                   APP_USBD_CDC_SCREEN_CLR_SIZE,
                                                   APP_USBD_CDC_TX_TIMEOUT_mS,
                                                  &err);

                          (void)USBD_ACM_SerialTx( subclass_nbr,
                                                   App_USBD_CDC_Msg2,
                                                   APP_USBD_CDC_MSG2_SIZE,
                                                   APP_USBD_CDC_TX_TIMEOUT_mS,
                                                  &err);

                           state    = APP_USBD_CDC_STATE_ECHO_N;
                           line_ctr = 0u;
                           break;


                     default:
                          break;

                 }
                 break;


            case APP_USBD_CDC_STATE_ECHO_1:                     /* 'Echo 1' state.                                      */
                 (void)USBD_ACM_SerialRx( subclass_nbr,         /*  Wait for character.                                 */
                                         &ch,
                                          1u,
                                          0u,
                                         &err);

                 if (err != USBD_ERR_NONE) {
                     break;
                 }

                 if (ch == ASCII_CHAR_END_OF_TEXT) {            /* If 'Ctrl-c' character is received.                   */
                     state = APP_USBD_CDC_STATE_MENU;           /* ... return to 'menu' state.                          */
                    (void)USBD_ACM_SerialTx( subclass_nbr,
                                             App_USBD_CDC_ScreenClr,
                                             APP_USBD_CDC_SCREEN_CLR_SIZE,
                                             APP_USBD_CDC_TX_TIMEOUT_mS,
                                            &err);

                 } else {
                     (void)USBD_ACM_SerialTx( subclass_nbr,     /* Echo back character.                                 */
                                             &ch,
                                              1u,
                                              APP_USBD_CDC_TX_TIMEOUT_mS,
                                             &err);
                      line_ctr++;

                      if (line_ctr == APP_USBD_CDC_SCREEN_SIZE - 3u) {
                                                                /* Move to next line.                                   */
                          (void)USBD_ACM_SerialTx( subclass_nbr,
                                                   App_USBD_CDC_NewLine,
                                                   APP_USBD_CDC_NEW_LINE_SIZE,
                                                   APP_USBD_CDC_TX_TIMEOUT_mS,
                                                  &err);
                           line_ctr = 0u;
                      }
                 }
                 break;


            case APP_USBD_CDC_STATE_ECHO_N:                     /* 'Echo N' state.                                      */
                 xfer_len = USBD_ACM_SerialRx( subclass_nbr,    /*  Wait for N characters.                              */
                                              &App_USBD_CDC_SerialBuf[0],
                                               APP_USBD_CDC_BUF_LEN,
                                               0u,
                                              &err);

                 if (err != USBD_ERR_NONE) {
                     break;
                 }

                 if ((xfer_len == 1) &&                         /* If 'Ctrl-c' character is received.                   */
                     (App_USBD_CDC_SerialBuf[0] == ASCII_CHAR_END_OF_TEXT)) {

                     state = APP_USBD_CDC_STATE_MENU;           /* ... return to 'menu' state.                          */
                    (void)USBD_ACM_SerialTx( subclass_nbr,
                                             App_USBD_CDC_ScreenClr,
                                             APP_USBD_CDC_SCREEN_CLR_SIZE,
                                             APP_USBD_CDC_TX_TIMEOUT_mS,
                                            &err);

                 } else {
                     (void)USBD_ACM_SerialTx( subclass_nbr,     /* Echo back characters.                                */
                                             &App_USBD_CDC_SerialBuf[0],
                                              xfer_len,
                                              APP_USBD_CDC_TX_TIMEOUT_mS,
                                             &err);
                      line_ctr += xfer_len;

                      if (line_ctr == APP_USBD_CDC_SCREEN_SIZE - 3u) {
                                                                /* Move to next line.                                   */
                          (void)USBD_ACM_SerialTx( subclass_nbr,
                                                   App_USBD_CDC_NewLine,
                                                   APP_USBD_CDC_NEW_LINE_SIZE,
                                                   APP_USBD_CDC_TX_TIMEOUT_mS,
                                                  &err);
                           line_ctr = 0u;
                      }
                 }
                 break;


            default:
                 break;
        }
    }
}
#endif


/*
*********************************************************************************************************
*                                    App_USBD_CDC_SerialLineCtrl()
*
* Description : Serial control line change notification callback.
*
* Argument(s) : nbr             CDC ACM serial emulation subclass instance number.
*
*               events          Current line state. The line state is a OR'ed of the following flags :
*
*                                   USBD_ACM_SERIAL_CTRL_BREAK
*                                   USBD_ACM_SERIAL_CTRL_RTS
*                                   USBD_ACM_SERIAL_CTRL_DTR
*
*               events_chngd    Line state flags that have changed.
*
*               p_arg           Callback argument.
*
* Return(s)   : none.
*
* Caller(s)   : USBD_ACM_SerialMgmtReq() via 'p_ctrl->LineCtrlChngdFnct()'.
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  App_USBD_CDC_SerialLineCtrl (CPU_INT08U   nbr,
                                           CPU_INT08U   events,
                                           CPU_INT08U   events_chngd,
                                           void        *p_arg)
{
    (void)&nbr;
    (void)&events;
    (void)&events_chngd;
    (void)&p_arg;
}


/*
*********************************************************************************************************
*                                   App_USBD_CDC_SerialLineCoding()
*
* Description : Serial line coding line change notification callback.
*
* Argument(s) : nbr             CDC ACM serial emulation subclass instance number.
*
*               p_line_coding   Pointer to line coding structure.
*
*               p_arg           Callback argument.
*
* Return(s)   : none.
*
* Caller(s)   : USBD_ACM_SerialMgmtReq() via 'p_ctrl->LineCtrlChngdFnct()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  App_USBD_CDC_SerialLineCoding (CPU_INT08U                    nbr,
                                                    USBD_ACM_SERIAL_LINE_CODING  *p_line_coding,
                                                    void                         *p_arg)
{
    (void)&nbr;
    (void)&p_line_coding;
    (void)&p_arg;

    return (DEF_OK);
}
#endif
