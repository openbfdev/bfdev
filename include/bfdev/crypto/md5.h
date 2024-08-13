/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(C) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRYPTO_MD5_H_
#define _BFDEV_CRYPTO_MD5_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/bits.h>

BFDEV_BEGIN_DECLS

#define BFDEV_MD5_F1(x, y, z) (z ^ (x & (y ^ z)))
#define BFDEV_MD5_F2(x, y, z) (BFDEV_MD5_F1(z, x, y))
#define BFDEV_MD5_F3(x, y, z) (x ^ y ^ z)
#define BFDEV_MD5_F4(x, y, z) (y ^ (x | ~z))

#define BFDEV_MD5_STEP(f, w, x, y, z, data, s) ({ \
    w += f(x, y, z) + data; \
    w = (w << s | w >> (BFDEV_BITS_PER_U32 - s)) + x; \
})

static inline void
bfdev_md5_transform(uint32_t *hash, const uint32_t *data)
{
    uint32_t A, B, C, D;

    A = hash[0];
    B = hash[1];
    C = hash[2];
    D = hash[3];

    /* Step 1 */
    BFDEV_MD5_STEP(BFDEV_MD5_F1, A, B, C, D, data[ 0] + 0xd76aa478, 7);
    BFDEV_MD5_STEP(BFDEV_MD5_F1, D, A, B, C, data[ 1] + 0xe8c7b756, 12);
    BFDEV_MD5_STEP(BFDEV_MD5_F1, C, D, A, B, data[ 2] + 0x242070db, 17);
    BFDEV_MD5_STEP(BFDEV_MD5_F1, B, C, D, A, data[ 3] + 0xc1bdceee, 22);
    BFDEV_MD5_STEP(BFDEV_MD5_F1, A, B, C, D, data[ 4] + 0xf57c0faf, 7);
    BFDEV_MD5_STEP(BFDEV_MD5_F1, D, A, B, C, data[ 5] + 0x4787c62a, 12);
    BFDEV_MD5_STEP(BFDEV_MD5_F1, C, D, A, B, data[ 6] + 0xa8304613, 17);
    BFDEV_MD5_STEP(BFDEV_MD5_F1, B, C, D, A, data[ 7] + 0xfd469501, 22);
    BFDEV_MD5_STEP(BFDEV_MD5_F1, A, B, C, D, data[ 8] + 0x698098d8, 7);
    BFDEV_MD5_STEP(BFDEV_MD5_F1, D, A, B, C, data[ 9] + 0x8b44f7af, 12);
    BFDEV_MD5_STEP(BFDEV_MD5_F1, C, D, A, B, data[10] + 0xffff5bb1, 17);
    BFDEV_MD5_STEP(BFDEV_MD5_F1, B, C, D, A, data[11] + 0x895cd7be, 22);
    BFDEV_MD5_STEP(BFDEV_MD5_F1, A, B, C, D, data[12] + 0x6b901122, 7);
    BFDEV_MD5_STEP(BFDEV_MD5_F1, D, A, B, C, data[13] + 0xfd987193, 12);
    BFDEV_MD5_STEP(BFDEV_MD5_F1, C, D, A, B, data[14] + 0xa679438e, 17);
    BFDEV_MD5_STEP(BFDEV_MD5_F1, B, C, D, A, data[15] + 0x49b40821, 22);

    /* Step 2 */
    BFDEV_MD5_STEP(BFDEV_MD5_F2, A, B, C, D, data[ 1] + 0xf61e2562, 5);
    BFDEV_MD5_STEP(BFDEV_MD5_F2, D, A, B, C, data[ 6] + 0xc040b340, 9);
    BFDEV_MD5_STEP(BFDEV_MD5_F2, C, D, A, B, data[11] + 0x265e5a51, 14);
    BFDEV_MD5_STEP(BFDEV_MD5_F2, B, C, D, A, data[ 0] + 0xe9b6c7aa, 20);
    BFDEV_MD5_STEP(BFDEV_MD5_F2, A, B, C, D, data[ 5] + 0xd62f105d, 5);
    BFDEV_MD5_STEP(BFDEV_MD5_F2, D, A, B, C, data[10] + 0x02441453, 9);
    BFDEV_MD5_STEP(BFDEV_MD5_F2, C, D, A, B, data[15] + 0xd8a1e681, 14);
    BFDEV_MD5_STEP(BFDEV_MD5_F2, B, C, D, A, data[ 4] + 0xe7d3fbc8, 20);
    BFDEV_MD5_STEP(BFDEV_MD5_F2, A, B, C, D, data[ 9] + 0x21e1cde6, 5);
    BFDEV_MD5_STEP(BFDEV_MD5_F2, D, A, B, C, data[14] + 0xc33707d6, 9);
    BFDEV_MD5_STEP(BFDEV_MD5_F2, C, D, A, B, data[ 3] + 0xf4d50d87, 14);
    BFDEV_MD5_STEP(BFDEV_MD5_F2, B, C, D, A, data[ 8] + 0x455a14ed, 20);
    BFDEV_MD5_STEP(BFDEV_MD5_F2, A, B, C, D, data[13] + 0xa9e3e905, 5);
    BFDEV_MD5_STEP(BFDEV_MD5_F2, D, A, B, C, data[ 2] + 0xfcefa3f8, 9);
    BFDEV_MD5_STEP(BFDEV_MD5_F2, C, D, A, B, data[ 7] + 0x676f02d9, 14);
    BFDEV_MD5_STEP(BFDEV_MD5_F2, B, C, D, A, data[12] + 0x8d2a4c8a, 20);

    /* Step 3 */
    BFDEV_MD5_STEP(BFDEV_MD5_F3, A, B, C, D, data[ 5] + 0xfffa3942, 4);
    BFDEV_MD5_STEP(BFDEV_MD5_F3, D, A, B, C, data[ 8] + 0x8771f681, 11);
    BFDEV_MD5_STEP(BFDEV_MD5_F3, C, D, A, B, data[11] + 0x6d9d6122, 16);
    BFDEV_MD5_STEP(BFDEV_MD5_F3, B, C, D, A, data[14] + 0xfde5380c, 23);
    BFDEV_MD5_STEP(BFDEV_MD5_F3, A, B, C, D, data[ 1] + 0xa4beea44, 4);
    BFDEV_MD5_STEP(BFDEV_MD5_F3, D, A, B, C, data[ 4] + 0x4bdecfa9, 11);
    BFDEV_MD5_STEP(BFDEV_MD5_F3, C, D, A, B, data[ 7] + 0xf6bb4b60, 16);
    BFDEV_MD5_STEP(BFDEV_MD5_F3, B, C, D, A, data[10] + 0xbebfbc70, 23);
    BFDEV_MD5_STEP(BFDEV_MD5_F3, A, B, C, D, data[13] + 0x289b7ec6, 4);
    BFDEV_MD5_STEP(BFDEV_MD5_F3, D, A, B, C, data[ 0] + 0xeaa127fa, 11);
    BFDEV_MD5_STEP(BFDEV_MD5_F3, C, D, A, B, data[ 3] + 0xd4ef3085, 16);
    BFDEV_MD5_STEP(BFDEV_MD5_F3, B, C, D, A, data[ 6] + 0x04881d05, 23);
    BFDEV_MD5_STEP(BFDEV_MD5_F3, A, B, C, D, data[ 9] + 0xd9d4d039, 4);
    BFDEV_MD5_STEP(BFDEV_MD5_F3, D, A, B, C, data[12] + 0xe6db99e5, 11);
    BFDEV_MD5_STEP(BFDEV_MD5_F3, C, D, A, B, data[15] + 0x1fa27cf8, 16);
    BFDEV_MD5_STEP(BFDEV_MD5_F3, B, C, D, A, data[ 2] + 0xc4ac5665, 23);

    /* Step 4 */
    BFDEV_MD5_STEP(BFDEV_MD5_F4, A, B, C, D, data[ 0] + 0xf4292244, 6);
    BFDEV_MD5_STEP(BFDEV_MD5_F4, D, A, B, C, data[ 7] + 0x432aff97, 10);
    BFDEV_MD5_STEP(BFDEV_MD5_F4, C, D, A, B, data[14] + 0xab9423a7, 15);
    BFDEV_MD5_STEP(BFDEV_MD5_F4, B, C, D, A, data[ 5] + 0xfc93a039, 21);
    BFDEV_MD5_STEP(BFDEV_MD5_F4, A, B, C, D, data[12] + 0x655b59c3, 6);
    BFDEV_MD5_STEP(BFDEV_MD5_F4, D, A, B, C, data[ 3] + 0x8f0ccc92, 10);
    BFDEV_MD5_STEP(BFDEV_MD5_F4, C, D, A, B, data[10] + 0xffeff47d, 15);
    BFDEV_MD5_STEP(BFDEV_MD5_F4, B, C, D, A, data[ 1] + 0x85845dd1, 21);
    BFDEV_MD5_STEP(BFDEV_MD5_F4, A, B, C, D, data[ 8] + 0x6fa87e4f, 6);
    BFDEV_MD5_STEP(BFDEV_MD5_F4, D, A, B, C, data[15] + 0xfe2ce6e0, 10);
    BFDEV_MD5_STEP(BFDEV_MD5_F4, C, D, A, B, data[ 6] + 0xa3014314, 15);
    BFDEV_MD5_STEP(BFDEV_MD5_F4, B, C, D, A, data[13] + 0x4e0811a1, 21);
    BFDEV_MD5_STEP(BFDEV_MD5_F4, A, B, C, D, data[ 4] + 0xf7537e82, 6);
    BFDEV_MD5_STEP(BFDEV_MD5_F4, D, A, B, C, data[11] + 0xbd3af235, 10);
    BFDEV_MD5_STEP(BFDEV_MD5_F4, C, D, A, B, data[ 2] + 0x2ad7d2bb, 15);
    BFDEV_MD5_STEP(BFDEV_MD5_F4, B, C, D, A, data[ 9] + 0xeb86d391, 21);

    hash[0] += A;
    hash[1] += B;
    hash[2] += C;
    hash[3] += D;
}

BFDEV_END_DECLS

#endif /* _BFDEV_CRYPTO_MD5_H_ */
