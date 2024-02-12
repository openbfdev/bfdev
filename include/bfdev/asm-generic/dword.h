/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_DWORD_H_
#define _BFDEV_ASM_GENERIC_DWORD_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/bits.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_DWORD_BITS
# define BFDEV_DWORD_BITS (BFDEV_BITS_PER_LONG >> 1)
# define BFDEV_DWORD_SIZE (1UL << BFDEV_DWORD_BITS)
#endif

#define BFDEV_DWORD_LOWER(val) \
    ((bfdev_uw_t)(val) & (BFDEV_DWORD_SIZE - 1))

#define BFDEV_DWORD_HIGHER(val) \
    ((bfdev_uw_t)(val) >> BFDEV_DWORD_BITS)

#ifndef bfdev_dword_addd
# define bfdev_dword_addd(sh, sl, ah, al, bh, bl) do {  \
    bfdev_uw_t __ah, __al, __bh, __bl;                  \
    bfdev_uw_t __x;                                     \
                                                        \
    __ah = (ah);                                        \
    __al = (al);                                        \
    __bh = (bh);                                        \
    __bl = (bl);                                        \
                                                        \
    __x = __al + __bl;                                  \
    (sh) = __ah + __bh + (__x < __al);                  \
    (sl) = __x;                                         \
} while (0)
#endif

#ifndef bfdev_sub_ddmmss
# define bfdev_sub_ddmmss(sh, sl, ah, al, bh, bl) do {  \
    bfdev_uw_t __ah, __al, __bh, __bl;                  \
    bfdev_uw_t __x;                                     \
                                                        \
    __ah = (ah);                                        \
    __al = (al);                                        \
    __bh = (bh);                                        \
    __bl = (bl);                                        \
                                                        \
    __x = __al - __bl;                                  \
    (sh) = __ah - __bh - (__x > __al);                  \
    (sl) = __x;                                         \
} while (0)
#endif

#ifndef bfdev_umul_ppmm
# define bfdev_umul_ppmm(dh, dl, va, vb) do {               \
    bfdev_uhw_t __ul, __vl, __uh, __vh;                     \
    bfdev_uw_t __x0, __x1, __x2, __x3;                      \
    bfdev_uw_t __va, __vb;                                  \
                                                            \
    __va = (va);                                            \
    __vb = (vb);                                            \
                                                            \
    __ul = BFDEV_DWORD_LOWER(__va);                         \
    __vl = BFDEV_DWORD_LOWER(__vb);                         \
    __uh = BFDEV_DWORD_HIGHER(__va);                        \
    __vh = BFDEV_DWORD_HIGHER(__vb);                        \
                                                            \
    __x0 = (bfdev_uw_t)__ul * __vl;                         \
    __x1 = (bfdev_uw_t)__ul * __vh;                         \
    __x2 = (bfdev_uw_t)__uh * __vl;                         \
    __x3 = (bfdev_uw_t)__uh * __vh;                         \
                                                            \
    __x1 += BFDEV_DWORD_HIGHER(__x0);                       \
    __x1 += __x2;                                           \
    if (__x1 < __x2)                                        \
        __x3 += BFDEV_DWORD_SIZE;                           \
                                                            \
    __x2 = BFDEV_DWORD_LOWER(__x1) << BFDEV_DWORD_BITS;     \
    (dh) = __x3 + BFDEV_DWORD_HIGHER(__x1);                 \
    (dl) = __x2 + BFDEV_DWORD_LOWER(__x0);                  \
} while (0)
#endif

#ifndef bfdev_udiv_qrnnd
# define bfdev_udiv_qrnnd(quot, rem, sh, sl, div) do {          \
    bfdev_uw_t __d1, __d0, __q1, __q0;                          \
    bfdev_uw_t __r1, __r0, __m;                                 \
    bfdev_uw_t __sh, __sl, __div;                               \
                                                                \
    __sh = (sh);                                                \
    __sl = (sl);                                                \
    __div = (div);                                              \
                                                                \
    __d1 = BFDEV_DWORD_HIGHER(__div);                           \
    __d0 = BFDEV_DWORD_LOWER(__div);                            \
                                                                \
    __r1 = __sh % __d1;                                         \
    __q1 = __sh / __d1;                                         \
    __m = (bfdev_uw_t)__q1 * __d0;                              \
    __r1 = __r1 * BFDEV_DWORD_SIZE | BFDEV_DWORD_HIGHER(__sl);  \
                                                                \
    if (__r1 < __m) {                                           \
        __q1--;                                                 \
        __r1 += __div;                                          \
                                                                \
        if (__r1 >= __div) {                                    \
            if (__r1 < __m) {                                   \
                __q1--;                                         \
                __r1 += (__div);                                \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    __r1 -= __m;                                                \
    __r0 = __r1 % __d1;                                         \
    __q0 = __r1 / __d1;                                         \
    __m = (bfdev_uw_t)__q0 * __d0;                              \
    __r0 = __r0 * BFDEV_DWORD_SIZE | BFDEV_DWORD_LOWER(__sl);   \
                                                                \
    if (__r0 < __m) {                                           \
        __q0--;                                                 \
        __r0 += (__div);                                        \
                                                                \
        if (__r0 >= __div) {                                    \
            if (__r0 < __m) {                                   \
                __q0--;                                         \
                __r0 += __div;                                  \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    __r0 -= __m;                                                \
    (quot) = (bfdev_uw_t)__q1 * BFDEV_DWORD_SIZE | __q0;        \
    (rem) = __r0;                                               \
} while (0)
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_DWORD_H_ */
