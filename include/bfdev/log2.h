/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_LOG2_H_
#define _BFDEV_LOG2_H_

#include <bfdev/config.h>
#include <bfdev/bitops.h>

/**
 * bfdev_ilog2_const - log base 2 of long long constant unsigned value.
 * @value: constant value to log base 2.
 */
#define bfdev_ilog2_const(value) (  \
__builtin_constant_p(value) ? (     \
    (value) < 2 ? 0 :               \
    (value) & (1ULL << 63) ? 63 :   \
    (value) & (1ULL << 62) ? 62 :   \
    (value) & (1ULL << 61) ? 61 :   \
    (value) & (1ULL << 60) ? 60 :   \
    (value) & (1ULL << 59) ? 59 :   \
    (value) & (1ULL << 58) ? 58 :   \
    (value) & (1ULL << 57) ? 57 :   \
    (value) & (1ULL << 56) ? 56 :   \
    (value) & (1ULL << 55) ? 55 :   \
    (value) & (1ULL << 54) ? 54 :   \
    (value) & (1ULL << 53) ? 53 :   \
    (value) & (1ULL << 52) ? 52 :   \
    (value) & (1ULL << 51) ? 51 :   \
    (value) & (1ULL << 50) ? 50 :   \
    (value) & (1ULL << 49) ? 49 :   \
    (value) & (1ULL << 48) ? 48 :   \
    (value) & (1ULL << 47) ? 47 :   \
    (value) & (1ULL << 46) ? 46 :   \
    (value) & (1ULL << 45) ? 45 :   \
    (value) & (1ULL << 44) ? 44 :   \
    (value) & (1ULL << 43) ? 43 :   \
    (value) & (1ULL << 42) ? 42 :   \
    (value) & (1ULL << 41) ? 41 :   \
    (value) & (1ULL << 40) ? 40 :   \
    (value) & (1ULL << 39) ? 39 :   \
    (value) & (1ULL << 38) ? 38 :   \
    (value) & (1ULL << 37) ? 37 :   \
    (value) & (1ULL << 36) ? 36 :   \
    (value) & (1ULL << 35) ? 35 :   \
    (value) & (1ULL << 34) ? 34 :   \
    (value) & (1ULL << 33) ? 33 :   \
    (value) & (1ULL << 32) ? 32 :   \
    (value) & (1ULL << 31) ? 31 :   \
    (value) & (1ULL << 30) ? 30 :   \
    (value) & (1ULL << 29) ? 29 :   \
    (value) & (1ULL << 28) ? 28 :   \
    (value) & (1ULL << 27) ? 27 :   \
    (value) & (1ULL << 26) ? 26 :   \
    (value) & (1ULL << 25) ? 25 :   \
    (value) & (1ULL << 24) ? 24 :   \
    (value) & (1ULL << 23) ? 23 :   \
    (value) & (1ULL << 22) ? 22 :   \
    (value) & (1ULL << 21) ? 21 :   \
    (value) & (1ULL << 20) ? 20 :   \
    (value) & (1ULL << 19) ? 19 :   \
    (value) & (1ULL << 18) ? 18 :   \
    (value) & (1ULL << 17) ? 17 :   \
    (value) & (1ULL << 16) ? 16 :   \
    (value) & (1ULL << 15) ? 15 :   \
    (value) & (1ULL << 14) ? 14 :   \
    (value) & (1ULL << 13) ? 13 :   \
    (value) & (1ULL << 12) ? 12 :   \
    (value) & (1ULL << 11) ? 11 :   \
    (value) & (1ULL << 10) ? 10 :   \
    (value) & (1ULL <<  9) ?  9 :   \
    (value) & (1ULL <<  8) ?  8 :   \
    (value) & (1ULL <<  7) ?  7 :   \
    (value) & (1ULL <<  6) ?  6 :   \
    (value) & (1ULL <<  5) ?  5 :   \
    (value) & (1ULL <<  4) ?  4 :   \
    (value) & (1ULL <<  3) ?  3 :   \
    (value) & (1ULL <<  2) ?  2 :   \
    1) : -1                         \
)

/**
 * bfdev_ilog2_dynamic - log base 2 unsigned long value dynamically.
 * @value: dynamic long value to log base 2.
 */
static inline __bfdev_attribute_const unsigned int
bfdev_ilog2_dynamic(unsigned long value)
{
    if (value < 2)
        return 0;
    return bfdev_fls(value) - 1;
}

/**
 * bfdev_ilog2_64_dynamic - log base 2 64-bit unsigned value dynamically.
 * @value: dynamic 64-bit value to log base 2.
 */
static inline __bfdev_attribute_const unsigned int
bfdev_ilog2_64_dynamic(uint64_t value)
{
    if (value < 2)
        return 0;
    return bfdev_fls64(value) - 1;
}

/**
 * bfdev_pow2_roundup_dynamic - round up to nearest power of two dynamically.
 * @value: dynamic value to round up.
 */
static inline unsigned long
bfdev_pow2_roundup_dynamic(unsigned long value)
{
    return 1UL << bfdev_fls(value - 1);
}

/**
 * bfdev_pow2_rounddown_dynamic - round down to nearest power of two dynamically.
 * @value: dynamic value to round up.
 */
static inline unsigned long
bfdev_pow2_rounddown_dynamic(unsigned long value)
{
    return 1UL << (bfdev_fls(value) - 1);
}

/**
 * bfdev_pow2_check - check if a value is a power of two.
 * @value: the value to check.
 */
static inline bool
bfdev_pow2_check(unsigned long value)
{
    return value != 0 && ((value & (value - 1)) == 0);
}

/**
 * bfdev_ilog2 - log base 2 constant unsigned value.
 * @value: value to log2.
 */
#define bfdev_ilog2(value) (            \
    __builtin_constant_p(value) ?       \
    ((value) < 2 ? 0 :                  \
     63 - __builtin_clzll(value)) :     \
    (sizeof(value) <= 4) ?              \
    bfdev_ilog2_dynamic(value) :        \
    bfdev_ilog2_64_dynamic(value)       \
)

/**
 * bfdev_pow2_roundup - round up to nearest power of two.
 * @value: value to round up.
 */
#define bfdev_pow2_roundup(value) (     \
    __builtin_constant_p(value) ?       \
    (((value) == 1) ? 1 : (1UL <<       \
    (bfdev_ilog2((value) - 1) + 1))) :  \
    bfdev_pow2_roundup_dynamic(value)   \
)

/**
 * bfdev_pow2_rounddown - round down to nearest power of two.
 * @value: value to round down.
 */
#define bfdev_pow2_rounddown(value) (   \
    __builtin_constant_p(value) ?       \
    (1UL << bfdev_ilog2(value)) :       \
    bfdev_pow2_rounddown_dynamic(value) \
)

#endif /* _BFDEV_LOG2_H_ */
