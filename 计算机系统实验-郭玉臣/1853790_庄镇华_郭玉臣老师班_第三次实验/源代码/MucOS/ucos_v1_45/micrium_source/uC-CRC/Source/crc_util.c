/*
*********************************************************************************************************
*                                               uC/CRC
*           ERROR DETECTING CODE (EDC) & ERROR CORRECTING CODE (ECC) CALCULATION UTILITIES
*
*                          (c) Copyright 2007-2012; Micrium, Inc.; Weston, FL
*
*              All rights reserved.  Protected by international copyright laws.
*
*              uC/PRODUCT is provided in source form to registered licensees ONLY.  It is
*              illegal to distribute this source code to any third party unless you receive
*              written permission by an authorized Micrium representative.  Knowledge of
*              the source code may NOT be used to develop a similar product.
*
*              Please help us continue to provide the Embedded community with the finest
*              software available.  Your honesty is greatly appreciated.
*
*              You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                       uC/CRC UTILITY LIBRARY
*
* Filename      : crc_util.c
* Version       : V1.09.01
* Programmer(s) : EJ
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    MICRIUM_SOURCE
#include <crc_util.h>


/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifdef CRC_UTIL_MODULE_PRESENT


/*
*********************************************************************************************************
*                                     EXTERNAL C LANGUAGE LINKAGE
*
* Note(s) : (1) C++ compilers MUST 'extern'ally declare ALL C function prototypes & variable/object
*               declarations for correct C language linkage.
*********************************************************************************************************
*/

#ifdef __cplusplus
extern  "C" {                                  /* See Note #1.                                         */
#endif


/*
*********************************************************************************************************
*                                   EXTERNAL C LANGUAGE LINKAGE END
*********************************************************************************************************
*/

#ifdef __cplusplus
}                                              /* End of 'extern'al C lang linkage.                    */
#endif


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

                                                                /* Pop cnt algorithms.                                  */
#define CRC_UTIL_POPCNT_METHOD_FAST_MULT  0
#define CRC_UTIL_POPCNT_METHOD_SLOW_MULT  1

#define CRC_UTIL_POPCNT_METHOD           CRC_UTIL_POPCNT_METHOD_FAST_MULT


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           CRCUtil_PopCnt_32()
*
* Description : Compute population count (hamming weight) for value (number of bits set).
*
* Argument(s) : value           Value to compute population count on.
*
*
* Return(s)   : value's population count.
*
* Caller(s)   : various.
*
* Note(s)     : (1) Algorithm taken from http://en.wikipedia.org/wiki/Hamming_weight
*********************************************************************************************************
*/

CPU_INT08U  CRCUtil_PopCnt_32 (CPU_INT32U  value)
{
    CPU_INT32U  even_cnt;
    CPU_INT32U  odd_cnt;
    CPU_INT32U  result;


    odd_cnt  = (value >> 1u) & CRC_UTIL_POPCNT_MASK01010101_32; /* 2-bits pieces.                                       */
    result   =  value - odd_cnt;                                /* Same result as result=odd_cnt+(value & 0x55555555).  */

    even_cnt =  result & CRC_UTIL_POPCNT_MASK00110011_32;       /* 4-bits pieces.                                       */
    odd_cnt  = (result >> 2u) & CRC_UTIL_POPCNT_MASK00110011_32;
    result   =  even_cnt + odd_cnt;

    even_cnt =  result & CRC_UTIL_POPCNT_MASK00001111_32;       /* 8-bits pieces.                                       */
    odd_cnt  = (result >> 4u) & CRC_UTIL_POPCNT_MASK00001111_32;
    result   =  even_cnt + odd_cnt;

#if CRC_UTIL_POPCNT_METHOD == CRC_UTIL_POPCNT_METHOD_SLOW_MULT
    result  +=  result >> 8u;                                   /* 16-bits pieces into their lowest 8 bits              */
    result  +=  result >> 16u;                                  /* Add together both counts.                            */

    result  &=  0x3Fu
    return (result);                                            /* Mask unwanted bits.                                  */
#else
    result = (result * CRC_UTIL_POPCNT_POWERSOF256_32) >> 24u;
    return (result);
#endif

}


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of CRC_UTIL module include.                           */
