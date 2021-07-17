/*
*********************************************************************************************************
*                                               uC/MQTTc
*                                Message Queue Telemetry Transport Client
*
*                         (c) Copyright 2014-2017; Micrium, Inc.; Weston, FL
*
*                  All rights reserved.  Protected by international copyright laws.
*
*                  uC/MQTTc is provided in source form to registered licensees ONLY.  It is
*                  illegal to distribute this source code to any third party unless you receive
*                  written permission by an authorized Micrium representative.  Knowledge of
*                  the source code may NOT be used to develop a similar product.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available.  Your honesty is greatly appreciated.
*
*                  You can find our product's user manual, API reference, release notes and
*                  more information at: https://doc.micrium.com
*
*                  You can contact us at: http://www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                             MQTT CLIENT
*
* Filename      : mqtt-c_sock.h
* Version       : V1.01.00
* Programmer(s) : OD
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This main network protocol suite header file is protected from multiple pre-processor
*               inclusion through use of the MQTTc module present pre-processor macro definition.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  MQTTc_CONN_MODULE_PRESENT
#define  MQTTc_CONN_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  "mqtt-c.h"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/

typedef  enum  mqttc_sel_desc_type {
    MQTTc_SEL_DESC_TYPE_RD,
    MQTTc_SEL_DESC_TYPE_WR,
    MQTTc_SEL_DESC_TYPE_ERR
} MQTTc_SEL_DESC_TYPE;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

void         MQTTc_SockConnOpen   (MQTTc_CONN           *p_conn,
                                   MQTTc_ERR            *p_err);

void         MQTTc_SockConnClose  (MQTTc_CONN           *p_conn,
                                   MQTTc_ERR            *p_err);

CPU_INT32U   MQTTc_SockTx         (MQTTc_CONN           *p_conn,
                                   CPU_INT08U           *p_buf,
                                   CPU_INT32U            buf_len,
                                   MQTTc_ERR            *p_err);

CPU_INT32U   MQTTc_SockRx         (MQTTc_CONN           *p_conn,
                                   CPU_INT08U           *p_buf,
                                   CPU_INT32U            buf_len,
                                   MQTTc_ERR            *p_err);

void         MQTTc_SockSelDescSet (MQTTc_CONN           *p_conn,
                                   MQTTc_SEL_DESC_TYPE   sel_desc_type);

void         MQTTc_SockSelDescClr (MQTTc_CONN           *p_conn,
                                   MQTTc_SEL_DESC_TYPE   sel_desc_type);

CPU_BOOLEAN  MQTTc_SockSelDescProc(MQTTc_CONN           *p_conn,
                                   MQTTc_SEL_DESC_TYPE   sel_desc_type);

void         MQTTc_SockSel        (MQTTc_CONN           *p_head_conn,
                                   MQTTc_ERR            *p_err);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif
