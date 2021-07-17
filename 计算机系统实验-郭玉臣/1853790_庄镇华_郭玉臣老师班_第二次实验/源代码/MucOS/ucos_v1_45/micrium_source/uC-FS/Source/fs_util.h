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
*                                  FILE SYSTEM SUITE UTILITY LIBRARY
*
* Filename : fs_util.h
* Version  : V4.07.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_UTIL_H
#define  FS_UTIL_H


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  "fs.h"


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_UTIL_MODULE
#define  FS_UTIL_EXT
#else
#define  FS_UTIL_EXT  extern
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


/*
*********************************************************************************************************
*                                              MACRO'S
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                   POWER-2 MULTIPLY/DIVIDE MACRO'S
*
* Note(s) : (1) Multiplications & divisions by powers of 2 are common within the file system suite.
*               If the power-of-2 multiplication or divisor is a constant, a compiler can optimize the
*               calculation (typically encoding it as a logical shift).  However, many of the powers-of-2
*               multiplicands & divisors are known only at run-time, so the integer multiplications &
*               divisions lose important information that could have been used for optimization.
*
*               Multiplications & divisions by powers of 2 within this file system suite are performed
*               with macros 'FS_UTIL_MULT_PWR2' & 'FS_UTIL_DIV_PWR2', using left & right shifts.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         FS_UTIL_MULT_PWR2()
*
* Description : Multiple integer by a power of 2.
*
* Argument(s) : nbr         First multiplicand.
*
*               pwr         Power of second multiplicand.
*
* Return(s)   : Product = nbr * 2^pwr.
*
* Note(s)     : (1) As stated in ISO/IEC 9899:TCP 6.5.8(4) :
*
*                      "The result of E1 << E2 is E1 left-shifted E2 bit-positions; vacated bits are
*                       filled with zeros.  If E1 has an unsigned type, the value of the result is E1 x
*                       2^E2, reduces modulo one more than the maximum value representable in the result
*                       type."
*
*                   Even with conforming compilers, this macro MAY ONLY be used with unsigned operands.
*                   Results with signed operands are undefined.
*
*               (2) With a non-conforming compiler, this macro should be commented out & redefined.
*********************************************************************************************************
*/

#define  FS_UTIL_MULT_PWR2(nbr, pwr)                ((nbr) << (pwr))


/*
*********************************************************************************************************
*                                         FS_UTIL_DIV_PWR2()
*
* Description : Divide integer by a power of 2.
*
* Argument(s) : nbr         Dividend.
*
*               pwr         Power of divisor.
*
* Return(s)   : Quotient = nbr / 2^pwr.
*
* Note(s)     : (1) As stated in ISO/IEC 9899:TCP 6.5.8(5) :
*
*                      "The result of E1 >> E2 is E1 right-shifted E2 bit-positions.  If E1 has an
*                       unsigned type ..., the value of the result is the integral part of the quotient
*                       of E1 / 2^E2."
*
*                   Even with conforming compilers, this macro MAY ONLY be used with unsigned operands.
*                   Results with signed operands are undefined.
*
*               (2) With a non-conforming compiler, this macro should be commented out & redefined.
*********************************************************************************************************
*/

#define  FS_UTIL_DIV_PWR2(nbr, pwr)                 ((nbr) >> (pwr))


/*
*********************************************************************************************************
*                                          FS_UTIL_IS_PWR2()
*
* Description : Determine whether unsigned integer is a power of 2 or not.
*
* Argument(s) : nbr         Unsigned integer.
*
* Return(s)   : DEF_YES, if integer is     a power of 2.
*               DEF_NO,  if integer is not a power of 2.
*
* Note(s)     : none
*********************************************************************************************************
*/

#define  FS_UTIL_IS_PWR2(nbr)                     ((((nbr) != 0u) && (((nbr) & ((nbr) - 1u)) == 0u)) ? DEF_YES : DEF_NO)


/*
*********************************************************************************************************
*                                          FS_UTIL_IS_ODD()
*
* Description : Determine whether unsigned integer is odd.
*
* Argument(s) : nbr         Unsigned integer.
*
* Return(s)   : DEF_YES, if integer is odd.
*               DEF_NO,  if integer is even.
*
* Note(s)     : none
*********************************************************************************************************
*/

#define  FS_UTIL_IS_ODD(nbr)                        (DEF_BIT_IS_SET((nbr), DEF_BIT_00))


/*
*********************************************************************************************************
*                                          FS_UTIL_IS_EVEN()
*
* Description : Determine whether unsigned integer is even.
*
* Argument(s) : nbr         Unsigned integer.
*
* Return(s)   : DEF_YES, if integer is even.
*               DEF_NO,  if integer is odd.
*
* Note(s)     : none
*********************************************************************************************************
*/

#define  FS_UTIL_IS_EVEN(nbr)                       (DEF_BIT_IS_CLR((nbr), DEF_BIT_00))


/*
*********************************************************************************************************
*                                   BIT/OCTET MANIPULATION MACRO'S
*
* Note(s) : (1) These macros allow to perform mutliple conversions between bits and octets, either at
*               runtime or compile time.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                     FS_UTIL_BIT_NBR_TO_OCTET_NBR()
*
* Description : Convert a number of required bits (generally for a bitmap) into a number of required
*               octets.
*
* Argument(s) : bit_nbr     Number of required bits.
*
* Return(s)   : The lowest number of octets that contains at least bit_nbr bits.
*
* Note(s)     : none
*********************************************************************************************************
*/

#define  FS_UTIL_BIT_NBR_TO_OCTET_NBR(bit_nbr)                ( ((bit_nbr) >> DEF_OCTET_TO_BIT_SHIFT) + \
                                                               (((bit_nbr) &  DEF_OCTET_TO_BIT_MASK) == 0u ? 0u : 1u))


/*
*********************************************************************************************************
*                                      FS_UTIL_BITMAP_LOC_GET()
*
* Description : Convert the position of a bit in a bitmap to the equivalent location of an octet in
*               this bitmap, and the location of the bit in this octet.
*
* Argument(s) : bit_pos     Position of the bit in the bitmap/array.
*
*               octet_loc   Location of the octet in the bitmap/array.
*
*               bit_loc     Location of the bit in the octet.
*
* Return(s)   : none.
*
* Note(s)     : (1) Care must be taken not to use the same variable for 'bit_pos' and 'bit_loc', because
*                   it is modified before being used for the calculation of 'octet_loc'.
*********************************************************************************************************
*/

#define  FS_UTIL_BITMAP_LOC_GET(bit_pos, octet_loc, bit_loc)    do {                                                    \
                                                                    (bit_loc)   = (bit_pos)  & DEF_OCTET_TO_BIT_MASK;   \
                                                                    (octet_loc) = (bit_pos) >> DEF_OCTET_TO_BIT_SHIFT;  \
                                                                } while (0)


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

CPU_INT08U   FSUtil_Log2              (CPU_INT32U  val);            /* Calculate base-2 logarithm of integer.           */

void         FSUtil_ValPack32         (CPU_INT08U  *p_dest,         /* Pack val in array using specified nbr of bits.   */
                                       CPU_SIZE_T  *p_offset_octet,
                                       CPU_DATA    *p_offset_bit,
                                       CPU_INT32U   val,
                                       CPU_DATA     nbr_bits);

CPU_INT32U   FSUtil_ValUnpack32       (CPU_INT08U  *p_src,          /* Unpack val from array.                           */
                                       CPU_SIZE_T  *p_offset_octet,
                                       CPU_DATA    *p_offset_bit,
                                       CPU_DATA     nbr_bits);

CPU_BOOLEAN  FSUtil_MapBitIsSet       (CPU_INT08U  *p_bitmap,       /* Determine if specified bit is set in bitmap.     */
                                       CPU_SIZE_T   offset_bit);

void         FSUtil_MapBitSet         (CPU_INT08U  *p_bitmap,       /* Set specified bit in bitmap.                     */
                                       CPU_SIZE_T   offset_bit);

void         FSUtil_MapBitClr         (CPU_INT08U  *p_bitmap,       /* Clr specified bit in bitmap.                     */
                                       CPU_SIZE_T   offset_bit);

void        *FSUtil_ModuleDataGet     (CPU_SIZE_T    data_size,     /* Allocate module data.                            */
                                       void        **pp_data_head,
                                       FS_ERR       *p_err);

void         FSUtil_ModuleDataFree    (void         *p_data,        /* Free module data.                                */
                                       CPU_SIZE_T    data_size,
                                       void        **pp_data_head,
                                       FS_ERR       *p_err);

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
