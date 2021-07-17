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
*                                          CLOCK / CALENDAR
*
* Filename : clk.h
* Version  : V3.09.07
*********************************************************************************************************
* Note(s)  : (1) Assumes the following versions (or more recent) of software modules are included
*                in the project build :
*
*                (a) uC/CPU V1.25
*                (b) uC/LIB V1.29
*
*
*            (2) (a) Clock module is based on Coordinated Universal Time (UTC) and supports the
*                    following features :
*
*                    (1) Time zones
*                    (2) Leap years
*                    (3) Leap seconds
*
*                (b) Clock module does NOT support Daylight (Savings) Time. If you want to handle
*                    Daylight Time in your application, set time zone offset accordingly.
*
*                (c) Timestamp and Coordinated Universal Time (UTC) related links :
*
*                    (1) http://www.timeanddate.com/time/aboututc.html
*                    (2) http://www.allanstime.com/Publications/DWA/Science_Timekeeping/TheScienceOfTimekeeping.pdf
*                    (3) http://www.cl.cam.ac.uk/~mgk25/time/metrologia-leapsecond.pdf
*                    (4) http://www.navcen.uscg.gov/pdf/cgsicMeetings/45/29a%20UTCLeapSecond.ppt
*
*
*            (3) (a) Clock module implements a software-maintained clock/calendar when 'CLK_CFG_EXT_EN'
*                    is disabled (see Note #4).
*
*                (b) (1) Software-maintained clock/calendar is based on a periodic delay or timeout
*                        when 'CLK_CFG_SIGNAL_EN' is disabled.
*
*                    (2) Software-maintained clock/calendar is based on a periodic signal or timer
*                        when 'CLK_CFG_SIGNAL_EN' is enabled.
*
*                (c) When software-maintained clock is enabled, Clock module's OS-dependent files and
*                    respective OS-application configuration MUST be included in the build.
*
*
*            (4) (a) Clock module initializes, gets and sets its timestamp via an External timestamp
*                    when 'CLK_CFG_EXT_EN' is enabled.
*
*                (b) (1) External timestamp can be maintained either in :
*
*                        (A) Hardware (e.g. via a hardware clock chip)
*                        (B) From another application (e.g. SNTPc)
*
*                    (2) External timestamp is accessed by application/BSP functions defined by the
*                        developer that MUST follow the functional requirements of the particular
*                        hardware/application(s).
*
*                        See also 'net.h  Clk_ExtTS_Init()',
*                                 'net.h  Clk_ExtTS_Get()',
*                               & 'net.h  Clk_ExtTS_Set()'.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This Clock module header file is protected from multiple pre-processor inclusion through
*               use of the Clock module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  CLK_MODULE_PRESENT                                     /* See Note #1.                                         */
#define  CLK_MODULE_PRESENT


/*
*********************************************************************************************************
*                                        CLOCK VERSION NUMBER
*
* Note(s) : (1) (a) The Clock software version is denoted as follows :
*
*                       Vx.yy.zz
*
*                           where
*                                   V               denotes 'Version' label
*                                   x               denotes     major software version revision number
*                                   yy              denotes     minor software version revision number
*                                   zz              denotes sub-minor software version revision number
*
*               (b) The Clock software version label #define is formatted as follows :
*
*                       ver = x.yyzz * 100 * 100
*
*                           where
*                                   ver             denotes software version number scaled as an integer value
*                                   x.yyzz          denotes software version number, where the unscaled integer
*                                                       portion denotes the major version number & the unscaled
*                                                       fractional portion denotes the (concatenated) minor
*                                                       version numbers
*********************************************************************************************************
*/

#define  CLK_VERSION                                   30907u   /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   CLK_MODULE
#define  CLK_EXT
#else
#define  CLK_EXT  extern
#endif


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*
* Note(s) : (1) Clock module files are located in the following directories :
*
*               (a) (1) \<Your Product Application>\clk_cfg.h
*                   (2)                            \app_cfg.h       See 'clk.h  Note #3c'
*
*               (b) (1) \<CLK>\Source\clk.h
*                                    \clk.c
*
*                   (2) \<CLK>\OS\<os>\clk_os.c
*
*                       where
*                               <Your Product Application>      directory path for Your Product's Application
*                               <CLK>                           directory path for Clock module
*                               <os>                            directory name for specific operating system (OS)
*
*           (2) CPU-configuration software files are located in the following directories :
*
*               (a) \<CPU-Compiler Directory>\cpu_def.h
*
*               (b) \<CPU-Compiler Directory>\<cpu>\<compiler>\cpu*.*
*
*                       where
*                               <CPU-Compiler Directory>        directory path for common   CPU-compiler software
*                               <cpu>                           directory name for specific processor (CPU)
*                               <compiler>                      directory name for specific compiler
*
*           (3) NO compiler-supplied standard library functions are used by the Clock module.
*
*               (a) Standard library functions are implemented in the custom library module(s) :
*
*                       \<Custom Library Directory>\lib*.*
*
*                       where
*                               <Custom Library Directory>      directory path for custom library software
*
*           (4) Compiler MUST be configured to include as additional include path directories :
*
*               (a) '\<Your Product Application>\'                                          See Note #1a
*
*               (b) '\<CLK>\' directories                                                   See Note #1b
*
*               (c) (1) '\<CPU-Compiler Directory>\'                                        See Note #2a
*                   (2) '\<CPU-Compiler Directory>\<cpu>\<compiler>\'                       See Note #2b
*
*               (d) '\<Custom Library Directory>\'                                          See Note #3a
*********************************************************************************************************
*/

#include  <cpu.h>                                               /* CPU Configuration       (see Note #2)                */
#include  <cpu_core.h>                                          /* CPU Core Library                                     */

#include  <lib_def.h>                                           /* Standard        Defines (see Note #3a)               */
#include  <lib_str.h>                                           /* Standard String Library (see Note #3a)               */

#include  <clk_cfg.h>                                           /* Clk Configuration File  (see Note #1a)               */


/*
*********************************************************************************************************
*                                        MODULE CONFIGURATION
*
* Note(s) : (1) (a) When the External timestamp is disabled, the Clock/Calendar is software-  maintained
*                   and 'clk_os.c' MUST             be included in the project (see 'net.h  Note #3c').
*
*               (b) When the External timestamp is enabled,  the Clock/Calendar is externally maintained
*                   and 'clk_os.c' does NOT need to be included in the project.
*********************************************************************************************************
*/

#if     (CLK_CFG_EXT_EN != DEF_ENABLED)
#define  CLK_OS_MODULE_PRESENT                                  /* See Note #1a.                                        */
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      CLOCK ERROR CODE DEFINES
*********************************************************************************************************
*/

#define  CLK_ERR_NONE                                      0u   /* No err.                                              */


/*
*********************************************************************************************************
*                                  CLOCK-OS LAYER ERROR CODE DEFINES
*********************************************************************************************************
*/

#define  CLK_OS_ERR_NONE                                 100u

#define  CLK_OS_ERR_INIT_TASK                            101u
#define  CLK_OS_ERR_INIT_SIGNAL                          102u
#define  CLK_OS_ERR_INIT_NAME                            105u

#define  CLK_OS_ERR_WAIT                                 120u
#define  CLK_OS_ERR_SIGNAL                               121u


/*
*********************************************************************************************************
*                                       CLK STR FORMAT DEFINES
*********************************************************************************************************
*/

#define  CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS_UTC               1u   /* Fmt date/time as "YYYY-MM-DD HH:MM:SS UTC+TZ" :      */
                                                                /*           ...    "YYYY-MM-DD HH:MM:SS UTC+hh:mm"     */
                                                                /*           ... or "YYYY-MM-DD HH:MM:SS UTC-hh:mm".    */
#define  CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS                   2u   /* Fmt date/time as "YYYY-MM-DD HH:MM:SS".              */
#define  CLK_STR_FMT_MM_DD_YY_HH_MM_SS                     3u   /* Fmt date/time as "MM-DD-YY HH:MM:SS".                */
#define  CLK_STR_FMT_YYYY_MM_DD                            4u   /* Fmt date/time as "YYYY-MM-DD".                       */
#define  CLK_STR_FMT_MM_DD_YY                              5u   /* Fmt date/time as "MM-DD-YY".                         */
#define  CLK_STR_FMT_DAY_MONTH_DD_YYYY                     6u   /* Fmt date/time as "Day Month DD, YYYY".               */
#define  CLK_STR_FMT_DAY_MONTH_DD_HH_MM_SS_YYYY            7u   /* Fmt date/time as "Day Mon DD HH:MM:SS YYYY".         */
#define  CLK_STR_FMT_HH_MM_SS                              8u   /* Fmt date/time as "HH:MM:SS".                         */
#define  CLK_STR_FMT_HH_MM_SS_AM_PM                        9u   /* Fmt date/time as "HH:MM:SS AM|PM".                   */

                                                                                    /*           1         2         3  */
                                                                                    /* 0123456789012345678901234567890  */
#define  CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS_UTC_LEN          30u   /*     Str len of fmt "YYYY-MM-DD HH:MM:SS UTC+TZ" :    */
                                                                /*             ...    "YYYY-MM-DD HH:MM:SS UTC+hh:mm"   */
                                                                /*             ... or "YYYY-MM-DD HH:MM:SS UTC-hh:mm".  */
#define  CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS_LEN              20u   /*     Str len of fmt "YYYY-MM-DD HH:MM:SS".            */
#define  CLK_STR_FMT_MM_DD_YY_HH_MM_SS_LEN                18u   /*     Str len of fmt "MM-DD-YY HH:MM:SS".              */
#define  CLK_STR_FMT_YYYY_MM_DD_LEN                       11u   /*     Str len of fmt "YYYY-MM-DD".                     */
#define  CLK_STR_FMT_MM_DD_YY_LEN                          9u   /*     Str len of fmt "MM-DD-YY".                       */
#define  CLK_STR_FMT_DAY_MONTH_DD_YYYY_MAX_LEN            29u   /* Max str len of fmt "Day Month DD, YYYY".             */
#define  CLK_STR_FMT_DAY_MONTH_DD_HH_MM_SS_YYYY_LEN       25u   /*     Str len of fmt "Day Mon DD HH:MM:SS YYYY".       */
#define  CLK_STR_FMT_HH_MM_SS_LEN                          9u   /*     Str len of fmt "HH:MM:SS".                       */
#define  CLK_STR_FMT_HH_MM_SS_AM_PM_LEN                   15u   /*     Str len of fmt "HH:MM:SS AM|PM".                 */

#define  CLK_STR_FMT_MAX_LEN                              30u   /* Max str len of all clk str fmts.                     */


#define  CLK_STR_DIG_YR_LEN                                4u   /*     Str len of       yr dig.                         */
#define  CLK_STR_DIG_YR_TRUNC_LEN                          2u   /*     Str len of trunc yr dig.                         */
#define  CLK_STR_DIG_MONTH_LEN                             2u   /*     Str len of mon      dig.                         */
#define  CLK_STR_DIG_DAY_LEN                               2u   /*     Str len of day      dig.                         */
#define  CLK_STR_DIG_HR_LEN                                2u   /*     Str len of hr       dig.                         */
#define  CLK_STR_DIG_MIN_LEN                               2u   /*     Str len of min      dig.                         */
#define  CLK_STR_DIG_SEC_LEN                               2u   /*     Str len of sec      dig.                         */
#define  CLK_STR_DIG_TZ_HR_LEN                             2u   /*     Str len of tz hr    dig.                         */
#define  CLK_STR_DIG_TZ_MIN_LEN                            2u   /*     Str len of tz min   dig.                         */
#define  CLK_STR_DIG_TZ_MAX_LEN                            2u   /* Max str len of tz       digs.                        */
#define  CLK_STR_DAY_OF_WK_MAX_LEN                         9u   /* Max str len of day of wk       str (e.g. Wednesday). */
#define  CLK_STR_DAY_OF_WK_TRUNC_LEN                       3u   /*     Str len of day of wk trunc str.                  */
#define  CLK_STR_MONTH_MAX_LEN                             9u   /* Max str len of month           str (e.g. September). */
#define  CLK_STR_MONTH_TRUNC_LEN                           3u   /*     Str len of month     trunc str.                  */
#define  CLK_STR_AM_PM_LEN                                 2u   /*     Str len of am-pm           str.                  */


/*
*********************************************************************************************************
*                                            CLOCK DEFINES
*
* Note(s) : (1) Year 2038 problem (e.g. Unix Millennium bug, Y2K38 or Y2.038K) may cause some computer
*               software to fail before or in the year 2038. The problem affects all software and
*               systems that both store time as a signed 32-bit integer and interpret this number as
*               the number of seconds since 00:00:00 UTC on 1970/01/1.
*
*               There is no straightforward and general fix for this problem. Changing timestamp
*               datatype to an unsigned 32-bit integer have been chosen to avoid this problem. Thus
*               timestamp will be accurate until the year 2106, but dates before 1970 are not possible.
*********************************************************************************************************
*/

#define  CLK_FIRST_MONTH_OF_YR                             1u   /* First month of a yr    [1 to  12].                   */
#define  CLK_FIRST_DAY_OF_MONTH                            1u   /* First day   of a month [1 to  31].                   */
#define  CLK_FIRST_DAY_OF_YR                               1u   /* First day   of a yr    [1 to 366].                   */
#define  CLK_FIRST_DAY_OF_WK                               1u   /* First day   of a wk    [1 to   7].                   */


#define  CLK_MONTH_PER_YR                                 12u
#define  CLK_HR_PER_HALF_DAY                              12uL

#define  CLK_YR_NONE                                       0u

#define  CLK_MONTH_NONE                                    0u
#define  CLK_MONTH_JAN                                     1u
#define  CLK_MONTH_FEB                                     2u
#define  CLK_MONTH_MAR                                     3u
#define  CLK_MONTH_APR                                     4u
#define  CLK_MONTH_MAY                                     5u
#define  CLK_MONTH_JUN                                     6u
#define  CLK_MONTH_JUL                                     7u
#define  CLK_MONTH_AUG                                     8u
#define  CLK_MONTH_SEP                                     9u
#define  CLK_MONTH_OCT                                    10u
#define  CLK_MONTH_NOV                                    11u
#define  CLK_MONTH_DEC                                    12u

#define  CLK_DAY_NONE                                      0u
#define  CLK_DAY_OF_WK_NONE                                0u
#define  CLK_DAY_OF_WK_SUN                                 1u
#define  CLK_DAY_OF_WK_MON                                 2u
#define  CLK_DAY_OF_WK_TUE                                 3u
#define  CLK_DAY_OF_WK_WED                                 4u
#define  CLK_DAY_OF_WK_THU                                 5u
#define  CLK_DAY_OF_WK_FRI                                 6u
#define  CLK_DAY_OF_WK_SAT                                 7u


                                                                /* ------------------ CLK TS DEFINES ------------------ */
#define  CLK_TS_SEC_MIN                 DEF_INT_32U_MIN_VAL
#define  CLK_TS_SEC_MAX                 DEF_INT_32U_MAX_VAL
#define  CLK_TS_SEC_NONE                CLK_TS_SEC_MIN


                                                                /* ------------------ CLK TZ DEFINES ------------------ */
#define  CLK_TZ_MIN_PRECISION                             15uL
#define  CLK_TZ_SEC_PRECISION          (CLK_TZ_MIN_PRECISION * DEF_TIME_NBR_SEC_PER_MIN)
#define  CLK_TZ_SEC_MIN              (-(CLK_HR_PER_HALF_DAY  * DEF_TIME_NBR_SEC_PER_HR))
#define  CLK_TZ_SEC_MAX                (CLK_HR_PER_HALF_DAY  * DEF_TIME_NBR_SEC_PER_HR)


                                                                /* ----------------- CLK TICK DEFINES ----------------- */
#define  CLK_TICK_NONE                                     0u


                                                                /* ---------------- CLK EPOCH DEFINES ----------------- */
#define  CLK_EPOCH_YR_START                             2000u   /* Clk epoch starts = 2000-01-01 00:00:00 UTC.          */
#define  CLK_EPOCH_YR_END                               2136u   /*           ends   = 2135-12-31 23:59:59 UTC.          */
#define  CLK_EPOCH_DAY_OF_WK                               7u   /*                    2000-01-01 is Sat.                */


                                                                /* -------------- NTP EPOCH DATE DEFINES -------------- */
#define  CLK_NTP_EPOCH_YR_START                         1900u   /* NTP epoch starts = 1900-01-01 00:00:00 UTC.          */
#define  CLK_NTP_EPOCH_YR_END                           2036u   /*           ends   = 2035-12-31 23:59:59 UTC.          */
#define  CLK_NTP_EPOCH_DAY_OF_WK                           2u   /*                    1900-01-01 is Mon.                */
#define  CLK_NTP_EPOCH_OFFSET_YR_CNT           (CLK_EPOCH_YR_START - CLK_NTP_EPOCH_YR_START)

                                                                /* Only 24 leap yrs because 1900 is NOT a leap yr.      */
#define  CLK_NTP_EPOCH_OFFSET_LEAP_DAY_CNT    ((CLK_NTP_EPOCH_OFFSET_YR_CNT / 4u) - 1u)

                                                                /*     100 yrs * 365 * 24 * 60 * 60 = 3153600000        */
                                                                /*   +  24 leap days * 24 * 60 * 60 =    2073600        */
                                                                /* CLK_NTP_OFFSET_SEC               = 3155673600        */
#define  CLK_NTP_EPOCH_OFFSET_SEC             ((CLK_NTP_EPOCH_OFFSET_YR_CNT       * DEF_TIME_NBR_SEC_PER_YR ) +  \
                                               (CLK_NTP_EPOCH_OFFSET_LEAP_DAY_CNT * DEF_TIME_NBR_SEC_PER_DAY))


                                                                /* ------------- UNIX EPOCH DATE DEFINES -------------- */
                                                                /* See Note #1.                                         */
#define  CLK_UNIX_EPOCH_YR_START                        1970u   /* Unix epoch starts = 1970-01-01 00:00:00 UTC.         */
#define  CLK_UNIX_EPOCH_YR_END                          2106u   /*            ends   = 2105-12-31 23:59:59 UTC.         */
#define  CLK_UNIX_EPOCH_DAY_OF_WK                          5u   /*                     1970-01-01 is Thu.               */
#define  CLK_UNIX_EPOCH_OFFSET_YR_CNT          (CLK_EPOCH_YR_START - CLK_UNIX_EPOCH_YR_START)
#define  CLK_UNIX_EPOCH_OFFSET_LEAP_DAY_CNT    (CLK_UNIX_EPOCH_OFFSET_YR_CNT / 4u)

                                                                /*     30 yrs * 365 * 24 * 60 * 60 = 946080000          */
                                                                /*   +  7 leap days * 24 * 60 * 60 =    604800          */
                                                                /* CLK_UNIX_OFFSET_SEC             = 946684800          */
#define  CLK_UNIX_EPOCH_OFFSET_SEC            ((CLK_UNIX_EPOCH_OFFSET_YR_CNT       * DEF_TIME_NBR_SEC_PER_YR ) +  \
                                               (CLK_UNIX_EPOCH_OFFSET_LEAP_DAY_CNT * DEF_TIME_NBR_SEC_PER_DAY))


/*
*********************************************************************************************************
*                                              DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                     CLOCK ERROR CODES DATA TYPE
*********************************************************************************************************
*/

typedef  CPU_INT08U  CLK_ERR;


/*
*********************************************************************************************************
*                                    CLOCK FORMAT STRING DATA TYPE
*********************************************************************************************************
*/

typedef  CPU_INT08U  CLK_STR_FMT;


/*
*********************************************************************************************************
*                                        CLOCK DATE DATA TYPES
*********************************************************************************************************
*/

typedef  CPU_INT16U  CLK_YR;
typedef  CPU_INT08U  CLK_MONTH;
typedef  CPU_INT16U  CLK_DAY;
typedef  CPU_INT32U  CLK_NBR_DAYS;


/*
*********************************************************************************************************
*                                        CLOCK TIME DATA TYPES
*********************************************************************************************************
*/

typedef  CPU_INT08U  CLK_HR;
typedef  CPU_INT08U  CLK_MIN;
typedef  CPU_INT08U  CLK_SEC;


/*
*********************************************************************************************************
*                                      CLOCK TIMESTAMP DATA TYPE
*********************************************************************************************************
*/

typedef  CPU_INT32U  CLK_TS_SEC;


/*
*********************************************************************************************************
*                                      CLOCK TIME ZONE DATA TYPE
*********************************************************************************************************
*/

typedef  CPU_INT32S  CLK_TZ_SEC;


/*
*********************************************************************************************************
*                                CLOCK PERIODIC TICK COUNTER DATA TYPE
*********************************************************************************************************
*/

typedef  CPU_INT32U  CLK_TICK_CTR;


/*
*********************************************************************************************************
*                                     CLOCK DATE/TIME DATA TYPE
*
* Note(s) : (1) Same date/time structure is used for all epoch. Thus Year value ('Yr') should be a value
*               between the epoch start and end years.
*
*           (2) Seconds value of 60 is valid to be compatible with leap second adjustment and the atomic
*               clock time stucture.
*
*           (3) Time zone is based on Coordinated Universal Time (UTC) & has valid values :
*
*               (a) Between  +|- 12 hours (+|- 43200 seconds)
*               (b) Multiples of 15 minutes
*********************************************************************************************************
*/

typedef  struct  clk_date_time {
    CLK_YR      Yr;                                             /* Yr        [epoch start to end yr), (see Note #1).    */
    CLK_MONTH   Month;                                          /* Month     [          1 to     12], (Jan to Dec).     */
    CLK_DAY     Day;                                            /* Day       [          1 to     31].                   */
    CLK_DAY     DayOfWk;                                        /* Day of wk [          1 to      7], (Sun to Sat).     */
    CLK_DAY     DayOfYr;                                        /* Day of yr [          1 to    366].                   */
    CLK_HR      Hr;                                             /* Hr        [          0 to     23].                   */
    CLK_MIN     Min;                                            /* Min       [          0 to     59].                   */
    CLK_SEC     Sec;                                            /* Sec       [          0 to     60], (see Note #2).    */
    CLK_TZ_SEC  TZ_sec;                                         /* TZ        [     -43200 to  43200], (see Note #3).    */
} CLK_DATE_TIME;


/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void         Clk_Init               (CLK_ERR        *p_err);

#ifdef  CLK_OS_MODULE_PRESENT
void         Clk_TaskHandler        (void);

#if    (CLK_CFG_SIGNAL_EN == DEF_ENABLED)
void         Clk_SignalClk          (CLK_ERR        *p_err);
#endif
#endif


                                                                    /* --------------- CLK TS GET & SET --------------- */
CLK_TS_SEC   Clk_GetTS              (void);                         /* Get clk TS.                                      */

CPU_BOOLEAN  Clk_SetTS              (CLK_TS_SEC      ts_sec);       /* Set clk TS.                                      */



                                                                    /* --------------- CLK TZ GET & SET --------------- */
CLK_TZ_SEC   Clk_GetTZ              (void);                         /* Get clk TZ offset.                               */

CPU_BOOLEAN  Clk_SetTZ              (CLK_TZ_SEC      tz_sec);       /* Set clk TZ offset.                               */



                                                                    /* ----------- CLK TS & DATE/TIME UTIL ------------ */
CPU_BOOLEAN  Clk_GetDateTime        (CLK_DATE_TIME  *date_time);    /* Get clk TS using a CLK_DATE_TIME struct.         */

CPU_BOOLEAN  Clk_SetDateTime        (CLK_DATE_TIME  *date_time);    /* Set clk TS using a CLK_DATE_TIME struct.         */

CPU_BOOLEAN  Clk_TS_ToDateTime      (CLK_TS_SEC      ts_sec,
                                     CLK_TZ_SEC      tz_sec,
                                     CLK_DATE_TIME  *p_date_time);

CPU_BOOLEAN  Clk_DateTimeToTS       (CLK_TS_SEC     *p_ts_sec,
                                     CLK_DATE_TIME  *date_time);

CPU_BOOLEAN  Clk_DateTimeMake       (CLK_DATE_TIME  *date_time,     /* Make a date/time struct.                         */
                                     CLK_YR          yr,
                                     CLK_MONTH       month,
                                     CLK_DAY         day,
                                     CLK_HR          hr,
                                     CLK_MIN         min,
                                     CLK_SEC         sec,
                                     CLK_TZ_SEC      tz_sec);

CPU_BOOLEAN  Clk_IsDateTimeValid    (CLK_DATE_TIME  *p_date_time);


                                                                    /* ------------------ DATE UTIL ------------------- */
CLK_DAY      Clk_GetDayOfWk         (CLK_YR          yr,
                                     CLK_MONTH       month,
                                     CLK_DAY         day);

CLK_DAY      Clk_GetDayOfYr         (CLK_YR          yr,
                                     CLK_MONTH       month,
                                     CLK_DAY         day);


#if (CLK_CFG_STR_CONV_EN == DEF_ENABLED)                            /* ---------------- STR CONV UTIL ----------------- */
CPU_BOOLEAN  Clk_DateTimeToStr      (CLK_DATE_TIME  *p_date_time,   /* Conv a date/time struct to a str.                */
                                     CPU_INT08U      fmt_nbr,
                                     CPU_CHAR       *p_str,
                                     CPU_SIZE_T      str_len);
#endif



#if (CLK_CFG_NTP_EN == DEF_ENABLED)
                                                                    /* --------------- NTP TS GET & SET --------------- */
CPU_BOOLEAN  Clk_GetTS_NTP          (CLK_TS_SEC     *ts_sec_ntp);   /* Get clk TS using NTP TS.                         */

CPU_BOOLEAN  Clk_SetTS_NTP          (CLK_TS_SEC      ts_sec_ntp);   /* Set clk TS using NTP TS.                         */


                                                                    /* ----------- NTP TS & DATE/TIME UTIL ------------ */
CPU_BOOLEAN  Clk_TS_ToTS_NTP        (CLK_TS_SEC      ts_sec,
                                     CLK_TS_SEC     *p_ts_sec_ntp);

CPU_BOOLEAN  Clk_TS_NTP_ToTS        (CLK_TS_SEC     *p_ts_sec,
                                     CLK_TS_SEC      ts_sec_ntp);

CPU_BOOLEAN  Clk_TS_NTP_ToDateTime  (CLK_TS_SEC      ts_ntp_sec,
                                     CLK_TZ_SEC      tz_sec,
                                     CLK_DATE_TIME  *p_date_time);

CPU_BOOLEAN  Clk_DateTimeToTS_NTP   (CLK_TS_SEC     *p_ts_ntp_sec,
                                     CLK_DATE_TIME  *p_date_time);

CPU_BOOLEAN  Clk_NTP_DateTimeMake   (CLK_DATE_TIME  *p_date_time,
                                     CLK_YR          yr,
                                     CLK_MONTH       month,
                                     CLK_DAY         day,
                                     CLK_HR          hr,
                                     CLK_MIN         min,
                                     CLK_SEC         sec,
                                     CLK_TZ_SEC      tz_sec);

CPU_BOOLEAN  Clk_IsNTP_DateTimeValid(CLK_DATE_TIME  *p_date_time);

#endif



#if (CLK_CFG_UNIX_EN == DEF_ENABLED)
                                                                    /* -------------- UNIX TS GET & SET --------------- */
CPU_BOOLEAN  Clk_GetTS_Unix         (CLK_TS_SEC     *ts_unix_sec);  /* Get clk TS using Unix TS.                        */

CPU_BOOLEAN  Clk_SetTS_Unix         (CLK_TS_SEC      ts_unix_sec);  /* Set clk TS using Unix TS.                        */


                                                                    /* ----------- UNIX TS & DATE/TIME UTIL ----------- */
CPU_BOOLEAN  Clk_TS_ToTS_Unix       (CLK_TS_SEC      ts_sec,
                                     CLK_TS_SEC     *p_ts_unix_sec);

CPU_BOOLEAN  Clk_TS_UnixToTS        (CLK_TS_SEC     *p_ts_sec,
                                     CLK_TS_SEC      ts_unix_sec);

CPU_BOOLEAN  Clk_TS_UnixToDateTime  (CLK_TS_SEC      ts_unix_sec,
                                     CLK_TZ_SEC      tz_sec,
                                     CLK_DATE_TIME  *p_date_time);

CPU_BOOLEAN  Clk_DateTimeToTS_Unix  (CLK_TS_SEC     *p_ts_unix_sec,
                                     CLK_DATE_TIME  *p_date_time);

CPU_BOOLEAN  Clk_UnixDateTimeMake   (CLK_DATE_TIME  *p_date_time,
                                     CLK_YR          yr,
                                     CLK_MONTH       month,
                                     CLK_DAY         day,
                                     CLK_HR          hr,
                                     CLK_MIN         min,
                                     CLK_SEC         sec,
                                     CLK_TZ_SEC      tz_sec);

CPU_BOOLEAN  Clk_IsUnixDateTimeValid(CLK_DATE_TIME  *p_date_time);

#endif


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                      RTOS INTERFACE FUNCTIONS
*                                    (see specific OS'S  clk_os.c)
*********************************************************************************************************
*/

#ifdef  CLK_OS_MODULE_PRESENT
void    Clk_OS_Init  (CLK_ERR  *p_err);

void    Clk_OS_Wait  (CLK_ERR  *p_err);

void    Clk_OS_Signal(CLK_ERR  *p_err);
#endif


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                      DEFINED IN PRODUCT'S BSP
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          Clk_ExtTS_Init()
*
* Description : Initialize & start External timestamp timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : (1) CLK_ExtTS_Init() is an application/BSP function that MUST be defined by the developer
*                   if External timestamp is enabled.
*
*                   See 'clk_cfg.h  CLK CONFIGURATION  Note #1'.
*
*               (2) (a) External timestamp values MUST be returned via 'CLK_TS_SEC' data type.
*
*                   (b) External timestamp values SHOULD be returned on the epoch of Clock module and
*                       include the time zone offset.
*
*               (3) (a) External timestamp SHOULD be an 'up' counter whose values increase at each second.
*                       It's possible to use a 'down' counter, but a conversion MUST be applied when setting
*                       and getting timestamp.
*
*                   (b) External timestamp COULD come from another application (e.g. by SNTPc).
*********************************************************************************************************
*/

#if (CLK_CFG_EXT_EN == DEF_ENABLED)
void  Clk_ExtTS_Init(void);
#endif


/*
*********************************************************************************************************
*                                           Clk_ExtTS_Get()
*
* Description : Get Clock module's timestamp from converted External timestamp.
*
* Argument(s) : none.
*
* Return(s)   : Current Clock timestamp (in seconds, UTC+00).
*
* Note(s)     : (1) Clk_ExtTS_Get() is an application/BSP function that MUST be defined by the developer
*                   if External timestamp is enabled.
*
*                   See 'clk_cfg.h  CLK CONFIGURATION  Note #1'
*
*
*               (2) (a) Clock timestamp values MUST be returned via 'CLK_TS_SEC' data type.
*
*                   (b) (1) If the External timestamp has more bits than the 'CLK_TS_SEC' data type,
*                           Clk_ExtTS_Get() MUST truncate the External timestamp's higher order bits
*                           greater than the 'CLK_TS_SEC' data type.
*
*                       (2) If the External timestamp has less bits than the 'CLK_TS_SEC' data type,
*                           Clk_ExtTS_Get() MUST pad the Clock timestamp's higher order bits with
*                           0 bits.
*
*
*               (3) (a) External timestamp values MUST be returned from the reference of the Clock
*                       epoch start date/time.
*
*                   (b) External timestamp SHOULD start on midnight of January 1st of its epoch start
*                       year. Otherwise, the equations to convert between External timestamp & Clock
*                       timestamp MUST also include the External timestamp's epoch Day-of-Year, Hour,
*                       Minute, & Second (see Note #4).
*
*                   (c) Returned Clock timestamp MUST be representable in Clock epoch. Thus equivalent
*                       date of the External timestamp MUST be between :
*
*                       (1) >= CLK_EPOCH_YR_START
*                       (2) <  CLK_EPOCH_YR_END
*
*                   (d) If the External timestamp includes an (optional) external time zone,
*                       Clk_ExtTS_Get() MUST subtract the external time zone offset from the
*                       converted External timestamp.
*
*
*               (4) The Clock timestamp is calculated by one of the following equations (assuming
*                   Note #3b) :
*
*                   (a) When External epoch start year is less than Clock epoch start year
*                       ('CLK_EPOCH_YR_START') :
*
*                       Clock TS = External TS
*                                - [(((Clock start year - External start year) * 365) + leap day count)
*                                    * seconds per day]
*                                - External TZ
*
*                       Examples with a 32-bit External timestamp :
*
*                       (1)   Valid equivalent date to convert is after  Clock epoch start year :
*
*                             2010 Oct 8, 11:11:11 UTC-05:00
*                           External TS (in seconds)                                 = 1286536271
*                           External start year                                      =       1970
*                           Clock    start year                                      =       2000
*                           Leap day count between External & Clock epoch start year =          7
*                           External TZ (in seconds)                                 =     -18000
*                           Clock    TS (in seconds)                                 =  339869471
*                             2010 Oct 8, 16:11:11 UTC
*
*                           This example successfully converts an External timestamp into a
*                           representable Clock timestamp without underflowing.
*
*                       (2) Invalid equivalent date to convert is before Clock epoch start year :
*
*                             1984 Oct 8, 11:11:11 UTC-05:00
*                           External TS (in seconds)                                 =  466081871
*                           External start year                                      =       1970
*                           Clock    start year                                      =       2000
*                           Leap day count between External & Clock epoch start year =          7
*                           External TZ (in seconds)                                 =     -18000
*                           Clock    TS (in seconds)                                 = -480584929
*
*                           This example underflows to a negative Clock timestamp since the
*                           equivalent date to convert is incorrectly less than the Clock epoch
*                           start year ('CLK_EPOCH_YR_START').
*
*
*                   (b) When External epoch start year is greater than Clock epoch start year
*                       ('CLK_EPOCH_YR_START') :
*
*                       Clock TS = External TS
*                                + [(((External start year - Clock start year) * 365) + leap day count)
*                                    * seconds per day]
*                                - External TZ
*
*
*                       Examples with a 32-bit External timestamp :
*
*                       (1)   Valid equivalent date to convert is before Clock epoch end year :
*
*                             2010 Oct 8, 11:11:11 UTC-05:00
*                           External TS (in seconds)                                 =   24232271
*                           External start year                                      =       2010
*                           Clock    end   year                                      =       2136
*                           Leap day count between External & Clock epoch start year =          3
*                           External TZ (in seconds)                                 =     -18000
*                           Clock    TS (in seconds)                                 =  339869471
*                             2010 Oct 8, 16:11:11 UTC-05:00
*
*                           This example successfully converts an External timestamp into a
*                           representable Clock timestamp without overflowing.
*
*                       (2) Invalid equivalent date to convert is after  Clock epoch end year :
*
*                             2140 Oct 8, 11:11:11 UTC-05:00
*                           External TS (in seconds)                                 = 4126677071
*                           External start year                                      =       2010
*                           Clock    end   year                                      =       2136
*                           Leap day count between External & Clock epoch start year =          3
*                           External TZ (in seconds)                                 =     -18000
*                           Clock    TS (in seconds)                                 = 4442314271
*
*                           This example overflows the Clock timestamp (32-bit) 'CLK_TS_SEC' data
*                           type with an equivalent date incorrectly greater than or equal to the
*                           Clock epoch end year ('CLK_EPOCH_YR_END').
*
*
*                   (c) Where
*
*                       (1) Clock    TS             Converted Clock timestamp (in seconds,
*                                                       from UTC+00)
*                       (2) External TS             External timestamp to convert (in seconds)
*                       (3) Clock    start year     Clock epoch start year ('CLK_EPOCH_YR_START')
*                       (4) Clock    end   year     Clock epoch end   year ('CLK_EPOCH_YR_END')
*                       (5) External start year     External timestamp epoch start year
*                       (6) Leap day count          Number of leap days between Clock epoch
*                                                       start year & External epoch start year
*                       (7) Seconds per day         Number of seconds per day (86400)
*                       (8) External TZ             Time zone offset applied to External TS
*                                                       (in seconds, from UTC+00)
*********************************************************************************************************
*/

#if (CLK_CFG_EXT_EN == DEF_ENABLED)
CLK_TS_SEC  Clk_ExtTS_Get(void);
#endif


/*
*********************************************************************************************************
*                                           Clk_ExtTS_Set()
*
* Description : Set External timestamp.
*
* Argument(s) : ts_sec      Timestamp value to set (in seconds, UTC+00).
*
* Return(s)   : DEF_OK,     if External timestamp succesfully set.
*
*               DEF_FAIL,   otherwise.
*
* Note(s)     : (1) CLK_ExtTS_Set() is an application/BSP function that MUST be defined by the developer
*                   if External timestamp is enabled.
*
*                       See 'clk_cfg.h  CLK CONFIGURATION  Note #1'.
*
*                   (a) If External timestamp is provided by another application, it's possible that the
*                       External timestamp may NOT be set (e.g. by SNTPc) in which case CLK_ExtTS_Set()
*                       MUST ALWAYS return 'DEF_FAIL'.
*
*
*               (2) (a) External timestamp values are converted from Clock timestamp's 'CLK_TS_SEC'
*                       data type.
*
*                   (b) (1) If the External timestamp has more bits than the 'CLK_TS_SEC' data type,
*                           Clk_ExtTS_Set() MUST pad the External timestamp's higher order bits with
*                           0 bits.
*
*                       (2) If the External timestamp has less bits than the 'CLK_TS_SEC' data type,
*                           Clk_ExtTS_Set() MUST truncate the Clock timestamp's higher order bits
*                           greater than the External timestamp.
*
*
*               (3) (a) External timestamp values MUST be converted from the reference of the Clock
*                       epoch start date/time.
*
*                   (b) External timestamp SHOULD start on midnight of January 1st of its epoch start
*                       year. Otherwise, the equations to convert between External timestamp & Clock
*                       timestamp MUST also include the External timestamp's epoch Day-of-Year, Hour,
*                       Minute, & Second (see Note #4).
*
*                   (c) Converted External timestamp MUST be representable in External epoch. Thus
*                       equivalent date of the External timestamp MUST be between :
*
*                       (1) External epoch start year
*                       (2) External epoch end   year
*
*                   (d) If the External timestamp includes an (optional) external time zone,
*                       Clk_ExtTS_Set() MUST add the external time zone offset to the converted
*                       External timestamp.
*
*
*               (4) The External timestamp is calculated by one of the following equations (assuming
*                   Note #3b) :
*
*                   (a) When External epoch start year is less than Clock epoch start year
*                       ('CLK_EPOCH_YR_START') :
*
*                       External TS = Clock TS
*                                   + [(((Clock start year - External start year) * 365) + leap day count)
*                                       * seconds per day]
*                                   + External TZ
*
*                       Examples with a 32-bit External timestamp :
*
*                       (1)   Valid equivalent date to convert is before External epoch end year :
*
*                             2010 Oct 8, 16:11:11 UTC
*                           Clock    TS (in seconds)                                 =  339869471
*                           External start year                                      =       1970
*                           External end   year                                      =       2106
*                           Leap day count between External & Clock epoch start year =          7
*                           External TZ (in seconds)                                 =     -18000
*                           External TS (in seconds)                                 = 1286536271
*                             2010 Oct 8, 11:11:11 UTC-05:00
*
*                           This example successfully converts an External timestamp into a
*                           representable Clock timestamp without overflowing.
*
*                       (2) Invalid equivalent date to convert is after  External epoch end year :
*
*                             2120 Oct 8, 11:11:11 UTC
*                           Clock    TS (in seconds)                                 = 3811144271
*                           External start year                                      =       1970
*                           External end   year                                      =       2106
*                           Leap day count between External & Clock epoch start year =          7
*                           External TZ (in seconds)                                 =     -18000
*                           External TS (in seconds)                                 = 4757811071
*
*                           This example overflows the External (32-bit) timestamp with an equivalent
*                           date incorrectly greater than or equal to the External epoch end year.
*
*
*                   (b) When External epoch start year is greater than Clock epoch start year
*                       ('CLK_EPOCH_YR_START') :
*
*                       External TS = Clock TS
*                                   - [(((External start year - Clock start year) * 365) + leap day count)
*                                       * seconds per day]
*                                   + External TZ
*
*
*                       Examples with a 32-bit External timestamp :
*
*                       (1)   Valid equivalent date to convert is after  External epoch start year :
*
*                             2010 Oct 8, 16:11:11 UTC
*                           Clock    TS (in seconds)                                 =  339869471
*                           External start year                                      =       2010
*                           Leap day count between External & Clock epoch start year =          3
*                           External TZ (in seconds)                                 =     -18000
*                           External TS (in seconds)                                 =   24232271
*                             2010 Oct 8, 11:11:11 UTC-05:00
*
*                           This example successfully converts an External timestamp into a
*                           representable Clock timestamp without underflowing.
*
*                       (2) Invalid equivalent date to convert is before External epoch start year :
*
*                             2005 Oct 8, 11:11:11 UTC
*                           Clock    TS (in seconds)                                 =  182085071
*                           External start year                                      =       2010
*                           Leap day count between External & Clock epoch start year =          3
*                           External TZ (in seconds)                                 =     -18000
*                           External TS (in seconds)                                 = -133552129
*
*                           This example underflows to a negative External timestamp since the
*                           equivalent date to convert is incorrectly less than the External
*                           epoch start year.
*
*
*                   (c) where
*
*                       (1) Clock    TS             Clock timestamp (in seconds, from UTC+00)
*                       (2) External TS             Converted External timestamp (in seconds)
*                       (3) Clock    start year     Clock epoch start year ('CLK_EPOCH_YR_START')
*                       (4) External start year     External timestamp epoch start year
*                       (5) External end   year     External timestamp epoch end   year
*                       (6) Leap day count          Number of leap days between Clock epoch
*                                                       start year & External epoch start year
*                       (7) Seconds per day         Number of seconds per day (86400)
*                       (8) External TZ             Time zone offset applied to External TS
*                                                       (in seconds, from UTC+00)
*********************************************************************************************************
*/

#if (CLK_CFG_EXT_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_ExtTS_Set(CLK_TS_SEC  ts_sec);
#endif


/*
*********************************************************************************************************
*                                              TRACING
*********************************************************************************************************
*/

                                                                /* Trace level, default to TRACE_LEVEL_OFF.             */
#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                                   0
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                                  1
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                                   2
#endif

#ifndef  CLK_TRACE_LEVEL
#define  CLK_TRACE_LEVEL                                   TRACE_LEVEL_OFF
#endif

#ifndef  CLK_TRACE
#define  CLK_TRACE                                         printf
#endif

#if ((defined(CLK_TRACE))       && \
     (defined(CLK_TRACE_LEVEL)) && \
     (CLK_TRACE_LEVEL >= TRACE_LEVEL_INFO))

    #if (CLK_TRACE_LEVEL >= TRACE_LEVEL_LOG)
        #define  CLK_TRACE_LOG(msg)     CLK_TRACE  msg
    #else
        #define  CLK_TRACE_LOG(msg)
    #endif


    #if (CLK_TRACE_LEVEL >= TRACE_LEVEL_DBG)
        #define  CLK_TRACE_DBG(msg)     CLK_TRACE  msg
    #else
        #define  CLK_TRACE_DBG(msg)
    #endif

    #define  CLK_TRACE_INFO(msg)        CLK_TRACE  msg

#else
    #define  CLK_TRACE_LOG(msg)
    #define  CLK_TRACE_DBG(msg)
    #define  CLK_TRACE_INFO(msg)
#endif


/*
*********************************************************************************************************
*                                         CONFIGURATION ERRORS
*********************************************************************************************************
*/

#ifndef  CLK_CFG_ARG_CHK_EN
#error  "CLK_CFG_ARG_CHK_EN                            not #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  DEF_DISABLED]           "
#error  "                                        [     ||  DEF_ENABLED ]           "
#elif  ((CLK_CFG_ARG_CHK_EN != DEF_ENABLED ) &&  \
        (CLK_CFG_ARG_CHK_EN != DEF_DISABLED))

#error  "CLK_CFG_ARG_CHK_EN                      illegally #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  DEF_DISABLED]           "
#error  "                                        [     ||  DEF_ENABLED ]           "
#endif



#ifndef  CLK_CFG_STR_CONV_EN
#error  "CLK_CFG_STR_CONV_EN                           not #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  DEF_DISABLED]           "
#error  "                                        [     ||  DEF_ENABLED ]           "
#elif  ((CLK_CFG_STR_CONV_EN != DEF_ENABLED ) &&  \
        (CLK_CFG_STR_CONV_EN != DEF_DISABLED))
#error  "CLK_CFG_STR_CONV_EN                     illegally #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  DEF_DISABLED]           "
#error  "                                        [     ||  DEF_ENABLED ]           "
#endif




#ifndef  CLK_CFG_NTP_EN
#error  "CLK_CFG_NTP_EN                                not #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  DEF_DISABLED]           "
#error  "                                        [     ||  DEF_ENABLED ]           "
#elif  ((CLK_CFG_NTP_EN != DEF_ENABLED ) &&  \
        (CLK_CFG_NTP_EN != DEF_DISABLED))
#error  "CLK_CFG_NTP_EN                          illegally #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  DEF_DISABLED]           "
#error  "                                        [     ||  DEF_ENABLED ]           "
#endif



#ifndef  CLK_CFG_UNIX_EN
#error  "CLK_CFG_UNIX_EN                               not #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  DEF_DISABLED]           "
#error  "                                        [     ||  DEF_ENABLED ]           "
#elif  ((CLK_CFG_UNIX_EN != DEF_ENABLED ) &&  \
        (CLK_CFG_UNIX_EN != DEF_DISABLED))
#error  "CLK_CFG_UNIX_EN                         illegally #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  DEF_DISABLED]           "
#error  "                                        [     ||  DEF_ENABLED ]           "
#endif




#ifndef  CLK_CFG_EXT_EN
#error  "CLK_CFG_EXT_EN                                not #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  DEF_DISABLED]           "
#error  "                                        [     ||  DEF_ENABLED ]           "

#elif  ((CLK_CFG_EXT_EN != DEF_ENABLED ) &&  \
        (CLK_CFG_EXT_EN != DEF_DISABLED))
#error  "CLK_CFG_EXT_EN                          illegally #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  DEF_DISABLED]           "
#error  "                                        [     ||  DEF_ENABLED ]           "


#elif   (CLK_CFG_EXT_EN != DEF_ENABLED)

#ifndef  CLK_CFG_SIGNAL_EN
#error  "CLK_CFG_SIGNAL_EN                             not #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  DEF_DISABLED]           "
#error  "                                        [     ||  DEF_ENABLED ]           "


#elif  ((CLK_CFG_SIGNAL_EN != DEF_ENABLED ) &&  \
        (CLK_CFG_SIGNAL_EN != DEF_DISABLED))
#error  "CLK_CFG_SIGNAL_EN                       illegally #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  DEF_DISABLED]           "
#error  "                                        [     ||  DEF_ENABLED ]           "


#elif   (CLK_CFG_SIGNAL_EN == DEF_ENABLED)

#ifndef  CLK_CFG_SIGNAL_FREQ_HZ
#error  "CLK_CFG_SIGNAL_FREQ_HZ                        not #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  > 0]                    "
#elif   (CLK_CFG_SIGNAL_FREQ_HZ < 1)
#error  "CLK_CFG_SIGNAL_FREQ_HZ                  illegally #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  > 0]                    "
#endif

#endif

#endif




#ifndef  CLK_CFG_TZ_DFLT_SEC
#error  "CLK_CFG_TZ_DFLT_SEC                           not #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  >= CLK_TZ_SEC_MIN               ]"
#error  "                                        [     &&  <= CLK_TZ_SEC_MAX               ]"
#error  "                                        [     &&  multiple of CLK_TZ_SEC_PRECISION]"
#elif  (( DEF_ABS(CLK_CFG_TZ_DFLT_SEC) > CLK_TZ_SEC_MAX) ||  \
        ((DEF_ABS(CLK_CFG_TZ_DFLT_SEC) % CLK_TZ_SEC_PRECISION) != 0u))
#error  "CLK_CFG_TZ_DFLT_SEC                     illegally #define'd in 'clk_cfg.h'"
#error  "                                        [MUST be  >= CLK_TZ_SEC_MIN               ]"
#error  "                                        [     &&  <= CLK_TZ_SEC_MAX               ]"
#error  "                                        [     &&  multiple of CLK_TZ_SEC_PRECISION]"

#endif


/*
*********************************************************************************************************
*                                    LIBRARY CONFIGURATION ERRORS
*********************************************************************************************************
*/

                                                                /* See 'clk.h  Note #1a'.                               */
#if     (CPU_CORE_VERSION < 125u)
#error  "CPU_CORE_VERSION  [SHOULD be >= V1.25]"
#endif


                                                                /* See 'clk.h  Note #1b'.                               */
#if     (LIB_VERSION < 129u)
#error  "LIB_VERSION       [SHOULD be >= V1.29]"
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*
* Note(s) : (1) See 'clk.h  MODULE'.
*********************************************************************************************************
*/

#endif                                                          /* End of CLK module include (see 'MODULE  Note #1').   */

