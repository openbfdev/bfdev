/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BITOPS_H_
#define _BFDEV_BITOPS_H_

#include <bfdev/config.h>
#include <bfdev/stdbool.h>
#include <bfdev/asm/bitops.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef bfdev_bit_clr
static __always_inline void
bfdev_bit_clr(volatile unsigned long *addr, unsigned int bit)
{
    bfdev_arch_bit_clr(addr, bit);
}
#endif

#ifndef bfdev_bit_set
static __always_inline void
bfdev_bit_set(volatile unsigned long *addr, unsigned int bit)
{
    bfdev_arch_bit_set(addr, bit);
}
#endif

#ifndef bfdev_bit_flip
static __always_inline void
bfdev_bit_flip(volatile unsigned long *addr, unsigned int bit)
{
    bfdev_arch_bit_flip(addr, bit);
}
#endif

#ifndef bfdev_bit_change
static __always_inline void
bfdev_bit_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    bfdev_arch_bit_change(addr, bit, val);
}
#endif

#ifndef bfdev_bit_test
static __always_inline bool
bfdev_bit_test(volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_test(addr, bit);
}
#endif

#ifndef bfdev_bit_test_clr
static __always_inline bool
bfdev_bit_test_clr(volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_test_clr(addr, bit);
}
#endif

#ifndef bfdev_bit_test_set
static __always_inline bool
bfdev_bit_test_set(volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_test_set(addr, bit);
}
#endif

#ifndef bfdev_bit_test_flip
static __always_inline bool
bfdev_bit_test_flip(volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_test_flip(addr, bit);
}
#endif

#ifndef bfdev_bit_test_change
static __always_inline bool
bfdev_bit_test_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    return bfdev_arch_bit_test_change(addr, bit, val);
}
#endif

#ifndef bfdev_bit_atomic_clr
static __always_inline void
bfdev_bit_atomic_clr(volatile unsigned long *addr, unsigned int bit)
{
    bfdev_arch_bit_atomic_clr(addr, bit);
}
#endif

#ifndef bfdev_bit_atomic_set
static __always_inline void
bfdev_bit_atomic_set(volatile unsigned long *addr, unsigned int bit)
{
    bfdev_arch_bit_atomic_set(addr, bit);
}
#endif

#ifndef bfdev_bit_atomic_flip
static __always_inline void
bfdev_bit_atomic_flip(volatile unsigned long *addr, unsigned int bit)
{
    bfdev_arch_bit_atomic_flip(addr, bit);
}
#endif

#ifndef bfdev_bit_atomic_change
static __always_inline void
bfdev_bit_atomic_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    bfdev_arch_bit_atomic_change(addr, bit, val);
}
#endif

#ifndef bfdev_bit_atomic_test
static __always_inline bool
bfdev_bit_atomic_test(volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_atomic_test(addr, bit);
}
#endif

#ifndef bfdev_bit_atomic_test_clr
static __always_inline bool
bfdev_bit_atomic_test_clr(volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_atomic_test_clr(addr, bit);
}
#endif

#ifndef bfdev_bit_atomic_test_set
static __always_inline bool
bfdev_bit_atomic_test_set(volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_atomic_test_set(addr, bit);
}
#endif

#ifndef bfdev_bit_atomic_test_flip
static __always_inline bool
bfdev_bit_atomic_test_flip(volatile unsigned long *addr, unsigned int bit)
{
    return bfdev_arch_bit_atomic_test_flip(addr, bit);
}
#endif

#ifndef bfdev_bit_atomic_test_change
static __always_inline bool
bfdev_bit_atomic_test_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    return bfdev_arch_bit_atomic_test_change(addr, bit, val);
}
#endif

#ifndef bfdev_ffsuf
static __always_inline unsigned int
bfdev_ffsuf(unsigned long value)
{
    return bfdev_arch_ffsuf(value);
}
#endif

#ifndef bfdev_flsuf
static __always_inline unsigned int
bfdev_flsuf(unsigned long value)
{
    return bfdev_arch_flsuf(value);
}
#endif

#ifndef bfdev_ffzuf
static __always_inline unsigned int
bfdev_ffzuf(unsigned long value)
{
    return bfdev_arch_ffzuf(value);
}
#endif

#ifndef bfdev_flzuf
static __always_inline unsigned int
bfdev_flzuf(unsigned long value)
{
    return bfdev_arch_flzuf(value);
}
#endif

#ifndef bfdev_ffs
static __always_inline unsigned int
bfdev_ffs(unsigned long value)
{
    return bfdev_arch_ffs(value);
}
#endif

#ifndef bfdev_fls
static __always_inline unsigned int
bfdev_fls(unsigned long value)
{
    return bfdev_arch_fls(value);
}
#endif

#ifndef bfdev_ffz
static __always_inline unsigned int
bfdev_ffz(unsigned long value)
{
    return bfdev_arch_ffz(value);
}
#endif

#ifndef bfdev_flz
static __always_inline unsigned int
bfdev_flz(unsigned long value)
{
    return bfdev_arch_flz(value);
}
#endif

#ifndef bfdev_ctz
static __always_inline unsigned int
bfdev_ctz(unsigned long value)
{
    return bfdev_arch_ctz(value);
}
#endif

#ifndef bfdev_clz
static __always_inline unsigned int
bfdev_clz(unsigned long value)
{
    return bfdev_arch_clz(value);
}
#endif

#if BFDEV_BITS_PER_LONG == 32
static __always_inline unsigned int
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
static __always_inline unsigned int
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
static __always_inline unsigned int
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
static __always_inline unsigned int
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
static __always_inline unsigned int
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
static __always_inline unsigned int
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
static __always_inline unsigned int
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
static __always_inline unsigned int
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
static __always_inline unsigned int
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
static __always_inline unsigned int
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

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_BITOPS_H_ */
