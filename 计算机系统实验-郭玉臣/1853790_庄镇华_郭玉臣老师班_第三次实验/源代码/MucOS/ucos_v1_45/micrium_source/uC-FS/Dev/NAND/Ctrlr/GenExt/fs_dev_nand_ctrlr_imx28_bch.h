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
*                         NAND DEVICE GENERIC CONTROLLER SOFTWARE ECC EXTENSION
*
* Filename : fs_dev_nand_ctrlr_imx28_bch.h
* Version  : V4.07.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_NAND_CGEN_BCH_MODULE_PRESENT
#define  FS_NAND_CGEN_BCH_MODULE_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "../fs_dev_nand_ctrlr_gen.h"


/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_NAND_CGEN_BCH_MODULE
#define  FS_NAND_CGEN_BCH_EXT
#else
#define  FS_NAND_CGEN_BCH_EXT  extern
#endif


/*
*********************************************************************************************************
*                                         DEFAULT CONFIGURATION
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/

#define  FS_NAND_CTRLR_GEN_BCH_CFG_FIELDS \
    FS_NAND_CTRLR_GEN_BCH_CFG_FIELD(       CPU_INT32U,  Ecc0,     2u) \
    FS_NAND_CTRLR_GEN_BCH_CFG_FIELD(       CPU_INT32U,  EccN,     2u) \
    FS_NAND_CTRLR_GEN_BCH_CFG_FIELD(       CPU_INT32U,  EccThres, 0u) \
    FS_NAND_CTRLR_GEN_BCH_CFG_FIELD(const  ECC_CALC,   *NextPtr, DEF_NULL)

#define FS_NAND_CTRLR_GEN_BCH_CFG_FIELD(type, name, dflt_val)  type name;
typedef  struct  {
    FS_NAND_CTRLR_GEN_BCH_CFG_FIELDS
} FS_NAND_CTRLR_GEN_BCH_CFG;
#undef  FS_NAND_CTRLR_GEN_BCH_CFG_FIELD

extern  const  FS_NAND_CTRLR_GEN_BCH_CFG  FS_NAND_CtrlrGen_BCH_DlftCfg;


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/

extern  FS_NAND_CTRLR_GEN_EXT  FS_NAND_CtrlrGen_BCH;

/*
*********************************************************************************************************
*                                                 MACROS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          CONFIGURATION ERRORS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE END
*********************************************************************************************************
*/

#endif

