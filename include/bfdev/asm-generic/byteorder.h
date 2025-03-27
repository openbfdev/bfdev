/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_BYTEORDER_H_
#define _BFDEV_ASM_GENERIC_BYTEORDER_H_

#include <bfdev/config.h>

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
# include <bfdev/byteorder/little-endian.h>
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
# include <bfdev/byteorder/big-endian.h>
#else
# error "Unknown byte order"
#endif

BFDEV_BEGIN_DECLS

#define BFDEV_GENERIC_BYTEORDER_ARRAY(name, type1, type2, func) \
static inline void                                              \
name(type1 *dest, const type2 *src, bfdev_size_t size)          \
{                                                               \
    bfdev_size_t index;                                         \
                                                                \
    for (index = 0; index < size; ++index)                      \
        dest[index] = func(src[index]);                         \
}

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_cpu_to_le16_array,
    bfdev_le16, bfdev_u16, bfdev_cpu_to_le16
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_cpu_to_le32_array,
    bfdev_le32, bfdev_u32, bfdev_cpu_to_le32
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_cpu_to_le64_array,
    bfdev_le64, bfdev_u64, bfdev_cpu_to_le64
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_le16_to_cpu_array,
    bfdev_u16, bfdev_le16, bfdev_le16_to_cpu
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_le32_to_cpu_array,
    bfdev_u32, bfdev_le32, bfdev_le32_to_cpu
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_le64_to_cpu_array,
    bfdev_u64, bfdev_le64, bfdev_le64_to_cpu
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_cpu_to_be16_array,
    bfdev_be16, bfdev_u16, bfdev_cpu_to_be16
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_cpu_to_be32_array,
    bfdev_be32, bfdev_u32, bfdev_cpu_to_be32
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_cpu_to_be64_array,
    bfdev_be64, bfdev_u64, bfdev_cpu_to_be64
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_be16_to_cpu_array,
    bfdev_u16, bfdev_be16, bfdev_be16_to_cpu
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_be32_to_cpu_array,
    bfdev_u32, bfdev_be32, bfdev_be32_to_cpu
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_be64_to_cpu_array,
    bfdev_u64, bfdev_be64, bfdev_be64_to_cpu
)

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_BYTEORDER_H_ */
