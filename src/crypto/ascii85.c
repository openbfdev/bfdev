/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/ascii85.h>
#include <bfdev/math.h>
#include <bfdev/bits.h>
#include <bfdev/unaligned.h>
#include <export.h>

static __bfdev_always_inline void
ascii85_encode(uint8_t *buff, const void *data, size_t *plen, size_t size)
{
    uint32_t value;
    size_t index;

    for (index = 0; size >= 4; size -= 4) {
        value = bfdev_unaligned_get_be32(data);
        data += 4;

        if (!value)
            buff[index] = 'z';
        else {
            buff[index + 4] = '!' + value % 85; value /= 85;
            buff[index + 3] = '!' + value % 85; value /= 85;
            buff[index + 2] = '!' + value % 85; value /= 85;
            buff[index + 1] = '!' + value % 85; value /= 85;
            buff[index] = '!' + value % 85;
            index += 4;
        }

        index++;
    }

    if (size) {
        value = bfdev_unaligned_get_be32(data);
        value &= BFDEV_BIT_HIGH_MASK((BFDEV_BYTES_PER_U32 - size) * BFDEV_BITS_PER_BYTE);

        if (!value)
            buff[index] = 'z';
        else {
            buff[index + 4] = '!' + value % 85; value /= 85;
            buff[index + 3] = '!' + value % 85; value /= 85;
            buff[index + 2] = '!' + value % 85; value /= 85;
            buff[index + 1] = '!' + value % 85; value /= 85;
            buff[index] = '!' + value % 85;
            index += 4;
        }

        index++;
    }

    *plen = index;
}

static __bfdev_always_inline int
ascii85_decode(void *buff, const uint8_t *data, size_t *plen, size_t size)
{
    unsigned int count;
    uint32_t value;
    size_t index;

    for (index = 0; size; index += 4) {
        if (*data == 'z') {
            bfport_memset(buff + index, 0, 4);
            data++;
            size--;
        }

        if (size >= 5) {
            for (count = 0; count < 5; ++count) {
                if (data[count] < '!' || 'u' < data[count])
                    return -BFDEV_EINVAL;
            }

            value = data[0] - '!'; value *= 85;
            value += data[1] - '!'; value *= 85;
            value += data[2] - '!'; value *= 85;
            value += data[3] - '!'; value *= 85;
            value += data[4] - '!';
            bfdev_unaligned_set_be32(buff + index, value);

            data += 5;
            size -= 5;
            continue;
        }

        return -BFDEV_EPROTO;
    }

    *plen = index;

    return -BFDEV_ENOERR;
}

export void
bfdev_ascii85_encode(void *buff, const void *data, size_t *plen, size_t size)
{
    ascii85_encode(buff, data, plen, size);
}

export int
bfdev_ascii85_decode(void *buff, const void *data, size_t *plen, size_t size)
{
    return ascii85_decode(buff, data, plen, size);
}
