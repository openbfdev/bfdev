/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/bitmap-comp.h>
#include <export.h>

export bool
bfdev_bitmap_comp_equal(const unsigned long *src1, const unsigned long *src2,
                        unsigned int bits)
{
    unsigned int index, length;
    unsigned long value;

    length = BFDEV_BITS_DIV_LONG(bits);
    for (index = 0; index < length; ++index) {
        if (src1[index] != src2[index])
            return false;
    }

    if (BFDEV_BITS_MOD_LONG(bits)) {
        value = src1[index] ^ src2[index];
        if (value & BFDEV_BIT_LOW_MASK(bits))
            return false;
    }

    return true;
}

export bool
bfdev_bitmap_comp_or_equal(const unsigned long *src1, const unsigned long *src2,
                           const unsigned long *src3, unsigned int bits)
{
    unsigned int index, length;
    unsigned long value;

    length = BFDEV_BITS_DIV_LONG(bits);
    for (index = 0; index < length; ++index) {
        if ((src1[index] | src2[index]) != src3[index])
            return false;
    }

    if (BFDEV_BITS_MOD_LONG(bits)) {
        value = (src1[index] | src2[index]) ^ src3[index];
        if (value & BFDEV_BIT_LOW_MASK(bits))
            return false;
    }

    return true;
}

export bool
bfdev_bitmap_comp_intersects(const unsigned long *src1, const unsigned long *src2,
                             unsigned int bits)
{
    unsigned int index, length;
    unsigned long value;

    length = BFDEV_BITS_DIV_LONG(bits);
    for (index = 0; index < length; ++index) {
        if ((src1[index] & src2[index]))
            return true;
    }

    if (BFDEV_BITS_MOD_LONG(bits)) {
        value = src1[index] & src2[index];
        if (value & BFDEV_BIT_LOW_MASK(bits))
            return true;
    }

    return false;
}

export bool
bfdev_bitmap_comp_and(unsigned long *dest, const unsigned long *src1,
                      const unsigned long *src2, unsigned int bits)
{
    unsigned int index, length;
    unsigned long value, result = 0;

    length = BFDEV_BITS_DIV_LONG(bits);
    for (index = 0; index < length; ++index) {
        value = src1[index] & src2[index];
        result |= (dest[index] = value);
    }

    if (BFDEV_BITS_MOD_LONG(bits)) {
        value = src1[index] & src2[index];
        result |= (dest[index] = (value & BFDEV_BIT_LOW_MASK(bits)));
    }

    return !!result;
}

export bool
bfdev_bitmap_comp_andnot(unsigned long *dest, const unsigned long *src1,
                         const unsigned long *src2, unsigned int bits)
{
    unsigned int index, length;
    unsigned long value, result = 0;

    length = BFDEV_BITS_DIV_LONG(bits);
    for (index = 0; index < length; ++index) {
        value = src1[index] & ~src2[index];
        result |= (dest[index] = value);
    }

    if (BFDEV_BITS_MOD_LONG(bits)) {
        value = src1[index] & ~src2[index];
        result |= (dest[index] = (value & BFDEV_BIT_LOW_MASK(bits)));
    }

    return !!result;
}

export void
bfdev_bitmap_comp_or(unsigned long *dest, const unsigned long *src1,
                     const unsigned long *src2, unsigned int bits)
{
    unsigned int index, length;
    unsigned long value;

    length = BFDEV_BITS_DIV_LONG(bits);
    for (index = 0; index < length; ++index) {
        value = src1[index] | src2[index];
        dest[index] = value;
    }

    if (BFDEV_BITS_MOD_LONG(bits)) {
        value = src1[index] | src2[index];
        dest[index] = value & BFDEV_BIT_LOW_MASK(bits);
    }
}

export void
bfdev_bitmap_comp_xor(unsigned long *dest, const unsigned long *src1,
                      const unsigned long *src2, unsigned int bits)
{
    unsigned int index, length;
    unsigned long value;

    length = BFDEV_BITS_DIV_LONG(bits);
    for (index = 0; index < length; ++index) {
        value = src1[index] ^ src2[index];
        dest[index] = value;
    }

    if (BFDEV_BITS_MOD_LONG(bits)) {
        value = src1[index] ^ src2[index];
        dest[index] = value & BFDEV_BIT_LOW_MASK(bits);
    }
}

export void
bfdev_bitmap_comp_shl(unsigned long *dest, const unsigned long *src,
                      unsigned int shift, unsigned int bits)
{
    unsigned int length, offset, rem, index;
    unsigned long value, vlow, vhigh;

    length = BFDEV_BITS_TO_LONG(bits);
    offset = BFDEV_BITS_DIV_LONG(shift);

    if (length <= offset) {
        memset(dest, 0, length * sizeof(*dest));
        return;
    }

    /* length > offset */
    rem = BFDEV_BITS_MOD_LONG(shift);
    index = length - offset - 1;

    if (BFDEV_BITS_MOD_LONG(bits)) {
        vhigh = src[index] << rem;

        if (rem && index)
            vlow = src[--index] >> (BFDEV_BITS_PER_LONG - rem);
        else
            vlow = 0;

        value = vhigh | vlow;
        dest[--length] = value & BFDEV_BIT_LOW_MASK(bits);
    }

    while (length) {
        vhigh = src[index] << rem;

        if (rem && index)
            vlow = src[--index] >> (BFDEV_BITS_PER_LONG - rem);
        else
            vlow = 0;

        value = vhigh | vlow;
        dest[--length] = value;
    }

    if (offset)
        memset(dest, 0, offset * sizeof(*dest));
}

export void
bfdev_bitmap_comp_shr(unsigned long *dest, const unsigned long *src,
                      unsigned int shift, unsigned int bits)
{
    unsigned int index, length, offset, rem;
    unsigned long value, vlow, vhigh;

    length = BFDEV_BITS_TO_LONG(bits);
    offset = BFDEV_BITS_DIV_LONG(shift);

    if (length <= offset) {
        memset(dest, 0, length * sizeof(*dest));
        return;
    }

    /* length > offset */
    rem = BFDEV_BITS_MOD_LONG(shift);
    index = 0;

    while (index < BFDEV_BITS_DIV_LONG(bits) - offset) {
        vlow = src[index + offset] >> rem;

        if (rem && index + offset + 1 < length)
            vhigh = src[index + offset + 1] << (BFDEV_BITS_PER_LONG - rem);
        else
            vhigh = 0;

        value = vhigh | vlow;
        dest[index++] = value;
    }

    if (BFDEV_BITS_MOD_LONG(bits)) {
        vlow = src[index] << rem;

        if (rem && index + offset + 1 < length)
            vhigh = src[index + offset + 1] << (BFDEV_BITS_PER_LONG - rem);
        else
            vhigh = 0;

        value = vhigh | vlow;
        dest[index + offset] = value & BFDEV_BIT_LOW_MASK(bits);
    }

    if (offset)
        memset(dest + length - offset, 0, offset * sizeof(*dest));
}

export void
bfdev_bitmap_comp_complement(unsigned long *dest, const unsigned long *src,
                             unsigned int bits)
{
    unsigned int index, length;
    unsigned long value;

    length = BFDEV_BITS_DIV_LONG(bits);
    for (index = 0; index < length; ++index) {
        value = ~src[index];
        dest[index] = value;
    }

    if (BFDEV_BITS_MOD_LONG(bits)) {
        value = ~src[index];
        dest[index] = value & BFDEV_BIT_LOW_MASK(bits);
    }
}

export void
bfdev_bitmap_comp_replace(unsigned long *bitmap, const unsigned long *oldp,
                          const unsigned long *newp, const unsigned long *mask,
                          unsigned int bits)
{
    unsigned int index, length;
    unsigned long value;

    length = BFDEV_BITS_DIV_LONG(bits);
    for (index = 0; index < length; ++index) {
        value = (oldp[index] & ~mask[index]) | (newp[index] & mask[index]);
        bitmap[index] = value;
    }

    if (BFDEV_BITS_MOD_LONG(bits)) {
        value = (oldp[index] & ~mask[index]) | (newp[index] & mask[index]);
        bitmap[index] = value & BFDEV_BIT_LOW_MASK(bits);
    }
}

export void
bfdev_bitmap_comp_set(unsigned long *bitmap, unsigned int start,
                      unsigned int bits)
{
    unsigned int bits_to_set, size;
    unsigned long mask_to_set, *curr;

    bits_to_set = BFDEV_BITS_PER_LONG - BFDEV_BITS_MOD_LONG(start);
    mask_to_set = BFDEV_BIT_HIGH_MASK(start);
    curr = bitmap + BFDEV_BITS_DIV_LONG(start);
    size = start + bits;

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
bfdev_bitmap_comp_clr(unsigned long *bitmap, unsigned int start,
                      unsigned int bits)
{
    unsigned int bits_to_clr, size;
    unsigned long mask_to_clr, *curr;

    bits_to_clr = BFDEV_BITS_PER_LONG - BFDEV_BITS_MOD_LONG(start);
    mask_to_clr = BFDEV_BIT_HIGH_MASK(start);
    curr = bitmap + BFDEV_BITS_DIV_LONG(start);
    size = start + bits;

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
bfdev_bitmap_alloc(const bfdev_alloc_t *alloc, unsigned int bits)
{
    return bfdev_malloc_array(
        alloc, BFDEV_BITS_TO_LONG(bits), sizeof(unsigned long)
    );
}

export unsigned long *
bfdev_bitmap_zalloc(const bfdev_alloc_t *alloc, unsigned int bits)
{
    return bfdev_zalloc_array(
        alloc, BFDEV_BITS_TO_LONG(bits), sizeof(unsigned long)
    );
}

export void
bfdev_bitmap_free(const bfdev_alloc_t *alloc, const unsigned long *bitmap)
{
    bfdev_free(alloc, bitmap);
}
