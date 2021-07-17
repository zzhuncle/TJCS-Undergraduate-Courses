/*
*********************************************************************************************************
*                                               uC/Clk
*                                          Clock / Calendar
*
*                         (c) Copyright 2005-2019; Silicon Laboratories Inc.,
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
*                                 CLOCK / CALENDAR CONFIGURATION FILE
*
*                                              TEMPLATE
*
* Filename : clk_cfg.h
* Version  : V3.09.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
* Note(s) : (1) This configuration header file is protected from multiple pre-processor inclusion.
*********************************************************************************************************
*/

#ifndef CLK_CFG_H
#define CLK_CFG_H


/*
*********************************************************************************************************
*                                          CLOCK CONFIGURATION
*
* Note(s) : (1) Configure CLK_CFG_EXT_EN to enable/disable an externally-maintained clock :
*
*               (a) When ENABLED,  clock is maintained externally via hardware or another application
*                      (see also 'clk.h  Note #4').
*               (b) When DISABLED, clock is maintained internally via software
*                      (see also 'clk.h  Note #3').
*
*           (2) (a) Configure CLK_CFG_SIGNAL_EN to enable/disable signaling of the internally-maintained
*                   software clock :
*
*                   (1) When ENABLED,  clock is signaled by application calls to Clk_SignalClk().
*                   (2) When DISABLED, clock is signaled by OS-dependent timing features.
*
*               (b) CLK_CFG_SIGNAL_EN configuration is required only if CLK_CFG_EXT_EN is disabled.
*
*               See also 'clk.h  Note #3b'.
*
*           (3) (a) Configure CLK_CFG_SIGNAL_FREQ_HZ to the number of times the application will signal
*                   the clock every second.
*
*               (b) CLK_CFG_SIGNAL_FREQ_HZ configuration is required only if CLK_CFG_SIGNAL_EN is enabled.
*
*           (4) (a) Time zone is based on Coordinated Universal Time (UTC) & has valid values :
*
*                   (1) Between +|- 12 hours (+|- 43200 seconds)
*                   (2) Multiples of 15 minutes
*
*               (b) Time zone offset MUST be set in seconds.
*
*               (c) Default values CAN be changed real-time by using an appropriate function.
*********************************************************************************************************
*/

                                                                /* Configure argument check feature :                   */
#define  CLK_CFG_ARG_CHK_EN                     DEF_ENABLED
                                                                /*   DEF_DISABLED   Argument checks DISABLED            */
                                                                /*   DEF_ENABLED    Argument checks ENABLED             */


                                                                /* Configure Clock string conversion features :         */
#define  CLK_CFG_STR_CONV_EN                    DEF_ENABLED
                                                                /*   DEF_DISABLED   Clock string conversions DISABLED   */
                                                                /*   DEF_ENABLED    Clock string conversions ENABLED    */


                                                                /* Configure Clock NTP conversion features :            */
#define  CLK_CFG_NTP_EN                         DEF_ENABLED
                                                                /*   DEF_DISABLED   NTP conversions DISABLED            */
                                                                /*   DEF_ENABLED    NTP conversions ENABLED             */


                                                                /* Configure Clock Unix conversion features :           */
#define  CLK_CFG_UNIX_EN                        DEF_ENABLED
                                                                /*   DEF_DISABLED   Unix conversions DISABLED           */
                                                                /*   DEF_ENABLED    Unix conversions ENABLED            */


                                                                /* Configure External timestamp feature (see Note #1) : */
#define  CLK_CFG_EXT_EN                         DEF_DISABLED
                                                                /*   DEF_DISABLED   Software Clock maintained           */
                                                                /*   DEF_ENABLED    External Clock maintained           */


                                                                /* Configure Clock signal feature (see Note #2) :       */
#define  CLK_CFG_SIGNAL_EN                      DEF_DISABLED
                                                                /*   DEF_DISABLED   Task time delayed                   */
                                                                /*   DEF_ENABLED    Signaled via application call ...   */
                                                                /*                  ... to Clk_SignalClk()              */


#define  CLK_CFG_SIGNAL_FREQ_HZ                         1000u   /* Configure signal frequency  (see Note #3).           */

#define  CLK_CFG_TZ_DFLT_SEC                               0    /* Configure default time zone (see Note #4).           */


/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                                   0
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                                  1
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                                   2
#endif


#define  CLK_TRACE_LEVEL                        TRACE_LEVEL_OFF

#define  CLK_TRACE                              printf


/*
*********************************************************************************************************
*                                             MODULE END
*
* Note(s) : (1) See 'clk_cfg.h  MODULE'.
*********************************************************************************************************
*/

#endif

