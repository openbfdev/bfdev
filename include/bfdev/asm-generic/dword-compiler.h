/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_DWORD_H_
#define _BFDEV_ASM_GENERIC_DWORD_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/bits.h>

BFDEV_BEGIN_DECLS

#if BFDEV_BITS_PER_LONG == 32
typedef bfdev_u64 bfdev_dword_t;
#else
typedef unsigned __int128 bfdev_dword_t;
#endif

#ifndef bfdev_dword_addd
# define bfdev_dword_addd bfdev_dword_addd
static __bfdev_always_inline void
bfdev_dword_addd(bfdev_uw_t *sh, bfdev_uw_t *sl, bfdev_uw_t ah,
                 bfdev_uw_t al, bfdev_uw_t bh, bfdev_uw_t bl)
{
    bfdev_dword_t ac, bc, sc;

    ac = (bfdev_dword_t)ah << BFDEV_BITS_PER_LONG | al;
    bc = (bfdev_dword_t)bh << BFDEV_BITS_PER_LONG | bl;

    sc = ac + bc;
    *sh = (bfdev_uw_t)(sc >> BFDEV_BITS_PER_LONG);
    *sl = (bfdev_uw_t)sc;
}
#endif

#ifndef bfdev_dword_sub_ddmmss
# define bfdev_dword_sub_ddmmss bfdev_dword_sub_ddmmss
static __bfdev_always_inline void
bfdev_dword_sub_ddmmss(bfdev_uw_t *sh, bfdev_uw_t *sl, bfdev_uw_t ah,
                       bfdev_uw_t al, bfdev_uw_t bh, bfdev_uw_t bl)
{
    bfdev_dword_t ac, bc, sc;

    ac = (bfdev_dword_t)ah << BFDEV_BITS_PER_LONG | al;
    bc = (bfdev_dword_t)bh << BFDEV_BITS_PER_LONG | bl;

    sc = ac - bc;
    *sh = (bfdev_uw_t)(sc >> BFDEV_BITS_PER_LONG);
    *sl = (bfdev_uw_t)sc;
}
#endif

#ifndef bfdev_dword_umul_ppmm
# define bfdev_dword_umul_ppmm bfdev_dword_umul_ppmm
static __bfdev_always_inline void
bfdev_dword_umul_ppmm(bfdev_uw_t *dh, bfdev_uw_t *dl,
                      bfdev_uw_t va, bfdev_uw_t vb)
{
    bfdev_dword_t sc;

    sc = (bfdev_dword_t)va * vb;
    *dh = (bfdev_uw_t)(sc >> BFDEV_BITS_PER_LONG);
    *dl = (bfdev_uw_t)sc;
}
#endif

#ifndef bfdev_dword_udiv_qrnnd
# define bfdev_dword_udiv_qrnnd bfdev_dword_udiv_qrnnd
static __bfdev_always_inline void
bfdev_dword_udiv_qrnnd(bfdev_uw_t *quot, bfdev_uw_t *rem, bfdev_uw_t vh,
                       bfdev_uw_t vl, bfdev_uw_t div)
{
    bfdev_dword_t vc;

    vc = (bfdev_dword_t)vh << BFDEV_BITS_PER_LONG | vl;
    *quot = (bfdev_uw_t)(vc / div);
    *rem = (bfdev_uw_t)(vc % div);
}
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_DWORD_H_ */
