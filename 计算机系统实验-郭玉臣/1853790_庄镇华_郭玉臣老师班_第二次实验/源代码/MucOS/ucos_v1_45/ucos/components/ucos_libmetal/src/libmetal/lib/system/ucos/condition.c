/*
 * Copyright (c) 2016, Xilinx Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file    ucos/condition.c
 * @brief   uC/OS libmetal condition variable handling.
 */

#include <metal/condition.h>

int metal_condition_wait(struct metal_condition *cv,
             metal_mutex_t *m)
{
    (void)cv;
    (void)m;
    
    return -ENOSYS;
}
