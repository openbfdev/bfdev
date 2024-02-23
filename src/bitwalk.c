/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/bitwalk.h>
#include <bfdev/swab.h>
#include <export.h>

export unsigned int
bfdev_comp_find_first_bit(const unsigned long *block, unsigned int bits, bool swap)
{
    unsigned int base;

    for (base = 0; base < bits; base += BFDEV_BITS_PER_LONG) {
        unsigned long value;

        if (swap)
            value = bfdev_swabp(block);
        else
            value = *block;

        if (value != BFDEV_ULONG_MIN)
            return base + bfdev_ffsuf(value);
        block++;
    }

    return bits;
}

export unsigned int
bfdev_comp_find_last_bit(const unsigned long *block, unsigned int bits, bool swap)
{
    if (bits) {
        unsigned long val = BFDEV_BIT_LOW_MASK(bits);
        unsigned long idx = (bits - 1) / BFDEV_BITS_PER_LONG;

        do {
            unsigned long value;

            if (swap)
                value = bfdev_swab(block[idx]);
            else
                value = block[idx];

            if ((value ^ BFDEV_ULONG_MIN) & val)
                return idx * BFDEV_BITS_PER_LONG + bfdev_flsuf(value);
            val = BFDEV_ULONG_MAX;
        } while (idx--);
    }

    return bits;
}

export unsigned int
bfdev_comp_find_first_zero(const unsigned long *block, unsigned int bits, bool swap)
{
    unsigned int base;

    for (base = 0; base < bits; base += BFDEV_BITS_PER_LONG) {
        unsigned long value;

        if (swap)
            value = bfdev_swabp(block);
        else
            value = *block;

        if (value != BFDEV_ULONG_MAX)
            return base + bfdev_ffzuf(value);
        block++;
    }

    return bits;
}

export unsigned int
bfdev_comp_find_last_zero(const unsigned long *block, unsigned int bits, bool swap)
{
    if (bits) {
        unsigned long val = BFDEV_BIT_LOW_MASK(bits);
        unsigned long idx = (bits - 1) / BFDEV_BITS_PER_LONG;

        do {
            unsigned long value;

            if (swap)
                value = bfdev_swab(block[idx]);
            else
                value = block[idx];

            if ((value ^ BFDEV_ULONG_MAX) & val)
                return idx * BFDEV_BITS_PER_LONG + bfdev_flzuf(value);
            val = BFDEV_ULONG_MAX;
        } while (idx--);
    }

    return bits;
}

export unsigned int
bfdev_comp_find_next_bit(const unsigned long *addr1, const unsigned long *addr2,
                         unsigned int bits, unsigned int start,
                         unsigned long invert, bool swap)
{
    unsigned long value, mask;

    if (bfdev_unlikely(start >= bits))
        return bits;

    value = addr1[BFDEV_BITS_DIV_LONG(start)];
    if (addr2)
        value &= addr2[BFDEV_BITS_DIV_LONG(start)];
    value ^= invert;

    mask = BFDEV_BIT_HIGH_MASK(start);
    if (swap)
        mask = bfdev_swab(mask);

    value &= mask;
    start = bfdev_round_down(start, BFDEV_BITS_PER_LONG);

    while (!value) {
        start += BFDEV_BITS_PER_LONG;
        if (start >= bits)
            return bits;

        value = addr1[BFDEV_BITS_DIV_LONG(start)];
        if (addr2)
            value &= addr2[BFDEV_BITS_DIV_LONG(start)];
        value ^= invert;
    }

    if (swap)
        value = bfdev_swab(value);

    return bfdev_min(start + bfdev_ffsuf(value), bits);
}

export unsigned int
bfdev_comp_find_prev_bit(const unsigned long *addr1, const unsigned long *addr2,
                         unsigned int bits, unsigned int start,
                         unsigned long invert, bool swap)
{
    unsigned long value, mask;

    if (bfdev_unlikely(start >= bits))
        return bits;

    value = addr1[BFDEV_BITS_DIV_LONG(start)];
    if (addr2)
        value &= addr2[BFDEV_BITS_DIV_LONG(start)];
    value ^= invert;

    mask = BFDEV_BIT_HIGH_MASK(start);
    if (swap)
        mask = bfdev_swab(mask);

    value &= mask;
    start = bfdev_round_down(start, BFDEV_BITS_PER_LONG);

    while (!value) {
        start -= BFDEV_BITS_PER_LONG;
        if (start >= bits)
            return bits;

        value = addr1[BFDEV_BITS_DIV_LONG(start)];
        if (addr2)
            value &= addr2[BFDEV_BITS_DIV_LONG(start)];
        value ^= invert;
    }

    if (swap)
        value = bfdev_swab(value);

    return bfdev_min(start + bfdev_flsuf(value), bits);
}
