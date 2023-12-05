/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_OVERFLOW_H_
#define _BFDEV_OVERFLOW_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_overflow_check_add() - Calculate addition with overflow checking.
 * @a: first addend.
 * @b: second addend.
 * @d: pointer to store sum.
 *
 * Returns 0 on success.
 */
#define bfdev_overflow_check_add(a, b, d)       \
bfdev_overflow_check(({                         \
    typeof(a) ___a = (a);                       \
    typeof(b) ___b = (b);                       \
    typeof(d) ___d = (d);                       \
    (void) (&___a == &___b);                    \
    (void) (&___a == ___d);                     \
    __builtin_add_overflow(___a, ___b, ___d);   \
}))

/**
 * bfdev_overflow_check_sub() - Calculate subtraction with overflow checking.
 * @a: minuend; value to subtract from.
 * @b: subtrahend; value to subtract from @a.
 * @d: pointer to store difference.
 *
 * Returns 0 on success.
 */
#define bfdev_overflow_check_sub(a, b, d)       \
bfdev_overflow_check(({                         \
    typeof(a) ___a = (a);                       \
    typeof(b) ___b = (b);                       \
    typeof(d) ___d = (d);                       \
    (void) (&___a == &___b);                    \
    (void) (&___a == ___d);                     \
    __builtin_sub_overflow(___a, ___b, ___d);   \
}))

/**
 * bfdev_overflow_check_mul() - Calculate multiplication with overflow checking.
 * @a: first factor.
 * @b: second factor.
 * @d: pointer to store product.
 *
 * Returns 0 on success.
 */
#define bfdev_overflow_check_mul(a, b, d)       \
bfdev_overflow_check(({                         \
    typeof(a) ___a = (a);                       \
    typeof(b) ___b = (b);                       \
    typeof(d) ___d = (d);                       \
    (void) (&___a == &___b);                    \
    (void) (&___a == ___d);                     \
    __builtin_mul_overflow(___a, ___b, ___d);   \
}))

#define bfdev_overflow_add_type(type, a, b) ({  \
    type __a = (type)(a);                       \
    type __b = (type)(b);                       \
    type __d;                                   \
    bfdev_overflow_check_add(__a, __b, &__d)    \
    ? (type)~0ULL : __d;                        \
})

#define bfdev_overflow_sub_type(type, a, b) ({  \
    type __a = (type)(a);                       \
    type __b = (type)(b);                       \
    type __d;                                   \
    bfdev_overflow_check_sub(__a, __b, &__d)    \
    ? (type)~0ULL : __d;                        \
})

#define bfdev_overflow_mul_type(type, a, b) ({  \
    type __a = (type)(a);                       \
    type __b = (type)(b);                       \
    type __d;                                   \
    bfdev_overflow_check_mul(__a, __b, &__d)    \
    ? (type)~0ULL : __d;                        \
})

static inline __bfdev_must_check bool
bfdev_overflow_check(bool overflow)
{
    return bfdev_unlikely(overflow);
}

/**
 * bfdev_overflow_add() - Calculate type addition with saturation.
 * @a: first addend.
 * @b: second addend.
 *
 * Returns (type)~0ULL on failed.
 */
#define bfdev_overflow_add(a, b) \
    bfdev_overflow_add_type(typeof(a), a, b)

/**
 * bfdev_overflow_sub() - Calculate type subtraction with saturation.
 * @a: minuend; value to subtract from.
 * @b: subtrahend; value to subtract from @a.
 *
 * Returns (type)~0ULL on failed.
 */
#define bfdev_overflow_sub(a, b) \
    bfdev_overflow_sub_type(typeof(a), a, b)

/**
 * bfdev_overflow_mul() - Calculate type multiplication with saturation.
 * @a: first factor.
 * @b: second factor.
 *
 * Returns (type)~0ULL on failed.
 */
#define bfdev_overflow_mul(a, b) \
    bfdev_overflow_mul_type(typeof(a), a, b)

#define BFDEV_GENERIC_OVERFLOW_OPS(name, type)      \
static __bfdev_always_inline type                   \
bfdev_overflow_add_##name(type a, type b)           \
{                                                   \
    return bfdev_overflow_add_type(type, a, b);     \
}                                                   \
                                                    \
static __bfdev_always_inline type                   \
bfdev_overflow_sub_##name(type a, type b)           \
{                                                   \
    return bfdev_overflow_sub_type(type, a, b);     \
}                                                   \
                                                    \
static __bfdev_always_inline type                   \
bfdev_overflow_mul_##name(type a, type b)           \
{                                                   \
    return bfdev_overflow_mul_type(type, a, b);     \
}

BFDEV_GENERIC_OVERFLOW_OPS(s8, int8_t)
BFDEV_GENERIC_OVERFLOW_OPS(s16, int16_t)
BFDEV_GENERIC_OVERFLOW_OPS(s32, int32_t)
BFDEV_GENERIC_OVERFLOW_OPS(s64, int64_t)

BFDEV_GENERIC_OVERFLOW_OPS(u8, uint8_t)
BFDEV_GENERIC_OVERFLOW_OPS(u16, uint16_t)
BFDEV_GENERIC_OVERFLOW_OPS(u32, uint32_t)
BFDEV_GENERIC_OVERFLOW_OPS(u64, uint64_t)

BFDEV_END_DECLS

#endif /* _BFDEV_OVERFLOW_H_ */
