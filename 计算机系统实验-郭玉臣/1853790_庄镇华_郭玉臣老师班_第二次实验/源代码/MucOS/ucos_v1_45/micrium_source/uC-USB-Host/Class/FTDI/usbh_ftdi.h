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
*                            FUTURE TECHNOLOGY DEVICES INTERNATIONAL CLASS
*
* Filename : usbh_ftdi.h
* Version  : V3.41.09
* Note(s)  : (1) The reference document "API for FTxxxx Devices Application Note AN_115" can be
*                requested from FTDI after signing a Non-Disclosure Agreement (NDA).
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  USBH_FTDI_MODULE_PRESENT
#define  USBH_FTDI_MODULE_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "../../Source/usbh_class.h"


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   USBH_FTDI_MODULE
#define  USBH_FTDI_EXT
#else
#define  USBH_FTDI_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  USBH_FTDI_SERIAL_STATUS_LEN                   2u


/*
*********************************************************************************************************
*                                          FTDI_RESET CONTROL
*
* Note(s) : (1) See "API for FTxxxx Devices Application Note AN_115", April 04 2011, Version 1.1,
*               section 3.1.
*
*           (2) The control value is defined as follow:
*
*               USBH_FTDI_RESET_CTRL_SIO                Reset SIO (Resets both RX and TX buffer)
*               USBH_FTDI_RESET_CTRL_RX                 Purge RX buffer
*               USBH_FTDI_RESET_CTRL_TX                 Purge TX buffer
*
*********************************************************************************************************
*/

#define  USBH_FTDI_RESET_CTRL_SIO                       DEF_BIT_NONE
#define  USBH_FTDI_RESET_CTRL_RX                        DEF_BIT_00
#define  USBH_FTDI_RESET_CTRL_TX                        DEF_BIT_01


/*
*********************************************************************************************************
*                                          FTDI_MODEM CONTROL
*
* Note(s) : (1) See "API for FTxxxx Devices Application Note AN_115", April 04 2011, Version 1.1,
*               section 3.2.
*
*           (2) The control value is defined as follow:
*
*               USBH_FTDI_MODEM_CTRL_DTR_SET            Set     DTR state
*               USBH_FTDI_MODEM_CTRL_DTR_RESET          Reset   DTR state
*               USBH_FTDI_MODEM_CTRL_DTR_ENABLED        Use     DTR state
*               USBH_FTDI_MODEM_CTRL_DTR_DISABLED       Disable DTR state
*               USBH_FTDI_MODEM_CTRL_RTS_SET            Set     RTS state
*               USBH_FTDI_MODEM_CTRL_RTS_RESET          Reset   RTS state
*               USBH_FTDI_MODEM_CTRL_RTS_ENABLED        Use     RTS state
*               USBH_FTDI_MODEM_CTRL_RTS_DISABLED       Disable RTS state
*
*********************************************************************************************************
*/

#define  USBH_FTDI_MODEM_CTRL_DTR_SET                   DEF_BIT_00
#define  USBH_FTDI_MODEM_CTRL_DTR_RESET                 DEF_BIT_NONE
#define  USBH_FTDI_MODEM_CTRL_DTR_ENABLED               DEF_BIT_08
#define  USBH_FTDI_MODEM_CTRL_DTR_DISABLED              DEF_BIT_NONE
#define  USBH_FTDI_MODEM_CTRL_RTS_SET                   DEF_BIT_01
#define  USBH_FTDI_MODEM_CTRL_RTS_RESET                 DEF_BIT_NONE
#define  USBH_FTDI_MODEM_CTRL_RTS_ENABLED               DEF_BIT_09
#define  USBH_FTDI_MODEM_CTRL_RTS_DISABLED              DEF_BIT_NONE


/*
*********************************************************************************************************
*                                        FTDI_SET_FLOW PROTOCOL
*
* Note(s) : (1) See "API for FTxxxx Devices Application Note AN_115", April 04 2011, Version 1.1,
*               section 3.3.
*
*           (2) The control value is defined as follow:
*
*               USBH_FTDI_PROTOCOL_RTS_CTS              Enable output handshaking using RTS/CTS
*               USBH_FTDI_PROTOCOL_DTR_DSR              Enable output handshaking using DTR/DSR
*               USBH_FTDI_PROTOCOL_XON_XOFF             Enable Xon/Xoff handshaking
*
*********************************************************************************************************
*/

#define  USBH_FTDI_PROTOCOL_RTS_CTS                     DEF_BIT_00
#define  USBH_FTDI_PROTOCOL_DTR_DSR                     DEF_BIT_01
#define  USBH_FTDI_PROTOCOL_XON_XOFF                    DEF_BIT_02


/*
*********************************************************************************************************
*                                         FTDI_SET_BAUD_RATE
*
* Note(s) : (1) See "API for FTxxxx Devices Application Note AN_115", April 04 2011, Version 1.1,
*               section 3.4.
*
*********************************************************************************************************
*/

#define  USBH_FTDI_BAUD_RATE_300                        0x2710u
#define  USBH_FTDI_BAUD_RATE_600                        0x1388u
#define  USBH_FTDI_BAUD_RATE_1200                       0x09C4u
#define  USBH_FTDI_BAUD_RATE_2400                       0x04E2u
#define  USBH_FTDI_BAUD_RATE_4800                       0x0271u
#define  USBH_FTDI_BAUD_RATE_9600                       0x4138u
#define  USBH_FTDI_BAUD_RATE_19200                      0x809Cu
#define  USBH_FTDI_BAUD_RATE_38400                      0xC04Eu
#define  USBH_FTDI_BAUD_RATE_57600                      0x0034u
#define  USBH_FTDI_BAUD_RATE_115200                     0x001Au
#define  USBH_FTDI_BAUD_RATE_230400                     0x000Du
#define  USBH_FTDI_BAUD_RATE_460800                     0x4006u
#define  USBH_FTDI_BAUD_RATE_921600                     0x8003u


/*
*********************************************************************************************************
*                                    FTDI_SET_DATA CHARACTERISTICS
*
* Note(s) : (1) See "API for FTxxxx Devices Application Note AN_115", April 04 2011, Version 1.1,
*               section 3.5.
*
*           (2) The control value is defined as follow:
*
*               USBH_FTDI_DATA_PARITY_NONE              Do not use the parity bit
*               USBH_FTDI_DATA_PARITY_ODD               Use odd        parity bit
*               USBH_FTDI_DATA_PARITY_EVEN              Use even       parity bit
*               USBH_FTDI_DATA_PARITY_MARK              Use mark       parity bit
*               USBH_FTDI_DATA_PARITY_SPACE             Use space      parity bit
*
*               USBH_FTDI_DATA_STOP_BITS_1              Use 1 stop bit
*               USBH_FTDI_DATA_STOP_BITS_2              Use 2 stop bit
*
*               USBH_FTDI_DATA_BREAK_ENABLED            Send break
*               USBH_FTDI_DATA_BREAK_DISABLED           Stop break
*
*********************************************************************************************************
*/

#define  USBH_FTDI_DATA_PARITY_NONE                     0x00u
#define  USBH_FTDI_DATA_PARITY_ODD                      0x01u
#define  USBH_FTDI_DATA_PARITY_EVEN                     0x02u
#define  USBH_FTDI_DATA_PARITY_MARK                     0x03u
#define  USBH_FTDI_DATA_PARITY_SPACE                    0x04u

#define  USBH_FTDI_DATA_STOP_BITS_1                     0x00u
#define  USBH_FTDI_DATA_STOP_BITS_2                     0x08u

#define  USBH_FTDI_DATA_BREAK_ENABLED                   0x40u
#define  USBH_FTDI_DATA_BREAK_DISABLED                  0x00u


/*
*********************************************************************************************************
*                                          FTDI SERIAL STATUS
*
* Note(s) : (1) See "API for FTxxxx Devices Application Note AN_115", April 04 2011, Version 1.1,
*               section 4.
*
*           (2) These defines may be used as a mask to determine whether a field of the modem status or
*               the line status is enabled or not.
*
*********************************************************************************************************
*/

#define  USBH_FTDI_MODEM_STATUS_FS                      DEF_BIT_00
#define  USBH_FTDI_MODEM_STATUS_HS                      DEF_BIT_01
#define  USBH_FTDI_MODEM_STATUS_CLEAR_TO_SEND           DEF_BIT_04
#define  USBH_FTDI_MODEM_STATUS_DATA_SET_READY          DEF_BIT_05
#define  USBH_FTDI_MODEM_STATUS_RING_INDICATOR          DEF_BIT_06
#define  USBH_FTDI_MODEM_STATUS_DATA_CARRIER_DETECT     DEF_BIT_07

#define  USBH_FTDI_LINE_STATUS_RX_OVERFLOW_ERROR        DEF_BIT_01
#define  USBH_FTDI_LINE_STATUS_PARITY_ERROR             DEF_BIT_02
#define  USBH_FTDI_LINE_STATUS_FRAMING_ERROR            DEF_BIT_03
#define  USBH_FTDI_LINE_STATUS_BREAK_INTERRUPT          DEF_BIT_04
#define  USBH_FTDI_LINE_STATUS_TX_REGISTER_EMPTY        DEF_BIT_05
#define  USBH_FTDI_LINE_STATUS_TX_EMPTY                 DEF_BIT_06


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

typedef  CPU_INT08U  USBH_FTDI_HANDLE;


/*
*********************************************************************************************************
*                                       USBH_FTDI_SERIAL_STATUS
*
* Note(s) : (1) The serial status is defined as follow:
*
*      OFFSET                 MODEM STATUS                                        MASK
*      --------------------------------------------------------------------------------------------------
*      Bit 0       Full-speed 64 bytes max packet              USBH_FTDI_MODEM_STATUS_FS
*      Bit 1       High-speed 512 bytes max packet             USBH_FTDI_MODEM_STATUS_HS
*      Bit 4       Clear to Send (CTS)                         USBH_FTDI_MODEM_STATUS_CLEAR_TO_SEND
*      Bit 5       Data Set Ready (DSR)                        USBH_FTDI_MODEM_STATUS_DATA_SET_READY
*      Bit 6       Ring Indicator (RI)                         USBH_FTDI_MODEM_STATUS_RING_INDICATOR
*      Bit 7       Receive Line Signal Detect (RLSD)           USBH_FTDI_MODEM_STATUS_DATA_CARRIER_DETECT
*      --------------------------------------------------------------------------------------------------
*
*      OFFSET                  LINE STATUS                                        MASK
*      --------------------------------------------------------------------------------------------------
*      Bit 1       Overrun Error (OE)                          USBH_FTDI_LINE_STATUS_RX_OVERFLOW_ERROR
*      Bit 2       Parity Error (PE)                           USBH_FTDI_LINE_STATUS_PARITY_ERROR
*      Bit 3       Framing Error (FE)                          USBH_FTDI_LINE_STATUS_FRAMING_ERROR
*      Bit 4       Break Interrupt (BI)                        USBH_FTDI_LINE_STATUS_BREAK_INTERRUPT
*      Bit 5       Transmitter Holding Register (THRE)         USBH_FTDI_LINE_STATUS_TX_REGISTER_EMPTY
*      Bit 6       Transmitter Empty (TEMT)                    USBH_FTDI_LINE_STATUS_TX_EMPTY
*      --------------------------------------------------------------------------------------------------
*
*********************************************************************************************************
*/

typedef  struct  usbh_ftdi_serial_status {
    CPU_INT08U  ModemStatus;
    CPU_INT08U  LineStatus;
} USBH_FTDI_SERIAL_STATUS;


/*
*********************************************************************************************************
*                                         USBH_FTDI_RXCB_FNCT
*
* Note(s) : (1) Application RX function callback.
*
*           (2) The arguments are defined as follow:
*
*               ftdi_handle         Handle on FTDI device.
*               p_arg               Pointer to argument.
*               p_buf               Pointer to received data buffer.
*               xfer_len            Number of octets received.
*               p_serial_status     Pointer to USBH_FTDI_SERIAL_STATUS structure that will be filled.
*               err                 Variable that will receive the return error code.
*
*********************************************************************************************************
*/

typedef  void  (*USBH_FTDI_ASYNC_RX_FNCT) (USBH_FTDI_HANDLE          ftdi_handle,
                                           void                     *p_arg,
                                           void                     *p_buf,
                                           CPU_INT32U                xfer_len,
                                           USBH_FTDI_SERIAL_STATUS  *p_serial_status,
                                           USBH_ERR                  err);


/*
*********************************************************************************************************
*                                         USBH_FTDI_TXCB_FNCT
*
* Note(s) : (1) Application TX function callback.
*
*           (2) The arguments are defined as follow:
*
*               ftdi_handle         Handle on FTDI device.
*               p_arg               Pointer to argument.
*               p_buf               Pointer to transmitted data buffer.
*               xfer_len            Number of octets transmitted.
*               err                 Variable that will receive the return error code.
*
*********************************************************************************************************
*/

typedef  void  (*USBH_FTDI_ASYNC_TX_FNCT) (USBH_FTDI_HANDLE   ftdi_handle,
                                           void              *p_arg,
                                           void              *p_buf,
                                           CPU_INT32U         xfer_len,
                                           USBH_ERR           err);


/*
*********************************************************************************************************
*                                           USBH_FTDI_CALLBACKS
*
* Note(s) : (1) Callback function pointers structure.
*
*********************************************************************************************************
*/

typedef  const  struct  usbh_ftdi_callbacks {
    void  (*ConnNotifyPtr)    (USBH_FTDI_HANDLE  ftdi_handle);  /* Ptr to conn    fnct callback.                        */
    void  (*DisconnNotifyPtr) (USBH_FTDI_HANDLE  ftdi_handle);  /* Ptr to disconn fnct callback.                        */
} USBH_FTDI_CALLBACKS;


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACROS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void        USBH_FTDI_Init          (USBH_FTDI_CALLBACKS      *p_ftdi_fnct,
                                     USBH_ERR                 *p_err);

void        USBH_FTDI_Reset         (USBH_FTDI_HANDLE          ftdi_handle,
                                     CPU_INT16U                reset_ctrl,
                                     USBH_ERR                 *p_err);

void        USBH_FTDI_ModemCtrlSet  (USBH_FTDI_HANDLE          ftdi_handle,
                                     CPU_INT16U                modem_ctrl,
                                     USBH_ERR                 *p_err);

void        USBH_FTDI_FlowCtrlSet   (USBH_FTDI_HANDLE          ftdi_handle,
                                     CPU_INT08U                protocol,
                                     CPU_INT08U                xon_char,
                                     CPU_INT08U                xoff_char,
                                     USBH_ERR                 *p_err);

void        USBH_FTDI_BaudRateSet   (USBH_FTDI_HANDLE          ftdi_handle,
                                     CPU_INT16U                baud_rate,
                                     USBH_ERR                 *p_err);

void        USBH_FTDI_DataSet       (USBH_FTDI_HANDLE          ftdi_handle,
                                     CPU_INT08U                data_size,
                                     CPU_INT08U                parity,
                                     CPU_INT08U                stop_bits,
                                     CPU_INT08U                break_bit,
                                     USBH_ERR                 *p_err);

void        USBH_FTDI_ModemStatusGet(USBH_FTDI_HANDLE          ftdi_handle,
                                     USBH_FTDI_SERIAL_STATUS  *p_serial_status,
                                     USBH_ERR                 *p_err);

CPU_INT32U  USBH_FTDI_Tx            (USBH_FTDI_HANDLE          ftdi_handle,
                                     void                     *p_buf,
                                     CPU_INT32U                buf_len,
                                     CPU_INT16U                timeout,
                                     USBH_ERR                 *p_err);

void        USBH_FTDI_TxAsync       (USBH_FTDI_HANDLE          ftdi_handle,
                                     void                     *p_buf,
                                     CPU_INT32U                buf_len,
                                     USBH_FTDI_ASYNC_TX_FNCT   tx_cmpl_notify,
                                     void                     *p_arg,
                                     USBH_ERR                 *p_err);

CPU_INT32U  USBH_FTDI_Rx            (USBH_FTDI_HANDLE          ftdi_handle,
                                     void                     *p_buf,
                                     CPU_INT32U                buf_len,
                                     CPU_INT16U                timeout,
                                     USBH_FTDI_SERIAL_STATUS  *p_serial_status,
                                     USBH_ERR                 *p_err);

void        USBH_FTDI_RxAsync       (USBH_FTDI_HANDLE          ftdi_handle,
                                     void                     *p_buf,
                                     CPU_INT32U                buf_len,
                                     USBH_FTDI_SERIAL_STATUS  *p_serial_status,
                                     USBH_FTDI_ASYNC_RX_FNCT   rx_cmpl_notify,
                                     void                     *p_arg,
                                     USBH_ERR                 *p_err);

USBH_DEV   *USBH_FTDI_DevGet        (USBH_FTDI_HANDLE          ftdi_handle,
                                     USBH_ERR                 *p_err);


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif
