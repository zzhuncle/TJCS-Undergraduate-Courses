/*
*********************************************************************************************************
*                                             uC/USB-Host
*                                     The Embedded USB Host Stack
*
*                         (c) Copyright 2004-2019; Silicon Laboratories Inc.,
*                                400 W. Cesar Chavez, Austin, TX 78701
*
*                   All rights reserved. Protected by international copyright laws.
*
*                  Your use of this software is subject to your acceptance of the terms
*                  of a Silicon Labs Micrium software license, which can be obtained by
*                  contacting info@micrium.com. If you do not agree to the terms of this
*                  license, you may not use this software.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available. Your honesty is greatly appreciated.
*
*                    You can find our product's documentation at: doc.micrium.com
*
*                          For more information visit us at: www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                  COMMUNICATIONS DEVICE CLASS (CDC)
*                                    ABSTRACT CONTROL MODEL (ACM)
*
* Filename : usbh_acm.h
* Version  : V3.41.09
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  USBH_CDC_ACM_MODULE_PRESENT
#define  USBH_CDC_ACM_MODULE_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "../usbh_cdc.h"


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   USBH_CDC_ACM_MODULE
#define  USBH_CDC_ACM_EXT
#else
#define  USBH_CDC_ACM_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  USBH_CDC_ACM_DTR_SET                           0x01u   /* RS-232 signal Data Terminal Ready                    */
#define  USBH_CDC_ACM_RTS_SET                           0x01u   /* RS-232 signal Request To Send                        */
#define  USBH_CDC_ACM_DTR_CLR                           0x00u   /* RS-232 signal Data Terminal Ready                    */
#define  USBH_CDC_ACM_RTS_CLR                           0x00u   /* RS-232 signal Request To Send                        */

#define  USBH_CDC_ACM_SER_DCD                           0x01u   /* RS-232 signal Data Carrier Detection                 */
#define  USBH_CDC_ACM_SER_DSR                           0x02u   /* RS-232 signal Data Set Ready                         */
#define  USBH_CDC_ACM_SER_BRK                           0x04u   /* RS-232 signal Break                                  */
#define  USBH_CDC_ACM_SER_RING_INDC                     0x08u   /* RS-232 signal Ring Indicator                         */
#define  USBH_CDC_ACM_SER_FRAME_ERR                     0x10u   /* RS-232 signal Frame Error                            */
#define  USBH_CDC_ACM_SER_PARITY_ERR                    0x20u   /* RS-232 signal Parity Error                           */
#define  USBH_CDC_ACM_SER_OVER_RUN                      0x40u   /* RS-232 signal Buffer Over Run Error                  */


/*
*********************************************************************************************************
*                                     SET/GET LINE CODING VALUES
*
* Note(s) : (1) See "Universal Serial Bus Communications Class Subclass Specification for PSTN Devices",
*               version 1.2, February 9, 2007. Section 6.3.11 Table 17
*********************************************************************************************************
*/

#define  USBH_CDC_ACM_LINE_CODING_BAUDRATE_110           110u
#define  USBH_CDC_ACM_LINE_CODING_BAUDRATE_300           300u
#define  USBH_CDC_ACM_LINE_CODING_BAUDRATE_1200         1200u
#define  USBH_CDC_ACM_LINE_CODING_BAUDRATE_2400         2400u
#define  USBH_CDC_ACM_LINE_CODING_BAUDRATE_4800         4800u
#define  USBH_CDC_ACM_LINE_CODING_BAUDRATE_9600         9600u
#define  USBH_CDC_ACM_LINE_CODING_BAUDRATE_19200       19200u
#define  USBH_CDC_ACM_LINE_CODING_BAUDRATE_38400       38400u
#define  USBH_CDC_ACM_LINE_CODING_BAUDRATE_56700       56700u
#define  USBH_CDC_ACM_LINE_CODING_BAUDRATE_115200     115200u
#define  USBH_CDC_ACM_LINE_CODING_BAUDRATE_230400     230400u
#define  USBH_CDC_ACM_LINE_CODING_BAUDRATE_460800     460800u
#define  USBH_CDC_ACM_LINE_CODING_BAUDRATE_921600     921600u

#define  USBH_CDC_ACM_LINE_CODING_STOP_BIT_1               0u
#define  USBH_CDC_ACM_LINE_CODING_STOP_BIT_1_5             1u
#define  USBH_CDC_ACM_LINE_CODING_STOP_BIT_2               2u

#define  USBH_CDC_ACM_LINE_CODING_PARITY_NONE              0u
#define  USBH_CDC_ACM_LINE_CODING_PARITY_ODD               1u
#define  USBH_CDC_ACM_LINE_CODING_PARITY_EVEN              2u
#define  USBH_CDC_ACM_LINE_CODING_PARITY_MARK              3u
#define  USBH_CDC_ACM_LINE_CODING_PARITY_SPACE             4u

#define  USBH_CDC_ACM_LINE_CODING_DATA_BITS_5              5u
#define  USBH_CDC_ACM_LINE_CODING_DATA_BITS_6              6u
#define  USBH_CDC_ACM_LINE_CODING_DATA_BITS_7              7u
#define  USBH_CDC_ACM_LINE_CODING_DATA_BITS_8              8u


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

typedef  struct  usbh_cdc_serial_state {
    CPU_BOOLEAN  RxCarrier;
    CPU_BOOLEAN  TxCarrier;
    CPU_BOOLEAN  Break;
    CPU_BOOLEAN  RingSignal;
    CPU_BOOLEAN  Framing;
    CPU_BOOLEAN  Parity;
    CPU_BOOLEAN  OverRun;
} USBH_CDC_SERIAL_STATE;

typedef  struct usbh_cdc_acm_notifications {
    CPU_BOOLEAN  NetworkConnection;
    CPU_BOOLEAN  SerialState;
} USBH_CDC_ACM_NOTIFICATIONS;

typedef  struct usbh_cdc_acm_requests {
    CPU_BOOLEAN  SetCommFeature;
    CPU_BOOLEAN  GetCommFeature;
    CPU_BOOLEAN  ClrCommFeature;
    CPU_BOOLEAN  SetLineCoding;
    CPU_BOOLEAN  GetLineCoding;
    CPU_BOOLEAN  SetControlLineState;
    CPU_BOOLEAN  SendBreak;
} USBH_CDC_ACM_REQUESTS;

typedef  void  (*USBH_CDC_SERIAL_STATE_NOTIFY) (USBH_CDC_SERIAL_STATE   serial_sate);

typedef  void  (*USBH_CDC_DATA_NOTIFY)         (void                   *p_data,
                                                CPU_INT08U             *p_buf,
                                                CPU_INT32U              xfer_len,
                                                USBH_ERR                err);

typedef  struct  usbh_cdc_acm_dev {
    USBH_CDC_DEV                  *CDC_DevPtr;
    USBH_CDC_ACM_NOTIFICATIONS     SupportedEvents;
    USBH_CDC_ACM_REQUESTS          SupportedRequests;
    CPU_INT08U                     LineCodingBuf[10];
    USBH_CDC_SERIAL_STATE_NOTIFY   EvtSerialStateNotifyPtr;
    USBH_CDC_DATA_NOTIFY           DataTxNotifyPtr;
    void                          *DataTxArgPtr;
    USBH_CDC_DATA_NOTIFY           DataRxNotifyPtr;
    void                          *DataRxArgPtr;
} USBH_CDC_ACM_DEV;


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

USBH_CDC_ACM_EXT  USBH_CLASS_DRV  USBH_CDC_ACM_ClassDrv;


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

USBH_ERR           USBH_CDC_ACM_GlobalInit      (void);

void               USBH_CDC_ACM_EventRxNotifyReg(USBH_CDC_ACM_DEV              *p_cdc_acm_dev,
                                                 USBH_CDC_SERIAL_STATE_NOTIFY   p_serial_state_notify);


USBH_CDC_ACM_DEV  *USBH_CDC_ACM_Add             (USBH_CDC_DEV                  *p_cdc_dev,
                                                 USBH_ERR                      *p_err);

USBH_ERR           USBH_CDC_ACM_Remove          (USBH_CDC_ACM_DEV              *p_cdc_acm_dev);

USBH_ERR           USBH_CDC_ACM_LineCodingSet   (USBH_CDC_ACM_DEV              *p_cdc_acm_dev,
                                                 CPU_INT32U                     baud_rate,
                                                 CPU_INT08U                     stop_bits,
                                                 CPU_INT08U                     parity_val,
                                                 CPU_INT08U                     data_bits);

USBH_ERR           USBH_CDC_ACM_LineCodingGet   (USBH_CDC_ACM_DEV              *p_cdc_acm_dev,
                                                 CPU_INT32U                    *p_baud_rate,
                                                 CPU_INT08U                    *p_stop_bits,
                                                 CPU_INT08U                    *p_parity_val,
                                                 CPU_INT08U                    *p_data_bits);

USBH_ERR           USBH_CDC_ACM_LineStateSet    (USBH_CDC_ACM_DEV              *p_cdc_acm_dev,
                                                 CPU_INT08U                     dtr_bit,
                                                 CPU_INT08U                     rts_bit);

USBH_ERR           USBH_CDC_ACM_BreakSend       (USBH_CDC_ACM_DEV              *p_cdc_acm_dev,
                                                 CPU_INT16U                     break_time);

USBH_ERR           USBH_CDC_ACM_CmdSend         (USBH_CDC_ACM_DEV              *p_cdc_acm_dev,
                                                 CPU_INT08U                    *p_buf,
                                                 CPU_INT32U                     buf_len);

USBH_ERR           USBH_CDC_ACM_RespRx          (USBH_CDC_ACM_DEV              *p_cdc_acm_dev,
                                                 CPU_INT08U                    *p_buf,
                                                 CPU_INT32U                     buf_len);

CPU_INT32U         USBH_CDC_ACM_DataTx          (USBH_CDC_ACM_DEV              *p_cdc_acm_dev,
                                                 void                          *p_buf,
                                                 CPU_INT32U                     buf_len,
                                                 USBH_ERR                      *p_err);

CPU_INT32U         USBH_CDC_ACM_DataRx          (USBH_CDC_ACM_DEV              *p_cdc_acm_dev,
                                                 void                          *p_buf,
                                                 CPU_INT32U                     buf_len,
                                                 USBH_ERR                      *p_err);

USBH_ERR           USBH_CDC_ACM_DataTxAsync     (USBH_CDC_ACM_DEV              *p_cdc_acm_dev,
                                                 void                          *p_buf,
                                                 CPU_INT32U                     buf_len,
                                                 USBH_CDC_DATA_NOTIFY           tx_cmpl_notify,
                                                 void                          *p_tx_cmpl_arg);

USBH_ERR           USBH_CDC_ACM_DataRxAsync     (USBH_CDC_ACM_DEV              *p_cdc_acm_dev,
                                                 void                          *p_buf,
                                                 CPU_INT32U                     buf_len,
                                                 USBH_CDC_DATA_NOTIFY           rx_cmpl_notify,
                                                 void                          *p_rx_cmpl_arg);


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 END
*********************************************************************************************************
*/

#endif
