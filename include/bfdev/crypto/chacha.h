/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRYPTO_SHA1_H_
#define _BFDEV_CRYPTO_SHA1_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/bitops.h>
#include <bfdev/unaligned.h>

#define BFDEV_CHACHA_BLEND(b, A, B, C, D, E) \
    b[A] += b[B]; b[C] = bfdev_rol32(b[C] ^ b[D], E)
static inline void
bfdev_chacha_permute(uint32_t *block, unsigned int rounds)
{
    unsigned int count;

    for (count = 0; count < rounds; count += 2) {
        BFDEV_CHACHA_BLEND(block, 0, 4, 12, 0, 16);
        BFDEV_CHACHA_BLEND(block, 1, 5, 13, 1, 16);
        BFDEV_CHACHA_BLEND(block, 2, 6, 14, 2, 16);
        BFDEV_CHACHA_BLEND(block, 3, 7, 15, 3, 16);

        BFDEV_CHACHA_BLEND(block, 8, 12, 4, 8, 12);
        BFDEV_CHACHA_BLEND(block, 9, 13, 5, 9, 12);
        BFDEV_CHACHA_BLEND(block, 10, 14, 6, 10, 12);
        BFDEV_CHACHA_BLEND(block, 11, 15, 7, 11, 12);

        BFDEV_CHACHA_BLEND(block, 0, 4, 12, 0, 8);
        BFDEV_CHACHA_BLEND(block, 1, 5, 13, 1, 8);
        BFDEV_CHACHA_BLEND(block, 2, 6, 14, 2, 8);
        BFDEV_CHACHA_BLEND(block, 3, 7, 15, 3, 8);

        BFDEV_CHACHA_BLEND(block, 8, 12, 4, 8, 7);
        BFDEV_CHACHA_BLEND(block, 9, 13, 5, 9, 7);
        BFDEV_CHACHA_BLEND(block, 10, 14, 6, 10, 7);
        BFDEV_CHACHA_BLEND(block, 11, 15, 7, 11, 7);

        BFDEV_CHACHA_BLEND(block, 0, 5, 15, 0, 16);
        BFDEV_CHACHA_BLEND(block, 1, 6, 12, 1, 16);
        BFDEV_CHACHA_BLEND(block, 2, 7, 13, 2, 16);
        BFDEV_CHACHA_BLEND(block, 3, 4, 14, 3, 16);

        BFDEV_CHACHA_BLEND(block, 10, 15, 5, 10, 12);
        BFDEV_CHACHA_BLEND(block, 11, 12, 6, 11, 12);
        BFDEV_CHACHA_BLEND(block, 8, 13, 7, 8, 12);
        BFDEV_CHACHA_BLEND(block, 9, 14, 4, 9, 12);

        BFDEV_CHACHA_BLEND(block, 0, 5, 15, 0, 8);
        BFDEV_CHACHA_BLEND(block, 1, 6, 12, 1, 8);
        BFDEV_CHACHA_BLEND(block, 2, 7, 13, 2, 8);
        BFDEV_CHACHA_BLEND(block, 3, 4, 14, 3, 8);

        BFDEV_CHACHA_BLEND(block, 10, 15, 5, 10, 7);
        BFDEV_CHACHA_BLEND(block, 11, 12, 6, 11, 7);
        BFDEV_CHACHA_BLEND(block, 8, 13, 7, 8, 7);
        BFDEV_CHACHA_BLEND(block, 9, 14, 4, 9, 7);
    }
}
