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
*                                    MICRIUM BOARD SUPPORT PACKAGE
*                                             AXI UARTLITE
*
* File : ucos_axiuartlite.h
*********************************************************************************************************
*/

#ifndef  UCOS_AXIUARTLITE_PRESENT
#define  UCOS_AXIUARTLITE_PRESENT


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <xil_types.h>
#include <lib_def.h>
#include <cpu.h>
#include <KAL/kal.h>


/*
*********************************************************************************************************
*                                        REGISTER DEFINITIONS
*********************************************************************************************************
*/

typedef struct axi_uartlite {
    CPU_REG32  RX;                                             /* RX FIFO                                               */
    CPU_REG32  TX;                                             /* TX FIFO                                               */
    CPU_REG32  STAT;                                           /* Status Register                                       */
    CPU_REG32  CTRL;                                           /* Control Register                                      */

} AXIUARTLITE, *AXIUARTLITE_PTR;


/* ------- UART CONTROL REGISTER BIT DEFINITION ------- */
#define  AXIUARTLITE_BIT_CTRL_TXRST  (DEF_BIT_00)              /* Reset TX FIFO                                         */
#define  AXIUARTLITE_BIT_CTRL_RXRST  (DEF_BIT_01)              /* Reset RX FIFO                                         */
#define  AXIUARTLITE_BIT_CTRL_INTEN  (DEF_BIT_04)              /* Enable Interrupts                                     */


/* ------- UART STATUS REGISTER BIT DEFINITION ------- */
#define  AXIUARTLITE_BIT_STAT_RXVALID   (DEF_BIT_00)           /* RX Valid                                              */
#define  AXIUARTLITE_BIT_STAT_RXFULL    (DEF_BIT_01)           /* RX FIFO Full                                          */
#define  AXIUARTLITE_BIT_STAT_TXEMPTY   (DEF_BIT_02)           /* TX FIFO Empty                                         */
#define  AXIUARTLITE_BIT_STAT_TXFULL    (DEF_BIT_03)           /* TX FIFO Full                                          */
#define  AXIUARTLITE_BIT_STAT_INTREN    (DEF_BIT_04)           /* Interrupts Enabled                                    */
#define  AXIUARTLITE_BIT_STAT_OVRUN     (DEF_BIT_05)           /* RX Overrun error                                      */
#define  AXIUARTLITE_BIT_STAT_FRAMEERR  (DEF_BIT_06)           /* RX Frame Error                                        */
#define  AXIUARTLITE_BIT_STAT_PARITYERR (DEF_BIT_07)           /* RX Parity Error                                       */

/*
*********************************************************************************************************
*                                        STRUCTURE DEFINITIONS
*********************************************************************************************************
*/


typedef struct {
    CPU_INT32U  DeviceId;
    CPU_INT32U  BaseAddress;
    CPU_INT32U  BaudRate;
    CPU_BOOLEAN UseParity;
    CPU_BOOLEAN OddParity;
    CPU_INT32U  DataBits;
    CPU_INT32U  IntSource;
} UCOS_AXIUARTLITE_Config;


typedef struct {
    CPU_INT32U      DeviceId;
    AXIUARTLITE_PTR UARTLite;
    CPU_INT32U      BaudRate;
    CPU_BOOLEAN     UseParity;
    CPU_BOOLEAN     OddParity;
    CPU_INT32U      DataBits;
    CPU_INT32U      IntReq;
    KAL_SEM_HANDLE  TxSem;
    KAL_SEM_HANDLE  RxSem;
    KAL_LOCK_HANDLE TxMutex;
    KAL_LOCK_HANDLE RxMutex;
    CPU_INT32U  IntSource;
} UCOS_AXIUARTLITE;

#define  AXIUARTLITE_INT_RX_REQ DEF_BIT_00
#define  AXIUARTLITE_INT_TX_REQ DEF_BIT_01

typedef UCOS_AXIUARTLITE *AXIUARTLITE_HANDLE;


/*
*********************************************************************************************************
*                                   STANDALONE COMPATIBILITY LAYER
*********************************************************************************************************
*/
void AXIUARTLite_SendByte(u32 BaseAddress, u8 Data);

u8 AXIUARTLite_RecvByte(u32 BaseAddress);

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

AXIUARTLITE_HANDLE AXIUARTLite_Init (CPU_INT32U device_id);

CPU_BOOLEAN AXIUARTLite_WrStr (AXIUARTLITE_HANDLE   handle,
                               const CPU_CHAR *p_str);

CPU_BOOLEAN AXIUARTLite_WrByte (AXIUARTLITE_HANDLE handle,
                                CPU_CHAR           byte);

CPU_CHAR AXIUARTLite_RdByte (AXIUARTLITE_HANDLE handle);

void AXIUARTLite_IntHandler (void  *p_int_arg);

#endif /* UCOS_AXIUARTLITE_PRESENT */
