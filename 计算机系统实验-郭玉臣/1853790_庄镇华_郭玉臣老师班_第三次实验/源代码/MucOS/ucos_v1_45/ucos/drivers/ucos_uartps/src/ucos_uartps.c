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
* File : ucos_uartps.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <xparameters.h>
#include <ucos_bsp.h>
#include <ucos_uartps.h>
#include <ucos_bsp.h>
#include <ucos_int.h>

#include <KAL/kal.h>


extern UCOS_UARTPS_Config UCOS_UARTPS_ConfigTable[];

static UCOS_UARTPS UCOS_UARTPS_Table[XPAR_UCOS_UARTPS_NUM_INSTANCES];



/*
*********************************************************************************************************
*                                   STANDALONE COMPATIBILITY LAYER
*********************************************************************************************************
*/

void UARTPS_SendByte(u32 BaseAddress, u8 Data)
{
    UARTPS_HANDLE handle;
    CPU_INT32U device_id;

    for (device_id = 0; device_id < XPAR_UCOS_UARTPS_NUM_INSTANCES; device_id++) {
        if (UCOS_UARTPS_ConfigTable[device_id].BaseAddress == BaseAddress)
        {
            handle = (UARTPS_HANDLE)&UCOS_UARTPS_Table[device_id];
            UARTPS_WrByte (handle, Data);
        }
    }
}

u8 UARTPS_RecvByte(u32 BaseAddress)
{
    UARTPS_HANDLE handle;
    CPU_INT32U device_id;
    CPU_CHAR c;

    c = '\0';
    for (device_id = 0; device_id < XPAR_UCOS_UARTPS_NUM_INSTANCES; device_id++) {
        if (UCOS_UARTPS_ConfigTable[device_id].BaseAddress == BaseAddress)
        {
            handle = (UARTPS_HANDLE)&UCOS_UARTPS_Table[device_id];
            c = UARTPS_RdByte (handle);
        }
    }

    return c;
}


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            UARTPS_Init()
*
* Description : Initialize a UART.
*
* Argument(s) : device_id  ID of the UART to init.
*
* Return(s)   : Handle to the peripheral instance or DEF_NULL if
*               the devide id is invalid.
*
* Note(s)     : Should be called before accessing any other functionalities of this peripheral.
*
*********************************************************************************************************
*/

UARTPS_HANDLE UARTPS_Init (CPU_INT32U device_id)
{
    UARTPS_HANDLE handle;
    UARTPS_PTR    uart_ps;
    KAL_ERR       kal_err;
    CPU_INT32U    int_source;
    CPU_SR_ALLOC();


    if (device_id > XPAR_UCOS_UARTPS_NUM_INSTANCES) {
        return (DEF_NULL);
    }

    handle = (UARTPS_HANDLE)&UCOS_UARTPS_Table[device_id];

    handle->DeviceId = UCOS_UARTPS_ConfigTable[device_id].DeviceId;
    uart_ps = (UARTPS_PTR)UCOS_UARTPS_ConfigTable[device_id].BaseAddress;
    handle->UARTPS = uart_ps;
    handle->ClkFreq = UCOS_UARTPS_ConfigTable[device_id].ClkFreq;
    handle->HasModem = UCOS_UARTPS_ConfigTable[device_id].HasModem;


    handle->TxSem = KAL_SemCreate("UARTPS Tx Semaphore", DEF_NULL, &kal_err);
    if (kal_err != KAL_ERR_NONE) {
        return (DEF_NULL);
    }

    handle->RxSem = KAL_SemCreate("UARTPS Rx Semaphore", DEF_NULL, &kal_err);
    if (kal_err != KAL_ERR_NONE) {
        return (DEF_NULL);
    }

    handle->TxMutex = KAL_LockCreate("UARTPS Tx Mutex", DEF_NULL, &kal_err);
    if (kal_err != KAL_ERR_NONE) {
        return (DEF_NULL);
    }

    handle->RxMutex = KAL_LockCreate("UARTPS Rx Mutex", DEF_NULL, &kal_err);
    if (kal_err != KAL_ERR_NONE) {
        return (DEF_NULL);
    }

    int_source = UCOS_UARTPS_ConfigTable[device_id].IntSource;
    handle->IntSource = int_source;

    if (int_source != UCOS_INT_SOURCE_NONE) {
        UCOS_IntVectSet(int_source,
                        0u,
                        0u,
                        UARTPS_IntHandler,
                        (void *)handle);

        UCOS_IntSrcEn(int_source);
    }


    CPU_CRITICAL_ENTER();

    UARTPS_Disable(handle);

    uart_ps->MODE = UARTPS_BIT_UART_MODE_CHMODE(0u) |
                    UARTPS_BIT_UART_MODE_NBSTOP(0u) |
                    UARTPS_BIT_UART_MODE_PAR(4u)    |
                    UARTPS_BIT_UART_MODE_CHRL(0u);

    UARTPS_DataFormatSet(handle, 115200, UARTPS_FORMAT_8_BITS, UARTPS_FORMAT_NO_PARITY, UARTPS_FORMAT_1_STOP_BIT);

    UARTPS_RTOSet(handle, 0U);

    uart_ps->RX_FIFO_LVL = 1u;

    UARTPS_Enable(handle);

    CPU_CRITICAL_EXIT();

    return (handle);
}


/*
*********************************************************************************************************
*                                            UARTPS_Enable()
*
* Description : Enable a UART instance.
*
* Argument(s) : handle  Handle of the UART to enable.
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN UARTPS_Enable (UARTPS_HANDLE handle)
{
    UARTPS_PTR uart_ps;
    CPU_SR_ALLOC();


    uart_ps = handle->UARTPS;

    CPU_CRITICAL_ENTER();
    uart_ps->CTRL =  (uart_ps->CTRL | UARTPS_BIT_UART_CTRL_TXEN | UARTPS_BIT_UART_CTRL_RXEN)
                     & ~(UARTPS_BIT_UART_CTRL_TXDIS | UARTPS_BIT_UART_CTRL_RXDIS);
    CPU_CRITICAL_EXIT();

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                            UARTPS_Disable()
*
* Description : Disable a UART instance.
*
* Argument(s) : handle  Handle of the UART to disable.
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN UARTPS_Disable (UARTPS_HANDLE handle)
{
    UARTPS_PTR uart_ps;
    CPU_SR_ALLOC();


    uart_ps = handle->UARTPS;

    CPU_CRITICAL_ENTER();
    uart_ps->CTRL =  (uart_ps->CTRL | UARTPS_BIT_UART_CTRL_RXDIS | UARTPS_BIT_UART_CTRL_TXDIS)
                     & ~(UARTPS_BIT_UART_CTRL_TXEN | UARTPS_BIT_UART_CTRL_RXEN);
    CPU_CRITICAL_EXIT();

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                            UARTPS_RTOSet()
*
* Description : Configure the receive timeout of a UART.
*
* Argument(s) : handle  Handle of the UART to configure.
*               rto     Receive timeout.
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN UARTPS_RTOSet (UARTPS_HANDLE handle,
                           CPU_INT08U    rto)
{
    UARTPS_PTR uart_ps;


    uart_ps = handle->UARTPS;

    uart_ps->RX_TIMEOUT = rto;

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                          UARTPS_DataFormatSet()
*
* Description : Configure the data format of a UART.
*
* Argument(s) : handle     Handle of the UART to configure.
*
*               baud_rate  Baud rate.
*
*               data_bits  Data bits. Possible options are :
*                   UARTPS_FORMAT_8_BITS
*                   UARTPS_FORMAT_7_BITS
*                   UARTPS_FORMAT_6_BITS
*
*               parity     Parity. Possible options are :
*                   UARTPS_FORMAT_NO_PARITY
*                   UARTPS_FORMAT_MARK_PARITY
*                   UARTPS_FORMAT_SPACE_PARITY
*                   UARTPS_FORMAT_ODD_PARITY
*                   UARTPS_FORMAT_EVEN_PARITY
*
*               stop_bits  Stop Bits. Possible options are :
*                   UARTPS_FORMAT_2_STOP_BIT
*                   UARTPS_FORMAT_1_5_STOP_BIT
*                   UARTPS_FORMAT_1_STOP_BIT
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN UARTPS_DataFormatSet (UARTPS_HANDLE handle,
                                  CPU_INT32U    baud_rate,
                                  CPU_INT32U    data_bits,
                                  CPU_INT32U    parity,
                                  CPU_INT32U    stop_bits)
{
    CPU_INT32U mode_reg;
    UARTPS_PTR uart_ps;


    uart_ps = handle->UARTPS;

    if ((data_bits != UARTPS_FORMAT_6_BITS) &&
        (data_bits != UARTPS_FORMAT_7_BITS  &&
        (data_bits != UARTPS_FORMAT_8_BITS))) {
        return (DEF_FAIL);
    }

    if (parity > UARTPS_FORMAT_NO_PARITY) {
        return (DEF_FAIL);
    }

    if (stop_bits > UARTPS_FORMAT_2_STOP_BIT) {
        return (DEF_FAIL);
    }

    if (UARTPS_BaudRateSet(handle, baud_rate) != DEF_OK) {
        return (DEF_FAIL);
    }

    mode_reg = uart_ps->MODE;
    mode_reg &= ~(UARTPS_BIT_UART_MODE_CHRL_MSK | UARTPS_BIT_UART_MODE_NBSTOP_MSK | UARTPS_BIT_UART_MODE_PAR_MSK);
    mode_reg |= (UARTPS_BIT_UART_MODE_CHRL(data_bits) | UARTPS_BIT_UART_MODE_PAR(parity) | UARTPS_BIT_UART_MODE_NBSTOP(stop_bits));
    uart_ps->MODE = mode_reg;

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                            UARTPS_OperModeSet()
*
* Description : Set the operation mode a uart.
*
* Argument(s) : handle    Handle of the UART to configure.
*
*               oper_mode Operation mode. Possible options are :
*                   UARTPS_OPER_MODE_NORMAL
*                   UARTPS_OPER_MODE_AUTO_ECHO
*                   UARTPS_OPER_MODE_LOCAL_LOOP
*                   UARTPS_OPER_MODE_REMOTE_LOOP
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN UARTPS_OperModeSet (UARTPS_HANDLE handle,
                                CPU_INT32U    oper_mode)
{
    CPU_INT32U mode_reg;
    UARTPS_PTR uart_ps;


    uart_ps = handle->UARTPS;

    if (oper_mode > UARTPS_OPER_MODE_REMOTE_LOOP) {
        return (DEF_FAIL);
    }

    mode_reg = uart_ps->MODE;

    mode_reg &= ~UARTPS_BIT_UART_MODE_CHMODE_MSK;
    mode_reg |=  UARTPS_BIT_UART_MODE_CHMODE(oper_mode);

    uart_ps->MODE = mode_reg;

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                            UARTPS_BaudRateSet()
*
* Description : Set the baud rate of a uart.
*
* Argument(s) : handle    Handle of the UART to configure.
*
*               baud_rate Baud rate.
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN UARTPS_BaudRateSet (UARTPS_HANDLE handle,
                                CPU_INT32U    baud_rate)
{
    UARTPS_PTR uart_ps;
    CPU_INT32U clk;
    CPU_INT32U actual_rate;
    CPU_INT32U baud_div;
    CPU_INT32U baud_rate_gen;
    CPU_INT32U best_baud_div;
    CPU_INT32U best_baud_rate_gen;
    CPU_INT32U baud_error;
    CPU_INT32U best_error = 0xFFFFFFFFu;


    uart_ps = handle->UARTPS;

    if ((baud_rate * 2) > handle->ClkFreq) {
        return (DEF_FAIL);
    }

    if ((uart_ps->MODE & UARTPS_BIT_UART_MODE_CLKS) != 0) {
        clk = handle->ClkFreq / 8;
    } else {
        clk = handle->ClkFreq;
    }

    for (baud_div = 4; baud_div < 255; baud_div++) {
        baud_rate_gen = clk / (baud_rate * (baud_div + 1));
        actual_rate = clk/ (baud_rate_gen * (baud_div + 1));
        if (baud_rate > actual_rate) {
            baud_error = baud_rate - actual_rate;
        }
        else {
            baud_error = actual_rate - baud_rate;
        }

        if (best_error > baud_error) {
            best_error = baud_error;
            best_baud_div = baud_div;
            best_baud_rate_gen = baud_rate_gen;
        }
    }

    uart_ps->BAUD = best_baud_rate_gen;
    uart_ps->BAUD_DIV = best_baud_div;

    uart_ps->CTRL = UARTPS_BIT_UART_CTRL_TXRES | UARTPS_BIT_UART_CTRL_RXRES;


    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                             UARTPS_WrStr()
*
* Description : Write a null terminated string through the UART.
*
* Argument(s) : handle    Handle of the UART to configure.
*
*               p_str     String to output.
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : Transmission is always blocking, interrupt based and protected by an internal TX mutex.
*               UARTPS_WrStr will return once the entire string has been written to the device FIFO.
*
*********************************************************************************************************
*/

CPU_BOOLEAN  UARTPS_WrStr (UARTPS_HANDLE   handle,
                           const CPU_CHAR *p_str)
{
    UARTPS_PTR uart_ps;
    KAL_ERR    kal_err;


    uart_ps = handle->UARTPS;

    KAL_LockAcquire(handle->TxMutex, 0u, 0u, &kal_err);

    while ((*p_str) != DEF_NULL) {
        if ((uart_ps->CHAN_STAT & UARTPS_BIT_UART_CSTAT_TNFUL) != 0u) {
            if (handle->IntSource != UCOS_INT_SOURCE_NONE) {
                uart_ps->INTR_EN = UARTPS_BIT_UART_INTR_TEMPTY;
                KAL_SemPend(handle->TxSem, 0u, 0u, &kal_err);
            } else {
                while ((uart_ps->CHAN_STAT & UARTPS_BIT_UART_CSTAT_TNFUL) != 0) {}
            }
        }
        uart_ps->TX_RX_FIFO = *p_str;
        p_str++;
    }

    KAL_LockRelease(handle->TxMutex, &kal_err);

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                             UARTPS_WrByte()
*
* Description : Write a single byte through the UART.
*
* Argument(s) : handle    Handle of the UART to configure.
*
*               p_str     String to output.
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : Transmission is always blocking, interrupt based and protected by an internal TX mutex.
*               UARTPS_WrByte will return once the byte has been written to the device FIFO.
*
*********************************************************************************************************
*/

CPU_BOOLEAN  UARTPS_WrByte (UARTPS_HANDLE   handle,
                            CPU_CHAR        byte)
{
    UARTPS_PTR uart_ps;
    KAL_ERR    kal_err;


    uart_ps = handle->UARTPS;

    KAL_LockAcquire(handle->TxMutex, 0u, 0u, &kal_err);

    if ((uart_ps->CHAN_STAT & UARTPS_BIT_UART_CSTAT_TNFUL) != 0u) {
        if (handle->IntSource != UCOS_INT_SOURCE_NONE) {
            uart_ps->INTR_EN = UARTPS_BIT_UART_INTR_TEMPTY;
            KAL_SemPend(handle->TxSem, 0u, 0u, &kal_err);
        } else {
            while ((uart_ps->CHAN_STAT & UARTPS_BIT_UART_CSTAT_TNFUL) != 0) {}
        }
    }
    uart_ps->TX_RX_FIFO = byte;

    KAL_LockRelease(handle->TxMutex, &kal_err);

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                             UARTPS_RdByte()
*
* Description : Read a byte from the UART.
*
* Argument(s) : handle    Handle of the UART to read.
*
* Return(s)   : Read byte
*
* Note(s)     : Reception is always blocking, interrupt based and protected by an internal RX mutex.
*               When the interrupt line of the UART isn't wired to the current CPU this function
*               will fallback to polling.
*
*********************************************************************************************************
*/

CPU_CHAR  UARTPS_RdByte (UARTPS_HANDLE handle)
{
    UARTPS_PTR uart_ps;
    KAL_ERR    kal_err;
    CPU_CHAR   c;

    uart_ps = handle->UARTPS;

    KAL_LockAcquire(handle->RxMutex, 0u, 0u, &kal_err);

    if ((uart_ps->CHAN_STAT & UARTPS_BIT_UART_CSTAT_REMPTY) != 0u) {
        while ((uart_ps->CHAN_STAT & UARTPS_BIT_UART_CSTAT_REMPTY) != 0u) {
            if (handle->IntSource != UCOS_INT_SOURCE_NONE) {
                uart_ps->INTR_EN = UARTPS_BIT_UART_INTR_RTRIG;
                KAL_SemPend(handle->RxSem, 0u, 0u, &kal_err);
            } else {
                while ((uart_ps->CHAN_STAT & UARTPS_BIT_UART_CSTAT_REMPTY) != 0u) {}
            }
        }
        c = uart_ps->TX_RX_FIFO;
    } else {
        c = uart_ps->TX_RX_FIFO;
    }

    KAL_LockRelease(handle->RxMutex, &kal_err);

    return (c);
}


/*
*********************************************************************************************************
*                                            UARTPS_IntHandler()
*
* Description : UART default interrupt handler.
*
* Argument(s) : p_int_arg    Handle of the UART.
*
* Return(s)   : none.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void UARTPS_IntHandler (void  *p_int_arg)
{
    UARTPS_HANDLE handle;
    UARTPS_PTR    uart_ps;
    KAL_ERR       kal_err;


    handle = (UARTPS_HANDLE)p_int_arg;
    uart_ps = handle->UARTPS;

    if (DEF_BIT_IS_SET(uart_ps->INTR_MASK, UARTPS_BIT_UART_INTR_RTRIG)) {
        KAL_SemPost(handle->RxSem, 0u, &kal_err);
        uart_ps->INTR_DIS = UARTPS_BIT_UART_INTR_RTRIG;
        uart_ps->INTR_STAT = UARTPS_BIT_UART_INTR_RTRIG;
    }

    if (DEF_BIT_IS_SET(uart_ps->INTR_MASK, UARTPS_BIT_UART_INTR_TEMPTY)) {
        KAL_SemPost(handle->TxSem, 0u, &kal_err);
        uart_ps->INTR_DIS = UARTPS_BIT_UART_INTR_TEMPTY;
        uart_ps->INTR_STAT = UARTPS_BIT_UART_INTR_TEMPTY;
    }
}
