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
*                                  FILE SYSTEM SUITE FILE MANAGEMENT
*
* Filename : fs_file.h
* Version  : V4.07.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_FILE_H
#define  FS_FILE_H


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_FILE_MODULE
#define  FS_FILE_EXT
#else
#define  FS_FILE_EXT  extern
#endif


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  "fs_cfg_fs.h"
#include  "fs_type.h"
#include  "fs_ctr.h"
#include  "fs_err.h"


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         FILE STATE DEFINES
*********************************************************************************************************
*/

#define  FS_FILE_STATE_CLOSED                              0u   /* File closed.                                         */
#define  FS_FILE_STATE_CLOSING                             1u   /* File closing.                                        */
#define  FS_FILE_STATE_OPENING                             2u   /* File opening.                                        */
#define  FS_FILE_STATE_OPEN                                3u   /* File open.                                           */

/*
*********************************************************************************************************
*                                       FILE I/O STATE DEFINES
*********************************************************************************************************
*/

#define  FS_FILE_IO_STATE_NONE                             0u   /* None.                                                */
#define  FS_FILE_IO_STATE_RD                               1u   /* Reading from file.                                   */
#define  FS_FILE_IO_STATE_WR                               2u   /* Writing to file.                                     */

/*
*********************************************************************************************************
*                                    FILE POSITION ORIGIN DEFINES
*
* Note(s) : (1) These three defines are set to corresponding with their POSIX API equivalent values:
*
*                   FS_FILE_ORIGIN_START  = SEEK_SET(0)
*                   FS_FILE_ORIGIN_CUR    = SEEK_CUR(1)
*                   FS_FILE_ORIGIN_END    = SEEK_END(2)
*
*********************************************************************************************************
*/

#define  FS_FILE_ORIGIN_START                              0u   /* Origin is beginning of file.                         */
#define  FS_FILE_ORIGIN_CUR                                1u   /* Origin is current file position.                     */
#define  FS_FILE_ORIGIN_END                                2u   /* Origin is end of file.                               */

/*
*********************************************************************************************************
*                                      FILE ACCESS MODE DEFINES
*********************************************************************************************************
*/

#define  FS_FILE_ACCESS_MODE_NONE                DEF_BIT_NONE
#define  FS_FILE_ACCESS_MODE_RD                  DEF_BIT_00     /* Read from file.                                      */
#define  FS_FILE_ACCESS_MODE_WR                  DEF_BIT_01     /* Write to  file.                                      */
#define  FS_FILE_ACCESS_MODE_CREATE              DEF_BIT_02     /* Create    file if it does not exist.                 */
#define  FS_FILE_ACCESS_MODE_TRUNCATE            DEF_BIT_03     /* Truncate  file if it does     exist.                 */
#define  FS_FILE_ACCESS_MODE_APPEND              DEF_BIT_04     /* Append to file.                                      */
#define  FS_FILE_ACCESS_MODE_EXCL                DEF_BIT_05     /* File must be created.                                */
#define  FS_FILE_ACCESS_MODE_CACHED              DEF_BIT_06     /* Defer file metadata updates until close operation.   */
#define  FS_FILE_ACCESS_MODE_RDWR               (FS_FILE_ACCESS_MODE_RD | FS_FILE_ACCESS_MODE_WR)

/*
*********************************************************************************************************
*                                      FILE BUFFER MODE DEFINES
*********************************************************************************************************
*/

#define  FS_FILE_BUF_MODE_NONE                   DEF_BIT_NONE
#define  FS_FILE_BUF_MODE_RD                     DEF_BIT_00     /* Data buffered ONLY for reads.                        */
#define  FS_FILE_BUF_MODE_WR                     DEF_BIT_01     /* Data buffered ONLY for writes.                       */
                                                                /* Data buffered for BOTH reads & writes.               */
#define  FS_FILE_BUF_MODE_RD_WR                 (DEF_BIT_00 | DEF_BIT_01)
#define  FS_FILE_BUF_MODE_SEC_ALIGNED            DEF_BIT_02     /* Force buffer align on sec boundaries.                */

/*
*********************************************************************************************************
*                                     FILE BUFFER STATUS DEFINES
*********************************************************************************************************
*/

#define  FS_FILE_BUF_STATUS_NONE                          1u    /* No buffer has been assigned.                         */
#define  FS_FILE_BUF_STATUS_NEVER                         2u    /* Buffer CANNOT be   assigned.                         */
#define  FS_FILE_BUF_STATUS_EMPTY                         3u    /* Buffer has been    assigned, but is  empty.          */
#define  FS_FILE_BUF_STATUS_NONEMPTY_RD                   4u    /* Buffer has been    assigned & contains rd data.      */
#define  FS_FILE_BUF_STATUS_NONEMPTY_WR                   5u    /* Buffer has been    assigned & contains data to wr.   */


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           FILE DATA TYPE
*
* Note(s) : (1) 'DataPtr' can be assigned pointer to file system driver-specific datum/struct/buffer.
*********************************************************************************************************
*/

struct  fs_file {
    FS_ID           ID;                                         /* File ID cfg'd @ init.                                */
    FS_STATE        State;                                      /* File state.                                          */
    FS_CTR          RefCnt;                                     /* Ref cnts.                                            */
    FS_CTR          RefreshCnt;                                 /* Refresh cnts.                                        */

    FS_FLAGS        AccessMode;                                 /* Access mode.                                         */
    CPU_BOOLEAN     FlagErr;                                    /* Err flag.                                            */
    CPU_BOOLEAN     FlagEOF;                                    /* EOF flag.                                            */
    FS_STATE        IO_State;                                   /* I/O state.                                           */
    FS_FILE_SIZE    Size;                                       /* File size.                                           */
    FS_FILE_SIZE    Pos;                                        /* File pos.                                            */

#if (FS_CFG_FILE_BUF_EN == DEF_ENABLED)
    FS_FILE_SIZE    BufStart;                                   /* Start pos of buf in file.                            */
    CPU_SIZE_T      BufMaxPos;                                  /* Max buf pos.                                         */
    CPU_SIZE_T      BufSize;                                    /* Buf size.                                            */
    FS_FLAGS        BufMode;                                    /* Buf mode.                                            */
    FS_STATE        BufStatus;                                  /* Buf status.                                          */
    void           *BufPtr;                                     /* Ptr to buf.                                          */
    FS_SEC_SIZE     BufSecSize;                                 /* Buf sec size.                                        */
#endif

    FS_VOL         *VolPtr;                                     /* Ptr to mounted vol containing the file.              */
    void           *DataPtr;                                    /* Ptr to data specific for a file system driver.       */

#if (FS_CFG_CTR_STAT_EN == DEF_ENABLED)
    FS_CTR          StatRdCtr;                                  /* Nbr rds.                                             */
    FS_CTR          StatWrCtr;                                  /* Nbr wrs.                                             */
#endif
};


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#if (FS_CFG_FILE_BUF_EN == DEF_ENABLED)
void           FSFile_BufAssign    (FS_FILE         *p_file,    /* Assign buffer to a file.                             */
                                    void            *p_buf,
                                    FS_FLAGS         mode,
                                    CPU_SIZE_T       size,
                                    FS_ERR          *p_err);

void           FSFile_BufFlush     (FS_FILE         *p_file,    /* Flush buffer contents to file.                       */
                                    FS_ERR          *p_err);
#endif

void           FSFile_Close        (FS_FILE         *p_file,    /* Close a file.                                        */
                                    FS_ERR          *p_err);

void           FSFile_ClrErr       (FS_FILE         *p_file,    /* Clear EOF & error indicators on a file.              */
                                    FS_ERR          *p_err);

CPU_BOOLEAN    FSFile_IsErr        (FS_FILE         *p_file,    /* Test error indicator on a file.                      */
                                    FS_ERR          *p_err);

CPU_BOOLEAN    FSFile_IsEOF        (FS_FILE         *p_file,    /* Test EOF indicator on a file.                        */
                                    FS_ERR          *p_err);

CPU_BOOLEAN    FSFile_IsOpen       (CPU_CHAR        *name_full, /* Test if file is open                                 */
                                    FS_FLAGS        *p_mode,
                                    FS_ERR          *p_err);

#if (FS_CFG_FILE_LOCK_EN == DEF_ENABLED)
void           FSFile_LockAccept   (FS_FILE         *p_file,    /* Acquire task ownership of a file, if available.      */
                                    FS_ERR          *p_err);

void           FSFile_LockGet      (FS_FILE         *p_file,    /* Acquire task ownership of a file.                    */
                                    FS_ERR          *p_err);

void           FSFile_LockSet      (FS_FILE         *p_file,    /* Release task ownership of a file.                    */
                                    FS_ERR          *p_err);
#endif

FS_FILE       *FSFile_Open         (CPU_CHAR        *name_full, /* Open a file.                                         */
                                    FS_FLAGS         mode,
                                    FS_ERR          *p_err);

FS_FILE_SIZE   FSFile_PosGet       (FS_FILE         *p_file,    /* Get file position indicator.                         */
                                    FS_ERR          *p_err);

void           FSFile_PosSet       (FS_FILE         *p_file,    /* Set file position indicator.                         */
                                    FS_FILE_OFFSET   offset,
                                    FS_STATE         origin,
                                    FS_ERR          *p_err);

void           FSFile_Query        (FS_FILE         *p_file,    /* Get information about a file.                        */
                                    FS_ENTRY_INFO   *p_info,
                                    FS_ERR          *p_err);

CPU_SIZE_T     FSFile_Rd           (FS_FILE         *p_file,    /* Read from a file.                                    */
                                    void            *p_dest,
                                    CPU_SIZE_T       size,
                                    FS_ERR          *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void           FSFile_Truncate     (FS_FILE         *p_file,    /* Truncate a file.                                     */
                                    FS_FILE_SIZE     size,
                                    FS_ERR          *p_err);
#endif

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
CPU_SIZE_T     FSFile_Wr           (FS_FILE         *p_file,    /* Write to a file.                                     */
                                    void            *p_src,
                                    CPU_SIZE_T       size,
                                    FS_ERR          *p_err);
#endif

/*
*********************************************************************************************************
*                                   MANAGEMENT FUNCTION PROTOTYPES
*********************************************************************************************************
*/

FS_QTY         FSFile_GetFileCnt   (void);                      /* Get number of open files.                            */

FS_QTY         FSFile_GetFileCntMax(void);                      /* Get maximum possible number of open files.           */

/*
*********************************************************************************************************
*                                    INTERNAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void           FSFile_ModuleInit   (FS_QTY           file_cnt,  /* Initialize file module.                              */
                                    FS_ERR          *p_err);

FS_FLAGS       FSFile_ModeParse    (CPU_CHAR        *str_mode,  /* Parse mode string.                                   */
                                    CPU_SIZE_T       str_len);

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                      DEFINED IN OS'S  fs_os.c
*********************************************`***********************************************************
*/

#if (FS_CFG_FILE_LOCK_EN == DEF_ENABLED)
void           FS_OS_FileInit      (FS_QTY           file_cnt,  /* Create file system device objects.                   */
                                    FS_ERR          *p_err);

void           FS_OS_FileAccept    (FS_ID            file_id,   /* Accept  access to file system file.                  */
                                    FS_ERR          *p_err);

void           FS_OS_FileLock      (FS_ID            file_id,   /* Acquire access to file system file.                  */
                                    FS_ERR          *p_err);

CPU_BOOLEAN    FS_OS_FileUnlock    (FS_ID            file_id);  /* Release access to file system file.                  */
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

