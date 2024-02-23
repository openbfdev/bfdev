/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_BITOPS_H_
#define _BFDEV_ASM_GENERIC_BITOPS_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/bits.h>
#include <bfdev/asm/atomic.h>

BFDEV_BEGIN_DECLS

#ifndef bfdev_arch_bit_clr
static __bfdev_always_inline void
bfdev_arch_bit_clr(volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    *addr &= ~BFDEV_BIT(bit);
}
#endif

#ifndef bfdev_arch_bit_set
static __bfdev_always_inline void
bfdev_arch_bit_set(volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    *addr |= BFDEV_BIT(bit);
}
#endif

#ifndef bfdev_arch_bit_flip
static __bfdev_always_inline void
bfdev_arch_bit_flip(volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    *addr ^= BFDEV_BIT(bit);
}
#endif

#ifndef bfdev_arch_bit_change
static __bfdev_always_inline void
bfdev_arch_bit_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    addr += BFDEV_BITS_WORD(bit);
    if (val)
        *addr |= BFDEV_BIT(bit);
    else
        *addr &= ~BFDEV_BIT(bit);
}
#endif

#ifndef bfdev_arch_bit_test
static __bfdev_always_inline bool
bfdev_arch_bit_test(const volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    return !!(*addr & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_test_clr
static __bfdev_always_inline bool
bfdev_arch_bit_test_clr(volatile unsigned long *addr, unsigned int bit)
{
    unsigned long old;
    addr += BFDEV_BITS_WORD(bit);
    old = *addr;
    *addr &= ~BFDEV_BIT(bit);
    return !!(old & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_test_set
static __bfdev_always_inline bool
bfdev_arch_bit_test_set(volatile unsigned long *addr, unsigned int bit)
{
    unsigned long old;
    addr += BFDEV_BITS_WORD(bit);
    old = *addr;
    *addr |= BFDEV_BIT(bit);
    return !!(old & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_test_flip
static __bfdev_always_inline bool
bfdev_arch_bit_test_flip(volatile unsigned long *addr, unsigned int bit)
{
    unsigned long old;
    addr += BFDEV_BITS_WORD(bit);
    old = *addr;
    *addr ^= BFDEV_BIT(bit);
    return !!(old & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_test_change
static __bfdev_always_inline bool
bfdev_arch_bit_test_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    unsigned long old;
    addr += BFDEV_BITS_WORD(bit);
    old = *addr;
    if (val)
        *addr |= BFDEV_BIT(bit);
    else
        *addr &= ~BFDEV_BIT(bit);
    return !!(old & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_clr
static __bfdev_always_inline void
bfdev_arch_bit_atomic_clr(volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    bfdev_arch_atomic_and((bfdev_atomic_t *)addr, ~BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_set
static __bfdev_always_inline void
bfdev_arch_bit_atomic_set(volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    bfdev_arch_atomic_or((bfdev_atomic_t *)addr, BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_flip
static __bfdev_always_inline void
bfdev_arch_bit_atomic_flip(volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    bfdev_arch_atomic_xor((bfdev_atomic_t *)addr, BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_change
static __bfdev_always_inline void
bfdev_arch_bit_atomic_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    addr += BFDEV_BITS_WORD(bit);
    if (val)
        bfdev_arch_atomic_or((bfdev_atomic_t *)addr, BFDEV_BIT(bit));
    else
        bfdev_arch_atomic_and((bfdev_atomic_t *)addr, ~BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_test
static __bfdev_always_inline bool
bfdev_arch_bit_atomic_test(const volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    return !!(bfdev_arch_atomic_read((bfdev_atomic_t *)addr) & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_test_clr
static __bfdev_always_inline bool
bfdev_arch_bit_atomic_test_clr(volatile unsigned long *addr, unsigned int bit)
{
    unsigned long old;

    addr += BFDEV_BITS_WORD(bit);
    if ((BFDEV_READ_ONCE(*addr) & BFDEV_BIT(bit)) == 0)
        return 0;

    old = bfdev_arch_atomic_fetch_and((bfdev_atomic_t *)addr, ~BFDEV_BIT(bit));
    return !!(old & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_test_set
static __bfdev_always_inline bool
bfdev_arch_bit_atomic_test_set(volatile unsigned long *addr, unsigned int bit)
{
    unsigned long old;

    addr += BFDEV_BITS_WORD(bit);
    if ((BFDEV_READ_ONCE(*addr) & BFDEV_BIT(bit)) != 0)
        return 1;

    old = bfdev_arch_atomic_fetch_or((bfdev_atomic_t *)addr, BFDEV_BIT(bit));
    return !!(old & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_test_flip
static __bfdev_always_inline bool
bfdev_arch_bit_atomic_test_flip(volatile unsigned long *addr, unsigned int bit)
{
    unsigned long old;
    addr += BFDEV_BITS_WORD(bit);
    old = bfdev_arch_atomic_fetch_xor((bfdev_atomic_t *)addr, BFDEV_BIT(bit));
    return !!(old & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_test_change
static __bfdev_always_inline bool
bfdev_arch_bit_atomic_test_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    unsigned long old;
    addr += BFDEV_BITS_WORD(bit);
    if (val)
        old = bfdev_arch_atomic_fetch_or((bfdev_atomic_t *)addr, BFDEV_BIT(bit));
    else
        old = bfdev_arch_atomic_fetch_and((bfdev_atomic_t *)addr, ~BFDEV_BIT(bit));
    return !!(old & BFDEV_BIT(bit));
}
#endif

/**
 * bfdev_arch_rol8 - rotate an 8-bit value left.
 * @value: value to rotate.
 * @shift: bits to roll.
 */
#ifndef bfdev_arch_rol8
static __bfdev_always_inline uint8_t
bfdev_arch_rol8(uint8_t value, unsigned int shift)
{
    return (value << (shift & 7)) | (value >> ((-shift) & 7));
}
#endif

/**
 * bfdev_arch_ror8 - rotate an 8-bit value right.
 * @value: value to rotate.
 * @shift: bits to roll.
 */
#ifndef bfdev_arch_ror8
static __bfdev_always_inline uint8_t
bfdev_arch_ror8(uint8_t value, unsigned int shift)
{
    return (value >> (shift & 7)) | (value << ((-shift) & 7));
}
#endif

/**
 * bfdev_arch_rol16 - rotate a 16-bit value left.
 * @value: value to rotate.
 * @shift: bits to roll.
 */
#ifndef bfdev_arch_rol16
static __bfdev_always_inline uint16_t
bfdev_arch_rol16(uint16_t value, unsigned int shift)
{
    return (value << (shift & 15)) | (value >> ((-shift) & 15));
}
#endif

/**
 * bfdev_arch_ror16 - rotate a 16-bit value right.
 * @value: value to rotate.
 * @shift: bits to roll.
 */
#ifndef bfdev_arch_ror16
static __bfdev_always_inline uint16_t
bfdev_arch_ror16(uint16_t value, unsigned int shift)
{
    return (value >> (shift & 15)) | (value << ((-shift) & 15));
}
#endif

/**
 * bfdev_arch_rol32 - rotate a 32-bit value left.
 * @value: value to rotate.
 * @shift: bits to roll.
 */
#ifndef bfdev_arch_rol32
static __bfdev_always_inline uint32_t
bfdev_arch_rol32(uint32_t value, unsigned int shift)
{
    return (value << (shift & 31)) | (value >> ((-shift) & 31));
}
#endif

/**
 * bfdev_arch_ror32 - rotate a 32-bit value right.
 * @value: value to rotate.
 * @shift: bits to roll.
 */
#ifndef bfdev_arch_ror32
static __bfdev_always_inline uint32_t
bfdev_arch_ror32(uint32_t value, unsigned int shift)
{
    return (value >> (shift & 31)) | (value << ((-shift) & 31));
}
#endif

/**
 * bfdev_arch_rol64 - rotate a 64-bit value left.
 * @value: value to rotate.
 * @shift: bits to roll.
 */
#ifndef bfdev_arch_rol64
static __bfdev_always_inline uint64_t
bfdev_arch_rol64(uint64_t value, unsigned int shift)
{
    return (value << (shift & 63)) | (value >> ((-shift) & 63));
}
#endif

/**
 * bfdev_arch_ror64 - rotate a 64-bit value right.
 * @value: value to rotate.
 * @shift: bits to roll.
 */
#ifndef bfdev_arch_ror64
static __bfdev_always_inline uint64_t
bfdev_arch_ror64(uint64_t value, unsigned int shift)
{
    return (value >> (shift & 63)) | (value << ((-shift) & 63));
}
#endif

/**
 * bfdev_arch_ffsuf - find first set bit in word (unsafe).
 * @val: The word to search.
 * Undefined if no bit exists, so code should check against 0 first..
 */
#ifndef bfdev_arch_ffsuf
static __bfdev_always_inline unsigned int
bfdev_arch_ffsuf(unsigned long value)
{
    unsigned int shift = 0;

#if BFDEV_BITS_PER_LONG == 64
    if ((value & 0xffffffff) == 0) {
        shift += 32;
        value >>= 32;
    }
#endif

    if ((value & 0xffff) == 0) {
        shift += 16;
        value >>= 16;
    }

    if ((value & 0xff) == 0) {
        shift += 8;
        value >>= 8;
    }

    if ((value & 0xf) == 0) {
        shift += 4;
        value >>= 4;
    }

    if ((value & 0x3) == 0) {
        shift += 2;
        value >>= 2;
    }

    if ((value & 0x1) == 0) {
        shift += 1;
    }

    return shift;
}
#endif

/*
 * bfdev_arch_flsuf: find last set bit in word (unsafe).
 * @val: The word to search.
 * Undefined if no set bit exists, so code should check against 0 first..
 */
#ifndef bfdev_arch_flsuf
static __bfdev_always_inline unsigned int
bfdev_arch_flsuf(unsigned long value)
{
    unsigned int shift = BFDEV_BITS_PER_LONG - 1;

#if BFDEV_BITS_PER_LONG == 64
    if (!(value & (~0ul << 32))) {
        shift -= 32;
        value <<= 32;
    }
#endif

    if (!(value & (~0ul << (BFDEV_BITS_PER_LONG - 16)))) {
        shift -= 16;
        value <<= 16;
    }
    if (!(value & (~0ul << (BFDEV_BITS_PER_LONG - 8)))) {
        shift -= 8;
        value <<= 8;
    }
    if (!(value & (~0ul << (BFDEV_BITS_PER_LONG - 4)))) {
        shift -= 4;
        value <<= 4;
    }
    if (!(value & (~0ul << (BFDEV_BITS_PER_LONG - 2)))) {
        shift -= 2;
        value <<= 2;
    }
    if (!(value & (~0ul << (BFDEV_BITS_PER_LONG - 1))))
        shift -= 1;

    return shift;
}
#endif

/*
 * bfdev_arch_ffzuf - find first zero in word (unsafe).
 * @word: The word to search.
 * Undefined if no zero exists, so code should check against ~0UL first..
 */
#ifndef bfdev_arch_ffzuf
static __bfdev_always_inline unsigned int
bfdev_arch_ffzuf(unsigned long value)
{
    return bfdev_arch_ffsuf(~value);
}
#endif

/*
 * bfdev_arch_flzuf - find last zero in word (unsafe).
 * @word: The word to search.
 * Undefined if no zero exists, so code should check against ~0UL first..
 */
#ifndef bfdev_arch_flzuf
static __bfdev_always_inline unsigned int
bfdev_arch_flzuf(unsigned long value)
{
    return bfdev_arch_flsuf(~value);
}
#endif

/**
 * bfdev_arch_ffs - find first bit set.
 * @value: the word to search.
 */
#ifndef bfdev_arch_ffs
static __bfdev_always_inline unsigned int
bfdev_arch_ffs(unsigned long value)
{
    if (!value)
        return 0;
    return bfdev_arch_ffsuf(value) + 1;
}
#endif

/**
 * bfdev_arch_fls - find last zero in word.
 * @value: the word to search.
 */
#ifndef bfdev_arch_fls
static __bfdev_always_inline unsigned int
bfdev_arch_fls(unsigned long value)
{
    if (!value)
        return 0;
    return bfdev_arch_flsuf(value) + 1;
}
#endif

/*
 * bfdev_arch_ffz - find first zero in word.
 * @value: The word to search.
 */
#ifndef bfdev_arch_ffz
static __bfdev_always_inline unsigned int
bfdev_arch_ffz(unsigned long value)
{
    if (value == ULONG_MAX)
        return 0;
    return bfdev_arch_ffs(~value);
}
#endif

/*
 * bfdev_arch_flz - find last zero in word.
 * @value: The word to search.
 */
#ifndef bfdev_arch_flz
static __bfdev_always_inline unsigned int
bfdev_arch_flz(unsigned long value)
{
    if (value == ULONG_MAX)
        return 0;
    return bfdev_arch_fls(~value);
}
#endif

/*
 * bfdev_arch_ctz - returns the number of trailing 0-bits in value.
 * @value: The word to search.
 */
#ifndef bfdev_arch_ctz
static __bfdev_always_inline unsigned int
bfdev_arch_ctz(unsigned long value)
{
    if (value == ULONG_MAX)
        return 0;
    if (!value)
        return BFDEV_BITS_PER_LONG;
    return bfdev_arch_ffsuf(value);
}
#endif

/*
 * bfdev_arch_clz - returns the number of leading 0-bits in value.
 * @value: The word to search.
 */
#ifndef bfdev_arch_clz
static __bfdev_always_inline unsigned int
bfdev_arch_clz(unsigned long value)
{
    if (value == ULONG_MAX)
        return 0;
    if (!value)
        return BFDEV_BITS_PER_LONG;
    return BFDEV_BITS_PER_LONG - 1 - bfdev_arch_flsuf(value);
}
#endif

#ifndef bfdev_arch_ffns
static __bfdev_always_inline unsigned int
bfdev_arch_ffnsp(unsigned long word, unsigned int *nr)
{
    unsigned int bit;

    if (word == ULONG_MAX)
        return *nr;

    while (word) {
        bit = bfdev_arch_ffsuf(word);
        if (!(*nr)--)
            return bit;
        bfdev_arch_bit_clr(&word, bit);
    }

    return BFDEV_BITS_PER_LONG;
}
#endif

#ifndef bfdev_arch_flns
static __bfdev_always_inline unsigned int
bfdev_arch_flnsp(unsigned long word, unsigned int *nr)
{
    unsigned int bit;

    if (word == ULONG_MAX)
        return *nr;

    while (word) {
        bit = bfdev_arch_flsuf(word);
        if (!(*nr)--)
            return bit;
        bfdev_arch_bit_clr(&word, bit);
    }

    return BFDEV_BITS_PER_LONG;
}
#endif

#ifndef bfdev_arch_ffnz
static __bfdev_always_inline unsigned int
bfdev_arch_ffnzp(unsigned long word, unsigned int *nr)
{
    return bfdev_arch_ffnsp(~word, nr);
}
#endif

#ifndef bfdev_arch_flnz
static __bfdev_always_inline unsigned int
bfdev_arch_flnzp(unsigned long word, unsigned int *nr)
{
    return bfdev_arch_flnsp(~word, nr);
}
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_BITOPS_H_ */
