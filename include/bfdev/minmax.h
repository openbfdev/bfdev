/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_MINMAX_H_
#define _BFDEV_MINMAX_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_min() - return minimum of two values of the same or compatible types.
 * @a: first value.
 * @b: second value.
 */
#define bfdev_min(a, b) ({ \
    typeof(a) __amin = (a); \
    typeof(a) __bmin = (b); \
    __amin < __bmin ? __amin : __bmin; \
})

/**
 * bfdev_max() - return maximum of two values of the same or compatible types.
 * @a: first value.
 * @b: second value.
 */
#define bfdev_max(a, b) ({ \
    typeof(a) __amax = (a); \
    typeof(a) __bmax = (b); \
    __amax > __bmax ? __amax : __bmax; \
})

/**
 * bfdev_clamp() - return a value clamped to a given range with strict typechecking.
 * @val: current value.
 * @lo: lowest allowable value.
 * @hi: highest allowable value.
 */
#define bfdev_clamp(val, lo, hi) ( \
    bfdev_min(bfdev_max(val, lo), hi) \
)

/**
 * bfdev_min_adj() - Adjust the minimum value of @val.
 * @val: value to adjust.
 * @lo: lowest allowable value.
 */
#define bfdev_min_adj(val, lo) ({ \
    (val) = bfdev_min(val, lo); \
})

/**
 * bfdev_max_adj() - Adjust the maximum value of @val.
 * @val: value to adjust.
 * @hi: highest allowable value.
 */
#define bfdev_max_adj(val, hi) ({ \
    (val) = bfdev_max(val, hi); \
})

/**
 * bfdev_clamp_adj() - Adjust the clamped value of @val.
 * @val: value to adjust.
 * @lo: lowest allowable value.
 * @hi: highest allowable value.
 */
#define bfdev_clamp_adj(val, lo, hi) ({ \
    (val) = bfdev_clamp(val, lo, hi); \
})

BFDEV_END_DECLS

#endif /* _BFDEV_MINMAX_H_ */
