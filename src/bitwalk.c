/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/bitwalk-comp.h>
#include <bfdev/swab.h>
#include <export.h>

export unsigned int
bfdev_comp_find_first_bit(const unsigned long *block, unsigned int bits,
                          unsigned long invert, bool swap)
{
    unsigned long value;
    unsigned int base, offset;

    for (base = 0; base < bits; base += BFDEV_BITS_PER_LONG) {
        if (swap)
            value = bfdev_swabp(block);
        else
            value = *block;
        value ^= invert;

        if (value == BFDEV_ULONG_MIN) {
            block++;
            continue;
        }

        offset = base + bfdev_ffsuf(value);
        if (offset >= bits)
            break;

        return offset;
    }

    return bits;
}

export unsigned int
bfdev_comp_find_last_bit(const unsigned long *block, unsigned int bits,
                         unsigned long invert, bool swap)
{
    unsigned long value, mask;
    unsigned int base, offset;

    if (bfdev_unlikely(!bits))
        return 0;

    mask = BFDEV_BIT_LOW_MASK(bits);
    base = BFDEV_BITS_DIV_LONG(bits - 1);

    do {
        if (swap)
            value = bfdev_swab(block[base]);
        else
            value = block[base];

        value ^= invert;
        value &= mask;

        if (value == BFDEV_ULONG_MIN) {
            mask = BFDEV_ULONG_MAX;
            continue;
        }

        offset = base * BFDEV_BITS_PER_LONG;
        return offset + bfdev_flsuf(value);
    } while (base--);

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

    mask = BFDEV_BIT_LOW_MASK(start + 1);
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
