/*
 * Copyright (c) 2018, Linaro Limited. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file    ucos/sleep.h
 * @brief    uC/OS sleep primitives for libmetal.
 */

#ifndef __METAL_SLEEP__H__
#error "Include metal/sleep.h instead of metal/ucos/sleep.h"
#endif

#ifndef __METAL_UCOS_SLEEP__H__
#define __METAL_UCOS_SLEEP__H__

#include <stdint.h>
#include <Source/os.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (OS_VERSION >= 30000u)
#define  UCOS_TICK_RATE     OSCfg_TickRate_Hz
#define  UCOS_TICK_TYPE     OS_TICK
#else
#define  UCOS_TICK_RATE     OS_TICKS_PER_SEC
#define  UCOS_TICK_TYPE     INT32U
#endif

static inline int __metal_sleep_usec(unsigned int usec)
{
    int      ret;

#if (OS_VERSION >= 30000u)
    OS_ERR   os_err;
    OS_TICK  ticks;
#else
    INT32U   ticks;
#endif


    ticks = (UCOS_TICK_TYPE)(((uint64_t)usec * UCOS_TICK_RATE) / 1000000);  /* Convert microseconds to kernel ticks.                */

    if (((uint64_t)usec * UCOS_TICK_RATE) % 1000000) {
        ticks++;                                                /* Add a tick if we truncated any time.                 */
    }

#if (OS_VERSION >= 30000u)
    OSTimeDly(ticks,
              OS_OPT_TIME_DLY,
              &os_err);

    ret = (os_err == OS_ERR_NONE) ? 0 : -1;
#else
    OSTimeDly(ticks);
    ret = 0;
#endif

    return ret;
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __METAL_UCOS_SLEEP__H__ */
