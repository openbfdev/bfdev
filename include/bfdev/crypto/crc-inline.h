/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRYPTO_CRC_INLINE_H_
#define _BFDEV_CRYPTO_CRC_INLINE_H_

#include <bfdev/config.h>
#include <bfdev/stdint.h>
#include <bfdev/stddef.h>
#include <bfdev/align.h>
#include <bfdev/byteorder.h>

BFDEV_BEGIN_DECLS

#define BFDEV_CRC_INLINE(name, type, table, bswap)                  \
static inline type                                                  \
name##_byte(type crc, const uint8_t data)                           \
{                                                                   \
    unsigned int index = (crc ^ data) & 0xff;                       \
    return table[0][index] ^ (crc >> 8);                            \
}                                                                   \
                                                                    \
static inline type                                                  \
name##_inline(const uint8_t *src, size_t len, type crc)             \
{                                                                   \
    const uint32_t *combine;                                        \
    uint32_t value[2];                                              \
                                                                    \
    crc = bswap(crc);                                               \
    while (len && !bfdev_align_ptr_check(src, sizeof(*value))) {    \
        crc = name##_byte(crc, *src++);                             \
        len--;                                                      \
    }                                                               \
                                                                    \
    for (combine = (const void *)src; len >= 8; len -= 8) {         \
        value[0] = bfdev_cpu_to_le32p(combine++) ^ crc;             \
        value[1] = bfdev_cpu_to_le32p(combine++);                   \
                                                                    \
        crc = (                                                     \
            table[7][(value[0] >>  0) & 0xff] ^                     \
            table[6][(value[0] >>  8) & 0xff] ^                     \
            table[5][(value[0] >> 16) & 0xff] ^                     \
            table[4][(value[0] >> 24) & 0xff] ^                     \
            table[3][(value[1] >>  0) & 0xff] ^                     \
            table[2][(value[1] >>  8) & 0xff] ^                     \
            table[1][(value[1] >> 16) & 0xff] ^                     \
            table[0][(value[1] >> 24) & 0xff]                       \
        );                                                          \
    }                                                               \
                                                                    \
    for (src = (const void *)combine; len; --len)                   \
        crc = name##_byte(crc, *src++);                             \
                                                                    \
    return bswap(crc);                                              \
}

BFDEV_END_DECLS

#endif /* _BFDEV_CRYPTO_CRC_INLINE_H_ */
