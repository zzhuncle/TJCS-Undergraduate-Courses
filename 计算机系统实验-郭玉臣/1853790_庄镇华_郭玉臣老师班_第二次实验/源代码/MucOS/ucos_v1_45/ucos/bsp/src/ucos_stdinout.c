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
*                                          STDIN and STDOUT
*
* File : ucos_stdinout.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <app_cfg.h>
#include  <cpu_core.h>
#include  <cpu.h>
#include  <ucos_bsp.h>
#include  "ucos_int.h"
#include  <xparameters.h>


#define  DRIVER_USED(drv) ((UCOS_STDOUT_DRIVER == (drv)) || (UCOS_STDIN_DRIVER == (drv)))

#if DRIVER_USED(UCOS_UART_PS7_UART)
#include  <ucos_uartps.h>
#endif

#if DRIVER_USED(UCOS_UART_AXI_UART_LITE)
#include  <ucos_axiuartlite.h>
#endif


#if (UCOS_STDOUT_DRIVER == UCOS_UART_PS7_UART)
UARTPS_HANDLE  STDOUT_Handle;
#elif (UCOS_STDOUT_DRIVER == UCOS_UART_AXI_UART_LITE)
AXIUARTLITE_HANDLE  STDOUT_Handle;
#endif

#if (UCOS_STDIN_DRIVER == UCOS_UART_PS7_UART)
UARTPS_HANDLE  STDIN_Handle;
#elif (UCOS_STDIN_DRIVER == UCOS_UART_AXI_UART_LITE)
AXIUARTLITE_HANDLE  STDIN_Handle;
#endif


void UCOS_StdInOutInit()
{
#if (UCOS_STDOUT_DRIVER == UCOS_UART_PS7_UART)
    STDOUT_Handle = UARTPS_Init(UCOS_STDOUT_DEVICE_ID);
#elif (UCOS_STDOUT_DRIVER == UCOS_UART_AXI_UART_LITE)
    STDOUT_Handle = AXIUARTLite_Init(UCOS_STDOUT_DEVICE_ID);
#endif

#if (UCOS_STDIN_DRIVER == UCOS_UART_PS7_UART)
    STDIN_Handle = UARTPS_Init(UCOS_STDIN_DEVICE_ID);
#elif (UCOS_STDIN_DRIVER == UCOS_UART_AXI_UART_LITE)
    STDIN_Handle = AXIUARTLite_Init(UCOS_STDIN_DEVICE_ID);
#endif
}


void  UCOS_Print (const CPU_CHAR *p_string)
{
#if (UCOS_STDOUT_DRIVER == UCOS_UART_PS7_UART)
    UARTPS_WrStr(STDOUT_Handle, p_string);
#elif (UCOS_STDOUT_DRIVER == UCOS_UART_AXI_UART_LITE)
    AXIUARTLite_WrStr(STDOUT_Handle, p_string);
#else
    (void)p_string;
#endif
}


CPU_INT32U UCOS_Read (CPU_CHAR *buf, CPU_INT32U cnt)
{
    CPU_INT32U i = 0;


#if (UCOS_STDIN_DRIVER == UCOS_UART_PS7_UART)
    for (i = 0; i < cnt; i++) {
        *(buf + i) = UARTPS_RdByte(STDIN_Handle);
        if ((*(buf + i) == '\n' || *(buf + i) == '\r'))
        {
            i++;
            break;
        }
    }
#elif (UCOS_STDIN_DRIVER == UCOS_UART_AXI_UART_LITE)
    for (i = 0; i < cnt; i++) {
        *(buf + i) = AXIUARTLite_RdByte(STDIN_Handle);
        if ((*(buf + i) == '\n' || *(buf + i) == '\r'))
        {
            i++;
            break;
        }
    }
#else
    (void)buf;
    (void)cnt;
#endif

    return i;
}
