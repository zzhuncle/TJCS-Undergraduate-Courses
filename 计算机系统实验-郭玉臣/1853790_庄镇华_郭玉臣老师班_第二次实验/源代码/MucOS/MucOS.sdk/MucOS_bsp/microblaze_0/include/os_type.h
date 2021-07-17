/*
*********************************************************************************************************
*                                                uC/OS-III
*                                          The Real-Time Kernel
*
*                         (c) Copyright 2009-2019; Silicon Laboratories Inc.,
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
* File    : os_type.h
* Version : V3.07.05
*********************************************************************************************************
*/

#ifndef   OS_TYPE_H
#define   OS_TYPE_H

#ifdef    VSC_INCLUDE_H_FILE_NAMES
const     CPU_CHAR  *os_type__h = "$Id: $";
#endif

/*
************************************************************************************************************************
*                                                 INCLUDE HEADER FILES
************************************************************************************************************************
*/

                                                       /*       Description                                    # Bits */
                                                       /*                                               <recommended> */
                                                       /* ----------------------------------------------------------- */

typedef   CPU_INT16U      OS_CPU_USAGE;                /* CPU Usage 0..10000                                  <16>/32 */

typedef   CPU_INT32U      OS_CTR;                      /* Counter,                                                 32 */

typedef   CPU_INT32U      OS_CTX_SW_CTR;               /* Counter of context switches,                             32 */

typedef   CPU_INT32U      OS_CYCLES;                   /* CPU clock cycles,                                   <32>/64 */

typedef   CPU_INT32U      OS_FLAGS;                    /* Event flags,                                      8/16/<32> */

typedef   CPU_INT32U      OS_IDLE_CTR;                 /* Holds the number of times the idle task runs,       <32>/64 */

typedef   CPU_INT16U      OS_MEM_QTY;                  /* Number of memory blocks,                            <16>/32 */
typedef   CPU_INT16U      OS_MEM_SIZE;                 /* Size in bytes of a memory block,                    <16>/32 */

typedef   CPU_INT16U      OS_MSG_QTY;                  /* Number of OS_MSGs in the msg pool,                  <16>/32 */
typedef   CPU_INT16U      OS_MSG_SIZE;                 /* Size of messages in number of bytes,                <16>/32 */

typedef   CPU_INT08U      OS_NESTING_CTR;              /* Interrupt and scheduler nesting,                  <8>/16/32 */

typedef   CPU_INT16U      OS_OBJ_QTY;                  /* Number of kernel objects counter,                   <16>/32 */
typedef   CPU_INT32U      OS_OBJ_TYPE;                 /* Special flag to determine object type,                   32 */

typedef   CPU_INT16U      OS_OPT;                      /* Holds function options,                             <16>/32 */

typedef   CPU_INT08U      OS_PRIO;                     /* Priority of a task,                               <8>/16/32 */

typedef   CPU_INT16U      OS_QTY;                      /* Quantity                                            <16>/32 */

typedef   CPU_INT32U      OS_RATE_HZ;                  /* Rate in Hertz                                            32 */

#if (CPU_CFG_ADDR_SIZE == CPU_WORD_SIZE_64)            /* Task register                                  8/16/<32/64> */
typedef   CPU_INT64U      OS_REG;
#else
typedef   CPU_INT32U      OS_REG;
#endif
typedef   CPU_INT08U      OS_REG_ID;                   /* Index to task register                            <8>/16/32 */

typedef   CPU_INT32U      OS_SEM_CTR;                  /* Semaphore value                                     16/<32> */

typedef   CPU_INT08U      OS_STATE;                    /* State variable                                    <8>/16/32 */

typedef   CPU_INT08U      OS_STATUS;                   /* Status                                            <8>/16/32 */

typedef   CPU_INT32U      OS_TICK;                     /* Clock tick counter                                  <32>/64 */

#endif
