/*
*********************************************************************************************************
*                                                uC/CRC
*           ERROR DETECTING CODE (EDC) & ERROR CORRECTING CODE (ECC) CALCULATION UTILITIES
*
*                         (c) Copyright 2007-2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved. Protected by international copyright laws.
*
*               uC/FS is provided in source form to registered licensees ONLY.  It is
*               illegal to distribute this source code to any third party unless you receive
*               written permission by an authorized Micrium representative.  Knowledge of
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                         CONFIGURATION FILE
*
*                                              TEMPLATE
*
* Filename      : crc_cfg.h
* Version       : V1.09.00
* Programmer(s) : BAN
*                 EJ
*********************************************************************************************************
*/

#ifndef  CRC_CFG_H
#define  CRC_CFG_H


/*
*********************************************************************************************************
*                                                 EDC
*
* Note(s) : (1) Configure EDC_CRC_CFG_ERR_ARG_CHK_EXT_EN to enable/disable the CRC external argument
*               check feature
*
*           (2) Configure EDC_CRC_CFG_OPTIMIZE_ASM_EN to enable/disable optimized assembly-language
*               calculation.
*
*           (3) Each of these determine whether the associated table of pre-computed CRC values will
*               be included in the build.  For more information about the tables, the meaning of the
*               identifiers, and how these are used, please refer to the documentation.
*********************************************************************************************************
*/

#define  EDC_CRC_CFG_ERR_ARG_CHK_EXT_EN          DEF_ENABLED    /* See Note #1.                                         */
#define  EDC_CRC_CFG_OPTIMIZE_ASM_EN             DEF_DISABLED   /* See Note #2.                                         */

#define  EDC_CRC_CFG_CRC16_1021_EN               DEF_ENABLED    /* See Note #3.                                         */
#define  EDC_CRC_CFG_CRC16_8005_EN               DEF_ENABLED
#define  EDC_CRC_CFG_CRC16_8048_EN               DEF_ENABLED
#define  EDC_CRC_CFG_CRC32_EN                    DEF_ENABLED
#define  EDC_CRC_CFG_CRC16_1021_REF_EN           DEF_ENABLED
#define  EDC_CRC_CFG_CRC16_8005_REF_EN           DEF_ENABLED
#define  EDC_CRC_CFG_CRC16_8048_REF_EN           DEF_ENABLED
#define  EDC_CRC_CFG_CRC32_REF_EN                DEF_ENABLED

/*
*********************************************************************************************************
*                                                 ECC
*
* Note(s) : (1) Configure ECC_HAMMING_CFG_ARG_CHK_EXT_EN to enable/disable the Hamming code external
*               argument check feature
*
*********************************************************************************************************
*/

#define  ECC_HAMMING_CFG_ARG_CHK_EXT_EN          DEF_ENABLED    /* See Note #1.                                         */


#endif
