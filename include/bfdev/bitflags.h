/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BITFLAGS_H_
#define _BFDEV_BITFLAGS_H_

#include <bfdev/config.h>
#include <bfdev/bitops.h>

BFDEV_BEGIN_DECLS

#define BFDEV_BITFLAGS_STRUCT(name, type, member)                   \
static __bfdev_always_inline void                                   \
name##_##member##_clr(type *ptr, unsigned int bit)                  \
{                                                                   \
    bfdev_bit_clr(&ptr->member, bit);                               \
}                                                                   \
                                                                    \
static __bfdev_always_inline void                                   \
name##_##member##_set(type *ptr, unsigned int bit)                  \
{                                                                   \
    bfdev_bit_set(&ptr->member, bit);                               \
}                                                                   \
                                                                    \
static __bfdev_always_inline void                                   \
name##_##member##_flip(type *ptr, unsigned int bit)                 \
{                                                                   \
    bfdev_bit_flip(&ptr->member, bit);                              \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_##member##_test(const type *ptr, unsigned int bit)           \
{                                                                   \
    return bfdev_bit_test(&ptr->member, bit);                       \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_##member##_test_clr(type *ptr, unsigned int bit)             \
{                                                                   \
    return bfdev_bit_test_clr(&ptr->member, bit);                   \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_##member##_test_set(type *ptr, unsigned int bit)             \
{                                                                   \
    return bfdev_bit_test_set(&ptr->member, bit);                   \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_##member##_test_flip(type *ptr, unsigned int bit)            \
{                                                                   \
    return bfdev_bit_test_flip(&ptr->member, bit);                  \
}

#define BFDEV_BITFLAGS_STRUCT_ATOMIC(name, type, member)            \
static __bfdev_always_inline void                                   \
name##_##member##_atomic_clr(type *ptr, unsigned int bit)           \
{                                                                   \
    bfdev_bit_atomic_clr(&ptr->member, bit);                        \
}                                                                   \
                                                                    \
static __bfdev_always_inline void                                   \
name##_##member##_atomic_set(type *ptr, unsigned int bit)           \
{                                                                   \
    bfdev_bit_atomic_set(&ptr->member, bit);                        \
}                                                                   \
                                                                    \
static __bfdev_always_inline void                                   \
name##_##member##_atomic_flip(type *ptr, unsigned int bit)          \
{                                                                   \
    bfdev_bit_atomic_flip(&ptr->member, bit);                       \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_##member##_atomic_test(const type *ptr, unsigned int bit)    \
{                                                                   \
    return bfdev_bit_atomic_test(&ptr->member, bit);                \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_##member##_atomic_test_clr(type *ptr, unsigned int bit)      \
{                                                                   \
    return bfdev_bit_atomic_test_clr(&ptr->member, bit);            \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_##member##_atomic_test_set(type *ptr, unsigned int bit)      \
{                                                                   \
    return bfdev_bit_atomic_test_set(&ptr->member, bit);            \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_##member##_atomic_test_flip(type *ptr, unsigned int bit)     \
{                                                                   \
    return bfdev_bit_atomic_test_flip(&ptr->member, bit);           \
}

#define BFDEV_BITFLAGS_STRUCT_FLAG(name, type, member,              \
                                   flag, index)                     \
static __bfdev_always_inline void                                   \
name##_clr_##flag(type *ptr)                                        \
{                                                                   \
    name##_##member##_clr(ptr, index);                              \
}                                                                   \
                                                                    \
static __bfdev_always_inline void                                   \
name##_set_##flag(type *ptr)                                        \
{                                                                   \
    name##_##member##_set(ptr, index);                              \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_test_##flag(const type *ptr)                                 \
{                                                                   \
    return name##_##member##_test(ptr, index);                      \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_test_clr_##flag(type *ptr)                                   \
{                                                                   \
    return name##_##member##_test_clr(ptr, index);                  \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_test_set_##flag(type *ptr)                                   \
{                                                                   \
    return name##_##member##_test_set(ptr, index);                  \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_test_flip_##flag(type *ptr)                                  \
{                                                                   \
    return name##_##member##_test_flip(ptr, index);                 \
}

#define BFDEV_BITFLAGS_STRUCT_ATOMIC_FLAG(name, type, member,       \
                                          flag, index)              \
static __bfdev_always_inline void                                   \
name##_atomic_clr_##flag(type *ptr)                                 \
{                                                                   \
    name##_##member##_atomic_clr(ptr, index);                       \
}                                                                   \
                                                                    \
static __bfdev_always_inline void                                   \
name##_atomic_set_##flag(type *ptr)                                 \
{                                                                   \
    name##_##member##_atomic_set(ptr, index);                       \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_atomic_test_##flag(const type *ptr)                          \
{                                                                   \
    return name##_##member##_atomic_test(ptr, index);               \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_atomic_test_clr_##flag(type *ptr)                            \
{                                                                   \
    return name##_##member##_atomic_test_clr(ptr, index);           \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_atomic_test_set_##flag(type *ptr)                            \
{                                                                   \
    return name##_##member##_atomic_test_set(ptr, index);           \
}                                                                   \
                                                                    \
static __bfdev_always_inline bool                                   \
name##_atomic_test_flip_##flag(type *ptr)                           \
{                                                                   \
    return name##_##member##_atomic_test_flip(ptr, index);          \
}

BFDEV_END_DECLS

#endif /* _BFDEV_BITFLAGS_H_ */
