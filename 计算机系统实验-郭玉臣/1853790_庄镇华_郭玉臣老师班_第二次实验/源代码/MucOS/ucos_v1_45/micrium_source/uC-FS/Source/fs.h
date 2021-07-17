/*
*********************************************************************************************************
*                                                uC/FS
*                                      The Embedded File System
*
*                         (c) Copyright 2008-2019; Silicon Laboratories Inc.,
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
*                                    FILE SYSTEM SUITE HEADER FILE
*
* Filename : fs.h
* Version  : V4.07.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_H
#define  FS_H


/*
*********************************************************************************************************
*                                     FILE SYSTEM VERSION NUMBER
*
* Note(s) : (1) (a) The file system software version is denoted as follows :
*
*                       Vx.yy
*
*                           where
*                                   V               denotes 'Version' label
*                                   x               denotes major software version revision number
*                                   yy              denotes minor software version revision number
*                                   zz              denotes sub-minor software version revision number
*
*               (b) The software version label #define is formatted as follows :
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

#define  FS_VERSION                                    40707u   /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include  <cpu.h>
#include  <lib_ascii.h>
#include  <fs_cfg.h>
#include  "fs_cfg_fs.h"
#include  "fs_type.h"
#include  "fs_err.h"


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_MODULE
#define  FS_EXT
#else
#define  FS_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#ifdef   FS_STATIC_DISABLE
#define  FS_STATIC
#else
#define  FS_STATIC  static
#endif


/*
*********************************************************************************************************
*                                         TRIAL VERSION LIMITS
*********************************************************************************************************
*/

#ifndef  FS_TRIAL_MAX_DEV_CNT
#define  FS_TRIAL_MAX_DEV_CNT          1u
#endif

#ifndef  FS_TRIAL_MAX_VOL_CNT
#define  FS_TRIAL_MAX_VOL_CNT          1u
#endif

#ifndef  FS_TRIAL_MAX_FILE_CNT
#define  FS_TRIAL_MAX_FILE_CNT         1u
#endif

#ifndef  FS_TRIAL_MAX_DIR_CNT
#define  FS_TRIAL_MAX_DIR_CNT          1u
#endif

#ifndef  FS_TRIAL_MAX_BUF_CNT
#define  FS_TRIAL_MAX_BUF_CNT          4u
#endif


/*
*********************************************************************************************************
*                                    PATH CHARACTER/STRING DEFINES
*
* Note(s) : (1) See 'fs.h  PATH CHARACTER/STRING DEFINES  Note #1'.
*********************************************************************************************************
*/

#define  FS_CHAR_PATH_SEP             ((CPU_CHAR  )ASCII_CHAR_REVERSE_SOLIDUS)
#define  FS_CHAR_PATH_SEP_ALT         ((CPU_CHAR  )ASCII_CHAR_SOLIDUS)
#define  FS_CHAR_DEV_SEP              ((CPU_CHAR  )ASCII_CHAR_COLON)

#define  FS_STR_PATH_SEP              ((CPU_CHAR *)"\\")
#define  FS_STR_DEV_SEP               ((CPU_CHAR *)":")


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                 FILE SYSTEM CONFIGURATION DATA TYPE
*
* Note(s) : (1) The file system suite is configured at initialization via the 'FS_CFG' structure :
*
*               (a) 'DevCnt' is the maximum number of devices that can be open simultaneously. It MUST
*                    be between 1 & FS_QTY_NBR_MAX, inclusive.
*
*               (b) 'VolCnt' is the maximum number of volumes that can be open simultaneously. It MUST
*                    be between 1 & FS_QTY_NBR_MAX, inclusive.
*
*               (c) 'FileCnt' is the maximum number of files that can be open simultaneously. It MUST
*                    be between 1 & FS_QTY_NBR_MAX, inclusive.
*
*               (d) 'DirCnt' is the maximum number of devices that can be open simultaneously.
*                   (1) If 'DirCnt' is 0, the directory module functions will be blocked after successful
*                       initialization, & the file system will operate as if compiled with directory
*                       support disabled.
*                   (2) If directory support is disabled, 'DirCnt' is ignored.
*                   (3) Otherwise, 'DirCnt' MUST be between 1 & FS_QTY_NBR_MAX, inclusive.
*
*               (e) 'BufCnt' is the maximum number of buffers that can be used simultaneously.
*                   The minimum necessary 'BufCnt' can be calculated from the number of volumes :
*
*                       BufCnt >= VolCnt * 2
*
*                   (1) If 'FSEntry_Copy()' or 'FSEntry_Rename()' is used, then up to one additional
*                       buffer for each volume may be necessary.
*
*               (f) 'DevDrvCnt' is the maximum number of device drivers that can be added.  It MUST
*                    be between 1 & FS_QTY_NBR_MAX, inclusive.
*
*               (g) 'MaxSecSize' is the maximum sector size, in octets.  It MUST be 512, 1024, 2048 or
*                    4096.  No device with a sector size larger than 'MaxSecSize' can be opened.
*********************************************************************************************************
*/

typedef  struct  fs_cfg {
    FS_QTY       DevCnt;                                        /* Max nbr devices     that can be open simultaneously. */
    FS_QTY       VolCnt;                                        /* Max nbr volumes     that can be open simultaneously. */
    FS_QTY       FileCnt;                                       /* Max nbr files       that can be open simultaneously. */
    FS_QTY       DirCnt;                                        /* Max nbr directories that can be open simultaneously. */
    FS_QTY       BufCnt;                                        /* Max nbr buffers     that can be used simultaneously. */
    FS_QTY       DevDrvCnt;                                     /* Max nbr device drivers that can be added.            */
    FS_SEC_SIZE  MaxSecSize;                                    /* Max sec size.                                        */
} FS_CFG;


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/

#define  FS_ERR_CHK_EMPTY_RTN_ARG

#define  FS_ERR_CHK_RTNEXPR(call, callOnError, retExpr)  call; \
                                                         if (*p_err != FS_ERR_NONE) { \
                                                             callOnError; \
                                                             return retExpr; \
                                                         }

#define  FS_ERR_CHK_RTN(call, callOnError, retVal)       FS_ERR_CHK_RTNEXPR(call, callOnError, (retVal))

#define  FS_ERR_CHK(call, callOnError)                   FS_ERR_CHK_RTNEXPR(call, callOnError, FS_ERR_CHK_EMPTY_RTN_ARG)

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

                                                                    /* --------------------- CORE --------------------- */
FS_ERR       FS_Init              (FS_CFG        *p_fs_cfg);        /* File system startup function.                    */

CPU_INT16U   FS_VersionGet        (void);                           /* Get file system suite software version.          */

CPU_SIZE_T   FS_MaxSecSizeGet     (void);                           /* Get maximum sector size.                         */


                                                                    /* --------------- WORKING DIRECTORY -------------- */
#if (FS_CFG_WORKING_DIR_EN == DEF_ENABLED)
void         FS_WorkingDirGet     (CPU_CHAR      *path_dir,         /* Get the working directory for current task.      */
                                   CPU_SIZE_T     size,
                                   FS_ERR        *p_err);

void         FS_WorkingDirSet     (CPU_CHAR      *path_dir,         /* Set the working directory for current task.      */
                                   FS_ERR        *p_err);
#endif

/*
*********************************************************************************************************
*                                    INTERNAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

CPU_CHAR    *FS_PathParse         (CPU_CHAR      *name_full,        /* Parse full entry path.                           */
                                   CPU_CHAR      *name_vol,
                                   FS_ERR        *p_err);

#if (FS_CFG_WORKING_DIR_EN == DEF_ENABLED)
CPU_CHAR    *FS_WorkingDirPathForm(CPU_CHAR      *name_full,        /* Form full entry path.                            */
                                   FS_ERR        *p_err);

void         FS_WorkingDirObjFree (CPU_CHAR      *path_buf);        /* Free working dir obj.                            */
#endif


/*
*********************************************************************************************************
*                                               TRACING
*********************************************************************************************************
*/

                                                                /* Trace level, default to TRACE_LEVEL_OFF.             */
#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                                   0u
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                                  1u
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                                   2u
#endif

#ifndef  TRACE_LEVEL_LOG
#define  TRACE_LEVEL_LOG                                   3u
#endif


#if ((defined(FS_TRACE))       && \
     (defined(FS_TRACE_LEVEL)) && \
     (FS_TRACE_LEVEL >= TRACE_LEVEL_INFO))

    #if (FS_TRACE_LEVEL >= TRACE_LEVEL_LOG)
        #define  FS_TRACE_LOG(msg)     FS_TRACE  msg
    #else
        #define  FS_TRACE_LOG(msg)
    #endif


    #if (FS_TRACE_LEVEL >= TRACE_LEVEL_DBG)
        #define  FS_TRACE_DBG(msg)     FS_TRACE  msg
    #else
        #define  FS_TRACE_DBG(msg)
    #endif

    #define  FS_TRACE_INFO(msg)        FS_TRACE  msg

#else
    #define  FS_TRACE_LOG(msg)
    #define  FS_TRACE_DBG(msg)
    #define  FS_TRACE_INFO(msg)

#endif


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                      DEFINED IN PRODUCT'S BSP
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          FS_BSP_Dly_ms()
*
* Description : Delay for specified time, in milliseconds.
*
* Argument(s) : ms      Time delay value, in milliseconds.
*
* Return(s)   : none.
*
* Note(s)     : (1) FS_BSP_Dly_ms() is an application/BSP function that MUST be defined by the developer
*                   if no OS is used (when FS_OS_PRESENT is not #define'd in fs_os.h). Otherwise, the
*                   function FS_OS_Dly_ms() is used. This function is serviced by the OS, and is defined
*                   in the user OS port (fs_os.c).
*********************************************************************************************************
*/

#ifndef FS_OS_PRESENT
void         FS_BSP_Dly_ms        (CPU_INT16U     ms);              /* Delay for specified time, in milliseconds.       */
#endif

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                      DEFINED IN OS'S  fs_os.c
*********************************************`***********************************************************
*/

#if (FS_CFG_WORKING_DIR_EN == DEF_ENABLED)
CPU_CHAR    *FS_OS_WorkingDirGet  (void);                           /* Get working dir assigned to active task.         */

void         FS_OS_WorkingDirSet  (CPU_CHAR      *p_working_dir,    /* Assign working directory to active task.         */
                                   FS_ERR        *p_err);
#endif


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

