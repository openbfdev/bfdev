/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BYTEORDER_H_
#define _BFDEV_BYTEORDER_H_

#include <bfdev/config.h>
#include <bfdev/bits.h>
#include <bfdev/asm/byteorder.h>

BFDEV_BEGIN_DECLS

#define _bfdev_cpu_to_le_type(type) bfdev_cpu_to_le##type
#define bfdev_cpu_to_le_type(type)  _bfdev_cpu_to_le_type(type)
#define _bfdev_cpu_to_be_type(type) bfdev_cpu_to_be##type
#define bfdev_cpu_to_be_type(type)  _bfdev_cpu_to_be_type(type)
#define _bfdev_le_type_to_cpu(type) bfdev_le##type##_to_cpu
#define bfdev_le_type_to_cpu(type)  _bfdev_le_type_to_cpu(type)
#define _bfdev_be_type_to_cpu(type) bfdev_be##type##_to_cpu
#define bfdev_be_type_to_cpu(type)  _bfdev_be_type_to_cpu(type)

#define _bfdev_cpu_to_le_type_array(type) bfdev_cpu_to_le##type##_array
#define bfdev_cpu_to_le_type_array(type)  _bfdev_cpu_to_le_type_array(type)
#define _bfdev_cpu_to_be_type_array(type) bfdev_cpu_to_be##type##_array
#define bfdev_cpu_to_be_type_array(type)  _bfdev_cpu_to_be_type_array(type)
#define _bfdev_le_type_to_cpu_array(type) bfdev_le##type##_to_cpu_array
#define bfdev_le_type_to_cpu_array(type)  _bfdev_le_type_to_cpu_array(type)
#define _bfdev_be_type_to_cpu_array(type) bfdev_be##type##_to_cpu_array
#define bfdev_be_type_to_cpu_array(type)  _bfdev_be_type_to_cpu_array(type)

#define bfdev_cpu_to_le_long(val) \
    bfdev_cpu_to_le_type(BFDEV_BITS_PER_LONG)(val)
#define bfdev_cpu_to_be_long(val) \
    bfdev_cpu_to_be_type(BFDEV_BITS_PER_LONG)(val)

#define bfdev_le_long_to_cpu(val) \
    bfdev_le_type_to_cpu(BFDEV_BITS_PER_LONG)(val)
#define bfdev_be_long_to_cpu(val) \
    bfdev_be_type_to_cpu(BFDEV_BITS_PER_LONG)(val)

#define bfdev_cpu_to_le_long_array(dest, src, size) \
    bfdev_cpu_to_le_type_array(BFDEV_BITS_PER_LONG)(dest, src, size)
#define bfdev_cpu_to_be_long_array(dest, src, size) \
    bfdev_cpu_to_be_type_array(BFDEV_BITS_PER_LONG)(dest, src, size)

#define bfdev_le_long_to_cpu_array(dest, src, size) \
    bfdev_le_type_to_cpu_array(BFDEV_BITS_PER_LONG)(dest, src, size)
#define bfdev_be_long_to_cpu_array(dest, src, size) \
    bfdev_be_type_to_cpu_array(BFDEV_BITS_PER_LONG)(dest, src, size)

BFDEV_END_DECLS

#endif /* _BFDEV_BYTEORDER_H_ */
