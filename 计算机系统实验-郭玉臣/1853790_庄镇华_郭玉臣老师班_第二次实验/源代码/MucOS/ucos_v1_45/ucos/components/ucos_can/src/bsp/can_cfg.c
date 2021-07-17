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
*                                        uC/CAN CONFIGURATION
*
*                                         ZYNQ ZC7000 Series
*                                             CAN DRIVER
*
* File : can_cfg.c
****************************************************************************************************
*/


/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#include  "can_cfg.h"
#include  "can_sig.h"
#include  "can_bus.h"
#include  "can_frm.h"
#include  "can_msg.h"
#include  "can_os.h"
#include  "drv_can.h"


/*
****************************************************************************************************
*                                             DEFINES
****************************************************************************************************
*/


/*
****************************************************************************************************
*                                              MACROS
****************************************************************************************************
*/

#if (CANSIG_GRANULARITY == CAN_CFG_BIT)
#define  CAN_CFG_SIZE_MOD       8
#else
#define  CAN_CFG_SIZE_MOD       1
#endif


/*
****************************************************************************************************
*                                            LOCAL DATA
****************************************************************************************************
*/

/*
****************************************************************************************************
*                                  CAN SIGNAL CALLBACK DEFINITION
*
* Description : Prototype of Signal Callback Function. This is only an example to be replaced by
*               one or more user Callback Functions.
*
* Note(s)     : none.
****************************************************************************************************
*/

#if (CANSIG_CALLBACK_EN == 1)
void  CallBackFct (void  *p_arg, CANSIG_VAL_T  *val, CPU_INT32U  callback_id);
#endif


/*
****************************************************************************************************
*                                            GOBAL DATA
****************************************************************************************************
*/


/*
****************************************************************************************************
*                                        CAN BUS SEMAPHORES
*
* Description :  Allocation of Semaphores for Buffering Resources. The following
*                implementation is an implementation for uC/OS-II.
*
* Note(s)     : none.
****************************************************************************************************
*/

#if (OS_VERSION > 30000u)
OS_SEM  CANOS_TxSem[CANBUS_N];
OS_SEM  CANOS_RxSem[CANBUS_N];
#else
OS_EVENT  CANOS_TxSem[CANBUS_N];
OS_EVENT  CANOS_RxSem[CANBUS_N];
#endif


/*
****************************************************************************************************
*                                           CAN SIGNALS
*
* Description : Allocation of CAN Signals
*
* Note(s)     : This table must be modified by the user to define all Signals needed for the
*               Application. The below defined Signals are only examples and might be modified or
*               removed.
****************************************************************************************************
*/

const  CANSIG_PARA  CanSig[CANSIG_N] = {
                                                                /* ---------------- SIGNAL NODESTATUS ----------------- */
    {CANSIG_UNCHANGED,                                          /*      Initial Status                                  */
      1,                                                        /*      Width in Bytes                                  */
      0,                                                        /*      Initial Value                                   */
#if (CANSIG_CALLBACK_EN > 0)
      0},                                                       /*      Callback Function: User Defined                 */
#else
    },
#endif
                                                                /* ----------------- SIGNAL CPULOAD ------------------- */
    {CANSIG_UNCHANGED,                                          /*      Initial Status                                  */
      1,                                                        /*      Width in Bytes                                  */
      0,                                                        /*      Initial Value                                   */
#if (CANSIG_CALLBACK_EN > 0)
      0}                                                        /*      No Callback                                     */
#else
    },
#endif
};


/*
****************************************************************************************************
*                                           CAN MESSAGES
*
* Description : Allocation of CAN Messages
*
* Note(s)     : This Table must be modified by the user to define all Messages needed for
*               the Application. The below defined Messages are only examples and might be
*               modified or removed.
****************************************************************************************************
*/

const  CANMSG_PARA  CanMsg[CANMSG_N] =
{
                                                                /* ------------------ MESSAGE STATUS ------------------ */
   { 0x123L,                                                    /*      CAN-Identifier                                  */
     CANMSG_TX,                                                 /*      Message Type                                    */
     3,                                                         /*      DLC of Message                                  */
     3,                                                         /*      No. of Links                                    */
      { { S_NODESTATUS,                                         /*      Signal ID                                       */
         0 },                                                   /*      Byte Position                                   */
        { S_COUNTER,                                            /*      Signal ID                                       */
         1 },                                                   /*      Byte Position                                   */
      { S_CPULOAD,                                              /*      Signal ID                                       */
       2 } } },                                                 /*      Byte Position                                   */

                                                                /* ----------------- MESSAGE COMMAND ------------------ */
   { 0x122L,                                                    /*      CAN-Identifier                                  */
     CANMSG_RX,                                                 /*      Message Type                                    */
     1,                                                         /*      DLC of Message                                  */
     1,                                                         /*      No. of Links                                    */
     { { S_NODESTATUS,                                          /*      Signal ID                                       */
       0 } } }                                                  /*      Byte Position                                   */
};


/*
****************************************************************************************************
*                                     CAN SIGNAL CONFIGURATION
*
* Description : Allocation of Global CAN Signal Table.
*
* Note(s)     : This is the Signal Table on which the CAN
*               Signal Layer will work. If the CANSIG_STATIC Configuration is chosen it will be
*               initialized with the CanCfg_Init() Function otherwise it will be filled by calling
*               CanSigCreate().
****************************************************************************************************
*/

CANSIG_DATA  CanSigTbl[CANSIG_N];


/*
****************************************************************************************************
*                                      CAN BUS CONFIGURATION
*
* Description : This structure contains the information for a bus for Can Controller 0.
*               A bus represents one interface to the world.
*
* Note(s)     : none.
****************************************************************************************************
*/

#if  (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)
const  CANBUS_PARA  CanCfg0 = {
    CAN_FALSE,                                                  /* EXTENDED FLAG                                        */
    CAN_DEFAULT_BAUDRATE,                                       /* BAUDRATE                                             */
    0u,                                                         /* BUS NODE                                             */
    0u,                                                         /* BUS DEVICE                                           */
                                                                /* DRIVER FUNCTIONS                                     */
    ZC7xxx_CAN_Init,                                            /*      Init                                            */
    ZC7xxx_CAN_Open,                                            /*      Open                                            */
    ZC7xxx_CAN_Close,                                           /*      Close                                           */
    ZC7xxx_CAN_IoCtl,                                           /*      IoCtl                                           */
    ZC7xxx_CAN_Read,                                            /*      Read                                            */
    ZC7xxx_CAN_Write,                                           /*      Write                                           */
    {                                                           /* DRIVER IO FUNCTION CODES  (Init Drv Codes)           */
        IO_ZC7xxx_CAN_SET_BAUDRATE,                             /*      Set Baud Rate                                   */
        IO_ZC7xxx_CAN_START,                                    /*      Start                                           */
        IO_ZC7xxx_CAN_CONFIG,                                   /*      Config                                          */
        IO_ZC7xxx_CAN_RX_STANDARD,                              /*      Rx Standard                                     */
        IO_ZC7xxx_CAN_RX_EXTENDED,                              /*      Rx Extended                                     */
        IO_ZC7xxx_CAN_TX_READY,                                 /*      Tx Ready                                        */
        IO_ZC7xxx_CAN_GET_NODE_STATUS,                          /*      Get Node Status                                 */
    }
};
#endif


/*
****************************************************************************************************
*                                      CAN BUS CONFIGURATION
*
* Description : This structure contains the information for a bus for Can Controller 1.
*               A bus represents one interface to the world.
*
* Note(s)     : none.
****************************************************************************************************
*/

#if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)
const  CANBUS_PARA  CanCfg1 = {
    CAN_FALSE,                                                  /* EXTENDED FLAG                                        */
    CAN_DEFAULT_BAUDRATE,                                       /* BAUDRATE                                             */
    1u,                                                         /* BUS NODE                                             */
    1u,                                                         /* BUS DEVICE                                           */
                                                                /* DRIVER FUNCTIONS                                     */
    ZC7xxx_CAN_Init,                                            /*      Init                                            */
    ZC7xxx_CAN_Open,                                            /*      Open                                            */
    ZC7xxx_CAN_Close,                                           /*      Close                                           */
    ZC7xxx_CAN_IoCtl,                                           /*      IoCtl                                           */
    ZC7xxx_CAN_Read,                                            /*      Read                                            */
    ZC7xxx_CAN_Write,                                           /*      Write                                           */
    {                                                           /* DRIVER IO FUNCTION CODES  (Init Drv Codes)           */
        IO_ZC7xxx_CAN_SET_BAUDRATE,                             /*      Set Baud Rate                                   */
        IO_ZC7xxx_CAN_START,                                    /*      Start                                           */
        IO_ZC7xxx_CAN_CONFIG,                                   /*      Config                                          */
        IO_ZC7xxx_CAN_RX_STANDARD,                              /*      Rx Standard                                     */
        IO_ZC7xxx_CAN_RX_EXTENDED,                              /*      Rx Extended                                     */
        IO_ZC7xxx_CAN_TX_READY,                                 /*      Tx Ready                                        */
        IO_ZC7xxx_CAN_GET_NODE_STATUS,                          /*      Get Node Status                                 */
    }
};
#endif


/*
*********************************************************************************************************
*                                       CAN BUS NODE STATUS HOOK
*
* Description: This function is called by CanBusNSHandler().
*
* Arguments  : bus Id
*********************************************************************************************************
*/

#if CANBUS_HOOK_NS_EN == 1
void  CanBusNSHook (CPU_INT16S  bus_id)
{
    (void)&bus_id;                                              /* Prevent Compiler Warning                             */
}
#endif


/*
*********************************************************************************************************
*                                            CAN BUS RX HOOK
*
* Description: This function is called by CanBusRxHandler().
*
* Arguments  : bus Id
*********************************************************************************************************
*/

#if CANBUS_HOOK_RX_EN == 1
CPU_INT16S  CanBusRxHook (CPU_INT16S  bus_id, void  *buf)
{
    (void)&bus_id;                                              /* Prevent Compiler Warning                             */
    return (0);
}
#endif


/*
*********************************************************************************************************
*                                           CALLBACK FUNCTION
*
* Description: This function is the callback function example of the CAN signal layer. It
*              has to be replaced by one or more user callback functions.
*
* Arguments  : arg          pointer to signal
*              value        of signal
*              callback_id  to identify where this callback function was called from
*********************************************************************************************************
*/

#if CANSIG_CALLBACK_EN == 1
void  CallBackFct (void  *p_arg, CANSIG_VAL_T  *val, CPU_INT32U  callback_id)
{
    (void)&callback_id;                                         /* Prevent Compiler Warning                             */
    (void)&val;                                                 /* Prevent Compiler Warning                             */
    (void)&p_arg;                                               /* Prevent Compiler Warning                             */

}
#endif
