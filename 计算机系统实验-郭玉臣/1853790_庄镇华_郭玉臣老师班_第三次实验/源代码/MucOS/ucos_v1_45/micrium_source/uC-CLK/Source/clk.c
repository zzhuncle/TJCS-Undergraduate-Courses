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
* Filename : clk.c
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
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define    MICRIUM_SOURCE
#define    CLK_MODULE
#include  "clk.h"


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
*                                            LOCAL TABLES
*********************************************************************************************************
*/

static  const  CLK_DAY  Clk_DaysInYr[2u] = {
    DEF_TIME_NBR_DAY_PER_YR, DEF_TIME_NBR_DAY_PER_YR_LEAP
};

static  const  CLK_DAY  Clk_DaysInMonth[2u][CLK_MONTH_PER_YR] = {
  /* Jan  Feb  Mar  Apr  May  Jun  Jul  Aug  Sep  Oct  Nov  Dec */
   { 31u, 28u, 31u, 30u, 31u, 30u, 31u, 31u, 30u, 31u, 30u, 31u },
   { 31u, 29u, 31u, 30u, 31u, 30u, 31u, 31u, 30u, 31u, 30u, 31u }
};

#if (CLK_CFG_STR_CONV_EN == DEF_ENABLED)
static  const  CPU_CHAR *  const  Clk_StrMonth[CLK_MONTH_PER_YR] = {
                    /*           1 */
                    /* 01234567890 */
   (const  CPU_CHAR *)"January",
   (const  CPU_CHAR *)"February",
   (const  CPU_CHAR *)"March",
   (const  CPU_CHAR *)"April",
   (const  CPU_CHAR *)"May",
   (const  CPU_CHAR *)"June",
   (const  CPU_CHAR *)"July",
   (const  CPU_CHAR *)"August",
   (const  CPU_CHAR *)"September",
   (const  CPU_CHAR *)"October",
   (const  CPU_CHAR *)"November",
   (const  CPU_CHAR *)"December"
};

static  const  CPU_CHAR *  const  Clk_StrDayOfWk[DEF_TIME_NBR_DAY_PER_WK] = {
                    /*           1 */
                    /* 01234567890 */
   (const  CPU_CHAR *)"Sunday",
   (const  CPU_CHAR *)"Monday",
   (const  CPU_CHAR *)"Tuesday",
   (const  CPU_CHAR *)"Wednesday",
   (const  CPU_CHAR *)"Thursday",
   (const  CPU_CHAR *)"Friday",
   (const  CPU_CHAR *)"Saturday"
};
#endif


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

#if    (CLK_CFG_EXT_EN    != DEF_ENABLED)
static  CLK_TS_SEC    Clk_TS_UTC_sec;
#if    (CLK_CFG_SIGNAL_EN == DEF_ENABLED)
static  CLK_TICK_CTR  Clk_TickCtr;
#endif
#endif
static  CLK_TZ_SEC    Clk_TZ_sec;

static  CLK_YR        Clk_CacheYr;
static  CLK_MONTH     Clk_CacheMonth;
static  CLK_NBR_DAYS  Clk_CacheYrDays;
static  CLK_DAY       Clk_CacheMonthDays;
static  CPU_INT08U    Clk_CacheLeapYr;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTIONS PROTOTYPES
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Clk_IsLeapYr              (CLK_YR          yr);


static  CPU_BOOLEAN  Clk_IsDateValid           (CLK_YR          yr,
                                                CLK_MONTH       month,
                                                CLK_DAY         day,
                                                CLK_YR          yr_start,
                                                CLK_YR          yr_end);


static  CPU_BOOLEAN  Clk_IsDayOfYrValid        (CLK_YR          yr,
                                                CLK_DAY         day_of_yr);

static  CPU_BOOLEAN  Clk_IsDayOfWkValid        (CLK_DAY         day_of_wk);


static  CPU_BOOLEAN  Clk_IsTimeValid           (CLK_HR          hr,
                                                CLK_MONTH       min,
                                                CLK_DAY         sec);

static  CPU_BOOLEAN  Clk_IsTZValid             (CLK_TZ_SEC      tz_sec);


static  CPU_BOOLEAN  Clk_IsDateTimeValidHandler(CLK_DATE_TIME  *p_date_time,
                                                CLK_YR          yr_start,
                                                CLK_YR          yr_end);



static  CLK_DAY      Clk_GetDayOfYrHandler     (CLK_YR          yr,
                                                CLK_MONTH       month,
                                                CLK_DAY         day);

static  CLK_DAY      Clk_GetDayOfWkHandler     (CLK_YR          yr,
                                                CLK_MONTH       month,
                                                CLK_DAY         day);


static  void         Clk_SetTZ_Handler         (CLK_TZ_SEC      tz_sec);


static  CPU_BOOLEAN  Clk_TS_ToDateTimeHandler  (CLK_TS_SEC      ts_sec,
                                                CLK_TZ_SEC      tz_sec,
                                                CLK_DATE_TIME  *p_date_time,
                                                CLK_YR          yr_start,
                                                CLK_YR          yr_end);

static  CPU_BOOLEAN  Clk_DateTimeToTS_Handler  (CLK_TS_SEC     *p_ts_sec,
                                                CLK_DATE_TIME  *p_date_time,
                                                CLK_YR          yr_start);

static  CPU_BOOLEAN  Clk_DateTimeMakeHandler   (CLK_DATE_TIME  *p_date_time,
                                                CLK_YR          yr,
                                                CLK_MONTH       month,
                                                CLK_DAY         day,
                                                CLK_HR          hr,
                                                CLK_MIN         min,
                                                CLK_SEC         sec,
                                                CLK_TZ_SEC      tz_sec,
                                                CLK_YR          yr_start,
                                                CLK_YR          yr_end);


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             Clk_Init()
*
* Description : (1) Initialize Clock module :
*
*                   (a) Initialize clock :
*                       (1) External timestamp, if available
*                       (2) Clock/operating system initialization
*
*                   (b) Initialize Clock module variables
*
*
* Argument(s) : p_err       Pointer to variable that will receive the return error code from this function :
*
*                               CLK_ERR_NONE                    Clock successfully initialized.
*
*                                                               -------- RETURNED BY Clk_OS_Init() : ---------
*                               CLK_OS_ERR_INIT_SIGNAL          Clock OS signal  NOT successfully initialized.
*                               CLK_OS_ERR_INIT_TASK            Clock OS task    NOT successfully initialized.
*                               CLK_OS_ERR_INIT_NAME            Clock OS name(s) NOT successfully initialized.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  Clk_Init (CLK_ERR  *p_err)
{
#if (CLK_CFG_ARG_CHK_EN == DEF_ENABLED)                         /* ----------------- VALIDATE ERR PTR ----------------- */
    if (p_err == (CLK_ERR *)0) {
        return;
    }
#endif

#if (CLK_CFG_EXT_EN == DEF_ENABLED)                             /* ------------------- INIT EXT TS -------------------- */
    Clk_ExtTS_Init();

#else                                                           /* ------------------- CLK/OS INIT -------------------- */
    Clk_OS_Init(p_err);
    if (*p_err != CLK_OS_ERR_NONE) {
         return;
    }
                                                                /* ---------------- INIT CLK VARIABLES ---------------- */
    Clk_TS_UTC_sec = CLK_TS_SEC_NONE;                           /* Clk epoch = 2000-01-01 00:00:00 UTC                  */
#if (CLK_CFG_SIGNAL_EN == DEF_ENABLED)
    Clk_TickCtr         = CLK_TICK_NONE;
#endif
#endif
    Clk_TZ_sec          = CLK_CFG_TZ_DFLT_SEC;                  /* Clk TZ = UTC offset                                  */
    Clk_CacheMonth      = CLK_MONTH_NONE;
    Clk_CacheMonthDays  = CLK_DAY_NONE;
    Clk_CacheYr         = CLK_YR_NONE;
    Clk_CacheYrDays     = CLK_DAY_NONE;
    Clk_CacheLeapYr     = 0u;


   *p_err = CLK_ERR_NONE;
}


/*
*********************************************************************************************************
*                                          Clk_TaskHandler()
*
* Description : (1) Handle Clock Signal :
*
*                  (a) Wait for a signal
*                  (b) Update timestamp
*
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : (1) 'Clk_TS_UTC_sec' MUST ALWAYS be accessed exclusively in critical sections.
*********************************************************************************************************
*/

#if (CLK_CFG_EXT_EN != DEF_ENABLED)
void  Clk_TaskHandler (void)
{
    CLK_ERR  err;
    CPU_SR_ALLOC();


    while (DEF_ON) {
                                                                /* --------------- WAIT FOR CLK SIGNAL ---------------- */
        do {
            Clk_OS_Wait(&err);
        } while (err != CLK_OS_ERR_NONE);

                                                                /* -------------------- UPDATE TS --------------------- */
        CPU_CRITICAL_ENTER();
        if (Clk_TS_UTC_sec < CLK_TS_SEC_MAX) {
            Clk_TS_UTC_sec++;
        }
        CPU_CRITICAL_EXIT();
    }
}
#endif


/*
*********************************************************************************************************
*                                           Clk_SignalClk()
*
* Description : (1) Increment the clock tick counter :
*
*                   (a) Increment the clock tick counter
*                   (b) Signal the clock task when one second has elapsed
*
*
* Argument(s) : p_err       Pointer to variable that will receive the return error code from this function :
*
*                               CLK_ERR_NONE                    Clock     successfully signaled.
*
*                                                               - RETURNED BY Clk_OS_Signal() : -
*                               CLK_OS_ERR_SIGNAL               Clock NOT successfully signaled.
*
* Return(s)   : none.
*
* Note(s)     : (1) CLK_CFG_SIGNAL_FREQ_HZ must be set to the number of times Clk_SignalClk() will be
*                   called every second.
*
*               (2) 'Clk_TickCtr' MUST ALWAYS be accessed exclusively in critical sections.
*********************************************************************************************************
*/

#if ((CLK_CFG_EXT_EN    != DEF_ENABLED) &&  \
     (CLK_CFG_SIGNAL_EN == DEF_ENABLED))
void  Clk_SignalClk (CLK_ERR  *p_err)
{
    CPU_BOOLEAN  signal;
    CPU_SR_ALLOC();


    signal = DEF_NO;
    CPU_CRITICAL_ENTER();                                       /* See Note #2.                                         */
    Clk_TickCtr++;
    if (Clk_TickCtr >= CLK_CFG_SIGNAL_FREQ_HZ) {
        Clk_TickCtr -= CLK_CFG_SIGNAL_FREQ_HZ;                  /* See Note #1.                                         */
        signal       = DEF_YES;
    }
    CPU_CRITICAL_EXIT();

    if (signal == DEF_YES) {
        Clk_OS_Signal(p_err);
        if (*p_err != CLK_OS_ERR_NONE) {
             return;
        }
    }

   *p_err = CLK_ERR_NONE;
}
#endif


/*
*********************************************************************************************************
*                                             Clk_GetTS()
*
* Description : Get current Clock timestamp.
*
* Argument(s) : none.
*
* Return(s)   : Current timestamp (in seconds, UTC+00).
*
* Note(s)     : (1) Internal Clock timestamp ('Clk_TS_UTC_sec') SHOULD be set for UTC+00. Thus any
*                   local time zone offset MUST be applied after calling Clk_GetTS().
*
*               (2) 'Clk_TS_UTC_sec' MUST ALWAYS be accessed exclusively in critical sections.
*********************************************************************************************************
*/

CLK_TS_SEC  Clk_GetTS (void)
{
    CLK_TS_SEC  ts_sec;
#if (CLK_CFG_EXT_EN != DEF_ENABLED)
    CPU_SR_ALLOC();

                                                                /* ---------------------- GET TS ---------------------- */
    CPU_CRITICAL_ENTER();
    ts_sec = Clk_TS_UTC_sec;
    CPU_CRITICAL_EXIT();
#else
    ts_sec = Clk_ExtTS_Get();
#endif

    return (ts_sec);
}


/*
*********************************************************************************************************
*                                             Clk_SetTS()
*
* Description : Set Clock timestamp.
*
* Argument(s) : ts_sec      Current timestamp to set (in seconds, UTC+00).
*
* Return(s)   : DEF_OK,   if timestamp is successfully set.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Internal Clock timestamp ('Clk_TS_UTC_sec') SHOULD be set for UTC+00 and should
*                   NOT include any local time zone offset.
*
*               (2) 'Clk_TS_UTC_sec' MUST ALWAYS be accessed exclusively in critical sections.
*********************************************************************************************************
*/

CPU_BOOLEAN  Clk_SetTS (CLK_TS_SEC  ts_sec)
{
     CPU_BOOLEAN  valid;
#if (CLK_CFG_EXT_EN != DEF_ENABLED)
    CPU_SR_ALLOC();

                                                                /* ---------------------- SET TS ---------------------- */
    CPU_CRITICAL_ENTER();
    Clk_TS_UTC_sec = ts_sec;
    CPU_CRITICAL_EXIT();

    valid          = DEF_OK;
#else
    valid          = Clk_ExtTS_Set(ts_sec);
#endif

    return (valid);
}


/*
*********************************************************************************************************
*                                             Clk_GetTZ()
*
* Description : Get Clock time zone offset.
*
* Argument(s) : none.
*
* Return(s)   : Time zone offset (in seconds, +|- from UTC).
*
* Note(s)     : (1) Time zone is based on Coordinated Universal Time (UTC) & has valid values :
*
*                   (a) Between  +|- 12 hours (+|- 43200 seconds)
*                   (b) Multiples of 15 minutes
*
*               (2) 'Clk_TZ_sec' MUST ALWAYS be accessed exclusively in critical sections.
*********************************************************************************************************
*/

CLK_TZ_SEC  Clk_GetTZ (void)
{
    CLK_TZ_SEC  tz_sec;
    CPU_SR_ALLOC();

                                                                /* ---------------------- GET TZ ---------------------- */
    CPU_CRITICAL_ENTER();
    tz_sec = Clk_TZ_sec;
    CPU_CRITICAL_EXIT();

    return (tz_sec);
}


/*
*********************************************************************************************************
*                                             Clk_SetTZ()
*
* Description : Set Clock time zone offset.
*
* Argument(s) : tz_sec      Time zone offset (in seconds, +|- from UTC).
*
* Return(s)   : DEF_OK,   if time zone is valid and set.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Time zone is based on Coordinated Universal Time (UTC) & has valid values :
*
*                   (a) Between  +|- 12 hours (+|- 43200 seconds)
*                   (b) Multiples of 15 minutes
*
*               (2) Time zone is not applied on the current Clock timestamp.
*********************************************************************************************************
*/

CPU_BOOLEAN  Clk_SetTZ (CLK_TZ_SEC  tz_sec)
{
    CPU_BOOLEAN  valid;

                                                                /* ------------------- VALIDATE TZ -------------------- */
    valid = Clk_IsTZValid(tz_sec);
    if (valid != DEF_YES) {
        return  (DEF_FAIL);
    }

                                                                /* ---------------------- SET TZ ---------------------- */
    Clk_SetTZ_Handler(tz_sec);

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                          Clk_GetDateTime()
*
* Description : Get current Clock timestamp as a date/time structure.
*
* Argument(s) : p_date_time     Pointer to variable that will receive the date/time structure.
*
* Return(s)   : DEF_OK,   if timestamp is valid & date/time structure successfully returned.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Internal Clock time zone offset ('Clk_TZ_sec') is used to calculate the
*                   local date/time ('p_date_time') returned. Thus local date/time is returned
*                   at UTC+TZ, where Clock time zone offset (TZ) is returned as local time zone
*                   offset ('p_date_time->TZ_sec').
*********************************************************************************************************
*/

CPU_BOOLEAN  Clk_GetDateTime (CLK_DATE_TIME  *p_date_time)
{
    CLK_TS_SEC   ts_sec;
    CLK_TZ_SEC   tz_sec;
    CPU_BOOLEAN  valid;


#if 0                                                           /* Validated in Clk_TS_ToDateTime().                    */
    if (p_date_time == (CLK_DATE_TIME *)0) {                    /* -------------- VALIDATE DATE/TIME PTR -------------- */
        return (DEF_FAIL);
    }
#endif


    ts_sec = Clk_GetTS();                                       /* ---------------------- GET TS ---------------------- */
    tz_sec = Clk_GetTZ();                                       /* ---------------------- GET TZ ---------------------- */

                                                                /* ------------- CONV CLK TS TO DATE/TIME ------------- */
    valid  = Clk_TS_ToDateTime(ts_sec,
                               tz_sec,
                               p_date_time);
    if (valid != DEF_OK) {
        return  (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                          Clk_SetDateTime()
*
* Description : Set Clock timestamp from a date/time structure.
*
* Argument(s) : p_date_time     Pointer to variable that contains the date/time structure.
*
* Return(s)   : DEF_OK,   if Clock date/time successfully set.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) (a) Date/time ('p_date_time') SHOULD be set to local time with correct time zone
*                       offset ('p_date_time->TZ_sec'). Clk_SetDateTime() removes the time zone offset
*                       from the date/time to calculate the Clock timestamp at UTC+00.
*
*                   (b) Internal Clock time zone offset ('Clk_TZ_sec') is set to the local time zone
*                       offset ('p_date_time->TZ_sec').
*
*                   (c) Time zone is based on Coordinated Universal Time (UTC) & has valid values :
*
*                       (1) Between  +|- 12 hours (+|- 43200 seconds)
*                       (2) Multiples of 15 minutes
*********************************************************************************************************
*/

CPU_BOOLEAN  Clk_SetDateTime (CLK_DATE_TIME  *p_date_time)
{
    CLK_TS_SEC   ts_sec;
    CPU_BOOLEAN  valid;


#if 0                                                           /* Validated in Clk_DateTimeToTS().                     */
    if (p_date_time == (CLK_DATE_TIME *)0) {                    /* -------------- VALIDATE DATE/TIME PTR -------------- */
        return (DEF_FAIL);
    }
#endif

                                                                /* ------------- CONV DATE/TIME TO CLK TS ------------- */
    valid = Clk_DateTimeToTS(&ts_sec, p_date_time);             /* Validates date/time & TZ (see Note #1c).             */
    if (valid != DEF_OK) {
        CLK_TRACE_DBG(("Date/time is not valid"));
        return  (DEF_FAIL);
    }

                                                                /* ---------------------- SET TS ---------------------- */
    valid = Clk_SetTS(ts_sec);                                  /* See Note #1a.                                        */
    if (valid != DEF_OK) {
        return  (DEF_FAIL);
    }

                                                                /* ---------------------- SET TZ ---------------------- */
    Clk_SetTZ_Handler(p_date_time->TZ_sec);                     /* See Note #1b.                                        */

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                         Clk_TS_ToDateTime()
*
* Description : Convert Clock timestamp to date/time structure.
*
* Argument(s) : ts_sec          Timestamp to convert (in seconds,          UTC+00).
*
*               tz_sec          Time zone offset     (in seconds, +|- from UTC).
*
*               p_date_time     Pointer to variable that will receive the date/time structure.
*
* Return(s)   : DEF_OK,   if date/time structure successfully returned.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) (a) Timestamp ('ts_sec') MUST be set for UTC+00 & SHOULD NOT include the time
*                       zone offset ('tz_sec') since Clk_TS_ToDateTime() includes the time zone
*                       offset in its date/time calculation. Thus the time zone offset SHOULD NOT
*                       be applied before or after calling Clk_TS_ToDateTime().
*
*                   (b) Time zone field of the date/time structure ('p_date_time->TZ_sec') is set
*                       to the value of the time zone argument ('tz_sec').
*
*                   (c) Time zone is based on Coordinated Universal Time (UTC) & has valid values :
*
*                       (1) Between  +|- 12 hours (+|- 43200 seconds)
*                       (2) Multiples of 15 minutes
*********************************************************************************************************
*/

CPU_BOOLEAN  Clk_TS_ToDateTime (CLK_TS_SEC      ts_sec,
                                CLK_TZ_SEC      tz_sec,
                                CLK_DATE_TIME  *p_date_time)
{
    CPU_BOOLEAN  valid;

                                                                /* ------------- CONV CLK TS TO DATE/TIME ------------- */
    CLK_TRACE_DBG(("\n\rConvert TS to Date/time:\n\r"
                   "    TS to convert= %u\n\n\r",
                   (unsigned int)ts_sec));

    valid = Clk_TS_ToDateTimeHandler(ts_sec,
                                     tz_sec,
                                     p_date_time,
                                     CLK_EPOCH_YR_START,
                                     CLK_EPOCH_YR_END);
    if (valid != DEF_OK) {
        CLK_TRACE_DBG(("    Date/time conversion has failed\n\r"));
        return  (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                         Clk_DateTimeToTS()
*
* Description : Convert date/time structure to Clock timestamp.
*
* Argument(s) : p_ts_sec        Pointer to variable that will receive the Clock timestamp :
*
*                                   In seconds UTC+00,  if NO error(s);
*                                   CLK_TS_SEC_NONE,    otherwise.
*
*               p_date_time     Pointer to variable that contains date/time structure to convert.
*
* Return(s)   : DEF_OK,   if timestamp successfully returned.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Date/time structure ('p_date_time') MUST be representable in Clock timestamp.
*                   Thus date to convert MUST be :
*
*                   (a) >= CLK_EPOCH_YR_START
*                   (b) <  CLK_EPOCH_YR_END
*
*               (2) (a) Date/time ('p_date_time') SHOULD be set to local time with correct time zone
*                       offset ('p_date_time->TZ_sec'). Clk_DateTimeToTS() removes the time zone
*                       offset from the date/time to calculate & return a Clock timestamp at UTC+00.
*
*                   (b) Time zone is based on Coordinated Universal Time (UTC) & has valid values :
*
*                       (1) Between  +|- 12 hours (+|- 43200 seconds)
*                       (2) Multiples of 15 minutes
*
*               (3) Pointers to variables that return values MUST be initialized PRIOR to all other
*                   validation or function handling in case of any error(s).
*********************************************************************************************************
*/

CPU_BOOLEAN  Clk_DateTimeToTS (CLK_TS_SEC     *p_ts_sec,
                               CLK_DATE_TIME  *p_date_time)
{
    CPU_BOOLEAN  valid;


#if (CLK_CFG_ARG_CHK_EN == DEF_ENABLED)                         /* ----------------- VALIDATE TS PTR ------------------ */
    if (p_ts_sec == (CLK_TS_SEC *)0) {
        return (DEF_FAIL);
    }
#endif

   *p_ts_sec = CLK_TS_SEC_NONE;                                 /* Init to ts none for err (see Note #3).               */


#if 0                                                           /* Validated in Clk_IsDateTimeValid().                  */
    if (p_date_time == (CLK_DATE_TIME *)0) {                    /* -------------- VALIDATE DATE/TIME PTR -------------- */
        return (DEF_FAIL);
    }
#endif

                                                                /* ---------------- VALIDATE DATE/TIME ---------------- */
    valid = Clk_IsDateTimeValid(p_date_time);
    if (valid != DEF_OK) {
        return  (DEF_FAIL);
    }

                                                                /* ------------- CONV DATE/TIME TO CLK TS ------------- */
    CLK_TRACE_DBG(("Convert Date/time to TS:\n\r"));
    valid = Clk_DateTimeToTS_Handler(p_ts_sec,
                                     p_date_time,
                                     CLK_EPOCH_YR_START);
    if (valid != DEF_OK) {
        return  (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                         Clk_DateTimeMake()
*
* Description : Build a valid Clock epoch date/time structure.
*
* Argument(s) : p_date_time     Pointer to variable that will receive the date/time structure.
*
*               yr              Year    value [CLK_EPOCH_YR_START to CLK_EPOCH_YR_END) [see Note #1].
*
*               month           Month   value [     CLK_MONTH_JAN to    CLK_MONTH_DEC].
*
*               day             Day     value [                 1 to               31].
*
*               hr              Hours   value [                 0 to               23].
*
*               min             Minutes value [                 0 to               59].
*
*               sec             Seconds value [                 0 to               60] (see Note #3).
*
*               tz_sec          Time zone offset (in seconds, +|- from UTC) [-43200 to 43200].
*
* Return(s)   : DEF_OK,   if date/time structure successfully returned.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Date/time structure ('p_date_time') MUST be representable in Clock timestamp.
*                   Thus date to convert MUST be :
*
*                   (a) >= CLK_EPOCH_YR_START
*                   (b) <  CLK_EPOCH_YR_END
*
*               (2) Day of week ('p_date_time->DayOfWk') and Day of year ('p_date_time->DayOfYr')
*                   are internally calculated and set in the date/time structure if date is valid.
*
*               (3) Seconds value of 60 is valid to be compatible with leap second adjustment and
*                   the atomic clock time structure.
*
*               (4) Time zone is based ('tz_sec') on Coordinated Universal Time (UTC) & has valid
*                   values :
*
*                   (a) Between  +|- 12 hours (+|- 43200 seconds)
*                   (b) Multiples of 15 minutes
*********************************************************************************************************
*/

CPU_BOOLEAN  Clk_DateTimeMake (CLK_DATE_TIME  *p_date_time,
                               CLK_YR          yr,
                               CLK_MONTH       month,
                               CLK_DAY         day,
                               CLK_HR          hr,
                               CLK_MIN         min,
                               CLK_SEC         sec,
                               CLK_TZ_SEC      tz_sec)
{
    CPU_BOOLEAN  valid;

                                                                /* ---------- VALIDATE & CONV CLK DATE/TIME ----------- */
    valid = Clk_DateTimeMakeHandler(p_date_time,
                                    yr,
                                    month,
                                    day,
                                    hr,
                                    min,
                                    sec,
                                    tz_sec,
                                    CLK_EPOCH_YR_START,
                                    CLK_EPOCH_YR_END);
    if (valid != DEF_YES) {
        return  (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                       Clk_IsDateTimeValid()
*
* Description : Determine if date/time structure is valid in Clock epoch.
*
* Argument(s) : p_date_time     Pointer to variable that contains the date/time structure to validate.
*
* Return(s)   : DEF_YES, if date/time structure is valid.
*
*               DEF_NO,  otherwise.
*
* Note(s)     : (1) Date/time structure ('p_date_time') MUST be representable in Clock epoch. Thus
*                   date to validate MUST be :
*
*                   (a) >= CLK_EPOCH_YR_START
*                   (b) <  CLK_EPOCH_YR_END
*
*               (2) Time zone is based ('p_date_time->TZ_sec') on Coordinated Universal Time (UTC)
*                   & has valid values :
*
*                   (a) Between  +|- 12 hours (+|- 43200 seconds)
*                   (b) Multiples of 15 minutes
*********************************************************************************************************
*/

CPU_BOOLEAN  Clk_IsDateTimeValid (CLK_DATE_TIME  *p_date_time)
{
    CPU_BOOLEAN  valid;

                                                                /* -------------- VALIDATE CLK DATE/TIME -------------- */
    CLK_TRACE_DBG(("Validate Clock date/time: "));

    valid = Clk_IsDateTimeValidHandler(p_date_time,
                                       CLK_EPOCH_YR_START,
                                       CLK_EPOCH_YR_END);
    if (valid != DEF_YES) {
        CLK_TRACE_DBG(("Fail\n\r"));
        return  (DEF_NO);
    }

    CLK_TRACE_DBG(("Ok\n\r"));

    return (DEF_YES);
}


/*
*********************************************************************************************************
*                                          Clk_GetDayOfWk()
*
* Description : Get the day of week.
*
* Argument(s) : yr          Year  value [1900 to 2135] (see Note #1).
*
*               month       Month value [   1 to   12] / [CLK_MONTH_JAN to CLK_MONTH_DEC].
*
*               day         Day   value [   1 to   31].
*
* Return(s)   : Day of week [1 to 7] / [CLK_DAY_OF_WK_SUN to CLK_DAY_OF_WK_SAT], if NO error(s).
*
*               CLK_DAY_OF_WK_NONE,                                              otherwise.
*
* Note(s)     : (1) It's only possible to get a day of week of an epoch supported by Clock :
*
*                   (a) Earliest year is the NTP   epoch start year, thus Year ('yr') MUST be greater
*                       than or equal to 'CLK_NTP_EPOCH_YR_START'.
*
*                   (b) Latest   year is the Clock epoch end   year, thus Year ('yr') MUST be less
*                       than 'CLK_EPOCH_YR_END'.
*********************************************************************************************************
*/

CLK_DAY  Clk_GetDayOfWk (CLK_YR     yr,
                         CLK_MONTH  month,
                         CLK_DAY    day)
{
    CLK_DAY      day_of_wk;
    CPU_BOOLEAN  valid;

                                                                /* ------------------ VALIDATE DATE ------------------- */
    valid = Clk_IsDateValid(yr,
                            month,
                            day,
                            CLK_NTP_EPOCH_YR_START,
                            CLK_EPOCH_YR_END);
    if (valid != DEF_YES) {
        return (CLK_DAY_OF_WK_NONE);
    }

                                                                /* ------------------ GET DAY OF WK ------------------- */
    day_of_wk = Clk_GetDayOfWkHandler(yr, month, day);

    return (day_of_wk);
}


/*
*********************************************************************************************************
*                                          Clk_GetDayOfYr()
*
* Description : Get the day of year.
*
* Argument(s) : yr          Year  value [1900 to 2135] (see Note #1).
*
*               month       Month value [   1 to   12] / [CLK_MONTH_JAN to CLK_MONTH_DEC].
*
*               day         Day   value [   1 to   31].
*
* Return(s)   : Day of year [1 to 366], if NO error(s).
*
*               CLK_DAY_OF_WK_NONE,     otherwise.
*
* Note(s)     : (1) It's only possible to get a day of year of an epoch supported by Clock :
*
*                   (a) Earliest year is the NTP   epoch start year, thus Year ('yr') MUST be greater
*                       than or equal to 'CLK_NTP_EPOCH_YR_START'.
*
*                   (b) Latest   year is the Clock epoch end   year, thus Year ('yr') MUST be less
*                       than 'CLK_EPOCH_YR_END'.
*********************************************************************************************************
*/

CLK_DAY  Clk_GetDayOfYr (CLK_YR     yr,
                         CLK_MONTH  month,
                         CLK_DAY    day)
{
    CLK_DAY      day_of_wk;
    CPU_BOOLEAN  valid;

                                                                /* ------------------ VALIDATE DATE ------------------- */
    valid = Clk_IsDateValid(yr,
                            month,
                            day,
                            CLK_NTP_EPOCH_YR_START,
                            CLK_EPOCH_YR_END);
    if (valid != DEF_YES) {
        return (CLK_DAY_OF_WK_NONE);
    }

                                                                /* ------------------ GET DAY OF YR ------------------- */
    day_of_wk = Clk_GetDayOfYrHandler(yr, month, day);

    return (day_of_wk);
}


/*
*********************************************************************************************************
*                                         Clk_DateTimeToStr()
*
* Description : Converts a date/time structure to an ASCII string.
*
* Argument(s) : p_date_time     Pointer to variable that contains the date/time structure to convert.
*
*               fmt             Desired string format :
*
*                                   CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS_UTC     "YYYY-MM-DD HH:MM:SS UTC+TZ"
*                                   CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS         "YYYY-MM-DD HH:MM:SS"
*                                   CLK_STR_FMT_MM_DD_YY_HH_MM_SS           "MM-DD-YY HH:MM:SS"
*                                   CLK_STR_FMT_YYYY_MM_DD                  "YYYY-MM-DD"
*                                   CLK_STR_FMT_MM_DD_YY                    "MM-DD-YY"
*                                   CLK_STR_FMT_DAY_MONTH_DD_YYYY           "Day Month DD, YYYY"
*                                   CLK_STR_FMT_DAY_MONTH_DD_HH_MM_SS_YYYY  "Day Mon DD HH:MM:SS YYYY"
*                                   CLK_STR_FMT_HH_MM_SS                    "HH:MM:SS"
*                                   CLK_STR_FMT_HH_MM_SS_AM_PM              "HH:MM:SS AM|PM"
*
*               p_str           Pointer to buffer that will receive the formated string (see Note #2).
*
*               str_len         Maximum number of characters the string can contains.
*
* Return(s)   : DEF_OK,   if string successfully returned.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) It's only possible to convert date supported by Clock :
*
*                   (a) Earliest year is the NTP   epoch start year, thus Year ('yr') MUST be greater
*                       than or equal to 'CLK_NTP_EPOCH_YR_START'.
*
*                   (b) Latest   year is the Clock epoch end   year, thus Year ('yr') MUST be less
*                       than 'CLK_EPOCH_YR_END'.
*
*               (2) The size of the string buffer that will receive the returned string address MUST be
*                   greater than or equal to CLK_STR_FMT_MAX_LEN.
*
*               (3) Pointers to variables that return values MUST be initialized PRIOR to all other
*                   validation or function handling in case of any error(s).
*
*               (4) Absolute value of the time zone offset is stored into 'CLK_TS_SEC' data type to be
*                   compliant with unsigned integer verification/operations.
*********************************************************************************************************
*/

#if (CLK_CFG_STR_CONV_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_DateTimeToStr (CLK_DATE_TIME  *p_date_time,
                                CLK_STR_FMT     fmt,
                                CPU_CHAR       *p_str,
                                CPU_SIZE_T      str_len)
{
    CPU_CHAR      yr   [CLK_STR_DIG_YR_LEN     + 1u];
    CPU_CHAR      month[CLK_STR_DIG_MONTH_LEN  + 1u];
    CPU_CHAR      day  [CLK_STR_DIG_DAY_LEN    + 1u];
    CPU_CHAR      hr   [CLK_STR_DIG_HR_LEN     + 1u];
    CPU_CHAR      min  [CLK_STR_DIG_MIN_LEN    + 1u];
    CPU_CHAR      sec  [CLK_STR_DIG_SEC_LEN    + 1u];
    CPU_CHAR      tz   [CLK_STR_DIG_TZ_MAX_LEN + 1u];
    CPU_CHAR      am_pm[CLK_STR_AM_PM_LEN      + 1u];
    CPU_CHAR     *p_yr;
    CPU_BOOLEAN   valid;
    CLK_HR        half_day_hr;
    CLK_TS_SEC    tz_hr_abs;                                     /* See Note #4.                                         */
    CLK_TS_SEC    tz_min_abs;                                    /* See Note #4.                                         */
    CLK_TS_SEC    tz_sec_rem_abs;                                /* See Note #4.                                         */


#if (CLK_CFG_ARG_CHK_EN == DEF_ENABLED)                         /* ----------------- VALIDATE STR PTR ----------------- */
    if (p_str == (CPU_CHAR *)0) {
        return (DEF_FAIL);
    }
    if (str_len < sizeof((CPU_CHAR)'\0')) {
        return (DEF_FAIL);
    }
#endif

   *p_str = '\0';                                               /* Init to NULL str for err (see Note #3).              */


#if (CLK_CFG_ARG_CHK_EN == DEF_ENABLED)                         /* ----------------- VALIDATE STR LEN ----------------- */
    switch (fmt) {
        case CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS_UTC:
             if (str_len < CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS_UTC_LEN) {
                 return (DEF_FAIL);
             }
             break;

        case CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS:
             if (str_len < CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS_LEN) {
                 return (DEF_FAIL);
             }
             break;

        case CLK_STR_FMT_MM_DD_YY_HH_MM_SS:
             if (str_len < CLK_STR_FMT_MM_DD_YY_HH_MM_SS_LEN) {
                 return (DEF_FAIL);
             }
             break;

        case CLK_STR_FMT_YYYY_MM_DD:
             if (str_len < CLK_STR_FMT_YYYY_MM_DD_LEN) {
                 return (DEF_FAIL);
             }
             break;

        case CLK_STR_FMT_MM_DD_YY:
             if (str_len < CLK_STR_FMT_MM_DD_YY_LEN) {
                 return (DEF_FAIL);
             }
             break;

        case CLK_STR_FMT_DAY_MONTH_DD_YYYY:
             if (str_len < CLK_STR_FMT_DAY_MONTH_DD_YYYY_MAX_LEN) {
                 return (DEF_FAIL);
             }
             break;

        case CLK_STR_FMT_DAY_MONTH_DD_HH_MM_SS_YYYY:
             if (str_len < CLK_STR_FMT_DAY_MONTH_DD_HH_MM_SS_YYYY_LEN) {
                 return (DEF_FAIL);
             }
             break;

        case CLK_STR_FMT_HH_MM_SS:
             if (str_len < CLK_STR_FMT_HH_MM_SS_LEN) {
                 return (DEF_FAIL);
             }
             break;

        case CLK_STR_FMT_HH_MM_SS_AM_PM:
             if (str_len < CLK_STR_FMT_HH_MM_SS_AM_PM_LEN) {
                 return (DEF_FAIL);
             }
             break;

        default:
             return (DEF_FAIL);
    }
#else
  (void)str_len;                                               /* Prevent 'variable unused' compiler warning.          */
#endif


                                                                /* ---------------- VALIDATE DATE/TIME ---------------- */
#if 0                                                           /* Validated in Clk_IsDateTimeValidHandler().           */
    if (p_date_time == (CLK_DATE_TIME *)0) {
        return (DEF_FAIL);
    }
#endif

    valid = Clk_IsDateTimeValidHandler(p_date_time,
                                       CLK_NTP_EPOCH_YR_START,
                                       CLK_EPOCH_YR_END);
    if (valid != DEF_YES) {
        return  (DEF_FAIL);
    }



                                                                /* -------------- CREATE DATE/TIME STRS --------------- */
   (void)Str_FmtNbr_Int32U((CPU_INT32U )p_date_time->Yr,        /* Create yr str.                                       */
                           (CPU_INT08U )CLK_STR_DIG_YR_LEN,
                           (CPU_INT08U )DEF_NBR_BASE_DEC,
                           (CPU_CHAR   )'\0',
                           (CPU_BOOLEAN)DEF_NO,
                           (CPU_BOOLEAN)DEF_YES,
                           (CPU_CHAR  *)yr);

   (void)Str_FmtNbr_Int32U((CPU_INT32U )p_date_time->Month,     /* Create month (dig) str.                              */
                           (CPU_INT08U )CLK_STR_DIG_MONTH_LEN,
                           (CPU_INT08U )DEF_NBR_BASE_DEC,
                           (CPU_CHAR   )'0',
                           (CPU_BOOLEAN)DEF_NO,
                           (CPU_BOOLEAN)DEF_YES,
                           (CPU_CHAR  *)month);

   (void)Str_FmtNbr_Int32U((CPU_INT32U )p_date_time->Day,       /* Create day str.                                      */
                           (CPU_INT08U )CLK_STR_DIG_DAY_LEN,
                           (CPU_INT08U )DEF_NBR_BASE_DEC,
                           (CPU_CHAR   )'0',
                           (CPU_BOOLEAN)DEF_NO,
                           (CPU_BOOLEAN)DEF_YES,
                           (CPU_CHAR  *)day);

   (void)Str_FmtNbr_Int32U((CPU_INT32U )p_date_time->Hr,        /* Create hr str.                                       */
                           (CPU_INT08U )CLK_STR_DIG_HR_LEN,
                           (CPU_INT08U )DEF_NBR_BASE_DEC,
                           (CPU_CHAR   )'0',
                           (CPU_BOOLEAN)DEF_NO,
                           (CPU_BOOLEAN)DEF_YES,
                           (CPU_CHAR  *)hr);

   (void)Str_FmtNbr_Int32U((CPU_INT32U )p_date_time->Min,       /* Create min str.                                      */
                           (CPU_INT08U )CLK_STR_DIG_MIN_LEN,
                           (CPU_INT08U )DEF_NBR_BASE_DEC,
                           (CPU_CHAR   )'0',
                           (CPU_BOOLEAN)DEF_NO,
                           (CPU_BOOLEAN)DEF_YES,
                           (CPU_CHAR  *)min);

   (void)Str_FmtNbr_Int32U((CPU_INT32U )p_date_time->Sec,       /* Create sec str.                                      */
                           (CPU_INT08U )CLK_STR_DIG_SEC_LEN,
                           (CPU_INT08U )DEF_NBR_BASE_DEC,
                           (CPU_CHAR   )'0',
                           (CPU_BOOLEAN)DEF_NO,
                           (CPU_BOOLEAN)DEF_YES,
                           (CPU_CHAR  *)sec);



    switch (fmt) {                                              /* ---------------- FMT DATE/TIME STR ----------------- */

        case CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS_UTC:               /* ------ BUILD STR "YYYY-MM-DD HH:MM:SS UTC+TZ" ------ */
             CLK_TRACE_DBG(("Date/time to string : YYYY-MM-DD HH:MM:SS UTC+TZ (+|-hh:mm)\n\r"));

            (void)Str_Copy_N(p_str, yr,     CLK_STR_DIG_YR_LEN + 1u);
            (void)Str_Cat_N (p_str, "-",    1u);

            (void)Str_Cat_N (p_str, month,  CLK_STR_DIG_MONTH_LEN);
            (void)Str_Cat_N (p_str, "-",    1u);

            (void)Str_Cat_N (p_str, day,    CLK_STR_DIG_DAY_LEN);
            (void)Str_Cat_N (p_str, " ",    1u);

            (void)Str_Cat_N (p_str, hr,     CLK_STR_DIG_HR_LEN);
            (void)Str_Cat_N (p_str, ":",    1u);

            (void)Str_Cat_N (p_str, min,    CLK_STR_DIG_MIN_LEN);
            (void)Str_Cat_N (p_str, ":",    1u);

            (void)Str_Cat_N (p_str, sec,    CLK_STR_DIG_SEC_LEN);
            (void)Str_Cat_N (p_str, " ",    1u);

            (void)Str_Cat_N (p_str, "UTC",  3u);

             if (p_date_time->TZ_sec >= 0) {
                (void)Str_Cat_N(p_str, "+", 1u);
             } else {
                (void)Str_Cat_N(p_str, "-", 1u);
             }

             tz_sec_rem_abs = DEF_ABS(p_date_time->TZ_sec);
             tz_hr_abs      = tz_sec_rem_abs / DEF_TIME_NBR_SEC_PER_HR;
             tz_sec_rem_abs = tz_sec_rem_abs % DEF_TIME_NBR_SEC_PER_HR;
            (void)Str_FmtNbr_Int32U((CPU_INT32S )tz_hr_abs,
                                    (CPU_INT08U )CLK_STR_DIG_TZ_HR_LEN,
                                    (CPU_INT08U )DEF_NBR_BASE_DEC,
                                    (CPU_CHAR   )'0',
                                    (CPU_BOOLEAN)DEF_NO,
                                    (CPU_BOOLEAN)DEF_YES,
                                    (CPU_CHAR  *)tz);

            (void)Str_Cat_N(p_str, tz,  CLK_STR_DIG_TZ_HR_LEN);
            (void)Str_Cat_N(p_str, ":", 1u);

             tz_min_abs = tz_sec_rem_abs / DEF_TIME_NBR_SEC_PER_MIN;
            (void)Str_FmtNbr_Int32U((CPU_INT32S )tz_min_abs,
                                    (CPU_INT08U )CLK_STR_DIG_TZ_MIN_LEN,
                                    (CPU_INT08U )DEF_NBR_BASE_DEC,
                                    (CPU_CHAR   )'0',
                                    (CPU_BOOLEAN)DEF_NO,
                                    (CPU_BOOLEAN)DEF_YES,
                                    (CPU_CHAR  *)tz);

            (void)Str_Cat_N(p_str, tz,  CLK_STR_DIG_TZ_MIN_LEN);
             break;



        case CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS:                   /* --------- BUILD STR "YYYY-MM-DD HH:MM:SS" ---------- */
             CLK_TRACE_DBG(("Date/time to string : YYYY-MM-DD HH:MM:SS\n\r"));

            (void)Str_Copy_N(p_str, yr,    CLK_STR_DIG_YR_LEN + 1u);
            (void)Str_Cat_N (p_str, "-",   1u);

            (void)Str_Cat_N (p_str, month, CLK_STR_DIG_MONTH_LEN);
            (void)Str_Cat_N (p_str, "-",   1u);

            (void)Str_Cat_N (p_str, day,   CLK_STR_DIG_DAY_LEN);
            (void)Str_Cat_N (p_str, " ",   1u);

            (void)Str_Cat_N (p_str, hr,    CLK_STR_DIG_HR_LEN);
            (void)Str_Cat_N (p_str, ":",   1u);

            (void)Str_Cat_N (p_str, min,   CLK_STR_DIG_MIN_LEN);
            (void)Str_Cat_N (p_str, ":",   1u);

            (void)Str_Cat_N (p_str, sec,   CLK_STR_DIG_SEC_LEN);
             break;


        case CLK_STR_FMT_MM_DD_YY_HH_MM_SS:                     /* ---------- BUILD STR "MM-DD-YY HH:MM:SS" ----------- */
             CLK_TRACE_DBG(("Date/time to string : MM-DD-YY HH:MM:SS\n\r"));

            (void)Str_Copy_N(p_str, month, CLK_STR_DIG_MONTH_LEN + 1u);
            (void)Str_Cat_N (p_str, "-",   1u);

            (void)Str_Cat_N (p_str, day,   CLK_STR_DIG_DAY_LEN);
            (void)Str_Cat_N (p_str, "-",   1u);

             p_yr = yr + CLK_STR_DIG_YR_TRUNC_LEN;
            (void)Str_Cat_N (p_str, p_yr,  CLK_STR_DIG_YR_TRUNC_LEN);
            (void)Str_Cat_N (p_str, " ",   1u);

            (void)Str_Cat_N (p_str, hr,    CLK_STR_DIG_HR_LEN);
            (void)Str_Cat_N (p_str, ":",   1u);

            (void)Str_Cat_N (p_str, min,   CLK_STR_DIG_MIN_LEN);
            (void)Str_Cat_N (p_str, ":",   1u);

            (void)Str_Cat_N (p_str, sec,   CLK_STR_DIG_SEC_LEN);
             break;



        case CLK_STR_FMT_YYYY_MM_DD:                            /* -------------- BUILD STR "YYYY-MM-DD" -------------- */
             CLK_TRACE_DBG(("Date/time to string : YYYY-MM-DD\n\r"));

            (void)Str_Copy_N(p_str, yr,    CLK_STR_DIG_YR_LEN + 1u);
            (void)Str_Cat_N (p_str, "-",   1u);

            (void)Str_Cat_N (p_str, month, CLK_STR_DIG_MONTH_LEN);
            (void)Str_Cat_N (p_str,  "-",  1u);

            (void)Str_Cat_N (p_str, day,   CLK_STR_DIG_DAY_LEN);
             break;



        case CLK_STR_FMT_MM_DD_YY:                              /* --------------- BUILD STR ""MM-DD-YY" -------------- */
             CLK_TRACE_DBG(("Date/time to string : MM-DD-YY\n\r"));

            (void)Str_Copy_N(p_str, month, CLK_STR_DIG_MONTH_LEN + 1u);
            (void)Str_Cat_N (p_str, "-",   1u);

            (void)Str_Cat_N (p_str, day,   CLK_STR_DIG_DAY_LEN);
            (void)Str_Cat_N (p_str, "-",   1u);

             p_yr = yr + CLK_STR_DIG_YR_TRUNC_LEN;
            (void)Str_Cat_N (p_str, p_yr,  CLK_STR_DIG_YR_TRUNC_LEN);
             break;



        case CLK_STR_FMT_DAY_MONTH_DD_YYYY:                     /* ---------- BUILD STR "Day Month DD, YYYY" ---------- */
             CLK_TRACE_DBG(("Date/time to string : Day Month DD, YYYY\n\r"));

            (void)Str_Copy_N((CPU_CHAR *)p_str,
                             (CPU_CHAR *)Clk_StrDayOfWk[p_date_time->DayOfWk - CLK_FIRST_DAY_OF_WK],
                             (CPU_SIZE_T)CLK_STR_DAY_OF_WK_MAX_LEN + 1u);
            (void)Str_Cat_N(  p_str, " ",  1u);

            (void)Str_Cat_N( (CPU_CHAR *)p_str,
                             (CPU_CHAR *)Clk_StrMonth[p_date_time->Month - CLK_FIRST_DAY_OF_MONTH],
                             (CPU_SIZE_T)CLK_STR_MONTH_MAX_LEN);
            (void)Str_Cat_N(  p_str, " ",  1u);

            (void)Str_Cat_N(  p_str, day,  CLK_STR_DIG_DAY_LEN);
            (void)Str_Cat_N(  p_str, ", ", 2u);

            (void)Str_Cat_N(  p_str, yr,   CLK_STR_DIG_YR_LEN);
             break;


        case CLK_STR_FMT_DAY_MONTH_DD_HH_MM_SS_YYYY:            /* ------- BUILD STR "Day Mon DD HH:MM:SS YYYY" ------- */
             CLK_TRACE_DBG(("Date/time to string : Day Mon DD HH:MM:SS YYYY\n\r"));

            (void)Str_Copy_N((CPU_CHAR *)p_str,
                             (CPU_CHAR *)Clk_StrDayOfWk[p_date_time->DayOfWk - CLK_FIRST_DAY_OF_WK],
                             (CPU_SIZE_T)CLK_STR_DAY_OF_WK_TRUNC_LEN + 1u);
             p_str[3] = '\0';
            (void)Str_Cat_N (p_str, " ", 1u);

            (void)Str_Cat_N((CPU_CHAR  *)p_str,
                            (CPU_CHAR  *)Clk_StrMonth[p_date_time->Month - CLK_FIRST_DAY_OF_MONTH],
                            (CPU_SIZE_T )CLK_STR_MONTH_TRUNC_LEN);
            (void)Str_Cat_N (p_str, " ", 1u);

            (void)Str_Cat_N (p_str, day, CLK_STR_DIG_DAY_LEN);
            (void)Str_Cat_N (p_str, " ", 1u);

            (void)Str_Cat_N (p_str, hr,  CLK_STR_DIG_HR_LEN);
            (void)Str_Cat_N (p_str, ":", 1u);

            (void)Str_Cat_N (p_str, min, CLK_STR_DIG_MIN_LEN);
            (void)Str_Cat_N (p_str, ":", 1u);

            (void)Str_Cat_N (p_str, sec, CLK_STR_DIG_SEC_LEN);
            (void)Str_Cat_N (p_str, " ", 1u);

            (void)Str_Cat_N (p_str, yr,  CLK_STR_DIG_YR_LEN);
             break;



        case CLK_STR_FMT_HH_MM_SS:                              /* --------------- BUILD STR "HH:MM:SS" --------------- */
             CLK_TRACE_DBG(("Date/time to string : HH:MM:SS\n\r"));

            (void)Str_Copy_N(p_str, hr,  CLK_STR_DIG_HR_LEN + 1u);
            (void)Str_Cat_N (p_str, ":", 1u);

            (void)Str_Cat_N (p_str, min, CLK_STR_DIG_MIN_LEN);
            (void)Str_Cat_N (p_str, ":", 1u);

            (void)Str_Cat_N (p_str, sec, CLK_STR_DIG_SEC_LEN);
             break;



        case CLK_STR_FMT_HH_MM_SS_AM_PM:                        /* ------------ BUILD STR "HH:MM:SS AM|PM" ------------ */
             CLK_TRACE_DBG(("Date/time to string : HH:MM:SS AM|PM\n\r"));

             if (p_date_time->Hr < CLK_HR_PER_HALF_DAY) {       /* Chk for AM or PM.                                    */
                (void)Str_Copy_N(am_pm, "AM", CLK_STR_AM_PM_LEN + 1u);
                 if (p_date_time->Hr == 0u) {
                     half_day_hr = CLK_HR_PER_HALF_DAY;
                 } else {
                     half_day_hr = p_date_time->Hr;
                 }
             } else {
                (void)Str_Copy_N(am_pm, "PM", CLK_STR_AM_PM_LEN + 1u);
                 half_day_hr = p_date_time->Hr - CLK_HR_PER_HALF_DAY;
             }

            (void)Str_FmtNbr_Int32U((CPU_INT32U )half_day_hr,
                                    (CPU_INT08U )CLK_STR_DIG_HR_LEN,
                                    (CPU_INT08U )DEF_NBR_BASE_DEC,
                                    (CPU_CHAR   )'0',
                                    (CPU_BOOLEAN)DEF_NO,
                                    (CPU_BOOLEAN)DEF_YES,
                                    (CPU_CHAR  *)hr);

            (void)Str_Copy_N(p_str, hr,    CLK_STR_DIG_HR_LEN + 1u);
            (void)Str_Cat_N (p_str, ":",   1u);

            (void)Str_Cat_N (p_str, min,   CLK_STR_DIG_MIN_LEN);
            (void)Str_Cat_N (p_str, ":",   1u);

            (void)Str_Cat_N (p_str, sec,   CLK_STR_DIG_SEC_LEN);
            (void)Str_Cat_N (p_str, " ",   1u);

            (void)Str_Cat_N (p_str, am_pm, CLK_STR_AM_PM_LEN);
             break;



        default:                                                /* ------------------- INVALID FMT -------------------- */
             CLK_TRACE_DBG(("Date/time to string : Invalid format\n\r"));
             return (DEF_FAIL);
    }

    CLK_TRACE_DBG(("    %s\n\r", p_str));

    return (DEF_OK);
}
#endif


/*
*********************************************************************************************************
*                                           Clk_GetTS_NTP()
*
* Description : Get current Clock timestamp as an NTP timestamp.
*
* Argument(s) : p_ts_ntp_sec    Pointer to variable that will receive the NTP timestamp :
*
*                                   In seconds UTC+00,  if NO error(s);
*                                   CLK_TS_SEC_NONE,    otherwise.
*
* Return(s)   : DEF_OK,   if timestamp successfully returned.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) (a) NTP timestamp is converted from the internal Clock timestamp which SHOULD
*                       be set for UTC+00. Thus the NTP timestamp is returned at UTC+00.
*
*                   (b) NTP timestamp does NOT include the internal Clock time zone. Thus any
*                       local time zone offset MUST be applied after calling Clk_GetTS_NTP().
*
*               (2) NTP timestamp will eventually overflow, thus it's not possible to get NTP
*                   timestamp for years on or after CLK_NTP_EPOCH_YR_END.
*
*               (3) Pointers to variables that return values MUST be initialized PRIOR to all
*                   other validation or function handling in case of any error(s).
*********************************************************************************************************
*/

#if (CLK_CFG_NTP_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_GetTS_NTP (CLK_TS_SEC  *p_ts_ntp_sec)
{
    CLK_TS_SEC   ts_sec;
    CPU_BOOLEAN  valid;


#if 0                                                           /* Validated & init'd in Clk_TS_ToTS_NTP().             */
    if (p_ts_ntp_sec == (CLK_TS_SEC *)0) {                      /* ----------------- VALIDATE TS PTR ------------------ */
        return (DEF_FAIL);
    }

   *p_ts_ntp_sec = CLK_TS_SEC_NONE;                             /* Init to ts none for err (see Note #3).               */
#endif

                                                                /* -------------------- GET CLK TS -------------------- */
    ts_sec = Clk_GetTS();

                                                                /* -------------- CONV CLK TS TO NTP TS --------------- */
    valid  = Clk_TS_ToTS_NTP(ts_sec, p_ts_ntp_sec);
    if (valid != DEF_OK) {
        return  (DEF_FAIL);
    }

    return (DEF_OK);
}
#endif


/*
*********************************************************************************************************
*                                           Clk_SetTS_NTP()
*
* Description : Set Clock timestamp from an NTP timestamp.
*
* Argument(s) : ts_ntp_sec      Current NTP timestamp to set (in seconds, UTC+00).
*
* Return(s)   : DEF_OK,   if timestamp successfully set.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Internal Clock timestamp SHOULD be set for UTC+00 and should NOT include any local
*                   time zone offset.
*
*               (2) Only years supported by Clock & NTP can be set, thus the timestamp date MUST be :
*
*                   (a) >= CLK_EPOCH_YR_START
*                   (b) <  CLK_NTP_EPOCH_YR_END
*********************************************************************************************************
*/

#if (CLK_CFG_NTP_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_SetTS_NTP (CLK_TS_SEC  ts_ntp_sec)
{
    CLK_TS_SEC   ts_sec;
    CPU_BOOLEAN  valid;

                                                                /* -------------- CONV NTP TS TO CLK TS --------------- */
    valid = Clk_TS_NTP_ToTS(&ts_sec, ts_ntp_sec);
    if (valid != DEF_OK) {
        return  (DEF_FAIL);
    }

                                                                /* -------------------- SET CLK TS -------------------- */
    Clk_SetTS(ts_sec);

    return (DEF_OK);
}
#endif


/*
*********************************************************************************************************
*                                          Clk_TS_ToTS_NTP()
*
* Description : Convert Clock timestamp to NTP timestamp.
*
* Argument(s) : ts_sec          Timestamp to convert (in seconds, UTC+00).
*
*               p_ts_ntp_sec    Pointer to variable that will receive the NTP timestamp :
*
*                                   In seconds UTC+00,  if NO error(s);
*                                   CLK_TS_SEC_NONE,    otherwise.
*
* Return(s)   : DEF_OK,   if timestamp successfully converted.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Returned timestamp does NOT include any time zone offset. Thus any local time
*                   zone offset SHOULD be applied before or after calling Clk_TS_ToTS_NTP().
*
*               (2) Only years supported by Clock & NTP can be converted, thus the timestamp date
*                   MUST be :
*
*                   (a) >= CLK_EPOCH_YR_START
*                   (b) <  CLK_NTP_EPOCH_YR_END
*
*               (3) Pointers to variables that return values MUST be initialized PRIOR to all
*                   other validation or function handling in case of any error(s).
*********************************************************************************************************
*/

#if (CLK_CFG_NTP_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_TS_ToTS_NTP (CLK_TS_SEC   ts_sec,
                              CLK_TS_SEC  *p_ts_ntp_sec)
{
    CLK_TS_SEC  ts_ntp_sec;


#if (CLK_CFG_ARG_CHK_EN == DEF_ENABLED)                         /* ----------------- VALIDATE TS PTR ------------------ */
    if (p_ts_ntp_sec == (CLK_TS_SEC *)0) {
        return (DEF_FAIL);
    }
#endif

   *p_ts_ntp_sec = CLK_TS_SEC_NONE;                             /* Init to ts none for err (see Note #3).               */

    CLK_TRACE_DBG(("\n\rConvert TS to NTP TS:\n\r"
                   "    TS = %u\n\r", (unsigned int)ts_sec));

                                                                /* -------------- CONV CLK TS TO NTP TS --------------- */
    ts_ntp_sec = ts_sec + CLK_NTP_EPOCH_OFFSET_SEC;
    if (ts_ntp_sec < CLK_NTP_EPOCH_OFFSET_SEC) {                /* Chk for ovf.                                         */
        CLK_TRACE_DBG(("    NTP TS conversion has failed\n\r"));
        return (DEF_FAIL);
    }

   *p_ts_ntp_sec = ts_ntp_sec;
    CLK_TRACE_DBG(("    NTP TS converted = %u\n\r", (unsigned int)*p_ts_ntp_sec));

    return (DEF_OK);
}
#endif


/*
*********************************************************************************************************
*                                          Clk_TS_NTP_ToTS()
*
* Description : Convert NTP timestamp to Clock timestamp.
*
* Argument(s) : p_ts_sec        Pointer to variable that will receive the Clock timestamp :
*
*                                   In seconds UTC+00,  if NO error(s);
*                                   CLK_TS_SEC_NONE,    otherwise.
*
*               ts_ntp_sec      NTP timestamp value to convert (in seconds, UTC+00).
*
* Return(s)   : DEF_OK,   if timestamp successfully converted.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Returned timestamp does NOT include any time zone offset. Thus any local time
*                   zone offset SHOULD be applied before or after calling Clk_TS_NTP_ToTS().
*
*               (2) Only years supported by Clock & NTP can be converted, thus the timestamp date
*                   MUST be :
*
*                   (a) >= CLK_EPOCH_YR_START
*                   (b) <  CLK_NTP_EPOCH_YR_END
*
*               (3) Pointers to variables that return values MUST be initialized PRIOR to all other
*                   validation or function handling in case of any error(s).
*********************************************************************************************************
*/

#if (CLK_CFG_NTP_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_TS_NTP_ToTS (CLK_TS_SEC  *p_ts_sec,
                              CLK_TS_SEC   ts_ntp_sec)
{
#if (CLK_CFG_ARG_CHK_EN == DEF_ENABLED)                         /* ----------------- VALIDATE TS PTR ------------------ */
    if (p_ts_sec == (CLK_TS_SEC *)0) {
        return (DEF_FAIL);
    }
#endif

   *p_ts_sec = CLK_TS_SEC_NONE;                                 /* Init to ts none for err (see Note #3).               */

    CLK_TRACE_DBG(("Convert NTP TS to TS:\n\r"
                   "    NTP TS = %u\n\r", (unsigned int)ts_ntp_sec));

    if (ts_ntp_sec < CLK_NTP_EPOCH_OFFSET_SEC) {                /* Chk for ovf.                                         */
        CLK_TRACE_DBG(("TS overflow\n\r"));
        return (DEF_FAIL);
    }

                                                                /* -------------- CONV NTP TS TO CLK TS --------------- */
   *p_ts_sec = ts_ntp_sec - CLK_NTP_EPOCH_OFFSET_SEC;
    CLK_TRACE_DBG(("    TS converted = %u\n\r", (unsigned int)*p_ts_sec));

    return (DEF_OK);
}
#endif


/*
*********************************************************************************************************
*                                       Clk_TS_NTP_ToDateTime()
*
* Description : Convert NTP timestamp to date/time structure.
*
* Argument(s) : ts_ntp_sec      Timestamp to convert (in seconds,          UTC+00).
*
*               tz_sec          Time zone offset     (in seconds, +|- from UTC).
*
*               p_date_time     Pointer to variable that will receive the date/time structure.
*
* Return(s)   : DEF_OK,   if date/time structure successfully returned.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) (a) Timestamp ('ts_ntp_sec') MUST be set for UTC+00 & SHOULD NOT include the time
*                       zone offset ('tz_sec') since Clk_TS_NTP_ToDateTime() includes the time zone
*                       offset in its date/time calculation. Thus the time zone offset SHOULD NOT be
*                       applied before or after calling Clk_TS_NTP_ToDateTime().
*
*                   (b) Time zone field of the date/time structure ('p_date_time->TZ_sec') is set to
*                       the value of the time zone argument ('tz_sec').
*
*                   (c) Time zone is based on Coordinated Universal Time (UTC) & has valid values :
*
*                       (1) Between  +|- 12 hours (+|- 43200 seconds)
*                       (2) Multiples of 15 minutes
*********************************************************************************************************
*/

#if (CLK_CFG_NTP_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_TS_NTP_ToDateTime (CLK_TS_SEC      ts_ntp_sec,
                                    CLK_TZ_SEC      tz_sec,
                                    CLK_DATE_TIME  *p_date_time)
{
    CPU_BOOLEAN  valid;

                                                                /* ------------- CONV NTP TS TO DATE/TIME ------------- */
    CLK_TRACE_DBG(("\n\rConvert TS NTP to Date/time:\n\r"
                   "    TS to convert= %u\n\r\n\r",
                   (unsigned int)ts_ntp_sec));

    valid = Clk_TS_ToDateTimeHandler(ts_ntp_sec,
                                     tz_sec,
                                     p_date_time,
                                     CLK_NTP_EPOCH_YR_START,
                                     CLK_NTP_EPOCH_YR_END);
    if (valid != DEF_OK) {
        CLK_TRACE_DBG(("    Date/time conversion has failed\n\r"));
        return  (DEF_FAIL);
    }

    return (DEF_OK);
}
#endif


/*
*********************************************************************************************************
*                                       Clk_DateTimeToTS_NTP()
*
* Description : Convert a date/time structure to NTP timestamp.
*
* Argument(s) : p_ts_ntp_sec    Pointer to variable that will receive the NTP timestamp :
*
*                                   In seconds UTC+00,  if NO error(s);
*                                   CLK_TS_SEC_NONE,    otherwise.
*
*               p_date_time     Pointer to variable that contains date/time structure to convert.
*
* Return(s)   : DEF_OK,   if timestamp successfully returned.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Date/time structure ('p_date_time') MUST be representable in NTP timestamp.
*                   Thus date to convert MUST be :
*
*                   (a) >= CLK_NTP_EPOCH_YR_START
*                   (b) <  CLK_NTP_EPOCH_YR_END
*
*               (2) (a) Date/time ('p_date_time') SHOULD be set to local time with correct time zone
*                       offset ('p_date_time->TZ_sec'). Clk_DateTimeToTS_NTP() removes the time zone
*                       offset from the date/time to calculate & return a Clock timestamp at UTC+00.
*
*                   (b) Time zone is based on Coordinated Universal Time (UTC) & has valid values :
*
*                       (1) Between  +|- 12 hours (+|- 43200 seconds)
*                       (2) Multiples of 15 minutes
*
*               (3) Pointers to variables that return values MUST be initialized PRIOR to all other
*                   validation or function handling in case of any error(s).
*********************************************************************************************************
*/

#if (CLK_CFG_NTP_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_DateTimeToTS_NTP (CLK_TS_SEC     *p_ts_ntp_sec,
                                   CLK_DATE_TIME  *p_date_time)
{
    CPU_BOOLEAN  valid;


#if (CLK_CFG_ARG_CHK_EN == DEF_ENABLED)                         /* ----------------- VALIDATE TS PTR ------------------ */
    if (p_ts_ntp_sec == (CLK_TS_SEC *)0) {
        return (DEF_FAIL);
    }
#endif

   *p_ts_ntp_sec = CLK_TS_SEC_NONE;                             /* Init to ts none for err (see Note #3).               */


#if 0                                                           /* Validated in Clk_IsNTP_DateTimeValid().              */
    if (p_date_time == (CLK_DATE_TIME *)0) {                    /* -------------- VALIDATE DATE/TIME PTR -------------- */
        return (DEF_FAIL);
    }
#endif

                                                                /* ---------------- VALIDATE DATE/TIME ---------------- */
    valid = Clk_IsNTP_DateTimeValid(p_date_time);
    if (valid != DEF_OK) {
        return  (DEF_FAIL);
    }

                                                                /* ------------- CONV DATE/TIME TO NTP TS ------------- */
    CLK_TRACE_DBG(("Convert Date/time to TS:\n\r"));
    valid = Clk_DateTimeToTS_Handler(p_ts_ntp_sec,
                                     p_date_time,
                                     CLK_NTP_EPOCH_YR_START);
    if (valid != DEF_OK) {
        return  (DEF_FAIL);
    }

    return (DEF_OK);
}
#endif


/*
*********************************************************************************************************
*                                       Clk_NTP_DateTimeMake()
*
* Description : Build a valid NTP epoch date/time structure.
*
* Argument(s) : p_date_time     Pointer to variable that will receive the date/time structure.
*
*               yr              Year    value [CLK_NTP_EPOCH_YR_START to CLK_NTP_EPOCH_YR_END) (see Note #1).
*
*               month           Month   value [         CLK_MONTH_JAN to        CLK_MONTH_DEC].
*
*               day             Day     value [                     1 to                   31].
*
*               hr              Hours   value [                     0 to                   23].
*
*               min             Minutes value [                     0 to                   59].
*
*               sec             Seconds value [                     0 to                   60] (see Note #3).
*
*               tz_sec          Time zone offset (in seconds, +|- from UTC) [-43200 to 43200].
*
* Return(s)   : DEF_OK,   if date/time structure successfully returned.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Date/time structure ('p_date_time') MUST be representable in NTP timestamp.
*                   Thus date to convert MUST be :
*
*                   (a) >= CLK_NTP_EPOCH_YR_START
*                   (b) <  CLK_NTP_EPOCH_YR_END
*
*               (2) Day of week ('p_date_time->DayOfWk') and Day of year ('p_date_time->DayOfYr')
*                   are internally calculated and set in the date/time structure if date is valid.
*
*               (3) Seconds value of 60 is valid to be compatible with leap second adjustment and
*                   the atomic clock time structure.
*
*               (4) Time zone is based ('tz_sec') on Coordinated Universal Time (UTC) & has valid
*                   values :
*
*                   (a) Between  +|- 12 hours (+|- 43200 seconds)
*                   (b) Multiples of 15 minutes
*********************************************************************************************************
*/

#if (CLK_CFG_NTP_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_NTP_DateTimeMake (CLK_DATE_TIME  *p_date_time,
                                   CLK_YR          yr,
                                   CLK_MONTH       month,
                                   CLK_DAY         day,
                                   CLK_HR          hr,
                                   CLK_MIN         min,
                                   CLK_SEC         sec,
                                   CLK_TZ_SEC      tz_sec)
{
    CPU_BOOLEAN  valid;

                                                                /* ---------- VALIDATE & CONV NTP DATE/TIME ----------- */
    valid = Clk_DateTimeMakeHandler(p_date_time,
                                    yr,
                                    month,
                                    day,
                                    hr,
                                    min,
                                    sec,
                                    tz_sec,
                                    CLK_NTP_EPOCH_YR_START,
                                    CLK_NTP_EPOCH_YR_END);
    if (valid != DEF_YES) {
        return  (DEF_FAIL);
    }

    return (DEF_OK);
}
#endif


/*
*********************************************************************************************************
*                                      Clk_IsNTP_DateTimeValid()
*
* Description : Determine if date/time structure is valid in NTP epoch.
*
* Argument(s) : p_date_time     Pointer to variable that contains the date/time structure to validate.
*
* Return(s)   : DEF_YES, if date/time structure is valid.
*
*               DEF_NO,  otherwise.
*
* Note(s)     : (1) Date/time structure ('p_date_time') MUST be representable in NTP epoch. Thus
*                   date to validate MUST be :
*
*                   (a) >= CLK_NTP_EPOCH_YR_START
*                   (b) <  CLK_NTP_EPOCH_YR_END
*
*               (2) Time zone is based ('p_date_time->TZ_sec') on Coordinated Universal Time (UTC)
*                   & has valid values :
*
*                   (a) Between  +|- 12 hours (+|- 43200 seconds)
*                   (b) Multiples of 15 minutes
*********************************************************************************************************
*/

#if (CLK_CFG_NTP_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_IsNTP_DateTimeValid (CLK_DATE_TIME  *p_date_time)
{
    CPU_BOOLEAN  valid;

                                                                /* -------------- VALIDATE NTP DATE/TIME -------------- */
    CLK_TRACE_DBG(("Validate NTP date/time: "));

    valid = Clk_IsDateTimeValidHandler(p_date_time,
                                       CLK_NTP_EPOCH_YR_START,
                                       CLK_NTP_EPOCH_YR_END);
    if (valid != DEF_YES) {
        CLK_TRACE_DBG(("Fail\n\r"));
        return  (DEF_NO);
    }

    CLK_TRACE_DBG(("Ok\n\r"));

    return (DEF_YES);
}
#endif


/*
*********************************************************************************************************
*                                          Clk_GetTS_Unix()
*
* Description : Get current Clock timestamp as a Unix timestamp.
*
* Argument(s) : p_ts_unix_sec   Pointer to variable that will receive the Unix timestamp :
*
*                                   In seconds UTC+00,  if NO error(s);
*                                   CLK_TS_SEC_NONE,    otherwise.
*
* Return(s)   : DEF_OK,   if timestamp successfully returned.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) (a) Unix timestamp is converted from the internal Clock timestamp which SHOULD
*                       be set for UTC+00. Thus the Unix timestamp is returned at UTC+00.
*
*                   (b) Unix timestamp does NOT include the internal Clock time zone. Thus any
*                       local time zone offset MUST be applied after calling Clk_GetTS_Unix().
*
*               (2) Unix timestamp will eventually overflow, thus it's not possible to get Unix
*                   timestamp for years on or after CLK_UNIX_EPOCH_YR_END.
*
*               (3) Pointers to variables that return values MUST be initialized PRIOR to all
*                   other validation or function handling in case of any error(s).
*********************************************************************************************************
*/

#if (CLK_CFG_UNIX_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_GetTS_Unix (CLK_TS_SEC  *p_ts_unix_sec)
{
    CLK_TS_SEC   ts_sec;
    CPU_BOOLEAN  valid;


#if 0                                                           /* Validated & init'd in Clk_TS_ToTS_Unix().            */
    if (p_ts_unix_sec == (CLK_TS_SEC *)0) {                     /* ----------------- VALIDATE TS PTR ------------------ */
        return (DEF_FAIL);
    }

   *p_ts_unix_sec = CLK_TS_SEC_NONE;                            /* Init to ts none for err (see Note #3).               */
#endif

                                                                /* -------------------- GET CLK TS -------------------- */
    ts_sec = Clk_GetTS();

                                                                /* -------------- CONV CLK TS TO UNIX TS -------------- */
    valid  = Clk_TS_ToTS_Unix(ts_sec, p_ts_unix_sec);
    if (valid != DEF_OK) {
        return  (DEF_FAIL);
    }

    return (DEF_OK);
}
#endif


/*
*********************************************************************************************************
*                                          Clk_SetTS_Unix()
*
* Description : Set Clock timestamp from a Unix timestamp.
*
* Argument(s) : ts_unix_sec     Current Unix timestamp to set (in seconds, UTC+00).
*
* Return(s)   : DEF_OK,   if timestamp successfully set.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Internal Clock timestamp SHOULD be set for UTC+00 and should NOT include any local
*                   time zone offset.
*
*               (2) Only years supported by Clock & Unix can be set, thus the timestamp date MUST be :
*
*                   (a) >= CLK_EPOCH_YR_START
*                   (b) <  CLK_UNIX_EPOCH_YR_END
*********************************************************************************************************
*/

#if (CLK_CFG_UNIX_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_SetTS_Unix (CLK_TS_SEC  ts_unix_sec)
{
    CLK_TS_SEC   ts_sec;
    CPU_BOOLEAN  valid;

                                                                /* -------------- CONV UNIX TS TO CLK TS -------------- */
    valid = Clk_TS_UnixToTS(&ts_sec, ts_unix_sec);
    if (valid != DEF_OK) {
        return  (DEF_FAIL);
    }

                                                                /* -------------------- SET CLK TS -------------------- */
    Clk_SetTS(ts_sec);

    return (DEF_OK);
}
#endif


/*
*********************************************************************************************************
*                                         Clk_TS_ToTS_Unix()
*
* Description : Convert Clock timestamp to Unix timestamp.
*
* Argument(s) : ts_sec          Timestamp to convert (in seconds, UTC+00).
*
*               p_ts_unix_sec   Pointer to variable that will receive the Unix timestamp :
*
*                                   In seconds UTC+00,  if NO error(s);
*                                   CLK_TS_SEC_NONE,    otherwise.
*
* Return(s)   : DEF_OK,   if timestamp successfully converted.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Returned timestamp does NOT include any time zone offset. Thus any local time
*                   zone offset SHOULD be applied before or after calling Clk_TS_ToTS_Unix().
*
*               (2) Only years supported by Clock & Unix can be converted, thus the timestamp date
*                   MUST be :
*
*                   (a) >= CLK_EPOCH_YR_START
*                   (b) <  CLK_UNIX_EPOCH_YR_END
*
*               (3) Pointers to variables that return values MUST be initialized PRIOR to all other
*                   validation or function handling in case of any error(s).
*********************************************************************************************************
*/

#if (CLK_CFG_UNIX_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_TS_ToTS_Unix (CLK_TS_SEC   ts_sec,
                               CLK_TS_SEC  *p_ts_unix_sec)
{
    CLK_TS_SEC  ts_unix_sec;


#if (CLK_CFG_ARG_CHK_EN == DEF_ENABLED)                         /* ----------------- VALIDATE TS PTR ------------------ */
    if (p_ts_unix_sec == (CLK_TS_SEC *)0) {
        return (DEF_FAIL);
    }
#endif

   *p_ts_unix_sec = CLK_TS_SEC_NONE;                            /* Init to ts none for err (see Note #3).               */

    CLK_TRACE_DBG(("\n\rConvert TS to Unix TS:\n\r"
                   "    TS = %u\n\r", (unsigned int)ts_sec));

                                                                /* -------------- CONV CLK TS TO UNIX TS -------------- */
    ts_unix_sec = ts_sec + CLK_UNIX_EPOCH_OFFSET_SEC;
    if (ts_unix_sec < CLK_UNIX_EPOCH_OFFSET_SEC) {              /* Chk for ovf.                                         */
        CLK_TRACE_DBG(("    Unix TS conversion has failed\n\r"));
        return (DEF_FAIL);
    }

   *p_ts_unix_sec = ts_unix_sec;
    CLK_TRACE_DBG(("    Unix TS converted = %u\n\r", (unsigned int)*p_ts_unix_sec));

    return (DEF_OK);
}
#endif


/*
*********************************************************************************************************
*                                          Clk_TS_UnixToTS()
*
* Description : Convert Unix timestamp to Clock timestamp.
*
* Argument(s) : p_ts_sec        Pointer to variable that will receive the Clock timestamp :
*
*                                   In seconds UTC+00,  if NO error(s);
*                                   CLK_TS_SEC_NONE,    otherwise.
*
*               ts_unix_sec     Unix timestamp value to convert (in seconds, UTC+00).
*
* Return(s)   : DEF_OK,   if timestamp successfully converted.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Returned timestamp does NOT include any time zone offset. Thus any local time
*                   zone offset SHOULD be applied before or after calling Clk_TS_UnixToTS().
*
*               (2) Only years supported by Clock & Unix can be converted, thus the timestamp date
*                   MUST be :
*
*                   (a) >= CLK_EPOCH_YR_START
*                   (b) <  CLK_UNIX_EPOCH_YR_END
*
*               (3) Pointers to variables that return values MUST be initialized PRIOR to all other
*                   validation or function handling in case of any error(s).
*********************************************************************************************************
*/

#if (CLK_CFG_UNIX_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_TS_UnixToTS (CLK_TS_SEC  *p_ts_sec,
                              CLK_TS_SEC   ts_unix_sec)
{
#if (CLK_CFG_ARG_CHK_EN == DEF_ENABLED)                         /* ----------------- VALIDATE TS PTR ------------------ */
    if (p_ts_sec == (CLK_TS_SEC *)0) {
        return (DEF_FAIL);
    }
#endif

   *p_ts_sec = CLK_TS_SEC_NONE;                                 /* Init to ts none for err (see Note #3).               */

    CLK_TRACE_DBG(("Convert Unix TS to TS:\n\r"
                   "    Unix TS = %u\n\r", (unsigned int)ts_unix_sec));

    if (ts_unix_sec < CLK_UNIX_EPOCH_OFFSET_SEC) {              /* Chk for ovf.                                         */
        CLK_TRACE_DBG(("TS overflow\n\r"));
        return (DEF_FAIL);
    }

                                                                /* -------------- CONV UNIX TS TO CLK TS -------------- */
   *p_ts_sec = ts_unix_sec - CLK_UNIX_EPOCH_OFFSET_SEC;
    CLK_TRACE_DBG(("    TS converted = %u\n\r", (unsigned int)*p_ts_sec));

    return (DEF_OK);
}
#endif


/*
*********************************************************************************************************
*                                       Clk_TS_UnixToDateTime()
*
* Description : Convert Unix timestamp to a date/time structure.
*
* Argument(s) : ts_unix_sec     Timestamp to convert (in seconds,          UTC+00).
*
*               tz_sec          Time zone offset     (in seconds, +|- from UTC).
*
*               p_date_time     Pointer to variable that will receive the date/time structure.
*
* Return(s)   : DEF_OK,   if date/time structure successfully returned.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) (a) Timestamp ('ts_unix_sec') MUST be set for UTC+00 & SHOULD NOT include the time
*                       zone offset ('tz_sec') since Clk_TS_UnixToDateTime() includes the time zone
*                       offset in its date/time calculation. Thus the time zone offset SHOULD NOT be
*                       applied before or after calling Clk_TS_UnixToDateTime().
*
*                   (b) Time zone field of the date/time structure ('p_date_time->TZ_sec') is set to
*                       the value of the time zone argument ('tz_sec').
*
*                   (c) Time zone is based on Coordinated Universal Time (UTC) & has valid values :
*
*                       (1) Between  +|- 12 hours (+|- 43200 seconds)
*                       (2) Multiples of 15 minutes
*********************************************************************************************************
*/

#if (CLK_CFG_UNIX_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_TS_UnixToDateTime (CLK_TS_SEC      ts_unix_sec,
                                    CLK_TZ_SEC      tz_sec,
                                    CLK_DATE_TIME  *p_date_time)
{
    CPU_BOOLEAN  valid;

                                                                /* ------------ CONV UNIX TS TO DATE/TIME ------------- */
    CLK_TRACE_DBG(("\n\rConvert TS Unix to Date/time:\n\r"
                   "    TS to convert= %u\n\n\r",
                   (unsigned int)ts_unix_sec));

    valid = Clk_TS_ToDateTimeHandler(ts_unix_sec,
                                     tz_sec,
                                     p_date_time,
                                     CLK_UNIX_EPOCH_YR_START,
                                     CLK_UNIX_EPOCH_YR_END);
    if (valid != DEF_OK) {
        CLK_TRACE_DBG(("    Date/time conversion has failed\n\r"));
        return  (DEF_FAIL);
    }

    return (DEF_OK);
}
#endif


/*
*********************************************************************************************************
*                                       Clk_DateTimeToTS_Unix()
*
* Description : Convert a date/time structure to Unix timestamp.
*
* Argument(s) : p_ts_sec        Pointer to variable that will receive the Unix timestamp :
*
*                                   In seconds UTC+00,  if NO error(s);
*                                   CLK_TS_SEC_NONE,    otherwise.
*
*               p_date_time     Pointer to variable that contains date/time structure to convert.
*
* Return(s)   : DEF_OK,   if timestamp successfully returned.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Date/time structure ('p_date_time') MUST be representable in Unix timestamp.
*                   Thus date to convert MUST be :
*
*                   (a) >= CLK_UNIX_EPOCH_YR_START
*                   (b) <  CLK_UNIX_EPOCH_YR_END
*
*               (2) (a) Date/time ('p_date_time') SHOULD be set to local time with correct  time zone
*                       offset ('p_date_time->TZ_sec'). Clk_DateTimeToTS_Unix() removes the time zone
*                       offset from the date/time to calculate & return a Clock timestamp at UTC+00.
*
*                   (b) Time zone is based on Coordinated Universal Time (UTC) & has valid values :
*
*                       (1) Between  +|- 12 hours (+|- 43200 seconds)
*                       (2) Multiples of 15 minutes
*
*               (3) Pointers to variables that return values MUST be initialized PRIOR to all other
*                   validation or function handling in case of any error(s).
*********************************************************************************************************
*/

#if (CLK_CFG_UNIX_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_DateTimeToTS_Unix (CLK_TS_SEC     *p_ts_unix_sec,
                                    CLK_DATE_TIME  *p_date_time)
{
    CPU_BOOLEAN  valid;


#if (CLK_CFG_ARG_CHK_EN == DEF_ENABLED)                         /* ----------------- VALIDATE TS PTR ------------------ */
    if (p_ts_unix_sec == (CLK_TS_SEC *)0) {
        return (DEF_FAIL);
    }
#endif

   *p_ts_unix_sec = CLK_TS_SEC_NONE;                            /* Init to ts none for err (see Note #3).               */


#if 0                                                           /* Validated in Clk_IsUnixDateTimeValid().              */
    if (p_date_time == (CLK_DATE_TIME *)0) {                    /* -------------- VALIDATE DATE/TIME PTR -------------- */
        return (DEF_FAIL);
    }
#endif

                                                                /* ---------------- VALIDATE DATE/TIME ---------------- */
    valid = Clk_IsUnixDateTimeValid(p_date_time);
    if (valid != DEF_OK) {
        return  (DEF_FAIL);
    }

                                                                /* ------------ CONV DATE/TIME TO UNIX TS ------------- */
    CLK_TRACE_DBG(("Convert Date/time to TS:\n\r"));
    valid = Clk_DateTimeToTS_Handler(p_ts_unix_sec,
                                     p_date_time,
                                     CLK_UNIX_EPOCH_YR_START);
    if (valid != DEF_OK) {
        return  (DEF_FAIL);
    }

    return (DEF_OK);
}
#endif


/*
*********************************************************************************************************
*                                       Clk_UnixDateTimeMake()
*
* Description : Build a valid Unix epoch date/time structure.
*
* Argument(s) : p_date_time     Pointer to variable that will receive the date/time structure.
*
*               yr              Year    value [CLK_UNIX_EPOCH_YR_START to CLK_UNIX_EPOCH_YR_END) (see Note #1).
*
*               month           Month   value [          CLK_MONTH_JAN to         CLK_MONTH_DEC].
*
*               day             Day     value [                      1 to                    31].
*
*               hr              Hours   value [                      0 to                    23].
*
*               min             Minutes value [                      0 to                    59].
*
*               sec             Seconds value [                      0 to                    60] (see Note #3).
*
*               tz_sec          Time zone offset (in seconds, +|- from UTC) [-43200 to 43200].
*
* Return(s)   : DEF_OK,   if date/time structure successfully returned.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Date/time structure ('p_date_time') MUST be representable in Unix timestamp.
*                   Thus date to convert MUST be :
*
*                   (a) >= CLK_UNIX_EPOCH_YR_START
*                   (b) <  CLK_UNIX_EPOCH_YR_END
*
*               (2) Day of week ('p_date_time->DayOfWk') and Day of year ('p_date_time->DayOfYr')
*                   are internally calculated and set in the date/time structure if date is valid.
*
*               (3) Seconds value of 60 is valid to be compatible with leap second adjustment and
*                   the atomic clock time structure.
*
*               (4) Time zone is based ('tz_sec') on Coordinated Universal Time (UTC) & has valid
*                   values :
*
*                   (a) Between  +|- 12 hours (+|- 43200 seconds)
*                   (b) Multiples of 15 minutes
*********************************************************************************************************
*/

#if (CLK_CFG_UNIX_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_UnixDateTimeMake (CLK_DATE_TIME  *p_date_time,
                                   CLK_YR          yr,
                                   CLK_MONTH       month,
                                   CLK_DAY         day,
                                   CLK_HR          hr,
                                   CLK_MIN         min,
                                   CLK_SEC         sec,
                                   CLK_TZ_SEC      tz_sec)
{
    CPU_BOOLEAN  valid;

                                                                /* ---------- VALIDATE & CONV UNIX DATE/TIME ---------- */
    valid = Clk_DateTimeMakeHandler(p_date_time,
                                    yr,
                                    month,
                                    day,
                                    hr,
                                    min,
                                    sec,
                                    tz_sec,
                                    CLK_UNIX_EPOCH_YR_START,
                                    CLK_UNIX_EPOCH_YR_END);
    if (valid != DEF_YES) {
        return  (DEF_FAIL);
    }

    return (DEF_OK);
}
#endif


/*
*********************************************************************************************************
*                                      Clk_IsUnixDateTimeValid()
*
* Description : Determine if date/time structure is valid in Unix epoch.
*
* Argument(s) : p_date_time     Pointer to variable that contains the date/time structure to validate.
*
* Return(s)   : DEF_YES, if date/time structure is valid.
*
*               DEF_NO,  otherwise.
*
* Note(s)     : (1) Date/time structure ('p_date_time') MUST be representable in Unix epoch. Thus
*                   date to validate MUST be :
*
*                   (a) >= CLK_UNIX_EPOCH_YR_START
*                   (b) <  CLK_UNIX_EPOCH_YR_END
*
*               (2) Time zone is based ('p_date_time->TZ_sec') on Coordinated Universal Time (UTC)
*                   & has valid values :
*
*                   (a) Between  +|- 12 hours (+|- 43200 seconds)
*                   (b) Multiples of 15 minutes
*********************************************************************************************************
*/

#if (CLK_CFG_UNIX_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_IsUnixDateTimeValid (CLK_DATE_TIME  *p_date_time)
{
    CPU_BOOLEAN  valid;

                                                                /* ------------- VALIDATE UNIX DATE/TIME -------------- */
    CLK_TRACE_DBG(("Validate Unix date/time: "));

    valid = Clk_IsDateTimeValidHandler(p_date_time,
                                       CLK_UNIX_EPOCH_YR_START,
                                       CLK_UNIX_EPOCH_YR_END);
    if (valid != DEF_YES) {
        CLK_TRACE_DBG(("Fail\n\r"));
        return  (DEF_FAIL);
    }

    CLK_TRACE_DBG(("Ok\n\r"));

    return (DEF_YES);
}
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           Clk_IsLeapYr()
*
* Description : Determines if year is a leap year.
*
* Argument(s) : yr          Year value [1900 to  2135].
*
* Return(s)   : DEF_YES, if 'yr' is     a leap year.
*
*               DEF_NO,  if 'yr' is NOT a leap year.
*
* Note(s)     : (1) (a) Most years that are evenly divisible by 4 are leap years; ...
*                   (b) (1) except years that are evenly divisible by 100,        ...
*                       (2) unless they  are also evenly divisible by 400.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Clk_IsLeapYr (CLK_YR  yr)
{
    CPU_BOOLEAN  leap_yr;


    leap_yr = ( ((yr %   4u) == 0u) &&                          /* Chk for leap yr (see Note #1).                       */
               (((yr % 100u) != 0u) || ((yr % 400u) == 0u))) ? DEF_YES : DEF_NO;

    return (leap_yr);
}


/*
*********************************************************************************************************
*                                          Clk_IsDateValid()
*
* Description : Determine if date values are valid.
*
* Argument(s) : yr          Year  value [yr_start to  yr_end].
*
*               month       Month value [       1 to      12] / [CLK_MONTH_JAN to CLK_MONTH_DEC].
*
*               day         Day   value [       1 to      31].
*
*               yr_start    Start year of the epoch.
*
*               yr_end      End   year of the epoch.
*
* Return(s)   : DEF_YES, if date is valid.
*
*               DEF_NO,  otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Clk_IsDateValid (CLK_YR     yr,
                                      CLK_MONTH  month,
                                      CLK_DAY    day,
                                      CLK_YR     yr_start,
                                      CLK_YR     yr_end)
{
    CPU_BOOLEAN  leap_yr;
    CPU_INT08U   leap_yr_ix;
    CLK_DAY      days_in_month;

                                                                /* ------------------- VALIDATE YR -------------------- */
    if ((yr <  yr_start) ||
        (yr >= yr_end  )) {
         CLK_TRACE_DBG(("Invalid year, must be > %u & < %u\n\r",
                        (unsigned int)yr_end,
                        (unsigned int)yr_start));
         return (DEF_NO);
    }

                                                                /* ------------------ VALIDATE MONTH ------------------ */
    if ((month < CLK_FIRST_MONTH_OF_YR) ||
        (month > CLK_MONTH_PER_YR     )) {
        CLK_TRACE_DBG(("Invalid year, must be >= %u & < %u\n\r",
                       (unsigned int)CLK_FIRST_MONTH_OF_YR,
                       (unsigned int)CLK_MONTH_PER_YR));
        return (DEF_NO);
    }

                                                                /* ------------------- VALIDATE DAY ------------------- */
    leap_yr       =  Clk_IsLeapYr(yr);
    leap_yr_ix    = (leap_yr == DEF_YES) ? 1u : 0u;
    days_in_month =  Clk_DaysInMonth[leap_yr_ix][month - CLK_FIRST_MONTH_OF_YR];
    if ((day < CLK_FIRST_DAY_OF_MONTH) ||
        (day > days_in_month         )) {
         CLK_TRACE_DBG(("Invalid day, must be > %u & < %u\n\r",
                        (unsigned int)CLK_FIRST_DAY_OF_MONTH,
                        (unsigned int)days_in_month));
         return (DEF_NO);
    }


    return (DEF_YES);
}


/*
*********************************************************************************************************
*                                        Clk_IsDayOfYrValid()
*
* Description : Determine if day of year is valid.
*
* Argument(s) : yr          Year        value [1900 to 2135].
*
*               day_of_yr   Day of year value [1    to   31].
*
* Return(s)   : DEF_YES, if day of year is valid.
*
*               DEF_NO,  otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Clk_IsDayOfYrValid (CLK_YR   yr,
                                         CLK_DAY  day_of_yr)
{
    CPU_BOOLEAN  leap_yr;
    CPU_INT08U   leap_yr_ix;
    CLK_DAY      yr_days_max;

                                                                /* ---------------- VALIDATE DAY OF YR ---------------- */
    leap_yr     =  Clk_IsLeapYr(yr);
    leap_yr_ix  = (leap_yr == DEF_YES) ? 1u : 0u;
    yr_days_max =  Clk_DaysInYr[leap_yr_ix];
    if ((day_of_yr < CLK_FIRST_DAY_OF_YR) ||
        (day_of_yr > yr_days_max        )) {
        CLK_TRACE_DBG(("Invalid day of year, must be >= %u & < %u\n\r",
                       (unsigned int)CLK_FIRST_DAY_OF_YR,
                       (unsigned int)yr_days_max));
        return (DEF_NO);
    }

    return (DEF_YES);
}


/*
*********************************************************************************************************
*                                        Clk_IsDayOfWkValid()
*
* Description : Determine if day of week is valid.
*
* Argument(s) : day_of_wk   Day of week value [1 to 7] / [CLK_DAY_OF_WK_SUN to CLK_DAY_OF_WK_SAT].
*
* Return(s)   : DEF_YES, if day of week is valid.
*
*               DEF_NO,  otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Clk_IsDayOfWkValid (CLK_DAY  day_of_wk)
{
                                                                /* ---------------- VALIDATE DAY OF WK ---------------- */
    if ((day_of_wk < CLK_FIRST_DAY_OF_WK    ) ||
        (day_of_wk > DEF_TIME_NBR_DAY_PER_WK)) {
        CLK_TRACE_DBG(("Invalid day of week, must be >= %u & < %u\n\r",
                       (unsigned int)CLK_FIRST_DAY_OF_WK,
                       (unsigned int)DEF_TIME_NBR_DAY_PER_WK));
        return (DEF_NO);
    }

    return (DEF_YES);
}


/*
*********************************************************************************************************
*                                          Clk_IsTimeValid()
*
* Description : Determine if time values are valid.
*
* Argument(s) : hr          Hours   value [0 to 23].
*
*               min         Minutes value [0 to 59].
*
*               sec         Seconds value [0 to 60] (see Note #1).
*
* Return(s)   : DEF_YES, if time is valid.
*
*               DEF_NO,  otherwise.
*
* Note(s)     : (1) Seconds value of 60 is valid to be compatible with leap second adjustment and
*                   the atomic clock time structure.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Clk_IsTimeValid (CLK_HR     hr,
                                      CLK_MONTH  min,
                                      CLK_DAY    sec)
{
                                                                /* ------------------ VALIDATE HOUR ------------------- */
    if (hr  >= DEF_TIME_NBR_HR_PER_DAY) {
        CLK_TRACE_DBG(("Invalid hour, must be < %u\n\r",    (unsigned int)DEF_TIME_NBR_HR_PER_DAY));
        return (DEF_NO);
    }

                                                                /* ------------------- VALIDATE MIN ------------------- */
    if (min >= DEF_TIME_NBR_MIN_PER_HR) {
        CLK_TRACE_DBG(("Invalid minute, must be < %u\n\r",  (unsigned int)DEF_TIME_NBR_MIN_PER_HR));
        return (DEF_NO);
    }

                                                                /* ------------------- VALIDATE SEC ------------------- */
    if (sec >  DEF_TIME_NBR_SEC_PER_MIN) {
        CLK_TRACE_DBG(("Invalid second, must be =< %u\n\r", (unsigned int)DEF_TIME_NBR_SEC_PER_MIN));
        return (DEF_NO);
    }

    return (DEF_YES);
}


/*
*********************************************************************************************************
*                                           Clk_IsTZValid()
*
* Description : Determine if time zone is valid.
*
* Argument(s) : tz_sec      Time zone offset (in seconds, +|- from UTC) [-43200 to 43200].
*
* Return(s)   : DEF_YES, if time zone is valid.
*
*               DEF_NO,  otherwise.
*
* Note(s)     : (1) Time zone is based ('tz_sec') on Coordinated Universal Time (UTC) & has valid values :
*
*                   (a) Between  +|- 12 hours (+|- 43200 seconds)
*                   (b) Multiples of 15 minutes
*
*               (2) Absolute value of the time zone offset is stored into 'CLK_TS_SEC' data type to be
*                   compliant with unsigned integer verification/operations.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Clk_IsTZValid (CLK_TZ_SEC  tz_sec)
{
    CLK_TS_SEC  tz_sec_abs;                                     /* See Note #2.                                         */

                                                                /* -------------- VALIDATE TZ PRECISION --------------- */
    tz_sec_abs = DEF_ABS(tz_sec);
    if ((tz_sec_abs % CLK_TZ_SEC_PRECISION) != 0u) {            /* See Note #1b.                                        */
        CLK_TRACE_DBG(("Invalid time zone, must be multiple of %d seconds\n\r",
                       (  signed int)CLK_TZ_SEC_PRECISION));
        return (DEF_NO);
    }

                                                                /* --------------- VALIDATE TZ MIN-MAX ---------------- */
    if (tz_sec_abs > CLK_TZ_SEC_MAX) {                          /* See Note #1a.                                        */
        CLK_TRACE_DBG(("Invalid time zone, must be > %d & < %u\n\r",
                       (  signed int)CLK_TZ_SEC_MIN,
                       (unsigned int)CLK_TZ_SEC_MAX));
        return (DEF_NO);
    }

    return (DEF_YES);
}


/*
*********************************************************************************************************
*                                    Clk_IsDateTimeValidHandler()
*
* Description : Determine if date/time structure is valid.
*
* Argument(s) : p_date_time     Pointer to variable that contains the date/time structure to validate.
*
*               yr_start        Start year of the epoch.
*
*               yr_end          End   year of the epoch.
*
* Return(s)   : DEF_YES, date/time structure is valid.
*
*               DEF_NO,  otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Clk_IsDateTimeValidHandler (CLK_DATE_TIME  *p_date_time,
                                                 CLK_YR          yr_start,
                                                 CLK_YR          yr_end)
{
    CPU_BOOLEAN  valid;


#if (CLK_CFG_ARG_CHK_EN == DEF_ENABLED)                         /* -------------- VALIDATE DATE/TIME PTR -------------- */
    if (p_date_time == (CLK_DATE_TIME *)0) {
        return (DEF_NO);
    }
#endif

                                                                /* ------------------ VALIDATE DATE ------------------- */
    valid = Clk_IsDateValid(p_date_time->Yr, p_date_time->Month, p_date_time->Day, yr_start, yr_end);
    if (valid != DEF_YES) {
        return  (DEF_NO);
    }

                                                                /* ------------------ VALIDATE TIME ------------------- */
    valid = Clk_IsTimeValid(p_date_time->Hr, p_date_time->Min, p_date_time->Sec);
    if (valid != DEF_YES) {
        return  (DEF_NO);
    }

                                                                /* ---------------- VALIDATE DAY OF WK ---------------- */
    valid = Clk_IsDayOfWkValid(p_date_time->DayOfWk);
    if (valid != DEF_YES) {
        return  (DEF_NO);
    }

                                                                /* ---------------- VALIDATE DAY OF YR ---------------- */
    valid = Clk_IsDayOfYrValid(p_date_time->Yr, p_date_time->DayOfYr);
    if (valid != DEF_YES) {
        return  (DEF_NO);
    }

                                                                /* ------------------- VALIDATE TZ -------------------- */
    valid = Clk_IsTZValid(p_date_time->TZ_sec);
    if (valid != DEF_YES) {
        return  (DEF_NO);
    }


    return (DEF_YES);
}


/*
*********************************************************************************************************
*                                       Clk_GetDayOfYrHandler()
*
* Description : Get the day of year.
*
* Argument(s) : yr          Year  value [1900 to 2135].
*               --          Argument checked in Clk_TS_ToDateTimeHandler(),
*                                               Clk_DateTimeMakeHandler().
*
*               month       Month value [1 to 12] / [CLK_MONTH_JAN to CLK_MONTH_DEC].
*               -----       Argument checked in Clk_TS_ToDateTimeHandler(),
*                                               Clk_DateTimeMakeHandler().
*
*               day         Day   value [1 to 31].
*               ---         Argument checked in Clk_TS_ToDateTimeHandler(),
*                                               Clk_DateTimeMakeHandler().
*
* Return(s)   : Day of year [1 to 366].
*
* Note(s)     : (1) Since global performance can be affected by the calculation of the days of year some
*                   caches have been implemented:
*
*                   (a) Clk_CacheMonth store the month requested.
*
*                   (b) Clk_CacheMonthDays store the number of days calculated for the month stored in
*                       Clk_CacheMonth
*
*                   (c) Clk_CacheLeapYr stores if the previous year was a leap year or not.
*********************************************************************************************************
*/

static  CLK_DAY  Clk_GetDayOfYrHandler (CLK_YR     yr,
                                        CLK_MONTH  month,
                                        CLK_DAY    day)
{
    CPU_BOOLEAN  leap_yr;
    CPU_INT08U   leap_yr_ix;
    CLK_MONTH    month_ix;
    CLK_DAY      day_of_yr;
    CLK_DAY      days_month;
    CLK_DAY      days_in_month;
    CLK_MONTH    cached_month;
    CLK_DAY      cached_month_days;
    CPU_INT08U   cached_leap_yr;
    CPU_SR_ALLOC();


    CLK_TRACE_DBG(("Day of year of  %u, %u, %u = ",
                   (unsigned int)day,
                   (unsigned int)month,
                   (unsigned int)yr));

    day_of_yr  =  day - CLK_FIRST_DAY_OF_MONTH;
    leap_yr    =  Clk_IsLeapYr(yr);
    leap_yr_ix = (leap_yr == DEF_YES) ? 1u : 0u;

    CPU_CRITICAL_ENTER();
    cached_month      = Clk_CacheMonth;
    cached_month_days = Clk_CacheMonthDays;
    cached_leap_yr    = Clk_CacheLeapYr;
    CPU_CRITICAL_EXIT();
                                                                /* Check if cached values can be used.                  */
    if ((cached_month   != month) ||                            /* See Note #1a.                                        */
        (cached_leap_yr != leap_yr)) {                          /* See Note #1c.                                        */
        days_month = 0u;
        for (month_ix = CLK_FIRST_MONTH_OF_YR; month_ix < month; month_ix++) {
            days_in_month  = Clk_DaysInMonth[leap_yr_ix][month_ix - CLK_FIRST_MONTH_OF_YR];
            days_month    += days_in_month;
        }

        CPU_CRITICAL_ENTER();
        Clk_CacheMonth     = month;                             /* See Note #1a.                                        */
        Clk_CacheMonthDays = days_month;                        /* See Note #1b.                                        */
        Clk_CacheLeapYr    = leap_yr;
        CPU_CRITICAL_EXIT();

    } else {
        days_month = cached_month_days;                         /* See Note #1b.                                        */
    }

    day_of_yr += days_month;
    day_of_yr += CLK_FIRST_DAY_OF_YR;
    CLK_TRACE_DBG(("Day of year = %u\n\r", (unsigned int)day_of_yr));

    return (day_of_yr);
}


/*
*********************************************************************************************************
*                                       Clk_GetDayOfWkHandler()
*
* Description : Get the day of week.
*
* Argument(s) : yr          Year  value [1900 to 2135].
*               --          Argument checked in Clk_GetDayOfWk(),
*                                               Clk_TS_ToDateTimeHandler(),
*                                               Clk_DateTimeMakeHandler().
*
*               month       Month value [1 to 12] / [CLK_MONTH_JAN to CLK_MONTH_DEC].
*               -----       Argument checked in Clk_GetDayOfWk(),
*                                               Clk_TS_ToDateTimeHandler(),
*                                               Clk_DateTimeMakeHandler().
*
*               day         Day   value [1 to 31].
*               ---         Argument checked in Clk_GetDayOfWk(),
*                                               Clk_TS_ToDateTimeHandler(),
*                                               Clk_DateTimeMakeHandler().
*
* Return(s)   : Day of week [1 to 7] / [CLK_DAY_OF_WK_SUN to CLK_DAY_OF_WK_SAT].
*
* Note(s)     : (1) Finds the day of week at the earliest supported year (NTP) and its first day of week :
*
*                   (a) CLK_NTP_EPOCH_YR_START
*                   (b) CLK_NTP_EPOCH_DAY_OF_WK
*
*               (2) Since global performance can be affected by the calculation of the days of week some
*                   caches have been implemented:
*
*                   (a) Clk_CacheMonth store the last month requested.
*
*                   (b) Clk_CacheMonthDays store the last number of days calculated for the month stored in
*                       Clk_CacheMonth.
*
*                   (c) Clk_CacheYr store the last year requested.
*
*                   (d) Clk_CacheYrDays store the last number of days calculated for the year stored in
*                       Clk_CacheYr.
*
*                   (e) Clk_CacheLeapYr stores if the previous year was a leap year or not.
*********************************************************************************************************
*/

static  CLK_DAY  Clk_GetDayOfWkHandler (CLK_YR     yr,
                                        CLK_MONTH  month,
                                        CLK_DAY    day)
{
    CPU_BOOLEAN   leap_yr;
    CPU_INT08U    leap_yr_ix;
    CLK_YR        yr_ix;
    CLK_MONTH     month_ix;
    CLK_DAY       day_of_wk;
    CLK_DAY       days_in_month;
    CLK_DAY       days_month;
    CLK_NBR_DAYS  days_yr;
    CLK_NBR_DAYS  days;
    CLK_YR        cached_yr;
    CLK_NBR_DAYS  cached_yr_days;
    CLK_MONTH     cached_month;
    CLK_DAY       cached_month_days;
    CPU_INT08U    cached_leap_yr;
    CPU_SR_ALLOC();


    CLK_TRACE_DBG(("Day of week of  %u, %u, %u = ",
                   (unsigned int)day,
                   (unsigned int)month,
                   (unsigned int)yr));

    days       =  day - CLK_FIRST_DAY_OF_MONTH;
    leap_yr    =  Clk_IsLeapYr(yr);
    leap_yr_ix = (leap_yr == DEF_YES) ? 1u : 0u;

    CPU_CRITICAL_ENTER();
    cached_yr         = Clk_CacheYr;
    cached_yr_days    = Clk_CacheYrDays;
    cached_month      = Clk_CacheMonth;
    cached_month_days = Clk_CacheMonthDays;
    cached_leap_yr    = Clk_CacheLeapYr;
    CPU_CRITICAL_EXIT();

                                                                /* Check if cached values can be used.                  */
    if ((cached_month   != month) ||                            /* See Note #2a.                                        */
        (cached_leap_yr != leap_yr)) {                          /* See Note #2e.                                        */
        days_month = 0u;
        for (month_ix = CLK_FIRST_MONTH_OF_YR; month_ix < month; month_ix++) {
            days_in_month  = Clk_DaysInMonth[leap_yr_ix][month_ix - CLK_FIRST_MONTH_OF_YR];
            days_month    += days_in_month;
        }

        CPU_CRITICAL_ENTER();
        Clk_CacheMonth     = month;                             /* See Note #2a.                                        */
        Clk_CacheMonthDays = days_month;                        /* See Note #2b.                                        */
        Clk_CacheLeapYr    = leap_yr;
        CPU_CRITICAL_EXIT();

    } else {
        days_month = cached_month_days;                         /* See Note #2b.                                        */
    }


    if (cached_yr != yr) {                                      /* See Note #2c.                                        */
        days_yr    = 0u;
                                                                /* See Note #1a.                                        */
        for (yr_ix = CLK_NTP_EPOCH_YR_START; yr_ix < yr; yr_ix++) {
            leap_yr     =  Clk_IsLeapYr(yr_ix);
            leap_yr_ix  = (leap_yr == DEF_YES) ? 1u : 0u;
            days_yr    +=  Clk_DaysInYr[leap_yr_ix];
        }

        CPU_CRITICAL_ENTER();
        Clk_CacheYr     = yr;                                   /* See Note #2c.                                        */
        Clk_CacheYrDays = days_yr;                              /* See Note #2d.                                        */
        CPU_CRITICAL_EXIT();

    } else {
        days_yr = cached_yr_days;                               /* See Note #2b.                                        */
    }


    days      += days_month;
    days      += days_yr;
    days      += CLK_NTP_EPOCH_DAY_OF_WK;                       /* See Note #1b.                                        */
    days      -= CLK_FIRST_DAY_OF_WK;
    day_of_wk  = days % DEF_TIME_NBR_DAY_PER_WK;
    day_of_wk += CLK_FIRST_DAY_OF_WK;
    CLK_TRACE_DBG(("Day of week = %u)\n\r", (unsigned int)day_of_wk));

    return (day_of_wk);
}


/*
*********************************************************************************************************
*                                         Clk_SetTZ_Handler()
*
* Description : Set Clock time zone offset.
*
* Argument(s) : tz_sec      Time zone offset (in seconds, +|- from UTC).
*               ------      Argument checked by Clk_SetTZ(),
*                                               Clk_SetDateTime().
*
* Return(s)   : none.
*
* Note(s)     : (1) 'Clk_TZ_sec' MUST ALWAYS be accessed exclusively in critical sections.
*********************************************************************************************************
*/

static  void  Clk_SetTZ_Handler (CLK_TZ_SEC  tz_sec)
{
    CPU_SR_ALLOC();

                                                                /* ---------------------- SET TZ ---------------------- */
    CPU_CRITICAL_ENTER();
    Clk_TZ_sec = tz_sec;
    CPU_CRITICAL_EXIT();
}


/*
*********************************************************************************************************
*                                     Clk_TS_ToDateTimeHandler()
*
* Description : Convert any type of timestamp to date/time structure.
*
* Argument(s) : ts_sec          Timestamp to convert (in seconds,          UTC+00).
*
*               tz_sec          Time zone offset     (in seconds, +|- from UTC).
*
*               p_date_time     Pointer to variable that contains the date/time structure to validate.
*
*               yr_start        Start year of the epoch.
*
*               yr_end          End   year of the epoch.
*
* Return(s)   : DEF_OK,   if date/time structure is valid.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) (a) Timestamp ('ts_sec') MUST be set for UTC+00 & SHOULD NOT include the time
*                       zone offset ('tz_sec') since Clk_TS_ToDateTimeHandler() includes the time
*                       zone offset in its date/time calculation. Thus the time zone offset SHOULD
*                       NOT be applied before or after calling Clk_TS_ToDateTimeHandler().
*
*                   (b) Time zone is based on Coordinated Universal Time (UTC) & has valid values :
*
*                       (1) Between  +|- 12 hours (+|- 43200 seconds)
*                       (2) Multiples of 15 minutes
*
*                   (c) Timestamp MUST be adjusted by adding timezone offset ('tz_sec') :
*
*                       (1) For negative timezone offset, it's subtracted
*                       (2) For positive timezone offset, it's added
*
*                   (d) Time zone field of the date/time structure ('p_date_time->TZ_sec') is set
*                       to the value of time zone argument ('tz_sec').
*
*               (2) Absolute value of the time zone offset is stored into 'CLK_TS_SEC' data type to be
*                   compliant with unsigned integer verification/operations.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Clk_TS_ToDateTimeHandler (CLK_TS_SEC      ts_sec,
                                               CLK_TZ_SEC      tz_sec,
                                               CLK_DATE_TIME  *p_date_time,
                                               CLK_YR          yr_start,
                                               CLK_YR          yr_end)
{
    CLK_TS_SEC    ts_sec_rem;
    CLK_TS_SEC    tz_sec_abs;                                   /* See Note #2.                                         */
    CLK_TS_SEC    sec_to_remove;
    CLK_NBR_DAYS  days_in_yr;
    CLK_DAY       days_in_month;
    CPU_INT08U    leap_yr_ix;
    CPU_BOOLEAN   leap_yr;
    CPU_BOOLEAN   valid;


#if (CLK_CFG_ARG_CHK_EN == DEF_ENABLED)                         /* -------------- VALIDATE DATE/TIME PTR -------------- */
    if (p_date_time == (CLK_DATE_TIME *)0) {
        return (DEF_FAIL);
    }
#endif


                                                                /* ------------ ADJ INIT TS FOR TZ OFFSET ------------- */
    ts_sec_rem = ts_sec;
    tz_sec_abs = DEF_ABS(tz_sec);
    if (tz_sec < 0) {
        if (ts_sec_rem < tz_sec_abs) {                          /* Chk for ovf when tz is neg.                          */
            CLK_TRACE_DBG(("    Timestamp is too small to substract time zone offset\n\r"));
            return (DEF_FAIL);
        }
        ts_sec_rem -= tz_sec_abs;                               /* See Note #1c1.                                       */

    } else {
        ts_sec_rem += tz_sec_abs;                               /* See Note #1c2.                                       */
        if (ts_sec_rem < tz_sec_abs) {                          /* Chk for ovf when tz is pos.                          */
            CLK_TRACE_DBG(("    Timestamp is too big to add time zone offset\n\r"));
            return (DEF_FAIL);
        }
    }


                                                                /* ---------------------- GET YR ---------------------- */
    p_date_time->Yr =  yr_start;
    leap_yr         =  Clk_IsLeapYr(p_date_time->Yr);
    leap_yr_ix      = (leap_yr == DEF_YES) ? 1u : 0u;
    days_in_yr      =  Clk_DaysInYr[leap_yr_ix];
    sec_to_remove   =  days_in_yr * DEF_TIME_NBR_SEC_PER_DAY;
    while ((ts_sec_rem      >= sec_to_remove) &&
           (p_date_time->Yr <  yr_end)) {
        ts_sec_rem    -=  sec_to_remove;
        p_date_time->Yr++;
        leap_yr        =  Clk_IsLeapYr(p_date_time->Yr);
        leap_yr_ix     = (leap_yr == DEF_YES) ? 1u : 0u;
        days_in_yr     =  Clk_DaysInYr[leap_yr_ix];
        sec_to_remove  =  days_in_yr * DEF_TIME_NBR_SEC_PER_DAY;
    }

    if (p_date_time->Yr >= yr_end) {
        CLK_TRACE_DBG(("    Year conversion has failed\n\r"));
        return (DEF_FAIL);
    }

                                                                /* -------------------- GET MONTH --------------------- */
    p_date_time->Month =  CLK_FIRST_MONTH_OF_YR;
#if 0                                                           /* Already determined in 'GET YR'.                      */
    leap_yr            =  Clk_IsLeapYr(p_date_time->Yr);
    leap_yr_ix         = (leap_yr == DEF_YES) ? 1u : 0u;
#endif
    days_in_month      =  Clk_DaysInMonth[leap_yr_ix][p_date_time->Month - CLK_FIRST_MONTH_OF_YR];
    sec_to_remove      =  days_in_month * DEF_TIME_NBR_SEC_PER_DAY;
    while ((ts_sec_rem         >= sec_to_remove) &&
           (p_date_time->Month <  CLK_MONTH_PER_YR)) {
        ts_sec_rem    -=  sec_to_remove;
        p_date_time->Month++;
        days_in_month  =  Clk_DaysInMonth[leap_yr_ix][p_date_time->Month - CLK_FIRST_MONTH_OF_YR];
        sec_to_remove  =  days_in_month * DEF_TIME_NBR_SEC_PER_DAY;
    }

    if (p_date_time->Month > CLK_MONTH_PER_YR) {
        CLK_TRACE_DBG(("    Month conversion has failed\n\r"));
        return (DEF_FAIL);
    }

                                                                /* --------------------- GET DAY ---------------------- */
    sec_to_remove     = DEF_TIME_NBR_SEC_PER_DAY;
    p_date_time->Day  = ts_sec_rem / sec_to_remove;
    p_date_time->Day += CLK_FIRST_DAY_OF_MONTH;
    ts_sec_rem        = ts_sec_rem % sec_to_remove;
#if 0                                                           /* Already determined in 'GET MONTH'.                   */
    days_in_month     = Clk_DaysInMonth[leap_yr_ix][p_date_time->Month - CLK_FIRST_MONTH_OF_YR];
#endif

    if (p_date_time->Day > days_in_month) {
         CLK_TRACE_DBG(("    Day conversion has failed\n\r"));
         return (DEF_FAIL);
    }

                                                                /* ------------------ GET DAY OF WK ------------------- */
    p_date_time->DayOfWk = Clk_GetDayOfWkHandler(p_date_time->Yr,
                                                 p_date_time->Month,
                                                 p_date_time->Day);
    valid                = Clk_IsDayOfWkValid(p_date_time->DayOfWk);
    if (valid != DEF_OK) {
        CLK_TRACE_DBG(("    Day of week conversion has failed\n\r"));
        return (DEF_FAIL);
    }


    p_date_time->DayOfYr = Clk_GetDayOfYrHandler(p_date_time->Yr,
                                                 p_date_time->Month,
                                                 p_date_time->Day);
    valid                = Clk_IsDayOfYrValid(p_date_time->Yr,
                                              p_date_time->DayOfYr);
    if (valid != DEF_OK) {
        CLK_TRACE_DBG(("    Day of year conversion has failed\n\r"));
        return (DEF_FAIL);
    }

                                                                /* --------------------- GET HR ----------------------- */
    sec_to_remove       = DEF_TIME_NBR_SEC_PER_HR;
    p_date_time->Hr     = ts_sec_rem / sec_to_remove;
    ts_sec_rem          = ts_sec_rem % sec_to_remove;

                                                                /* --------------------- GET MIN ---------------------- */
    sec_to_remove       = DEF_TIME_NBR_SEC_PER_MIN;
    p_date_time->Min    = ts_sec_rem / sec_to_remove;
    ts_sec_rem          = ts_sec_rem % sec_to_remove;

                                                                /* --------------------- GET SEC ---------------------- */
    p_date_time->Sec    = ts_sec_rem;

                                                                /* --------------------- GET TZ ----------------------- */
    p_date_time->TZ_sec = tz_sec;


                                                                /* ------------------ VALIDATE TIME ------------------- */
    valid = Clk_IsTimeValid(p_date_time->Hr, p_date_time->Min, p_date_time->Sec);
    if (valid != DEF_OK) {
        CLK_TRACE_DBG(("    Time conversion has failed\n\r"));
        return  (DEF_FAIL);
    }

    CLK_TRACE_DBG(("Date/time converted:  \n\r"
                   "     Year         = %u\n\r"
                   "     Month        = %u\n\r"
                   "     Day          = %u\n\r"
                   "     Hour         = %u\n\r"
                   "     Minutes      = %u\n\r"
                   "     Seconds      = %u\n\r"
                   "     Time zone    = %d\n\r",
                   (unsigned int)p_date_time->Yr,
                   (unsigned int)p_date_time->Month,
                   (unsigned int)p_date_time->Day,
                   (unsigned int)p_date_time->Hr,
                   (unsigned int)p_date_time->Min,
                   (unsigned int)p_date_time->Sec,
                   (  signed int)p_date_time->TZ_sec));

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                     Clk_DateTimeToTS_Handler()
*
* Description : Convert date/time structure to any type of timestamp.
*
* Argument(s) : p_ts_sec        Pointer to variable that will receive the timestamp :
*               --------
*                                   In seconds UTC+00,  if NO error(s);
*                                   CLK_TS_SEC_NONE,    otherwise.
*
*                               Argument checked in Clk_DateTimeToTS(),
*                                                   Clk_DateTimeToTS_NTP(),
*                                                   Clk_DateTimeToTS_Unix().
*
*               p_date_time     Pointer to variable that contains the date/time structure to validate.
*               -----------     Argument checked in Clk_DateTimeToTS(),
*                                                   Clk_DateTimeToTS_NTP(),
*                                                   Clk_DateTimeToTS_Unix().
*
*               yr_start        Start year of the epoch.
*
* Return(s)   : DEF_OK,   if timestamp is valid.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) (a) Date/time ('p_date_time') SHOULD be set to local time with correct time zone
*                       offset ('p_date_time->TZ_sec'). Clk_DateTimeToTS_Handler() removes the time
*                       zone offset from the date/time to calculate & return a Clock timestamp at
*                       UTC+00.

*                   (b) Time zone is based on Coordinated Universal Time (UTC) & has valid values :
*
*                       (1) Between  +|- 12 hours (+|- 43200 seconds)
*                       (2) Multiples of 15 minutes
*
*                   (c) Timestamp MUST be adjusted by subtracting timezone offset ('p_date_time->TZ_sec') :
*
*                       (1) For negative timezone offset, it's added
*                       (2) For positive timezone offset, it's subtracted
*
*               (2) Pointers to variables that return values MUST be initialized PRIOR to all other
*                   validation or function handling in case of any error(s).
*
*                   (a) However, some pointers may already be initialized by calling functions. These
*                       pointers do NOT need to be re-initialized but are shown for completeness.
*
*               (3) Absolute value of the time zone offset is stored into 'CLK_TS_SEC' data type to be
*                   compliant with unsigned integer verification/operations.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Clk_DateTimeToTS_Handler (CLK_TS_SEC     *p_ts_sec,
                                               CLK_DATE_TIME  *p_date_time,
                                               CLK_YR          yr_start)
{
    CPU_BOOLEAN   leap_yr;
    CPU_INT08U    leap_yr_ix;
    CLK_YR        yr_ix;
    CLK_MONTH     month_ix;
    CLK_NBR_DAYS  nbr_days;
    CLK_TS_SEC    ts_sec;
    CLK_TS_SEC    tz_sec_abs;                                   /* See Note #3.                                         */


#if 0                                                           /* See Note #2a.                                        */
   *p_ts_sec = CLK_TS_SEC_NONE;                                 /* Init to ts none for err (see Note #2).               */
#endif

    CLK_TRACE_DBG(("Date/time converted:  \n\r"
                   "     Year         = %u\n\r"
                   "     Month        = %u\n\r"
                   "     Day          = %u\n\r"
                   "     Hour         = %u\n\r"
                   "     Minutes      = %u\n\r"
                   "     Seconds      = %u\n\r"
                   "     Time zone    = %d\n\r",
                   (unsigned int)p_date_time->Yr,
                   (unsigned int)p_date_time->Month,
                   (unsigned int)p_date_time->Day,
                   (unsigned int)p_date_time->Hr,
                   (unsigned int)p_date_time->Min,
                   (unsigned int)p_date_time->Sec,
                   (  signed int)p_date_time->TZ_sec));

                                                                /* ------------- CONV DATE/TIME TO CLK TS ------------- */
    nbr_days   =  p_date_time->Day - CLK_FIRST_DAY_OF_MONTH;
    leap_yr    =  Clk_IsLeapYr(p_date_time->Yr);
    leap_yr_ix = (leap_yr == DEF_YES) ? 1u : 0u;
    for (month_ix = CLK_FIRST_MONTH_OF_YR; month_ix < p_date_time->Month; month_ix++) {
        nbr_days += Clk_DaysInMonth[leap_yr_ix][month_ix - CLK_FIRST_MONTH_OF_YR];
    }

    for (yr_ix = yr_start; yr_ix < p_date_time->Yr; yr_ix++) {
        leap_yr     =  Clk_IsLeapYr(yr_ix);
        leap_yr_ix  = (leap_yr == DEF_YES) ? 1u : 0u;
        nbr_days   +=  Clk_DaysInYr[leap_yr_ix];
    }

    ts_sec  = nbr_days         * DEF_TIME_NBR_SEC_PER_DAY;
    ts_sec += p_date_time->Hr  * DEF_TIME_NBR_SEC_PER_HR;
    ts_sec += p_date_time->Min * DEF_TIME_NBR_SEC_PER_MIN;
    ts_sec += p_date_time->Sec;

                                                                /* ------------ ADJ FINAL TS FOR TZ OFFSET ------------ */
    tz_sec_abs = DEF_ABS(p_date_time->TZ_sec);
    if (p_date_time->TZ_sec < 0) {
        ts_sec += tz_sec_abs;                                   /* See Note #1c1.                                       */
        if (ts_sec < tz_sec_abs) {                              /* Chk for ovf when tz is neg.                          */
            CLK_TRACE_DBG(("    Timestamp is too big to add time zone offset\n\r"));
            return (DEF_FAIL);
        }

    } else {
        if (ts_sec < tz_sec_abs) {                              /* Chk for ovf when tz is pos.                          */
            CLK_TRACE_DBG(("    Timestamp is too small to substract time zone offset\n\r"));
            return (DEF_FAIL);
        }
        ts_sec -= tz_sec_abs;                                   /* See Note #1c2.                                       */
    }

   *p_ts_sec = ts_sec;
    CLK_TRACE_DBG(("    TS converted       = %u\n\r", (unsigned int)*p_ts_sec));

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                     Clk_DateTimeMakeHandler()
*
* Description : Build a valid epoch date/time structure.
*
* Argument(s) : p_date_time     Pointer to variable that will receive the date/time structure.
*
*               yr              Year    value [yr_start      to yr_end).
*
*               month           Month   value [CLK_MONTH_JAN to CLK_MONTH_DEC].
*
*               day             Day     value [            1 to            31].
*
*               hr              Hours   value [            0 to            23].
*
*               min             Minutes value [            0 to            59].
*
*               sec             Seconds value [            0 to            60] (see Note #2).
*
*               tz_sec          Time zone offset (in seconds, +|- from UTC) [-43200 to 43200].
*
*               yr_start        Start year of the epoch.
*
*               yr_end          End   year of the epoch.
*
* Return(s)   : DEF_OK,   if date/time structure is valid.
*
*               DEF_FAIL, otherwise.
*
* Note(s)     : (1) Day of week ('p_date_time->DayOfWk') and Day of year ('p_date_time->DayOfYr')
*                   are internally calculated and set in the date/time structure if date is valid.
*
*               (2) Seconds value of 60 is valid to be compatible with leap second adjustment and
*                   the atomic clock time structure.
*
*               (3) Time zone is based ('tz_sec') on Coordinated Universal Time (UTC) & has valid
*                   values :
*
*                   (a) Between  +|- 12 hours (+|- 43200 seconds)
*                   (b) Multiples of 15 minute
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Clk_DateTimeMakeHandler (CLK_DATE_TIME  *p_date_time,
                                              CLK_YR          yr,
                                              CLK_MONTH       month,
                                              CLK_DAY         day,
                                              CLK_HR          hr,
                                              CLK_MIN         min,
                                              CLK_SEC         sec,
                                              CLK_TZ_SEC      tz_sec,
                                              CLK_YR          yr_start,
                                              CLK_YR          yr_end)
{
    CPU_BOOLEAN  valid;


#if (CLK_CFG_ARG_CHK_EN == DEF_ENABLED)                         /* -------------- VALIDATE DATE/TIME PTR -------------- */
    if (p_date_time == (CLK_DATE_TIME *)0) {
        return (DEF_FAIL);
    }
#endif

                                                                /* ------------------ VALIDATE DATE ------------------- */
    valid = Clk_IsDateValid(yr, month, day, yr_start, yr_end);
    if (valid != DEF_YES) {
        return  (DEF_FAIL);
    }

    p_date_time->Yr      = yr;
    p_date_time->Month   = month;
    p_date_time->Day     = day;
    p_date_time->DayOfWk = Clk_GetDayOfWkHandler(yr, month, day);
    p_date_time->DayOfYr = Clk_GetDayOfYrHandler(yr, month, day);


                                                                /* ---------------- VALIDATE DAY OF WK ---------------- */
    valid = Clk_IsDayOfWkValid(p_date_time->DayOfWk);
    if (valid != DEF_YES) {
        return  (DEF_FAIL);
    }

                                                                /* ---------------- VALIDATE DAY OF YR ---------------- */
    valid = Clk_IsDayOfYrValid(yr, p_date_time->DayOfYr);
    if (valid != DEF_YES) {
        return  (DEF_FAIL);
    }

                                                                /* ------------------ VALIDATE TIME ------------------- */
    valid = Clk_IsTimeValid(hr, min, sec);
    if (valid != DEF_YES) {
        return  (DEF_FAIL);
    }

    p_date_time->Hr  = hr;
    p_date_time->Min = min;
    p_date_time->Sec = sec;

                                                                /* ------------------- VALIDATE TZ -------------------- */
    valid = Clk_IsTZValid(tz_sec);
    if (valid != DEF_YES) {
        return  (DEF_FAIL);
    }

    p_date_time->TZ_sec = tz_sec;

    return (DEF_OK);
}
