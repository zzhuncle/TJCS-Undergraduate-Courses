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
* Filename : fs_dev_nand_ctrlr_gen_soft_ecc.h
* Version  : V4.07.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_NAND_CGEN_SOFT_ECC_MODULE_PRESENT
#define  FS_NAND_CGEN_SOFT_ECC_MODULE_PRESENT


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

#ifdef   FS_NAND_CGEN_SOFT_ECC_MODULE
#define  FS_NAND_CGEN_SOFT_ECC_EXT
#else
#define  FS_NAND_CGEN_SOFT_ECC_EXT  extern
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

#define  FS_NAND_CTRLR_GEN_SOFT_ECC_CFG_FIELDS \
    FS_NAND_CTRLR_GEN_SOFT_ECC_CFG_FIELD(const  ECC_CALC, *ECC_ModulePtr, DEF_NULL /* Pointer to ECC module */)

#define FS_NAND_CTRLR_GEN_SOFT_ECC_CFG_FIELD(type, name, dflt_val)  type name;
typedef  struct  {
    FS_NAND_CTRLR_GEN_SOFT_ECC_CFG_FIELDS
} FS_NAND_CTRLR_GEN_SOFT_ECC_CFG;
#undef  FS_NAND_CTRLR_GEN_SOFT_ECC_CFG_FIELD

extern  const  FS_NAND_CTRLR_GEN_SOFT_ECC_CFG  FS_NAND_CtrlrGen_SoftECC_DfltCfg;


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/

extern  FS_NAND_CTRLR_GEN_EXT  FS_NAND_CtrlrGen_SoftECC;

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

