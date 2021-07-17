/*
 * Copyright (c) 2016, Xilinx Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file    ucos/alloc.c
 * @brief   uC/OS libmetal memory allocation definitions.
 */

#ifndef __METAL_ALLOC__H__
#error "Include metal/alloc.h instead of metal/ucos/alloc.h"
#endif

#ifndef __METAL_UCOS_ALLOC__H__
#define __METAL_UCOS_ALLOC__H__

#include <stdlib.h>
#include <Source/os.h>

/* 
 * The use of malloc and free assumes that Thread-Local Storage (TLS) is enabled in uC/OS.
 */
#ifdef __cplusplus
extern "C" {
#endif

static inline void *metal_allocate_memory(unsigned int size)
{
    return (malloc(size));
}

static inline void metal_free_memory(void *ptr)
{
    free(ptr);
}

#ifdef __cplusplus
}
#endif

#endif /* __METAL_UCOS_ALLOC__H__ */
