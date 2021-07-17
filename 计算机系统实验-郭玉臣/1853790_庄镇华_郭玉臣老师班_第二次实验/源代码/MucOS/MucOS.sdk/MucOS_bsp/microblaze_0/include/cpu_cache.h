/*
*********************************************************************************************************
*                                                uC/CPU
*                                    CPU CONFIGURATION & PORT LAYER
*
*                         (c) Copyright 2004-2019; Silicon Laboratories Inc.,
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
*                                           CACHE CPU MODULE
*
* Filename : cpu_cache.h
* Version  : V1.31.05
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This cache CPU header file is protected from multiple pre-processor inclusion through use of
*               the  cache CPU module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  CPU_CACHE_MODULE_PRESENT                               /* See Note #1.                                         */
#define  CPU_CACHE_MODULE_PRESENT


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   CPU_CACHE_MODULE
#define  CPU_CACHE_EXT
#else
#define  CPU_CACHE_EXT  extern
#endif


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <cpu_cfg.h>


/*
*********************************************************************************************************
*                                         CACHE CONFIGURATION
*********************************************************************************************************
*/

#ifndef CPU_CFG_CACHE_MGMT_EN
#define CPU_CFG_CACHE_MGMT_EN  DEF_DISABLED
#endif


/*
*********************************************************************************************************
*                                      CACHE OPERATIONS DEFINES
*********************************************************************************************************
*/

#if (CPU_CFG_CACHE_MGMT_EN == DEF_ENABLED)
#ifndef  CPU_DCACHE_RANGE_FLUSH
#define  CPU_DCACHE_RANGE_FLUSH(addr_start, len)  CPU_DCache_RangeFlush(addr_start, len)
#endif /* CPU_DCACHE_RANGE_FLUSH */
#else
#define  CPU_DCACHE_RANGE_FLUSH(addr_start, len)
#endif /* CPU_CFG_CACHE_MGMT_EN) */


#if (CPU_CFG_CACHE_MGMT_EN == DEF_ENABLED)
#ifndef  CPU_DCACHE_RANGE_INV
#define  CPU_DCACHE_RANGE_INV(addr_start, len)  CPU_DCache_RangeInv(addr_start, len)
#endif /* CPU_DCACHE_RANGE_INV */
#else
#define  CPU_DCACHE_RANGE_INV(addr_start, len)
#endif /* CPU_CFG_CACHE_MGMT_EN) */


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#if (CPU_CFG_CACHE_MGMT_EN == DEF_ENABLED)

#ifdef __cplusplus
extern  "C" {
#endif

void  CPU_Cache_Init       (void);

void  CPU_DCache_RangeFlush(void      *addr_start,
                            CPU_ADDR   len);

void  CPU_DCache_RangeInv  (void      *addr_start,
                            CPU_ADDR   len);

#ifdef __cplusplus
}
#endif

#endif /* CPU_CFG_CACHE_MGMT_EN */


/*
*********************************************************************************************************
*                                             MODULE END
*
* Note(s) : (1) See 'cpu_core.h  MODULE'.
*********************************************************************************************************
*/

#endif                                                          /* End of CPU core module include.                      */
