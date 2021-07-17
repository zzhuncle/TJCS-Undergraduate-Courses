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
*                                     FILE SYSTEM SUITE API LAYER
*
*                                         POSIX API FUNCTIONS
*
* Filename  : fs_api.h
* Version   : V4.07.07
*********************************************************************************************************
* Notice(s) : (1) The Institute of Electrical and Electronics Engineers and The Open Group, have given
*                 us permission to reprint portions of their documentation.  Portions of this text are
*                 reprinted and reproduced in electronic form from the IEEE Std 1003.1, 2004 Edition,
*                 Standard for Information Technology -- Portable Operating System Interface (POSIX),
*                 The Open Group Base Specifications Issue 6, Copyright (C) 2001-2004 by the Institute
*                 of Electrical and Electronics Engineers, Inc and The Open Group.  In the event of any
*                 discrepancy between these versions and the original IEEE and The Open Group Standard,
*                 the original IEEE and The Open Group Standard is the referee document.  The original
*                 Standard can be obtained online at http://www.opengroup.org/unix/online.html.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE GUARDS
*********************************************************************************************************
*/

#ifndef  FS_API_H
#define  FS_API_H


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "fs_cfg_fs.h"
#include  "fs_dir.h"
#include  "fs_file.h"
#include  "fs_type.h"


/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) The following FAT-module-present configuration value MUST be pre-#define'd in
*               'fs_cfg_fs.h' PRIOR to all other file system modules that require API Layer Configuration
*               (see 'fs_cfg_fs.h  FAT LAYER CONFIGURATION  Note #2b') :
*
*                   FS_API_MODULE_PRESENT
*********************************************************************************************************
*/

#ifdef   FS_API_MODULE_PRESENT                                  /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                           LINT INHIBITION
*
* Note(s) : (1) The functions prototyped in this file conform to the POSIX standard, with the same
*               parameter types & return values.  Certain MISRA guidelines are thereby violated.
*
*                   "Note 970: Use of modifier or type 'int' outside of a typedef [MISRA 2004 Rule 6.3]"
*                   "Note 970: Use of modifier or type 'char' outside of a typedef [MISRA 2004 Rule 6.3]"
*********************************************************************************************************
*/

/*lint -e970*/


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_API_MODULE
#define  FS_API_EXT
#else
#define  FS_API_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  FS_BUFSIZ                                      4096u

#define  FS_EOF                                          (-1)

#define  FS__IOFBF                          FS_FILE_BUF_MODE_RD_WR
#define  FS__IONBR                          FS_FILE_BUF_MODE_NONE

#define  FS_FOPEN_MAX                       (FSFile_GetFileCntMax())

#define  FS_FILENAME_MAX                    FS_CFG_MAX_FULL_NAME_LEN

#define  FS_NULL                                  ((void *)0)

#define  FS_STDIO_ERR                                   (-1)


/*
*********************************************************************************************************
*                                            POSIX DEFINES
*
* Note(s) : (1) Our stack implements these to allow customers to use POSIX API's with our File System stack.
*
*           (2) If stdio.h is already included then SEEK_SET, SEEK_CUR, SEEK_END will already be defined. No need
*               to re-define. If stdio.h is not included then this will create a version for use that still
*               complies with the POSIX API and uC/FS stack.
*********************************************************************************************************
*/

#ifndef  SEEK_SET
#define  SEEK_SET                        FS_FILE_ORIGIN_START
#endif
#ifndef  SEEK_CUR
#define  SEEK_CUR                        FS_FILE_ORIGIN_CUR
#endif
#ifndef  SEEK_END
#define  SEEK_END                        FS_FILE_ORIGIN_END
#endif

/*
*********************************************************************************************************
*                                            ERROR NUMBERS
*********************************************************************************************************
*/

#define  FS_EACCES                                         1u   /* "Permission denied."                                 */
#define  FS_EAGAIN                                         2u   /* "Resource temporarily unavailable."                  */
#define  FS_EBADF                                          3u   /* "Bad file descriptor."                               */
#define  FS_EBUSY                                          4u   /* "Resource busy."                                     */
#define  FS_EEXIST                                         5u   /* "File exists."                                       */
#define  FS_EFBIG                                          6u   /* "File too large."                                    */
#define  FS_EINVAL                                         7u   /* "Invalid argument."                                  */
#define  FS_EIO                                            8u   /* "Input/output error."                                */
#define  FS_EISDIR                                         9u   /* "Is a directory."                                    */
#define  FS_EMFILE                                        10u   /* "Too many files open in system."                     */
#define  FS_ENAMETOOLONG                                  11u   /* "Filename too long."                                 */
#define  FS_EBFILE                                        12u   /* "Too many open files."                               */
#define  FS_NOENT                                         13u   /* "No such file or directory."                         */
#define  FS_ENOMEM                                        14u   /* "Not enough space."                                  */
#define  FS_ENOSPC                                        15u   /* "No space left on device."                           */
#define  FS_ENOTDIR                                       16u   /* "Not a directory."                                   */
#define  FS_ENOTEMPTY                                     17u   /* "Directory not empty."                               */
#define  FS_EOVERFLOW                                     18u   /* "Value too large to be stored in date type."         */
#define  FS_ERANGE                                        19u   /* "Result too large or too small."                     */
#define  FS_EROFS                                         20u   /* "Read-only file system."                             */
#define  FS_EXDEV                                         21u   /* "Improper link."                                     */

/*
*********************************************************************************************************
*                                             MODE VALUES
*********************************************************************************************************
*/

#define  FS_S_IFMT                                    0x7000u
#define  FS_S_IFBLK                                   0x1000u
#define  FS_S_IFCHR                                   0x2000u
#define  FS_S_IFIFO                                   0x3000u
#define  FS_S_IFREG                                   0x4000u
#define  FS_S_IFDIR                                   0x5000u
#define  FS_S_IFLNK                                   0x6000u
#define  FS_S_IFSOCK                                  0x7000u

#define  FS_S_IRUSR                                   0x0400u
#define  FS_S_IWUSR                                   0x0200u
#define  FS_S_IXUSR                                   0x0100u
#define  FS_S_IRWXU                     (FS_S_IRUSR | FS_S_IWUSR | FS_S_IXUSR)

#define  FS_S_IRGRP                                   0x0040u
#define  FS_S_IWGRP                                   0x0020u
#define  FS_S_IXGRP                                   0x0010u
#define  FS_S_IRWXG                     (FS_S_IRGRP | FS_S_IWGRP | FS_S_IXGRP)

#define  FS_S_IROTH                                   0x0004u
#define  FS_S_IWOTH                                   0x0002u
#define  FS_S_IXOTH                                   0x0001u
#define  FS_S_IRWXO                     (FS_S_IROTH | FS_S_IWOTH | FS_S_IXOTH)


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/

#define  FS_S_ISREG(m)               ((((m) & FS_S_IFMT) == FS_S_IFREG) ? 1 : 0)
#define  FS_S_ISDIR(m)               ((((m) & FS_S_IFMT) == FS_S_IFDIR) ? 1 : 0)


/*
*********************************************************************************************************
*                                             DATA TYPES
*
* Note(s) : (1) IEEE Std 1003.1, 2004 Edition, Section 'stdio.h() : DESCRIPTION' states that fpos_t
*               should be A non-array type containing all information needed to specify uniquely every
*               position within a file."
*
*           (2) IEEE Std 1003.1, 2004 Edition, Section 'sys/types.h() : DESCRIPTION' states that
*
*               (a) off_t is "Used for file sizes" and, additionally, that it "shall be [a] signed
*                   integer [type]".
*
*               (a) size_t is "Used for sizes of objects" and, additionally, that it "shall be an
*                   unsigned integer type".
*********************************************************************************************************
*/

typedef  CPU_INT32U  fs_fpos_t;

typedef  CPU_INT32U  fs_off_t;

typedef  CPU_SIZE_T  fs_size_t;

typedef  CPU_INT32U  fs_dev_t;

typedef  CPU_INT32U  fs_ino_t;

typedef  CPU_INT32U  fs_mode_t;

typedef  CPU_INT32U  fs_nlink_t;

typedef  CPU_INT32U  fs_uid_t;

typedef  CPU_INT32U  fs_gid_t;

typedef  CPU_INT32U  fs_time_t;

typedef  CPU_INT32U  fs_blksize_t;

typedef  CPU_INT32U  fs_blkcnt_t;

/*
*********************************************************************************************************
*                                        FILE STATS DATA TYPE
*********************************************************************************************************
*/

struct  fs_stat {
    fs_dev_t      st_dev;                                       /* Device ID of device containing file.                 */
    fs_ino_t      st_ino;                                       /* File serial number.                                  */
    fs_mode_t     st_mode;                                      /* Mode of file.                                        */
    fs_nlink_t    st_nlink;                                     /* Number of hard links to the file.                    */
    fs_uid_t      st_uid;                                       /* User ID of file.                                     */
    fs_gid_t      st_gid;                                       /* Group ID of file.                                    */
    fs_off_t      st_size;                                      /* File size in bytes.                                  */
    fs_time_t     st_atime;                                     /* Time of last access.                                 */
    fs_time_t     st_mtime;                                     /* Time of last data modification.                      */
    fs_time_t     st_ctime;                                     /* Time of last status change.                          */
    fs_blksize_t  st_blksize;                                   /* Preferred I/O block size for file.                   */
    fs_blkcnt_t   st_blocks;                                    /* Number of blocks allocated for file.                 */
};

/*
*********************************************************************************************************
*                                           TIME DATA TYPE
*********************************************************************************************************
*/

struct  fs_tm {
    int           tm_sec;                                       /* Seconds [0,60].                                      */
    int           tm_min;                                       /* Minutes [0,59].                                      */
    int           tm_hour;                                      /* Hour [0,23].                                         */
    int           tm_mday;                                      /* Day of month [1,31].                                 */
    int           tm_mon;                                       /* Month of year [1,12].                                */
    int           tm_year;                                      /* Years since 1900.                                    */
    int           tm_wday;                                      /* Day of week [1,7] (Sunday = 1).                      */
    int           tm_yday;                                      /* Day of year [1,366].                                 */
    int           tm_isdst;                                     /* Daylight Savings flag.                               */
};


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*
* Note(s) : (1) (a) Equivalents of all standard POSIX 'stdio.h' functions are provided EXCEPT :
*
*                   (1) The following file operation functions :
*                       (A) tmpfile()
*                       (B) tmpnam()
*                   (2) The following file access functions :
*                       (A) freopen()
*                   (3) ALL formatted input/output functions.
*                   (4) ALL character input/output functions.
*                   (5) The following error-handling functions:
*                       (A) perror()
*
*               (b) The original function name, besides the prefixed 'fs_', is preserved.
*
*               (c) The types of arguments & return values are transformed for the file system suite
*                   environment :
*
*                   (a) 'FILE *' --> 'FS_FILE *'.
*                   (b) 'fpos_t' --> 'fs_fpos_t'.
*                   (c) 'size_t' --> 'fs_size_t'.
*
*           (2) (a) Equivalents of several POSIX 'dirent.h' functions are provided :
*
*                   (1) closedir()
*                   (2) opendir()
*                   (3) readdir()
*
*               (b) The original function name, besides the prefixed 'fs_', is preserved.
*
*               (c) The types of arguments & return values were transformed for the file system suite
*                   environment :
*
*                   (1) 'DIR *'           --> 'FS_DIR *'.
*                   (2) 'struct dirent *' --> 'struct fs_dirent *'
*
*           (3) (a) Equivalents of several POSIX 'sys/stat.h' functions are provided :
*
*                   (1) fstat()
*                   (2) mkdir()
*                   (3) stat()
*
*               (b) The original function name, besides the prefixed 'fs_', is preserved.
*
*           (4) (a) Equivalents of several POSIX 'time.h' function are provided :
*
*                   (1) asctime_r()
*                   (2) ctime_r()
*                   (3) localtime_r()
*                   (4) mktime()
*
*               (b) The original function name, besides the prefixed 'fs_', is preserved.
*
*           (5) (a) Equivalents of several POSIX 'unistd.h' functions are provided :
*
*                   (1) chdir()
*                   (2) ftruncate()
*                   (3) getcwd()
*                   (4) rmdir()
*
*               (b) The original function name, besides the prefixed 'fs_', is preserved.
*
*               (c) The types of arguments & return values were transformed for the file system suite
*                   environment :
*
*                   (1) 'int'     --> 'FS_FILE *'. (file descriptor)
*                   (2) 'off_t'   --> 'fs_off_t'.
*                   (3) 'size_t'  --> 'fs_size_t'.
*********************************************************************************************************
*/

                                                                            /* ------ WORKING DIRECTORY FUNCTIONS ----- */
#if (FS_CFG_WORKING_DIR_EN == DEF_ENABLED)
int             fs_chdir       (const  char                *path_dir);      /* Set the working dir for current task.    */

char           *fs_getcwd      (       char                *path_dir,       /* Get the working dir for current task.    */
                                       fs_size_t            size);
#endif


                                                                            /* ---------- DIRECTORY FUNCTIONS --------- */
#ifdef   FS_DIR_MODULE_PRESENT
int             fs_closedir    (       FS_DIR              *p_dir);         /* Close & free a directory.                */

FS_DIR         *fs_opendir     (const  char                *name_full);     /* Open a directory.                        */

int             fs_readdir_r   (       FS_DIR              *p_dir,          /* Read a directory entry from a directory. */
                                       struct  fs_dirent   *p_dir_entry,
                                       struct  fs_dirent  **pp_result);
#endif


                                                                            /* ------------ ENTRY FUNCTIONS ----------- */
#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
int             fs_mkdir       (const  char                *name_full);     /* Create a directory.                      */

int             fs_remove      (const  char                *name_full);     /* Delete a file or directory.              */

int             fs_rename      (const  char                *name_full_old,  /* Rename a file or directory.              */
                                const  char                *name_full_new);

int             fs_rmdir       (const  char                *name_full);     /* Delete a directory.                      */
#endif

int             fs_stat        (const  char                *name_full,      /* Get information about a file or dir.     */
                                       struct  fs_stat     *p_info);


                                                                            /* ------------ FILE FUNCTIONS ------------ */
void            fs_clearerr    (       FS_FILE             *p_file);        /* Clear EOF & error indicators on a file.  */

int             fs_fclose      (       FS_FILE             *p_file);        /* Close & free a file.                     */

int             fs_feof        (       FS_FILE             *p_file);        /* Test EOF indicator on a file.            */

int             fs_ferror      (       FS_FILE             *p_file);        /* Test error indicator on a file.          */

#if (FS_CFG_FILE_BUF_EN == DEF_ENABLED)
int             fs_fflush      (       FS_FILE             *p_file);        /* Flush buffer contents to file.           */
#endif

int             fs_fgetpos     (       FS_FILE             *p_file,         /* Get file position indicator.             */
                                       fs_fpos_t           *p_pos);

#if (FS_CFG_FILE_LOCK_EN == DEF_ENABLED)
void            fs_flockfile   (       FS_FILE             *p_file);        /* Acquire task ownership of a file.        */
#endif

FS_FILE        *fs_fopen       (const  char                *name_full,      /* Open a file.                             */
                                const  char                *str_mode);

fs_size_t       fs_fread       (       void                *p_dest,         /* Read from a file.                        */
                                       fs_size_t            size,
                                       fs_size_t            nitems,
                                       FS_FILE             *p_file);

int             fs_fseek       (       FS_FILE             *p_file,         /* Set file position indicator.             */
                                       long  int            offset,
                                       int                  origin);

int             fs_fsetpos     (       FS_FILE             *p_file,         /* Set file position indicator.             */
                                const  fs_fpos_t           *p_pos);

int             fs_fstat       (       FS_FILE             *p_file,         /* Get information about a file.            */
                                       struct  fs_stat     *p_info);


long  int       fs_ftell       (       FS_FILE             *p_file);        /* Get file position indicator.             */

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
int             fs_ftruncate   (       FS_FILE             *p_file,         /* Truncate a file.                         */
                                       fs_off_t             size);
#endif

#if (FS_CFG_FILE_LOCK_EN == DEF_ENABLED)
int             fs_ftrylockfile(       FS_FILE             *p_file);        /* Acquire task ownership of a file, if avail*/

void            fs_funlockfile (       FS_FILE             *p_file);        /* Release task ownership of a file.        */
#endif

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
fs_size_t       fs_fwrite      (const  void                *p_src,          /* Write to a file.                         */
                                       fs_size_t            size,
                                       fs_size_t            nitems,
                                       FS_FILE             *p_file);
#endif

void            fs_rewind      (       FS_FILE             *p_file);        /* Reset file position indicator of a file. */

#if (FS_CFG_FILE_BUF_EN == DEF_ENABLED)
void            fs_setbuf      (       FS_FILE             *p_file,         /* Assign buffer to a file.                 */
                                       char                *p_buf);

int             fs_setvbuf     (       FS_FILE             *p_file,         /* Assign buffer to a file.                 */
                                       char                *p_buf,
                                       int                  mode,
                                       fs_size_t            size);
#endif


                                                                            /* ------------ TIME FUNCTIONS ------------ */
char           *fs_asctime_r   (const  struct  fs_tm       *p_time,         /* Convert date/time to string.             */
                                       char                *str_time);

char           *fs_ctime_r     (const  fs_time_t           *p_ts,           /* Convert timestamp to string.             */
                                       char                *str_time);

struct  fs_tm  *fs_localtime_r (const  fs_time_t           *p_ts,           /* Convert timestamp to date/time.          */
                                       struct  fs_tm       *p_time);

fs_time_t       fs_mktime      (       struct  fs_tm       *p_time);        /* Convert date/time to timestamp.          */


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*
* Note(s) : (1) See 'MODULE  Note #1'.
*********************************************************************************************************
*/

#endif                                                          /* End of API module include (see Note #1).             */
#endif
