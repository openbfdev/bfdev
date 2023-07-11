/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_CMPXCHG_H_
#define _BFDEV_ASM_GENERIC_CMPXCHG_H_

#include <bfdev/config.h>
#include <bfdev/stdint.h>

BFDEV_BEGIN_DECLS

#define bfdev_arch_xchg bfdev_arch_xchg
static __bfdev_always_inline bfdev_atomic_t
bfdev_arch_xchg(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return __sync_bool_compare_and_swap(atomic, *atomic, value);
}

#define bfdev_arch_cmpxchg bfdev_arch_cmpxchg
static __bfdev_always_inline bfdev_atomic_t
bfdev_arch_cmpxchg(bfdev_atomic_t *atomic, bfdev_atomic_t old, bfdev_atomic_t value)
{
    return __sync_bool_compare_and_swap(atomic, old, value);
}

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_CMPXCHG_H_ */
