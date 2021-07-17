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
*                                   FILE SYSTEM COUNTER MANAGEMENT
*
* Filename : fs_ctr.h
* Version  : V4.07.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_CTR_H
#define  FS_CTR_H


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  "fs_cfg_fs.h"


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_CTR_MODULE
#define  FS_CTR_EXT
#else
#define  FS_CTR_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                    FILE SYSTEM COUNTER DATA TYPE
*
* Note(s) : (1) FS_CTR_MAX SHOULD be #define'd based on 'FS_CTR' data type declared.
*********************************************************************************************************
*/

typedef  CPU_INT32U  FS_CTR;                                    /* Defines   max nbr of errs/stats to cnt.              */

#define  FS_CTR_MAX                         DEF_INT_32U_MAX_VAL /* Define as max unsigned val (see Note #1).            */


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
*                                     FILE SYSTEM COUNTER MACRO'S
*
* Description : Increment file system counter(s).
*
* Argument(s) : Various file system counter variable(s) & values.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

                                                                /* ---------------- GENERIC CTR MACRO'S --------------- */
#define  FS_CTR_INC(ctr)                                     { (ctr)++; }

#define  FS_CTR_INC_LARGE(ctr_hi, ctr_lo)                    { (ctr_lo)++;            \
                                                                if ((ctr_lo) == 0u) { \
                                                                    (ctr_hi)++;       \
                                                                }                     \
                                                             }

#define  FS_CTR_ADD(ctr, val)                                { (ctr) += (val); }



#if (FS_CFG_CTR_STAT_EN == DEF_ENABLED)                         /* ----------------- STAT CTR MACRO'S ----------------- */

#define  FS_CTR_STAT_INC(stat_ctr)                              FS_CTR_INC(stat_ctr)
#define  FS_CTR_STAT_ADD(stat_ctr, val)                         FS_CTR_ADD((stat_ctr), (val))

#else

#define  FS_CTR_STAT_INC(stat_ctr)
#define  FS_CTR_STAT_ADD(stat_ctr, val)

#endif


#if (FS_CFG_CTR_ERR_EN == DEF_ENABLED)                          /* ------------------ ERR CTR MACRO'S ----------------- */

#define  FS_CTR_ERR_INC(err_ctr)                                FS_CTR_INC(err_ctr)

#else

#define  FS_CTR_ERR_CLR(err_ctr);

#define  FS_CTR_ERR_INC(err_ctr)

#endif


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/


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

