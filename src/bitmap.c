/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/bitmap.h>
#include <export.h>

export bool
bfdev_bitmap_comp_equal(const unsigned long *src1, const unsigned long *src2,
                        unsigned int bits)
{
    unsigned int index, len = BFDEV_BITS_DIV_LONG(bits);

    for (index = 0; index < len; ++index)
        if (src1[index] != src2[index])
            return false;

    if (BFDEV_BITS_MOD_LONG(bits))
        if ((src1[index] ^ src2[index]) & BFDEV_BIT_LOW_MASK(bits))
            return false;

    return true;
}

export bool
bfdev_bitmap_comp_or_equal(const unsigned long *src1, const unsigned long *src2,
                           const unsigned long *src3, unsigned int bits)
{
    unsigned int index, len = BFDEV_BITS_DIV_LONG(bits);

    for (index = 0; index < len; ++index)
        if ((src1[index] | src2[index]) != src3[index])
            return false;

    if (BFDEV_BITS_MOD_LONG(bits))
        if (((src1[index] | src2[index]) ^ src3[index]) & BFDEV_BIT_LOW_MASK(bits))
            return false;

    return true;
}

export bool
bfdev_bitmap_comp_intersects(const unsigned long *src1, const unsigned long *src2,
                             unsigned int bits)
{
    unsigned int index, len = BFDEV_BITS_DIV_LONG(bits);

    for (index = 0; index < len; ++index)
        if ((src1[index] & src2[index]))
            return true;

    if (BFDEV_BITS_MOD_LONG(bits))
        if ((src1[index] & src2[index]) & BFDEV_BIT_LOW_MASK(bits))
            return true;

    return false;
}

export bool
bfdev_bitmap_comp_and(unsigned long *dest, const unsigned long *src1,
                      const unsigned long *src2, unsigned int bits)
{
    unsigned int index, len = BFDEV_BITS_DIV_LONG(bits);
    unsigned long result = 0;

    for (index = 0; index < len; ++index)
        result |= (dest[index] = src1[index] & src2[index]);

    if (BFDEV_BITS_MOD_LONG(bits))
        result |= (dest[index] = src1[index] & src2[index] & BFDEV_BIT_LOW_MASK(bits));

    return !!result;
}

export bool
bfdev_bitmap_comp_andnot(unsigned long *dest, const unsigned long *src1,
                         const unsigned long *src2, unsigned int bits)
{
    unsigned int index, len = BFDEV_BITS_DIV_LONG(bits);
    unsigned long result = 0;

    for (index = 0; index < len; ++index)
        result |= (dest[index] = src1[index] & ~src2[index]);

    if (BFDEV_BITS_MOD_LONG(bits))
        result |= (dest[index] = src1[index] & ~src2[index] & BFDEV_BIT_LOW_MASK(bits));

    return !!result;
}

export void
bfdev_bitmap_comp_or(unsigned long *dest, const unsigned long *src1,
                     const unsigned long *src2, unsigned int bits)
{
    unsigned int index, len = BFDEV_BITS_TO_LONG(bits);

    for (index = 0; index < len; ++index)
        dest[index] = src1[index] | src2[index];
}

export void
bfdev_bitmap_comp_xor(unsigned long *dest, const unsigned long *src1,
                      const unsigned long *src2, unsigned int bits)
{
    unsigned int index, len = BFDEV_BITS_TO_LONG(bits);

    for (index = 0; index < len; ++index)
        dest[index] = src1[index] ^ src2[index];
}

export void
bfdev_bitmap_comp_complement(unsigned long *dest, const unsigned long *src,
                             unsigned int bits)
{
    unsigned int index, len = BFDEV_BITS_TO_LONG(bits);

    for (index = 0; index < len; ++index)
        dest[index] = ~src[index];
}

export void
bfdev_bitmap_comp_replace(unsigned long *bitmap, const unsigned long *oldp,
                          const unsigned long *newp, const unsigned long *mask,
                          unsigned int bits)
{
    unsigned int index, len = BFDEV_BITS_TO_LONG(bits);

    for (index = 0; index < len; ++index)
        bitmap[index] = (oldp[index] & ~mask[index]) | (newp[index] & mask[index]);
}

export void
bfdev_bitmap_comp_set(unsigned long *bitmap, unsigned int start, unsigned int bits)
{
    unsigned int bits_to_set = BFDEV_BITS_PER_LONG - BFDEV_BITS_MOD_LONG(start);
    unsigned long mask_to_set = BFDEV_BIT_HIGH_MASK(start);
    unsigned long *curr = bitmap + BFDEV_BITS_DIV_LONG(start);
    const unsigned int size = start + bits;

    while (bits - bits_to_set) {
        *curr++ |= mask_to_set;
        bits -= bits_to_set;
        mask_to_set = ULONG_MAX;
        bits_to_set = BFDEV_BITS_PER_LONG;
    }

    if (bits) {
        mask_to_set &= BFDEV_BIT_LOW_MASK(size);
        *curr |= mask_to_set;
    }
}

export void
bfdev_bitmap_comp_clr(unsigned long *bitmap, unsigned int start, unsigned int bits)
{
    unsigned int bits_to_clr = BFDEV_BITS_PER_LONG - BFDEV_BITS_MOD_LONG(start);
    unsigned long mask_to_clr = BFDEV_BIT_HIGH_MASK(start);
    unsigned long *curr = bitmap + BFDEV_BITS_DIV_LONG(start);
    const unsigned int size = start + bits;

    while (bits - bits_to_clr) {
        *curr++ &= ~mask_to_clr;
        bits -= bits_to_clr;
        mask_to_clr = ULONG_MAX;
        bits_to_clr = BFDEV_BITS_PER_LONG;
    }

    if (bits) {
        mask_to_clr &= BFDEV_BIT_LOW_MASK(size);
        *curr &= ~mask_to_clr;
    }
}

export unsigned long *
bfdev_bitmap_malloc(const struct bfdev_alloc *alloc, unsigned int bits)
{
    return bfdev_malloc_array(
        alloc, BFDEV_BITS_TO_LONG(bits), sizeof(unsigned long)
    );
}

export unsigned long *
bfdev_bitmap_zalloc(const struct bfdev_alloc *alloc, unsigned int bits)
{
    return bfdev_zalloc_array(
        alloc, BFDEV_BITS_TO_LONG(bits), sizeof(unsigned long)
    );
}

export void
bfdev_bitmap_free(const struct bfdev_alloc *alloc, const unsigned long *bitmap)
{
    bfdev_free(alloc, bitmap);
}
