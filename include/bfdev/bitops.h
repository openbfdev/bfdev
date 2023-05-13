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

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_BITOPS_H_ */
