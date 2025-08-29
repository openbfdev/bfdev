/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_CMPXCHG_H_
#define _BFDEV_ASM_GENERIC_CMPXCHG_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/asm/rwonce.h>

BFDEV_BEGIN_DECLS

#define BFDEV_GENERIC_XCHG(name, order)                         \
static __bfdev_always_inline bfdev_atomic_t                     \
bfdev_arch_##name(bfdev_atomic_t *atomic, bfdev_atomic_t value) \
{                                                               \
    bfdev_atomic_t result;                                      \
    __atomic_exchange(atomic, &value, &result, order);          \
    return result;                                              \
}

#define BFDEV_GENERIC_CMPXCHG(name, weak, okay, fail)           \
static __bfdev_always_inline bfdev_atomic_t                     \
bfdev_arch_##name(bfdev_atomic_t *atomic, bfdev_atomic_t old,   \
                  bfdev_atomic_t value)                         \
{                                                               \
    __atomic_compare_exchange(atomic, &old, &value,             \
        weak, okay, fail);                                      \
    return old;                                                 \
}

#define BFDEV_GENERIC_TRY_CMPXCHG(name, weak, okay, fail)       \
static __bfdev_always_inline bfdev_bool                         \
bfdev_arch_##name(bfdev_atomic_t *atomic, bfdev_atomic_t *old,  \
                  bfdev_atomic_t value)                         \
{                                                               \
    return __atomic_compare_exchange(atomic, old, &value,       \
        weak, okay, fail);                                      \
}

#ifndef bfdev_arch_xchg
# define bfdev_arch_xchg bfdev_arch_xchg
BFDEV_GENERIC_XCHG(xchg, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_xchg_acquire
# define bfdev_arch_xchg_acquire bfdev_arch_xchg_acquire
BFDEV_GENERIC_XCHG(xchg_acquire, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_xchg_release
# define bfdev_arch_xchg_release bfdev_arch_xchg_release
BFDEV_GENERIC_XCHG(xchg_release, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_xchg_relaxed
# define bfdev_arch_xchg_relaxed bfdev_arch_xchg_relaxed
BFDEV_GENERIC_XCHG(xchg_relaxed, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_cmpxchg
# define bfdev_arch_cmpxchg bfdev_arch_cmpxchg
BFDEV_GENERIC_CMPXCHG(cmpxchg, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_cmpxchg_acquire
# define bfdev_arch_cmpxchg_acquire bfdev_arch_cmpxchg_acquire
BFDEV_GENERIC_CMPXCHG(cmpxchg_acquire, 0, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_cmpxchg_release
# define bfdev_arch_cmpxchg_release bfdev_arch_cmpxchg_release
BFDEV_GENERIC_CMPXCHG(cmpxchg_release, 0, __ATOMIC_RELEASE, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_cmpxchg_relaxed
# define bfdev_arch_cmpxchg_relaxed bfdev_arch_cmpxchg_relaxed
BFDEV_GENERIC_CMPXCHG(cmpxchg_relaxed, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_try_cmpxchg
# define bfdev_arch_try_cmpxchg bfdev_arch_try_cmpxchg
BFDEV_GENERIC_TRY_CMPXCHG(try_cmpxchg, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_try_cmpxchg_acquire
# define bfdev_arch_try_cmpxchg_acquire bfdev_arch_try_cmpxchg_acquire
BFDEV_GENERIC_TRY_CMPXCHG(try_cmpxchg_acquire, 0, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_try_cmpxchg_release
# define bfdev_arch_try_cmpxchg_release bfdev_arch_try_cmpxchg_release
BFDEV_GENERIC_TRY_CMPXCHG(try_cmpxchg_release, 0, __ATOMIC_RELEASE, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_try_cmpxchg_relaxed
# define bfdev_arch_try_cmpxchg_relaxed bfdev_arch_try_cmpxchg_relaxed
BFDEV_GENERIC_TRY_CMPXCHG(try_cmpxchg_relaxed, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED)
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_CMPXCHG_H_ */
