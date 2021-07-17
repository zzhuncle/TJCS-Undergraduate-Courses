/*
 * Copyright (c) 2015, Xilinx Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file    ucos/mutex.h
 * @brief   uC/OS mutex primitives for libmetal.
 */

#ifndef __METAL_MUTEX__H__
#error "Include metal/mutex.h instead of metal/ucos/mutex.h"
#endif

#ifndef __METAL_UCOS_MUTEX__H__
#define __METAL_UCOS_MUTEX__H__

#include <errno.h>
#include <Source/os.h>
#include <metal/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (OS_VERSION >= 30000u)
typedef OS_MUTEX metal_mutex_t;
#else
typedef OS_EVENT *metal_mutex_t;
#endif

/*
 * METAL_MUTEX_INIT - used for initializing an mutex elmenet in a static struct
 * or global
 */
#define METAL_MUTEX_INIT(m)        { ATOMIC_VAR_INIT(0) }
/*
 * METAL_MUTEX_DEFINE - used for defining and initializing a global or
 * static singleton mutex
 */
#define METAL_MUTEX_DEFINE(m) metal_mutex_t m = METAL_MUTEX_INIT(m)

static inline void __metal_mutex_init(metal_mutex_t *mutex)
{
#if (OS_VERSION >= 30000u)
    OS_ERR  err;


    OSMutexCreate((OS_MUTEX *)mutex, (CPU_CHAR *)0, &err);
#else
	INT8U  err;


    *mutex = OSMutexCreate(OS_PRIO_MUTEX_CEIL_DIS, &err);
#endif
}

static inline void __metal_mutex_deinit(metal_mutex_t *mutex)
{
#if (OS_VERSION >= 30000u)
    OS_ERR  err;


    OSMutexDel((OS_MUTEX *)mutex, OS_OPT_DEL_ALWAYS, &err);
#else
    INT8U  err;


    OSMutexDel(*mutex, OS_DEL_ALWAYS, &err);
#endif
}

static inline int __metal_mutex_try_acquire(metal_mutex_t *mutex)
{
#if (OS_VERSION >= 30000u)
    OS_ERR  err;


    OSMutexPend((OS_MUTEX *)mutex, 0, OS_OPT_PEND_NON_BLOCKING, (CPU_TS *)0, &err);

    if ((err == OS_ERR_NONE       ) ||
        (err == OS_ERR_MUTEX_OWNER)) {
        return 1;
    }

    return 0;
#else
    (void)mutex;

    return 0;                                             		/* No function to do this in uCOS-II.                   */
#endif
}

static inline void __metal_mutex_acquire(metal_mutex_t *mutex)
{
#if (OS_VERSION >= 30000u)
    OS_ERR  err;


    OSMutexPend((OS_MUTEX *)mutex, 0, OS_OPT_PEND_BLOCKING, (CPU_TS *)0, &err);
#else
    INT8U  err;


	OSMutexPend((OS_EVENT *)(*mutex), 0, &err);
#endif
}

static inline void __metal_mutex_release(metal_mutex_t *mutex)
{
#if (OS_VERSION >= 30000u)
    OS_ERR  err;


    OSMutexPost((OS_MUTEX *)mutex, OS_OPT_POST_NONE, &err);
#else
    OSMutexPost((OS_EVENT *)(*mutex));
#endif
}

static inline int __metal_mutex_is_acquired(metal_mutex_t *mutex)
{
    int isAcquired;
#if (OS_VERSION >= 30000u)
    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();
    isAcquired = (((OS_MUTEX *)mutex)->OwnerNestingCtr == 0) ? 0 : 1;
    CPU_CRITICAL_EXIT();
#else
    OS_MUTEX_DATA  mutex_data;


	OSMutexQuery ((OS_EVENT *)(*mutex), &mutex_data);

    isAcquired = (mutex_data.OSValue == OS_FALSE) ? 1 : 0;
#endif

    return isAcquired;
}

#ifdef __cplusplus
}
#endif

#endif /* __METAL_UCOS_MUTEX__H__ */
