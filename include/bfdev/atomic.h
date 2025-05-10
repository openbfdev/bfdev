/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ATOMIC_H_
#define _BFDEV_ATOMIC_H_

#include <bfdev/config.h>
#include <bfdev/asm/atomic.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_atomic_read - atomic read variable.
 * @atomic: pointer of type atomic_t.
 */
#ifndef bfdev_atomic_read
static __bfdev_always_inline bfdev_atomic_t
bfdev_atomic_read(const bfdev_atomic_t *atomic)
{
    return bfdev_arch_atomic_read(atomic);
}
#endif

#ifndef bfdev_atomic_read_acquire
static __bfdev_always_inline bfdev_atomic_t
bfdev_atomic_read_acquire(const bfdev_atomic_t *atomic)
{
    return bfdev_arch_atomic_read_acquire(atomic);
}
#endif

#ifndef bfdev_atomic_read_relaxed
static __bfdev_always_inline bfdev_atomic_t
bfdev_atomic_read_relaxed(const bfdev_atomic_t *atomic)
{
    return bfdev_arch_atomic_read_relaxed(atomic);
}
#endif

/**
 * bfdev_atomic_write - atomic write variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_write
static __bfdev_always_inline void
bfdev_atomic_write(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_write(atomic, value);
}
#endif

#ifndef bfdev_atomic_write_release
static __bfdev_always_inline void
bfdev_atomic_write_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_write_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_write_relaxed
static __bfdev_always_inline void
bfdev_atomic_write_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_write_relaxed(atomic, value);
}
#endif

/**
 * bfdev_atomic_add - atomic add variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_add
static __bfdev_always_inline void
bfdev_atomic_add(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_add(atomic, value);
}
#endif

#ifndef bfdev_atomic_add_acquire
static __bfdev_always_inline void
bfdev_atomic_add_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_add_acquire(atomic, value);
}
#endif

#ifndef bfdev_atomic_add_release
static __bfdev_always_inline void
bfdev_atomic_add_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_add_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_add_relaxed
static __bfdev_always_inline void
bfdev_atomic_add_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_add_relaxed(atomic, value);
}
#endif

/**
 * bfdev_atomic_sub - atomic subtract variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_sub
static __bfdev_always_inline void
bfdev_atomic_sub(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_sub(atomic, value);
}
#endif

#ifndef bfdev_atomic_sub_acquire
static __bfdev_always_inline void
bfdev_atomic_sub_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_sub_acquire(atomic, value);
}
#endif

#ifndef bfdev_atomic_sub_release
static __bfdev_always_inline void
bfdev_atomic_sub_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_sub_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_sub_relaxed
static __bfdev_always_inline void
bfdev_atomic_sub_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_sub_relaxed(atomic, value);
}
#endif

/**
 * bfdev_atomic_and - atomic and variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_and
static __bfdev_always_inline void
bfdev_atomic_and(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_and(atomic, value);
}
#endif

#ifndef bfdev_atomic_and_acquire
static __bfdev_always_inline void
bfdev_atomic_and_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_and_acquire(atomic, value);
}
#endif

#ifndef bfdev_atomic_and_release
static __bfdev_always_inline void
bfdev_atomic_and_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_and_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_and_relaxed
static __bfdev_always_inline void
bfdev_atomic_and_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_and_relaxed(atomic, value);
}
#endif

/**
 * bfdev_atomic_or - atomic or variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_or
static __bfdev_always_inline void
bfdev_atomic_or(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_or(atomic, value);
}
#endif

#ifndef bfdev_atomic_or_acquire
static __bfdev_always_inline void
bfdev_atomic_or_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_or_acquire(atomic, value);
}
#endif

#ifndef bfdev_atomic_or_release
static __bfdev_always_inline void
bfdev_atomic_or_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_or_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_or_relaxed
static __bfdev_always_inline void
bfdev_atomic_or_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_or_relaxed(atomic, value);
}
#endif

/**
 * bfdev_atomic_xor - atomic xor variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_xor
static __bfdev_always_inline void
bfdev_atomic_xor(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_xor(atomic, value);
}
#endif

#ifndef bfdev_atomic_xor_acquire
static __bfdev_always_inline void
bfdev_atomic_xor_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_xor_acquire(atomic, value);
}
#endif

#ifndef bfdev_atomic_xor_release
static __bfdev_always_inline void
bfdev_atomic_xor_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_xor_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_xor_relaxed
static __bfdev_always_inline void
bfdev_atomic_xor_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_xor_relaxed(atomic, value);
}
#endif

/**
 * bfdev_atomic_fetch_add - fetch and atomic add variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_fetch_add
static __bfdev_always_inline bfdev_atomic_t
bfdev_atomic_fetch_add(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_fetch_add(atomic, value);
}
#endif

#ifndef bfdev_atomic_fetch_add_acquire
static __bfdev_always_inline void
bfdev_atomic_fetch_add_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_fetch_add_acquire(atomic, value);
}
#endif

#ifndef bfdev_atomic_fetch_add_release
static __bfdev_always_inline void
bfdev_atomic_fetch_add_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_fetch_add_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_fetch_add_relaxed
static __bfdev_always_inline void
bfdev_atomic_fetch_add_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_fetch_add_relaxed(atomic, value);
}
#endif

/**
 * bfdev_atomic_fetch_sub - fetch and atomic subtract variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_fetch_sub
static __bfdev_always_inline bfdev_atomic_t
bfdev_atomic_fetch_sub(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_fetch_sub(atomic, value);
}
#endif

#ifndef bfdev_atomic_fetch_sub_acquire
static __bfdev_always_inline void
bfdev_atomic_fetch_sub_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_fetch_sub_acquire(atomic, value);
}
#endif

#ifndef bfdev_atomic_fetch_sub_release
static __bfdev_always_inline void
bfdev_atomic_fetch_sub_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_fetch_sub_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_fetch_sub_relaxed
static __bfdev_always_inline void
bfdev_atomic_fetch_sub_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_fetch_sub_relaxed(atomic, value);
}
#endif

/**
 * bfdev_atomic_fetch_and - fetch and atomic and variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_fetch_and
static __bfdev_always_inline bfdev_atomic_t
bfdev_atomic_fetch_and(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_fetch_and(atomic, value);
}
#endif

#ifndef bfdev_atomic_fetch_and_acquire
static __bfdev_always_inline void
bfdev_atomic_fetch_and_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_fetch_and_acquire(atomic, value);
}
#endif

#ifndef bfdev_atomic_fetch_and_release
static __bfdev_always_inline void
bfdev_atomic_fetch_and_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_fetch_and_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_fetch_and_relaxed
static __bfdev_always_inline void
bfdev_atomic_fetch_and_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_fetch_and_relaxed(atomic, value);
}
#endif

/**
 * bfdev_atomic_fetch_or - fetch and atomic or variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_fetch_or
static __bfdev_always_inline bfdev_atomic_t
bfdev_atomic_fetch_or(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_fetch_or(atomic, value);
}
#endif

#ifndef bfdev_atomic_fetch_or_acquire
static __bfdev_always_inline void
bfdev_atomic_fetch_or_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_fetch_or_acquire(atomic, value);
}
#endif

#ifndef bfdev_atomic_fetch_or_release
static __bfdev_always_inline void
bfdev_atomic_fetch_or_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_fetch_or_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_fetch_or_relaxed
static __bfdev_always_inline void
bfdev_atomic_fetch_or_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_fetch_or_relaxed(atomic, value);
}
#endif

/**
 * bfdev_atomic_fetch_xor - fetch and atomic xor variable.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_fetch_xor
static __bfdev_always_inline bfdev_atomic_t
bfdev_atomic_fetch_xor(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_fetch_xor(atomic, value);
}
#endif

#ifndef bfdev_atomic_fetch_xor_acquire
static __bfdev_always_inline void
bfdev_atomic_fetch_xor_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_fetch_xor_acquire(atomic, value);
}
#endif

#ifndef bfdev_atomic_fetch_xor_release
static __bfdev_always_inline void
bfdev_atomic_fetch_xor_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_fetch_xor_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_fetch_xor_relaxed
static __bfdev_always_inline void
bfdev_atomic_fetch_xor_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_fetch_xor_relaxed(atomic, value);
}
#endif

/**
 * bfdev_atomic_add_fetch - atomic add variable and fetch.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_add_fetch
static __bfdev_always_inline bfdev_atomic_t
bfdev_atomic_add_fetch(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_add_fetch(atomic, value);
}
#endif

#ifndef bfdev_atomic_add_fetch_acquire
static __bfdev_always_inline void
bfdev_atomic_add_fetch_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_add_fetch_acquire(atomic, value);
}
#endif

#ifndef bfdev_atomic_add_fetch_release
static __bfdev_always_inline void
bfdev_atomic_add_fetch_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_add_fetch_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_add_fetch_relaxed
static __bfdev_always_inline void
bfdev_atomic_add_fetch_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_add_fetch_relaxed(atomic, value);
}
#endif

/**
 * bfdev_atomic_sub_fetch - atomic subtract variable and fetch.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_sub_fetch
static __bfdev_always_inline bfdev_atomic_t
bfdev_atomic_sub_fetch(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_sub_fetch(atomic, value);
}
#endif

#ifndef bfdev_atomic_sub_fetch_acquire
static __bfdev_always_inline void
bfdev_atomic_sub_fetch_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_sub_fetch_acquire(atomic, value);
}
#endif

#ifndef bfdev_atomic_sub_fetch_release
static __bfdev_always_inline void
bfdev_atomic_sub_fetch_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_sub_fetch_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_sub_fetch_relaxed
static __bfdev_always_inline void
bfdev_atomic_sub_fetch_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_sub_fetch_relaxed(atomic, value);
}
#endif

/**
 * bfdev_atomic_and_fetch - atomic and variable and fetch.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_and_fetch
static __bfdev_always_inline bfdev_atomic_t
bfdev_atomic_and_fetch(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_and_fetch(atomic, value);
}
#endif

#ifndef bfdev_atomic_and_fetch_acquire
static __bfdev_always_inline void
bfdev_atomic_and_fetch_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_and_fetch_acquire(atomic, value);
}
#endif

#ifndef bfdev_atomic_and_fetch_release
static __bfdev_always_inline void
bfdev_atomic_and_fetch_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_and_fetch_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_and_fetch_relaxed
static __bfdev_always_inline void
bfdev_atomic_and_fetch_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_and_fetch_relaxed(atomic, value);
}
#endif

/**
 * bfdev_atomic_or_fetch - atomic or variable and fetch.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_or_fetch
static __bfdev_always_inline bfdev_atomic_t
bfdev_atomic_or_fetch(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_or_fetch(atomic, value);
}
#endif

#ifndef bfdev_atomic_or_fetch_acquire
static __bfdev_always_inline void
bfdev_atomic_or_fetch_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_or_fetch_acquire(atomic, value);
}
#endif

#ifndef bfdev_atomic_or_fetch_release
static __bfdev_always_inline void
bfdev_atomic_or_fetch_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_or_fetch_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_or_fetch_relaxed
static __bfdev_always_inline void
bfdev_atomic_or_fetch_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_or_fetch_relaxed(atomic, value);
}
#endif

/**
 * bfdev_atomic_xor_fetch - atomic xor variable and fetch.
 * @atomic: pointer of type atomic_t.
 * @value: required value.
 */
#ifndef bfdev_atomic_xor_fetch
static __bfdev_always_inline bfdev_atomic_t
bfdev_atomic_xor_fetch(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_atomic_xor_fetch(atomic, value);
}
#endif

#ifndef bfdev_atomic_xor_fetch_acquire
static __bfdev_always_inline void
bfdev_atomic_xor_fetch_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_xor_fetch_acquire(atomic, value);
}
#endif

#ifndef bfdev_atomic_xor_fetch_release
static __bfdev_always_inline void
bfdev_atomic_xor_fetch_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_xor_fetch_release(atomic, value);
}
#endif

#ifndef bfdev_atomic_xor_fetch_relaxed
static __bfdev_always_inline void
bfdev_atomic_xor_fetch_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_arch_atomic_xor_fetch_relaxed(atomic, value);
}
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_ATOMIC_H_ */
