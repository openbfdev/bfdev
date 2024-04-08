/* SPDX-License-Identifier: GPL-2.0-or-later */
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
ascii85_encode(char *buff, const void *data, size_t size)
{
    uint32_t value;

    for (; size >= 4; size -= 4) {
        value = bfdev_unaligned_get_be32(data);
        data += 4;

        if (value == 0)
            *buff++ = 'z';
        else {
            buff[4] = '!' + value % 85; value /= 85;
            buff[3] = '!' + value % 85; value /= 85;
            buff[2] = '!' + value % 85; value /= 85;
            buff[1] = '!' + value % 85; value /= 85;
            buff[0] = '!' + value % 85;
            buff += 5;
        }
    }

    if (size) {
        value = bfdev_unaligned_get_be32(data);
        value &= BFDEV_BIT_HIGH_MASK((BFDEV_BYTES_PER_U32 - size) * BFDEV_BITS_PER_BYTE);

        if (value == 0)
            *buff++ = 'z';
        else {
            buff[4] = '!' + value % 85; value /= 85;
            buff[3] = '!' + value % 85; value /= 85;
            buff[2] = '!' + value % 85; value /= 85;
            buff[1] = '!' + value % 85; value /= 85;
            buff[0] = '!' + value % 85;
            buff += 5;
        }
    }

    *buff = '\0';
}

static __bfdev_always_inline int
ascii85_decode(void *buff, const char *data, size_t size)
{
    unsigned int index;
    uint32_t value;

    while (size) {
        if (*data == 'z' && size >= 1) {
            bfport_memset(buff, 0, 4);
            data += 1;
            size -= 1;
        } else if (size >= 5) {
            for (index = 0; index < 5; ++index) {
                if (data[index] < '!' || 'u' < data[index])
                    return -BFDEV_EINVAL;
            }

            value  = data[0] - '!'; value *= 85;
            value += data[1] - '!'; value *= 85;
            value += data[2] - '!'; value *= 85;
            value += data[3] - '!'; value *= 85;
            value += data[4] - '!';
            bfdev_unaligned_set_be32(buff, value);

            data += 5;
            size -= 5;
        } else
            return -BFDEV_EINVAL;

        buff += 4;
    }

    return -BFDEV_ENOERR;
}

static __bfdev_always_inline size_t
ascii85_encode_length(const uint32_t *data, size_t size)
{
    size_t csize, length;
    uint32_t value;

    for (length = 0; (csize = bfdev_min(size, 4)); size -= csize) {
        value = bfdev_unaligned_get_be32(data);
        value &= BFDEV_BIT_HIGH_MASK((BFDEV_BYTES_PER_U32 - size) * BFDEV_BITS_PER_BYTE);
        length += value ? 5 : 1;
        data++;
    }

    return length + 1;
}

static __bfdev_always_inline size_t
ascii85_decode_length(const char *data, size_t size)
{
    while ((data = bfport_strchr(data, 'z'))) {
        size += 4;
        data++;
    }

    return BFDEV_DIV_ROUND_UP(size, 5) * 4;
}

export void
bfdev_ascii85_encode(void *buff, const void *data, size_t size)
{
    ascii85_encode(buff, data, size);
}

export int
bfdev_ascii85_decode(void *buff, const void *data, size_t size)
{
    return ascii85_decode(buff, data, size);
}

export size_t
bfdev_ascii85_encode_length(const void *data, size_t size)
{
    return ascii85_encode_length(data, size);
}

export size_t
bfdev_ascii85_decode_length(const void *data, size_t size)
{
    return ascii85_decode_length(data, size);
}
