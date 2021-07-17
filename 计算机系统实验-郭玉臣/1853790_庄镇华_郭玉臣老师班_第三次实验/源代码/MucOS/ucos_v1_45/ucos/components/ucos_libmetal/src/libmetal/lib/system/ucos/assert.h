/*
 * Copyright (c) 2018, Xilinx Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file    assert.h
 * @brief    uC/OS assertion support.
 */

#ifndef __METAL_ASSERT__H__
#error "Include metal/assert.h instead of metal/ucos/assert.h"
#endif

#ifndef __METAL_UCOS_ASSERT__H__
#define __METAL_UCOS_ASSERT__H__

#include <assert.h>

/**
 * @brief Assertion macro for uC/OS applications.
 * @param cond Condition to evaluate.
 */
#define metal_sys_assert(cond) assert(cond)

#endif /* __METAL_UCOS_ASSERT__H__ */

