/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_ATOMIC_H_
#define _BFDEV_ASM_GENERIC_ATOMIC_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/asm/rwonce.h>

BFDEV_BEGIN_DECLS

#define BFDEV_GENERIC_ATOMIC(name, func, order)                         \
static __bfdev_always_inline void                                       \
bfdev_arch_atomic_##name(bfdev_atomic_t *atomic, bfdev_atomic_t value)  \
{                                                                       \
    (void)func(atomic, value, order);                                   \
}

#define BFDEV_GENERIC_ATOMIC_FETCH(name, func, order)                   \
static __bfdev_always_inline bfdev_atomic_t                             \
bfdev_arch_atomic_##name(bfdev_atomic_t *atomic, bfdev_atomic_t value)  \
{                                                                       \
    return func(atomic, value, order);                                  \
}

#define BFDEV_GENERIC_ATOMIC_READ(name, order)                          \
static __bfdev_always_inline bfdev_atomic_t                             \
bfdev_arch_atomic_##name(const bfdev_atomic_t *atomic)                  \
{                                                                       \
    bfdev_atomic_t value;                                               \
    __atomic_load(atomic, &value, order);                               \
    return value;                                                       \
}

#define BFDEV_GENERIC_ATOMIC_WRITE(name, order)                         \
static __bfdev_always_inline void                                       \
bfdev_arch_atomic_##name(bfdev_atomic_t *atomic, bfdev_atomic_t value)  \
{                                                                       \
    __atomic_store(atomic, &value, order);                              \
}

#ifndef bfdev_arch_atomic_read
# define bfdev_arch_atomic_read bfdev_arch_atomic_read
BFDEV_GENERIC_ATOMIC_READ(read, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_read_acquire
# define bfdev_arch_atomic_read_acquire bfdev_arch_atomic_read_acquire
BFDEV_GENERIC_ATOMIC_READ(read_acquire, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_read_relaxed
# define bfdev_arch_atomic_read_relaxed bfdev_arch_atomic_read_relaxed
BFDEV_GENERIC_ATOMIC_READ(read_relaxed, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_write
# define bfdev_arch_atomic_write bfdev_arch_atomic_write
BFDEV_GENERIC_ATOMIC_WRITE(write, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_write_release
# define bfdev_arch_atomic_write_release bfdev_arch_atomic_write_release
BFDEV_GENERIC_ATOMIC_WRITE(write_release, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_write_relaxed
# define bfdev_arch_atomic_write_relaxed bfdev_arch_atomic_write_relaxed
BFDEV_GENERIC_ATOMIC_WRITE(write_relaxed, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_add
# define bfdev_arch_atomic_add bfdev_arch_atomic_add
BFDEV_GENERIC_ATOMIC(add, __atomic_fetch_add, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_add_acquire
# define bfdev_arch_atomic_add_acquire bfdev_arch_atomic_add_acquire
BFDEV_GENERIC_ATOMIC(add_acquire, __atomic_fetch_add, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_add_release
# define bfdev_arch_atomic_add_release bfdev_arch_atomic_add_release
BFDEV_GENERIC_ATOMIC(add_release, __atomic_fetch_add, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_add_relaxed
# define bfdev_arch_atomic_add_relaxed bfdev_arch_atomic_add_relaxed
BFDEV_GENERIC_ATOMIC(add_relaxed, __atomic_fetch_add, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_sub
# define bfdev_arch_atomic_sub bfdev_arch_atomic_sub
BFDEV_GENERIC_ATOMIC(sub, __atomic_fetch_sub, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_sub_acquire
# define bfdev_arch_atomic_sub_acquire bfdev_arch_atomic_sub_acquire
BFDEV_GENERIC_ATOMIC(sub_acquire, __atomic_fetch_sub, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_sub_release
# define bfdev_arch_atomic_sub_release bfdev_arch_atomic_sub_release
BFDEV_GENERIC_ATOMIC(sub_release, __atomic_fetch_sub, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_sub_relaxed
# define bfdev_arch_atomic_sub_relaxed bfdev_arch_atomic_sub_relaxed
BFDEV_GENERIC_ATOMIC(sub_relaxed, __atomic_fetch_sub, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_and
# define bfdev_arch_atomic_and bfdev_arch_atomic_and
BFDEV_GENERIC_ATOMIC(and, __atomic_fetch_and, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_and_acquire
# define bfdev_arch_atomic_and_acquire bfdev_arch_atomic_and_acquire
BFDEV_GENERIC_ATOMIC(and_acquire, __atomic_fetch_and, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_and_release
# define bfdev_arch_atomic_and_release bfdev_arch_atomic_and_release
BFDEV_GENERIC_ATOMIC(and_release, __atomic_fetch_and, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_and_relaxed
# define bfdev_arch_atomic_and_relaxed bfdev_arch_atomic_and_relaxed
BFDEV_GENERIC_ATOMIC(and_relaxed, __atomic_fetch_and, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_or
# define bfdev_arch_atomic_or bfdev_arch_atomic_or
BFDEV_GENERIC_ATOMIC(or, __atomic_fetch_or, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_or_acquire
# define bfdev_arch_atomic_or_acquire bfdev_arch_atomic_or_acquire
BFDEV_GENERIC_ATOMIC(or_acquire, __atomic_fetch_or, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_or_release
# define bfdev_arch_atomic_or_release bfdev_arch_atomic_or_release
BFDEV_GENERIC_ATOMIC(or_release, __atomic_fetch_or, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_or_relaxed
# define bfdev_arch_atomic_or_relaxed bfdev_arch_atomic_or_relaxed
BFDEV_GENERIC_ATOMIC(or_relaxed, __atomic_fetch_or, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_xor
# define bfdev_arch_atomic_xor bfdev_arch_atomic_xor
BFDEV_GENERIC_ATOMIC(xor, __atomic_fetch_xor, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_xor_acquire
# define bfdev_arch_atomic_xor_acquire bfdev_arch_atomic_xor_acquire
BFDEV_GENERIC_ATOMIC(xor_acquire, __atomic_fetch_xor, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_xor_release
# define bfdev_arch_atomic_xor_release bfdev_arch_atomic_xor_release
BFDEV_GENERIC_ATOMIC(xor_release, __atomic_fetch_xor, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_xor_relaxed
# define bfdev_arch_atomic_xor_relaxed bfdev_arch_atomic_xor_relaxed
BFDEV_GENERIC_ATOMIC(xor_relaxed, __atomic_fetch_xor, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_fetch_add
# define bfdev_arch_atomic_fetch_add bfdev_arch_atomic_fetch_add
BFDEV_GENERIC_ATOMIC_FETCH(fetch_add, __atomic_fetch_add, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_fetch_add_acquire
# define bfdev_arch_atomic_fetch_add_acquire bfdev_arch_atomic_fetch_add_acquire
BFDEV_GENERIC_ATOMIC_FETCH(fetch_add_acquire, __atomic_fetch_add, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_fetch_add_release
# define bfdev_arch_atomic_fetch_add_release bfdev_arch_atomic_fetch_add_release
BFDEV_GENERIC_ATOMIC_FETCH(fetch_add_release, __atomic_fetch_add, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_fetch_add_relaxed
# define bfdev_arch_atomic_fetch_add_relaxed bfdev_arch_atomic_fetch_add_relaxed
BFDEV_GENERIC_ATOMIC_FETCH(fetch_add_relaxed, __atomic_fetch_add, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_fetch_sub
# define bfdev_arch_atomic_fetch_sub bfdev_arch_atomic_fetch_sub
BFDEV_GENERIC_ATOMIC_FETCH(fetch_sub, __atomic_fetch_sub, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_fetch_sub_acquire
# define bfdev_arch_atomic_fetch_sub_acquire bfdev_arch_atomic_fetch_sub_acquire
BFDEV_GENERIC_ATOMIC_FETCH(fetch_sub_acquire, __atomic_fetch_sub, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_fetch_sub_release
# define bfdev_arch_atomic_fetch_sub_release bfdev_arch_atomic_fetch_sub_release
BFDEV_GENERIC_ATOMIC_FETCH(fetch_sub_release, __atomic_fetch_sub, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_fetch_sub_relaxed
# define bfdev_arch_atomic_fetch_sub_relaxed bfdev_arch_atomic_fetch_sub_relaxed
BFDEV_GENERIC_ATOMIC_FETCH(fetch_sub_relaxed, __atomic_fetch_sub, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_fetch_and
# define bfdev_arch_atomic_fetch_and bfdev_arch_atomic_fetch_and
BFDEV_GENERIC_ATOMIC_FETCH(fetch_and, __atomic_fetch_and, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_fetch_and_acquire
# define bfdev_arch_atomic_fetch_and_acquire bfdev_arch_atomic_fetch_and_acquire
BFDEV_GENERIC_ATOMIC_FETCH(fetch_and_acquire, __atomic_fetch_and, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_fetch_and_release
# define bfdev_arch_atomic_fetch_and_release bfdev_arch_atomic_fetch_and_release
BFDEV_GENERIC_ATOMIC_FETCH(fetch_and_release, __atomic_fetch_and, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_fetch_and_relaxed
# define bfdev_arch_atomic_fetch_and_relaxed bfdev_arch_atomic_fetch_and_relaxed
BFDEV_GENERIC_ATOMIC_FETCH(fetch_and_relaxed, __atomic_fetch_and, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_fetch_or
# define bfdev_arch_atomic_fetch_or bfdev_arch_atomic_fetch_or
BFDEV_GENERIC_ATOMIC_FETCH(fetch_or, __atomic_fetch_or, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_fetch_or_acquire
# define bfdev_arch_atomic_fetch_or_acquire bfdev_arch_atomic_fetch_or_acquire
BFDEV_GENERIC_ATOMIC_FETCH(fetch_or_acquire, __atomic_fetch_or, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_fetch_or_release
# define bfdev_arch_atomic_fetch_or_release bfdev_arch_atomic_fetch_or_release
BFDEV_GENERIC_ATOMIC_FETCH(fetch_or_release, __atomic_fetch_or, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_fetch_or_relaxed
# define bfdev_arch_atomic_fetch_or_relaxed bfdev_arch_atomic_fetch_or_relaxed
BFDEV_GENERIC_ATOMIC_FETCH(fetch_or_relaxed, __atomic_fetch_or, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_fetch_xor
# define bfdev_arch_atomic_fetch_xor bfdev_arch_atomic_fetch_xor
BFDEV_GENERIC_ATOMIC_FETCH(fetch_xor, __atomic_fetch_xor, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_fetch_xor_acquire
# define bfdev_arch_atomic_fetch_xor_acquire bfdev_arch_atomic_fetch_xor_acquire
BFDEV_GENERIC_ATOMIC_FETCH(fetch_xor_acquire, __atomic_fetch_xor, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_fetch_xor_release
# define bfdev_arch_atomic_fetch_xor_release bfdev_arch_atomic_fetch_xor_release
BFDEV_GENERIC_ATOMIC_FETCH(fetch_xor_release, __atomic_fetch_xor, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_fetch_xor_relaxed
# define bfdev_arch_atomic_fetch_xor_relaxed bfdev_arch_atomic_fetch_xor_relaxed
BFDEV_GENERIC_ATOMIC_FETCH(fetch_xor_relaxed, __atomic_fetch_xor, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_add_fetch
# define bfdev_arch_atomic_add_fetch bfdev_arch_atomic_add_fetch
BFDEV_GENERIC_ATOMIC_FETCH(add_fetch, __atomic_add_fetch, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_add_fetch_acquire
# define bfdev_arch_atomic_add_fetch_acquire bfdev_arch_atomic_add_fetch_acquire
BFDEV_GENERIC_ATOMIC_FETCH(add_fetch_acquire, __atomic_add_fetch, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_add_fetch_release
# define bfdev_arch_atomic_add_fetch_release bfdev_arch_atomic_add_fetch_release
BFDEV_GENERIC_ATOMIC_FETCH(add_fetch_release, __atomic_add_fetch, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_add_fetch_relaxed
# define bfdev_arch_atomic_add_fetch_relaxed bfdev_arch_atomic_add_fetch_relaxed
BFDEV_GENERIC_ATOMIC_FETCH(add_fetch_relaxed, __atomic_add_fetch, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_sub_fetch
# define bfdev_arch_atomic_sub_fetch bfdev_arch_atomic_sub_fetch
BFDEV_GENERIC_ATOMIC_FETCH(sub_fetch, __atomic_sub_fetch, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_sub_fetch_acquire
# define bfdev_arch_atomic_sub_fetch_acquire bfdev_arch_atomic_sub_fetch_acquire
BFDEV_GENERIC_ATOMIC_FETCH(sub_fetch_acquire, __atomic_sub_fetch, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_sub_fetch_release
# define bfdev_arch_atomic_sub_fetch_release bfdev_arch_atomic_sub_fetch_release
BFDEV_GENERIC_ATOMIC_FETCH(sub_fetch_release, __atomic_sub_fetch, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_sub_fetch_relaxed
# define bfdev_arch_atomic_sub_fetch_relaxed bfdev_arch_atomic_sub_fetch_relaxed
BFDEV_GENERIC_ATOMIC_FETCH(sub_fetch_relaxed, __atomic_sub_fetch, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_and_fetch
# define bfdev_arch_atomic_and_fetch bfdev_arch_atomic_and_fetch
BFDEV_GENERIC_ATOMIC_FETCH(and_fetch, __atomic_and_fetch, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_and_fetch_acquire
# define bfdev_arch_atomic_and_fetch_acquire bfdev_arch_atomic_and_fetch_acquire
BFDEV_GENERIC_ATOMIC_FETCH(and_fetch_acquire, __atomic_and_fetch, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_and_fetch_release
# define bfdev_arch_atomic_and_fetch_release bfdev_arch_atomic_and_fetch_release
BFDEV_GENERIC_ATOMIC_FETCH(and_fetch_release, __atomic_and_fetch, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_and_fetch_relaxed
# define bfdev_arch_atomic_and_fetch_relaxed bfdev_arch_atomic_and_fetch_relaxed
BFDEV_GENERIC_ATOMIC_FETCH(and_fetch_relaxed, __atomic_and_fetch, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_or_fetch
# define bfdev_arch_atomic_or_fetch bfdev_arch_atomic_or_fetch
BFDEV_GENERIC_ATOMIC_FETCH(or_fetch, __atomic_or_fetch, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_or_fetch_acquire
# define bfdev_arch_atomic_or_fetch_acquire bfdev_arch_atomic_or_fetch_acquire
BFDEV_GENERIC_ATOMIC_FETCH(or_fetch_acquire, __atomic_or_fetch, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_or_fetch_release
# define bfdev_arch_atomic_or_fetch_release bfdev_arch_atomic_or_fetch_release
BFDEV_GENERIC_ATOMIC_FETCH(or_fetch_release, __atomic_or_fetch, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_or_fetch_relaxed
# define bfdev_arch_atomic_or_fetch_relaxed bfdev_arch_atomic_or_fetch_relaxed
BFDEV_GENERIC_ATOMIC_FETCH(or_fetch_relaxed, __atomic_or_fetch, __ATOMIC_RELAXED)
#endif

#ifndef bfdev_arch_atomic_xor_fetch
# define bfdev_arch_atomic_xor_fetch bfdev_arch_atomic_xor_fetch
BFDEV_GENERIC_ATOMIC_FETCH(xor_fetch, __atomic_xor_fetch, __ATOMIC_SEQ_CST)
#endif

#ifndef bfdev_arch_atomic_xor_fetch_acquire
# define bfdev_arch_atomic_xor_fetch_acquire bfdev_arch_atomic_xor_fetch_acquire
BFDEV_GENERIC_ATOMIC_FETCH(xor_fetch_acquire, __atomic_xor_fetch, __ATOMIC_ACQUIRE)
#endif

#ifndef bfdev_arch_atomic_xor_fetch_release
# define bfdev_arch_atomic_xor_fetch_release bfdev_arch_atomic_xor_fetch_release
BFDEV_GENERIC_ATOMIC_FETCH(xor_fetch_release, __atomic_xor_fetch, __ATOMIC_RELEASE)
#endif

#ifndef bfdev_arch_atomic_xor_fetch_relaxed
# define bfdev_arch_atomic_xor_fetch_relaxed bfdev_arch_atomic_xor_fetch_relaxed
BFDEV_GENERIC_ATOMIC_FETCH(xor_fetch_relaxed, __atomic_xor_fetch, __ATOMIC_RELAXED)
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_ATOMIC_H_ */
