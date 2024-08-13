/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BITOPS_H_
#define _BFDEV_BITOPS_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/math.h>
#include <bfdev/bits.h>
#include <bfdev/asm/bitops.h>

BFDEV_BEGIN_DECLS

#define BFDEV_BITS_DIV_U8(nr)           ((nr) / BFDEV_BITS_PER_U8)
#define BFDEV_BITS_DIV_U16(nr)          ((nr) / BFDEV_BITS_PER_U16)
#define BFDEV_BITS_DIV_U32(nr)          ((nr) / BFDEV_BITS_PER_U32)
#define BFDEV_BITS_DIV_U64(nr)          ((nr) / BFDEV_BITS_PER_U64)
#define BFDEV_BITS_DIV_CHAR(nr)         ((nr) / BFDEV_BITS_PER_CHAR)
#define BFDEV_BITS_DIV_SHORT(nr)        ((nr) / BFDEV_BITS_PER_SHORT)
#define BFDEV_BITS_DIV_INT(nr)          ((nr) / BFDEV_BITS_PER_INT)
#define BFDEV_BITS_DIV_LONG(nr)         ((nr) / BFDEV_BITS_PER_LONG)
#define BFDEV_BITS_DIV_LONG_LONG(nr)    ((nr) / BFDEV_BITS_PER_LONG_LONG)

#define BFDEV_BITS_MOD_U8(nr)           ((nr) % BFDEV_BITS_PER_U8)
#define BFDEV_BITS_MOD_U16(nr)          ((nr) % BFDEV_BITS_PER_U16)
#define BFDEV_BITS_MOD_U32(nr)          ((nr) % BFDEV_BITS_PER_U32)
#define BFDEV_BITS_MOD_U64(nr)          ((nr) % BFDEV_BITS_PER_U64)
#define BFDEV_BITS_MOD_CHAR(nr)         ((nr) % BFDEV_BITS_PER_CHAR)
#define BFDEV_BITS_MOD_SHORT(nr)        ((nr) % BFDEV_BITS_PER_SHORT)
#define BFDEV_BITS_MOD_INT(nr)          ((nr) % BFDEV_BITS_PER_INT)
#define BFDEV_BITS_MOD_LONG(nr)         ((nr) % BFDEV_BITS_PER_LONG)
#define BFDEV_BITS_MOD_LONG_LONG(nr)    ((nr) % BFDEV_BITS_PER_LONG_LONG)

#define BFDEV_BITS_TO_U8(nr)            BFDEV_DIV_ROUND_UP(nr, BFDEV_BITS_PER_U8)
#define BFDEV_BITS_TO_U16(nr)           BFDEV_DIV_ROUND_UP(nr, BFDEV_BITS_PER_U16)
#define BFDEV_BITS_TO_U32(nr)           BFDEV_DIV_ROUND_UP(nr, BFDEV_BITS_PER_U32)
#define BFDEV_BITS_TO_U64(nr)           BFDEV_DIV_ROUND_UP(nr, BFDEV_BITS_PER_U64)
#define BFDEV_BITS_TO_CHAR(nr)          BFDEV_DIV_ROUND_UP(nr, BFDEV_BITS_PER_CHAR)
#define BFDEV_BITS_TO_SHORT(nr)         BFDEV_DIV_ROUND_UP(nr, BFDEV_BITS_PER_SHORT)
#define BFDEV_BITS_TO_INT(nr)           BFDEV_DIV_ROUND_UP(nr, BFDEV_BITS_PER_INT)
#define BFDEV_BITS_TO_LONG(nr)          BFDEV_DIV_ROUND_UP(nr, BFDEV_BITS_PER_LONG)
#define BFDEV_BITS_TO_LONG_LONG(nr)     BFDEV_DIV_ROUND_UP(nr, BFDEV_BITS_PER_LONG_LONG)

#ifndef bfdev_bit_clr
static __bfdev_always_inline void
bfdev_bit_clr(volatile unsigned long *addr, unsigned int bit)
{
    bfdev_arch_bit_clr(addr, bit);
}
#endif

#ifndef bfdev_bit_set
static __bfdev_always_inline void
bfdev_bit_set(volatile unsigned long *addr, unsigned int bit)
{
    bfdev_arch_bit_set(addr, bit);
}
#endif

#ifndef bfdev_bit_flip
static __bfdev_always_inline void
bfdev_bit_flip(volatile unsigned long *addr, unsigned int bit)
{
    bfdev_arch_bit_flip(addr, bit);
}
#endif

#ifndef bfdev_bit_change
static __bfdev_always_inline void
bfdev_bit_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    bfdev_arch_bit_change(addr, bit, val);
}
#endif

#ifndef bfdev_bit_test
static __bfdev_always_inline bool
bfdev_bit_test(const volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_test(addr, bit);
}
#endif

#ifndef bfdev_bit_test_clr
static __bfdev_always_inline bool
bfdev_bit_test_clr(volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_test_clr(addr, bit);
}
#endif

#ifndef bfdev_bit_test_set
static __bfdev_always_inline bool
bfdev_bit_test_set(volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_test_set(addr, bit);
}
#endif

#ifndef bfdev_bit_test_flip
static __bfdev_always_inline bool
bfdev_bit_test_flip(volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_test_flip(addr, bit);
}
#endif

#ifndef bfdev_bit_test_change
static __bfdev_always_inline bool
bfdev_bit_test_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    return bfdev_arch_bit_test_change(addr, bit, val);
}
#endif

#ifndef bfdev_bit_atomic_clr
static __bfdev_always_inline void
bfdev_bit_atomic_clr(volatile unsigned long *addr, unsigned int bit)
{
    bfdev_arch_bit_atomic_clr(addr, bit);
}
#endif

#ifndef bfdev_bit_atomic_set
static __bfdev_always_inline void
bfdev_bit_atomic_set(volatile unsigned long *addr, unsigned int bit)
{
    bfdev_arch_bit_atomic_set(addr, bit);
}
#endif

#ifndef bfdev_bit_atomic_flip
static __bfdev_always_inline void
bfdev_bit_atomic_flip(volatile unsigned long *addr, unsigned int bit)
{
    bfdev_arch_bit_atomic_flip(addr, bit);
}
#endif

#ifndef bfdev_bit_atomic_change
static __bfdev_always_inline void
bfdev_bit_atomic_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    bfdev_arch_bit_atomic_change(addr, bit, val);
}
#endif

#ifndef bfdev_bit_atomic_test
static __bfdev_always_inline bool
bfdev_bit_atomic_test(const volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_atomic_test(addr, bit);
}
#endif

#ifndef bfdev_bit_atomic_test_clr
static __bfdev_always_inline bool
bfdev_bit_atomic_test_clr(volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_atomic_test_clr(addr, bit);
}
#endif

#ifndef bfdev_bit_atomic_test_set
static __bfdev_always_inline bool
bfdev_bit_atomic_test_set(volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_atomic_test_set(addr, bit);
}
#endif

#ifndef bfdev_bit_atomic_test_flip
static __bfdev_always_inline bool
bfdev_bit_atomic_test_flip(volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_atomic_test_flip(addr, bit);
}
#endif

#ifndef bfdev_bit_atomic_test_change
static __bfdev_always_inline bool
bfdev_bit_atomic_test_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    return bfdev_arch_bit_atomic_test_change(addr, bit, val);
}
#endif

#ifndef bfdev_rol8
static __bfdev_always_inline uint8_t
bfdev_rol8(uint8_t value, unsigned int shift)
{
    return bfdev_arch_rol8(value, shift);
}
#endif

#ifndef bfdev_ror8
static __bfdev_always_inline uint8_t
bfdev_ror8(uint8_t value, unsigned int shift)
{
    return bfdev_arch_ror8(value, shift);
}
#endif

#ifndef bfdev_rol16
static __bfdev_always_inline uint16_t
bfdev_rol16(uint16_t value, unsigned int shift)
{
    return bfdev_arch_rol16(value, shift);
}
#endif

#ifndef bfdev_ror16
static __bfdev_always_inline uint16_t
bfdev_ror16(uint16_t value, unsigned int shift)
{
    return bfdev_arch_ror16(value, shift);
}
#endif

#ifndef bfdev_rol32
static __bfdev_always_inline uint32_t
bfdev_rol32(uint32_t value, unsigned int shift)
{
    return bfdev_arch_rol32(value, shift);
}
#endif

#ifndef bfdev_ror32
static __bfdev_always_inline uint32_t
bfdev_ror32(uint32_t value, unsigned int shift)
{
    return bfdev_arch_ror32(value, shift);
}
#endif

#ifndef bfdev_rol64
static __bfdev_always_inline uint64_t
bfdev_rol64(uint64_t value, unsigned int shift)
{
    return bfdev_arch_rol64(value, shift);
}
#endif

#ifndef bfdev_ror64
static __bfdev_always_inline uint64_t
bfdev_ror64(uint64_t value, unsigned int shift)
{
    return bfdev_arch_ror64(value, shift);
}
#endif

#ifndef bfdev_ffsuf
static __bfdev_always_inline unsigned int
bfdev_ffsuf(unsigned long value)
{
    return bfdev_arch_ffsuf(value);
}
#endif

#ifndef bfdev_flsuf
static __bfdev_always_inline unsigned int
bfdev_flsuf(unsigned long value)
{
    return bfdev_arch_flsuf(value);
}
#endif

#ifndef bfdev_ffzuf
static __bfdev_always_inline unsigned int
bfdev_ffzuf(unsigned long value)
{
    return bfdev_arch_ffzuf(value);
}
#endif

#ifndef bfdev_flzuf
static __bfdev_always_inline unsigned int
bfdev_flzuf(unsigned long value)
{
    return bfdev_arch_flzuf(value);
}
#endif

#ifndef bfdev_ffs
static __bfdev_always_inline unsigned int
bfdev_ffs(unsigned long value)
{
    return bfdev_arch_ffs(value);
}
#endif

#ifndef bfdev_fls
static __bfdev_always_inline unsigned int
bfdev_fls(unsigned long value)
{
    return bfdev_arch_fls(value);
}
#endif

#ifndef bfdev_ffz
static __bfdev_always_inline unsigned int
bfdev_ffz(unsigned long value)
{
    return bfdev_arch_ffz(value);
}
#endif

#ifndef bfdev_flz
static __bfdev_always_inline unsigned int
bfdev_flz(unsigned long value)
{
    return bfdev_arch_flz(value);
}
#endif

#ifndef bfdev_ctz
static __bfdev_always_inline unsigned int
bfdev_ctz(unsigned long value)
{
    return bfdev_arch_ctz(value);
}
#endif

#ifndef bfdev_clz
static __bfdev_always_inline unsigned int
bfdev_clz(unsigned long value)
{
    return bfdev_arch_clz(value);
}
#endif

#ifndef bfdev_ffns
static __bfdev_always_inline unsigned int
bfdev_ffnsp(unsigned long word, unsigned int *nr)
{
    return bfdev_arch_ffnsp(word, nr);
}
#endif

#ifndef bfdev_flns
static __bfdev_always_inline unsigned int
bfdev_flnsp(unsigned long word, unsigned int *nr)
{
    return bfdev_arch_flnsp(word, nr);
}
#endif

#ifndef bfdev_ffnz
static __bfdev_always_inline unsigned int
bfdev_ffnzp(unsigned long word, unsigned int *nr)
{
    return bfdev_arch_ffnzp(word, nr);
}
#endif

#ifndef bfdev_fznz
static __bfdev_always_inline unsigned int
bfdev_flnzp(unsigned long word, unsigned int *nr)
{
    return bfdev_arch_flnzp(word, nr);
}
#endif

#ifndef bfdev_ffns
static __bfdev_always_inline unsigned int
bfdev_ffns(unsigned long word, unsigned int nr)
{
    return bfdev_ffnsp(word, &nr);
}
#endif

#ifndef bfdev_flns
static __bfdev_always_inline unsigned int
bfdev_flns(unsigned long word, unsigned int nr)
{
    return bfdev_flnsp(word, &nr);
}
#endif

#ifndef bfdev_ffnz
static __bfdev_always_inline unsigned int
bfdev_ffnz(unsigned long word, unsigned int nr)
{
    return bfdev_ffnzp(word, &nr);
}
#endif

#ifndef bfdev_fznz
static __bfdev_always_inline unsigned int
bfdev_flnz(unsigned long word, unsigned int nr)
{
    return bfdev_flnzp(word, &nr);
}
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_ffsuf64(uint64_t value)
{
    uint32_t hi = value >> 32;

    if ((uint32_t)value)
        return bfdev_ffsuf((uint32_t)value);

    return bfdev_ffsuf(hi) + 32;
}
#else
# define bfdev_ffsuf64 bfdev_ffsuf
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_flsuf64(uint64_t value)
{
    uint32_t hi = value >> 32;

    if (hi)
        return bfdev_flsuf(hi) + 32;

    return bfdev_flsuf((uint32_t)value);
}
#else
# define bfdev_flsuf64 bfdev_flsuf
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_ffzuf64(uint64_t value)
{
    uint32_t hi = value >> 32;

    if ((uint32_t)value)
        return bfdev_ffzuf((uint32_t)value);

    return bfdev_ffzuf(hi) + 32;
}
#else
# define bfdev_ffzuf64 bfdev_ffzuf
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_flzuf64(uint64_t value)
{
    uint32_t hi = value >> 32;

    if (hi)
        return bfdev_flzuf(hi) + 32;

    return bfdev_flzuf((uint32_t)value);
}
#else
# define bfdev_flzuf64 bfdev_flzuf
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_ffs64(uint64_t value)
{
    uint32_t hi = value >> 32;

    if ((uint32_t)value)
        return bfdev_ffs((uint32_t)value);

    return bfdev_ffs(hi) + 32;
}
#else
# define bfdev_ffs64 bfdev_ffs
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_fls64(uint64_t value)
{
    uint32_t hi = value >> 32;

    if (hi)
        return bfdev_fls(hi) + 32;

    return bfdev_fls((uint32_t)value);
}
#else
# define bfdev_fls64 bfdev_fls
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_ffz64(uint64_t value)
{
    uint32_t hi = value >> 32;

    if ((uint32_t)value)
        return bfdev_ffz((uint32_t)value);

    return bfdev_ffz(hi) + 32;
}
#else
# define bfdev_ffz64 bfdev_ffz
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_flz64(uint64_t value)
{
    uint32_t hi = value >> 32;

    if (hi)
        return bfdev_flz(hi) + 32;

    return bfdev_flz((uint32_t)value);
}
#else
# define bfdev_flz64 bfdev_flz
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_ctz64(uint64_t value)
{
    uint32_t hi = value >> 32;

    if ((uint32_t)value)
        return bfdev_ctz(value);

    return bfdev_ctz(hi) + 32;
}
#else
# define bfdev_ctz64 bfdev_ctz
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_clz64(uint64_t value)
{
    uint32_t hi = value >> 32;

    if (hi)
        return bfdev_clz(hi);

    return bfdev_clz((uint32_t)value) + 32;
}
#else
# define bfdev_clz64 bfdev_clz
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_ffnsp64(uint64_t word, unsigned int *nr)
{
    uint32_t hi = word >> 32;
    unsigned int bit;

    bit = bfdev_ffnsp((uint32_t)word, nr);
    if (bit < BFDEV_BITS_PER_LONG)
        return bit;

    return bfdev_ffnsp(hi, nr) + 32;
}
#else
# define bfdev_ffnsp64 bfdev_ffnsp
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_flnsp64(uint64_t word, unsigned int *nr)
{
    uint32_t hi = word >> 32;
    unsigned int bit;

    bit = bfdev_flnsp(hi, nr);
    if (bit < BFDEV_BITS_PER_LONG)
        return bit + 32;

    return bfdev_flnsp((uint32_t)word, nr);
}
#else
# define bfdev_flnsp64 bfdev_flns
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_ffnzp64(uint64_t word, unsigned int *nr)
{
    uint32_t hi = word >> 32;
    unsigned int bit;

    bit = bfdev_ffnzp((uint32_t)word, nr);
    if (bit < BFDEV_BITS_PER_LONG)
        return bit;

    return bfdev_ffnzp(hi, nr) + 32;
}
#else
# define bfdev_ffnzp64 bfdev_ffnzp
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_flnzp64(uint64_t word, unsigned int *nr)
{
    uint32_t hi = word >> 32;
    unsigned int bit;

    bit = bfdev_flnzp(hi, nr);
    if (bit < BFDEV_BITS_PER_LONG)
        return bit + 32;

    return bfdev_flnzp((uint32_t)word, nr);
}
#else
# define bfdev_flnzp64 bfdev_flnzp
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_ffns64(uint64_t word, unsigned int nr)
{
    return bfdev_ffnsp64(word, &nr);
}
#else
# define bfdev_ffns64 bfdev_ffns
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_flns64(uint64_t word, unsigned int nr)
{
    return bfdev_flnsp64(word, &nr);
}
#else
# define bfdev_flns64 bfdev_flns
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_ffnz64(uint64_t word, unsigned int nr)
{
    return bfdev_ffnzp64(word, &nr);
}
#else
# define bfdev_ffnz64 bfdev_ffnz
#endif

#if BFDEV_BITS_PER_LONG == 32
static __bfdev_always_inline unsigned int
bfdev_flnz64(uint64_t word, unsigned int nr)
{
    return bfdev_flnzp64(word, &nr);
}
#else
# define bfdev_flnz64 bfdev_flnz
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_BITOPS_H_ */
