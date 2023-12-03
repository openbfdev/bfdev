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

BFDEV_BEGIN_DECLS

/**
 * bfdev_comp_find_first_bit - complex find first bit in a region.
 * @block: the block to find.
 * @bits: number of bits in the block.
 * @swap: byte swap in the block.
 */
extern unsigned int
bfdev_comp_find_first_bit(const unsigned long *block, unsigned int bits, bool swap);

/**
 * bfdev_comp_find_last_bit - complex find last bit in a region.
 * @block: the block to find.
 * @bits: number of bits in the block.
 * @swap: byte swap in the block.
 */
extern unsigned int
bfdev_comp_find_last_bit(const unsigned long *block, unsigned int bits, bool swap);

/**
 * bfdev_comp_find_first_zero - complex find first zero in a region.
 * @block: the block to find.
 * @bits: number of bits in the block.
 * @swap: byte swap in the block.
 */
extern unsigned int
bfdev_comp_find_first_zero(const unsigned long *block, unsigned int bits, bool swap);

/**
 * bfdev_comp_find_last_zero - complex find last zero in a region.
 * @block: the block to find.
 * @bits: number of bits in the block.
 * @swap: byte swap in the block.
 */
extern unsigned int
bfdev_comp_find_last_zero(const unsigned long *block, unsigned int bits, bool swap);

/**
 * bfdev_comp_find_last_zero - complex find last zero in a region.
 * @block: the block to find.
 * @bits: number of bits in the block.
 * @swap: byte swap in the block.
 */
extern unsigned int
bfdev_comp_find_next_bit(const unsigned long *addr1, const unsigned long *addr2,
                         unsigned int bits, unsigned int start,
                         unsigned long invert, bool swap);

extern unsigned int
bfdev_comp_find_prev_bit(const unsigned long *addr1, const unsigned long *addr2,
                         unsigned int bits, unsigned int start,
                         unsigned long invert, bool swap);

#ifndef bfdev_find_first_bit
static inline unsigned int
bfdev_find_first_bit(const unsigned long *addr, unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        return *addr ? bfdev_ffsuf(*addr & BFDEV_BIT_LOW_MASK(bits)) : bits;
    else
        return bfdev_comp_find_first_bit(addr, bits, false);
}
#endif

#ifndef bfdev_find_last_bit
static inline unsigned int
bfdev_find_last_bit(const unsigned long *addr, unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        return *addr ? bfdev_ffsuf(*addr & BFDEV_BIT_LOW_MASK(bits)) : bits;
    else
        return bfdev_comp_find_last_bit(addr, bits, false);
}
#endif

#ifndef bfdev_find_first_zero
static inline unsigned int
bfdev_find_first_zero(const unsigned long *addr, unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        return *addr ? bfdev_ffz(*addr | BFDEV_BIT_HIGH_MASK(bits)) : bits;
    else
        return bfdev_comp_find_first_zero(addr, bits, false);
}
#endif

#ifndef bfdev_find_last_zero
static inline unsigned int
bfdev_find_last_zero(const unsigned long *addr, unsigned int bits)
{
    if (bfdev_const_small_nbits(bits))
        return *addr ? bfdev_flz(*addr | BFDEV_BIT_HIGH_MASK(bits)) : bits;
    else
        return bfdev_comp_find_last_zero(addr, bits, false);
}
#endif

#ifndef bfdev_find_next_bit
static inline unsigned int
bfdev_find_next_bit(const unsigned long *addr, unsigned int bits, unsigned int offset)
{
    if (bfdev_const_small_nbits(bits)) {
        unsigned long val;

        if (bfdev_unlikely(offset >= bits))
            return bits;

        val = *addr & BFDEV_BIT_RANGE(bits - 1, offset);
        return val ? bfdev_ffsuf(val) : bits;
    }

    return bfdev_comp_find_next_bit(addr, NULL, bits, offset, 0UL, false);
}
#endif

#ifndef bfdev_find_prev_bit
static inline unsigned int
bfdev_find_prev_bit(const unsigned long *addr, unsigned int bits, unsigned int offset)
{
    if (bfdev_const_small_nbits(bits)) {
        unsigned long val;

        if (bfdev_unlikely(offset >= bits))
            return bits;

        val = *addr & BFDEV_BIT_RANGE(bits - 1, offset);
        return val ? bfdev_flsuf(val) : bits;
    }

    return bfdev_comp_find_prev_bit(addr, NULL, bits, offset, 0UL, false);
}
#endif

#ifndef bfdev_find_next_zero
static inline unsigned int
bfdev_find_next_zero(const unsigned long *addr, unsigned int bits, unsigned int offset)
{
    if (bfdev_const_small_nbits(bits)) {
        unsigned long val;

        if (bfdev_unlikely(offset >= bits))
            return bits;

        val = *addr | ~BFDEV_BIT_RANGE(bits - 1, offset);
        return val ? bfdev_ffzuf(val) : bits;
    }

    return bfdev_comp_find_next_bit(addr, NULL, bits, offset, ~0UL, false);
}
#endif

#ifndef bfdev_find_prev_zero
static inline unsigned int
bfdev_find_prev_zero(const unsigned long *addr, unsigned int bits, unsigned int offset)
{
    if (bfdev_const_small_nbits(bits)) {
        unsigned long val;

        if (bfdev_unlikely(offset >= bits))
            return bits;

        val = *addr | ~BFDEV_BIT_RANGE(bits - 1, offset);
        return val ? bfdev_flzuf(val) : bits;
    }

    return bfdev_comp_find_prev_bit(addr, NULL, bits, offset, ~0UL, false);
}
#endif

#ifndef bfdev_find_next_and_bit
static inline unsigned int
bfdev_find_next_and_bit(const unsigned long *addr1, const unsigned long *addr2,
                        unsigned int bits, unsigned int offset)
{
    if (bfdev_const_small_nbits(bits)) {
        unsigned long val;

        if (bfdev_unlikely(offset >= bits))
            return bits;

        val = *addr1 & *addr2 & BFDEV_BIT_RANGE(bits - 1, offset);
        return val ? bfdev_ffsuf(val) : bits;
    }

    return bfdev_comp_find_next_bit(addr1, addr2, bits, offset, 0UL, false);
}
#endif

#ifndef bfdev_find_prev_and_bit
static inline unsigned int
bfdev_find_prev_and_bit(const unsigned long *addr1, const unsigned long *addr2,
                        unsigned int bits, unsigned int offset)
{
    if (bfdev_const_small_nbits(bits)) {
        unsigned long val;

        if (bfdev_unlikely(offset >= bits))
            return bits;

        val = *addr1 & *addr2 & BFDEV_BIT_RANGE(bits - 1, offset);
        return val ? bfdev_flsuf(val) : bits;
    }

    return bfdev_comp_find_prev_bit(addr1, addr2, bits, offset, 0UL, false);
}
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_BITWALK_H_ */
