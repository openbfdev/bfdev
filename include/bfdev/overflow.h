/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_OVERFLOW_H_
#define _BFDEV_OVERFLOW_H_

#include <bfdev/config.h>
#include <bfdev/stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define bfdev_overflow_check_add(a, b, d) ({    \
    typeof(a) ___a = (a);                       \
    typeof(b) ___b = (b);                       \
    typeof(d) ___d = (d);                       \
    (void) (&___a == &___b);                    \
    (void) (&___a == ___d);                     \
    __builtin_add_overflow(___a, ___b, ___d);   \
})

#define bfdev_overflow_check_sub(a, b, d) ({    \
    typeof(a) ___a = (a);                       \
    typeof(b) ___b = (b);                       \
    typeof(d) ___d = (d);                       \
    (void) (&___a == &___b);                    \
    (void) (&___a == ___d);                     \
    __builtin_sub_overflow(___a, ___b, ___d);   \
})

#define bfdev_overflow_check_mul(a, b, d) ({    \
    typeof(a) ___a = (a);                       \
    typeof(b) ___b = (b);                       \
    typeof(d) ___d = (d);                       \
    (void) (&___a == &___b);                    \
    (void) (&___a == ___d);                     \
    __builtin_mul_overflow(___a, ___b, ___d);   \
})

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

#define bfdev_overflow_add(a, b) bfdev_overflow_add_type(typeof(a), a, b)
#define bfdev_overflow_sub(a, b) bfdev_overflow_sub_type(typeof(a), a, b)
#define bfdev_overflow_mul(a, b) bfdev_overflow_mul_type(typeof(a), a, b)

#define BFDEV_GENERIC_OVERFLOW_OPS(name, type)      \
static __always_inline type                         \
bfdev_overflow_add_##name(type a, type b)           \
{                                                   \
    return bfdev_overflow_add_type(type, a, b);     \
}                                                   \
                                                    \
static __always_inline type                         \
bfdev_overflow_sub_##name(type a, type b)           \
{                                                   \
    return bfdev_overflow_sub_type(type, a, b);     \
}                                                   \
                                                    \
static __always_inline type                         \
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

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_OVERFLOW_H_ */
