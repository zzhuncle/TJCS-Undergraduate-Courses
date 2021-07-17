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
*                            NETWORK BOARD SUPPORT PACKAGE (BSP) FUNCTIONS
*                                    Cadence Gigabit Ethernet MAC
*                                               on the
*                                          Ultrascale+ MPSoC
*
* File : net_ultrascale_gem_bsp.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    NET_BSP_MODULE

#include  <cpu.h>
#include  <lib_def.h>

#include  <ucos_bsp.h>
#include  <ucos_int.h>

#include  <IF/net_if_ether.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                  NETWORK DEVICE INTERFACE NUMBERS
*********************************************************************************************************
*/

static  NET_IF_NBR  ZCU102_GEM_3_IF_Nbr = NET_IF_NBR_NONE;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                               NETWORK DEVICE BSP FUNCTION PROTOTYPES
*********************************************************************************************************
*/

                                                                /* -- IF #1 : GENERIC ETHER BSP FUNCTION PROTOTYPES -- */
static  void        NetDev_GEM_CfgClk3        (NET_IF      *p_if,
                                               NET_ERR     *p_err);

static  void        NetDev_GEM_CfgIntCtrl3    (NET_IF      *p_if,
                                               NET_ERR     *p_err);

static  void        NetDev_GEM_CfgGPIO3       (NET_IF      *p_if,
                                               NET_ERR     *p_err);

static  CPU_INT32U  NetDev_GEM_ClkFreqGet3    (NET_IF      *p_if,
                                               NET_ERR     *p_err);

static  void        NetDev_GEM_ISR_Handler3   (void        *p_arg);


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                    NETWORK DEVICE BSP INTERFACE
*********************************************************************************************************
*/

const  NET_DEV_BSP_ETHER     NetDev_BSP_Ultrascale_GEM_3 = {                    /* Board-/device-specific BSP fnct ptrs : */
                                                         &NetDev_GEM_CfgClk3,       /*   Cfg clk(s)                           */
                                                         &NetDev_GEM_CfgIntCtrl3,   /*   Cfg int ctrl(s)                      */
                                                         &NetDev_GEM_CfgGPIO3,      /*   Cfg GPIO                             */
                                                         &NetDev_GEM_ClkFreqGet3    /*   Get clk freq                         */
                                                };

/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                              NETWORK ETHERNET DEVICE DRIVER FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         NetDev_GEM_CfgClk3()
*
* Description : Configure clocks for the specified interface/device.
*
* Argument(s) : p_if         Pointer to network interface to configure.
*               ----         Argument validated in NetIF_Add().
*
*               p_err        Pointer to variable that will receive the return error code from this function :
*
*                               NET_DEV_ERR_NONE                Device clock(s)     successfully configured.
*                               NET_DEV_ERR_FAULT               Device clock(s) NOT successfully configured.
*
* Return(s)   : none.
*
* Caller(s)   : NetDev_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  NetDev_GEM_CfgClk3 (NET_IF   *p_if,
                                  NET_ERR  *p_err)
{
    CPU_INT32U  reg_val;
   (void)&p_if;                                                  /* Prevent 'variable unused' compiler warning.          */

   *p_err = NET_DEV_ERR_NONE;
}


/*
*********************************************************************************************************
*                                       NetDev_GEM_CfgIntCtrl3()
*
* Description : Configure interrupts &/or interrupt controller for the specified interface/device.
*
* Argument(s) : p_if         Pointer to network interface to configure.
*               ----         Argument validated in NetIF_Add().
*
*               p_err        Pointer to variable that will receive the return error code from this function :
*
*                               NET_DEV_ERR_NONE                Device interrupt(s)     successfully configured.
*                               NET_DEV_ERR_FAULT               Device interrupt(s) NOT successfully configured.
*
* Return(s)   : none.
*
* Caller(s)   : NetDev_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  NetDev_GEM_CfgIntCtrl3 (NET_IF   *p_if,
                                      NET_ERR  *p_err)
{
    ZCU102_GEM_3_IF_Nbr = p_if->Nbr;

    UCOS_IntVectSet(95u,
                     0u,
                     0u,
                     NetDev_GEM_ISR_Handler3,
                     DEF_NULL);

    UCOS_IntSrcEn(95u);

   *p_err = NET_DEV_ERR_NONE;
}


/*
*********************************************************************************************************
*                                        NetDev_GEM_CfgGPIO3()
*
* Description : Configure general-purpose I/O (GPIO) for the specified interface/device.
*
* Argument(s) : p_if         Pointer to network interface to configure.
*               ----         Argument validated in NetIF_Add().
*
*               p_err        Pointer to variable that will receive the return error code from this function :
*
*                               NET_DEV_ERR_NONE                Device GPIO     successfully configured.
*                               NET_DEV_ERR_FAULT               Device GPIO NOT successfully configured.
*
* Return(s)   : none.
*
* Caller(s)   : NetDev_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  NetDev_GEM_CfgGPIO3 (NET_IF   *p_if,
                                   NET_ERR  *p_err)
{
   (void)&p_if;                                                  /* Prevent 'variable unused' compiler warning.          */

   *p_err = NET_DEV_ERR_NONE;
}


/*
*********************************************************************************************************
*                                       NetDev_GEM_ClkFreqGet3()
*
* Description : Get device clock frequency.
*
* Argument(s) : p_if         Pointer to network interface to get clock frequency.
*               ----         Argument validated in NetIF_Add().
*
*               p_err        Pointer to variable that will receive the return error code from this function :
*
*                               NET_DEV_ERR_NONE                Device clock frequency     successfully
*                                                                   returned.
*                               NET_DEV_ERR_FAULT               Device clock frequency NOT successfully
*                                                                   returned.
*
* Return(s)   : Device clock frequency (in Hz).
*
* Caller(s)   : NetDev_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_INT32U  NetDev_GEM_ClkFreqGet3 (NET_IF   *p_if,
                                            NET_ERR  *p_err)
{
    CPU_INT32U  clk_freq;


   (void)&p_if;                                                  /* Prevent 'variable unused' compiler warning.          */

    clk_freq = 0;
   *p_err    = NET_DEV_ERR_NONE;

    return (clk_freq);
}


/*
*********************************************************************************************************
*                                      NetDev_GEM_ISR_Handler3()
*
* Description : BSP-level ISR handler(s) for device interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU &/or device interrupts.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  NetDev_GEM_ISR_Handler3 (void  *p_arg)
{
    NET_IF_NBR        if_nbr;
    NET_DEV_ISR_TYPE  type;
    NET_ERR           err;


    if_nbr = ZCU102_GEM_3_IF_Nbr;
    type   = NET_DEV_ISR_TYPE_UNKNOWN;

    NetIF_ISR_Handler(if_nbr, type, &err);
}
