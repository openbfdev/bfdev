/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_TIME_H_
#define _BFDEV_TIME_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/macro.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_time_compare - compares two bfdev_time_t variables.
 * @va: variables a.
 * @vb: variables b.
 */
static inline int
bfdev_time_compare(const bfdev_time_t va, const bfdev_time_t vb)
{
    if (va == vb)
        return BFDEV_EQ;

    return bfdev_cmp(va > vb);
}

/**
 * bfdev_time_after - compare if a time value is bigger than another one.
 * @va: the bigger one.
 * @vb: the smaller one.
 */
static inline bool
bfdev_time_after(const bfdev_time_t va, const bfdev_time_t vb)
{
    return bfdev_time_compare(va, vb) > 0;
}

/**
 * bfdev_time_before - compare if a time value is smaller than another one.
 * @va: the smaller one.
 * @vb: the bigger one.
 */
static inline bool
bfdev_time_before(const bfdev_time_t va, const bfdev_time_t vb)
{
    return bfdev_time_compare(va, vb) < 0;
}

/**
 * bfdev_time_after_equal() - compare if a time value is bigger or equal than another one.
 * @va: the bigger one.
 * @vb: the smaller one.
 */
static inline bool
bfdev_time_after_equal(const bfdev_time_t va, const bfdev_time_t vb)
{
    return bfdev_time_compare(va, vb) >= 0;
}

/**
 * bfdev_time_before_equal - compare if a time value is smaller than or equal another one.
 * @va: the smaller one.
 * @vb: the bigger one.
 */
static inline bool
bfdev_time_before_equal(const bfdev_time_t va, const bfdev_time_t vb)
{
    return bfdev_time_compare(va, vb) <= 0;
}

BFDEV_END_DECLS

#endif /* _BFDEV_TIME_H_ */
