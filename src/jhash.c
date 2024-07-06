/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/jhash.h>
#include <bfdev/unaligned.h>
#include <export.h>

static __bfdev_always_inline uint32_t
jhash(const uint8_t *data, size_t length, uint32_t initval)
{
    uint32_t a, b, c;

    /* Set up the internal state */
    a = BFDEV_JHASH_INITVAL + length + initval;

    /* All but the last block: affect some 32 bits of (a,b,c) */
    for (c = b = a; length > 12; length -= 12, data += 12) {
        a += bfdev_unaligned_get_u32(data);
        b += bfdev_unaligned_get_u32(data + 4);
        c += bfdev_unaligned_get_u32(data + 8);
        bfdev_jhash_mix(a, b, c);
    }

    /* Last block: affect all 32 bits of (c) */
    switch (length) {
        case 12: c += (uint32_t)data[11] << 24; bfdev_fallthrough;
        case 11: c += (uint32_t)data[10] << 16; bfdev_fallthrough;
        case 10: c += (uint32_t)data[ 9] <<  8; bfdev_fallthrough;
        case  9: c += (uint32_t)data[ 8] <<  0; bfdev_fallthrough;
        case  8: b += (uint32_t)data[ 7] << 24; bfdev_fallthrough;
        case  7: b += (uint32_t)data[ 6] << 16; bfdev_fallthrough;
        case  6: b += (uint32_t)data[ 5] <<  8; bfdev_fallthrough;
        case  5: b += (uint32_t)data[ 4] <<  0; bfdev_fallthrough;
        case  4: a += (uint32_t)data[ 3] << 24; bfdev_fallthrough;
        case  3: a += (uint32_t)data[ 2] << 16; bfdev_fallthrough;
        case  2: a += (uint32_t)data[ 1] <<  8; bfdev_fallthrough;
        case  1: a += (uint32_t)data[ 0] <<  0;
            bfdev_jhash_final(a, b, c);
            break;

        /* Nothing left to add */
        default:
            break;
    }

    return c;
}

export uint32_t
bfdev_jhash(const void *data, size_t length, uint32_t initval)
{
    return jhash(data, length, initval);
}
