/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/jhash.h>
#include <bfdev/unaligned.h>
#include <export.h>

static __bfdev_always_inline bfdev_u32
jhash(const bfdev_u8 *data, bfdev_size_t length, bfdev_u32 initval)
{
    bfdev_u32 a, b, c;

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
        case 12: c += (bfdev_u32)data[11] << 24; bfdev_fallthrough;
        case 11: c += (bfdev_u32)data[10] << 16; bfdev_fallthrough;
        case 10: c += (bfdev_u32)data[ 9] <<  8; bfdev_fallthrough;
        case  9: c += (bfdev_u32)data[ 8] <<  0; bfdev_fallthrough;
        case  8: b += (bfdev_u32)data[ 7] << 24; bfdev_fallthrough;
        case  7: b += (bfdev_u32)data[ 6] << 16; bfdev_fallthrough;
        case  6: b += (bfdev_u32)data[ 5] <<  8; bfdev_fallthrough;
        case  5: b += (bfdev_u32)data[ 4] <<  0; bfdev_fallthrough;
        case  4: a += (bfdev_u32)data[ 3] << 24; bfdev_fallthrough;
        case  3: a += (bfdev_u32)data[ 2] << 16; bfdev_fallthrough;
        case  2: a += (bfdev_u32)data[ 1] <<  8; bfdev_fallthrough;
        case  1: a += (bfdev_u32)data[ 0] <<  0;
            bfdev_jhash_final(a, b, c);
            break;

        /* Nothing left to add */
        default:
            break;
    }

    return c;
}

export bfdev_u32
bfdev_jhash(const void *data, bfdev_size_t length, bfdev_u32 initval)
{
    return jhash(data, length, initval);
}
