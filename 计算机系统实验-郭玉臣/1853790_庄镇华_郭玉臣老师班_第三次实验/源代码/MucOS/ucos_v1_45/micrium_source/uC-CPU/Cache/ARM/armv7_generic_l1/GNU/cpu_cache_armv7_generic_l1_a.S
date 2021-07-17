@********************************************************************************************************
@                                                uC/CPU
@                                    CPU CONFIGURATION & PORT LAYER
@
@                         (c) Copyright 2004-2019; Silicon Laboratories Inc.,
@                                400 W. Cesar Chavez, Austin, TX 78701
@
@                   All rights reserved. Protected by international copyright laws.
@
@                  Your use of this software is subject to your acceptance of the terms
@                  of a Silicon Labs Micrium software license, which can be obtained by
@                  contacting info@micrium.com. If you do not agree to the terms of this
@                  license, you may not use this software.
@
@                  Please help us continue to provide the Embedded community with the finest
@                  software available. Your honesty is greatly appreciated.
@
@                    You can find our product's documentation at: doc.micrium.com
@
@                          For more information visit us at: www.micrium.com
@********************************************************************************************************

@********************************************************************************************************
@
@                                       CPU CACHE IMPLEMENTATION
@                                        ARMv7 Generic L1 Cache
@                                            GNU C Compiler
@
@ Filename : cpu_cache_armv7_generic_l1_a.S
@ Version  : V1.31.05
@********************************************************************************************************

@********************************************************************************************************
@                                           MACROS AND DEFINIITIONS
@********************************************************************************************************


    .code 32


.equ CPU_CACHE_L2C_REG7_CACHE_SYNC,      0x730
.equ CPU_CACHE_L2C_REG7_CACHE_INV_PA,    0x770
.equ CPU_CACHE_L2C_REG7_CACHE_INV_WAY,   0x77C
.equ CPU_CACHE_L2C_REG7_CACHE_CLEAN_PA,  0x7B0


@********************************************************************************************************
@                                           CPU_DCache_LineSizeGet()
@
@ Description : Returns the cache line size.
@
@ Prototypes  : void  CPU_DCache_LineSizeGet (void)
@
@ Argument(s) : none.
@********************************************************************************************************

.global CPU_DCache_LineSizeGet

CPU_DCache_LineSizeGet:

    MRC     p15, 0, r0, c0, c0, 1
    AND     r0, r0, #0xF0000
    LSR     r0, r0, #16
    MOV     r1, #1
    LSL     r1, r1, r0
    LSL     r0, r1, #2

    BX      lr


@********************************************************************************************************
@                                      INVALIDATE DATA CACHE RANGE
@
@ Description : Invalidate a range of data cache by MVA.
@
@ Prototypes  : void  CPU_DCache_RangeInv  (void       *p_mem,
@                                           CPU_SIZE_T  range);
@
@ Argument(s) : p_mem    Start address of the region to invalidate.
@
@               range    Size of the region to invalidate in bytes.
@
@ Note(s)     : none.
@********************************************************************************************************

.global CPU_DCache_RangeInv

CPU_DCache_RangeInv:
    CMP  r1, #0
    BEQ  CPU_DCache_RangeInv_END

    DSB
    MOVW r2, #:lower16:CPU_Cache_Linesize
    MOVT r2, #:upper16:CPU_Cache_Linesize
    LDR  r2, [r2]
    SUB  r3, r2, #1
    ADD  r1, r1, r0
    BIC  r0, r0, r3

CPU_DCache_RangeInvL1:
    MCR  p15,0, r0, c7, c6, 1
    ADD  r0, r0, r2
    CMP  r0, r1
    BLT  CPU_DCache_RangeInvL1
    DSB

CPU_DCache_RangeInv_END:
    BX LR


@********************************************************************************************************
@                                       FLUSH DATA CACHE RANGE
@
@ Description : Flush (clean) a range of data cache by MVA.
@
@ Prototypes  : void  CPU_DCache_RangeFlush  (void       *p_mem,
@                                             CPU_SIZE_T  range)@
@
@ Argument(s) : p_mem    Start address of the region to flush.
@
@               range    Size of the region to invalidate in bytes.
@
@ Note(s)     : none.
@********************************************************************************************************

.global CPU_DCache_RangeFlush

CPU_DCache_RangeFlush:
    CMP  r1, #0
    BEQ  CPU_DCache_RangeFlush_END

    DSB
    MOVW r2, #:lower16:CPU_Cache_Linesize
    MOVT r2, #:upper16:CPU_Cache_Linesize
    LDR  r2, [r2]
    SUB  r3, r2, #1
    ADD  r1, r1, r0
    BIC  r0, r0, r3

CPU_DCache_RangeFlushL1:
    MCR  p15, 0, r0, c7, c10, 1
    ADD  r0, r0, r2
    CMP  r0, r1
    BLT  CPU_DCache_RangeFlushL1
    DSB

CPU_DCache_RangeFlush_END:
    BX LR

