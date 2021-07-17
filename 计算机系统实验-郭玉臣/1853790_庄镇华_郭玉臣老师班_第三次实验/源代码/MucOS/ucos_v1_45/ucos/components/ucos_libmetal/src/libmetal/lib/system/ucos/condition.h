/*
 * Copyright (c) 2016, Xilinx Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file    ucos/condition.h
 * @brief   uC/OS condition variable primitives for libmetal.
 */

#ifndef __METAL_CONDITION__H__
#error "Include metal/condition.h instead of metal/ucos/condition.h"
#endif

#ifndef __METAL_UCOS_CONDITION__H__
#define __METAL_UCOS_CONDITION__H__

#include <metal/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

struct metal_condition {
    metal_mutex_t *m; /**< mutex.
                           The condition variable is attached to
                           this mutex when it is waiting.
                           It is also used to check correctness
                           in case there are multiple waiters. */

    atomic_int v; /**< condition variable value. */
};

/** Static metal condition variable initialization. */
#define METAL_CONDITION_INIT        { NULL, ATOMIC_VAR_INIT(0) }

static inline void metal_condition_init(struct metal_condition *cv)
{
    (void)cv;
    return;
}

static inline int metal_condition_signal(struct metal_condition *cv)
{
    (void)cv;
    return -ENOSYS;
}

static inline int metal_condition_broadcast(struct metal_condition *cv)
{
    (void)cv;
    return -ENOSYS;
}


#ifdef __cplusplus
}
#endif

#endif /* __METAL_UCOS_CONDITION__H__ */
