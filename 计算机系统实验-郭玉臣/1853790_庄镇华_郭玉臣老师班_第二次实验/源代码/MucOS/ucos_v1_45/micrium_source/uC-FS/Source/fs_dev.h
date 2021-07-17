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
*                                 FILE SYSTEM SUITE DEVICE MANAGEMENT
*
* Filename : fs_dev.h
* Version  : V4.07.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_DEV_H
#define  FS_DEV_H


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_DEV_MODULE
#define  FS_DEV_EXT
#else
#define  FS_DEV_EXT  extern
#endif


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <fs_cfg.h>
#include  "fs_cfg_fs.h"
#include  "fs_ctr.h"
#include  "fs_err.h"
#include  "fs_type.h"


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        DEVICE STATE DEFINES
*********************************************************************************************************
*/

#define  FS_DEV_STATE_CLOSED                               0u   /* Dev closed.                                          */
#define  FS_DEV_STATE_CLOSING                              1u   /* Dev closing.                                         */
#define  FS_DEV_STATE_OPENING                              2u   /* Dev opening.                                         */
#define  FS_DEV_STATE_OPEN                                 3u   /* Dev open.                                            */
#define  FS_DEV_STATE_PRESENT                              4u   /* Dev present.                                         */
#define  FS_DEV_STATE_LOW_FMT_VALID                        5u   /* Dev low fmt valid.                                   */

/*
*********************************************************************************************************
*                                     DEVICE I/O CONTROL DEFINES
*********************************************************************************************************
*/

                                                                /* ------------------ GENERIC OPTIONS ----------------- */
#define  FS_DEV_IO_CTRL_REFRESH                            1u   /* Refresh dev.                                         */
#define  FS_DEV_IO_CTRL_LOW_FMT                            2u   /* Low-level fmt dev.                                   */
#define  FS_DEV_IO_CTRL_LOW_MOUNT                          3u   /* Low-level mount dev.                                 */
#define  FS_DEV_IO_CTRL_LOW_UNMOUNT                        4u   /* Low-level unmount dev.                               */
#define  FS_DEV_IO_CTRL_LOW_COMPACT                        5u   /* Low-level compact dev.                               */
#define  FS_DEV_IO_CTRL_LOW_DEFRAG                         6u   /* Low-level defrag dev.                                */
#define  FS_DEV_IO_CTRL_SEC_RELEASE                        7u   /* Release data in sec.                                 */
#define  FS_DEV_IO_CTRL_PHY_RD                             8u   /* Read  physical dev.                                  */
#define  FS_DEV_IO_CTRL_PHY_WR                             9u   /* Write physical dev.                                  */
#define  FS_DEV_IO_CTRL_PHY_RD_PAGE                       10u   /* Read  physical dev page.                             */
#define  FS_DEV_IO_CTRL_PHY_WR_PAGE                       11u   /* Write physical dev page.                             */
#define  FS_DEV_IO_CTRL_PHY_ERASE_BLK                     12u   /* Erase physical dev blk.                              */
#define  FS_DEV_IO_CTRL_PHY_ERASE_CHIP                    13u   /* Erase physical dev.                                  */
#define  FS_DEV_IO_CTRL_RD_SEC                            14u   /* Read  physical dev sector.                           */
#define  FS_DEV_IO_CTRL_WR_SEC                            15u   /* Write physical dev sector.                           */
#define  FS_DEV_IO_CTRL_SYNC                              16u   /* Sync dev.                                            */
#define  FS_DEV_IO_CTRL_CHIP_ERASE                        17u   /* Erase all data on phy dev.                           */

                                                                /* ------------ SD-DRIVER SPECIFIC OPTIONS ------------ */
#define  FS_DEV_IO_CTRL_SD_QUERY                          64u   /* Get info about SD/MMC card.                          */
#define  FS_DEV_IO_CTRL_SD_RD_CID                         65u   /* Read SD/MMC card Card ID reg.                        */
#define  FS_DEV_IO_CTRL_SD_RD_CSD                         66u   /* Read SD/MMC card Card-Specific Data reg.             */

                                                                /* ----------- NAND-DRIVER SPECIFIC OPTIONS ----------- */
#define  FS_DEV_IO_CTRL_NAND_PARAM_PG_RD                  80u   /* Read parameter-page from ONFI device.                */
#define  FS_DEV_IO_CTRL_NAND_DUMP                         81u   /* Dump raw NAND dev.                                   */

/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          DEVICE DATA TYPE
*********************************************************************************************************
*/

struct  fs_dev {
    FS_ID          ID;                                          /* Dev ID.                                              */
    FS_STATE       State;                                       /* State.                                               */
    FS_CTR         RefCnt;                                      /* Ref cnts.                                            */
    FS_CTR         RefreshCnt;                                  /* Refresh cnts.                                        */

    CPU_CHAR       Name[FS_CFG_MAX_DEV_NAME_LEN + 1u];          /* Dev name.                                            */
    FS_QTY         UnitNbr;                                     /* Dev unit nbr.                                        */
    FS_SEC_QTY     Size;                                        /* Size of dev (in secs).                               */
    FS_SEC_SIZE    SecSize;                                     /* Size of dev sec.                                     */
    CPU_BOOLEAN    Fixed;                                       /* Indicates whether device is fixed or removable.      */

    FS_QTY         VolCnt;                                      /* Nbr of open vols on this dev.                        */

    FS_DEV_API    *DevDrvPtr;                                   /* Ptr to dev drv for this dev.                         */
    void          *DataPtr;                                     /* Ptr to data specific for a device driver.            */

#if (FS_CFG_CTR_STAT_EN == DEF_ENABLED)
    FS_CTR         StatRdSecCtr;                                /* Nbr rd secs.                                         */
    FS_CTR         StatWrSecCtr;                                /* Nbr wr secs.                                         */
#endif

};

/*
*********************************************************************************************************
*                                        DEVICE INFO DATA TYPE
*********************************************************************************************************
*/

typedef  struct  fs_dev_info {
    FS_STATE      State;                                        /* Device state.                                        */
    FS_SEC_QTY    Size;                                         /* Size of dev (in secs).                               */
    FS_SEC_SIZE   SecSize;                                      /* Size of dev sec.                                     */
    CPU_BOOLEAN   Fixed;                                        /* Indicates whether device is fixed or removable.      */
} FS_DEV_INFO;


/*
*********************************************************************************************************
*                                     DEVICE DRIVER API DATA TYPE
*********************************************************************************************************
*/

struct  fs_dev_api {
    const  CPU_CHAR  *(*NameGet)           (void);                        /* Get base name of driver.                             */

    void              (*Init)              (FS_ERR       *p_err);         /* Initialize driver.                                   */

    void              (*Open)              (FS_DEV       *p_dev,          /* Open        a  device instance.                      */
                                            void         *p_dev_cfg,
                                            FS_ERR       *p_err);

    void              (*Close)             (FS_DEV       *p_dev);         /* Close       a  device instance.                      */

    void              (*Rd)                (FS_DEV       *p_dev,          /* Read from   a  device instance.                      */
                                            void         *p_dest,
                                            FS_SEC_NBR    start,
                                            FS_SEC_QTY    cnt,
                                            FS_ERR       *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
    void              (*Wr)                (FS_DEV       *p_dev,          /* Write to    a  device instance.                      */
                                            void         *p_src,
                                            FS_SEC_NBR    start,
                                            FS_SEC_QTY    cnt,
                                            FS_ERR       *p_err);
#endif

    void              (*Query)             (FS_DEV       *p_dev,          /* Get info about device instance.                      */
                                            FS_DEV_INFO  *p_dev_info,
                                            FS_ERR       *p_err);

    void              (*IO_Ctrl)           (FS_DEV       *p_dev,          /* Ctrl req to a  device instance.                      */
                                            CPU_INT08U    opt,
                                            void         *p_data,
                                            FS_ERR       *p_err);
};

/*
*********************************************************************************************************
*                                   DEVICE DRIVER SPI API DATA TYPE
*********************************************************************************************************
*/

typedef  struct  fs_dev_spi_api {
    CPU_BOOLEAN  (*Open)             (FS_QTY          unit_nbr);       /* Open (initialize) SPI.                */

    void         (*Close)            (FS_QTY          unit_nbr);       /* Close (uninitialize) SPI.             */

    void         (*Lock)             (FS_QTY          unit_nbr);       /* Acquire SPI lock.                     */

    void         (*Unlock)           (FS_QTY          unit_nbr);       /* Release SPI lock.                     */

    void         (*Rd)               (FS_QTY          unit_nbr,        /* Read from SPI.                        */
                                      void           *p_dest,
                                      CPU_SIZE_T      cnt);

    void         (*Wr)               (FS_QTY          unit_nbr,        /* Write to SPI.                         */
                                      void           *p_src,
                                      CPU_SIZE_T      cnt);

    void         (*ChipSelEn)        (FS_QTY          unit_nbr);       /* Enable chip select.                   */

    void         (*ChipSelDis)       (FS_QTY          unit_nbr);       /* Disable chip select.                  */

    void         (*SetClkFreq)       (FS_QTY          unit_nbr,        /* Set SPI clock frequency.              */
                                      CPU_INT32U      freq);
} FS_DEV_SPI_API;


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


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void               FS_DevDrvAdd             (FS_DEV_API          *p_dev_drv,   /* Add device driver to file system.        */
                                             FS_ERR              *p_err);

void               FSDev_Close              (CPU_CHAR            *name_dev,    /* Remove a device from the file system.    */
                                             FS_ERR              *p_err);

#if (FS_CFG_PARTITION_EN == DEF_ENABLED)
FS_PARTITION_NBR   FSDev_GetNbrPartitions   (CPU_CHAR            *name_dev,    /* Get number of partitions on a device.    */
                                             FS_ERR              *p_err);
#endif

void               FSDev_IO_Ctrl            (CPU_CHAR            *name_dev,    /* Perform device I/O control operation.    */
                                             CPU_INT08U           opt,
                                             void                *p_data,
                                             FS_ERR              *p_err);

void               FSDev_Open               (CPU_CHAR            *name_dev,    /* Add a device to the file system.         */
                                             void                *p_dev_cfg,
                                             FS_ERR              *p_err);

#if (FS_CFG_PARTITION_EN == DEF_ENABLED)
#if (FS_CFG_RD_ONLY_EN   == DEF_DISABLED)
FS_PARTITION_NBR   FSDev_PartitionAdd       (CPU_CHAR            *name_dev,    /* Add partition to a device.               */
                                             FS_SEC_QTY           partition_size,
                                             FS_ERR              *p_err);
#endif

void               FSDev_PartitionFind      (CPU_CHAR            *name_dev,    /* Find partition on a device.              */
                                             FS_PARTITION_NBR     partition_nbr,
                                             FS_PARTITION_ENTRY  *p_partition_entry,
                                             FS_ERR              *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void               FSDev_PartitionInit      (CPU_CHAR            *name_dev,    /* Initialize the partition on a device.    */
                                             FS_SEC_QTY           partition_size,
                                             FS_ERR              *p_err);
#endif
#endif

void               FSDev_Query              (CPU_CHAR            *name_dev,    /* Obtain information about a device.       */
                                             FS_DEV_INFO         *p_info,
                                             FS_ERR              *p_err);

void               FSDev_Rd                 (CPU_CHAR            *name_dev,    /* Read data from device sector(s).         */
                                             void                *p_dest,
                                             FS_SEC_NBR           start,
                                             FS_SEC_QTY           cnt,
                                             FS_ERR              *p_err);

CPU_BOOLEAN        FSDev_Refresh            (CPU_CHAR            *name_dev,    /* Refresh device.                          */
                                             FS_ERR              *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void               FSDev_Sync               (CPU_CHAR            *name_dev,    /* Sync device.                             */
                                             FS_ERR              *p_err);

void               FSDev_Wr                 (CPU_CHAR            *name_dev,    /* Write data to device sector(s).          */
                                             void                *p_src,
                                             FS_SEC_NBR           start,
                                             FS_SEC_QTY           cnt,
                                             FS_ERR              *p_err);
#endif

void               FSDev_Invalidate         (CPU_CHAR            *name_dev,    /* Invalidate opened volumes and files.     */
		                                     FS_ERR              *p_err);

void               FSDev_AccessLock         (CPU_CHAR            *name_dev,    /* Acquire device access lock.              */
                                             CPU_INT32U           timeout,
                                             FS_ERR              *p_err);

void               FSDev_AccessUnlock       (CPU_CHAR            *name_dev,    /* Release device access lock.              */
                                             FS_ERR              *p_err);


/*
*********************************************************************************************************
*                                   MANAGEMENT FUNCTION PROTOTYPES
*********************************************************************************************************
*/

FS_QTY             FSDev_GetDevCnt       (void);                            /* Get number of open devices.              */

FS_QTY             FSDev_GetDevCntMax    (void);                            /* Get max possible number of open devices. */

void               FSDev_GetDevName      (FS_QTY               dev_nbr,     /* Get name of nth open device.             */
                                          CPU_CHAR            *name_dev);

/*
*********************************************************************************************************
*                                    INTERNAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

                                                                            /* ------------ INITIALIZATION ------------ */
void               FSDev_ModuleInit      (FS_QTY               dev_cnt,     /* Initialize device module.                */
                                          FS_QTY               dev_drv_cnt,
                                          FS_ERR              *p_err);


                                                                            /* ------------ ACCESS CONTROL ------------ */
FS_DEV            *FSDev_AcquireLockChk  (CPU_CHAR            *name_dev,    /* Acquire device reference & lock.         */
                                          FS_ERR              *p_err);

FS_DEV            *FSDev_Acquire         (CPU_CHAR            *name_dev);   /* Acquire device reference.                */

void               FSDev_Release         (FS_DEV              *p_dev);      /* Release device reference.                */

void               FSDev_ReleaseUnlock   (FS_DEV              *p_dev);      /* Release device reference & lock.         */

CPU_BOOLEAN        FSDev_Lock            (FS_DEV              *p_dev);      /* Acquire device lock.                     */

void               FSDev_Unlock          (FS_DEV              *p_dev);      /* Release device lock.                     */


void               FSDev_VolAdd          (FS_DEV              *p_dev,       /* Add volume to open volume list.          */
                                          FS_VOL              *p_vol);

void               FSDev_VolRemove       (FS_DEV              *p_dev,       /* Remove volume from open volume list.     */
                                          FS_VOL              *p_vol);


                                                                            /* ------------- LOCKED ACCESS ------------ */
void               FSDev_QueryLocked     (FS_DEV              *p_dev,       /* Get information about a device.          */
                                          FS_DEV_INFO         *p_info,
                                          FS_ERR              *p_err);

void               FSDev_RdLocked        (FS_DEV              *p_dev,       /* Read data from device sector(s).         */
                                          void                *p_dest,
                                          FS_SEC_NBR           start,
                                          FS_SEC_QTY           cnt,
                                          FS_ERR              *p_err);

CPU_BOOLEAN        FSDev_RefreshLocked   (FS_DEV              *p_dev,       /* Refresh device.                          */
                                          FS_ERR              *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void               FSDev_ReleaseLocked   (FS_DEV              *p_dev,       /* Release device sector(s).                */
                                          FS_SEC_NBR           start,
                                          FS_SEC_QTY           cnt,
                                          FS_ERR              *p_err);

void               FSDev_SyncLocked      (FS_DEV              *p_dev,       /* Sync device.                             */
                                          FS_ERR              *p_err);
#endif

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void               FSDev_WrLocked        (FS_DEV              *p_dev,       /* Write data to device sector(s).          */
                                          void                *p_src,
                                          FS_SEC_NBR           start,
                                          FS_SEC_QTY           cnt,
                                          FS_ERR              *p_err);
#endif

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                      DEFINED IN OS'S  fs_os.c
*********************************************`***********************************************************
*/

void               FS_OS_Init            (FS_ERR              *p_err);      /* Create file system objects.                        */

void               FS_OS_Lock            (FS_ERR              *p_err);      /* Acquire access to file system.                     */

void               FS_OS_Unlock          (void);                            /* Release access to file system.                     */


void               FS_OS_DevInit         (FS_QTY               dev_cnt,     /* Create file system device objects.                 */
                                          FS_ERR              *p_err);

void               FS_OS_DevAccessLock   (FS_ID                dev_id,      /* Acquire global exclusive access to a device.       */
                                          CPU_INT32U           timeout,
                                          FS_ERR              *p_err);

void               FS_OS_DevAccessUnlock (FS_ID                dev_id);     /* Release global exclusive access to a device.       */

void               FS_OS_DevLock         (FS_ID                dev_id,      /* Acquire access to file system device.              */
                                          FS_ERR              *p_err);

void               FS_OS_DevUnlock       (FS_ID                dev_id);     /* Release access to file system device.              */

void               FS_OS_Dly_ms          (CPU_INT16U           ms);         /* Delay for specified time, in ms.                   */


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
