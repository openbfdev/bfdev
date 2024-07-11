/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_BYTEORDER_H_
#define _BFDEV_ASM_GENERIC_BYTEORDER_H_

#include <bfdev/config.h>
#include <bfdev/byteorder/little-endian.h>

BFDEV_BEGIN_DECLS

#define BFDEV_GENERIC_BYTEORDER_ARRAY(name, type1, type2, func) \
static inline void                                              \
name(type1 *dest, const type2 *src, size_t size)                \
{                                                               \
    size_t index;                                               \
                                                                \
	for (index = 0; index < size; ++index)                      \
		dest[index] = func(src[index]);                         \
}

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_cpu_to_le16_array,
    bfdev_le16, uint16_t, bfdev_cpu_to_le16
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_cpu_to_le32_array,
    bfdev_le32, uint32_t, bfdev_cpu_to_le32
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_cpu_to_le64_array,
    bfdev_le64, uint64_t, bfdev_cpu_to_le64
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_le16_to_cpu_array,
    uint16_t, bfdev_le16, bfdev_le16_to_cpu
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_le32_to_cpu_array,
    uint32_t, bfdev_le32, bfdev_le32_to_cpu
)

BFDEV_GENERIC_BYTEORDER_ARRAY(
    bfdev_le64_to_cpu_array,
    uint64_t, bfdev_le64, bfdev_le64_to_cpu
)

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_BYTEORDER_H_ */
