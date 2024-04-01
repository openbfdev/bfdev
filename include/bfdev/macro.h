/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_MACRO_H_
#define _BFDEV_MACRO_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_swap() - swap values.
 * @a: first value.
 * @b: second value.
 */
#define bfdev_swap(a, b) ({ \
    typeof(a) __a = (a); \
    (a) = (b); (b) = __a; \
})

/**
 * bfdev_cmp() - compare values.
 * @cond: compare condition.
 *
 * If the @cond is met, return an positive number;
 * otherwise, return a negative number.
 */
#define bfdev_cmp(cond) ( \
    (cond) ? BFDEV_BT : BFDEV_LT \
)

/**
 * BFDEV_ARRAY_SIZE() - get the number of elements in array.
 * @arr: array to be sized.
 */
#define BFDEV_ARRAY_SIZE(arr) ( \
    sizeof(arr) / sizeof((arr)[0]) \
)

/* This counts to 12. Any more, it will return 13th argument. */
#define __BFDEV_COUNT_ARGS(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _n, X...) _n
#define BFDEV_COUNT_ARGS(X...) __BFDEV_COUNT_ARGS(, ##X, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

BFDEV_END_DECLS

#endif /* _BFDEV_MACRO_H_ */
