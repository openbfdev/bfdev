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

BFDEV_BEGIN_DECLS

#define SHA1_W(c, b) (b[(c) & 0x0f])
#define SHA1_SRC(c, b, d) bfdev_unaligned_get_be32((uint32_t *)d + c)

#define SHA1_MIX(c, b, d) \
    bfdev_rol32(SHA1_W(c + 13, b) ^ SHA1_W(c + 8, b) ^ \
                SHA1_W(c +  2, b) ^ SHA1_W(c, b), 1)

#define SHA1_ROUND(c, b, d, A, B, C, D, E, func, const, exp) do {   \
    uint32_t __value;                                               \
    __value = func(c, b, d);                                        \
    SHA1_W(c, b) = __value;                                         \
    E += __value + bfdev_rol32(A, 5) + (exp) + (const);             \
    B = bfdev_ror32(B, 2); __value = E;                             \
    E = D; D = C; C = B; B = A; A = __value;                        \
} while (0)

#define SHA1_00_15(c, b, d, A, B, C, D, E) \
        SHA1_ROUND(c, b, d, A, B, C, D, E, SHA1_SRC, \
                   0x5a827999, (((C ^ D) & B) ^ D))

#define SHA1_16_19(c, b, d, A, B, C, D, E) \
        SHA1_ROUND(c, b, d, A, B, C, D, E, SHA1_MIX, \
                   0x5a827999, (((C ^ D) & B) ^ D))

#define SHA1_20_39(c, b, d, A, B, C, D, E) \
        SHA1_ROUND(c, b, d, A, B, C, D, E, SHA1_MIX, \
                   0x6ed9eba1, (B ^ C ^ D))

#define SHA1_40_59(c, b, d, A, B, C, D, E) \
        SHA1_ROUND(c, b, d, A, B, C, D, E, SHA1_MIX, \
                   0x8f1bbcdc, ((B & C) + (D & (B ^ C))))

#define SHA1_60_79(c, b, d, A, B, C, D, E) \
        SHA1_ROUND(c, b, d, A, B, C, D, E, SHA1_MIX, \
                   0xca62c1d6, (B ^ C ^ D))

static inline void
bfdev_sha1_transform(uint32_t *digest, uint32_t *block, const char *data)
{
    uint32_t A, B, C, D, E;
    unsigned int count;

    A = digest[0];
    B = digest[1];
    C = digest[2];
    D = digest[3];
    E = digest[4];
    count = 0;

	/* Round 1 MIX */
    for (; count < 16; ++count)
        SHA1_00_15(count, block, data, A, B, C, D, E);

	/* Round 1 SRC */
    for (; count < 20; ++count)
        SHA1_16_19(count, block, data, A, B, C, D, E);

	/* Round 2 */
    for (; count < 40; ++count)
        SHA1_20_39(count, block, data, A, B, C, D, E);

	/* Round 3 */
    for (; count < 60; ++count)
        SHA1_40_59(count, block, data, A, B, C, D, E);

	/* Round 4 */
    for (; count < 80; ++count)
        SHA1_60_79(count, block, data, A, B, C, D, E);

    digest[0] += A;
    digest[1] += B;
    digest[2] += C;
    digest[3] += D;
    digest[4] += E;
}

BFDEV_END_DECLS

#endif /* _BFDEV_CRYPTO_SHA1_H_ */
