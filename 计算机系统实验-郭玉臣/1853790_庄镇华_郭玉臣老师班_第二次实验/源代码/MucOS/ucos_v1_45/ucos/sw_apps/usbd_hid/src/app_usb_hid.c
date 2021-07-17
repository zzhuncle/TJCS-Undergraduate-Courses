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
*                                USB DEVICE HID CLASS TEST APPLICATION
*
* File : app_usbd_hid.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <Class/HID/usbd_hid.h>
#include  <Source/os.h>
#include  <ucos_bsp.h>


/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         LOCAL CONFIGURATION
*********************************************************************************************************
*/

#define  APP_USBD_HID_REPORT_LEN           4u

#define  APP_CFG_USBD_HID_TASK_STK_SIZE    512u
#define  APP_CFG_USBD_HID_READ_TASK_PRIO   5u
#define  APP_CFG_USBD_HID_WRITE_TASK_PRIO  5u
#define  APP_CFG_USBD_HID_MOUSE_TASK_PRIO  5u

#define  APP_CFG_USBD_HID_TEST_MOUSE_EN    DEF_ENABLED


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

static  CPU_INT08U  App_USBD_HID_Protocol;
static  CPU_INT08U  App_USBD_HID_ReportBuf[APP_USBD_HID_REPORT_LEN];

#if (APP_CFG_USBD_HID_TEST_MOUSE_EN == DEF_DISABLED)
static  OS_TCB      App_USBD_HID_ReadTaskTCB;
static  OS_TCB      App_USBD_HID_WriteTaskTCB;
static  CPU_STK     App_USBD_HID_ReadTaskStk[APP_CFG_USBD_HID_TASK_STK_SIZE];
static  CPU_STK     App_USBD_HID_WriteTaskStk[APP_CFG_USBD_HID_TASK_STK_SIZE];

static  CPU_INT08U  App_USBD_HID_ReportDesc[] = {               /* HID vendor-specific report                           */
        USBD_HID_GLOBAL_USAGE_PAGE   + 2, 0xFF, 0xFF,
        USBD_HID_LOCAL_USAGE         + 1, 0xFF,
        USBD_HID_MAIN_COLLECTION     + 1, USBD_HID_COLLECTION_APPLICATION,

        USBD_HID_GLOBAL_USAGE_PAGE   + 2, 0xFF, 0xFF,
        USBD_HID_LOCAL_USAGE_MIN     + 1, 0x00,
#if   (APP_USBD_HID_REPORT_LEN > 32768)
        USBD_HID_LOCAL_USAGE_MAX     + 3, (APP_USBD_HID_REPORT_LEN - 1) & 0xFF, ((APP_USBD_HID_REPORT_LEN - 1) >> 8) & 0xFF, ((APP_USBD_HID_REPORT_LEN - 1) >> 16) & 0xFF, ((APP_USBD_HID_REPORT_LEN - 1) >> 24) & 0xFF,
#elif (APP_USBD_HID_REPORT_LEN >   128)
        USBD_HID_LOCAL_USAGE_MAX     + 2, (APP_USBD_HID_REPORT_LEN - 1) & 0xFF, ((APP_USBD_HID_REPORT_LEN - 1) >> 8) & 0xFF,
#else
        USBD_HID_LOCAL_USAGE_MAX     + 1,  APP_USBD_HID_REPORT_LEN - 1,
#endif
        USBD_HID_GLOBAL_LOG_MIN      + 1, 0x00,
        USBD_HID_GLOBAL_LOG_MAX      + 2, 0xFF, 0x00,
        USBD_HID_GLOBAL_REPORT_SIZE  + 1, 0x08,
#if   (APP_USBD_HID_REPORT_LEN >= 32768)
        USBD_HID_GLOBAL_REPORT_COUNT + 3, APP_USBD_HID_REPORT_LEN & 0xFF, (APP_USBD_HID_REPORT_LEN >> 8) & 0xFF, (APP_USBD_HID_REPORT_LEN >> 16) & 0xFF, (APP_USBD_HID_REPORT_LEN >> 24) & 0xFF,
#elif (APP_USBD_HID_REPORT_LEN >=   128)
        USBD_HID_GLOBAL_REPORT_COUNT + 2, APP_USBD_HID_REPORT_LEN & 0xFF, (APP_USBD_HID_REPORT_LEN >> 8) & 0xFF,
#else
        USBD_HID_GLOBAL_REPORT_COUNT + 1, APP_USBD_HID_REPORT_LEN,
#endif
        USBD_HID_MAIN_INPUT          + 1, USBD_HID_MAIN_DATA | USBD_HID_MAIN_VARIABLE | USBD_HID_MAIN_ABSOLUTE,

        USBD_HID_GLOBAL_USAGE_PAGE   + 2, 0xFF, 0xFF,
        USBD_HID_LOCAL_USAGE_MIN     + 1, 0x00,
#if   (APP_USBD_HID_REPORT_LEN > 32768)
        USBD_HID_LOCAL_USAGE_MAX     + 3, (APP_USBD_HID_REPORT_LEN - 1) & 0xFF, ((APP_USBD_HID_REPORT_LEN - 1) >> 8) & 0xFF, ((APP_USBD_HID_REPORT_LEN - 1) >> 16) & 0xFF, ((APP_USBD_HID_REPORT_LEN - 1) >> 24) & 0xFF,
#elif (APP_USBD_HID_REPORT_LEN >   128)
        USBD_HID_LOCAL_USAGE_MAX     + 2, (APP_USBD_HID_REPORT_LEN - 1) & 0xFF, ((APP_USBD_HID_REPORT_LEN - 1) >> 8) & 0xFF,
#else
        USBD_HID_LOCAL_USAGE_MAX     + 1,  APP_USBD_HID_REPORT_LEN - 1,
#endif
        USBD_HID_GLOBAL_LOG_MIN      + 1, 0x00,
        USBD_HID_GLOBAL_LOG_MAX      + 2, 0xFF, 0x00,
        USBD_HID_GLOBAL_REPORT_SIZE  + 1, 0x08,
#if   (APP_USBD_HID_REPORT_LEN >= 32768)
        USBD_HID_GLOBAL_REPORT_COUNT + 3, APP_USBD_HID_REPORT_LEN & 0xFF, (APP_USBD_HID_REPORT_LEN >> 8) & 0xFF, (APP_USBD_HID_REPORT_LEN >> 16) & 0xFF, (APP_USBD_HID_REPORT_LEN >> 24) & 0xFF,
#elif (APP_USBD_HID_REPORT_LEN >=   128)
        USBD_HID_GLOBAL_REPORT_COUNT + 2, APP_USBD_HID_REPORT_LEN & 0xFF, (APP_USBD_HID_REPORT_LEN >> 8) & 0xFF,
#else
        USBD_HID_GLOBAL_REPORT_COUNT + 1, APP_USBD_HID_REPORT_LEN,
#endif
        USBD_HID_MAIN_OUTPUT         + 1, USBD_HID_MAIN_DATA | USBD_HID_MAIN_VARIABLE | USBD_HID_MAIN_ABSOLUTE,

        USBD_HID_MAIN_ENDCOLLECTION
};

#else

static  OS_TCB      App_USBD_HID_MouseTaskTCB;
static  CPU_STK     App_USBD_HID_MouseTaskStk[APP_CFG_USBD_HID_TASK_STK_SIZE];
static  CPU_INT32S  App_USBD_HID_X;
static  CPU_INT32S  App_USBD_HID_Y;

static  CPU_INT08U  App_USBD_HID_ReportDesc[] = {               /* HID Mouse report                                     */
    USBD_HID_GLOBAL_USAGE_PAGE           + 1, USBD_HID_USAGE_PAGE_GENERIC_DESKTOP_CONTROLS,
    USBD_HID_LOCAL_USAGE                 + 1, USBD_HID_CA_MOUSE,
    USBD_HID_MAIN_COLLECTION             + 1, USBD_HID_COLLECTION_APPLICATION,
        USBD_HID_LOCAL_USAGE             + 1, USBD_HID_CP_POINTER,
        USBD_HID_MAIN_COLLECTION         + 1, USBD_HID_COLLECTION_PHYSICAL,
            USBD_HID_GLOBAL_USAGE_PAGE   + 1, USBD_HID_USAGE_PAGE_BUTTON,
            USBD_HID_LOCAL_USAGE_MIN     + 1, 0x01,
            USBD_HID_LOCAL_USAGE_MAX     + 1, 0x03,
            USBD_HID_GLOBAL_LOG_MIN      + 1, 0x00,
            USBD_HID_GLOBAL_LOG_MAX      + 1, 0x01,
            USBD_HID_GLOBAL_REPORT_COUNT + 1, 0x03,
            USBD_HID_GLOBAL_REPORT_SIZE  + 1, 0x01,
            USBD_HID_MAIN_INPUT          + 1, USBD_HID_MAIN_DATA | USBD_HID_MAIN_VARIABLE | USBD_HID_MAIN_ABSOLUTE,
            USBD_HID_GLOBAL_REPORT_COUNT + 1, 0x01,
            USBD_HID_GLOBAL_REPORT_SIZE  + 1, 0x0D,
            USBD_HID_MAIN_INPUT          + 1, USBD_HID_MAIN_CONSTANT,
            USBD_HID_GLOBAL_USAGE_PAGE   + 1, USBD_HID_USAGE_PAGE_GENERIC_DESKTOP_CONTROLS,
            USBD_HID_LOCAL_USAGE         + 1, USBD_HID_DV_X,
            USBD_HID_LOCAL_USAGE         + 1, USBD_HID_DV_Y,
            USBD_HID_GLOBAL_LOG_MIN      + 1, 0x81,
            USBD_HID_GLOBAL_LOG_MAX      + 1, 0x7F,
            USBD_HID_GLOBAL_REPORT_SIZE  + 1, 0x08,
            USBD_HID_GLOBAL_REPORT_COUNT + 1, 0x02,
            USBD_HID_MAIN_INPUT          + 1, USBD_HID_MAIN_DATA | USBD_HID_MAIN_VARIABLE | USBD_HID_MAIN_RELATIVE,
        USBD_HID_MAIN_ENDCOLLECTION,
    USBD_HID_MAIN_ENDCOLLECTION
};
#endif


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#if (APP_CFG_USBD_HID_TEST_MOUSE_EN == DEF_ENABLED)
static  void         App_USBD_HID_MouseTask     (void        *p_arg);

static  void         App_USBD_HID_MouseSetReport(CPU_INT08U  *p_buf);

#else

static  void         App_USBD_HID_ReadTask      (void        *p_arg);

static  void         App_USBD_HID_WriteTask     (void        *p_arg);
#endif

static  CPU_BOOLEAN  App_USBD_HID_GetFeatureReport(CPU_INT08U   class_nbr,
                                                 CPU_INT08U     report_id,
                                                 CPU_INT08U  *p_report_buf,
                                                 CPU_INT16U     report_len);

static  CPU_BOOLEAN  App_USBD_HID_SetFeatureReport(CPU_INT08U   class_nbr,
                                                 CPU_INT08U     report_id,
                                                 CPU_INT08U  *p_report_buf,
                                                 CPU_INT16U     report_len);

static  CPU_INT08U   App_USBD_HID_GetProtocol   (CPU_INT08U   class_nbr,
                                                 USBD_ERR    *p_err);

static  void         App_USBD_HID_SetProtocol   (CPU_INT08U   class_nbr,
                                                 CPU_INT08U   protocol,
                                                 USBD_ERR    *p_err);


static  USBD_HID_CALLBACK  App_USBD_HID_Callback = {
    App_USBD_HID_GetFeatureReport,
    App_USBD_HID_SetFeatureReport,
    App_USBD_HID_GetProtocol,
    App_USBD_HID_SetProtocol,
};


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

#if    (APP_CFG_USBD_HID_TEST_MOUSE_EN == DEF_DISABLED)
#if    (APP_USBD_HID_REPORT_LEN < 4)
#error "APP_USBD_HID_REPORT_LEN must be at least 4 octets"
#endif
#endif


/*
*********************************************************************************************************
*                                         App_USBD_HID_Init()
*
* Description : Add a HID interface to the USB device stack.
*
* Argument(s) : dev_nbr     Device number.
*
*               cfg_hs      Index of high-speed configuration to which this interface will be added to.
*
*               cfg_fs      Index of full-speed configuration to which this interface will be added to.
*
* Return(s)   : DEF_OK,   if HID interface successfully added.
*
*               DEF_FAIL, otherwise.
*
* Caller(s)   : App_USBD_Create().
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  App_USBD_HID_Init (CPU_INT08U  dev_nbr,
                                CPU_INT08U  cfg_hs,
                                CPU_INT08U  cfg_fs)
{
    USBD_ERR    err;
    USBD_ERR    err_hs;
    USBD_ERR    err_fs;
    CPU_INT08U  class_nbr;
    OS_ERR      os_err;


    err_hs = USBD_ERR_NONE;
    err_fs = USBD_ERR_NONE;

#if (APP_CFG_USBD_HID_TEST_MOUSE_EN == DEF_ENABLED)
    App_USBD_HID_X = 101;
    App_USBD_HID_Y = 101;
#endif

    UCOS_Print("Initializing HID class\r\n");

    USBD_HID_Init(&err);                                        /* Init HID class.                                      */
    if (err != USBD_ERR_NONE) {
        UCOS_Printf("Error initializing HID class. USBD_HID_Init() returned error code %d\r\n", err);
        return (DEF_FAIL);
    }
                                                                /* Create a HID class instance.                         */
    class_nbr = USBD_HID_Add(             USBD_HID_SUBCLASS_BOOT,
#if (APP_CFG_USBD_HID_TEST_MOUSE_EN == DEF_ENABLED)
                                          USBD_HID_PROTOCOL_MOUSE,
#else
                                          USBD_HID_PROTOCOL_NONE,
#endif
                                          USBD_HID_COUNTRY_CODE_NOT_SUPPORTED,
                                         &App_USBD_HID_ReportDesc[0],
                                          sizeof(App_USBD_HID_ReportDesc),
                            (CPU_INT08U *)0,
                                          0u,
                                          2u,
                                          2u,
                                          DEF_YES,
                                         &App_USBD_HID_Callback,
                                         &err);
    if (err != USBD_ERR_NONE) {
        UCOS_Printf("Error adding HID class. USBD_HID_Add() returned error code %d\r\n", err);
        return (DEF_FAIL);
    }

    if (cfg_hs != USBD_CFG_NBR_NONE) {
                                                                /* Add HID class to HS dflt cfg.                        */
        USBD_HID_CfgAdd(class_nbr, dev_nbr, cfg_hs, &err_hs);
        if (err_hs != USBD_ERR_NONE) {
            UCOS_Printf("Error adding HID instance. USBD_HID_CfgAdd() returned error code %d\r\n", err_hs);
        }
    }

    if (cfg_fs != USBD_CFG_NBR_NONE) {
                                                                /* Add HID class to FS dflt cfg.                        */
        USBD_HID_CfgAdd(class_nbr, dev_nbr, cfg_fs, &err_fs);
        if (err_fs != USBD_ERR_NONE) {
            UCOS_Printf("Error adding HID instance. USBD_HID_CfgAdd() returned error code %d\r\n", err_fs);
        }
    }

    if ((err_hs != USBD_ERR_NONE) &&                            /* If HS and FS cfg fail, stop class init.              */
        (err_fs != USBD_ERR_NONE)) {
        return (DEF_FAIL);
    }

#if (APP_CFG_USBD_HID_TEST_MOUSE_EN == DEF_DISABLED)
    OSTaskCreate(       &App_USBD_HID_ReadTaskTCB,
                         "USB Device HID Read",
                         App_USBD_HID_ReadTask,
                 (void *)class_nbr,
                         APP_CFG_USBD_HID_READ_TASK_PRIO,
                        &App_USBD_HID_ReadTaskStk[0],
                         APP_CFG_USBD_HID_TASK_STK_SIZE / 10u,
                         APP_CFG_USBD_HID_TASK_STK_SIZE,
                         0u,
                         0u,
                 (void *)0,
                         OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                        &os_err);
    if (os_err != OS_ERR_NONE) {
        UCOS_Printf("Error creating HID read task. OSTaskCreate() returned error code %d\r\n", os_err);
        return (DEF_FAIL);
    }

    OSTaskCreate(       &App_USBD_HID_WriteTaskTCB,
                         "USB Device HID Write",
                         App_USBD_HID_WriteTask,
                 (void *)class_nbr,
                         APP_CFG_USBD_HID_WRITE_TASK_PRIO,
                        &App_USBD_HID_WriteTaskStk[0],
                         APP_CFG_USBD_HID_TASK_STK_SIZE / 10u,
                         APP_CFG_USBD_HID_TASK_STK_SIZE,
                         0u,
                         0u,
                 (void *)0,
                         OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                        &os_err);
    if (os_err != OS_ERR_NONE) {
        UCOS_Printf("Error creating HID write task. OSTaskCreate() returned error code %d\r\n", os_err);
        return (DEF_FAIL);
    }


#else


    OSTaskCreate(       &App_USBD_HID_MouseTaskTCB,
                         "USB Device HID Mouse",
                         App_USBD_HID_MouseTask,
                 (void *)class_nbr,
                         APP_CFG_USBD_HID_MOUSE_TASK_PRIO,
                        &App_USBD_HID_MouseTaskStk[0],
                         APP_CFG_USBD_HID_TASK_STK_SIZE / 10u,
                         APP_CFG_USBD_HID_TASK_STK_SIZE,
                         0u,
                         0u,
                 (void *)0,
                         OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                        &os_err);
    if (os_err != OS_ERR_NONE) {
        UCOS_Printf("Error creating HID mouse task. OSTaskCreate() returned error code %d\r\n", os_err);
        return (DEF_FAIL);
    }
#endif

    return (DEF_OK);
}

/*
*********************************************************************************************************
*                                      App_USBD_HID_MouseTask()
*
* Description : Perform HID writes to host. The HID writes simulate the movement of a mouse.
*
* Argument(s) : p_arg       Argument passed to 'App_USBD_HID_MouseTask()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (APP_CFG_USBD_HID_TEST_MOUSE_EN == DEF_ENABLED)
static  void  App_USBD_HID_MouseTask (void  *p_arg)
{
    CPU_INT08U   class_nbr;
    CPU_BOOLEAN  conn;
    USBD_ERR     err;
    OS_ERR       os_err;


    class_nbr = (CPU_INT08U)(CPU_ADDR)p_arg;

    while (DEF_TRUE) {
        conn = USBD_HID_IsConn(class_nbr);
        while (conn != DEF_YES) {
            OSTimeDlyHMSM(0u, 0u, 0u, 250u, OS_OPT_TIME_HMSM_STRICT, &os_err);

            conn = USBD_HID_IsConn(class_nbr);
        }

        App_USBD_HID_MouseSetReport(&App_USBD_HID_ReportBuf[0u]);

        (void)USBD_HID_Wr( class_nbr,
                          &App_USBD_HID_ReportBuf[0],
                           APP_USBD_HID_REPORT_LEN,
                           0u,
                          &err);

        OSTimeDlyHMSM(0, 0, 0, 100, OS_OPT_TIME_HMSM_STRICT, &os_err);

        if ((App_USBD_HID_ReportBuf[2u] != 0u) ||
            (App_USBD_HID_ReportBuf[3u] != 0u)) {

            (void)USBD_HID_Wr( class_nbr,
                              &App_USBD_HID_ReportBuf[0u],
                               APP_USBD_HID_REPORT_LEN,
                               0u,
                              &err);

            OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &os_err);
        } else {
            OSTimeDlyHMSM(0u, 0u, 0u,   2u, OS_OPT_TIME_HMSM_STRICT, &os_err);
        }
    }
}
#endif

/*
*********************************************************************************************************
*                                       App_USBD_HID_ReadTask()
*
* Description : Perform reads from HID.
*
* Argument(s) : p_arg       Argument passed to 'App_USBD_HID_ReadTask()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (APP_CFG_USBD_HID_TEST_MOUSE_EN == DEF_DISABLED)
static  void  App_USBD_HID_ReadTask (void  *p_arg)
{
    CPU_INT08U   class_nbr;
    CPU_BOOLEAN  conn;
    USBD_ERR     err;
    OS_ERR       os_err;


    class_nbr = (CPU_INT08U)(CPU_ADDR)p_arg;

    while (DEF_TRUE) {
        conn = USBD_HID_IsConn(class_nbr);
        while (conn != DEF_YES) {
            OSTimeDlyHMSM(0, 0, 0, 250, OS_OPT_TIME_HMSM_STRICT, &os_err);

            conn = USBD_HID_IsConn(class_nbr);
        }

        (void)USBD_HID_Rd( class_nbr,
                          &App_USBD_HID_ReportBuf[0],
                           APP_USBD_HID_REPORT_LEN,
                           0u,
                          &err);
        if (err != USBD_ERR_NONE)  {
            UCOS_Printf("HID demo failed to receive data from host. USBD_HID_Rd() returned error code %d\r\n", err);
        }
    }
}
#endif


/*
*********************************************************************************************************
*                                      App_USBD_HID_WriteTask()
*
* Description : Perform writes to HID.
*
* Argument(s) : p_arg       Argument passed to 'App_USBD_HID_WriteTask()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (APP_CFG_USBD_HID_TEST_MOUSE_EN == DEF_DISABLED)
static  void  App_USBD_HID_WriteTask (void  *p_arg)
{
    CPU_INT08U   class_nbr;
    CPU_BOOLEAN  conn;
    USBD_ERR     err;
    OS_ERR       os_err;


    class_nbr = (CPU_INT08U)(CPU_ADDR)p_arg;

    Mem_Clr(&App_USBD_HID_ReportBuf[0], APP_USBD_HID_REPORT_LEN);

    while (DEF_TRUE) {
        conn = USBD_HID_IsConn(class_nbr);
        while (conn != DEF_YES) {
            OSTimeDlyHMSM(0, 0, 0, 250, OS_OPT_TIME_HMSM_STRICT, &os_err);

            conn = USBD_HID_IsConn(class_nbr);
        }

        App_USBD_HID_ReportBuf[0]++;
        App_USBD_HID_ReportBuf[1] += 2;
        App_USBD_HID_ReportBuf[2] += 3;
        App_USBD_HID_ReportBuf[3] += 4;

        (void)USBD_HID_Wr( class_nbr,
                          &App_USBD_HID_ReportBuf[0],
                           APP_USBD_HID_REPORT_LEN,
                           0u,
                          &err);
        if (err != USBD_ERR_NONE)  {
            UCOS_Printf("HID demo failed to send data to host. USBD_HID_Rd() returned error code %d\r\n", err);
        }

        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          USB HID CALLBACKS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                   App_USBD_HID_GetFeatureReport()
*
* Description : Get HID feature report corresponding to report ID.
*
* Argument(s) : class_nbr       Class instance number.
*
*               report_id       Report ID.
*
*               p_report_buf    Pointer to feature report buffer.
*
*               report_len      Length of report.
*
* Return(s)   : DEF_OK,   if NO error(s) occurred and report ID is supported.
*
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Interface drivers.
*
* Note(s)     : (1) Report ID must not be written into the feature report buffer.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  App_USBD_HID_GetFeatureReport (CPU_INT08U   class_nbr,
                                                    CPU_INT08U     report_id,
                                                    CPU_INT08U  *p_report_buf,
                                                    CPU_INT16U     report_len)
{
    (void)&class_nbr;
    (void)&report_id;

    Mem_Clr(p_report_buf, report_len);

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                   App_USBD_HID_SetFeatureReport()
*
* Description : Set HID feature report corresponding to report ID.
*
* Argument(s) : class_nbr       Class instance number.
*
*               report_id       Report ID.
*
*               p_report_buf    Pointer to feature report buffer.
*
*               report_len      Length of report.
*
* Return(s)   : DEF_OK,   if NO error(s) occurred and report ID is supported.
*
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Interface drivers.
*
* Note(s)     : (1) Report ID is not present in the feature report buffer.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  App_USBD_HID_SetFeatureReport (CPU_INT08U   class_nbr,
                                                    CPU_INT08U     report_id,
                                                    CPU_INT08U  *p_report_buf,
                                                    CPU_INT16U     report_len)
{
    (void)&class_nbr;
    (void)&report_id;
    (void)&p_report_buf;
    (void)&report_len;

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                     App_USBD_HID_GetProtocol()
*
* Description : Retrieve active protocol: BOOT or REPORT protocol.
*
* Argument(s) : class_nbr   Class instance number.
*
*               p_err       Pointer to variable that will receive the return error code from this function :
*
*                               USBD_ERR_NONE   Protocol successfully retrieved.
*
* Return(s)   : Protocol, if NO error(s).
*
*               0,        otherwise.
*
* Caller(s)   : Interface drivers.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_INT08U  App_USBD_HID_GetProtocol (CPU_INT08U   class_nbr,
                                              USBD_ERR    *p_err)
{
    (void)&class_nbr;

   *p_err = USBD_ERR_NONE;

    return (App_USBD_HID_Protocol);
}


/*
*********************************************************************************************************
*                                     App_USBD_HID_SetProtocol()
*
* Description : Store active protocol: BOOT or REPORT protocol.
*
* Argument(s) : class_nbr   Class instance number.
*
*               protocol    Protocol.
*
*               p_err       Pointer to variable that will receive the return error code from this function :
*
*                               USBD_ERR_NONE   Protocol successfully set.
* Return(s)   : none.
*
* Caller(s)   : Interface drivers.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_USBD_HID_SetProtocol (CPU_INT08U   class_nbr,
                                        CPU_INT08U   protocol,
                                        USBD_ERR    *p_err)
{
    (void)&class_nbr;

    App_USBD_HID_Protocol = protocol;

   *p_err = USBD_ERR_NONE;
}


/*
*********************************************************************************************************
*                                    App_USBD_HID_MouseSetReport()
*
* Description : Populate buffer with mouse report.
*
* Argument(s) : p_buf       Pointer to buffer.
*
* Return(s)   : none.
*
* Caller(s)   : App_USBD_HID_MouseTask().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (APP_CFG_USBD_HID_TEST_MOUSE_EN == DEF_ENABLED)
static void  App_USBD_HID_MouseSetReport (CPU_INT08U  *p_buf)
{
    CPU_SR_ALLOC();


    p_buf[0] = 0;
    p_buf[1] = 0;

    CPU_CRITICAL_ENTER();
    p_buf[2] = (App_USBD_HID_X > 0) ? 50 : -50;
    p_buf[3] = (App_USBD_HID_Y > 0) ? 50 : -50;

    App_USBD_HID_X = !App_USBD_HID_X;
    App_USBD_HID_Y = !App_USBD_HID_Y;
    CPU_CRITICAL_EXIT();

#if 0
    CPU_CRITICAL_ENTER();
    if (App_USBD_HID_X >  100) {
        p_buf[2]        = 100;
        App_USBD_HID_X -= 100;
    } else if (App_USBD_HID_X < -100) {
        p_buf[2]        = (CPU_INT08U)-100;
        App_USBD_HID_X += 100;
    } else {
        p_buf[2]        = App_USBD_HID_X;
        App_USBD_HID_X  = 0;
    }

    if (App_USBD_HID_Y >  100) {
        p_buf[3]        = 100;
        App_USBD_HID_Y -= 100;
    } else if (App_USBD_HID_Y < -100) {
        p_buf[3]        = (CPU_INT08U)-100;
        App_USBD_HID_Y += 100;
    } else {
        p_buf[3]        = App_USBD_HID_Y;
        App_USBD_HID_Y  = 0;
    }
    CPU_CRITICAL_EXIT();
#endif
}
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

