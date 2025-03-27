/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_MATH_H_
#define _BFDEV_MATH_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

/*
 * This looks more complex than it should be. But we need to
 * get the type for the ~ right in round_down (it needs to be
 * as wide as the result!), and we want to evaluate the macro
 * arguments just once each.
 */
#define bfdev_round_mask(x, y) ( \
    (typeof(x))((y) - 1) \
)

/**
 * bfdev_round_up - round up to next specified power of 2.
 * @x: the value to round.
 * @y: multiple to round up to (must be a power of 2).
 */
#define bfdev_round_up(x, y) ( \
    (((x) - 1) | bfdev_round_mask(x, y)) + 1 \
)

/**
 * bfdev_round_down - round down to next specified power of 2.
 * @x: the value to round.
 * @y: multiple to round down to (must be a power of 2).
 */
#define bfdev_round_down(x, y) ( \
    (x) & ~bfdev_round_mask(x, y) \
)

#define bfdev_abs_expr(var, type, other) __builtin_choose_expr( \
    __builtin_types_compatible_p(typeof(var), signed type) ||   \
    __builtin_types_compatible_p(typeof(var), unsigned type),   \
    ({                                                          \
        signed type __var;                                      \
        __var = (signed type)(var);                             \
        __var < 0 ? - __var : __var;                            \
    }), other                                                   \
)

/**
 * bfdev_abs - return absolute value of an argument.
 *
 * If it is unsigned type, it is converted to signed type first.
 * char is treated as if it was signed but the macro's
 * return type is preserved as char.
 */
#define bfdev_abs(var)                          \
    bfdev_abs_expr(var, long long,              \
    bfdev_abs_expr(var, long,                   \
    bfdev_abs_expr(var, int,                    \
    bfdev_abs_expr(var, short,                  \
    bfdev_abs_expr(var, char,                   \
        __builtin_choose_expr(                  \
        __builtin_types_compatible_p(           \
            typeof(var), char),                 \
            (char) ({                           \
                signed char __var;              \
                __var = (signed char)(var);     \
                __var < 0 ? - __var : __var;    \
            }),                                 \
            ((void)0)                           \
        )                                       \
    ))))                                        \
)

/**
 * bfdev_abs_diff - return absolute value of the difference between the args.
 * @var1: the first argument.
 * @var2: the second argument.
 */
#define bfdev_abs_diff(var1, var2) ({   \
    typeof(var1) __var1, __var2;        \
                                        \
    __var1 = (typeof(var1))(var1);      \
    __var2 = (typeof(var1))(var2);      \
                                        \
    __var1 > __var2                     \
        ? (__var1 - __var2)             \
        : (__var2 - __var1);            \
})

/**
 * BFDEV_MULT_FRAC
 *
 * Calculate "value * numer / denom"
 * without unnecessary overflow or loss of precision.
 */
#define BFDEV_MULT_FRAC(value, numer, denom) ({ \
    typeof(value) __value, __numer, __denom;    \
    typeof(value) __quot, __rem;                \
                                                \
    __value = (typeof(value))(value);           \
    __numer = (typeof(value))(numer);           \
    __denom = (typeof(value))(denom);           \
                                                \
    __quot = (__value) / (__denom);             \
    __rem = (__value) % (__denom);              \
                                                \
    (__quot * (__numer)) +                      \
        ((__rem * (__numer)) / (__denom));      \
})

/**
 * BFDEV_DIV_ROUND_UP - round up division.
 * @n: divisor number.
 * @d: dividend number.
 */
#define BFDEV_DIV_ROUND_UP(n, d) ( \
    ((n) + (d) - 1) / (d) \
)

/*
 * Divide positive or negative dividend by positive or negative divisor
 * and round to closest integer. Result is undefined for negative
 * divisors if the dividend variable type is unsigned and for negative
 * dividends if the divisor variable type is unsigned.
 */
#define BFDEV_DIV_ROUND_CLOSEST(x, divisor) ({  \
    typeof(x) __x = x;                          \
    typeof(divisor) __d = divisor;              \
    (bfdev_is_unsigned(x) ||                    \
     bfdev_is_unsigned(divisor) ||              \
     (((__x) > 0) == ((__d) > 0)))              \
        ? (((__x) + ((__d) / 2)) / (__d))       \
        : (((__x) - ((__d) / 2)) / (__d));      \
})

BFDEV_END_DECLS

#endif /* _BFDEV_MATH_H_ */
