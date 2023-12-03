/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_REFCNT_H_
#define _BFDEV_REFCNT_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/atomic.h>
#include <bfdev/cmpxchg.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_refcnt {
    bfdev_atomic_t count;
} bfdev_refcnt_t;

enum bfdev_refcnt_saturation {
    BFDEV_REFCNT_ADD_UAF,
    BFDEV_REFCNT_ADD_LEAK,
    BFDEV_REFCNT_ADD_OVF,
    BFDEV_REFCNT_SUB_UAF,
    BFDEV_REFCNT_SUB_LEAK,
    BFDEV_REFCNT_SUB_OVF,
    BFDEV_REFCNT_ADDNZ_LEAK,
    BFDEV_REFCNT_ADDNZ_OVF,
};

#define BFDEV_REFCNT_STATIC \
    { 1 }

#define BFDEV_REFCNT_INIT \
    (bfdev_refcnt_t)BFDEV_REFCNT_STATIC

#define BFDEV_DEFINE_REFCNT(name) \
    bfdev_refcnt_t name = BFDEV_REFCNT_INIT

#ifdef BFDEV_DEBUG_REFCNT
extern void
bfdev_refcnt_report(bfdev_refcnt_t *ref, enum bfdev_refcnt_saturation type);
#endif

static inline bfdev_atomic_t
bfdev_refcnt_get(bfdev_refcnt_t *ref)
{
    return bfdev_atomic_read(&ref->count);
}

static inline void
bfdev_refcnt_set(bfdev_refcnt_t *ref, bfdev_atomic_t cnt)
{
    bfdev_atomic_write(&ref->count, cnt);
}

static inline void
bfdev_refcnt_fetch_add(bfdev_refcnt_t *ref, bfdev_atomic_t cnt,
                       bfdev_atomic_t *oldp)
{
    bfdev_atomic_t prev;

    prev = bfdev_atomic_fetch_add(&ref->count, cnt);
    if (oldp)
        *oldp = prev;

#ifdef BFDEV_DEBUG_REFCNT
    if (bfdev_unlikely(!prev))
        bfdev_refcnt_report(ref, BFDEV_REFCNT_ADD_UAF);
    else if (bfdev_unlikely(prev < 0))
        bfdev_refcnt_report(ref, BFDEV_REFCNT_ADD_LEAK);
    else if (bfdev_unlikely(prev + cnt < 0))
        bfdev_refcnt_report(ref, BFDEV_REFCNT_ADD_OVF);
#endif
}

static inline bool
bfdev_refcnt_fetch_sub_test(bfdev_refcnt_t *ref, bfdev_atomic_t nr,
                            bfdev_atomic_t *oldp)
{
	bfdev_atomic_t prev;

    prev = bfdev_atomic_fetch_sub(&ref->count, nr);
    if (oldp)
        *oldp = prev;

    if (prev == nr)
        return true;

#ifdef BFDEV_DEBUG_REFCNT
    if (bfdev_unlikely(!prev))
        bfdev_refcnt_report(ref, BFDEV_REFCNT_SUB_UAF);
    else if (bfdev_unlikely(prev < 0))
        bfdev_refcnt_report(ref, BFDEV_REFCNT_SUB_LEAK);
    else if (bfdev_unlikely(prev - nr < 0))
        bfdev_refcnt_report(ref, BFDEV_REFCNT_SUB_OVF);
#endif

    return false;
}

static inline bool
bfdev_refcnt_fetch_addnz_test(bfdev_refcnt_t *ref, bfdev_atomic_t nr,
                              bfdev_atomic_t *oldp)
{
    bfdev_atomic_t prev = bfdev_refcnt_get(ref);

    do {
        if (!prev)
            break;
    } while (bfdev_try_cmpxchg(&ref->count, &prev, prev + nr));

    if (oldp)
        *oldp = prev;

#ifdef BFDEV_DEBUG_REFCNT
    if (bfdev_unlikely(prev < 0))
        bfdev_refcnt_report(ref, BFDEV_REFCNT_ADDNZ_LEAK);
    else if (bfdev_unlikely(prev - nr < 0))
        bfdev_refcnt_report(ref, BFDEV_REFCNT_ADDNZ_OVF);
#endif

    return !prev;
}

#define BFDEV_REFCNT_OPS(addsub, incdec, operation)                                 \
static inline void                                                                  \
bfdev_generic_refcnt_##addsub(bfdev_refcnt_t *ref, bfdev_atomic_t nr)               \
{                                                                                   \
    operation(ref, nr, NULL);                                                       \
}                                                                                   \
                                                                                    \
static inline void                                                                  \
bfdev_generic_refcnt_##incdec(bfdev_refcnt_t *ref)                                  \
{                                                                                   \
    operation(ref, 1, NULL);                                                        \
}                                                                                   \
                                                                                    \
static inline void                                                                  \
bfdev_generic_refcnt_fetch_##incdec(bfdev_refcnt_t *ref, bfdev_atomic_t *oldp)      \
{                                                                                   \
    operation(ref, 1, oldp);                                                        \
}

#define BFDEV_REFCNT_TEST_OPS(addsub, incdec, operation)                            \
static inline void                                                                  \
bfdev_generic_refcnt_fetch_##addsub(bfdev_refcnt_t *ref,                            \
                                    bfdev_atomic_t nr, bfdev_atomic_t *oldp)        \
{                                                                                   \
    operation(ref, nr, oldp);                                                       \
}                                                                                   \
                                                                                    \
static inline bool                                                                  \
bfdev_generic_refcnt_test_##addsub(bfdev_refcnt_t *ref, bfdev_atomic_t nr)          \
{                                                                                   \
    return operation(ref, nr, NULL);                                                \
}                                                                                   \
                                                                                    \
static inline bool                                                                  \
bfdev_generic_refcnt_test_##incdec(bfdev_refcnt_t *ref)                             \
{                                                                                   \
    return operation(ref, 1, NULL);                                                 \
}                                                                                   \
                                                                                    \
static inline bool                                                                  \
bfdev_generic_refcnt_fetch_test_##incdec(bfdev_refcnt_t *ref, bfdev_atomic_t *oldp) \
{                                                                                   \
    return operation(ref, 1, oldp);                                                 \
}                                                                                   \
BFDEV_REFCNT_OPS(addsub, incdec, operation)

BFDEV_REFCNT_OPS(add, inc, bfdev_refcnt_fetch_add)
#define bfdev_refcnt_add(ref, nr)                   bfdev_generic_refcnt_add(ref, nr)
#define bfdev_refcnt_inc(ref)                       bfdev_generic_refcnt_inc(ref)
#define bfdev_refcnt_fetch_inc(ref, oldp)           bfdev_generic_refcnt_fetch_inc(ref, oldp)

BFDEV_REFCNT_TEST_OPS(sub, dec, bfdev_refcnt_fetch_sub_test)
#define bfdev_refcnt_sub(ref, nr)                   bfdev_generic_refcnt_sub(ref, nr)
#define bfdev_refcnt_sub_test(ref, nr)              bfdev_generic_refcnt_test_sub(ref, nr)
#define bfdev_refcnt_fetch_sub(ref, nr, oldp)       bfdev_generic_refcnt_fetch_sub(ref, nr, oldp)
#define bfdev_refcnt_dec(ref)                       bfdev_generic_refcnt_dec(ref)
#define bfdev_refcnt_dec_test(ref)                  bfdev_generic_refcnt_test_dec(ref)
#define bfdev_refcnt_fetch_dec(ref, oldp)           bfdev_generic_refcnt_fetch_dec(ref, oldp)
#define bfdev_refcnt_fetch_dec_test(ref, oldp)      bfdev_generic_refcnt_fetch_test_dec(ref, oldp)

BFDEV_REFCNT_TEST_OPS(addnz, incnz, bfdev_refcnt_fetch_addnz_test)
#define bfdev_refcnt_addnz(ref, nr)                 bfdev_generic_refcnt_addnz(ref, nr)
#define bfdev_refcnt_addnz_test(ref, nr)            bfdev_generic_refcnt_test_addnz(ref, nr)
#define bfdev_refcnt_fetch_addnz(ref, nr, oldp)     bfdev_generic_refcnt_fetch_addnz(ref, nr, oldp)
#define bfdev_refcnt_incnz(ref)                     bfdev_generic_refcnt_incnz(ref)
#define bfdev_refcnt_incnz_test(ref)                bfdev_generic_refcnt_test_incnz(ref)
#define bfdev_refcnt_fetch_incnz(ref, oldp)         bfdev_generic_refcnt_fetch_incnz(ref, oldp)
#define bfdev_refcnt_fetch_incnz_test(ref, oldp)    bfdev_generic_refcnt_fetch_test_incnz(ref, oldp)

BFDEV_END_DECLS

#endif /* _BFDEV_REFCNT_H_ */
