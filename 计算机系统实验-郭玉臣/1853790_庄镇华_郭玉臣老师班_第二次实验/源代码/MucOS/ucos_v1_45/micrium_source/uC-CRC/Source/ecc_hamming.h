/*
*********************************************************************************************************
*                                                uC/CRC
*           ERROR DETECTING CODE (EDC) & ERROR CORRECTING CODE (ECC) CALCULATION UTILITIES
*
*                         (c) Copyright 2007-2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved. Protected by international copyright laws.
*
*               uC/CRC is provided in source form to registered licensees ONLY.  It is
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
*                                      HAMMING CODE CALCULATION
*
* Filename      : ecc_hamming.h
* Version       : V1.09.01
* Programmer(s) : FBJ
*                 BAN
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef   ECC_HAMMING_PRESENT
#define   ECC_HAMMING_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <cpu_core.h>
#include  <lib_def.h>
#include  <lib_mem.h>
#include  <crc_cfg.h>
#include  <ecc.h>


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   ECC_HAMMING_MODULE
#define  ECC_HAMMING_EXT
#else
#define  ECC_HAMMING_EXT  extern
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
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

extern  const  ECC_CALC  Hamming_ECC;


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/

/*$PAGE*/
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void        Hamming_Calc   (void          *p_buf,               /* Calc Hamming code.                                   */
                            CPU_SIZE_T     len,
                            void          *p_buf_ext,
                            CPU_SIZE_T     len_ext,
                            CPU_INT08U    *p_ecc,
                            ECC_ERR       *p_err);

CPU_INT08U  Hamming_Chk    (void          *p_buf,               /* Chk Hamming code.                                    */
                            CPU_SIZE_T     len,
                            void          *p_buf_ext,
                            CPU_SIZE_T     len_ext,
                            CPU_INT08U    *p_ecc,
                            ECC_ERR_LOC    err_loc_tbl[],
                            CPU_INT08U     err_loc_tbl_size,
                            ECC_ERR       *p_err);

void        Hamming_Correct(void          *p_buf,               /* Correct errors.                                      */
                            CPU_SIZE_T     len,
                            void          *p_buf_ext,
                            CPU_SIZE_T     len_ext,
                            CPU_INT08U    *p_ecc,
                            ECC_ERR       *p_err);

/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

#ifndef  ECC_HAMMING_CFG_ARG_CHK_EXT_EN
#error  "ECC_HAMMING_CFG_ARG_CHK_EXT_EN         not #define'd in 'crc_cfg.h'"
#error  "                                 [MUST be DEF_ENABLED ]            "
#error  "                                 [     || DEF_DISABLED]            "

#elif  ((ECC_HAMMING_CFG_ARG_CHK_EXT_EN != DEF_DISABLED) && \
        (ECC_HAMMING_CFG_ARG_CHK_EXT_EN != DEF_ENABLED ))
#error  "ECC_HAMMING_CFG_ARG_CHK_EXT_EN   illegally #define'd in 'crc_cfg.h'"
#error  "                                 [MUST be DEF_ENABLED ]            "
#error  "                                 [     || DEF_DISABLED]            "
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of HAMMING module include.                       */
