/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BITOPS_ENDIAN_H_
#define _BFDEV_BITOPS_ENDIAN_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/bitops.h>
#include <bfdev/byteorder.h>

BFDEV_BEGIN_DECLS

#if defined(__BFDEV_LITTLE_ENDIAN__)
# define BFDEV_BITOPS_LE_SWIZZLE 0
# define BFDEV_BITOPS_BE_SWIZZLE ((BFDEV_BITS_PER_LONG - 1) & ~0x7)
#elif defined(__BFDEV_BIG_ENDIAN__)
# define BFDEV_BITOPS_LE_SWIZZLE ((BFDEV_BITS_PER_LONG - 1) & ~0x7)
# define BFDEV_BITOPS_BE_SWIZZLE 0
#else
# error "Unknown endian"
#endif

static inline void
bfdev_bit_clr_le(void *addr, unsigned int bit)
{
    bfdev_bit_clr(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE);
}

static inline void
bfdev_bit_set_le(void *addr, unsigned int bit)
{
    bfdev_bit_set(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE);
}

static inline void
bfdev_bit_flip_le(void *addr, unsigned int bit)
{
    bfdev_bit_flip(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE);
}

static inline void
bfdev_bit_change_le(void *addr, unsigned int bit, bool val)
{
    bfdev_bit_change(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE, val);
}

static inline bool
bfdev_bit_test_le(const void *addr, unsigned int bit)
{
    return bfdev_bit_test(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE);
}

static inline bool
bfdev_bit_test_clr_le(void *addr, unsigned int bit)
{
    return bfdev_bit_test_clr(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE);
}

static inline bool
bfdev_bit_test_set_le(void *addr, unsigned int bit)
{
    return bfdev_bit_test_set(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE);
}

static inline bool
bfdev_bit_test_flip_le(void *addr, unsigned int bit)
{
    return bfdev_bit_test_flip(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE);
}

static inline void
bfdev_bit_atomic_clr_le(void *addr, unsigned int bit)
{
    bfdev_bit_atomic_clr(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE);
}

static inline void
bfdev_bit_atomic_set_le(void *addr, unsigned int bit)
{
    bfdev_bit_atomic_set(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE);
}

static inline void
bfdev_bit_atomic_flip_le(void *addr, unsigned int bit)
{
    bfdev_bit_atomic_flip(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE);
}

static inline void
bfdev_bit_atomic_change_le(void *addr, unsigned int bit, bool val)
{
    bfdev_bit_atomic_change(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE, val);
}

static inline bool
bfdev_bit_atomic_test_le(const void *addr, unsigned int bit)
{
    return bfdev_bit_atomic_test(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE);
}

static inline bool
bfdev_bit_atomic_test_clr_le(void *addr, unsigned int bit)
{
    return bfdev_bit_atomic_test_clr(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE);
}

static inline bool
bfdev_bit_atomic_test_set_le(void *addr, unsigned int bit)
{
    return bfdev_bit_atomic_test_set(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE);
}

static inline bool
bfdev_bit_atomic_test_flip_le(void *addr, unsigned int bit)
{
    return bfdev_bit_atomic_test_flip(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE);
}

static inline bool
bfdev_bit_atomic_test_change_le(void *addr, unsigned int bit, bool val)
{
    return bfdev_bit_atomic_test_change(addr, bit ^ BFDEV_BITOPS_LE_SWIZZLE, val);
}

static inline void
bfdev_bit_clr_be(void *addr, unsigned int bit)
{
    bfdev_bit_clr(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE);
}

static inline void
bfdev_bit_set_be(void *addr, unsigned int bit)
{
    bfdev_bit_set(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE);
}

static inline void
bfdev_bit_flip_be(void *addr, unsigned int bit)
{
    bfdev_bit_flip(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE);
}

static inline void
bfdev_bit_change_be(void *addr, unsigned int bit, bool val)
{
    bfdev_bit_change(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE, val);
}

static inline bool
bfdev_bit_test_be(const void *addr, unsigned int bit)
{
    return bfdev_bit_test(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE);
}

static inline bool
bfdev_bit_test_clr_be(void *addr, unsigned int bit)
{
    return bfdev_bit_test_clr(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE);
}

static inline bool
bfdev_bit_test_set_be(void *addr, unsigned int bit)
{
    return bfdev_bit_test_set(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE);
}

static inline bool
bfdev_bit_test_flip_be(void *addr, unsigned int bit)
{
    return bfdev_bit_test_flip(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE);
}

static inline bool
bfdev_bit_test_change_be(void *addr, unsigned int bit, bool val)
{
    return bfdev_bit_test_change(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE, val);
}

static inline void
bfdev_bit_atomic_clr_be(void *addr, unsigned int bit)
{
    bfdev_bit_atomic_clr(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE);
}

static inline void
bfdev_bit_atomic_set_be(void *addr, unsigned int bit)
{
    bfdev_bit_atomic_set(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE);
}

static inline void
bfdev_bit_atomic_flip_be(void *addr, unsigned int bit)
{
    bfdev_bit_atomic_flip(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE);
}

static inline void
bfdev_bit_atomic_change_be(void *addr, unsigned int bit, bool val)
{
    bfdev_bit_atomic_change(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE, val);
}

static inline bool
bfdev_bit_atomic_test_be(const void *addr, unsigned int bit)
{
    return bfdev_bit_atomic_test(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE);
}

static inline bool
bfdev_bit_atomic_test_clr_be(void *addr, unsigned int bit)
{
    return bfdev_bit_atomic_test_clr(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE);
}

static inline bool
bfdev_bit_atomic_test_set_be(void *addr, unsigned int bit)
{
    return bfdev_bit_atomic_test_set(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE);
}

static inline bool
bfdev_bit_atomic_test_flip_be(void *addr, unsigned int bit)
{
    return bfdev_bit_atomic_test_flip(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE);
}

static inline bool
bfdev_bit_atomic_test_change_be(void *addr, unsigned int bit, bool val)
{
    return bfdev_bit_atomic_test_change(addr, bit ^ BFDEV_BITOPS_BE_SWIZZLE, val);
}

BFDEV_END_DECLS

#endif /* _BFDEV_BITOPS_ENDIAN_H_ */
