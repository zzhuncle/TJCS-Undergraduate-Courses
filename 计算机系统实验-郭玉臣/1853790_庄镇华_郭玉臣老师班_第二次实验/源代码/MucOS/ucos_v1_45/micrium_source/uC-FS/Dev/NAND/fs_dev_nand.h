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
*                                         FILE SYSTEM DEVICE DRIVER
*
*                                             NAND FLASH DEVICES
*
* Filename : fs_dev_nand.h
* Version  : V4.07.07
*
*********************************************************************************************************
* Note(s)  : (1) Supports NAND-type Flash memory devices, including :
*                (a) Parallel NAND Flash.
*                    (1) Small-page (512-B) SLC Devices.
*                    (2) Large-page (2048-, 4096-, 8096-B) SLC Devices.
*                    (3) Some MLC Devices.
*
*            (2) Supported media MUST have the following characteristics :
*                (a) Medium organized into units (called blocks) which are erased at the same time.
*                (b) When erased, all bits are set to 1.
*                (c) Only an erase operation can change a bit from 0 to 1.
*                (d) Each block divided into smaller units called pages: each page has a data area
*                    as well as a spare area. 16 bytes spare area are required for each 512 bytes
*                    sector in the page.
*
*            (3) Supported media TYPICALLY have the following characteristics :
*                (a) A  program operation takes much longer than a read    operation.
*                (b) An erase   operation takes much longer than a program operation.
*                (c) The number of erase operations per block is limited.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_NAND_MODULE_PRESENT
#define  FS_NAND_MODULE_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <fs_dev_nand_cfg.h>
#include  "../../Source/fs.h"
#include  "../../Source/fs_dev.h"


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_NAND_MODULE
#define  FS_NAND_EXT
#else
#define  FS_NAND_EXT  extern
#endif


/*
*********************************************************************************************************
*                                        DEFAULT CONFIGURATION
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  FS_NAND_CFG_DEFAULT                                  0u

#define  FS_NAND_PG_IX_INVALID             ((FS_NAND_PG_SIZE)-1)

#define  FS_NAND_PART_ONFI_PARAM_PAGE_LEN                   256u/* Len of param pg.                                     */

#define  FS_NAND_CTRS_TBL_SIZE                                4u/* Max nbr of ctrs structs in global tbl.               */


/*
*********************************************************************************************************
*                                              DATA TYPES
*********************************************************************************************************
*/

typedef  CPU_INT16U  FS_NAND_BLK_QTY;
typedef  CPU_INT08U  FS_NAND_UB_QTY;
typedef  CPU_INT16U  FS_NAND_PG_PER_BLK_QTY;
typedef  CPU_INT16U  FS_NAND_SEC_PER_BLK_QTY;
typedef  CPU_INT08U  FS_NAND_ASSOC_BLK_QTY;
typedef  CPU_INT16U  FS_NAND_PG_SIZE;


/*
*********************************************************************************************************
*                                       NAND IO CTRL DATA DATA TYPE
*********************************************************************************************************
*/

typedef  struct  fs_nand_io_ctrl_data {
    void        *DataPtr;                                       /* Ptr to data to write.                                */
    void        *OOS_Ptr;                                       /* Ptr to out of sec data to write.                     */
    FS_SEC_NBR   IxPhy;                                         /* Physical sec ix.                                     */
} FS_NAND_IO_CTRL_DATA;


/*
*********************************************************************************************************
*                                     NAND FREE SPARE AREA DATA TYPE
*********************************************************************************************************
*/

typedef  struct  fs_nand_free_spare_data  FS_NAND_FREE_SPARE_DATA;
struct  fs_nand_free_spare_data {
    FS_NAND_PG_SIZE  OctetOffset;                               /* Offset in octets of free section of spare.           */
    FS_NAND_PG_SIZE  OctetLen;                                  /* Len in octets of free section of spare.           */
};


/*
*********************************************************************************************************
*                                   NAND FLASH DEVICE SPECIFIC DATA
*********************************************************************************************************
*/

typedef enum {
    DEFECT_SPARE_L_1_PG_1_OR_N_ALL_0 = 0,                       /* Spare byte/word 1 in first or last pg == 0.          */
    DEFECT_SPARE_ANY_PG_1_OR_N_ALL_0 = 1,                       /* Spare any byte/word in first or last pg == 0.        */
    DEFECT_SPARE_B_6_W_1_PG_1_OR_2   = 2,                       /* Spare byte 6/word 1 in pg 1 or 2 != FFh.             */
    DEFECT_SPARE_L_1_PG_1_OR_2       = 3,                       /* Spare byte/word 1 in pg 1 or 2 != FFh.               */
    DEFECT_SPARE_B_1_6_W_1_IN_PG_1   = 4,                       /* Spare byte 1&6/word 1 in pg 1 != FFh.                */
    DEFECT_PG_L_1_OR_N_PG_1_OR_2     = 5,                       /* Byte/word 1 (main area) in page 1 or 2 != FFh.       */
    DEFECT_MARK_TYPE_NBR             = 6                        /* Must be last in enum.                                */
} FS_NAND_DEFECT_MARK_TYPE;


typedef  struct  fs_nand_part_data  FS_NAND_PART_DATA;
struct  fs_nand_part_data {
    FS_NAND_BLK_QTY           BlkCnt;                           /* Total number of blocks.                              */
    FS_NAND_PG_PER_BLK_QTY    PgPerBlk;                         /* Nbr of pgs per blk.                                  */
    FS_NAND_PG_SIZE           PgSize;                           /* Size (in octets) of each pg.                         */
    FS_NAND_PG_SIZE           SpareSize;                        /* Size (in octets) of spare area per pg.               */
    CPU_INT08U                NbrPgmPerPg;                      /* Nbr of program operation per pg.                     */
    CPU_INT08U                BusWidth;                         /* Bus width of NAND dev.                               */
    CPU_INT08U                ECC_NbrCorrBits;                  /* Nbr of bits of ECC correctability.                   */
    FS_NAND_PG_SIZE           ECC_CodewordSize;                 /* ECC codeword size in bytes.                          */
    FS_NAND_DEFECT_MARK_TYPE  DefectMarkType;                   /* Factory defect mark type.                            */
    FS_NAND_BLK_QTY           MaxBadBlkCnt;                     /* Max nbr of bad blk in dev.                           */
    CPU_INT32U                MaxBlkErase;                      /* Maximum number of erase operations per block.        */
    FS_NAND_FREE_SPARE_DATA  *FreeSpareMap;                     /* Pointer to the map of available bytes in spare area. */


    FS_NAND_PART_DATA        *NextPtr;
};


/*
**********************************************************************************************************
*                              NAND FLASH DEVICE CONTROLLER API DATA TYPE
**********************************************************************************************************
*/

typedef  struct  fs_nand_oos_info {
    FS_NAND_PG_SIZE   Size;
    void             *BufPtr;
} FS_NAND_OOS_INFO;

typedef  struct  fs_nand_part_api FS_NAND_PART_API;

typedef  struct  fs_nand_ctrlr_api {
    void                (*Init)        (FS_ERR            *p_err);          /* Init NAND ctrlr.                         */

    void               *(*Open)        (FS_NAND_PART_API  *p_part_api,      /* Open NAND ctrlr.                         */
                                        void              *p_bsp_api,
                                        void              *p_ctrlr_cfg,
                                        void              *p_part_cfg,
                                        FS_ERR            *p_err);

    void                (*Close)       (void              *p_ctrlr_data);   /* Close NAND ctrlr.                        */

    FS_NAND_PART_DATA  *(*PartDataGet) (void              *p_ctrlr_data);   /* Get part data ptr.                       */

    FS_NAND_OOS_INFO    (*Setup)       (void              *p_ctrlr_data,    /* Setup ctrlr.                             */
                                        FS_NAND_PG_SIZE    sec_size,
                                        FS_ERR            *p_err);

                                                                            /* ------- NAND FLASH HIGH LVL OPS -------- */
    void                (*SecRd)       (void              *p_ctrlr_data,    /* Read from pg.                            */
                                        void              *p_dest,
                                        void              *p_dest_oos,
                                        FS_SEC_NBR         sec_ix_phy,
                                        FS_ERR            *p_err);

    void                (*OOSRdRaw)    (void              *p_ctrlr_data,    /* Read oos raw data without ECC chk.       */
                                        void              *p_dest_oos,
                                        FS_SEC_NBR         sec_nbr_phy,
                                        FS_NAND_PG_SIZE    offset,
                                        FS_NAND_PG_SIZE    length,
                                        FS_ERR            *p_err);

    void                (*SpareRdRaw)  (void              *p_ctrlr_data,    /* Read spare data without ECC check.       */
                                        void              *p_dest_oos,
                                        FS_SEC_QTY         pg_nbr_phy,
                                        FS_NAND_PG_SIZE    offset,
                                        FS_NAND_PG_SIZE    length,
                                        FS_ERR            *p_err);

    void                (*SecWr)       (void              *p_ctrlr_data,    /* Write page to NAND device.               */
                                        void              *p_src,
                                        void              *p_src_spare,
                                        FS_SEC_NBR         sec_nbr_phy,
                                        FS_ERR            *p_err);

    void                (*BlkErase)    (void              *p_ctrlr_data,    /* Erase block on NAND device.              */
                                        CPU_INT32U         blk_nbr_phy,
                                        FS_ERR            *p_err);

    void                (*IO_Ctrl)     (void              *p_ctrlr_data,    /* Perform NAND device I/O control.         */
                                        CPU_INT08U         cmd,
                                        void              *p_buf,
                                        FS_ERR            *p_err);
} FS_NAND_CTRLR_API;


/*
**********************************************************************************************************
*                             NAND FLASH DEVICE PART-SPECIFIC API DATA TYPE
**********************************************************************************************************
*/

struct  fs_nand_part_api {
    FS_NAND_PART_DATA  *(*Open)  (const  FS_NAND_CTRLR_API  *p_ctrlr_api,   /* Get NAND part-specific data.             */
                                         void               *p_ctrlr_data,
                                         void               *p_part_cfg,
                                         FS_ERR             *p_err);

    void                (*Close) (       FS_NAND_PART_DATA  *p_part_data); /* Close part layer impl.                    */
};


/*
*********************************************************************************************************
*                               NAND FLASH DEVICE CONFIGURATION DATA TYPE
**********************************************************************************************************
*/

#define  FS_NAND_CFG_FIELDS                                                            \
    FS_NAND_CFG_FIELD(void             , *BSPPtr                , DEF_NULL           ) \
    FS_NAND_CFG_FIELD(FS_NAND_CTRLR_API, *CtrlrPtr              , DEF_NULL           ) \
    FS_NAND_CFG_FIELD(void             , *CtrlrCfgPtr           , DEF_NULL           ) \
    FS_NAND_CFG_FIELD(FS_NAND_PART_API , *PartPtr               , DEF_NULL           ) \
    FS_NAND_CFG_FIELD(void             , *PartCfgPtr            , DEF_NULL           ) \
    FS_NAND_CFG_FIELD(FS_SEC_SIZE      ,  SecSize               , FS_NAND_CFG_DEFAULT) \
    FS_NAND_CFG_FIELD(FS_NAND_BLK_QTY  ,  BlkCnt                , FS_NAND_CFG_DEFAULT) \
    FS_NAND_CFG_FIELD(FS_NAND_BLK_QTY  ,  BlkIxFirst            , 0u                 ) \
    FS_NAND_CFG_FIELD(FS_NAND_UB_QTY   ,  UB_CntMax             , 3u                 ) \
    FS_NAND_CFG_FIELD(CPU_INT08U       ,  RUB_MaxAssoc          , 2u                 ) \
    FS_NAND_CFG_FIELD(CPU_INT08U       ,  AvailBlkTblEntryCntMax, FS_NAND_CFG_DEFAULT)


#define FS_NAND_CFG_FIELD(type, name, dftl_val) type name;
typedef  struct  fs_nand_cfg {
    FS_NAND_CFG_FIELDS
} FS_NAND_CFG;
#undef  FS_NAND_CFG_FIELD

extern  const  FS_NAND_CFG  FS_NAND_DfltCfg;


/*
**********************************************************************************************************
*                             NAND FLASH DEVICE PART-SPECIFIC API DATA TYPE
**********************************************************************************************************
*/

typedef  struct  fs_nand_param_pg_io_ctrl_data {
    CPU_INT16U      RelAddr;                                    /* Relative addr of the param pg to rd from.            */
    CPU_INT16U      ByteCnt;                                    /* Nb of bytes to rd.                                   */
    CPU_INT08U     *DataPtr;                                    /* Ptr to param pg data.                                */
} FS_NAND_RD_PARAM_PG_IO_CTRL_DATA;


/*
*********************************************************************************************************
*                                       NAND STAT AND ERR CTRS
*********************************************************************************************************
*/

#if ((FS_CFG_CTR_STAT_EN == DEF_ENABLED) || \
     (FS_CFG_CTR_ERR_EN  == DEF_ENABLED))
typedef  struct  fs_nand_ctrs {
#if (FS_CFG_CTR_STAT_EN == DEF_ENABLED)                         /* -------------------- STAT CTRS --------------------- */
    FS_CTR                    StatRdCtr;                        /* Nbr of sec rd.                                       */
    FS_CTR                    StatWrCtr;                        /* Nbr of sec wr.                                       */

    FS_CTR                    StatMetaSecCommitCtr;             /* Nbr of meta sec commits.                             */
    FS_CTR                    StatSUB_MergeCtr;                 /* Nbr of SUB merges done.                              */
    FS_CTR                    StatRUB_MergeCtr;                 /* Nbr of RUB full merges done.                         */
    FS_CTR                    StatRUB_PartialMergeCtr;          /* Nbr of RUB partial merges done.                      */

    FS_CTR                    StatBlkRefreshCtr;                /* Nbr of blk refreshes done.                           */
#endif

#if (FS_CFG_CTR_ERR_EN == DEF_ENABLED)                          /* --------------------- ERR CTRS --------------------- */
    FS_CTR                    ErrRefreshDataLoss;               /* Nbr of unrefreshable/lost data sectors.              */
#endif
} FS_NAND_CTRS;
#endif


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

extern  const  FS_DEV_API     FS_NAND;
FS_NAND_EXT    FS_QTY         FS_NAND_UnitCtr;
#if ((FS_CFG_CTR_STAT_EN == DEF_ENABLED) || \
     (FS_CFG_CTR_ERR_EN  == DEF_ENABLED))
FS_NAND_EXT    FS_NAND_CTRS  *FS_NAND_CtrsTbl[FS_NAND_CTRS_TBL_SIZE];
#endif

/*
*********************************************************************************************************
*                                         CTRLR DRIVERS APIs
*********************************************************************************************************
*/

extern           const  FS_NAND_CTRLR_API  FS_NAND_CtrlrGen;


/*
*********************************************************************************************************
*                                          PART DRIVERS APIs
*********************************************************************************************************
*/

extern           const  FS_NAND_PART_API  FS_NAND_PartStatic;


/*
*********************************************************************************************************
*                                               MACROS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               TRACING
*********************************************************************************************************
*/

#if ((defined(FS_NAND_TRACE))       && \
     (defined(FS_NAND_TRACE_LEVEL)) && \
     (FS_NAND_TRACE_LEVEL >= TRACE_LEVEL_INFO))

    #if (FS_NAND_TRACE_LEVEL >= TRACE_LEVEL_LOG)
        #define  FS_NAND_TRACE_LOG(msg)       FS_NAND_TRACE  msg
    #else
        #define  FS_NAND_TRACE_LOG(msg)
    #endif


    #if (FS_NAND_TRACE_LEVEL >= TRACE_LEVEL_DBG)
        #define  FS_NAND_TRACE_DBG(msg)       FS_NAND_TRACE  msg
    #else
        #define  FS_NAND_TRACE_DBG(msg)
    #endif

    #define  FS_NAND_TRACE_INFO(msg)          FS_NAND_TRACE  msg
#else
    #ifndef  FS_NAND_TRACE_LOG
        #define  FS_NAND_TRACE_LOG(msg)       FS_TRACE_LOG(msg)
    #endif

    #ifndef  FS_NAND_TRACE_DBG
        #define  FS_NAND_TRACE_DBG(msg)       FS_TRACE_DBG(msg)
    #endif

    #ifndef  FS_NAND_TRACE_INFO
        #define  FS_NAND_TRACE_INFO(msg)  FS_TRACE_INFO(msg)
    #endif

#endif


/*
*********************************************************************************************************
*                                        FUNCTION PROTOTYPES
*********************************************************************************************************
*/

                                                                /* ------------------ LOW-LEVEL FNCTS ----------------- */
void         FS_NAND_LowFmt    (CPU_CHAR  *p_name_dev,          /* Low-level format  device.                            */
                                FS_ERR    *p_err);

void         FS_NAND_LowMount  (CPU_CHAR  *p_name_dev,          /* Low-level mount   device.                            */
                                FS_ERR    *p_err);

void         FS_NAND_LowUnmount(CPU_CHAR  *p_name_dev,          /* Low-level unmount device.                            */
                                FS_ERR    *p_err);


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
                                                                /* End of NAND module include.                          */
#endif
