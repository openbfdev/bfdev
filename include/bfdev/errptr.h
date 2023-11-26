/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ERRPTR_H_
#define _BFDEV_ERRPTR_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/errno.h>

BFDEV_BEGIN_DECLS

/**
 * BFDEV_ERR_PTR - converts a int to a pointer containing the state.
 * @error: the state to convert.
 */
static inline void *
BFDEV_ERR_PTR(int error)
{
    return (void *)(intptr_t)error;
}

/**
 * BFDEV_PTR_ERR - converts a pointer containing an state to an int.
 * @ptr: the pointer to convert.
 */
static inline int
BFDEV_PTR_ERR(const void *ptr)
{
    return (int)(intptr_t)ptr;
}

/**
 * BFDEV_PTR_INVAL - converts a pointer containing an state to an int or null.
 * @ptr: the pointer to convert.
 */
static inline int
BFDEV_PTR_INVAL(const void *ptr)
{
    return BFDEV_PTR_ERR(ptr) ?: -BFDEV_EPERM;
}

/**
 * BFDEV_IS_ERR - Check whether pointer is error value.
 * @ptr: the pointer to check.
 */
static inline bool
BFDEV_IS_ERR(const void *ptr)
{
    return (uintptr_t)ptr >= (uintptr_t)-BFDEV_ERRNO_MAX;
}

/**
 * BFDEV_IS_INVAL - Check whether pointer is error value or null.
 * @ptr: the pointer to check.
 */
static inline bool
BFDEV_IS_INVAL(const void *ptr)
{
    return bfdev_unlikely(!ptr) || BFDEV_IS_ERR(ptr);
}

/**
 * BFDEV_PTR_ERR_ZERO - Check whether pointer is error value.
 * @ptr: the pointer to check.
 */
static inline int
BFDEV_PTR_ERR_ZERO(const void *ptr)
{
    if (bfdev_unlikely(BFDEV_IS_ERR(ptr)))
        return BFDEV_PTR_ERR(ptr);
    return -BFDEV_ENOERR;
}

/**
 * BFDEV_PTR_INVAL_ZERO - Check whether pointer is error value or null.
 * @ptr: the pointer to check.
 */
static inline int
BFDEV_PTR_INVAL_ZERO(const void *ptr)
{
    if (bfdev_unlikely(BFDEV_IS_INVAL(ptr)))
        return BFDEV_PTR_INVAL(ptr);
    return -BFDEV_ENOERR;
}

BFDEV_END_DECLS

#endif /* _BFDEV_ERRPTR_H_ */
