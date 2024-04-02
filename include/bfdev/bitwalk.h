/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BITWALK_H_
#define _BFDEV_BITWALK_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/bitops.h>
#include <bfdev/bitwalk-comp.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_find_first_bit() - complex find first bit in a region.
 * @block: the block to find.
 * @bits: number of bits in the block.
 *
 * Returns the bit number of the first set bit.
 * If no bits are set, returns @bits.
 */
#ifndef bfdev_find_first_bit
static inline unsigned int
bfdev_find_first_bit(const unsigned long *addr, unsigned int bits)
{
    unsigned long value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_comp_find_first_bit(addr, bits, 0UL, false);

    value = *addr & BFDEV_BIT_LOW_MASK(bits);
    if (value == BFDEV_ULONG_MIN)
        return bits;

    return bfdev_ffsuf(value);
}
#endif

#ifndef bfdev_find_last_bit
static inline unsigned int
bfdev_find_last_bit(const unsigned long *addr, unsigned int bits)
{
    unsigned long value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_comp_find_last_bit(addr, bits, 0UL, false);

    value = *addr & BFDEV_BIT_LOW_MASK(bits);
    if (value == BFDEV_ULONG_MIN)
        return bits;

    return bfdev_flsuf(value);
}
#endif

/**
 * bfdev_comp_find_first_zero - find first zero in a region.
 * @block: the block to find.
 * @bits: number of bits in the block.
 *
 * Returns the bit number of the first cleared bit.
 * If no bits are zero, returns @bits.
 */
#ifndef bfdev_find_first_zero
static inline unsigned int
bfdev_find_first_zero(const unsigned long *addr, unsigned int bits)
{
    unsigned long value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_comp_find_first_bit(addr, bits, ~0UL, false);

    value = *addr | BFDEV_BIT_HIGH_MASK(bits);
    if (value == BFDEV_ULONG_MAX)
        return bits;

    return bfdev_ffzuf(value);
}
#endif

#ifndef bfdev_find_last_zero
static inline unsigned int
bfdev_find_last_zero(const unsigned long *addr, unsigned int bits)
{
    unsigned long value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_comp_find_last_bit(addr, bits, ~0UL, false);

    value = *addr | BFDEV_BIT_HIGH_MASK(bits);
    if (value == BFDEV_ULONG_MAX)
        return bits;

    return bfdev_flzuf(value);
}
#endif

/**
 * bfdev_find_next_bit() - find next bit in a region.
 * @block: the block to find.
 * @bits: number of bits in the block.
 * @offset: The bitnumber to start searching at.
 *
 * Returns the bit number for the next set bit
 * If no bits are set, returns @bits.
 */
#ifndef bfdev_find_next_bit
static inline unsigned int
bfdev_find_next_bit(const unsigned long *addr,
                    unsigned int bits, unsigned int offset)
{
    unsigned long value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_comp_find_next_bit(addr, NULL, bits, offset, 0UL, false);

    if (bfdev_unlikely(offset >= bits))
        return bits;

    value = *addr & BFDEV_BIT_RANGE(bits - 1, offset);
    if (value == BFDEV_ULONG_MIN)
        return bits;

    return bfdev_ffsuf(value);
}
#endif

#ifndef bfdev_find_prev_bit
static inline unsigned int
bfdev_find_prev_bit(const unsigned long *addr,
                    unsigned int bits, unsigned int offset)
{
    unsigned long value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_comp_find_prev_bit(addr, NULL, bits, offset, 0UL, false);

    if (bfdev_unlikely(offset >= bits))
        return bits;

    value = *addr & BFDEV_BIT_LOW_MASK(offset + 1);
    if (value == BFDEV_ULONG_MIN)
        return bits;

    return bfdev_flsuf(value);
}
#endif

/**
 * bfdev_find_next_bit() - find next zero in a region.
 * @block: the block to find.
 * @bits: number of bits in the block.
 * @offset: the bitnumber to start searching at.
 *
 * Returns the bit number for the next set bit
 * If no bits are set, returns @bits.
 */
#ifndef bfdev_find_next_zero
static inline unsigned int
bfdev_find_next_zero(const unsigned long *addr,
                     unsigned int bits, unsigned int offset)
{
    unsigned long value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_comp_find_next_bit(addr, NULL, bits, offset, ~0UL, false);

    if (bfdev_unlikely(offset >= bits))
        return bits;

    value = *addr | ~BFDEV_BIT_RANGE(bits - 1, offset);
    if (value == BFDEV_ULONG_MAX)
        return bits;

    return bfdev_ffzuf(value);
}
#endif

#ifndef bfdev_find_prev_zero
static inline unsigned int
bfdev_find_prev_zero(const unsigned long *addr,
                     unsigned int bits, unsigned int offset)
{
    unsigned long value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_comp_find_prev_bit(addr, NULL, bits, offset, ~0UL, false);

    if (bfdev_unlikely(offset >= bits))
        return bits;

    value = *addr | BFDEV_BIT_HIGH_MASK(offset + 1);
    if (value == BFDEV_ULONG_MAX)
        return bits;

    return bfdev_flzuf(value);
}
#endif

/**
 * bfdev_find_next_and_bit() - find the next set bit in both memory regions.
 * @addr1: the first address to base the search on.
 * @addr2: the second address to base the search on.
 * @bits: number of bits in the block.
 * @offset: the bitnumber to start searching at.
 *
 * Returns the bit number for the next set bit, or first set bit up to @offset
 * If no bits are set, returns @bits.
 */
#ifndef bfdev_find_next_and_bit
static inline unsigned int
bfdev_find_next_and_bit(const unsigned long *addr1, const unsigned long *addr2,
                        unsigned int bits, unsigned int offset)
{
    unsigned long value;

    if (bfdev_unlikely(offset >= bits))
        return bits;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_comp_find_next_bit(addr1, addr2, bits, offset, 0UL, false);

    value = *addr1 & *addr2 & BFDEV_BIT_RANGE(bits - 1, offset);
    if (value == BFDEV_ULONG_MIN)
        return bits;

    return value ? bfdev_ffsuf(value) : bits;
}
#endif

#ifndef bfdev_find_prev_and_bit
static inline unsigned int
bfdev_find_prev_and_bit(const unsigned long *addr1, const unsigned long *addr2,
                        unsigned int bits, unsigned int offset)
{
    unsigned long value;

    if (!bfdev_const_small_nbits(bits))
        return bfdev_comp_find_prev_bit(addr1, addr2, bits, offset, 0UL, false);

    if (bfdev_unlikely(offset >= bits))
        return bits;

    value = *addr1 & *addr2 & BFDEV_BIT_LOW_MASK(offset + 1);
    if (value == BFDEV_ULONG_MIN)
        return bits;

    return value ? bfdev_flsuf(value) : bits;
}
#endif

#define bfdev_for_each_bit(index, bitmap, bits) \
    for ((index) = bfdev_find_first_bit(bitmap, bits); \
         (index) < (bits); \
         (index) = bfdev_find_next_bit(bitmap, bits, (index) + 1))

#define bfdev_for_each_zero(index, bitmap, bits) \
    for ((index) = bfdev_find_first_zero(bitmap, bits); \
         (index) < (bits); \
         (index) = bfdev_find_next_zero(bitmap, bits, (index) + 1))

#define bfdev_for_each_bit_reverse(index, bitmap, bits) \
    for ((index) = bfdev_find_last_bit(bitmap, bits); \
         (index) < (bits); \
         (index) = bfdev_find_prev_bit(bitmap, bits, (index) - 1))

#define bfdev_for_each_zero_reverse(index, bitmap, bits) \
    for ((index) = bfdev_find_last_zero(bitmap, bits); \
         (index) < (bits); \
         (index) = bfdev_find_prev_zero(bitmap, bits, (index) - 1))

#define bfdev_for_each_bit_form(index, bitmap, bits) \
    for (; (index) < (bits); \
         (index) = bfdev_find_next_bit(bitmap, bits, (index) + 1))

#define bfdev_for_each_zero_form(index, bitmap, bits) \
    for (; (index) < (bits); \
         (index) = bfdev_find_next_zero(bitmap, bits, (index) + 1))

#define bfdev_for_each_bit_reverse_form(index, bitmap, bits) \
    for (; (index) < (bits); \
         (index) = bfdev_find_prev_bit(bitmap, bits, (index) - 1))

#define bfdev_for_each_zero_reverse_form(index, bitmap, bits) \
    for (; (index) < (bits); \
         (index) = bfdev_find_prev_zero(bitmap, bits, (index) - 1))

#define bfdev_for_each_bit_continue(index, bitmap, bits) \
    for ((index) = bfdev_find_next_bit(bitmap, bits, (index) + 1); \
         (index) < (bits); \
         (index) = bfdev_find_next_bit(bitmap, bits, (index) + 1))

#define bfdev_for_each_zero_continue(index, bitmap, bits) \
    for ((index) = bfdev_find_next_zero(bitmap, bits, (index) + 1); \
         (index) < (bits); \
         (index) = bfdev_find_next_zero(bitmap, bits, (index) + 1))

#define bfdev_for_each_bit_reverse_continue(index, bitmap, bits) \
    for ((index) = bfdev_find_prev_bit(bitmap, bits, (index) - 1); \
         (index) < (bits); \
         (index) = bfdev_find_prev_bit(bitmap, bits, (index) - 1))

#define bfdev_for_each_zero_reverse_continue(index, bitmap, bits) \
    for ((index) = bfdev_find_prev_zero(bitmap, bits, (index) - 1); \
         (index) < (bits); \
         (index) = bfdev_find_prev_zero(bitmap, bits, (index) - 1))

BFDEV_END_DECLS

#endif /* _BFDEV_BITWALK_H_ */
