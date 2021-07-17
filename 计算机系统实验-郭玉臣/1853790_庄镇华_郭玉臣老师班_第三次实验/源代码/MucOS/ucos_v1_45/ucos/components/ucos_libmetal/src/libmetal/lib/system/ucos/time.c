/*
 * Copyright (c) 2016, Xilinx Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file    ucos/time.c
 * @brief   uC/OS libmetal time handling.
 */

#include <metal/time.h>
#include <Source/os.h>

unsigned long long metal_get_timestamp(void)
{
#if (OS_VERSION >= 30000u)
    OS_ERR  os_err;

    return OSTimeGet(&os_err);
#else
    return OSTimeGet();
#endif
}

