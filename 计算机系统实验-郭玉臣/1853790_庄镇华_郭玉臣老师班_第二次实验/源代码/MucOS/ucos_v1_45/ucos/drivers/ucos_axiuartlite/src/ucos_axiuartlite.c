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
* File : ucos_axiuartlite.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <xparameters.h>
#include <ucos_bsp.h>
#include <ucos_axiuartlite.h>
#include <ucos_int.h>

#include <KAL/kal.h>


extern UCOS_AXIUARTLITE_Config UCOS_AXIUARTLITE_ConfigTable[];

static UCOS_AXIUARTLITE UCOS_AXIUARTLITE_Table[XPAR_UCOS_AXIUARTLITE_NUM_INSTANCES];


/*
*********************************************************************************************************
*                                   STANDALONE COMPATIBILITY LAYER
*********************************************************************************************************
*/

void AXIUARTLite_SendByte(u32 BaseAddress, u8 Data)
{
    AXIUARTLITE_HANDLE handle;
    CPU_INT32U device_id;

    for (device_id = 0; device_id < XPAR_UCOS_AXIUARTLITE_NUM_INSTANCES; device_id++) {
        if (UCOS_AXIUARTLITE_ConfigTable[device_id].BaseAddress == BaseAddress)
        {
            handle = (AXIUARTLITE_HANDLE)&UCOS_AXIUARTLITE_Table[device_id];
            AXIUARTLite_WrByte (handle, Data);
        }
    }
}

u8 AXIUARTLite_RecvByte(u32 BaseAddress)
{
    AXIUARTLITE_HANDLE handle;
    CPU_INT32U device_id;
    CPU_CHAR c;

    c = '\0';
    for (device_id = 0; device_id < XPAR_UCOS_AXIUARTLITE_NUM_INSTANCES; device_id++) {
        if (UCOS_AXIUARTLITE_ConfigTable[device_id].BaseAddress == BaseAddress)
        {
            handle = (AXIUARTLITE_HANDLE)&UCOS_AXIUARTLITE_Table[device_id];
            c = AXIUARTLite_RdByte (handle);
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
*                                            AXIUARTLite_Init()
*
* Description : Initialize an AXI UART.
*
* Argument(s) : device_id  ID of the timer to init.
*
* Return(s)   : Handle to the AXI UART instance or DEF_NULL if
*               the devide id is invalid.
*
* Note(s)     : Should be called before accessing any other functionalities of this peripheral.
*
*********************************************************************************************************
*/

AXIUARTLITE_HANDLE AXIUARTLite_Init (CPU_INT32U device_id)
{
    AXIUARTLITE_HANDLE handle;
    AXIUARTLITE_PTR uart;
    CPU_INT32U int_source;
    KAL_ERR kal_err;


    if (device_id > XPAR_UCOS_AXIUARTLITE_NUM_INSTANCES) {
        return (DEF_NULL);
    }

    handle = (AXIUARTLITE_HANDLE)&UCOS_AXIUARTLITE_Table[device_id];

    handle->DeviceId = UCOS_AXIUARTLITE_ConfigTable[device_id].DeviceId;
    uart = (AXIUARTLITE_PTR)UCOS_AXIUARTLITE_ConfigTable[device_id].BaseAddress;
    handle->UARTLite = uart;
    handle->BaudRate = UCOS_AXIUARTLITE_ConfigTable[device_id].BaudRate;
    handle->UseParity = UCOS_AXIUARTLITE_ConfigTable[device_id].UseParity;
    handle->OddParity = UCOS_AXIUARTLITE_ConfigTable[device_id].OddParity;
    handle->DataBits = UCOS_AXIUARTLITE_ConfigTable[device_id].DataBits;
    handle->IntReq = 0u;


    handle->TxSem = KAL_SemCreate("AXIUARTLite Tx Semaphore", DEF_NULL, &kal_err);
    if (kal_err != KAL_ERR_NONE) {
        return (DEF_NULL);
    }

    handle->RxSem = KAL_SemCreate("AXIUARTLite Rx Semaphore", DEF_NULL, &kal_err);
    if (kal_err != KAL_ERR_NONE) {
        return (DEF_NULL);
    }

    handle->TxMutex = KAL_LockCreate("AXIUARTLite Tx Mutex", DEF_NULL, &kal_err);
    if (kal_err != KAL_ERR_NONE) {
        return (DEF_NULL);
    }

    handle->RxMutex = KAL_LockCreate("AXIUARTLite Rx Mutex", DEF_NULL, &kal_err);
    if (kal_err != KAL_ERR_NONE) {
        return (DEF_NULL);
    }


    int_source = UCOS_AXIUARTLITE_ConfigTable[device_id].IntSource;
    handle->IntSource = int_source;

    if (int_source != UCOS_INT_SOURCE_NONE) {
        UCOS_IntVectSet(int_source,
                        0u,
                        0u,
                        AXIUARTLite_IntHandler,
                        (void *)handle);

        UCOS_IntTypeSet(int_source, UCOS_INT_TYPE_EDGE);

        UCOS_IntSrcEn(int_source);
    }

    return (handle);
}


/*
*********************************************************************************************************
*                                           AXIUARTLite_WrStr()
*
* Description : Write a null terminated string through the UART.
*
* Argument(s) : handle    Handle of the UART to write.
*
*               p_str     String to output.
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : When possible transmission is always blocking, interrupt based and protected by an internal
*               TX mutex. AXIUARTLite_WrStr will return once the entire string has been written to the device FIFO.
*               If the interrupt source of the UART isn't write to this CPU this function will fallback
*               to polling.
*
*********************************************************************************************************
*/

CPU_BOOLEAN  AXIUARTLite_WrStr (AXIUARTLITE_HANDLE handle,
                                const CPU_CHAR    *p_str)
{
    AXIUARTLITE_PTR uart;
    KAL_ERR kal_err;
    CPU_SR_ALLOC();


    uart = handle->UARTLite;

    KAL_LockAcquire(handle->TxMutex, 0u, 0u, &kal_err);

    while ((*p_str) != DEF_NULL) {

        CPU_CRITICAL_ENTER();
        if ((uart->STAT & AXIUARTLITE_BIT_STAT_TXFULL) != 0u) {
            if (handle->IntSource != UCOS_INT_SOURCE_NONE) {
                handle->IntReq |= AXIUARTLITE_INT_TX_REQ;
                uart->CTRL |= AXIUARTLITE_BIT_CTRL_INTEN;
                CPU_CRITICAL_EXIT();
                KAL_SemPend(handle->TxSem, 0u, 0u, &kal_err);
            } else {
                CPU_CRITICAL_EXIT();
                while((uart->STAT & AXIUARTLITE_BIT_STAT_TXFULL) != 0u) {}
            }
        } else {
            CPU_CRITICAL_EXIT();
        }
        uart->TX = *p_str;
        p_str++;
    }

    KAL_LockRelease(handle->TxMutex, &kal_err);

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                           AXIUARTLite_WrByte()
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
*               AXIUARTLite_WrByte will return once the byte has been written to the device FIFO.
*
*********************************************************************************************************
*/

CPU_BOOLEAN  AXIUARTLite_WrByte (AXIUARTLITE_HANDLE handle,
                                 CPU_CHAR           byte)
{
    AXIUARTLITE_PTR uart;
    KAL_ERR kal_err;
    CPU_SR_ALLOC();


    uart = handle->UARTLite;

    KAL_LockAcquire(handle->TxMutex, 0u, 0u, &kal_err);

    CPU_CRITICAL_ENTER();
    if ((uart->STAT & AXIUARTLITE_BIT_STAT_TXFULL) != 0u) {
        if (handle->IntSource != UCOS_INT_SOURCE_NONE) {
            handle->IntReq |= AXIUARTLITE_INT_TX_REQ;
            uart->CTRL |= AXIUARTLITE_BIT_CTRL_INTEN;
            CPU_CRITICAL_EXIT();
            KAL_SemPend(handle->TxSem, 0u, 0u, &kal_err);
        } else {
            CPU_CRITICAL_EXIT();
            while((uart->STAT & AXIUARTLITE_BIT_STAT_TXFULL) != 0u) {}
        }
    } else {
        CPU_CRITICAL_EXIT();
    }
    uart->TX = byte;

    KAL_LockRelease(handle->TxMutex, &kal_err);

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                           AXIUARTLite_RdByte()
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

CPU_CHAR  AXIUARTLite_RdByte (AXIUARTLITE_HANDLE handle)
{
    AXIUARTLITE_PTR uart;
    KAL_ERR kal_err;
    CPU_CHAR c;
    CPU_SR_ALLOC();


    uart = handle->UARTLite;

    KAL_LockAcquire(handle->RxMutex, 0u, 0u, &kal_err);
    c = 0;

    if ((uart->STAT & AXIUARTLITE_BIT_STAT_RXVALID) != 0) {
        c = uart->RX;
    } else {
        if (handle->IntSource != UCOS_INT_SOURCE_NONE) {
            CPU_CRITICAL_ENTER();
            handle->IntReq |= AXIUARTLITE_INT_RX_REQ;
            uart->CTRL |= AXIUARTLITE_BIT_CTRL_INTEN;
            CPU_CRITICAL_EXIT();
            KAL_SemPend(handle->RxSem, 0u, 0u, &kal_err);
        } else {
            while ((uart->STAT & AXIUARTLITE_BIT_STAT_RXVALID) != 0) {}
        }
        c = uart->RX;
    }
    KAL_LockRelease(handle->RxMutex, &kal_err);

    return (c);
}

/*
*********************************************************************************************************
*                                        AXIUARTLite_IntHandler()
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

void AXIUARTLite_IntHandler (void  *p_int_arg)
{
    AXIUARTLITE_HANDLE handle;
    AXIUARTLITE_PTR    uart;
    KAL_ERR            kal_err;


    handle = (AXIUARTLITE_HANDLE)p_int_arg;
    uart = handle->UARTLite;

    if (DEF_BIT_IS_SET(uart->STAT, AXIUARTLITE_BIT_STAT_RXVALID) && DEF_BIT_IS_SET(handle->IntReq, AXIUARTLITE_INT_RX_REQ)) {
        KAL_SemPost(handle->RxSem, 0u, &kal_err);
        handle->IntReq &= ~AXIUARTLITE_INT_RX_REQ;
    }

    if (DEF_BIT_IS_SET(uart->STAT, AXIUARTLITE_BIT_STAT_TXEMPTY) && DEF_BIT_IS_SET(handle->IntReq, AXIUARTLITE_INT_TX_REQ)) {
        KAL_SemPost(handle->TxSem, 0u, &kal_err);
        handle->IntReq &= ~AXIUARTLITE_INT_TX_REQ;
    }

    if (handle->IntReq == 0u) {
        uart->CTRL &= ~AXIUARTLITE_BIT_CTRL_INTEN;
    }
}
