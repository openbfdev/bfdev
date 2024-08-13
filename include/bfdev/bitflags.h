/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BITFLAGS_H_
#define _BFDEV_BITFLAGS_H_

#include <bfdev/config.h>
#include <bfdev/bitops.h>

BFDEV_BEGIN_DECLS

#define BFDEV_GENERIC_BITFLAGS(name, func, type, member, index)     \
static __bfdev_always_inline void                                   \
name##_clr(type *ptr)                                               \
{                                                                   \
    bfdev_bit##func##_clr(member, index);                           \
}                                                                   \
                                                                    \
static __bfdev_always_inline void                                   \
name##_set(type *ptr)                                               \
{                                                                   \
    bfdev_bit##func##_set(member, index);                           \
}                                                                   \
                                                                    \
static __bfdev_always_inline void                                   \
name##_flip(type *ptr)                                              \
{                                                                   \
    bfdev_bit##func##_flip(member, index);                          \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_test(const type *ptr)                                        \
{                                                                   \
    return bfdev_bit##func##_test(member, index);                   \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_test_clr(type *ptr)                                          \
{                                                                   \
    return bfdev_bit##func##_test_clr(member, index);               \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_test_set(type *ptr)                                          \
{                                                                   \
    return bfdev_bit##func##_test_set(member, index);               \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_test_flip(type *ptr)                                         \
{                                                                   \
    return bfdev_bit##func##_test_flip(member, index);              \
}

#define BFDEV_BITFLAGS(name, index) \
    BFDEV_GENERIC_BITFLAGS(name, /* None */, unsigned long, ptr, index)

#define BFDEV_BITFLAGS_ATOMIC(name, index) \
    BFDEV_GENERIC_BITFLAGS(name, _atomic, unsigned long, ptr, index)

#define BFDEV_BITFLAGS_STRUCT(name, type, member, index) \
    BFDEV_GENERIC_BITFLAGS(name, /* None */, type, &ptr->member, index)

#define BFDEV_BITFLAGS_STRUCT_ATOMIC(name, type, member, index) \
    BFDEV_GENERIC_BITFLAGS(name, _atomic, type, &ptr->member, index)

BFDEV_END_DECLS

#endif /* _BFDEV_BITFLAGS_H_ */
