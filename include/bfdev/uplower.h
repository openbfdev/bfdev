/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
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
    (bfdev_u16)((val) >> 16) \
)

/**
 * bfdev_lower_16_bits - return bits 0-15 of a number.
 * @val: the value we're accessing.
 */
#define bfdev_lower_16_bits(val) ( \
    (bfdev_u16)((val) & 0xffff) \
)

/**
 * bfdev_upper_32_bits - return bits 32-63 of a number.
 * @val: the value we're accessing.
 */
#define bfdev_upper_32_bits(val) ( \
    (bfdev_u32)(((val) >> 16) >> 16) \
)

/**
 * bfdev_lower_32_bits - return bits 0-31 of a number.
 * @val: the value we're accessing.
 */
#define bfdev_lower_32_bits(val) ( \
    (bfdev_u32)((val) & 0xffffffff) \
)

BFDEV_END_DECLS

#endif /* _BFDEV_UPLOWER_ */
