/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-dword"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <base.h>
#include <bfdev/dword.h>
#include <bfdev/bitops.h>
#include <export.h>

export void
bfdev_dword_generic_udiv(bfdev_uw_t *quot, bfdev_uw_t *rem,
                         const bfdev_uw_t *src, bfdev_uw_t div)
{
    bfdev_uw_t q0, q1, b, bm;
    bfdev_uw_t n0, n1, n2;

    if (!quot && !rem)
        return;

    n0 = src[0];
    n1 = src[1];

    if (div > n1) {
        bm = bfdev_clz(div);
        if (bm) {
            div = div << bm;
            n1 = (n1 << bm) | (n0 >> (BFDEV_BITS_PER_LONG - bm));
            n0 = n0 << bm;
        }

        bfdev_udiv_qrnnd(q0, n0, n1, n0, div);
        q1 = 0;
    } else {
        /* divide by zero */
        BFDEV_BUG_ON(!div);

        bm = bfdev_clz(div);
        if (bm == 0) {
            n1 -= div;
            q1 = 1;
        } else {
            b = BFDEV_BITS_PER_LONG - bm;
            div = div << bm;
            n2 = n1 >> b;
            n1 = (n1 << bm) | (n0 >> b);
            n0 = n0 << bm;
            bfdev_udiv_qrnnd(q1, n1, n2, n1, div);
        }

        bfdev_udiv_qrnnd(q0, n0, n1, n0, div);
    }

    if (quot) {
        quot[0] = q0;
        quot[1] = q1;
    }

    if (rem)
        *rem = n0 >> bm;
}

export void
bfdev_dword_generic_udivd(bfdev_uw_t *quot, bfdev_uw_t *rem,
                          const bfdev_uw_t *src, const bfdev_uw_t *div)
{
    bfdev_uw_t q0, q1, b, bm;
    bfdev_uw_t d0, d1, n0, n1, n2;
    bfdev_uw_t m1, m0;

    if (!quot && !rem)
        return;

    n0 = src[0];
    n1 = src[1];
    d0 = div[0];
    d1 = div[1];

    /* degrade to udiv */
    if (!d1)
        return bfdev_dword_udiv(quot, rem, src, d0);

    bm = bfdev_clz(d1);
    if (bm == 0) {
        if (n1 <= d1 && n0 < d0)
            q0 = 0;
        else {
            q0 = 1;
            bfdev_sub_ddmmss(n1, n0, n1, n0, d1, d0);
        }

        q1 = 0;
        if (rem) {
            rem[0] = n0;
            rem[1] = n1;
        }
    } else {
        b = BFDEV_BITS_PER_LONG - bm;
        d1 = (d1 << bm) | (d0 >> b);
        d0 = d0 << bm;
        n2 = n1 >> b;
        n1 = (n1 << bm) | (n0 >> b);
        n0 = n0 << bm;

        bfdev_udiv_qrnnd(q0, n1, n2, n1, d1);
        bfdev_umul_ppmm(m1, m0, q0, d0);

        if (m1 > n1 || (m1 == n1 && m0 > n0)) {
            q0--;
            bfdev_sub_ddmmss(m1, m0, m1, m0, d1, d0);
        }

        q1 = 0;
        if (rem) {
            bfdev_sub_ddmmss(n1, n0, n1, n0, m1, m0);
            rem[0] = (n1 << b) | (n0 >> bm);
            rem[1] = n1 >> bm;
        }
    }

    if (quot) {
        quot[0] = q0;
        quot[1] = q1;
    }
}
