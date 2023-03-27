/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_MACRO_H_
#define _BFDEV_MACRO_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ARRAY_SIZE - get the number of elements in array.
 * @arr: array to be sized.
 */
#define ARRAY_SIZE(arr) ( \
    sizeof(arr) / sizeof((arr)[0]) \
)

/**
 * swap - swap values.
 * @a: first value.
 * @b: second value.
 */
#define swap(a, b) ({ \
    typeof(a) _a = (a); \
    (a) = (b); (b) = _a; \
})

/* This counts to 12. Any more, it will return 13th argument. */
#define __COUNT_ARGS(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _n, X...) _n
#define COUNT_ARGS(X...) __COUNT_ARGS(, ##X, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_MACRO_H_ */
