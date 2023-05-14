/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_ATOMIC_H_
#define _BFDEV_ASM_GENERIC_ATOMIC_H_

#include <bfdev/config.h>
#include <bfdev/stdint.h>
#include <bfdev/attributes.h>
#include <bfdev/asm/rwonce.h>

#ifdef __cplusplus
extern "C" {
#endif

#define bfdev_arch_atomic_read bfdev_arch_atomic_read
static __always_inline bfdev_atomic_t
bfdev_arch_atomic_read(const bfdev_atomic_t *atomic)
{
    return BFDEV_READ_ONCE(*atomic);
}

#define bfdev_arch_atomic_write bfdev_arch_atomic_write
static __always_inline void
bfdev_arch_atomic_write(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    BFDEV_WRITE_ONCE(*atomic, value);
}

#define BFDEV_GENERIC_ATOMIC(name, func)                                \
static __always_inline void                                             \
bfdev_arch_atomic_##name(bfdev_atomic_t *atomic, bfdev_atomic_t value)  \
{                                                                       \
    (void)func(atomic, value);                                          \
}

#define BFDEV_GENERIC_ATOMIC_FETCH(name, func)                          \
static __always_inline bfdev_atomic_t                                   \
bfdev_arch_atomic_##name(bfdev_atomic_t *atomic, bfdev_atomic_t value)  \
{                                                                       \
    return func(atomic, value);                                         \
}

#define bfdev_arch_atomic_add bfdev_arch_atomic_add
#define bfdev_arch_atomic_sub bfdev_arch_atomic_sub
#define bfdev_arch_atomic_and bfdev_arch_atomic_and
#define bfdev_arch_atomic_or bfdev_arch_atomic_or
#define bfdev_arch_atomic_xor bfdev_arch_atomic_xor

BFDEV_GENERIC_ATOMIC(add, __sync_fetch_and_add)
BFDEV_GENERIC_ATOMIC(sub, __sync_fetch_and_sub)
BFDEV_GENERIC_ATOMIC(and, __sync_fetch_and_and)
BFDEV_GENERIC_ATOMIC(or, __sync_fetch_and_or)
BFDEV_GENERIC_ATOMIC(xor, __sync_fetch_and_xor)

#define bfdev_arch_atomic_fetch_add bfdev_arch_atomic_fetch_add
#define bfdev_arch_atomic_fetch_sub bfdev_arch_atomic_fetch_sub
#define bfdev_arch_atomic_fetch_and bfdev_arch_atomic_fetch_and
#define bfdev_arch_atomic_fetch_or bfdev_arch_atomic_fetch_or
#define bfdev_arch_atomic_fetch_xor bfdev_arch_atomic_fetch_xor

BFDEV_GENERIC_ATOMIC_FETCH(fetch_add, __sync_fetch_and_add)
BFDEV_GENERIC_ATOMIC_FETCH(fetch_sub, __sync_fetch_and_sub)
BFDEV_GENERIC_ATOMIC_FETCH(fetch_and, __sync_fetch_and_and)
BFDEV_GENERIC_ATOMIC_FETCH(fetch_or, __sync_fetch_and_or)
BFDEV_GENERIC_ATOMIC_FETCH(fetch_xor, __sync_fetch_and_xor)

#define bfdev_arch_atomic_add_fetch bfdev_arch_atomic_add_fetch
#define bfdev_arch_atomic_sub_fetch bfdev_arch_atomic_sub_fetch
#define bfdev_arch_atomic_and_fetch bfdev_arch_atomic_and_fetch
#define bfdev_arch_atomic_or_fetch bfdev_arch_atomic_or_fetch
#define bfdev_arch_atomic_xor_fetch bfdev_arch_atomic_xor_fetch

BFDEV_GENERIC_ATOMIC_FETCH(add_fetch, __sync_add_and_fetch)
BFDEV_GENERIC_ATOMIC_FETCH(sub_fetch, __sync_sub_and_fetch)
BFDEV_GENERIC_ATOMIC_FETCH(and_fetch, __sync_and_and_fetch)
BFDEV_GENERIC_ATOMIC_FETCH(or_fetch, __sync_or_and_fetch)
BFDEV_GENERIC_ATOMIC_FETCH(xor_fetch, __sync_xor_and_fetch)

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_ASM_GENERIC_ATOMIC_H_ */
