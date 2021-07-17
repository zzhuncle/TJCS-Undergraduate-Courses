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
*                                            CPU PORT FILE
*
*                                              Microblaze
*                                                 GNU
*
* Filename : cpu_c.c
* Version  : V1.31.05
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    MICRIUM_SOURCE
#include  <cpu.h>
#include  <cpu_core.h>
#include  <mb_interface.h>

#ifdef __cplusplus
extern  "C" {
#endif


/*
*********************************************************************************************************
*                                           LOCAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       CPU_FlushDCache()
*
* Description : Flush a specific range in the cache memory 
*
* Argument(s) :     addr    the start address of the memory area to flush
*                   len     the size of the memory area to flush
* 
* Return(s)   : none.
*
* Note(s)     : The function uses microblaze_flush_dcache_range() which is part of Xilinx libraries  
*********************************************************************************************************
*/

void  CPU_CacheDataFlush (void       *addr, 
                          CPU_INT32U  len)
{
     microblaze_flush_dcache_range((CPU_INT32S)addr, len);           
}

/*
*********************************************************************************************************
*                                       CPU_InvalidateDCache()
*
* Description : Invalide a specific range in the cache memory
*
* Argument(s) :     addr    the start address of the memory area to invalidate
*                   len     the size of the memory area to invalidate
*
* Return(s)   : none.
*
* Note(s)     : The function uses microblaze_invalidate_dcache_range() which is part of Xilinx libraries  
*********************************************************************************************************
*/
                                              
void  CPU_CacheDataInvalidate (void        *addr,
                               CPU_INT32U   len)
{
     microblaze_invalidate_dcache_range((CPU_INT32S)addr, len);                 
}

#ifdef __cplusplus
}
#endif

