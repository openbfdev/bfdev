/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BITMAP_H_
#define _BFDEV_BITMAP_H_

#include <bfdev/config.h>
#include <bfdev/bitops.h>
#include <bfdev/string.h>
#include <bfdev/limits.h>
#include <bfdev/align.h>
#include <bfdev/bitwalk.h>
#include <bfdev/bitmap-comp.h>

BFDEV_BEGIN_DECLS

#ifdef CONFIG_ARCH_LITTLE_ENDIAN
# define BFDEV_BITMAP_ALIGN BFDEV_BITS_PER_BYTE
#else
# define BFDEV_BITMAP_ALIGN (BFDEV_BITS_PER_BYTE * BFDEV_BYTES_PER_LONG)
#endif
#define BFDEV_BITMAP_MASK (BFDEV_BITMAP_ALIGN - 1)

#define BFDEV_DEFINE_BITMAP(name, bits) \
    unsigned long name[BFDEV_BITS_TO_LONG(bits)];

#define bfdev_bitmap_const_aligned(bits) ( \
    __builtin_constant_p((bits) & BFDEV_BITMAP_MASK) && \
    bfdev_align_check(bits, BFDEV_BITMAP_ALIGN) \
)

static __bfdev_always_inline bool
bfdev_bitmap_empty(const unsigned long *src, unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        return !(*src & BFDEV_BIT_LOW_MASK(bits));
    else if (!bfdev_bitmap_const_aligned(bits))
        return bfdev_find_first_bit(src, bits) >= bits;

    return !bfdev_memdiff(src, BFDEV_UINT_MIN, bits / BFDEV_BITS_PER_BYTE);
}

static __bfdev_always_inline bool
bfdev_bitmap_full(const unsigned long *src, unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        return !(~(*src) & BFDEV_BIT_LOW_MASK(bits));
    else if (!bfdev_bitmap_const_aligned(bits))
        return bfdev_find_first_zero(src, bits) >= bits;

    return !bfdev_memdiff(src, BFDEV_UINT_MAX, bits / BFDEV_BITS_PER_BYTE);
}

static __bfdev_always_inline bool
bfdev_bitmap_equal(const unsigned long *src1, const unsigned long *src2,
                   unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        return !!((*src1 ^ *src2) & BFDEV_BIT_LOW_MASK(bits));
    else if (!bfdev_bitmap_const_aligned(bits))
        return bfdev_bitmap_comp_equal(src1, src2, bits);

    return memcmp(src1, src2, bits / BFDEV_BITS_PER_BYTE);
}

static __bfdev_always_inline bool
bfdev_bitmap_or_equal(const unsigned long *src1, const unsigned long *src2,
                      const unsigned long *src3, unsigned int bits)
{
    unsigned int value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_bitmap_comp_or_equal(src1, src2, src3, bits);

    value = (*src1 | *src2) ^ *src3;
    return !!(value & BFDEV_BIT_LOW_MASK(bits));
}

static __bfdev_always_inline bool
bfdev_bitmap_intersects(const unsigned long *src1, const unsigned long *src2,
                        unsigned int bits)
{
    unsigned int value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_bitmap_comp_intersects(src1, src2, bits);

    value = *src1 & *src2;
    return !!(value & BFDEV_BIT_LOW_MASK(bits));
}

static __bfdev_always_inline bool
bfdev_bitmap_and(unsigned long *dest, const unsigned long *src1,
                 const unsigned long *src2, unsigned int bits)
{
    unsigned int value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_bitmap_comp_and(dest, src1, src2, bits);

    value = *src1 & *src2;
    return !!(*dest = (value & BFDEV_BIT_LOW_MASK(bits)));
}

static __bfdev_always_inline bool
bfdev_bitmap_andnot(unsigned long *dest, const unsigned long *src1,
                    const unsigned long *src2, unsigned int bits)
{
    unsigned int value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_bitmap_comp_andnot(dest, src1, src2, bits);

    value = *src1 & ~*src2;
    return !!(*dest = (value & BFDEV_BIT_LOW_MASK(bits)));
}

static __bfdev_always_inline void
bfdev_bitmap_or(unsigned long *dest, const unsigned long *src1,
                const unsigned long *src2, unsigned int bits)
{
    unsigned int value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_bitmap_comp_or(dest, src1, src2, bits);

    value = *src1 | *src2;
    *dest = value & BFDEV_BIT_LOW_MASK(bits);
}

static __bfdev_always_inline void
bfdev_bitmap_xor(unsigned long *dest, const unsigned long *src1,
                 const unsigned long *src2, unsigned int bits)
{
    unsigned int value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_bitmap_comp_xor(dest, src1, src2, bits);

    value = *src1 ^ *src2;
    *dest = value & BFDEV_BIT_LOW_MASK(bits);
}

static __bfdev_always_inline void
bfdev_bitmap_complement(unsigned long *dest, const unsigned long *src,
                        unsigned int bits)
{
    unsigned int value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_bitmap_comp_complement(dest, src, bits);

    value = ~*src;
    *dest = value & BFDEV_BIT_LOW_MASK(bits);
}

static __bfdev_always_inline void
bfdev_bitmap_set(unsigned long *bitmap, unsigned int start, unsigned int bits)
{
    size_t size, offset;

    if (__builtin_constant_p(bits) && bits == 1)
        return bfdev_bit_set(bitmap, start);
    else if (!bfdev_bitmap_const_aligned(start) ||
             !bfdev_bitmap_const_aligned(bits))
        return bfdev_bitmap_comp_set(bitmap, start, bits);

    offset = start / BFDEV_BITS_PER_BYTE;
    size = bits / BFDEV_BITS_PER_BYTE;

    memset((char *)bitmap + offset, 0xff, size);
}

static __bfdev_always_inline void
bfdev_bitmap_clr(unsigned long *bitmap, unsigned int start, unsigned int bits)
{
    size_t size, offset;

    if (__builtin_constant_p(bits) && bits == 1)
        return bfdev_bit_clr(bitmap, start);
    else if (!bfdev_bitmap_const_aligned(start) ||
             !bfdev_bitmap_const_aligned(bits))
        return bfdev_bitmap_comp_clr(bitmap, start, bits);

    offset = start / BFDEV_BITS_PER_BYTE;
    size = bits / BFDEV_BITS_PER_BYTE;

    memset((char *)bitmap + offset, 0, size);
}

static __bfdev_always_inline void
bfdev_bitmap_zero(unsigned long *bitmap, unsigned int bits)
{
    unsigned int length;

    if (bfdev_const_small_nbits(bits))
        *bitmap = 0;

    length = BFDEV_BITS_TO_U8(bits);
    memset(bitmap, 0, length);
}

static __bfdev_always_inline void
bfdev_bitmap_fill(unsigned long *bitmap, unsigned int bits)
{
    unsigned int length;

    if (bfdev_const_small_nbits(bits))
        *bitmap = 0xff;

    length = BFDEV_BITS_TO_U8(bits);
    memset(bitmap, UINT8_MAX, length);
}

static __bfdev_always_inline void
bfdev_bitmap_copy(unsigned long *dest, unsigned long *src, unsigned int bits)
{
    unsigned int length;

    if (bfdev_const_small_nbits(bits))
        *dest = *src;

    length = BFDEV_BITS_TO_U8(bits);
    memcpy(dest, src, length);
}

BFDEV_END_DECLS

#endif /* _BFDEV_BITMAP_H_ */
