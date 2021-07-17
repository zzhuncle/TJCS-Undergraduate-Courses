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
*                                 FILE SYSTEM SUITE VOLUME MANAGEMENT
*
* Filename : fs_vol.h
* Version  : V4.07.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_VOL_H
#define  FS_VOL_H


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_VOL_MODULE
#define  FS_VOL_EXT
#else
#define  FS_VOL_EXT  extern
#endif


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  "fs_cfg_fs.h"
#include  "fs_err.h"
#include  "fs_ctr.h"
#include  "fs_type.h"


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        VOLUME STATE DEFINES
*********************************************************************************************************
*/

#define  FS_VOL_STATE_CLOSED                               0u   /* Volume closed.                                       */
#define  FS_VOL_STATE_CLOSING                              1u   /* Volume closing.                                      */
#define  FS_VOL_STATE_OPENING                              2u   /* Volume opening.                                      */
#define  FS_VOL_STATE_OPEN                                 3u   /* Volume open.                                         */
#define  FS_VOL_STATE_PRESENT                              4u   /* Volume device present.                               */
#define  FS_VOL_STATE_MOUNTED                              5u   /* Volume mounted.                                      */

/*
*********************************************************************************************************
*                                     VOLUME ACCESS MODE DEFINES
*********************************************************************************************************
*/

#define  FS_VOL_ACCESS_MODE_NONE                 DEF_BIT_NONE
#define  FS_VOL_ACCESS_MODE_RD                   DEF_BIT_00     /* Readable.                                            */
#define  FS_VOL_ACCESS_MODE_WR                   DEF_BIT_01     /* Writeable.                                           */
#define  FS_VOL_ACCESS_MODE_RDWR                (FS_FILE_ACCESS_MODE_RD | FS_FILE_ACCESS_MODE_WR)

/*
*********************************************************************************************************
*                                     VOLUME SECTOR TYPE DEFINES
*********************************************************************************************************
*/

#define  FS_VOL_SEC_TYPE_UNKNOWN                 DEF_BIT_NONE
#define  FS_VOL_SEC_TYPE_MGMT                    DEF_BIT_00
#define  FS_VOL_SEC_TYPE_DIR                     DEF_BIT_01
#define  FS_VOL_SEC_TYPE_FILE                    DEF_BIT_02

/*
*********************************************************************************************************
*                                      VOLUME CACHE MODE DEFINES
*********************************************************************************************************
*/

#define  FS_VOL_CACHE_MODE_NONE                            0u
#define  FS_VOL_CACHE_MODE_RD                              1u
#define  FS_VOL_CACHE_MODE_WR_THROUGH                      2u
#define  FS_VOL_CACHE_MODE_WR_BACK                         3u


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
*                                          VOLUME DATA TYPE
*********************************************************************************************************
*/

struct  fs_vol {
    FS_STATE           State;                                   /* State.                                               */
    FS_CTR             RefCnt;                                  /* Ref cnts.                                            */
    FS_CTR             RefreshCnt;                              /* Refresh cnts.                                        */

    FS_FLAGS           AccessMode;                              /* Access mode.                                         */
    CPU_CHAR           Name[FS_CFG_MAX_VOL_NAME_LEN + 1u];      /* Vol name.                                            */
    FS_PARTITION_NBR   PartitionNbr;                            /* Partition nbr.                                       */
    FS_SEC_NBR         PartitionStart;                          /* Partition start sec.                                 */
    FS_SEC_QTY         PartitionSize;                           /* Partition size (in sec's).                           */
    FS_SEC_SIZE        SecSize;                                 /* Size of a sec, in octets.                            */

    FS_QTY             FileCnt;                                 /* Nbr of open files on this vol.                       */
#ifdef FS_DIR_MODULE_PRESENT
    FS_QTY             DirCnt;                                  /* Nbr of open dirs  on this vol.                       */
#endif
    FS_DEV            *DevPtr;                                  /* Ptr to dev for this vol.                             */
    void              *DataPtr;                                 /* Ptr to data specific for a file system driver.       */
#ifdef FS_CACHE_MODULE_PRESENT
    FS_VOL_CACHE_API  *CacheAPI_Ptr;                            /* Ptr to cache API for this vol.                       */
    void              *CacheDataPtr;                            /* Ptr to data specific for a cache.                    */
#endif

#if (FS_CFG_CTR_STAT_EN == DEF_ENABLED)
    FS_CTR             StatRdSecCtr;                            /* Nbr rd secs.                                         */
    FS_CTR             StatWrSecCtr;                            /* Nbr wr secs.                                         */
#endif
};


/*
*********************************************************************************************************
*                                        VOLUME INFO DATA TYPE
*********************************************************************************************************
*/

typedef  struct  fs_vol_info {
    FS_STATE           State;                                   /* Volume state.                                        */
    FS_STATE           DevState;                                /* Device state.                                        */
    FS_SEC_QTY         DevSize;                                 /* Number of sectors on dev.                            */
    FS_SEC_SIZE        DevSecSize;                              /* Sector size of dev.                                  */
    FS_SEC_QTY         PartitionSize;                           /* Number of sectors in partition.                      */
    FS_SEC_QTY         VolBadSecCnt;                            /* Number of bad   data sectors on vol.                 */
    FS_SEC_QTY         VolFreeSecCnt;                           /* Number of free  data sectors on vol.                 */
    FS_SEC_QTY         VolUsedSecCnt;                           /* Number of used  data sectors on vol.                 */
    FS_SEC_QTY         VolTotSecCnt;                            /* Number of total data sectors on vol.                 */
} FS_VOL_INFO;


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

#ifdef FS_CACHE_MODULE_PRESENT
void          FSVol_CacheAssign    (CPU_CHAR          *name_vol,    /* Assign cache to a volume.                        */
                                    FS_VOL_CACHE_API  *p_cache_api,
                                    void              *p_cache_data,
                                    CPU_INT32U         size,
                                    CPU_INT08U         pct_mgmt,
                                    CPU_INT08U         pct_dir,
                                    FS_FLAGS           mode,
                                    FS_ERR            *p_err);

void          FSVol_CacheInvalidate(CPU_CHAR          *name_vol,    /* Invalidate cache on a volume.                         */
                                    FS_ERR            *p_err);

void          FSVol_CacheFlush     (CPU_CHAR          *name_vol,    /* Flush cache on a volume.                         */
                                    FS_ERR            *p_err);
#endif

void          FSVol_Close          (CPU_CHAR          *name_vol,    /* Close (unmount) a volume.                        */
                                    FS_ERR            *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void          FSVol_Fmt            (CPU_CHAR          *name_vol,    /* Format a volume.                                 */
                                    void              *p_fs_cfg,
                                    FS_ERR            *p_err);
#endif

CPU_BOOLEAN   FSVol_IsMounted      (CPU_CHAR          *name_vol);   /* Determine whether a volume is mounted.           */

void          FSVol_LabelGet       (CPU_CHAR          *name_vol,    /* Get volume label.                                */
                                    CPU_CHAR          *label,
                                    CPU_SIZE_T         len_max,
                                    FS_ERR            *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void          FSVol_LabelSet       (CPU_CHAR          *name_vol,    /* Set volume label.                                */
                                    CPU_CHAR          *label,
                                    FS_ERR            *p_err);
#endif

void          FSVol_Open           (CPU_CHAR          *name_vol,    /* Open (mount) a volume.                           */
                                    CPU_CHAR          *name_dev,
                                    FS_PARTITION_NBR   partition_nbr,
                                    FS_ERR            *p_err);

void          FSVol_Query          (CPU_CHAR          *name_vol,    /* Obtain information about a volume.               */
                                    FS_VOL_INFO       *p_info,
                                    FS_ERR            *p_err);

void          FSVol_Rd             (CPU_CHAR          *name_vol,    /* Read data from volume sector(s).                 */
                                    void              *p_dest,
                                    FS_SEC_NBR         start,
                                    FS_SEC_QTY         cnt,
                                    FS_ERR            *p_err);

CPU_BOOLEAN   FSVol_Refresh        (CPU_CHAR          *name_vol,    /* Refresh volume.                                  */
                                    FS_ERR            *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void          FSVol_Wr             (CPU_CHAR          *name_vol,    /* Write data to volume sector(s).                  */
                                    void              *p_src,
                                    FS_SEC_NBR         start,
                                    FS_SEC_QTY         cnt,
                                    FS_ERR            *p_err);
#endif

/*
*********************************************************************************************************
*                                   MANAGEMENT FUNCTION PROTOTYPES
*********************************************************************************************************
*/

FS_QTY        FSVol_GetVolCnt      (void);                          /* Get number of open volumes.                      */

FS_QTY        FSVol_GetVolCntMax   (void);                          /* Get maximum possible number of open volumes.     */

void          FSVol_GetVolName     (FS_QTY             vol_nbr,     /* Get name of nth open volume.                     */
                                    CPU_CHAR          *name_vol,
                                    FS_ERR            *p_err);

void          FSVol_GetDfltVolName (CPU_CHAR          *name_vol,    /* Get name of default volume.                      */
                                    FS_ERR            *p_err);

CPU_BOOLEAN   FSVol_IsDflt         (CPU_CHAR          *name_vol,    /* Determine whether volume is default volume.      */
                                    FS_ERR            *p_err);

/*
*********************************************************************************************************
*                                    INTERNAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

                                                                    /* ---------------- INITIALIZATION ---------------- */
void          FSVol_ModuleInit     (FS_QTY             vol_cnt,     /* Initialize volume module.                        */
                                    FS_ERR            *p_err);


                                                                    /* ---------------- ACCESS CONTROL ---------------- */
FS_VOL       *FSVol_AcquireLockChk (CPU_CHAR          *name_vol,    /* Acquire volume reference & lock.                 */
                                    CPU_BOOLEAN        mounted,
                                    FS_ERR            *p_err);

FS_VOL       *FSVol_Acquire        (CPU_CHAR          *name_vol,    /* Acquire volume reference.                        */
                                    FS_ERR            *p_err);

FS_VOL       *FSVol_AcquireDflt    (void);                          /* Acquire default volume reference.                */

void          FSVol_Release        (FS_VOL            *p_vol);      /* Release volume reference.                        */

void          FSVol_ReleaseUnlock  (FS_VOL            *p_vol);      /* Release volume reference & lock.                 */

CPU_BOOLEAN   FSVol_Lock           (FS_VOL            *p_vol);      /* Acquire volume lock.                             */

void          FSVol_Unlock         (FS_VOL            *p_vol);      /* Release volume lock.                             */


                                                                    /* ----------------- REGISTRATION ----------------- */
#ifdef FS_DIR_MODULE_PRESENT
void          FSVol_DirAdd         (FS_VOL            *p_vol,        /* Add directory to open directory list.            */
                                   FS_DIR            *p_dir);

void          FSVol_DirRemove      (FS_VOL            *p_vol,        /* Remove directory from open directory list.       */
                                    FS_DIR            *p_dir);
#endif

void          FSVol_FileAdd        (FS_VOL            *p_vol,        /* Add file to open file list.                      */
                                    FS_FILE           *p_file);

void          FSVol_FileRemove     (FS_VOL            *p_vol,        /* Remove file from open file list.                 */
                                    FS_FILE           *p_file);


                                                                    /* ----------------- LOCKED ACCESS ---------------- */
void          FSVol_RdLocked       (FS_VOL            *p_vol,       /* Read data from volume sector(s).                 */
                                    void              *p_dest,
                                    FS_SEC_NBR         start,
                                    FS_SEC_QTY         cnt,
                                    FS_ERR            *p_err);

void          FSVol_RdLockedEx     (FS_VOL            *p_vol,       /* Read data from volume sector(s).                 */
                                    void              *p_dest,
                                    FS_SEC_NBR         start,
                                    FS_SEC_QTY         cnt,
                                    FS_FLAGS           sec_type,
                                    FS_ERR            *p_err);

CPU_BOOLEAN   FSVol_RefreshLocked  (FS_VOL            *p_vol,       /* Refresh volume.                                  */
                                    FS_ERR            *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void          FSVol_ReleaseLocked  (FS_VOL            *p_vol,       /* Release volume sector(s).                        */
                                    FS_SEC_NBR         start,
                                    FS_SEC_QTY         cnt,
                                    FS_ERR            *p_err);
#endif

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void          FSVol_WrLocked       (FS_VOL            *p_vol,       /* Write data to volume sector(s).                  */
                                    void              *p_src,
                                    FS_SEC_NBR         start,
                                    FS_SEC_QTY         cnt,
                                    FS_ERR            *p_err);

void          FSVol_WrLockedEx     (FS_VOL            *p_vol,       /* Write data to volume sector(s).                  */
                                    void              *p_src,
                                    FS_SEC_NBR         start,
                                    FS_SEC_QTY         cnt,
                                    FS_FLAGS           sec_type,
                                    FS_ERR            *p_err);
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

