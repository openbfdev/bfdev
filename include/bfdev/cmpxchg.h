/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CMPXCHG_H_
#define _BFDEV_CMPXCHG_H_

#include <bfdev/config.h>
#include <bfdev/asm/cmpxchg.h>

BFDEV_BEGIN_DECLS

#ifndef bfdev_xchg
static __bfdev_always_inline bfdev_atomic_t
bfdev_xchg(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_xchg(atomic, value);
}
#endif

#ifndef bfdev_xchg_acquire
static __bfdev_always_inline bfdev_atomic_t
bfdev_xchg_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_xchg_acquire(atomic, value);
}
#endif

#ifndef bfdev_xchg_release
static __bfdev_always_inline bfdev_atomic_t
bfdev_xchg_release(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_xchg_release(atomic, value);
}
#endif

#ifndef bfdev_xchg_relaxed
static __bfdev_always_inline bfdev_atomic_t
bfdev_xchg_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_xchg_relaxed(atomic, value);
}
#endif

#ifndef bfdev_cmpxchg
static __bfdev_always_inline bfdev_atomic_t
bfdev_cmpxchg(bfdev_atomic_t *atomic, bfdev_atomic_t old,
              bfdev_atomic_t value)
{
    return bfdev_arch_cmpxchg(atomic, old, value);
}
#endif

#ifndef bfdev_cmpxchg_acquire
static __bfdev_always_inline bfdev_atomic_t
bfdev_cmpxchg_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t old,
                      bfdev_atomic_t value)
{
    return bfdev_arch_cmpxchg_acquire(atomic, old, value);
}
#endif

#ifndef bfdev_cmpxchg_release
static __bfdev_always_inline bfdev_atomic_t
bfdev_cmpxchg_release(bfdev_atomic_t *atomic, bfdev_atomic_t old,
                      bfdev_atomic_t value)
{
    return bfdev_arch_cmpxchg_release(atomic, old, value);
}
#endif

#ifndef bfdev_cmpxchg_relaxed
static __bfdev_always_inline bfdev_atomic_t
bfdev_cmpxchg_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t old,
                      bfdev_atomic_t value)
{
    return bfdev_arch_cmpxchg_relaxed(atomic, old, value);
}
#endif

#ifndef bfdev_try_cmpxchg
static __bfdev_always_inline bfdev_bool
bfdev_try_cmpxchg(bfdev_atomic_t *atomic, bfdev_atomic_t *old,
                  bfdev_atomic_t value)
{
    return bfdev_arch_try_cmpxchg(atomic, old, value);
}
#endif

#ifndef bfdev_try_cmpxchg_acquire
static __bfdev_always_inline bfdev_bool
bfdev_try_cmpxchg_acquire(bfdev_atomic_t *atomic, bfdev_atomic_t *old,
                          bfdev_atomic_t value)
{
    return bfdev_arch_try_cmpxchg_acquire(atomic, old, value);
}
#endif

#ifndef bfdev_try_cmpxchg_release
static __bfdev_always_inline bfdev_bool
bfdev_try_cmpxchg_release(bfdev_atomic_t *atomic, bfdev_atomic_t *old,
                          bfdev_atomic_t value)
{
    return bfdev_arch_try_cmpxchg_release(atomic, old, value);
}
#endif

#ifndef bfdev_try_cmpxchg_relaxed
static __bfdev_always_inline bfdev_bool
bfdev_try_cmpxchg_relaxed(bfdev_atomic_t *atomic, bfdev_atomic_t *old,
                          bfdev_atomic_t value)
{
    return bfdev_arch_try_cmpxchg_relaxed(atomic, old, value);
}
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_CMPXCHG_H_ */
