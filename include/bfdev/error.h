/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ERROR_H_
#define _BFDEV_ERROR_H_

#include "stdint.h"
#include "stdbool.h"
#include "compiler.h"
#include "errno.h"

/**
 * ERR_PTR - converts a state to a pointer containing the state.
 * @err: the state to convert.
 */
static inline void *ERR_PTR(int err)
{
    return (void *)(intptr_t)err;
}

/**
 * PTR_ERR - converts a pointer containing an state to an state.
 * @ptr: the pointer to convert.
 */
static inline int PTR_ERR(const void *ptr)
{
    return (int)(intptr_t)ptr;
}

/**
 * PTR_INVAL - converts a pointer containing an state to an state or null.
 * @ptr: the pointer to convert.
 */
static inline int PTR_INVAL(const void *ptr)
{
    return PTR_ERR(ptr) ?: -BFDEV_EPERM;
}

/**
 * IS_ERR - Check whether pointer is error value.
 * @ptr: the pointer to check.
 */
static inline bool IS_ERR(const void *ptr)
{
    return (uintptr_t)ptr >= (uintptr_t)-BFDEV_ERRNO_MAX;
}

/**
 * IS_INVAL - Check whether pointer is error value or null.
 * @ptr: the pointer to check.
 */
static inline bool IS_INVAL(const void *ptr)
{
    return unlikely(!ptr) || IS_ERR(ptr);
}

/**
 * PTR_ERR_ZERO - Check whether pointer is error value.
 * @ptr: the pointer to check.
 */
static inline int PTR_ERR_ZERO(const void *ptr)
{
    return unlikely(IS_ERR(ptr)) ? PTR_ERR(ptr) : -BFDEV_ENOERR;
}

/**
 * PTR_INVAL_ZERO - Check whether pointer is error value or null.
 * @ptr: the pointer to check.
 */
static inline int PTR_INVAL_ZERO(const void *ptr)
{
    return unlikely(IS_INVAL(ptr)) ? PTR_INVAL(ptr) : -BFDEV_ENOERR;
}

#endif /* _BFDEV_ERROR_H_ */
