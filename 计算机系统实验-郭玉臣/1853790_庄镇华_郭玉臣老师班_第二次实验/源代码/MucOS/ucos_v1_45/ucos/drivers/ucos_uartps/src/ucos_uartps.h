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
*                                             CADENCE UART
*
* File : ucos_uartps.h
*********************************************************************************************************
*/

#ifndef  UCOS_UARTPS_PRESENT
#define  UCOS_UARTPS_PRESENT


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

typedef struct uartps {
    CPU_REG32  CTRL;                                            /* UART Control Register                                */
    CPU_REG32  MODE;                                            /* UART Mode Register                                   */
    CPU_REG32  INTR_EN;                                         /* Interrupt Enable Register                            */
    CPU_REG32  INTR_DIS;                                        /* Interrupt Disable Register                           */
    CPU_REG32  INTR_MASK;                                       /* Interrupt Mask Register                              */
    CPU_REG32  INTR_STAT;                                       /* Channel Interrupt Status Register                    */
    CPU_REG32  BAUD;                                            /* Baud Rate Generator Register                         */
    CPU_REG32  RX_TIMEOUT;                                      /* Receiver Timeout Register                            */
    CPU_REG32  RX_FIFO_LVL;                                     /* Receiver FIFO Trigger Level Register                 */
    CPU_REG32  MODEM_CTRL;                                      /* Modem Control Register                               */
    CPU_REG32  MODEM_STAT;                                      /* Modem Status Register                                */
    CPU_REG32  CHAN_STAT;                                       /* Channel Status Register                              */
    CPU_REG32  TX_RX_FIFO;                                      /* Transmit an Receive FIFO                             */
    CPU_REG32  BAUD_DIV;                                        /* Baud Rate Divider Register                           */
    CPU_REG32  FLOW_DLY;                                        /* Flow Control Delay Register                          */
    CPU_REG32  TX_FIFO_LVL;                                     /* Transmit FIFO Trigger Level Register                 */
} UARTPS, *UARTPS_PTR;


/* ------- UART CONTROL REGISTER BIT DEFINITION ------- */
#define  UARTPS_BIT_UART_CTRL_STPBRK  (DEF_BIT_08)                /* Stop transmitter break.                              */
#define  UARTPS_BIT_UART_CTRL_STTBRK  (DEF_BIT_07)                /* Start transmitter break.                             */
#define  UARTPS_BIT_UART_CTRL_RSTTO   (DEF_BIT_06)                /* Restart receiver timeout counter.                    */
#define  UARTPS_BIT_UART_CTRL_TXDIS   (DEF_BIT_05)                /* Transmit disable.                                    */
#define  UARTPS_BIT_UART_CTRL_TXEN    (DEF_BIT_04)                /* Transmit enable.                                     */
#define  UARTPS_BIT_UART_CTRL_RXDIS   (DEF_BIT_03)                /* Receive disable.                                     */
#define  UARTPS_BIT_UART_CTRL_RXEN    (DEF_BIT_02)                /* Receive enable.                                      */
#define  UARTPS_BIT_UART_CTRL_TXRES   (DEF_BIT_01)                /* TX software reset.                                   */
#define  UARTPS_BIT_UART_CTRL_RXRES   (DEF_BIT_00)                /* RX software reset.                                   */


/* -------- UART MODE REGISTER BIT DEFINITION --------- */
#define  UARTPS_BIT_UART_MODE_CHMODE_MSK  (DEF_BIT_FIELD(8, 2))   /* Channel mode.                                        */
#define  UARTPS_BIT_UART_MODE_CHMODE(mode) (DEF_BIT_MASK(mode, 2) & UARTPS_BIT_UART_MODE_CHMODE_MSK)
#define  UARTPS_BIT_UART_MODE_NBSTOP_MSK  (DEF_BIT_FIELD(6, 2))   /* Number of stop bits.                                 */
#define  UARTPS_BIT_UART_MODE_NBSTOP(nbr) (DEF_BIT_MASK(nbr, 2) & UARTPS_BIT_UART_MODE_NBSTOP_MSK)
#define  UARTPS_BIT_UART_MODE_PAR_MSK     (DEF_BIT_FIELD(3, 3))   /* Parity type select.                                  */
#define  UARTPS_BIT_UART_MODE_PAR(par)    (DEF_BIT_MASK(par, 3) & UARTPS_BIT_UART_MODE_PAR_MSK)
#define  UARTPS_BIT_UART_MODE_CHRL_MSK    (DEF_BIT_FIELD(1, 2))   /* Character length.                                    */
#define  UARTPS_BIT_UART_MODE_CHRL(len)   (DEF_BIT_MASK(len, 2) & UARTPS_BIT_UART_MODE_CHRL_MSK)
#define  UARTPS_BIT_UART_MODE_CLKS        (DEF_BIT_00)            /* Clock source select.                                 */


/* ------ CHANNEL STATUS REGISTER BIT DEFINITION ------ */
#define  UARTPS_BIT_UART_CSTAT_TNFUL       (DEF_BIT_14)           /* Transmit FIFO nearly full.                           */
#define  UARTPS_BIT_UART_CSTAT_TTRIG       (DEF_BIT_13)           /* Transmit FIFO continuous status.                     */
#define  UARTPS_BIT_UART_CSTAT_FDELT       (DEF_BIT_12)           /* Receive FIFO delay trigger continuous status.        */
#define  UARTPS_BIT_UART_CSTAT_TACTIVE     (DEF_BIT_11)           /* Transmit state machine active status.                */
#define  UARTPS_BIT_UART_CSTAT_RACTIVE     (DEF_BIT_10)           /* Receiver state machine active status.                */
#define  UARTPS_BIT_UART_CSTAT_TFUL        (DEF_BIT_04)           /* Transmit FIFO full continuous status.                */
#define  UARTPS_BIT_UART_CSTAT_TEMPTY      (DEF_BIT_03)           /* Transmit FIFO empty continuous status.               */
#define  UARTPS_BIT_UART_CSTAT_RFUL        (DEF_BIT_02)           /* Receive FIFO full continuous status.                 */
#define  UARTPS_BIT_UART_CSTAT_REMPTY      (DEF_BIT_01)           /* Receive FIFO empty continuous status.                */
#define  UARTPS_BIT_UART_CSTAT_RTRIG       (DEF_BIT_00)           /* Receive FIFO trigger continuous status.              */


/* --------- INTERRUPT SOURCE BIT DEFINITION ---------- */
#define  UARTPS_BIT_UART_INTR_TOVR         (DEF_BIT_12)           /* Transmitter FIFO overflow interrupt.                 */
#define  UARTPS_BIT_UART_INTR_TNFUL        (DEF_BIT_11)           /* Transmitter FIFO nearly full interrupt.              */
#define  UARTPS_BIT_UART_INTR_TTRIG        (DEF_BIT_10)           /* Transmitter FIFO trigger interrupt.                  */
#define  UARTPS_BIT_UART_INTR_DMSI         (DEF_BIT_09)           /* Delta MODEM status indicator interrupt.              */
#define  UARTPS_BIT_UART_INTR_TIMEOUT      (DEF_BIT_08)           /* Receiver timeout error interrupt.                    */
#define  UARTPS_BIT_UART_INTR_PARE         (DEF_BIT_07)           /* Receiver parity error interrupt.                     */
#define  UARTPS_BIT_UART_INTR_FRAME        (DEF_BIT_06)           /* Receiver framing error interrupt.                    */
#define  UARTPS_BIT_UART_INTR_ROVR         (DEF_BIT_05)           /* Receiver overflow error interrupt.                   */
#define  UARTPS_BIT_UART_INTR_TFUL         (DEF_BIT_04)           /* Transmitter FIFO full interrupt.                     */
#define  UARTPS_BIT_UART_INTR_TEMPTY       (DEF_BIT_03)           /* Transmitter FIFO empty interrupt.                    */
#define  UARTPS_BIT_UART_INTR_RFUL         (DEF_BIT_02)           /* Receiver FIFO full interrupt.                        */
#define  UARTPS_BIT_UART_INTR_REMPTY       (DEF_BIT_01)           /* Receiver FIFO empty interrupt.                       */
#define  UARTPS_BIT_UART_INTR_RTRIG        (DEF_BIT_00)           /* Receiver FIFO trigger interrupt.                     */


/* --------- DATA FORMAT OPTIONS ---------- */
#define  UARTPS_FORMAT_8_BITS  0u
#define  UARTPS_FORMAT_7_BITS  2u
#define  UARTPS_FORMAT_6_BITS  3u

#define  UARTPS_FORMAT_NO_PARITY     4u
#define  UARTPS_FORMAT_MARK_PARITY   3u
#define  UARTPS_FORMAT_SPACE_PARITY  2u
#define  UARTPS_FORMAT_ODD_PARITY    1u
#define  UARTPS_FORMAT_EVEN_PARITY   0u

#define  UARTPS_FORMAT_2_STOP_BIT    2u
#define  UARTPS_FORMAT_1_5_STOP_BIT  1u
#define  UARTPS_FORMAT_1_STOP_BIT    0u


/* --------- OPERATING MODE OPTIONS --------- */
#define UARTPS_OPER_MODE_NORMAL       0u
#define UARTPS_OPER_MODE_AUTO_ECHO    1u
#define UARTPS_OPER_MODE_LOCAL_LOOP   2u
#define UARTPS_OPER_MODE_REMOTE_LOOP  3u


/*
*********************************************************************************************************
*                                        STRUCTURE DEFINITIONS
*********************************************************************************************************
*/


typedef struct {
    CPU_INT32U  DeviceId;
    CPU_INT32U  BaseAddress;
    CPU_INT32U  ClkFreq;
    CPU_BOOLEAN HasModem;
    CPU_INT32U  IntSource;
} UCOS_UARTPS_Config;

typedef struct {
    CPU_INT32U      DeviceId;
    UARTPS_PTR      UARTPS;
    CPU_INT32U      ClkFreq;
    CPU_BOOLEAN     HasModem;
    KAL_SEM_HANDLE  TxSem;
    KAL_SEM_HANDLE  RxSem;
    KAL_LOCK_HANDLE TxMutex;
    KAL_LOCK_HANDLE RxMutex;
    CPU_INT32U      IntSource;
} UCOS_UARTPS;

typedef UCOS_UARTPS *UARTPS_HANDLE;


/*
*********************************************************************************************************
*                                   STANDALONE COMPATIBILITY LAYER
*********************************************************************************************************
*/
void UARTPS_SendByte(u32 BaseAddress, u8 Data);

u8 UARTPS_RecvByte(u32 BaseAddress);

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

UARTPS_HANDLE UARTPS_Init (CPU_INT32U device_id);

CPU_BOOLEAN UARTPS_Enable (UARTPS_HANDLE handle);

CPU_BOOLEAN UARTPS_Disable (UARTPS_HANDLE handle);

CPU_BOOLEAN UARTPS_DataFormatSet (UARTPS_HANDLE handle,
                                  CPU_INT32U    baud_rate,
                                  CPU_INT32U    data_bits,
                                  CPU_INT32U    parity,
                                  CPU_INT32U    stop_bits);

CPU_BOOLEAN UARTPS_OperModeSet (UARTPS_HANDLE handle,
                                CPU_INT32U    oper_mode);

CPU_BOOLEAN UARTPS_BaudRateSet (UARTPS_HANDLE handle,
                                CPU_INT32U    baud_rate);

CPU_BOOLEAN UARTPS_RTOSet (UARTPS_HANDLE handle,
                           CPU_INT08U    rto);

CPU_BOOLEAN UARTPS_WrStr (UARTPS_HANDLE   handle,
                          const CPU_CHAR *p_str);

CPU_BOOLEAN  UARTPS_WrByte (UARTPS_HANDLE   handle,
                            CPU_CHAR        byte);

CPU_CHAR  UARTPS_RdByte (UARTPS_HANDLE handle);

void UARTPS_IntHandler (void  *p_int_arg);


#endif /* UCOS_UARTPS_PRESENT */
