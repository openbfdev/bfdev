/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_UPLOWER_
#define _BFDEV_UPLOWER_

#include <bfdev/config.h>
#include <bfdev/types.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_upper_16_bits - return bits 16-31 of a number.
 * @val: the value we're accessing.
 */
#define bfdev_upper_16_bits(val) ( \
    (uint16_t)((val) >> 16) \
)

/**
 * bfdev_lower_16_bits - return bits 0-15 of a number.
 * @val: the value we're accessing.
 */
#define bfdev_lower_16_bits(val) ( \
    (uint16_t)((val) & 0xffff) \
)

/**
 * bfdev_upper_32_bits - return bits 32-63 of a number.
 * @val: the value we're accessing.
 */
#define bfdev_upper_32_bits(val) ( \
    (uint32_t)(((val) >> 16) >> 16) \
)

/**
 * bfdev_lower_32_bits - return bits 0-31 of a number.
 * @val: the value we're accessing.
 */
#define bfdev_lower_32_bits(val) ( \
    (uint32_t)((val) & 0xffffffff) \
)

BFDEV_END_DECLS

#endif /* _BFDEV_UPLOWER_ */
