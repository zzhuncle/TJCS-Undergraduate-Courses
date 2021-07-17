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
*                                      FILE SYSTEM CONFIGURATION
*
* Filename : fs_cfg_fs.h
* Version  : V4.07.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_CFG_FS_H
#define  FS_CFG_FS_H


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <Source/clk.h>
#include  <fs_cfg.h>


/*
************************************************************************************************************************
*                                    LIBRARY CONFIGURATION ERRORS
************************************************************************************************************************
*/

#if (LIB_VERSION < 13800u)
#error  "lib_def.h, LIB_VERSION SHOULD be >= V1.38.00"
#endif


/*
*********************************************************************************************************
*                           FILE SYSTEM ERROR CHECKING CONFIGURATION ERRORS
*********************************************************************************************************
*/

                                                                /* ------------- FS_CFG_ERR_ARG_CHK_EXT_EN ------------ */
#ifndef  FS_CFG_ERR_ARG_CHK_EXT_EN
#error  "FS_CFG_ERR_ARG_CHK_EXT_EN                    not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  ((FS_CFG_ERR_ARG_CHK_EXT_EN != DEF_DISABLED) && \
        (FS_CFG_ERR_ARG_CHK_EXT_EN != DEF_ENABLED ))
#error  "FS_CFG_ERR_ARG_CHK_EXT_EN              illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif



                                                                /* ------------- FS_CFG_ERR_ARG_CHK_DBG_EN ------------ */
#ifndef  FS_CFG_ERR_ARG_CHK_DBG_EN
#error  "FS_CFG_ERR_ARG_CHK_DBG_EN                    not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  ((FS_CFG_ERR_ARG_CHK_DBG_EN != DEF_DISABLED) && \
        (FS_CFG_ERR_ARG_CHK_DBG_EN != DEF_ENABLED ))
#error  "FS_CFG_ERR_ARG_CHK_DBG_EN              illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif

/*
*********************************************************************************************************
*                              FILE SYSTEM COUNTER CONFIGURATION ERRORS
*********************************************************************************************************
*/

                                                                /* ---------------- FS_CFG_CTR_STAT_EN ---------------- */
#ifndef  FS_CFG_CTR_STAT_EN
#error  "FS_CFG_CTR_STAT_EN                           not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  ((FS_CFG_CTR_STAT_EN != DEF_DISABLED) && \
        (FS_CFG_CTR_STAT_EN != DEF_ENABLED ))
#error  "FS_CFG_CTR_STAT_EN                     illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif



                                                                /* ----------------- FS_CFG_CTR_ERR_EN ---------------- */
#ifndef  FS_CFG_CTR_ERR_EN
#error  "FS_CFG_CTR_ERR_EN                            not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  ((FS_CFG_CTR_ERR_EN != DEF_DISABLED) && \
        (FS_CFG_CTR_ERR_EN != DEF_ENABLED ))
#error  "FS_CFG_CTR_ERR_EN                      illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif

/*
*********************************************************************************************************
*                               FILE SYSTEM DEBUG CONFIGURATION ERRORS
*********************************************************************************************************
*/

                                                                /* --------------- FS_CFG_DBG_MEM_CLR_EN -------------- */
#ifndef  FS_CFG_DBG_MEM_CLR_EN
#error  "FS_CFG_DBG_MEM_CLR_EN                        not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  ((FS_CFG_DBG_MEM_CLR_EN != DEF_DISABLED) && \
        (FS_CFG_DBG_MEM_CLR_EN != DEF_ENABLED ))
#error  "FS_CFG_DBG_MEM_CLR_EN                  illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif



                                                                /* -------------- FS_CFG_DBG_WR_VERIFY_EN ------------- */
#ifndef  FS_CFG_DBG_WR_VERIFY_EN
#error  "FS_CFG_DBG_WR_VERIFY_EN                      not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  ((FS_CFG_DBG_WR_VERIFY_EN != DEF_DISABLED) && \
        (FS_CFG_DBG_WR_VERIFY_EN != DEF_ENABLED ))
#error  "FS_CFG_DBG_WR_VERIFY_EN                illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif

/*
*********************************************************************************************************
*                                      FILE SYSTEM CONFIGURATION
*********************************************************************************************************
*/

#ifdef   FS_CFG_SYS_DRV_SEL
#if     (FS_CFG_SYS_DRV_SEL == FS_SYS_DRV_SEL_FAT)
#define  FS_FAT_MODULE_PRESENT
#endif
#endif


#ifdef   FS_CFG_CACHE_EN
#if     (FS_CFG_CACHE_EN    == DEF_ENABLED)
#define  FS_CACHE_MODULE_PRESENT
#endif
#endif


#ifdef   FS_CFG_API_EN
#if     (FS_CFG_API_EN      == DEF_ENABLED)
#define  FS_API_MODULE_PRESENT
#endif
#endif


#ifdef   FS_CFG_DIR_EN
#if     (FS_CFG_DIR_EN      == DEF_ENABLED)
#define  FS_DIR_MODULE_PRESENT
#endif
#endif


#ifdef   FS_FAT_MODULE_PRESENT
#if     (FS_FAT_CFG_LFN_EN   == DEF_ENABLED)
#define  FS_FAT_LFN_MODULE_PRESENT
#endif

#if     (FS_FAT_CFG_FAT12_EN == DEF_ENABLED)
#define  FS_FAT_FAT12_MODULE_PRESENT
#endif

#if     (FS_FAT_CFG_FAT16_EN == DEF_ENABLED)
#define  FS_FAT_FAT16_MODULE_PRESENT
#endif

#if     (FS_FAT_CFG_FAT32_EN == DEF_ENABLED)
#define  FS_FAT_FAT32_MODULE_PRESENT
#endif

#ifdef   FS_FAT_CFG_JOURNAL_EN
#if     (FS_FAT_CFG_JOURNAL_EN  == DEF_ENABLED)
#define  FS_FAT_JOURNAL_MODULE_PRESENT
#endif
#endif
#endif


                                                                /* -------------- FS_CFG_MAX_DEV_NAME_LEN ------------- */
#ifndef  FS_CFG_MAX_DEV_NAME_LEN
#error  "FS_CFG_MAX_DEV_NAME_LEN                      not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  >= 1]                                 "

#elif   (FS_CFG_MAX_DEV_NAME_LEN < 1u)
#error  "FS_CFG_MAX_DEV_NAME_LEN                illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  >= 1]                                 "

#elif   (FS_CFG_MAX_DEV_NAME_LEN < FS_CFG_MAX_DEV_DRV_NAME_LEN + 3u)
#error  "FS_CFG_MAX_DEV_NAME_LEN                illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  >= FS_CFG_MAX_DEV_DRV_NAME_LEN + 3]   "

#elif   (FS_CFG_MAX_DEV_NAME_LEN > FS_CFG_MAX_DEV_DRV_NAME_LEN + 5u)
#error  "FS_CFG_MAX_DEV_NAME_LEN                illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  <= FS_CFG_MAX_DEV_DRV_NAME_LEN + 5]   "
#endif


                                                                /* --------------- FS_CFG_64_BITS_LBA_EN -------------- */
#ifndef  FS_CFG_64_BITS_LBA_EN
#error  "FS_CFG_64_BITS_LBA_EN                        not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  ((FS_CFG_64_BITS_LBA_EN != DEF_DISABLED) && \
        (FS_CFG_64_BITS_LBA_EN != DEF_ENABLED ))
#error  "FS_CFG_64_BITS_LBA_EN                  illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif


                                                                /* -------------- FS_CFG_BUF_ALIGN_OCTETS ------------- */
#ifndef  FS_CFG_BUF_ALIGN_OCTETS
#error  "FS_CFG_BUF_ALIGN_OCTETS                      not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  >= 1]                                 "
#endif


/*
*********************************************************************************************************
*                                   FILE SYSTEM NAME CONFIGURATION
*
* Note(s) : (1) Ideally, the Type module would define ALL file system lengths
*********************************************************************************************************
*/

#ifdef   FS_CFG_MAX_VOL_NAME_LEN
#ifdef   FS_CFG_MAX_PATH_NAME_LEN
#define  FS_CFG_MAX_FULL_NAME_LEN               (FS_CFG_MAX_VOL_NAME_LEN + FS_CFG_MAX_PATH_NAME_LEN)
#endif
#endif


/*
*********************************************************************************************************
*                                  FILE SYSTEM CONFIGURATION ERRORS
*
* Note(s) : (1) Only FAT currently supported.
*********************************************************************************************************
*/
                                                                /* ---------------- FS_CFG_SYS_DRV_SEL ---------------- */
                                                                /* See Note #1.                                         */
#ifndef  FS_CFG_SYS_DRV_SEL
#error  "FS_CFG_SYS_DRV_SEL                           not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  FS_SYS_DRV_SEL_FAT]                   "

#elif   (FS_CFG_SYS_DRV_SEL != FS_SYS_DRV_SEL_FAT)
#error  "FS_CFG_SYS_DRV_SEL                     illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  FS_SYS_DRV_SEL_FAT]                   "
#endif



                                                                /* ------------------ FS_CFG_CACHE_EN ----------------- */
                                                                /* See Note #2.                                         */
#ifndef  FS_CFG_CACHE_EN
#error  "FS_CFG_CACHE_EN                              not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  ((FS_CFG_CACHE_EN != DEF_DISABLED) && \
        (FS_CFG_CACHE_EN != DEF_ENABLED ))
#error  "FS_CFG_CACHE_EN                        illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif



                                                                /* ------------------- FS_CFG_API_EN ------------------ */
#ifndef  FS_CFG_API_EN
#error  "FS_CFG_API_EN                                not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  ((FS_CFG_API_EN != DEF_DISABLED) && \
        (FS_CFG_API_EN != DEF_ENABLED ))
#error  "FS_CFG_API_EN                          illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif



                                                                /* ------------------- FS_CFG_DIR_EN ------------------ */
#ifndef  FS_CFG_DIR_EN
#error  "FS_CFG_DIR_EN                                not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  ((FS_CFG_DIR_EN != DEF_DISABLED) && \
        (FS_CFG_DIR_EN != DEF_ENABLED ))
#error  "FS_CFG_DIR_EN                          illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif


/*
*********************************************************************************************************
*                         FILE SYSTEM FEATURE INCLUSION CONFIGURATION ERRORS
*********************************************************************************************************
*/

                                                                /* ---------------- FS_CFG_FILE_BUF_EN ---------------- */
#ifndef  FS_CFG_FILE_BUF_EN
#error  "FS_CFG_FILE_BUF_EN                           not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be DEF_DISABLED]                          "
#error  "                                       [     || DEF_ENABLED ]                          "

#elif  ((FS_CFG_FILE_BUF_EN != DEF_ENABLED ) && \
        (FS_CFG_FILE_BUF_EN != DEF_DISABLED))
#error  "FS_CFG_FILE_BUF_EN                     illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be DEF_DISABLED]                          "
#error  "                                       [     || DEF_ENABLED ]                          "
#endif



                                                                /* ---------------- FS_CFG_FILE_LOCK_EN --------------- */
#ifndef  FS_CFG_FILE_LOCK_EN
#error  "FS_CFG_FILE_LOCK_EN                          not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be DEF_DISABLED]                          "

#elif  ((FS_CFG_FILE_LOCK_EN != DEF_ENABLED ) && \
        (FS_CFG_FILE_LOCK_EN != DEF_DISABLED))
#error  "FS_CFG_FILE_LOCK_EN                    illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be DEF_DISABLED]                          "
#error  "                                       [     || DEF_ENABLED ]                          "
#endif



                                                                /* ---------------- FS_CFG_PARTITION_EN --------------- */
#ifndef  FS_CFG_PARTITION_EN
#error  "FS_CFG_PARTITION_EN                          not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be DEF_DISABLED]                          "
#error  "                                       [     || DEF_ENABLED ]                          "

#elif  ((FS_CFG_PARTITION_EN != DEF_ENABLED ) && \
        (FS_CFG_PARTITION_EN != DEF_DISABLED))
#error  "FS_CFG_PARTITION_EN                    illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be DEF_DISABLED]                          "
#error  "                                       [     || DEF_ENABLED ]                          "
#endif



                                                                /* --------------- FS_CFG_WORKING_DIR_EN -------------- */
                                                                /* See Note #1.                                         */
#ifndef  FS_CFG_WORKING_DIR_EN
#error  "FS_CFG_WORKING_DIR_EN                        not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be DEF_DISABLED]                          "
#error  "                                       [     || DEF_ENABLED ]                          "

#elif  ((FS_CFG_WORKING_DIR_EN != DEF_ENABLED ) && \
        (FS_CFG_WORKING_DIR_EN != DEF_DISABLED))
#error  "FS_CFG_WORKING_DIR_EN                  illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be DEF_DISABLED]                          "
#error  "                                       [     || DEF_ENABLED ]                          "
#endif



                                                                /* ------------------ FS_CFG_UTF8_EN ------------------ */
#ifndef  FS_CFG_UTF8_EN
#error  "FS_CFG_UTF8_EN                               not #define'd in 'fs_cfg.h'               "
#error  "                                       [     || DEF_ENABLED ]                          "
#error  "                                       [MUST be  DEF_DISABLED]                         "

#elif  ((FS_CFG_UTF8_EN != DEF_ENABLED ) && \
        (FS_CFG_UTF8_EN != DEF_DISABLED))
#error  "FS_CFG_UTF8_EN                         illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     || DEF_ENABLED ]                          "
#endif



                                                                /* -------- FS_CFG_CONCURRENT_ENTRIES_ACCESS_EN ------- */
#ifndef  FS_CFG_CONCURRENT_ENTRIES_ACCESS_EN
#error  "FS_CFG_CONCURRENT_ENTRIES_ACCESS_EN          not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be DEF_DISABLED]                          "
#error  "                                       [     || DEF_ENABLED ]                          "

#elif  ((FS_CFG_CONCURRENT_ENTRIES_ACCESS_EN != DEF_ENABLED) && \
        (FS_CFG_CONCURRENT_ENTRIES_ACCESS_EN != DEF_DISABLED))
#error  "FS_CFG_CONCURRENT_ENTRIES_ACCESS_EN    illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be DEF_DISABLED]                          "
#error  "                                       [     || DEF_ENABLED ]                          "
#endif



                                                                /* ----------------- FS_CFG_RD_ONLY_EN ---------------- */
#ifndef  FS_CFG_RD_ONLY_EN
#error  "FS_CFG_RD_ONLY_EN                            not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be DEF_DISABLED]                          "
#error  "                                       [     || DEF_ENABLED ]                          "

#elif  ((FS_CFG_RD_ONLY_EN != DEF_ENABLED) && \
        (FS_CFG_RD_ONLY_EN != DEF_DISABLED))
#error  "FS_CFG_RD_ONLY_EN                      illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be DEF_DISABLED]                          "
#error  "                                       [     || DEF_ENABLED ]                          "
#endif


/*
*********************************************************************************************************
*                                FILE SYSTEM FAT CONFIGURATION ERRORS
*********************************************************************************************************
*/

#ifdef   FS_FAT_MODULE_PRESENT
                                                                /* ----------------- FS_FAT_CFG_LFN_EN ---------------- */
#ifndef  FS_FAT_CFG_LFN_EN
#error  "FS_FAT_CFG_LFN_EN                            not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  ((FS_FAT_CFG_LFN_EN != DEF_DISABLED) && \
        (FS_FAT_CFG_LFN_EN != DEF_ENABLED ))
#error  "FS_FAT_CFG_LFN_EN                      illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif



                                                                /* ---------------- FS_FAT_CFG_FAT12_EN --------------- */
                                                                /* See Note #1.                                         */
#ifndef  FS_FAT_CFG_FAT12_EN
#error  "FS_FAT_CFG_FAT12_EN                          not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  ((FS_FAT_CFG_FAT12_EN != DEF_DISABLED) && \
        (FS_FAT_CFG_FAT12_EN != DEF_ENABLED ))
#error  "FS_FAT_CFG_FAT12_EN                    illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif



                                                                /* ---------------- FS_FAT_CFG_FAT16_EN --------------- */
#ifndef  FS_FAT_CFG_FAT16_EN
#error  "FS_FAT_CFG_FAT16_EN                          not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  ((FS_FAT_CFG_FAT16_EN != DEF_DISABLED) && \
        (FS_FAT_CFG_FAT16_EN != DEF_ENABLED ))
#error  "FS_FAT_CFG_FAT16_EN                    illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif



                                                                /* ---------------- FS_FAT_CFG_FAT32_EN --------------- */
#ifndef  FS_FAT_CFG_FAT32_EN
#error  "FS_FAT_CFG_FAT32_EN                          not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  ((FS_FAT_CFG_FAT32_EN != DEF_DISABLED) && \
        (FS_FAT_CFG_FAT32_EN != DEF_ENABLED ))
#error  "FS_FAT_CFG_FAT32_EN                    illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif

                                                                /* Make sure at least one FAT Config. is enabled        */
#if    ((FS_FAT_CFG_FAT12_EN != DEF_ENABLED) && \
        (FS_FAT_CFG_FAT16_EN != DEF_ENABLED) && \
        (FS_FAT_CFG_FAT32_EN != DEF_ENABLED))
#error  "INVALID FS FAT CONFIG                                      in  'fs_cfg.h'              "
#error  "At least one of (FS_FAT_CFG_FAT12_EN, FS_FAT_CFG_FAT16_EN, FS_FAT_CFG_FAT32_EN)        "
#error  "                                       [MUST be  DEF_ENABLED ]                         "
#endif


                                                                /* --------------- FS_FAT_CFG_JOURNAL_EN -------------- */
#ifndef  FS_FAT_CFG_JOURNAL_EN
#error  "FS_FAT_CFG_JOURNAL_EN                        not #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  ((FS_FAT_CFG_JOURNAL_EN != DEF_DISABLED) && \
        (FS_FAT_CFG_JOURNAL_EN != DEF_ENABLED ))
#error  "FS_FAT_CFG_JOURNAL_EN                  illegally #define'd in 'fs_cfg.h'               "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif

#if    ((FS_FAT_CFG_JOURNAL_EN == DEF_ENABLED) && \
        (FS_CFG_RD_ONLY_EN     == DEF_ENABLED))
#error  "INVALID FS FAT CONFIG                                      in  'fs_cfg.h'              "
#error  "Journaling is useless when FS_CFG_RD_ONLY_EN is DEF_ENABLED. FS_FAT_CFG_JOURNAL_EN     "
#error  "                                       [MUST be  DEF_DISABLED]                         "
#endif

#endif
/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif
