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
*                                      FILE SYSTEM DEVICE DRIVER
*
*                                          NOR FLASH DEVICES
*
* Filename : fs_dev_nor.h
* Version  : V4.07.07
*********************************************************************************************************
* Note(s)  : (1) Supports NOR-type Flash memory devices, including :
*                (a) Parallel NOR Flash.
*                    (1) AMD-compatible devices.
*                    (2) Intel-compatible devices.
*                    (3) SST devices.
*                    (4) Any other similar memory devices.
*
*                (b) Serial   NOR Flash.
*                    (1) ST M25 SPI devices.
*                    (2) SST SST25 SPI devices.
*                    (3) Any other similar memory devices.
*
*            (2) Supported media MUST have the following characteristics :
*                (a) Medium organized into units (called blocks) which are erased at the same time.
*                (b) When erased, all bits are 1.
*                (c) Only an erase operation can change a bit from a 0 to a 1.
*                (d) Any bit  can be individually programmed  from a 1 to a 0.
*                (e) Any word can be individually accessed (read or programmed).
*
*            (3) Supported media TYPICALLY have the following characteristics :
*                (a) A program operation takes much longer than a read    operation.
*                (b) An erase  operation takes much longer than a program operation.
*                (c) The number of erase operations per block is limited.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_DEV_NOR_PRESENT
#define  FS_DEV_NOR_PRESENT


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_DEV_NOR_MODULE
#define  FS_DEV_NOR_EXT
#else
#define  FS_DEV_NOR_EXT  extern
#endif


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "../../Source/fs_dev.h"


/*
*********************************************************************************************************
*                                        DEFAULT CONFIGURATION
*********************************************************************************************************
*/

#ifndef  FS_DEV_NOR_CFG_WR_CHK_EN
#define  FS_DEV_NOR_CFG_WR_CHK_EN                   DEF_DISABLED
#endif

#ifndef  FS_DEV_NOR_CFG_DBG_CHK_EN
#define  FS_DEV_NOR_CFG_DBG_CHK_EN                  DEF_DISABLED
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  FS_DEV_NOR_ERASE_CNT_DIFF_TH_MIN                  5u
#define  FS_DEV_NOR_ERASE_CNT_DIFF_TH_MAX               1000u
#define  FS_DEV_NOR_ERASE_CNT_DIFF_TH_DFLT                20u

#define  FS_DEV_NOR_PCT_RSVD_MIN                           5u
#define  FS_DEV_NOR_PCT_RSVD_MAX                          35u
#define  FS_DEV_NOR_PCT_RSVD_DFLT                         10u

#define  FS_DEV_NOR_PCT_RSVD_SEC_ACTIVE_MAX               90u


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              NOR FLASH DEVICE PHYSICAL DATA DATA TYPE
*
* Note(s) : (1) (a) Prior to file system suite initialization, application may populate a
*                  'FS_DEV_NOR_PHY_DATA' structure & pass it directly to a physical-layer driver.
*
*               (b) At file system suite initialization, NOR driver will maintain a 'FS_DEV_NOR_PHY_DATA'
*                   populated based on configuration.  Thereafter, the application should NOT access the
*                   NOR device directly through the physical-layer driver; instead, the NOR driver
*                   raw access functions should be used.
*
*           (2) (a) Application/NOR driver populates 'UnitNbr' based on the device being addressed.  For
*                   example, if "nor:2:" is to be accessed, then 'UnitNbr' will hold the integer 2.
*
*               (b) Application/NOR driver populates 'AddrBase' & 'RegionNbr' to specify which NOR &
*                   NOR region to access.  See 'NOR FLASH DEVICE CONFIGURATION DATA TYPE  Note #1a, #1b'.
*
*               (c) Application/NOR driver populates 'BusWidth', 'BusWidthMax', 'PhyDevCnt' & 'MaxClkFreq'
*                   to determine bus/communication parameters.  See 'NOR FLASH DEVICE CONFIGURATION DATA TYPE  Note #1i'.
*
*               (d) Physical-layer driver populates 'BlkCnt', 'BlkSize' & 'AddrRegionStart', which describe
*                   the block region 'RegionNbr' to be accessed.  It MAY assign a pointer to physical-
*                   layer driver-specific information to 'DataPtr'.
*********************************************************************************************************
*/

typedef  struct  fs_dev_nor_phy_data {
    FS_QTY               UnitNbr;

    CPU_ADDR             AddrBase;                              /* Base address of flash.                               */
    CPU_INT08U           RegionNbr;                             /* Block region within flash.                           */

    CPU_INT08U           BusWidth;                              /* Bus width of flash.                                  */
    CPU_INT08U           BusWidthMax;                           /* Maximum bus width of flash.                          */
    CPU_INT08U           PhyDevCnt;                             /* Number of devices interleaved for flash.             */
    CPU_INT32U           MaxClkFreq;                            /* Maximum clock frequency of serial flash.             */

    FS_SEC_QTY           BlkCnt;                                /* Total number of blocks in region.                    */
    FS_SEC_SIZE          BlkSize;                               /* Size of each block in region.                        */
    CPU_ADDR             AddrRegionStart;                       /* Start address of block region within NOR.            */
    void                *DataPtr;                               /* Pointer to phy-specific data.                        */

    CPU_INT32U           WrMultSize;                            /* Flash write multiple size.                           */
} FS_DEV_NOR_PHY_DATA;

/*
*********************************************************************************************************
*                           NOR FLASH DEVICE PHYSICAL DRIVER API DATA TYPE
*********************************************************************************************************
*/

typedef  struct  fs_dev_nor_phy_api {
    void  (*Open)     (FS_DEV_NOR_PHY_DATA  *p_phy_data,
                       FS_ERR               *p_err);

    void  (*Close)    (FS_DEV_NOR_PHY_DATA  *p_phy_data);

    void  (*Rd)       (FS_DEV_NOR_PHY_DATA  *p_phy_data,
                       void                 *p_dest,
                       CPU_INT32U            start,
                       CPU_INT32U            cnt,
                       FS_ERR               *p_err);

    void  (*Wr)       (FS_DEV_NOR_PHY_DATA  *p_phy_data,
                       void                 *p_src,
                       CPU_INT32U            start,
                       CPU_INT32U            cnt,
                       FS_ERR               *p_err);

    void  (*EraseBlk) (FS_DEV_NOR_PHY_DATA  *p_phy_data,
                       CPU_INT32U            start,
                       CPU_INT32U            size,
                       FS_ERR               *p_err);

    void  (*IO_Ctrl)  (FS_DEV_NOR_PHY_DATA  *p_phy_data,
                       CPU_INT08U            cmd,
                       void                 *p_buf,
                       FS_ERR               *p_err);
} FS_DEV_NOR_PHY_API;

/*
*********************************************************************************************************
*                              NOR FLASH DEVICE CONFIGURATION DATA TYPE
*
* Note(s) : (1) The user must specify a valid configuration structure with configuration data.
*
*               (a) 'AddrBase' MUST specify
*                   (1) ... the base address of the flash memory, for a parallel flash.
*                   (2) ... 0x00000000 for a serial flash.
*
*               (b) 'RegionNbr' MUST specify the block region which will be used for the file system area.
*                    Block regions are enumerated by the physical-layer driver; for more information, see
*                    the physical-layer driver header file.  (On monolithic devices, devices with only one
*                    block region, this MUST be 0.)
*
*               (c) 'AddrStart' MUST specify
*                   (1) ... the absolute start address of the file system area in the flash memory, for
*                           a parallel flash.
*                   (2) ... the offset of the start of the file system area in the flash, for a serial
*                           flash.
*
*                   The address specified by 'AddrStart' MUST lie within the region 'RegionNbr' (see Note
*                   #1b).
*
*               (d) 'DevSize' MUST specify the number of octets that will belong to the file system area.
*
*               (e) 'SecSize' MUST specify the sector size for the low-level flash format (either 512,
*                   1024, 2048 or 4096).
*
*               (f) (a) 'PctRsvd' MUST specify the percentage of sectors on the flash that will be
*                       reserved for extra-file system storage (to improve efficiency).  This value must
*                       be between 5% & 35%, except if 0 is specified whereupon the default will be used (10%).
*
*               (g) 'EraseCntDiffTh' MUST specify the difference between minimum & maximum erase counts
*                   that will trigger passive wear-leveling.  This value must be between 5 & 100, except
*                   if 0 is specified whereupon the default will be used (20).
*
*               (h) 'PhyPtr' MUST point to the appropriate physical-layer driver :
*
*                       FSDev_NOR_AMD_1x08      CFI-compatible parallel NOR implementing AMD   command set,
*                                                   8-bit data bus
*                       FSDev_NOR_AMD_1x16      CFI-compatible parallel NOR implementing AMD   command set,
*                                                   16-bit data bus
*                       FSDev_NOR_Intel_1x16    CFI-compatible parallel NOR implementing Intel command set,
*                                                   16-bit data bus
*                       FSDev_NOR_SST39         SST SST39 Multi-Purpose Flash
*                       FSDev_NOR_STM25         ST  M25   serial flash
*                       FSDev_NOR_SST25         SST SST25 serial flash
*                       Other                   User-developed
*
*                   For lists of devices supported by each driver, please see the physical layer driver's
*                   header file.
*
*               (i) (1) For a parallel flash, the bus configuration is specified via 'BusWidth',
*                       'BusWidthMax' & 'PhyDevCnt' :
*                       (A) 'BusWidth' is the bus width, in bits, between the MCU/MPU & each connected device.
*                       (B) 'BusWidthMax' is the maximum width supported by each connected device.
*                       (C) 'PhyDevCnt' is the number of devices interleaved on the bus.
*
*                       For example, if a single flash capable of x8 or x16 operation is located on the
*                       bus & used in x8 mode, then
*                           BusWidth    =  8
*                           BusWidthMax = 16
*                           PhyDevCnt   =  1
*                       If two NORs operating in x16 mode are interleaved to form a x32 NOR, then
*                           BusWidth    = 16
*                           BuxWidthMax = 16
*                           PhyDevCnt   =  2
*
*                   (2) For a serial flash, the serial configuration is specified via 'MaxClkFreq'.
*                       'MaxClkFreq' is the maximum clock frequency of the serial flash.
*********************************************************************************************************
*/

typedef  struct  fs_dev_nor_cfg {
    CPU_ADDR             AddrBase;                              /* Base address of flash.                               */
    CPU_INT08U           RegionNbr;                             /* Block region within flash.                           */

    CPU_ADDR             AddrStart;                             /* Start address of data within flash.                  */
    CPU_INT32U           DevSize;                               /* Size of flash, in octets.                            */
    FS_SEC_SIZE          SecSize;                               /* Sector size of low-level formatted flash.            */
    CPU_INT08U           PctRsvd;                               /* Percentage of device area reserved.                  */
    CPU_INT16U           EraseCntDiffTh;                        /* Erase count difference threshold.                    */

    FS_DEV_NOR_PHY_API  *PhyPtr;                                /* Pointer to phy driver.                               */

    CPU_INT08U           BusWidth;                              /* Bus width of flash.                                  */
    CPU_INT08U           BusWidthMax;                           /* Maximum bus width of flash.                          */
    CPU_INT08U           PhyDevCnt;                             /* Number of flash devices interleaved.                 */
    CPU_INT32U           MaxClkFreq;                            /* Maximum clock frequency of serial flash.             */
} FS_DEV_NOR_CFG;


/*
*********************************************************************************************************
*                                     NOR IO CTRL DATA DATA TYPE
*********************************************************************************************************
*/

typedef  struct  fs_dev_nor_io_ctrl_data {
    void                 *DataPtr;
    CPU_INT32U            Start;
    CPU_INT32U            Size;
} FS_DEV_NOR_IO_CTRL_DATA;


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

extern          const  FS_DEV_API          FSDev_NOR;
FS_DEV_NOR_EXT         FS_CTR              FSDev_NOR_UnitCtr;

/*
*********************************************************************************************************
*                                       PHYSICAL LAYER DRIVERS
*********************************************************************************************************
*/

extern          const  FS_DEV_NOR_PHY_API  FSDev_NOR_AMD_1x08;
extern          const  FS_DEV_NOR_PHY_API  FSDev_NOR_AMD_1x16;

extern          const  FS_DEV_NOR_PHY_API  FSDev_NOR_Intel_1x16;

extern          const  FS_DEV_NOR_PHY_API  FSDev_NOR_SST25;
extern          const  FS_DEV_NOR_PHY_API  FSDev_NOR_SST39_1x16;

extern          const  FS_DEV_NOR_PHY_API  FSDev_NOR_STM25;
extern          const  FS_DEV_NOR_PHY_API  FSDev_NOR_STM29_1x08;
extern          const  FS_DEV_NOR_PHY_API  FSDev_NOR_STM29_1x16;

extern          const  FS_DEV_NOR_PHY_API  FSDev_NOR_AT25;

extern          const  FS_DEV_NOR_PHY_API  FSDev_NOR_Micron_NP5Q;

extern          const  FS_DEV_NOR_PHY_API  FSDev_NOR_W25Q80;


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

                                                                                /* ---------- LOW-LEVEL FNCTS --------- */
void         FSDev_NOR_LowFmt               (CPU_CHAR              *name_dev,   /* Low-level format  device.            */
                                             FS_ERR                *p_err);

void         FSDev_NOR_LowMount             (CPU_CHAR              *name_dev,   /* Low-level mount   device.            */
                                             FS_ERR                *p_err);

void         FSDev_NOR_LowUnmount           (CPU_CHAR              *name_dev,   /* Low-level unmount device.            */
                                             FS_ERR                *p_err);

void         FSDev_NOR_LowCompact           (CPU_CHAR              *name_dev,   /* Low-level compact device.            */
                                             FS_ERR                *p_err);

void         FSDev_NOR_LowDefrag            (CPU_CHAR              *name_dev,   /* Low-level defrag  device.            */
                                             FS_ERR                *p_err);

                                                                                /* ---------- PHYSICAL FNCTS ---------- */
void         FSDev_NOR_PhyRd                (CPU_CHAR              *name_dev,   /* Read data from physical device.      */
                                             void                  *p_dest,
                                             CPU_INT32U             start,
                                             CPU_INT32U             cnt,
                                             FS_ERR                *p_err);

void         FSDev_NOR_PhyWr                (CPU_CHAR              *name_dev,   /* Write data to physical device.       */
                                             void                  *p_src,
                                             CPU_INT32U             start,
                                             CPU_INT32U             cnt,
                                             FS_ERR                *p_err);

void         FSDev_NOR_PhyEraseBlk          (CPU_CHAR              *name_dev,   /* Erase block on physical device.      */
                                             CPU_INT32U             start,
                                             CPU_INT32U             size,
                                             FS_ERR                *p_err);

void         FSDev_NOR_PhyEraseChip         (CPU_CHAR              *name_dev,   /* Erase entire physical device.        */
                                             FS_ERR                *p_err);

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                 DEFINED IN BSP'S 'bsp_fs_dev_nor.c'
*********************************************************************************************************
*/

CPU_BOOLEAN  FSDev_NOR_BSP_Open             (FS_QTY                 unit_nbr,   /* Open (init) NOR bus interface.       */
                                             CPU_ADDR               addr_base,
                                             CPU_INT08U             bus_width,
                                             CPU_INT08U             phy_dev_cnt);

void         FSDev_NOR_BSP_Close            (FS_QTY                 unit_nbr);  /* Close (uninit) NOR bus interface.    */

void         FSDev_NOR_BSP_Rd_08            (FS_QTY                 unit_nbr,   /* Read  from bus interface  (8-bit).   */
                                             void                  *p_dest,
                                             CPU_ADDR               addr_src,
                                             CPU_INT32U             cnt);

void         FSDev_NOR_BSP_Rd_16            (FS_QTY                 unit_nbr,   /* Read  from bus interface (16-bit).   */
                                             void                  *p_dest,
                                             CPU_ADDR               addr_src,
                                             CPU_INT32U             cnt);

CPU_INT08U   FSDev_NOR_BSP_RdWord_08        (FS_QTY                 unit_nbr,   /* Read  from bus interface  (8-bit).   */
                                             CPU_ADDR               addr_src);

CPU_INT16U   FSDev_NOR_BSP_RdWord_16        (FS_QTY                 unit_nbr,   /* Read  from bus interface (16-bit).   */
                                             CPU_ADDR               addr_src);

void         FSDev_NOR_BSP_WrWord_08        (FS_QTY                 unit_nbr,   /* Write to   bus interface  (8-bit).   */
                                             CPU_ADDR               addr_dest,
                                             CPU_INT08U             datum);

void         FSDev_NOR_BSP_WrWord_16        (FS_QTY                 unit_nbr,   /* Write to   bus interface (16-bit).   */
                                             CPU_ADDR               addr_dest,
                                             CPU_INT16U             datum);

CPU_BOOLEAN  FSDev_NOR_BSP_WaitWhileBusy    (FS_QTY                 unit_nbr,   /* Wait while NOR busy.                 */
                                             FS_DEV_NOR_PHY_DATA   *p_phy_data,
                                             CPU_BOOLEAN          (*poll_fnct)(FS_DEV_NOR_PHY_DATA  *p_phy_data_arg),
                                             CPU_INT32U             to_us);

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                 DEFINED IN BSP'S 'bsp_fs_dev_nor.c'
*
* Note(s) : (1) SPI functions MUST be gathered into a SPI API structure.
*********************************************************************************************************
*/

extern  const  FS_DEV_SPI_API  FSDev_NOR_BSP_SPI;

/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

#ifndef  FS_DEV_NOR_CFG_WR_CHK_EN
#error  "FS_DEV_NOR_CFG_WR_CHK_EN          not #define'd in 'app_cfg.h'"
#error  "                            [MUST be  DEF_DISABLED]           "
#error  "                            [     ||  DEF_ENABLED ]           "

#elif  ((FS_DEV_NOR_CFG_WR_CHK_EN != DEF_DISABLED) && \
        (FS_DEV_NOR_CFG_WR_CHK_EN != DEF_ENABLED ))
#error  "FS_DEV_NOR_CFG_WR_CHK_EN    illegally #define'd in 'app_cfg.h'"
#error  "                            [MUST be  DEF_DISABLED]           "
#error  "                            [     ||  DEF_ENABLED ]           "
#endif


#ifndef  FS_DEV_NOR_CFG_DBG_CHK_EN
#error  "FS_DEV_NOR_CFG_DBG_CHK_EN         not #define'd in 'app_cfg.h'"
#error  "                            [MUST be  DEF_DISABLED]           "
#error  "                            [     ||  DEF_ENABLED ]           "

#elif  ((FS_DEV_NOR_CFG_DBG_CHK_EN != DEF_DISABLED) && \
        (FS_DEV_NOR_CFG_DBG_CHK_EN != DEF_ENABLED ))
#error  "FS_DEV_NOR_CFG_DBG_CHK_EN   illegally #define'd in 'app_cfg.h'"
#error  "                            [MUST be  DEF_DISABLED]           "
#error  "                            [     ||  DEF_ENABLED ]           "
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of NOR module include.                           */
