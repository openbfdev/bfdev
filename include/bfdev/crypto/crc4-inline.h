/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRYPTO_CRC4_INLINE_H_
#define _BFDEV_CRYPTO_CRC4_INLINE_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/minmax.h>
#include <bfdev/bits.h>

BFDEV_BEGIN_DECLS

static const bfdev_u8
bfdev_crc4_table[16] = {
    0x00, 0x07, 0x0e, 0x09,
    0x0b, 0x0c, 0x05, 0x02,
    0x01, 0x06, 0x0f, 0x08,
    0x0a, 0x0d, 0x04, 0x03,
};

static inline bfdev_u8
bfdev_crc4_byte(bfdev_u8 crc, const bfdev_u8 data)
{
    unsigned int index;

    index = (crc ^ (data >> 4)) & 0xf;
    crc = bfdev_crc4_table[index];

    index = (crc ^ (data & 0xf)) & 0xf;
    crc = bfdev_crc4_table[index];

    return crc;
}

static inline bfdev_u8
bfdev_crc4_inline(const bfdev_u8 *src, bfdev_size_t bits, bfdev_u8 crc)
{
    unsigned int count;
    bfdev_u8 value;

    for (; bits >= 32; bits -= 32) {
        crc = bfdev_crc4_byte(crc, *src++);
        crc = bfdev_crc4_byte(crc, *src++);
        crc = bfdev_crc4_byte(crc, *src++);
        crc = bfdev_crc4_byte(crc, *src++);
    }

    for (; (count = bfdev_min(bits, 8)); bits -= count) {
        value = BFDEV_BIT_LOW_MASK(bits) & *src++;
        crc = bfdev_crc4_byte(crc, value);
    }

    return crc;
}

BFDEV_END_DECLS

#endif /* _BFDEV_CRYPTO_CRC4_INLINE_H_ */
