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
#include <bfdev/allocator.h>
#include <bfdev/bitwalk.h>

BFDEV_BEGIN_DECLS

#ifdef CONFIG_ARCH_LITTLE_ENDIAN
# define BFDEV_BITMAP_MEM_ALIGNMENT BFDEV_BITS_PER_BYTE
#else
# define BFDEV_BITMAP_MEM_ALIGNMENT (BFDEV_BITS_PER_BYTE * BFDEV_BYTES_PER_LONG)
#endif
#define BFDEV_BITMAP_MEM_MASK (BFDEV_BITMAP_MEM_ALIGNMENT - 1)

#define BFDEV_DEFINE_BITMAP(name, bits) \
    unsigned long name[BFDEV_BITS_TO_LONG(bits)];

extern bool
bfdev_bitmap_comp_equal(const unsigned long *src1, const unsigned long *src2,
                        unsigned int bits);

extern bool
bfdev_bitmap_comp_or_equal(const unsigned long *src1, const unsigned long *src2,
                           const unsigned long *src3, unsigned int bits);

extern bool
bfdev_bitmap_comp_intersects(const unsigned long *src1, const unsigned long *src2,
                             unsigned int bits);

extern bool
bfdev_bitmap_comp_and(unsigned long *dest, const unsigned long *src1,
                      const unsigned long *src2, unsigned int bits);

extern bool
bfdev_bitmap_comp_andnot(unsigned long *dest, const unsigned long *src1,
                         const unsigned long *src2, unsigned int bits);

extern void
bfdev_bitmap_comp_or(unsigned long *dest, const unsigned long *src1,
                     const unsigned long *src2, unsigned int bits);

extern void
bfdev_bitmap_comp_xor(unsigned long *dest, const unsigned long *src1,
                      const unsigned long *src2, unsigned int bits);

extern void
bfdev_bitmap_comp_complement(unsigned long *dest, const unsigned long *src,
                             unsigned int bits);

extern void
bfdev_bitmap_comp_replace(unsigned long *bitmap, const unsigned long *oldp,
                          const unsigned long *newp, const unsigned long *mask,
                          unsigned int bits);

extern void
bfdev_bitmap_comp_set(unsigned long *bitmap, unsigned int start,
                      unsigned int bits);

extern void
bfdev_bitmap_comp_clr(unsigned long *bitmap, unsigned int start,
                      unsigned int bits);

static __bfdev_always_inline bool
bfdev_bitmap_empty(const unsigned long *src, unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        return !(*src & BFDEV_BIT_LOW_MASK(bits));
    else if (__builtin_constant_p(bits & BFDEV_BITMAP_MEM_MASK) &&
             bfdev_align_check(bits, BFDEV_BITMAP_MEM_ALIGNMENT))
        return !bfdev_memdiff(src, BFDEV_UINT_MIN, bits / BFDEV_BITS_PER_BYTE);
    else
        return bfdev_find_first_bit(src, bits) >= bits;
}

static __bfdev_always_inline bool
bfdev_bitmap_full(const unsigned long *src, unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        return !(~(*src) & BFDEV_BIT_LOW_MASK(bits));
    else if (__builtin_constant_p(bits & BFDEV_BITMAP_MEM_MASK) &&
             bfdev_align_check(bits, BFDEV_BITMAP_MEM_ALIGNMENT))
        return !bfdev_memdiff(src, BFDEV_UINT_MAX, bits / BFDEV_BITS_PER_BYTE);
    else
        return bfdev_find_first_zero(src, bits) >= bits;
}

static __bfdev_always_inline bool
bfdev_bitmap_equal(const unsigned long *src1, const unsigned long *src2,
                   unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        return !!((*src1 ^ *src2) & BFDEV_BIT_LOW_MASK(bits));
    else if (__builtin_constant_p(bits & BFDEV_BITMAP_MEM_MASK) &&
             bfdev_align_check(bits, BFDEV_BITMAP_MEM_ALIGNMENT))
        return memcmp(src1, src2, bits / BFDEV_BITS_PER_BYTE);
    else
        return bfdev_bitmap_comp_equal(src1, src2, bits);
}

static __bfdev_always_inline bool
bfdev_bitmap_or_equal(const unsigned long *src1, const unsigned long *src2,
                      const unsigned long *src3, unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        return !!(((*src1 | *src2) ^ *src3) & BFDEV_BIT_LOW_MASK(bits));
    else
        return bfdev_bitmap_comp_or_equal(src1, src2, src3, bits);
}

static __bfdev_always_inline bool
bfdev_bitmap_intersects(const unsigned long *src1, const unsigned long *src2,
                        unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        return !!((*src1 & *src2) & BFDEV_BIT_LOW_MASK(bits));
    else
        return bfdev_bitmap_comp_intersects(src1, src2, bits);
}

static __bfdev_always_inline bool
bfdev_bitmap_and(unsigned long *dest, const unsigned long *src1,
                 const unsigned long *src2, unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        return !!(*dest = *src1 & *src2 & BFDEV_BIT_LOW_MASK(bits));
    else
        return bfdev_bitmap_comp_and(dest, src1, src2, bits);
}

static __bfdev_always_inline void
bfdev_bitmap_or(unsigned long *dest, const unsigned long *src1,
                const unsigned long *src2, unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        *dest = *src1 | *src2;
    else
        bfdev_bitmap_comp_or(dest, src1, src2, bits);
}

static __bfdev_always_inline void
bfdev_bitmap_xor(unsigned long *dest, const unsigned long *src1,
                 const unsigned long *src2, unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        *dest = *src1 ^ *src2;
    else
        bfdev_bitmap_comp_xor(dest, src1, src2, bits);
}

static __bfdev_always_inline void
bfdev_bitmap_complement(unsigned long *dest, const unsigned long *src,
                        unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        *dest = ~(*src);
    else
        bfdev_bitmap_comp_complement(dest, src, bits);
}

static __bfdev_always_inline void
bfdev_bitmap_set(unsigned long *bitmap, unsigned int start, unsigned int bits)
{
    if (__builtin_constant_p(bits) && bits == 1)
        bfdev_bit_set(bitmap, start);
    else if (__builtin_constant_p(start & BFDEV_BITMAP_MEM_MASK) &&
             bfdev_align_check(start, BFDEV_BITMAP_MEM_ALIGNMENT) &&
             __builtin_constant_p(bits & BFDEV_BITMAP_MEM_MASK) &&
             bfdev_align_check(bits, BFDEV_BITMAP_MEM_ALIGNMENT))
        memset((char *)bitmap + start / BFDEV_BITS_PER_BYTE, 0xff, bits / BFDEV_BITS_PER_BYTE);
    else
        bfdev_bitmap_comp_set(bitmap, start, bits);
}

static __bfdev_always_inline void
bfdev_bitmap_clr(unsigned long *bitmap, unsigned int start, unsigned int bits)
{
    if (__builtin_constant_p(bits) && bits == 1)
        bfdev_bit_clr(bitmap, start);
    else if (__builtin_constant_p(start & BFDEV_BITMAP_MEM_MASK) &&
             bfdev_align_check(start, BFDEV_BITMAP_MEM_ALIGNMENT) &&
             __builtin_constant_p(bits & BFDEV_BITMAP_MEM_MASK) &&
             bfdev_align_check(bits, BFDEV_BITMAP_MEM_ALIGNMENT))
        memset((char *)bitmap + start / BFDEV_BITS_PER_BYTE, 0x00, bits / BFDEV_BITS_PER_BYTE);
    else
        bfdev_bitmap_comp_clr(bitmap, start, bits);
}

static __bfdev_always_inline void
bfdev_bitmap_zero(unsigned long *bitmap, unsigned int bits)
{
    unsigned int len = BFDEV_BITS_TO_U8(bits);
    memset(bitmap, 0, len);
}

static __bfdev_always_inline void
bfdev_bitmap_fill(unsigned long *bitmap, unsigned int bits)
{
    unsigned int len = BFDEV_BITS_TO_U8(bits);
    memset(bitmap, UINT8_MAX, len);
}

static __bfdev_always_inline void
bfdev_bitmap_copy(unsigned long *dest, unsigned long *src, unsigned int bits)
{
    unsigned int len = BFDEV_BITS_TO_U8(bits);
    memcpy(dest, src, len);
}

/**
 * bfdev_bitmap_alloc - alloc a bitmap.
 * @bits: number of bits in the bitmap.
 * @flags: allocate flags.
 */
extern unsigned long *
bfdev_bitmap_alloc(const bfdev_alloc_t *alloc, unsigned int bits);

/**
 * bfdev_bitmap_zalloc - alloc and zeroed a bitmap.
 * @bits: number of bits in the bitmap.
 * @flags: allocate flags.
 */
extern unsigned long *
bfdev_bitmap_zalloc(const bfdev_alloc_t *alloc, unsigned int bits);

/**
 * bfdev_bitmap_free - free a bitmap.
 * @bitmap: bitmap to free.
 */
extern void
bfdev_bitmap_free(const bfdev_alloc_t *alloc, const unsigned long *bitmap);

BFDEV_END_DECLS

#endif /* _BFDEV_BITMAP_H_ */
