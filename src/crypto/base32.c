/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/base32.h>
#include <export.h>

static const uint8_t
base32_encode_table[32] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', '2', '3', '4', '5', '6', '7',
};

static const uint8_t
base32_decode_table[256] = {
    [0 ... 255] = 0xff,
    ['A'] = 0x00, ['B'] = 0x01, ['C'] = 0x02, ['D'] = 0x03,
    ['E'] = 0x04, ['F'] = 0x05, ['G'] = 0x06, ['H'] = 0x07,
    ['I'] = 0x08, ['J'] = 0x09, ['K'] = 0x0a, ['L'] = 0x0b,
    ['M'] = 0x0c, ['N'] = 0x0d, ['O'] = 0x0e, ['P'] = 0x0f,
    ['Q'] = 0x10, ['R'] = 0x11, ['S'] = 0x12, ['T'] = 0x13,
    ['U'] = 0x14, ['V'] = 0x15, ['W'] = 0x16, ['X'] = 0x17,
    ['Y'] = 0x18, ['Z'] = 0x19, ['2'] = 0x1a, ['3'] = 0x1b,
    ['4'] = 0x1c, ['5'] = 0x1d, ['6'] = 0x1e, ['7'] = 0x1f,
};

static __bfdev_always_inline void
base32_encode(uint8_t *buff, const uint8_t *data, size_t size)
{
    const uint8_t *prev;
    unsigned int bstate;

    prev = data;
    for (; size >= 5; size -= 5) {
        *buff++ = base32_encode_table[*data++ >> 3];
        *buff++ = base32_encode_table[((*prev++ & 0x7) << 2) | (*data++ >> 6)];
        *buff++ = base32_encode_table[(*prev >> 1) & 0x1f];
        *buff++ = base32_encode_table[((*prev++ & 0x1) << 4) | (*data++ >> 4)];
        *buff++ = base32_encode_table[((*prev++ & 0xf) << 1) | (*data++ >> 7)];
        *buff++ = base32_encode_table[(*prev >> 2) & 0x1f];
        *buff++ = base32_encode_table[((*prev++ & 0x3) << 3) | (*data++ >> 5)];
        *buff++ = base32_encode_table[*prev++ & 0x1f];
    }

    bstate = 0;
    while (size--) switch (bstate) {
        case 0:
            *buff++ = base32_encode_table[*data++ >> 3];
            bstate = 1;
            break;

        case 1:
            *buff++ = base32_encode_table[((*prev++ & 0x7) << 2) | (*data++ >> 6)];
            *buff++ = base32_encode_table[(*prev >> 1) & 0x1f];
            bstate = 2;
            break;

        case 2:
            *buff++ = base32_encode_table[((*prev++ & 0x1) << 4) | (*data++ >> 4)];
            bstate = 3;
            break;

        case 3:
            *buff++ = base32_encode_table[((*prev++ & 0xf) << 1) | (*data++ >> 7)];
            *buff++ = base32_encode_table[(*prev >> 2) & 0x1f];
            bstate = 4;
            break;

        default:
            *buff++ = base32_encode_table[((*prev++ & 0x3) << 3) | (*data++ >> 5)];
            *buff++ = base32_encode_table[*prev++ & 0x1f];
            bstate = 0;
            break;
    }

    switch (bstate) {
        case 1:
            *buff++ = base32_encode_table[(*prev & 0x7) << 2];
            *buff++ = '=';
            *buff++ = '=';
            *buff++ = '=';
            *buff++ = '=';
            *buff++ = '=';
            *buff = '=';
            break;

        case 2:
            *buff++ = base32_encode_table[(*prev & 0x1) << 4];
            *buff++ = '=';
            *buff++ = '=';
            *buff++ = '=';
            *buff = '=';
            break;

        case 3:
            *buff++ = base32_encode_table[(*prev & 0xf) << 1];
            *buff++ = '=';
            *buff++ = '=';
            *buff = '=';
            break;

        case 4:
            *buff++ = base32_encode_table[(*prev & 0x3) << 3];
            *buff = '=';
            break;

        default:
            break;
    }
}

static __bfdev_always_inline int
base32_decode(uint8_t *buff, const uint8_t *data, size_t size)
{
    unsigned int bstate;
    uint8_t decode;

    bstate = 0;
    while (size--) {
        decode = base32_decode_table[*data];
        if (decode == 0xff) {
            if (bfdev_likely(*data == '='))
                break;
            return -BFDEV_EINVAL;
        }

        data++;
        switch (bstate++ & 0x7) {
            case 0:
                *buff = decode << 3;
                break;

            case 1:
                *buff++ |= (decode >> 2) & 0x7;
                *buff = decode << 6;
                break;

            case 2:
                *buff |= (decode & 0x1f) << 1;
                break;

            case 3:
                *buff++ |= (decode >> 4) & 0x1;
                *buff = decode << 4;
                break;

            case 4:
                *buff++ |= (decode >> 1) & 0xf;
                *buff = decode << 7;
                break;

            case 5:
                *buff |= (decode & 0x1f) << 2;
                break;

            case 6:
                *buff++ |= (decode >> 3) & 0x3;
                *buff = decode << 5;
                break;

            default:
                *buff++ |= decode & 0x1f;
                break;
        }
    }

    return -BFDEV_ENOERR;
}

export void
bfdev_base32_encode(void *buff, const void *data, size_t size)
{
    base32_encode(buff, data, size);
}

export int
bfdev_base32_decode(void *buff, const void *data, size_t size)
{
    return base32_decode(buff, data, size);
}
