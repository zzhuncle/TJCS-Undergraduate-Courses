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
*                                          uC/FS INIT CODE
*
* File : ucos_fs_init.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <app_cfg.h>
#include <cpu.h>
#include <ucos_bsp.h>
#include <ucos_int.h>
#include <xparameters.h>
#include <KAL/kal.h>

#if (APP_FS_ENABLED == DEF_ENABLED)
#include  <Source/fs.h>
#include  <Source/fs_vol.h>

#if (UCOS_RAMDISK_EN == DEF_ENABLED)
#include <Dev/RAMDisk/fs_dev_ramdisk.h>
#endif

#if (UCOS_SDCARD_EN == DEF_ENABLED)
#include <Dev/SD/Card/fs_dev_sd_card.h>
#endif

#if (APP_USBH_ENABLED == DEF_ENABLED)
#include <Dev/MSC/fs_dev_msc.h>
#endif

#if (UCOS_RAMDISK_EN == DEF_ENABLED)
static CPU_BOOLEAN UCOS_FS_RAMDiskInit (void);
#endif

#if (UCOS_SDCARD_EN == DEF_ENABLED)
static CPU_BOOLEAN UCOS_FS_SDCardInit (void);
#endif

static  const  FS_CFG  App_FS_Cfg = {
    FS_CFG_MAX_DEV_CNT,                                         /* DevCnt                                               */
    FS_CFG_MAX_VOL_CNT,                                         /* VolCnt                                               */
    FS_CFG_MAX_FILE_CNT,                                        /* FileCnt                                              */
    FS_CFG_MAX_DIR_CNT,                                         /* DirCnt                                               */
    FS_CFG_MAX_BUF_CNT,                                         /* BufCnt                                               */
    FS_CFG_MAX_DEV_DRV_CNT,                                     /* DevDrvCnt                                            */
    FS_CFG_MAX_SEC_SIZE,                                        /* MaxSecSize                                           */
};

#if (UCOS_RAMDISK_EN == DEF_ENABLED)
    CPU_INT08U RAMDiskTbl[UCOS_RAMDISK_SIZE*UCOS_RAMDISK_SECTOR_SIZE];

    FS_DEV_RAM_CFG ramdisk_cfg = {
        UCOS_RAMDISK_SECTOR_SIZE,
        UCOS_RAMDISK_SIZE,
        (void *)RAMDiskTbl
    };

#endif

CPU_BOOLEAN UCOS_FS_Init (void)
{
    FS_ERR  err_fs;


#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
    UCOS_Print("UCOS - Initializing uC/FS.\r\n");
#endif

    err_fs = FS_Init((FS_CFG *)&App_FS_Cfg);

    if (err_fs != FS_ERR_NONE) {
#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
        UCOS_Printf("UCOS - Error initializing uC/FS. Error code %d\r\n", err_fs);
#endif
        return (DEF_FAIL);
    }

#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
    UCOS_Print("UCOS - uC/FS initialization successful.\r\n");
#endif

#if (UCOS_RAMDISK_EN == DEF_ENABLED)
    UCOS_FS_RAMDiskInit();
#endif

#if (UCOS_SDCARD_EN == DEF_ENABLED)
    UCOS_FS_SDCardInit();
#endif

#if (APP_USBH_ENABLED == DEF_ENABLED)
    FS_DevDrvAdd((FS_DEV_API *)&FSDev_MSC, &err_fs);
#endif

}

#if (UCOS_RAMDISK_EN == DEF_ENABLED)
static CPU_BOOLEAN UCOS_FS_RAMDiskInit (void)
{
    FS_ERR  err_fs;

#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
    UCOS_Print("UCOS - Creating RAMDisk.\r\n");
#endif

    FS_DevDrvAdd((FS_DEV_API *)&FSDev_RAM, &err_fs);
    if (err_fs != FS_ERR_NONE) {
#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
        UCOS_Printf("UCOS - Error adding RAMDisk driver. FS_DevDrvAdd() returned error code %d\r\n", err_fs);
#endif
        return (DEF_FAIL);
    }

    FSDev_Open("ram:0:", &ramdisk_cfg, &err_fs);
    if (err_fs != FS_ERR_NONE) {
#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
        UCOS_Printf("UCOS - Error opening RAMDisk driver. FSDev_Open() returned error code %d\r\n", err_fs);
#endif
        return (DEF_FAIL);
    }

    FSVol_Open("ram:0:", "ram:0:", 0, &err_fs);
    if (err_fs != FS_ERR_PARTITION_NOT_FOUND) {
#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
        UCOS_Printf("UCOS - Error opening RAMDisk volume. FSVol_Open() returned error code %d\r\n", err_fs);
#endif
        return (DEF_FAIL);
    }

    FSVol_Fmt("ram:0:", (void *)0, &err_fs);
    if (err_fs != FS_ERR_NONE) {
#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
        UCOS_Printf("UCOS - Error formatting RAMDisk volume. FSVol_Fmt() returned error code %d\r\n", err_fs);
#endif
        return (DEF_FAIL);
    }

#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
    UCOS_Print("UCOS - Volume ram:0: opened\r\n");
#endif
}
#endif /* #if (UCOS_RAMDISK_EN == DEF_ENABLED) */


#if (UCOS_SDCARD_EN == DEF_ENABLED)
static CPU_BOOLEAN UCOS_FS_SDCardInit (void)
{
    FS_ERR  err_fs;

#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
    UCOS_Print("UCOS - Initializing SDCard.\r\n");
#endif

    FS_DevDrvAdd((FS_DEV_API *)&FSDev_SD_Card, &err_fs);
    if (err_fs != FS_ERR_NONE) {
#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
        UCOS_Printf("UCOS - Error adding SD Card driver. FS_DevDrvAdd() returned error code %d\r\n", err_fs);
#endif
        return (DEF_FAIL);
    }

    FSDev_Open("sdcard:0:", DEF_NULL, &err_fs);
    if (err_fs != FS_ERR_NONE) {
#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
        UCOS_Printf("UCOS - Error opening SD Card device. FSDev_Open() returned error code %d\r\n", err_fs);
#endif
        return (DEF_FAIL);
    }

    FSVol_Open("sdcard:0:", "sdcard:0:", 0, &err_fs);
    if (err_fs != FS_ERR_NONE && err_fs != FS_ERR_PARTITION_NOT_FOUND) {
#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
        UCOS_Printf("UCOS - Error opening SD Card volume. FSVol_Open() returned error code %d\r\n", err_fs);
#endif
        return (DEF_FAIL);
    }

#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
    if (err_fs == FS_ERR_PARTITION_NOT_FOUND) {
        UCOS_Print("UCOS - Unformatted volume sdcard:0: opened\r\n");
    } else {
        UCOS_Print("UCOS - Volume sdcard:0: opened\r\n");
    }
#endif

#if 0 /* Do not format the sd card by default. */
    if (err_fs == FS_ERR_PARTITION_NOT_FOUND) {
        FSVol_Fmt("sdcard:0:", (void *)0, &err_fs);
        if (err_fs != FS_ERR_NONE) {
#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
            UCOS_Printf("UCOS - Error formatting SD Card volume. FSVol_Fmt() returned error code %d\r\n", err_fs);
#endif
            return (DEF_FAIL);
        }
    }
#endif
}
#endif /* #if (UCOS_SDCARD_EN == DEF_ENABLED) */

#endif /* #if (APP_FS_ENABLED == DEF_ENABLED) */
