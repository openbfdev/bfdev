/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRYPTO_SHA2_H_
#define _BFDEV_CRYPTO_SHA2_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/bitops.h>
#include <bfdev/unaligned.h>

BFDEV_BEGIN_DECLS

static const uint32_t
bfdev_sha2_k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

#define BFDEV_SHA2_S0(x) ( \
    bfdev_ror32(x,  7) ^ bfdev_ror32(x, 18) ^ (x >>  3) \
)

#define BFDEV_SHA2_S1(x) ( \
    bfdev_ror32(x, 17) ^ bfdev_ror32(x, 19) ^ (x >> 10) \
)

#define BFDEV_SHA2_E0(x) ( \
    bfdev_ror32(x,  2) ^ bfdev_ror32(x, 13) ^ bfdev_ror32(x, 22) \
)

#define BFDEV_SHA2_E1(x) ( \
    bfdev_ror32(x,  6) ^ bfdev_ror32(x, 11) ^ bfdev_ror32(x, 25) \
)

#define BFDEV_SHA2_CH(x, y, z) (z ^ (x & (y ^ z)))
#define BFDEV_SHA2_MAJ(x, y, z) ((x & y) | (z & (x | y)))

#define BFDEV_SHA2_LOAD(c, b, d) ( \
    b[c] = bfdev_unaligned_get_be32((uint32_t *)d + c) \
)

#define BFDEV_SHA2_BLEND(c, b) ( \
    b[c] = BFDEV_SHA2_S1(b[c -  2]) + b[c -  7] + \
           BFDEV_SHA2_S0(b[c - 15]) + b[c - 16] \
)

#define BFDEV_SHA2_ROUND(c, b, A, B, C, D, E, F, G, H) do {     \
    uint32_t __tmp1, __tmp2;                                    \
    __tmp1 = H + BFDEV_SHA2_E1(E) + BFDEV_SHA2_CH(E, F, G) +    \
             bfdev_sha2_k[c] + b[c];                            \
    __tmp2 = BFDEV_SHA2_E0(A) + BFDEV_SHA2_MAJ(A, B, C);        \
    D += __tmp1; H = __tmp1 + __tmp2;                           \
} while (0)

static inline void
bfdev_sha2_transform(uint32_t *digest, uint32_t *block, const uint8_t *data)
{
    uint32_t A, B, C, D, E, F, G, H;
    unsigned int count;

    /* Load state */
    A = digest[0];
    B = digest[1];
    C = digest[2];
    D = digest[3];
    E = digest[4];
    F = digest[5];
    G = digest[6];
    H = digest[7];

    /* Load data */
    for (count = 0; count < 16; count += 8) {
        BFDEV_SHA2_LOAD(count + 0, block, data);
        BFDEV_SHA2_LOAD(count + 1, block, data);
        BFDEV_SHA2_LOAD(count + 2, block, data);
        BFDEV_SHA2_LOAD(count + 3, block, data);
        BFDEV_SHA2_LOAD(count + 4, block, data);
        BFDEV_SHA2_LOAD(count + 5, block, data);
        BFDEV_SHA2_LOAD(count + 6, block, data);
        BFDEV_SHA2_LOAD(count + 7, block, data);
    }

    /* Blend data */
    for (count = 16; count < 64; count += 8) {
        BFDEV_SHA2_BLEND(count + 0, block);
        BFDEV_SHA2_BLEND(count + 1, block);
        BFDEV_SHA2_BLEND(count + 2, block);
        BFDEV_SHA2_BLEND(count + 3, block);
        BFDEV_SHA2_BLEND(count + 4, block);
        BFDEV_SHA2_BLEND(count + 5, block);
        BFDEV_SHA2_BLEND(count + 6, block);
        BFDEV_SHA2_BLEND(count + 7, block);
    }

    /* Iterate register */
    for (count = 0; count < 64; count += 8) {
        BFDEV_SHA2_ROUND(count + 0, block, A, B, C, D, E, F, G, H);
        BFDEV_SHA2_ROUND(count + 1, block, H, A, B, C, D, E, F, G);
        BFDEV_SHA2_ROUND(count + 2, block, G, H, A, B, C, D, E, F);
        BFDEV_SHA2_ROUND(count + 3, block, F, G, H, A, B, C, D, E);
        BFDEV_SHA2_ROUND(count + 4, block, E, F, G, H, A, B, C, D);
        BFDEV_SHA2_ROUND(count + 5, block, D, E, F, G, H, A, B, C);
        BFDEV_SHA2_ROUND(count + 6, block, C, D, E, F, G, H, A, B);
        BFDEV_SHA2_ROUND(count + 7, block, B, C, D, E, F, G, H, A);
    }

    /* Update state */
    digest[0] += A;
    digest[1] += B;
    digest[2] += C;
    digest[3] += D;
    digest[4] += E;
    digest[5] += F;
    digest[6] += G;
    digest[7] += H;
}

BFDEV_END_DECLS

#endif /* _BFDEV_CRYPTO_SHA2_H_ */
