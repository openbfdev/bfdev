/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/base64.h>
#include <export.h>

static const uint8_t
base64_encode_table[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/',
};

static const uint8_t
base64_decode_table[256] = {
    [0 ... 255] = 0xff,
    ['A'] = 0x00, ['B'] = 0x01, ['C'] = 0x02, ['D'] = 0x03,
    ['E'] = 0x04, ['F'] = 0x05, ['G'] = 0x06, ['H'] = 0x07,
    ['I'] = 0x08, ['J'] = 0x09, ['K'] = 0x0a, ['L'] = 0x0b,
    ['M'] = 0x0c, ['N'] = 0x0d, ['O'] = 0x0e, ['P'] = 0x0f,
    ['Q'] = 0x10, ['R'] = 0x11, ['S'] = 0x12, ['T'] = 0x13,
    ['U'] = 0x14, ['V'] = 0x15, ['W'] = 0x16, ['X'] = 0x17,
    ['Y'] = 0x18, ['Z'] = 0x19, ['a'] = 0x1a, ['b'] = 0x1b,
    ['c'] = 0x1c, ['d'] = 0x1d, ['e'] = 0x1e, ['f'] = 0x1f,
    ['g'] = 0x20, ['h'] = 0x21, ['i'] = 0x22, ['j'] = 0x23,
    ['k'] = 0x24, ['l'] = 0x25, ['m'] = 0x26, ['n'] = 0x27,
    ['o'] = 0x28, ['p'] = 0x29, ['q'] = 0x2a, ['r'] = 0x2b,
    ['s'] = 0x2c, ['t'] = 0x2d, ['u'] = 0x2e, ['v'] = 0x2f,
    ['w'] = 0x30, ['x'] = 0x31, ['y'] = 0x32, ['z'] = 0x33,
    ['0'] = 0x34, ['1'] = 0x35, ['2'] = 0x36, ['3'] = 0x37,
    ['4'] = 0x38, ['5'] = 0x39, ['6'] = 0x3a, ['7'] = 0x3b,
    ['8'] = 0x3c, ['9'] = 0x3d, ['+'] = 0x3e, ['/'] = 0x3f,
};

static __bfdev_always_inline void
base64_encode(uint8_t *buff, const uint8_t *data, size_t size)
{
    const uint8_t *prev;
    unsigned int bstate;

    prev = data;
    for (; size >= 3; size -= 3) {
        *buff++ = base64_encode_table[*data++ >> 2];
        *buff++ = base64_encode_table[((*prev++ & 0x3) << 4) | (*data++ >> 4)];
        *buff++ = base64_encode_table[((*prev++ & 0xf) << 2) | (*data++ >> 6)];
        *buff++ = base64_encode_table[*prev++ & 0x3f];
    }

    bstate = 0;
    while (size--) switch (bstate) {
        case 0:
            *buff++ = base64_encode_table[*data++ >> 2];
            bstate = 1;
            break;

        case 1:
            *buff++ = base64_encode_table[((*prev++ & 0x3) << 4) | (*data++ >> 4)];
            bstate = 2;
            break;

        default:
            *buff++ = base64_encode_table[((*prev++ & 0xf) << 2) | (*data++ >> 6)];
            *buff++ = base64_encode_table[*prev++ & 0x3f];
            bstate = 0;
            break;
    }

    switch (bstate) {
        case 1:
            *buff++ = base64_encode_table[(*prev & 0x3) << 4];
            *buff++ = '=';
            *buff = '=';
            break;

        case 2:
            *buff++ = base64_encode_table[(*prev & 0xf) << 2];
            *buff = '=';
            break;

        default:
            break;
    }
}

static __bfdev_always_inline int
base64_decode(uint8_t *buff, const uint8_t *data, size_t size)
{
    unsigned int bstate;
    uint8_t decode;

    bstate = 0;
    while (size--) {
        decode = base64_decode_table[*data];
        if (decode == 0xff) {
            if (bfdev_likely(*data == '='))
                break;
            return -BFDEV_EINVAL;
        }

        data++;
        switch (bstate++ & 0x3) {
            case 0:
                *buff = decode << 2;
                break;

            case 1:
                *buff++ |= (decode >> 4) & 0x3;
                *buff = decode << 4;
                break;

            case 2:
                *buff++ |= (decode >> 2) & 0xf;
                *buff = decode << 6;
                break;

            default:
                *buff++ |= decode;
                break;
        }
    }

    return -BFDEV_ENOERR;
}

export void
bfdev_base64_encode(void *buff, const void *data, size_t size)
{
    base64_encode(buff, data, size);
}

export int
bfdev_base64_decode(void *buff, const void *data, size_t size)
{
    return base64_decode(buff, data, size);
}
