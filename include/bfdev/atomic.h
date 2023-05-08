/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ATOMIC_H_
#define _BFDEV_ATOMIC_H_

#include <bfdev/asm/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * bfdev_atomic_read - atomic read variable.
 * @atomic: pointer of type atomic_t.
 */
static __always_inline bfdev_atomic_t
bfdev_atomic_read(const bfdev_atomic_t *atomic)
{
    return bfdev_arch_atomic_read(atomic);
}

/**
 * bfdev_atomic_write - atomic write variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline void
bfdev_atomic_write(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_write(atomic, value);
}

/**
 * bfdev_atomic_add - atomic add variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline void
bfdev_atomic_add(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_add(atomic, value);
}

/**
 * bfdev_atomic_sub - atomic subtract variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline void
bfdev_atomic_sub(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_sub(atomic, value);
}

/**
 * bfdev_atomic_and - atomic and variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline void
bfdev_atomic_and(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_and(atomic, value);
}

/**
 * bfdev_atomic_or - atomic or variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline void
bfdev_atomic_or(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_or(atomic, value);
}

/**
 * bfdev_atomic_xor - atomic xor variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline void
bfdev_atomic_xor(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_xor(atomic, value);
}

/**
 * bfdev_atomic_nand - atomic nand variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline void
bfdev_atomic_nand(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_nand(atomic, value);
}

/**
 * bfdev_atomic_fetch_add - fetch and atomic add variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline bfdev_atomic_t
bfdev_atomic_fetch_add(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_fetch_add(atomic, value);
}

/**
 * bfdev_atomic_fetch_sub - fetch and atomic subtract variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline bfdev_atomic_t
bfdev_atomic_fetch_sub(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_fetch_sub(atomic, value);
}

/**
 * bfdev_atomic_fetch_and - fetch and atomic and variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline bfdev_atomic_t
bfdev_atomic_fetch_and(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_fetch_and(atomic, value);
}

/**
 * bfdev_atomic_fetch_or - fetch and atomic or variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline bfdev_atomic_t
bfdev_atomic_fetch_or(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_fetch_or(atomic, value);
}

/**
 * bfdev_atomic_fetch_xor - fetch and atomic xor variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline bfdev_atomic_t
bfdev_atomic_fetch_xor(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_fetch_xor(atomic, value);
}

/**
 * bfdev_atomic_fetch_nand - fetch and atomic nand variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline bfdev_atomic_t
bfdev_atomic_fetch_nand(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_fetch_nand(atomic, value);
}

/**
 * bfdev_atomic_add_fetch - atomic add variable and fetch.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline bfdev_atomic_t
bfdev_atomic_add_fetch(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_add_fetch(atomic, value);
}

/**
 * bfdev_atomic_sub_fetch - atomic subtract variable and fetch.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline bfdev_atomic_t
bfdev_atomic_sub_fetch(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_sub_fetch(atomic, value);
}

/**
 * bfdev_atomic_and_fetch - atomic and variable and fetch.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline bfdev_atomic_t
bfdev_atomic_and_fetch(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_and_fetch(atomic, value);
}

/**
 * bfdev_atomic_or_fetch - atomic or variable and fetch.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline bfdev_atomic_t
bfdev_atomic_or_fetch(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_or_fetch(atomic, value);
}

/**
 * bfdev_atomic_xor_fetch - atomic xor variable and fetch.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline bfdev_atomic_t
bfdev_atomic_xor_fetch(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_xor_fetch(atomic, value);
}

/**
 * bfdev_atomic_nand_fetch - atomic nand variable and fetch.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
static __always_inline bfdev_atomic_t
bfdev_atomic_nand_fetch(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_nand_fetch(atomic, value);
}

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_ATOMIC_H_ */
