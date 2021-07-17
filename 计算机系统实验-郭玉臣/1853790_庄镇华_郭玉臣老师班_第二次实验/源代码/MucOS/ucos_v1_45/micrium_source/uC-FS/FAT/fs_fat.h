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
*                                     FILE SYSTEM FAT MANAGEMENT
*
* Filename : fs_fat.h
* Version  : V4.07.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE GUARD
*********************************************************************************************************
*/

#ifndef  FS_FAT_H
#define  FS_FAT_H


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "../Source/fs_cfg_fs.h"
#include  "../Source/fs_ctr.h"
#include  "../Source/fs_type.h"


/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) The following FAT-module-present configuration value MUST be pre-#define'd in
*               'fs_cfg_fs.h' PRIOR to all other file system modules that require FAT Layer
*               Configuration (see 'fs_cfg_fs.h  FAT LAYER CONFIGURATION  Note #2b') :
*
*                   FS_FAT_MODULE_PRESENT
*********************************************************************************************************
*/

#ifdef   FS_FAT_MODULE_PRESENT                                  /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_FAT_MODULE
#define  FS_FAT_EXT
#else
#define  FS_FAT_EXT  extern
#endif


/*
*********************************************************************************************************
*                                          INCLUDE FILES
*********************************************************************************************************
*/

#include  <Source/clk.h>
#include  <cpu.h>
#include  "../Source/fs_err.h"
#include  "../Source/fs_util.h"
#include  "fs_fat_type.h"


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  FS_FAT_PATH_SEP_CHAR                    FS_CHAR_PATH_SEP

#define  FS_FAT_SIZE_DIR_ENTRY                            32u

#ifdef   FS_FAT_LFN_MODULE_PRESENT
#define  FS_FAT_MAX_PATH_NAME_LEN                        256u
#define  FS_FAT_MAX_FILE_NAME_LEN                        255u
#define  FS_FAT_FN_API_Active                     FS_FAT_LFN_API

#else
#define  FS_FAT_MAX_PATH_NAME_LEN                        256u
#define  FS_FAT_MAX_FILE_NAME_LEN                         12u
#define  FS_FAT_FN_API_Active                     FS_FAT_SFN_API
#endif


#define  FS_FAT_SFN_NAME_MAX_NBR_CHAR                      8u
#define  FS_FAT_SFN_EXT_MAX_NBR_CHAR                       3u
#define  FS_FAT_SFN_MAX_STEM_LEN                           6u

#define  FS_FAT_MODE_NONE                        FS_FILE_ACCESS_MODE_NONE
#define  FS_FAT_MODE_RD                          FS_FILE_ACCESS_MODE_RD
#define  FS_FAT_MODE_WR                          FS_FILE_ACCESS_MODE_WR
#define  FS_FAT_MODE_CREATE                      FS_FILE_ACCESS_MODE_CREATE
#define  FS_FAT_MODE_TRUNCATE                    FS_FILE_ACCESS_MODE_TRUNCATE
#define  FS_FAT_MODE_APPEND                      FS_FILE_ACCESS_MODE_APPEND
#define  FS_FAT_MODE_MUST_CREATE                 FS_FILE_ACCESS_MODE_EXCL
#define  FS_FAT_MODE_CACHED                      FS_FILE_ACCESS_MODE_CACHED
#define  FS_FAT_MODE_DEL                         DEF_BIT_07
#define  FS_FAT_MODE_DIR                         DEF_BIT_08
#define  FS_FAT_MODE_FILE                        DEF_BIT_09

#define  FS_FAT_MIN_CLUS_NBR                               2u

#define  FS_FAT_FAT16_ENTRY_NBR_OCTETS                     2u
#define  FS_FAT_FAT32_ENTRY_NBR_OCTETS                     4u

#define  FS_FAT_VOL_LABEL_LEN                             11u

/*
*********************************************************************************************************
*                                      BOOT SECTOR & BPB DEFINES
*********************************************************************************************************
*/

#define  FS_FAT_BS_OFF_JMPBOOT                             0u
#define  FS_FAT_BS_OFF_OEMNAME                             3u
#define  FS_FAT_BPB_OFF_BYTSPERSEC                        11u
#define  FS_FAT_BPB_OFF_SECPERCLUS                        13u
#define  FS_FAT_BPB_OFF_RSVDSECCNT                        14u
#define  FS_FAT_BPB_OFF_NUMFATS                           16u
#define  FS_FAT_BPB_OFF_ROOTENTCNT                        17u
#define  FS_FAT_BPB_OFF_TOTSEC16                          19u
#define  FS_FAT_BPB_OFF_MEDIA                             21u
#define  FS_FAT_BPB_OFF_FATSZ16                           22u
#define  FS_FAT_BPB_OFF_SECPERTRK                         24u
#define  FS_FAT_BPB_OFF_NUMHEADS                          26u
#define  FS_FAT_BPB_OFF_HIDDSEC                           28u
#define  FS_FAT_BPB_OFF_TOTSEC32                          32u

#define  FS_FAT_BS_FAT1216_OFF_DRVNUM                     36u
#define  FS_FAT_BS_FAT1216_OFF_RESERVED1                  37u
#define  FS_FAT_BS_FAT1216_OFF_BOOTSIG                    38u
#define  FS_FAT_BS_FAT1216_OFF_VOLID                      39u
#define  FS_FAT_BS_FAT1216_OFF_VOLLAB                     43u
#define  FS_FAT_BS_FAT1216_OFF_FILSYSTYPE                 54u

#define  FS_FAT_BPB_FAT32_OFF_FATSZ32                     36u
#define  FS_FAT_BPB_FAT32_OFF_EXTFLAGS                    40u
#define  FS_FAT_BPB_FAT32_OFF_FSVER                       42u
#define  FS_FAT_BPB_FAT32_OFF_ROOTCLUS                    44u
#define  FS_FAT_BPB_FAT32_OFF_FSINFO                      48u
#define  FS_FAT_BPB_FAT32_OFF_BKBOOTSEC                   50u
#define  FS_FAT_BPB_FAT32_OFF_RESERVED                    52u
#define  FS_FAT_BS_FAT32_OFF_DRVNUM                       64u
#define  FS_FAT_BS_FAT32_OFF_RESERVED1                    65u
#define  FS_FAT_BS_FAT32_OFF_BOOTSIG                      66u
#define  FS_FAT_BS_FAT32_OFF_VOLID                        67u
#define  FS_FAT_BS_FAT32_OFF_VOLLAB                       71u
#define  FS_FAT_BS_FAT32_OFF_FILSYSTYPE                   82u

#define  FS_FAT_BS_FAT12_FILESYSTYPE            "FAT12   "
#define  FS_FAT_BS_FAT16_FILESYSTYPE            "FAT16   "
#define  FS_FAT_BS_FAT32_FILESYSTYPE            "FAT32   "

#define  FS_FAT_BS_JMPBOOT_0                            0xEBu
#define  FS_FAT_BS_JMPBOOT_1                            0x58u
#define  FS_FAT_BS_JMPBOOT_2                            0x90u
#define  FS_FAT_BS_OEMNAME                         "MSWIN4.1"

#define  FS_FAT_BPB_MEDIA_FIXED                         0xF8u
#define  FS_FAT_BPB_MEDIA_REMOVABLE                     0xF0u

#define  FS_FAT_BS_BOOTSIG                              0x29u

#define  FS_FAT_BS_VOLLAB                       "NO NAME    "

#define  FS_FAT_BOOT_SIG                              0xAA55u
#define  FS_FAT_BOOT_SIG_LO                             0x55u
#define  FS_FAT_BOOT_SIG_HI                             0xAAu

#define  FS_FAT_BOOT_SIG_LO_OFF                          510u
#define  FS_FAT_BOOT_SIG_HI_OFF                          511u

/*
*********************************************************************************************************
*                                           DEFAULT VALUES
*********************************************************************************************************
*/

#define  FS_FAT_DFLT_RSVD_SEC_CNT_FAT12                    1u
#define  FS_FAT_DFLT_RSVD_SEC_CNT_FAT16                    1u
#define  FS_FAT_DFLT_RSVD_SEC_CNT_FAT32                   32u

#define  FS_FAT_DFLT_NBR_FATS_FAT12                        2u
#define  FS_FAT_DFLT_NBR_FATS_FAT16                        2u
#define  FS_FAT_DFLT_NBR_FATS_FAT32                        2u

#define  FS_FAT_DFLT_ROOT_ENT_CNT_FAT12                  512u
#define  FS_FAT_DFLT_ROOT_ENT_CNT_FAT16                  512u
#define  FS_FAT_DFLT_ROOT_ENT_CNT_FAT32                    0u

#define  FS_FAT_DFLT_ROOT_CLUS_NBR                         2u
#define  FS_FAT_DFLT_FSINFO_SEC_NBR                        1u
#define  FS_FAT_DFLT_BKBOOTSEC_SEC_NBR                     6u

/*
*********************************************************************************************************
*                                         DIRECTORY ENTRY DEFINES
*********************************************************************************************************
*/

#define  FS_FAT_DIRENT_NAME_ERASED_AND_FREE             0xE5u
#define  FS_FAT_DIRENT_NAME_FREE                        0x00u
#define  FS_FAT_DIRENT_NAME_LAST_LONG_ENTRY             0x40u
#define  FS_FAT_DIRENT_NAME_LONG_ENTRY_MASK             0x3Fu

#define  FS_FAT_DIRENT_ATTR_NONE                 DEF_BIT_NONE
#define  FS_FAT_DIRENT_ATTR_READ_ONLY            DEF_BIT_00     /* Writes to the file should fail.                      */
#define  FS_FAT_DIRENT_ATTR_HIDDEN               DEF_BIT_01     /* Normal directory listings should not show file.      */
#define  FS_FAT_DIRENT_ATTR_SYSTEM               DEF_BIT_02     /* Operating system file.                               */
#define  FS_FAT_DIRENT_ATTR_VOLUME_ID            DEF_BIT_03     /* Marks file with name corresponding to volume label.  */
#define  FS_FAT_DIRENT_ATTR_DIRECTORY            DEF_BIT_04     /* File is container for other files.                   */
#define  FS_FAT_DIRENT_ATTR_ARCHIVE              DEF_BIT_05     /* Set when file is created, renamed or written to.     */

#define  FS_FAT_DIRENT_ATTR_LONG_NAME           (FS_FAT_DIRENT_ATTR_READ_ONLY | FS_FAT_DIRENT_ATTR_HIDDEN    | \
                                                 FS_FAT_DIRENT_ATTR_SYSTEM    | FS_FAT_DIRENT_ATTR_VOLUME_ID)

#define  FS_FAT_DIRENT_ATTR_LONG_NAME_MASK      (FS_FAT_DIRENT_ATTR_READ_ONLY | FS_FAT_DIRENT_ATTR_HIDDEN    | \
                                                 FS_FAT_DIRENT_ATTR_SYSTEM    | FS_FAT_DIRENT_ATTR_VOLUME_ID | \
                                                 FS_FAT_DIRENT_ATTR_DIRECTORY | FS_FAT_DIRENT_ATTR_ARCHIVE)

#define  FS_FAT_DIRENT_ATTR_IS_LONG_NAME(attrib)     ((((attrib) & FS_FAT_DIRENT_ATTR_LONG_NAME_MASK) == FS_FAT_DIRENT_ATTR_LONG_NAME) ? DEF_YES : DEF_NO)

#define  FS_FAT_DIRENT_NTRES_NAME_LOWER_CASE     DEF_BIT_03
#define  FS_FAT_DIRENT_NTRES_EXT_LOWER_CASE      DEF_BIT_04

#define  FS_FAT_DIRENT_OFF_NAME                            0u
#define  FS_FAT_DIRENT_OFF_ATTR                           11u
#define  FS_FAT_DIRENT_OFF_NTRES                          12u
#define  FS_FAT_DIRENT_OFF_CRTTIMETENTH                   13u
#define  FS_FAT_DIRENT_OFF_CRTTIME                        14u
#define  FS_FAT_DIRENT_OFF_CRTDATE                        16u
#define  FS_FAT_DIRENT_OFF_LSTACCDATE                     18u
#define  FS_FAT_DIRENT_OFF_FSTCLUSHI                      20u
#define  FS_FAT_DIRENT_OFF_WRTTIME                        22u
#define  FS_FAT_DIRENT_OFF_WRTDATE                        24u
#define  FS_FAT_DIRENT_OFF_FSTCLUSLO                      26u
#define  FS_FAT_DIRENT_OFF_FILESIZE                       28u

/*
*********************************************************************************************************
*                                            JOURNAL DEFINES
*********************************************************************************************************
*/

#define  FS_FAT_JOURNAL_STATE_OPEN               DEF_BIT_01
#define  FS_FAT_JOURNAL_STATE_START              DEF_BIT_02
#define  FS_FAT_JOURNAL_STATE_REPLAY             DEF_BIT_03

/*
*********************************************************************************************************
*                                          FAT FAT TYPE DEFINES
*********************************************************************************************************
*/

#define  FS_FAT_FAT_TYPE_UNKNOWN                           0u
#define  FS_FAT_FAT_TYPE_FAT12                            12u
#define  FS_FAT_FAT_TYPE_FAT16                            16u
#define  FS_FAT_FAT_TYPE_FAT32                            32u

/*
*********************************************************************************************************
*                                            FAT TYPE DEFINES
*
* Note(s) : (1) FS_FAT_TYPE_??? #define values specifically chosen as ASCII representations of the FAT
*               types.  Memory displays of FAT types will display the FAT TYPE with the chosen ASCII
*               name.
*********************************************************************************************************
*/

#define  FS_FAT_TYPE_FAT_NONE       FS_TYPE_CREATE(ASCII_CHAR_LATIN_UPPER_N,  \
                                                   ASCII_CHAR_LATIN_UPPER_O,  \
                                                   ASCII_CHAR_LATIN_UPPER_N,  \
                                                   ASCII_CHAR_LATIN_UPPER_E)

#define  FS_FAT_TYPE_FAT_INFO       FS_TYPE_CREATE(ASCII_CHAR_LATIN_UPPER_F,  \
                                                   ASCII_CHAR_LATIN_UPPER_A,  \
                                                   ASCII_CHAR_LATIN_UPPER_T,  \
                                                   ASCII_CHAR_SPACE)

#define  FS_FAT_TYPE_FAT_FILE_INFO  FS_TYPE_CREATE(ASCII_CHAR_LATIN_UPPER_F,  \
                                                   ASCII_CHAR_LATIN_UPPER_A,  \
                                                   ASCII_CHAR_LATIN_UPPER_F,  \
                                                   ASCII_CHAR_LATIN_UPPER_I)


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       FAT FILE DATA DATA TYPE
*********************************************************************************************************
*/

struct  fs_fat_file_data {
    FS_FAT_FILE_SIZE          FilePos;                          /* Current file pos.                                    */
    FS_FAT_FILE_SIZE          FileSize;                         /* Nbr octets in file.                                  */
    CPU_BOOLEAN               UpdateReqd;                       /* Dir sec update req'd.                                */
    FS_FLAGS                  Mode;                             /* Access mode.                                         */

    FS_FAT_SEC_NBR            DirFirstSec;                      /* First sec nbr of file's parent dir.                  */
    FS_FAT_SEC_NBR            DirStartSec;                      /* Sec  nbr of file's first dir entry.                  */
    FS_SEC_SIZE               DirStartSecPos;                   /* Pos of      file's first dir entry in sec.           */
    FS_FAT_SEC_NBR            DirEndSec;                        /* Sec  nbr of file's last  dir entry.                  */
    FS_SEC_SIZE               DirEndSecPos;                     /* Pos of      file's last  dir entry in sec.           */

    FS_FAT_CLUS_NBR           FileFirstClus;                    /* Clus nbr of first file clus.                         */
    FS_FAT_SEC_NBR            FileCurSec;                       /* Sec  nbr of cur   file sec.                          */
    FS_SEC_SIZE               FileCurSecPos;                    /* Pos      of cur   file pos in sec.                   */

    FS_FLAGS                  Attrib;                           /* File attrib.                                         */
    FS_FAT_DATE               DateCreate;                       /* File creation date.                                  */
    FS_FAT_TIME               TimeCreate;                       /* File creation time.                                  */
    FS_FAT_DATE               DateAccess;                       /* File last access date.                               */
    FS_FAT_DATE               DateWr;                           /* File last wr  date.                                  */
    FS_FAT_TIME               TimeWr;                           /* File last wr  time.                                  */
};


/*
*********************************************************************************************************
*                                         FAT INFO DATA TYPE
*********************************************************************************************************
*/

struct  fs_fat_data {
    FS_FAT_SEC_NBR            RsvdSize;                         /* Nbr of sec  in rsvd area.                            */
    FS_FAT_SEC_NBR            FAT_Size;                         /* Nbr of sec  in each FAT.                             */
    FS_FAT_SEC_NBR            RootDirSize;                      /* Nbr of sec  occupied by the root dir.                */
    FS_FAT_SEC_NBR            VolSize;                          /* Nbr of sec  in vol.                                  */
    FS_FAT_SEC_NBR            DataSize;                         /* Nbr of sec  in data section of vol.                  */
    FS_FAT_CLUS_NBR           MaxClusNbr;                       /* Max clus nbr.                                        */

    FS_FAT_SEC_NBR            FS_InfoStart;                     /* Sec nbr of FSINFO.                                   */
    FS_FAT_SEC_NBR            BPB_BkStart;                      /* Sec nbr of backup BPB.                               */
    FS_FAT_SEC_NBR            FAT1_Start;                       /* Sec nbr of first sec of 1st FAT.                     */
    FS_FAT_SEC_NBR            FAT2_Start;                       /* Sec nbr of first sec of 2nd FAT.                     */
    FS_FAT_SEC_NBR            RootDirStart;                     /* Sec nbr of first sec of root dir.                    */
    FS_FAT_SEC_NBR            DataStart;                        /* Sec nbr of first data sec.                           */

    FS_FAT_CLUS_NBR           NextClusNbr;                      /* Clus nbr of next clus to alloc.                      */

    FS_SEC_SIZE               SecSize;                          /* Sector  size (in octets).                            */
    CPU_INT08U                SecSizeLog2;                      /* Sector  size  base-2 log.                            */
    FS_FAT_SEC_NBR            ClusSize_sec;                     /* Cluster size (in sectors).                           */
    CPU_INT08U                ClusSizeLog2_sec;                 /* Cluster size  base-2 log (in sectors).               */
    FS_SEC_SIZE               ClusSize_octet;                   /* Cluster size (in octets).                            */
    CPU_INT08U                ClusSizeLog2_octet;               /* Cluster size  base-2 log (in octets).                */

    CPU_INT08U                NbrFATs;                          /* Number of FATs.                                      */
    CPU_INT08U                FAT_Type;                         /* FAT type (12, 16 or 32).                             */
    const  FS_FAT_TYPE_API   *FAT_TypeAPI_Ptr;

    CPU_BOOLEAN               QueryInfoValid;                   /* Whether 'QueryClusBadCnt' & 'QueryClusFreeCnt' valid.*/
    FS_FAT_CLUS_NBR           QueryBadClusCnt;                  /* Count of bad  clusters.                              */
    FS_FAT_CLUS_NBR           QueryFreeClusCnt;                 /* Count of free clusters.                              */

#ifdef  FS_FAT_JOURNAL_MODULE_PRESENT
    CPU_INT08U                JournalState;
    FS_FAT_FILE_DATA         *JournalDataPtr;
#endif

#if (FS_CFG_CTR_STAT_EN == DEF_ENABLED)
    FS_CTR                    StatAllocClusCtr;                 /* Number of cluster allocations.                       */
    FS_CTR                    StatFreeClusCtr;                  /* Number of cluster frees.                             */
#endif
};


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
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

                                                                /* ------------------ CLK_CFG_EXT_EN ------------------ */
#ifdef  FS_CFG_GET_TS_FROM_OS
#error  "FS_CFG_GET_TS_FROM_OS should not be #define'd in 'fs_cfg.h' -- replaced by CLK_CFG_EXT_EN in 'clk_cfg.h'   "
#endif


#ifndef  CLK_CFG_STR_CONV_EN
#error  "CLK_CFG_STR_CONV_EN                          not #define'd in 'clk_cfg.h'              "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  (CLK_CFG_STR_CONV_EN != DEF_ENABLED )
#error  "CLK_CFG_STR_CONV_EN                    illegally #define'd in 'clk_cfg.h'              "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif


#ifndef  CLK_CFG_UNIX_EN
#error  "CLK_CFG_UNIX_EN                              not #define'd in 'clk_cfg.h'              "
#error  "                                       [     ||  DEF_ENABLED ]                         "

#elif  (CLK_CFG_UNIX_EN != DEF_ENABLED )
#error  "CLK_CFG_UNIX_EN                        illegally #define'd in 'clk_cfg.h'              "
#error  "                                       [     ||  DEF_ENABLED ]                         "
#endif


/*
*********************************************************************************************************
*                                     FS_FAT_IS_LEGAL_xxx_CHAR()
*
* Description : Determine whether character is ...
*
*               (a) ... legal Short File Name character ('FS_FAT_IS_LEGAL_SFN_CHAR()') (see Note #1).
*
*               (b) ... legal Long  File Name character ('FS_FAT_IS_LEGAL_LFN_CHAR()') (see Note #2).
*
* Argument(s) : c           Character.
*
* Return(s)   : DEF_YES, if character is   legal.
*
*               DEF_NO,  if character is illegal.
*
* Note(s) : (1) (a) According to Microsoft's "FAT: General Overview of On-DiSk Format", the legal characters
*                   for a short name are :
*
*                   (1) Letters, digits & characters with code values greater than 127.
*                   (2) The special characters
*
*                       !  #  $  %  &  '  (  )  -  @  ^  _  `  {  }  ~
*
*                   Taken together, this means that the set of legal character codes is the union of the
*                   following character code ranges :
*
*                   (1) [0x80, 0xFF]    characters with code values above 127
*                   (2) [0x7D, 0x7E]    }  ~
*                   (3) [0x5E, 0x7B]    ^  _  `  lower-case letters  {
*                   (4) [0x40, 0x5A]    @  upper-case letters
*                   (5) [0x30, 0x39]    digits
*                   (6) [0x2D, 0x2D]    -
*                   (7) [0x23, 0x29]    #  $  %  &  '  (  )
*                   (8) [0x21, 0x21]    !
*
*               (b) The legal characters for a long name are :
*
*                   (1) The legal SFN characters.
*                   (2) Periods.
*                   (3) Spaces.
*                   (4) The special characters
*
*                       +  ,  ;  =  [  ]
*
*                   Taken together, this means that the set of legal character codes is the union of the
*                   following character code ranges :
*
*                   (1) [0x80, 0xFF]    characters with code values above 127
*                   (2) [0x7D, 0x7E]    }  ~
*                   (3) [0x5D, 0x7B]    ]  ^  _  `  lower-case letters  {
*                   (4) [0x40, 0x5B]    @  upper-case letters  [
*                   (5) [0x3D, 0x3D]    =
*                   (6) [0x3B, 0x3B]    ;
*                   (7) [0x30, 0x39]    digits
*                   (8) [0x2B, 0x2E]    +  ,  -  .
*                   (9) [0x23, 0x29]    #  $  %  &  '  (  )
*                  (10) [0x20, 0x21]    space  !
*********************************************************************************************************
*/

#define  FS_FAT_IS_LEGAL_SFN_CHAR(c)           (((((c) >= (CPU_CHAR)ASCII_CHAR_CIRCUMFLEX_ACCENT)  && \
                                                  ((c) != (CPU_CHAR)ASCII_CHAR_VERTICAL_LINE    )  && \
                                                  ((c) != (CPU_CHAR)ASCII_CHAR_DELETE           )) || \
                                                 (((c) >= (CPU_CHAR)ASCII_CHAR_COMMERCIAL_AT    )  && \
                                                  ((c) <= (CPU_CHAR)ASCII_CHAR_LATIN_UPPER_Z    )) || \
                                                 (((c) >= (CPU_CHAR)ASCII_CHAR_DIGIT_ZERO       )  && \
                                                  ((c) <= (CPU_CHAR)ASCII_CHAR_DIGIT_NINE       )) || \
                                                 (((c) == (CPU_CHAR)ASCII_CHAR_HYPHEN_MINUS     )) || \
                                                 (((c) >= (CPU_CHAR)ASCII_CHAR_EXCLAMATION_MARK )  && \
                                                  ((c) <= (CPU_CHAR)ASCII_CHAR_RIGHT_PARENTHESIS)  && \
                                                  ((c) != (CPU_CHAR)ASCII_CHAR_QUOTATION_MARK   ))) ? (DEF_YES) : (DEF_NO))


#define  FS_FAT_IS_LEGAL_LFN_CHAR(c)           (((((c) >= (CPU_CHAR)ASCII_CHAR_COMMERCIAL_AT    )  && \
                                                  ((c) != (CPU_CHAR)ASCII_CHAR_REVERSE_SOLIDUS  )  && \
                                                  ((c) != (CPU_CHAR)ASCII_CHAR_VERTICAL_LINE    )  && \
                                                  ((c) != (CPU_CHAR)ASCII_CHAR_DELETE           )) || \
                                                  ((c) == (CPU_CHAR)ASCII_CHAR_EQUALS_SIGN      )  || \
                                                  ((c) == (CPU_CHAR)ASCII_CHAR_SEMICOLON        )  || \
                                                 (((c) >= (CPU_CHAR)ASCII_CHAR_DIGIT_ZERO       )  && \
                                                 ((c) <= (CPU_CHAR)ASCII_CHAR_DIGIT_NINE        )) || \
                                                 (((c) >= (CPU_CHAR)ASCII_CHAR_SPACE            )  && \
                                                 ((c) <= (CPU_CHAR)ASCII_CHAR_FULL_STOP         )  && \
                                                 ((c) != (CPU_CHAR)ASCII_CHAR_QUOTATION_MARK    )  && \
                                                 ((c) != (CPU_CHAR)ASCII_CHAR_ASTERISK)         ))  ? (DEF_YES) : (DEF_NO))


#define  FS_FAT_IS_LEGAL_VOL_LABEL_CHAR(c)     ((((c) != ASCII_CHAR_QUOTATION_MARK              )  && \
                                                 ((c) != ASCII_CHAR_AMPERSAND                   )  && \
                                                 ((c) != ASCII_CHAR_ASTERISK                    )  && \
                                                 ((c) != ASCII_CHAR_PLUS_SIGN                   )  && \
                                                 ((c) != ASCII_CHAR_HYPHEN_MINUS                )  && \
                                                 ((c) != ASCII_CHAR_COMMA                       )  && \
                                                 ((c) != ASCII_CHAR_FULL_STOP                   )  && \
                                                 ((c) != ASCII_CHAR_SOLIDUS                     )  && \
                                                 ((c) != ASCII_CHAR_COLON                       )  && \
                                                 ((c) != ASCII_CHAR_SEMICOLON                   )  && \
                                                 ((c) != ASCII_CHAR_LESS_THAN_SIGN              )  && \
                                                 ((c) != ASCII_CHAR_EQUALS_SIGN                 )  && \
                                                 ((c) != ASCII_CHAR_GREATER_THAN_SIGN           )  && \
                                                 ((c) != ASCII_CHAR_QUESTION_MARK               )  && \
                                                 ((c) != ASCII_CHAR_LEFT_SQUARE_BRACKET         )  && \
                                                 ((c) != ASCII_CHAR_RIGHT_SQUARE_BRACKET        )  && \
                                                 ((c) != ASCII_CHAR_REVERSE_SOLIDUS             ))  ? (DEF_YES) : (DEF_NO))


/*
*********************************************************************************************************
*                                        FS_FAT_SEC_TO_CLUS()
*
* Description : Get cluster in which sector lies.
*
* Argument(s) : p_fat_data  Pointer to FAT info.
*
*               sec_nbr     Sector number.
*
* Return(s)   : Cluster number.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#define  FS_FAT_SEC_TO_CLUS(p_fat_data, sec_nbr)     ((FS_FAT_CLUS_NBR)FS_UTIL_DIV_PWR2(((sec_nbr) - (p_fat_data)->DataStart), (p_fat_data)->ClusSizeLog2_sec) + 2u)

/*
*********************************************************************************************************
*                                        FS_FAT_SEC_TO_CLUS()
*
* Description : Get first sector of cluster.
*
* Argument(s) : p_fat_data  Pointer to FAT info.
*
*               clus_nbr    Cluster number.
*
* Return(s)   : Sector number.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#define  FS_FAT_CLUS_TO_SEC(p_fat_data, clus_nbr)      ((p_fat_data)->DataStart + (FS_FAT_SEC_NBR)FS_UTIL_MULT_PWR2(((clus_nbr) - FS_FAT_MIN_CLUS_NBR), (p_fat_data)->ClusSizeLog2_sec))

/*
*********************************************************************************************************
*                                        FS_FAT_CLUS_SEC_REM()
*
* Description : Get number of sectors remaining in cluster after a certain sector.
*
* Argument(s) : p_fat_data  Pointer to FAT info.
*
*               sec_nbr     Sector number.
*
* Return(s)   : Number of sectors remaining in cluster.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#define  FS_FAT_CLUS_SEC_REM(p_fat_data, sec_nbr)      ((FS_FAT_SEC_NBR)(p_fat_data)->ClusSize_sec - (((sec_nbr) - (p_fat_data)->DataStart) & ((FS_FAT_SEC_NBR)(p_fat_data)->ClusSize_sec - 1u)))

/*
*********************************************************************************************************
*                                       FS_FAT_IS_VALID_CLUS()
*
* Description : Determine whether cluster number specifies a valid cluster.
*
* Argument(s) : p_fat_data  Pointer to FAT info.
*
*               clus_nbr    Cluster number.
*
* Return(s)   : DEF_YES, if cluster number specifies a valid cluster.
*
*               DEF_NO,  otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#define  FS_FAT_IS_VALID_CLUS(p_fat_data, clus_nbr)  ((((clus_nbr) <  (p_fat_data)->MaxClusNbr) && \
                                                       ((clus_nbr) >=  FS_FAT_MIN_CLUS_NBR))       ? (DEF_YES) : (DEF_NO))

/*
*********************************************************************************************************
*                                        FS_FAT_IS_VALID_SEC()
*
* Description : Determine whether sector number specifies a valid sector.
*
* Argument(s) : p_fat_data  Pointer to FAT info.
*
*               sec_nbr     Sector number.
*
* Return(s)   : DEF_YES, if sector number specifies a valid sector.
*
*               DEF_NO,  otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#define  FS_FAT_IS_VALID_SEC(p_fat_data, sec_nbr)    ((((sec_nbr) >= (p_fat_data)->RootDirStart) && \
                                                       ((sec_nbr) <= (p_fat_data)->RootDirStart + (p_fat_data)->RootDirSize + (p_fat_data)->DataSize)) ? (DEF_YES) : (DEF_NO))


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#if (FS_FAT_CFG_VOL_CHK_EN == DEF_ENABLED)
void             FS_FAT_VolChk                 (CPU_CHAR          *name_vol,    /* Check file system on volume.         */
                                                FS_ERR            *p_err);
#endif

/*
*********************************************************************************************************
*                                  SYSTEM DRIVER FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void             FS_FAT_ModuleInit             (FS_QTY             vol_cnt,     /* Initialize FAT system driver.        */
                                                FS_QTY             file_cnt,
                                                FS_QTY             dir_cnt,
                                                FS_ERR            *p_err);

void             FS_FAT_VolClose               (FS_VOL            *p_vol);      /* Close a volume.                      */

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void             FS_FAT_VolFmt                 (FS_VOL            *p_vol,       /* Create a volume.                     */
                                                void              *p_sys_cfg,
                                                FS_SEC_SIZE        sec_size,
                                                FS_SEC_QTY         size,
                                                FS_ERR            *p_err);
#endif

void             FS_FAT_VolLabelGet            (FS_VOL            *p_vol,       /* Get volume label.                    */
                                                CPU_CHAR          *label,
                                                CPU_SIZE_T         len_max,
                                                FS_ERR            *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void             FS_FAT_VolLabelSet            (FS_VOL            *p_vol,       /* Set volume label.                    */
                                                CPU_CHAR          *label,
                                                FS_ERR            *p_err);
#endif

void             FS_FAT_VolOpen                (FS_VOL            *p_vol,       /* Open a volume.                       */
                                                FS_ERR            *p_err);

void             FS_FAT_VolQuery               (FS_VOL            *p_vol,       /* Get info about a volume.             */
                                                FS_SYS_INFO       *p_info,
                                                FS_ERR            *p_err);

/*
*********************************************************************************************************
*                                     UTILITY FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
FS_FAT_DATE      FS_FAT_DateFmt                (CLK_DATE_TIME     *p_time);     /* Fmt date for FAT.                    */

FS_FAT_TIME      FS_FAT_TimeFmt                (CLK_DATE_TIME     *p_time);     /* Fmt time for FAT.                    */
#endif

void             FS_FAT_DateTimeParse          (CLK_TS_SEC        *p_ts,        /* Parse FAT date, time.                */
                                                FS_FAT_DATE        date,
                                                FS_FAT_TIME        time);


#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void             FS_FAT_SecClr                 (FS_VOL            *p_vol,       /* Clr secs on vol.                     */
                                                void              *p_temp,
                                                FS_FAT_SEC_NBR     start,
                                                FS_FAT_SEC_NBR     cnt,
                                                FS_SEC_SIZE        sec_size,
                                                FS_FLAGS           sec_type,
                                                FS_ERR            *p_err);

CPU_CHAR        *FS_FAT_Char_LastPathSep       (CPU_CHAR          *pstr);       /* Find last path sep char.             */
#endif

/*
*********************************************************************************************************
*                                   FAT ACCESS FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void             FS_FAT_LowEntryUpdate         (FS_VOL            *p_vol,       /* Update dir entry.                    */
                                                FS_BUF            *p_buf,
                                                FS_FAT_FILE_DATA  *p_entry_data,
                                                CPU_BOOLEAN        get_date,
                                                FS_ERR            *p_err);
#endif

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void             FS_FAT_LowEntryCreate         (FS_VOL            *p_vol,       /* Create new entry (low-level).        */
                                                FS_BUF            *p_buf,
                                                FS_FAT_FILE_DATA  *p_entry_data,
                                                CPU_CHAR          *name_entry,
                                                FS_FAT_SEC_NBR     dir_first_sec,
                                                CPU_BOOLEAN        is_dir,
                                                FS_ERR            *p_err);

void             FS_FAT_LowEntryDel            (FS_VOL            *p_vol,       /* Del entry (low-level).               */
                                                FS_BUF            *p_buf,
                                                FS_FAT_FILE_DATA  *p_entry_data,
                                                FS_ERR            *p_err);
#endif

void             FS_FAT_LowEntryFind           (FS_VOL            *p_vol,       /* Find entry (low-level).              */
                                                FS_FAT_FILE_DATA  *p_entry_data,
                                                CPU_CHAR          *name_entry,
                                                FS_FLAGS           mode,
                                                FS_ERR            *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
void             FS_FAT_LowEntryRename         (FS_VOL            *p_vol,       /* Rename entry (low-level).            */
                                                FS_BUF            *p_buf,
                                                FS_FAT_FILE_DATA  *p_entry_data_old,
                                                FS_FAT_FILE_DATA  *p_entry_data_new,
                                                CPU_BOOLEAN        exists,
                                                CPU_CHAR          *name_entry_new,
                                                FS_ERR            *p_err);

void             FS_FAT_LowEntryTruncate       (FS_VOL            *p_vol,       /* Truncate entry (low-level).          */
                                                FS_BUF            *p_buf,
                                                FS_FAT_FILE_DATA  *p_entry_data,
                                                FS_FAT_FILE_SIZE   file_size_truncated,
                                                FS_ERR            *p_err);

CPU_BOOLEAN      FS_FAT_LowDirChkEmpty         (FS_VOL            *p_vol,       /* Chk whether dir is empty.            */
                                                FS_BUF            *p_buf,
                                                FS_FAT_CLUS_NBR    dir_clus,
                                                FS_ERR            *p_err);

void             FS_FAT_LowFileFirstClusAdd    (FS_VOL            *p_vol,       /* Add 1st clus to file.                */
                                                FS_FAT_FILE_DATA  *p_entry_data,
                                                FS_BUF            *p_buf,
                                                FS_ERR            *p_err);

#endif

FS_FAT_CLUS_NBR  FS_FAT_LowFileFirstClusGet    (FS_VOL            *p_vol,       /* Get 1st clus address of file.        */
                                                CPU_CHAR          *name_entry,
                                                FS_ERR            *p_err);


/*
*********************************************************************************************************
*                                 FAT CLUSTER ACCESS FUNCTION PROTOTYPES
*********************************************************************************************************
*/


#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
FS_FAT_CLUS_NBR  FS_FAT_ClusChainAlloc         (FS_VOL            *p_vol,       /* Alloc clus chain.                        */
                                                FS_BUF            *p_buf,
                                                FS_FAT_CLUS_NBR    start_clus,
                                                FS_FAT_CLUS_NBR    nbr_clus,
                                                FS_ERR            *p_err);

FS_FAT_CLUS_NBR  FS_FAT_ClusChainDel           (FS_VOL            *p_vol,       /* Reverse delete cluster chain.        */
                                                FS_BUF            *p_buf,
                                                FS_FAT_CLUS_NBR    start_clus,
                                                CPU_BOOLEAN        del_first,
                                                FS_ERR            *p_err);

void             FS_FAT_ClusChainReverseDel    (FS_VOL            *p_vol,       /* Reverse delete cluster chain.        */
                                                FS_BUF            *p_buf,
                                                FS_FAT_CLUS_NBR    start_clus,
                                                CPU_BOOLEAN        del_first,
                                                FS_ERR            *p_err);
#endif

FS_FAT_CLUS_NBR  FS_FAT_ClusChainFollow        (FS_VOL            *p_vol,       /* Follow cluster chain.                */
                                                FS_BUF            *p_buf,
                                                FS_FAT_CLUS_NBR    start_clus,
                                                FS_FAT_CLUS_NBR    len,
                                                FS_FAT_CLUS_NBR   *p_clus_cnt,
                                                FS_ERR            *p_err);

FS_FAT_CLUS_NBR  FS_FAT_ClusChainEndFind       (FS_VOL            *p_vol,       /* Find cluster chain end cluster.      */
                                                FS_BUF            *p_buf,
                                                FS_FAT_CLUS_NBR    start_clus,
                                                FS_FAT_CLUS_NBR   *p_clus_cnt,
                                                FS_ERR            *p_err);

FS_FAT_CLUS_NBR  FS_FAT_ClusChainReverseFollow (FS_VOL            *p_vol,       /* Reverse follow cluster chain.        */
                                                FS_BUF            *p_buf,
                                                FS_FAT_CLUS_NBR    start_clus,
                                                FS_FAT_CLUS_NBR    stop_clus,
                                                FS_ERR            *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
FS_FAT_CLUS_NBR  FS_FAT_ClusFreeFind           (FS_VOL            *p_vol,       /* Find free cluster.                   */
                                                FS_BUF            *p_buf,
                                                FS_ERR            *p_err);
#endif

FS_FAT_CLUS_NBR  FS_FAT_ClusNextGet            (FS_VOL            *p_vol,       /* Get next cluster in chain.           */
                                                FS_BUF            *p_buf,
                                                FS_FAT_CLUS_NBR    start_clus,
                                                FS_ERR            *p_err);

FS_FAT_SEC_NBR   FS_FAT_SecNextGet             (FS_VOL            *p_vol,       /* Get next sector in cluster chain.    */
                                                FS_BUF            *p_buf,
                                                FS_FAT_SEC_NBR     start_sec,
                                                FS_ERR            *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
FS_FAT_SEC_NBR   FS_FAT_SecNextGetAlloc        (FS_VOL            *p_vol,       /* Get next sector OR allocate new.     */
                                                FS_BUF            *p_buf,
                                                FS_FAT_SEC_NBR     start_sec,
                                                CPU_BOOLEAN        clr,
                                                FS_ERR            *p_err);

void             FS_FAT_MakeBootSec            (void              *p_temp,      /* Make boot sector.                    */
                                                FS_FAT_SYS_CFG    *p_sys_cfg,
                                                FS_SEC_SIZE        sec_size,
                                                FS_FAT_SEC_NBR     size,
                                                FS_FAT_SEC_NBR     fat_size,
                                                FS_SEC_NBR         partition_start);
#endif

void             FS_FAT_Query                  (FS_VOL            *p_vol,       /* Get info about file system.          */
                                                FS_BUF            *p_buf,
                                                FS_SYS_INFO       *p_info,
                                                FS_ERR            *p_err);


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

#ifndef  FS_CFG_MAX_PATH_NAME_LEN
#error  "FS_CFG_MAX_PATH_NAME_LEN             not #define'd in 'fs_cfg.h'    "
#error  "                               [MUST be  > FS_FAT_MAX_FILE_NAME_LEN]"

#elif   (FS_CFG_MAX_PATH_NAME_LEN < FS_FAT_MAX_FILE_NAME_LEN)
#error  "FS_CFG_MAX_PATH_NAME_LEN       illegally #define'd in 'fs_cfg.h'    "
#error  "                               [MUST be  > FS_FAT_MAX_FILE_NAME_LEN]"
#endif



#ifndef  FS_CFG_MAX_FILE_NAME_LEN
#error  "FS_CFG_MAX_FILE_NAME_LEN             not #define'd in 'fs_cfg.h'    "
#error  "                               [MUST be  > FS_FAT_MAX_FILE_NAME_LEN]"

#elif   (FS_CFG_MAX_FILE_NAME_LEN < FS_FAT_MAX_FILE_NAME_LEN)
#error  "FS_CFG_MAX_FILE_NAME_LEN       illegally #define'd in 'fs_cfg.h'    "
#error  "                               [MUST be  > FS_FAT_MAX_FILE_NAME_LEN]"
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif
#endif
