/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_CMPXCHG_H_
#define _BFDEV_ASM_GENERIC_CMPXCHG_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/asm/rwonce.h>

BFDEV_BEGIN_DECLS

#ifndef bfdev_arch_cmpxchg
# define bfdev_arch_cmpxchg bfdev_arch_cmpxchg
static __bfdev_always_inline bfdev_atomic_t
bfdev_arch_cmpxchg(bfdev_atomic_t *atomic, bfdev_atomic_t old, bfdev_atomic_t value)
{
    return __sync_val_compare_and_swap(atomic, old, value);
}
#endif

#ifndef bfdev_arch_xchg
# define bfdev_arch_xchg bfdev_arch_xchg
static __bfdev_always_inline bfdev_atomic_t
bfdev_arch_xchg(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    bfdev_atomic_t prev, result;

    do {
        prev = *atomic;
        result = bfdev_arch_cmpxchg(atomic, prev, value);
    } while (bfdev_unlikely(result != prev));

    return result;
}
#endif

#ifndef bfdev_arch_try_cmpxchg
# define bfdev_arch_try_cmpxchg bfdev_arch_try_cmpxchg
static __bfdev_always_inline bool
bfdev_arch_try_cmpxchg(bfdev_atomic_t *atomic, bfdev_atomic_t *old, bfdev_atomic_t value)
{
    bfdev_atomic_t result, prev;

    prev = *old;
    result = bfdev_arch_cmpxchg(atomic, prev, value);
    if (bfdev_unlikely(result != prev))
        *old = result;

    return bfdev_likely(result == prev);
}
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_CMPXCHG_H_ */
