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
*                               FILE SYSTEM SUITE PARTITION MANAGEMENT
*
* Filename : fs_partition.h
* Version  : V4.07.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_PARTITION_H
#define  FS_PARTITION_H


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  "fs_cfg_fs.h"
#include  "fs_dev.h"
#include  "fs_err.h"
#include  "fs_type.h"


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_PARTITION_MODULE
#define  FS_PARTITION_EXT
#else
#define  FS_PARTITION_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*
* Note(s) : (1) [Ref 1] gives various values used in the partition type field of DOS partitions.  As
*               noted there, some operating systems do not rely on this marker during file system type
*               determination; others, such as Microsoft Windows, do.
*********************************************************************************************************
*/

#define  FS_PARTITION_DOS_TBL_ENTRY_SIZE                  16u

#define  FS_PARTITION_DOS_OFF_ENTRY_1                    446u
#define  FS_PARTITION_DOS_OFF_SIG                        510u

#define  FS_PARTITION_DOS_OFF_BOOT_FLAG_1               (FS_PARTITION_DOS_OFF_ENTRY_1 +  0u)
#define  FS_PARTITION_DOS_OFF_START_CHS_ADDR_1          (FS_PARTITION_DOS_OFF_ENTRY_1 +  1u)
#define  FS_PARTITION_DOS_OFF_PARTITION_TYPE_1          (FS_PARTITION_DOS_OFF_ENTRY_1 +  4u)
#define  FS_PARTITION_DOS_OFF_END_CHS_ADDR_1            (FS_PARTITION_DOS_OFF_ENTRY_1 +  5u)
#define  FS_PARTITION_DOS_OFF_START_LBA_1               (FS_PARTITION_DOS_OFF_ENTRY_1 +  8u)
#define  FS_PARTITION_DOS_OFF_SIZE_1                    (FS_PARTITION_DOS_OFF_ENTRY_1 + 12u)

#define  FS_PARTITION_DOS_BOOT_FLAG                     0x80u

#define  FS_PARTITION_TYPE_CHS_MICROSOFT_EXT            0x05u   /* See Note #1.                                         */
#define  FS_PARTITION_TYPE_FAT32_LBA                    0x0Cu
#define  FS_PARTITION_TYPE_FAT12_CHS                    0x01u
#define  FS_PARTITION_TYPE_FAT16_16_32MB                0x04u
#define  FS_PARTITION_TYPE_FAT16_CHS_32MB_2GB           0x06u
#define  FS_PARTITION_TYPE_FAT32_CHS                    0x0Bu
#define  FS_PARTITION_TYPE_FAT16_LBA_32MB_2GB           0x0Eu
#define  FS_PARTITION_TYPE_LBA_MICROSOFT_EXT            0x0Fu
#define  FS_PARTITION_TYPE_HID_FAT12_CHS                0x11u
#define  FS_PARTITION_TYPE_HID_FAT16_16_32MB_CHS        0x14u
#define  FS_PARTITION_TYPE_HID_FAT16_CHS_32MB_2GB       0x15u
#define  FS_PARTITION_TYPE_HID_CHS_FAT32                0x1Bu
#define  FS_PARTITION_TYPE_HID_LBA_FAT32                0x1Cu
#define  FS_PARTITION_TYPE_HID_FAT16_LBA_32MB_2GB       0x1Eu
#define  FS_PARTITION_TYPE_NTFS                         0x07u
#define  FS_PARTITION_TYPE_MICROSOFT_MBR                0x42u
#define  FS_PARTITION_TYPE_SOLARIS_X86                  0x82u
#define  FS_PARTITION_TYPE_LINUX_SWAP                   0x82u
#define  FS_PARTITION_TYPE_LINUX                        0x83u
#define  FS_PARTITION_TYPE_HIBERNATION_A                0x84u
#define  FS_PARTITION_TYPE_LINUX_EXT                    0x85u
#define  FS_PARTITION_TYPE_NTFS_VOLSETA                 0x86u
#define  FS_PARTITION_TYPE_NTFS_VOLSETB                 0x87u
#define  FS_PARTITION_TYPE_HIBERNATION_B                0xA0u
#define  FS_PARTITION_TYPE_HIBERNATION_C                0xA1u
#define  FS_PARTITION_TYPE_FREE_BSD                     0xA5u
#define  FS_PARTITION_TYPE_OPEN_BSD                     0xA6u
#define  FS_PARTITION_TYPE_MAX_OSX                      0xA8u
#define  FS_PARTITION_TYPE_NET_BSD                      0xA9u
#define  FS_PARTITION_TYPE_MAC_OSX_BOOT                 0xABu
#define  FS_PARTITION_TYPE_BSDI                         0xB7u
#define  FS_PARTITION_TYPE_BSDI_SWAP                    0xB8u
#define  FS_PARTITION_TYPE_EFI_GPT_DISK                 0xEEu
#define  FS_PARTITION_TYPE_EFI_SYS_PART                 0xEFu
#define  FS_PARTITION_TYPE_VMWARE_FILE_SYS              0xFBu
#define  FS_PARTITION_TYPE_VMWARE_SWAP                  0xFCu

#define  FS_PARTITION_DOS_NAME_EXT                 "Extended Partition"
#define  FS_PARTITION_DOS_NAME_FAT12               "FAT12"
#define  FS_PARTITION_DOS_NAME_FAT16               "FAT16"
#define  FS_PARTITION_DOS_NAME_FAT32               "FAT32"
#define  FS_PARTITION_DOS_NAME_OTHER               "Other"

#define  FS_PARTITION_DOS_CHS_SECTORS_PER_TRK             63u
#define  FS_PARTITION_DOS_CSH_HEADS_PER_CYLINDER         255u


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
*                                      PARTITION ENTRY DATA TYPE
*********************************************************************************************************
*/

struct  fs_partition_entry {
    FS_SEC_NBR  Start;                                          /* Start sec of partition.                              */
    FS_SEC_QTY  Size;                                           /* Size of partition.                                   */
    CPU_INT08U  Type;                                           /* Type of partition.                                   */
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

#if (FS_CFG_PARTITION_EN == DEF_ENABLED)
FS_PARTITION_NBR   FSPartition_GetNbrPartitions(FS_DEV              *p_dev, /* Get number of partitions on a device.    */
                                                FS_ERR              *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
FS_PARTITION_NBR   FSPartition_Add             (FS_DEV              *p_dev, /* Add partition to device.                 */
                                                FS_SEC_QTY           partition_size,
                                                FS_ERR              *p_err);
#endif

void               FSPartition_Find            (FS_DEV              *p_dev, /* Find partition on device.                */
                                                FS_PARTITION_NBR     partition_nbr,
                                                FS_PARTITION_ENTRY  *p_partition_entry,
                                                FS_ERR              *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void               FSPartition_Update          (FS_DEV              *p_dev, /* Update partition type.                   */
                                                FS_PARTITION_NBR     partition_nbr,
                                                CPU_INT08U           partition_type,
                                                FS_ERR              *p_err);
#endif
#else
void               FSPartition_FindSimple      (FS_DEV              *p_dev, /* Find partition on device.                */
                                                FS_PARTITION_ENTRY  *p_partition_entry,
                                                FS_ERR              *p_err);
#endif

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void               FSPartition_Init            (FS_DEV              *p_dev, /* Initialize partition (i.e., write MBR).  */
                                                FS_SEC_QTY           partition_size,
                                                FS_ERR              *p_err);
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
