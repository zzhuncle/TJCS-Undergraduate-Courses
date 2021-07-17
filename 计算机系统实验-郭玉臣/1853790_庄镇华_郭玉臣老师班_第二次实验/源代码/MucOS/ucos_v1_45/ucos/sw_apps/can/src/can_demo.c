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
****************************************************************************************************
* File : can_demo.c
****************************************************************************************************
*/


/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#include <app_cfg.h>
#include <can_cfg.h>
#include "can_demo.h"
#include <can_bsp.h>

#include "can_bus.h"
#include "can_frm.h"
#include "can_msg.h"
#include "can_sig.h"
#include "can_err.h"
#include "drv_can.h"


/*
****************************************************************************************************
*                                              DEFINES
****************************************************************************************************
*/


#define  APP_CAN_DEMO_ECHO_TASK                         0u      /* Definition for APP_CAN_DEMO_SELECT, choose.. */
#define  APP_CAN_DEMO_RX_TX_TASK                        1u      /* .. between Echo Task or Tx/Rx Tasks          */

                                                                /* Select CAN Demo to run in Application        */
#define  APP_CAN_DEMO_SELECT      APP_CAN_DEMO_RX_TX_TASK



#if (APP_CAN_DEMO_SELECT == APP_CAN_DEMO_ECHO_TASK)             /* Demo 'Echo' Task to Rx & Tx same Msg back   */
#define  APP_CAN0_ECHO_TASK_PRIO                        4u
#define  APP_CAN1_ECHO_TASK_PRIO                        5u

#define  APP_CAN0_ECHO_TASK_STK_SIZE                  256u
#define  APP_CAN1_ECHO_TASK_STK_SIZE                  256u
#endif


#if (APP_CAN_DEMO_SELECT == APP_CAN_DEMO_RX_TX_TASK)            /* Main Rx_Task for Incomming CAN BUS Msg(s)   */
#define  APP_CAN0_RX_TASK_PRIO                         10u
#define  APP_CAN0_TX_TASK_PRIO                         11u

#define  APP_CAN1_RX_TASK_PRIO                         12u
#define  APP_CAN1_TX_TASK_PRIO                         13u

#define  APP_CAN0_RX_TASK_STK_SIZE                    512u
#define  APP_CAN0_TX_TASK_STK_SIZE                    512u

#define  APP_CAN1_RX_TASK_STK_SIZE                    512u
#define  APP_CAN1_TX_TASK_STK_SIZE                    512u
#endif


/*
****************************************************************************************************
*                                          EXTERNAL DATA
****************************************************************************************************
*/

#if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)
extern  const  CANBUS_PARA  CanCfg0;
#endif

#if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)
extern  const  CANBUS_PARA  CanCfg1;
#endif

//extern  const  CANSIG_PARA  CanSig[];
//extern  const  CANMSG_PARA  CanMsg[];


/*
****************************************************************************************************
*                                           LOCAL DATA
****************************************************************************************************
*/

#if (APP_CAN_DEMO_SELECT == APP_CAN_DEMO_ECHO_TASK)             /* -------------------- ECHO TASK --------------------- */
    #if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)
        static  OS_STK  App_CAN0_EchoTask_Stk[APP_CAN0_ECHO_TASK_STK_SIZE];
    #endif

    #if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)
        static  OS_STK  App_CAN1_EchoTask_Stk[APP_CAN1_ECHO_TASK_STK_SIZE];
    #endif
#endif

#if (APP_CAN_DEMO_SELECT == APP_CAN_DEMO_RX_TX_TASK)            /* -------------------- RX/TX TASK -------------------- */
    #if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)
        static  OS_STK  App_CAN0_RxTaskStk[APP_CAN0_RX_TASK_STK_SIZE];
        static  OS_STK  App_CAN0_TxTaskStk[APP_CAN0_TX_TASK_STK_SIZE];

        CPU_INT32U  App_CAN0_RxTx_Ctr = 0u;                     /* Global Rx Counter. Tx will Transmit the Count.       */
    #endif

    #if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)
        static  OS_STK  App_CAN1_RxTaskStk[APP_CAN1_RX_TASK_STK_SIZE];
        static  OS_STK  App_CAN1_TxTaskStk[APP_CAN1_TX_TASK_STK_SIZE];

        CPU_INT32U  App_CAN1_RxTx_Ctr = 0u;                     /* Global Rx Counter. Tx will Transmit the Count.       */
    #endif
#endif

/*
****************************************************************************************************
*                                       FUNCTION PROTOTYPES
****************************************************************************************************
*/

static  void     App_CAN_TaskCreate     (void);

#if (APP_CAN_DEMO_SELECT == APP_CAN_DEMO_RX_TX_TASK)            /* -------------------- RX/TX TASK -------------------- */
    #if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)
        static  void  App_CAN0_RxTask   (void *p_arg);
        static  void  App_CAN0_TxTask   (void *p_arg);
    #endif

    #if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)
        static  void  App_CAN1_RxTask   (void *p_arg);
        static  void  App_CAN1_TxTask   (void *p_arg);
    #endif
#endif


#if (APP_CAN_DEMO_SELECT == APP_CAN_DEMO_ECHO_TASK)             /* -------------------- ECHO TASK --------------------- */
    #if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)
        static  void  App_CAN0_EchoTask (void *p_arg);
    #endif

    #if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)
        static  void  App_CAN1_EchoTask (void *p_arg);
    #endif
#endif


/*
*********************************************************************************************************
*                                       CAN DEMO STARTUP FUNCTION
*
* Description: This Function must be Called by the Application to Start uC/CAN.
*
* Arguments  : none.
*********************************************************************************************************
*/

void App_CAN_Startup (void)
{
    CPU_INT16S  can_err;


    ZC7xxx_CAN_BSP_IntVectSet();                                /* Assign ISR handlers for CAN Channels                 */

    CanSigInit(0L);                                             /* Initialize uC/CAN Signals                            */
    CanMsgInit(0L);                                             /* Initialize uC/CAN Messages                           */
    CanBusInit(0L);                                             /* Initialize uC/CAN Bus                                */

#if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)                       /* --------------------- CAN 0 ------------------------ */
    can_err = CanBusEnable((CANBUS_PARA *)&CanCfg0);            /* Enable CAN0 Device acc. Configuration                */
    if (can_err != CAN_ERR_NONE) {
        UCOS_Print("Error in CAN0 Bus Enable...\n\r");
    }
#endif

#if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)                       /* --------------------- CAN 1 ------------------------ */
    can_err = CanBusEnable((CANBUS_PARA *)&CanCfg1);            /* Enable CAN1 Device acc. Configuration                */
    if (can_err != CAN_ERR_NONE) {
    	UCOS_Print("Error in CAN1 Bus Enable...\n\r");
    }
#endif

    App_CAN_TaskCreate();                                       /* Creates Necessary CAN Application Tasks              */
}


/*
*********************************************************************************************************
*                                        App_CAN_TaskCreate()
*
* Description :  Create the CAN Application tasks.
*
* Argument(s) :  none.
*
* Return(s)   :  none.
*
* Caller(s)   :  Com_Start()
*
* Note(s)     :  none.
*********************************************************************************************************
*/

static  void  App_CAN_TaskCreate (void)
{
#if (APP_CAN_DEMO_SELECT == APP_CAN_DEMO_RX_TX_TASK)            /* Main Rx_Task for Incomming CAN BUS Msg(s)            */
    #if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)                   /* ---------------- CAN CHANNEL 0 TASKS --------------- */
        OSTaskCreateExt( App_CAN0_RxTask,                       /*  - RX Task.                                          */
                         0u,
                        &App_CAN0_RxTaskStk[APP_CAN0_RX_TASK_STK_SIZE - 1u],
                         APP_CAN0_RX_TASK_PRIO,
                         APP_CAN0_RX_TASK_PRIO,
                        &App_CAN0_RxTaskStk[0u],
                         APP_CAN0_RX_TASK_STK_SIZE,
                         0u,
                        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

        OSTaskCreateExt( App_CAN0_TxTask,                       /*  - TX Task.                                          */
                         0u,
                        &App_CAN0_TxTaskStk[APP_CAN0_TX_TASK_STK_SIZE - 1u],
                         APP_CAN0_TX_TASK_PRIO,
                         APP_CAN0_TX_TASK_PRIO,
                        &App_CAN0_TxTaskStk[0u],
                         APP_CAN0_TX_TASK_STK_SIZE,
                         0u,
                        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    #endif

    #if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)                   /* ---------------- CAN CHANNEL 1 TASKS --------------- */
        OSTaskCreateExt( App_CAN1_RxTask,                       /*  - RX Task.                                          */
                         0u,
                        &App_CAN1_RxTaskStk[APP_CAN1_RX_TASK_STK_SIZE - 1u],
                         APP_CAN1_RX_TASK_PRIO,
                         APP_CAN1_RX_TASK_PRIO,
                        &App_CAN1_RxTaskStk[0u],
                         APP_CAN1_RX_TASK_STK_SIZE,
                         0u,
                        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

        OSTaskCreateExt( App_CAN1_TxTask,                       /*  - TX Task.                                          */
                         0u,
                        &App_CAN1_TxTaskStk[APP_CAN1_TX_TASK_STK_SIZE - 1u],
                         APP_CAN1_TX_TASK_PRIO,
                         APP_CAN1_TX_TASK_PRIO,
                        &App_CAN1_TxTaskStk[0u],
                         APP_CAN1_TX_TASK_STK_SIZE,
                         0u,
                        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    #endif
#endif



#if (APP_CAN_DEMO_SELECT == APP_CAN_DEMO_ECHO_TASK)             /* Demo 'Echo' Task to Rx & Tx same Msg back            */
    #if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)                   /* ------------ CAN CHANNEL 0 ECHO TASK --------------- */
        OSTaskCreateExt( App_CAN0_EchoTask,
                         0u,
                        &App_CAN0_EchoTask_Stk[APP_CAN0_ECHO_TASK_STK_SIZE - 1u],
                         APP_CAN0_ECHO_TASK_PRIO,
                         APP_CAN0_ECHO_TASK_PRIO,
                        &App_CAN0_EchoTask_Stk[0u],
                         APP_CAN0_ECHO_TASK_STK_SIZE,
                         0u,
                        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    #endif

    #if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)                   /* ------------ CAN CHANNEL 1 ECHO TASK --------------- */
        OSTaskCreateExt( App_CAN1_EchoTask,
                         0u,
                        &App_CAN1_EchoTask_Stk[APP_CAN1_ECHO_TASK_STK_SIZE - 1u],
                         APP_CAN1_ECHO_TASK_PRIO,
                         APP_CAN1_ECHO_TASK_PRIO,
                        &App_CAN1_EchoTask_Stk[0u],
                         APP_CAN1_ECHO_TASK_STK_SIZE,
                         0u,
                        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    #endif
#endif
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           CAN_RX_TX_TASK DEMO
*********************************************************************************************************
*********************************************************************************************************
*/

#if (APP_CAN_DEMO_SELECT == APP_CAN_DEMO_RX_TX_TASK)

#if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)
/*
*********************************************************************************************************
*                                             App_CAN0_RxTask
*
* Description : This Task waits until a new Frame is available from the CAN0 Bus. It Reads the frame
*               and updates the global counter of Received Messages.
*
* Arguments   : p_arg    Argument Pointer passed by OSTaskCreate().
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_CAN0_RxTask(void *p_arg)
{
    CANFRM  can_frm;


    (void)p_arg;                                                /* Avoid Compiler Warning.                              */

    CanBusIoCtl(ZC7xxx_CAN_BUS_0, CANBUS_SET_RX_TIMEOUT, 0u);   /* Set Timeout to wait forever for new CAN Msg.         */

    while (DEF_ON) {
        CanBusRead(         ZC7xxx_CAN_BUS_0,                   /* Pend till New CAN msg is found on CAN BUS            */
                   (void *)&can_frm,
                            sizeof(CANFRM));

        App_CAN0_RxTx_Ctr++;                                    /* Increment Global Counter.                            */
    }
}


/*
*********************************************************************************************************
*                                             App_CAN0_TxTask
*
* Description : This Task will read the Generic CAN0 Global Rx/Tx Counter, Parse the counter to  transmit
*               the count of the Global Counter as part of the CAN Message Data with a specified ID.
*
* Arguments   : p_arg    Argument Pointer passed by OSTaskCreate().
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : (1) To Transmit the value of the Global Counter, the Tx Task will start the parsing and
*                   will place the Rx ctr value in reverse order to be easier to view through the CAN
*                   sniffer. Since the largest value for a 32-bit integer is 0xFFFFFFFF it only requires
*                   a FRM.DLC of 4 since each Data frame is of size 0xFF.
*                       Example:
*                           App_CAN_Rx_Ctr : 0x 11 22 33 44 (in Hex)
*                           FRM.Data[0u] = 44u;
*                           FRM.Data[1u] = 33u;
*                           FRM.Data[2u] = 22u;
*                           FRM.Data[3u] = 11u;
*
*                       so that the Frame.Data appears on the CAN Sniffer as such:
*                           Data[] = 0x 11 22 33 44
*********************************************************************************************************
*/

static  void  App_CAN0_TxTask(void *p_arg)
{
    CANFRM      can_frm;
    CPU_INT32U  rx_ctr;
    CPU_INT08U  dlc_ctr;


    (void)p_arg;                                                /* Avoid Compiler Warning                               */

    while (DEF_ON) {
        can_frm.Identifier = 0x500u;                            /* Specify Identifier to show CANx Bus.                 */
        can_frm.DLC        = 4u;                                /* Always Tx 4 Bytes. See Note (1).                     */

        rx_ctr = App_CAN0_RxTx_Ctr;                             /* Obtain Rx Counter Value.                             */

                                                                /* ----------------- PARSE Rx CTR VAL ----------------- */
        for (dlc_ctr = 1u; dlc_ctr <= can_frm.DLC; dlc_ctr++) {
            can_frm.Data[(can_frm.DLC - dlc_ctr)] = ((rx_ctr >> (8u * (dlc_ctr - 1u))) & 0xFFu);
        }

        CanBusWrite(ZC7xxx_CAN_BUS_0, (void *)&can_frm, sizeof(CANFRM));

        OSTimeDlyHMSM(0u, 0u, 1u, 0u);
    }
}
#endif                                                          /* END CAN_MODULE_CHANNEL_0                             */


#if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)
/*
*********************************************************************************************************
*                                             App_CAN1_RxTask
*
* Description : This Task waits until a new Frame is available from the CAN0 Bus. It Reads the frame
*               and updates the global counter of Received Messages.
*
* Arguments   : p_arg    Argument Pointer passed by OSTaskCreate().
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  App_CAN1_RxTask(void *p_arg)
{
    CANFRM  can_frm;


    (void)p_arg;                                                /* Avoid Compiler Warning.                              */

    CanBusIoCtl(ZC7xxx_CAN_BUS_1, CANBUS_SET_RX_TIMEOUT, 0u);   /* Set Timeout to wait forever for new CAN Msg.         */

    while (DEF_ON) {
        CanBusRead(         ZC7xxx_CAN_BUS_1,                   /* Pend till New CAN msg is found on CAN BUS            */
                   (void *)&can_frm,
                            sizeof(CANFRM));

        App_CAN0_RxTx_Ctr++;                                    /* Increment Global Counter.                            */
    }
}


/*
*********************************************************************************************************
*                                             App_CAN1_TxTask
*
* Description : This Task will read the Generic CAN1 Global Rx/Tx Counter, Parse the counter to  transmit
*               the count of the Global Counter as part of the CAN Message Data with a specified ID.
*
* Arguments   : p_arg    Argument Pointer passed by OSTaskCreate().
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : (1) To Transmit the value of the Global Counter, the Tx Task will start the parsing and
*                   will place the Rx ctr value in reverse order to be easier to view through the CAN
*                   sniffer. Since the largest value for a 32-bit integer is 0xFFFFFFFF it only requires
*                   a FRM.DLC of 4 since each Data frame is of size 0xFF.
*                       Example:
*                           App_CAN_Rx_Ctr : 0x 11 22 33 44 (in Hex)
*                           FRM.Data[0u] = 44u;
*                           FRM.Data[1u] = 33u;
*                           FRM.Data[2u] = 22u;
*                           FRM.Data[3u] = 11u;
*
*                       so that the Frame.Data appears on the CAN Sniffer as such:
*                           Data[] = 0x 11 22 33 44
*********************************************************************************************************
*/

static  void  App_CAN1_TxTask(void *p_arg)
{
    CANFRM      can_frm;
    CPU_INT32U  rx_ctr;
    CPU_INT08U  dlc_ctr;


    (void)p_arg;                                                /* Avoid Compiler Warning                               */

    while (DEF_ON) {
        can_frm.Identifier = 0x501u;                            /* Specify Identifier to show CANx Bus.                 */
        can_frm.DLC        = 4u;                                /* Always Tx 4 Bytes. See Note (1).                     */

        rx_ctr = App_CAN1_RxTx_Ctr;                             /* Obtain Rx Counter Value.                             */

                                                                /* ----------------- PARSE Rx CTR VAL ----------------- */
        for (dlc_ctr = 1u; dlc_ctr <= can_frm.DLC; dlc_ctr++) {
            can_frm.Data[(can_frm.DLC - dlc_ctr)] = ((rx_ctr >> (8u * (dlc_ctr - 1u))) & 0xFFu);
        }

        CanBusWrite(ZC7xxx_CAN_BUS_1, (void *)&can_frm, sizeof(CANFRM));

        OSTimeDlyHMSM(0u, 0u, 1u, 0u);
    }
}
#endif                                                          /* END CAN_MODULE_CHANNEL_1                             */


/*
*********************************************************************************************************
*                                   APP_CAN_DEMO_RX_TX_TASK END
*********************************************************************************************************
*/

#endif                                                          /* END APP_CAN_DEMO_RX_TX_TASK                          */


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            CAN_ECHO_TASK DEMO
*********************************************************************************************************
*********************************************************************************************************
*/

#if (APP_CAN_DEMO_SELECT == APP_CAN_DEMO_ECHO_TASK)

/*
*********************************************************************************************************
*                                            ECHO TASK
*
* Description: This function echos a received frame back to the sender. The frame Identifier is
*              changed before echoing. Using CAN Channel 0
*
* Arguments  : p_arg (unused).
*********************************************************************************************************
*/

#if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)
static  void  App_CAN0_EchoTask (void *p_arg)
{
    CANFRM frm;


    (void)p_arg;                                                /* Avoid Compiler Warning                               */

    CanBusIoCtl(ZC7xxx_CAN_BUS_0, CANBUS_SET_RX_TIMEOUT, 0u);
                                                                /* Endless task loop                                    */
    while (DEF_ON) {
        CanBusRead(ZC7xxx_CAN_BUS_0, (void *)&frm, sizeof(CANFRM));
        frm.Identifier = 0x100L;                                /* Specify Identifier to show CAN0 for Echo             */
        CanBusWrite(ZC7xxx_CAN_BUS_0, (void *)&frm, sizeof(CANFRM));
   }
}
#endif                                                          /* END CAN_MODULE_CHANNEL_0                             */


/*
*********************************************************************************************************
*                                            ECHO TASK
*
* Description: This function echos a received frame back to the sender. The frame Identifier is
*              changed before echoing. Using CAN Channel 0
*
* Arguments  : p_arg (unused).
*********************************************************************************************************
*/

#if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)
static  void  App_CAN1_EchoTask (void *p_arg)
{
    CANFRM frm;


    (void)p_arg;                                                /* Avoid Compiler Warning                               */

    CanBusIoCtl(ZC7xxx_CAN_BUS_1, CANBUS_SET_RX_TIMEOUT, 0u);
                                                                /* Endless task loop                                    */
    while (DEF_ON) {
        CanBusRead(ZC7xxx_CAN_BUS_1, (void *)&frm, sizeof(CANFRM));
        frm.Identifier = 0x200L;                                /* Specify Identifier to show CAN1 for Echo             */
        CanBusWrite(ZC7xxx_CAN_BUS_1, (void *)&frm, sizeof(CANFRM));
   }
}
#endif                                                          /* END CAN_MODULE_CHANNEL_1                             */


/*
*********************************************************************************************************
*                                    APP_CAN_DEMO_ECHO_TASK END
*********************************************************************************************************
*/

#endif                                                          /* END APP_CAN_DEMO_ECHO_TASK */
