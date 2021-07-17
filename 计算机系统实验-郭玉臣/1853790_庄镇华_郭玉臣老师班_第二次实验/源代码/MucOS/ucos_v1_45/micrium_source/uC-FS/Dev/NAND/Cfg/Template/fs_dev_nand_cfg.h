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
*                                   NAND DRIVER CONFIGURATION FILE
*
*                                              TEMPLATE
*
* Filename : fs_nand_cfg.c
* Version  : V4.07.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef  FS_NAND_CFG_H
#define  FS_NAND_CFG_H


/*
*********************************************************************************************************
*                                                INCLUDE
*********************************************************************************************************
*/

#include <lib_def.h>


/*
*********************************************************************************************************
*                                      NAND DRIVER CONFIGURATION
*
* Note(s) : (1) FS_NAND_CFG_MAX_CTRLR_IMPL determines the maximum number of registered NAND controller
*               implementations. Each controller that will be used needs to be registered. Registering
*               a controller implementation will invoke its 'Init()' function.

*           (2) FS_NAND_CFG_AUTO_SYNC_EN determines if, for each operation on the device (i.e. each call
*               to the device's API), the metadata should be synchronized. Synchronizing at the end of
*               each operation is safer; it ensures the device can be remounted and appear exactly as it
*               should. Disabling automatic synchronization will result in a large write speed increase,
*               as the metadata won't be committed automatically, unless done in the application. If a
*               power down occurs between a device operation and a sync operation, the device will appear
*               as it was in a prior state when remounted. Device synchronization can be forced with a
*               call to FSDev_Sync().
*
*               Note that using large write buffers will reduce the metadata synchronization performance
*               hit as fewer calls to the device API will be needed.
*
*           (3) FS_NAND_CFG_UPDATE_BLK_META_CACHE_EN determines if, for each update block, the
*               metadata will be cached. Enabling this will allow searching for a specific updated sector
*               through data in RAM instead of accessing the device, which would require additional read
*               page operations.
*
*               More RAM will be consumed if this option is enabled, but write/read speed will be improved.
*
*               RAM usage = (<Nbr update blks> x (log2(<Max associativity>) + log2(<Nbr secs per blk>)) /
*                            8) octets (rounded up).
*
*           (4) FS_NAND_CFG_DIRTY_MAP_CACHE_EN determines if the dirty blocks map will be cached. With
*               this feature enabled, a copy of the dirty blocks map on the device is cached. It is
*               possible then to determine if the state "dirty" of a block is commited on the device
*               without the need to actually read the device.
*
*               With this feature enabled, overall write and read speed should be improved. Also,
*               robustness will be improved for specific cases. However, more RAM will be consumed.
*
*               RAM usage = (<Nbr of blks on device> / 8) octets (rounded up).
*
*           (5) FS_NAND_CFG_UPDATE_BLK_TBL_SUBSET_SIZE controls the size of the subsets of sectors pointed
*               by each entry of the update block tables. The value must be a power of 2 (or 0).
*
*               If, for example, the value is 4, each time a specific updated sector is requested, the
*               NAND translation layer must find the sector from a group of 4 sectors. Thus, if the cache
*               is disabled, 4 sectors must be read from the device. Otherwise, the 4 entries will be
*               searched from in the cache. If the value is set to 0, the table will be disabled
*               completely, meaning that all sectors of the block might have to be be read before the
*               specified sector is found. If the value is 1, the table completely specifies the location
*               of the sector, and thus no search must be performed. In that case, enabling the update
*               blocks metadata cache will yield no performance benefit.
*
*               RAM usage = (<Nbr update blks> x (log2(Nbr secs per blk>) - log2(<Subset size>) x
*                            <Max associativity> / 8) octets (rounded up).
*
*           (6) FS_NAND_CFG_RSVD_AVAIL_BLK_CNT indicates the number of blocks in the available blocks
*               table that are reserved for metadata block folding. Since this operation is critical
*               and must be done before adding blocks to the available blocks table, the driver needs
*               enough reserved blocks to make sure at least one of them is not bad so that the metadata
*               can be folded successfully. When set to 3, probability for the metadata folding operation
*               to fail is really low. This value should be sufficient for most applications.
*
*           (7) FS_NAND_CFG_MAX_RD_RETRIES indicates the maximum number of retries performed when a read
*               operation fails. It is recommended by most manufacturers to retry reading a page if it
*               fails, as successive read operations might be successful. This number should be at least
*               set to 2 for smooth  operation, but might be set higher to improve reliability.
*
*           (8) FS_NAND_CFG_MAX_SUB_PCT indicates the maximum number of update blocks that can be
*               sequential update blocks (SUB). This value is set as a percentage of the total number
*               of update blocks.
*
*********************************************************************************************************
*/

                                                                /* Config max nbr of reg'd ctrlr layer impl.            */
#define  FS_NAND_CFG_MAX_CTRLR_IMPL                       1u    /*                                     (see Note #1)  : */

                                                                /* Config auto sync                    (see Note #2)  : */
#define  FS_NAND_CFG_AUTO_SYNC_EN                DEF_ENABLED
                                                                /*   DEF_DISABLED   auto sync of meta data disabled.    */
                                                                /*   DEF_ENABLED    auto sync of meta data enabled.     */

                                                                /* Config meta cache                   (see Note #3)  : */
#define  FS_NAND_CFG_UB_META_CACHE_EN            DEF_ENABLED
                                                                /*   DEF_DISABLED   meta cache NOT present.             */
                                                                /*   DEF_ENABLED    meta cache     present.             */

                                                                /* Config commited dirty map cache     (see Note #4)  : */
#define  FS_NAND_CFG_DIRTY_MAP_CACHE_EN          DEF_ENABLED
                                                                /*   DEF_DISABLED   dirty cache NOT present.            */
                                                                /*   DEF_ENABLED    dirty cache     present.            */

                                                                /* Config update blk tbl subset size   (see Note #5)  : */
#define  FS_NAND_CFG_UB_TBL_SUBSET_SIZE                   1u

                                                                /* Config cnt of rsvd avail blks       (see Note #6)  : */
#define  FS_NAND_CFG_RSVD_AVAIL_BLK_CNT                   3u

                                                                /* Config nbr retries after rd fail    (see Note #7)  : */
#define  FS_NAND_CFG_MAX_RD_RETRIES                      10u

                                                                /* Config max pct of UB that can be SUB(see Note #8)  : */
#define  FS_NAND_CFG_MAX_SUB_PCT                         30


/*
*********************************************************************************************************
*                                   NAND DRIVER ADVANCED CONFIGURATION
*
* Note(s) : (1) We strongly recommend to leave default values to these configurations. These are advanced
*               configurations that do not need to be modified.
*
*********************************************************************************************************
*/

                                                                /* ------------------ FTL TH CONFIG ------------------- */
                                                                /* Config th (see FS_NAND_SecWrInUB() note #2).         */
#define  FS_NAND_CFG_TH_PCT_MERGE_RUB_START_SUB          20
#define  FS_NAND_CFG_TH_PCT_CONVERT_SUB_TO_RUB           10     /* See also FS_NAND_UB_Alloc() note #2b.                */
#define  FS_NAND_CFG_TH_PCT_PAD_SUB                       5

                                                                /* Config th (see FS_NAND_UB_Alloc() note #2).          */
#define  FS_NAND_CFG_TH_PCT_MERGE_SUB                    10
#define  FS_NAND_CFG_TH_SUB_MIN_IDLE_TO_FOLD              5


/*
*********************************************************************************************************
*                                   NAND DRIVER DEBUG CONFIGURATION
*
* Note(s) : (1) We strongly recommend to leave default values to these configurations. These are advanced
*               configurations that usually do not need to be modified.
*
*********************************************************************************************************
*/

#define  FS_NAND_CFG_DUMP_SUPPORT_EN           DEF_DISABLED


/*
*********************************************************************************************************
*                             NAND DRIVER METADATA CORRUPTION CONFIGURATION
*
* Note(s) : (1) This configuration should only be enabled when there are issues with meta block corruption.
*               This includes issues where devices can no longer be low level formatted.
*
*           (2) If the device is being configured for release this configuration should NOT be left ENABLED.
*               It creates a large number of excessive writes and is recommended to be left DISABLED.
*
*********************************************************************************************************
*/

#define  FS_NAND_CFG_CLR_CORRUPT_METABLK     DEF_DISABLED


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif


