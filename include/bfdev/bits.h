/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BITS_H_
#define _BFDEV_BITS_H_

#include "stdint.h"
#include "limits.h"
#include "bitsperlong.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BFDEV_BITS_PER_BYTE         __CHAR_BIT__
#define BFDEV_BITS_PER_TYPE(type)   (sizeof(type) * BFDEV_BITS_PER_BYTE)
#define BFDEV_BITS_WORD(bit)        ((bit) / BFDEV_BITS_PER_LONG)

#define BFDEV_BITS_PER_U8           BFDEV_BITS_PER_TYPE(uint8_t)
#define BFDEV_BITS_PER_U16          BFDEV_BITS_PER_TYPE(uint16_t)
#define BFDEV_BITS_PER_U32          BFDEV_BITS_PER_TYPE(uint32_t)
#define BFDEV_BITS_PER_U64          BFDEV_BITS_PER_TYPE(uint64_t)

#define BFDEV_BITS_PER_SHORT        BFDEV_BITS_PER_TYPE(short)
#define BFDEV_BITS_PER_INT          BFDEV_BITS_PER_TYPE(int)
#define BFDEV_BITS_PER_LONG_LONG    BFDEV_BITS_PER_TYPE(long long)

/**
 * BFDEV_BIT_LOW_MASK - create a low position mask.
 * @nbits: mask length.
 * For example:
 * BFDEV_BIT_LOW_MASK(8) gives us the vector 0x000000ff.
 */
#define BFDEV_BIT_LOW_MASK(nbits) ( \
    ULONG_MAX >> \
    (-(nbits) & (BFDEV_BITS_PER_LONG - 1)) \
)

/**
 * BFDEV_BIT_HIGH_MASK - create a high position mask.
 * @nbits: mask length.
 * For example:
 * BFDEV_BIT_HIGH_MASK(8) gives us the vector 0xffffff00.
 */
#define BFDEV_BIT_HIGH_MASK(nbits) ( \
    ULONG_MAX << \
    ((nbits) & (BFDEV_BITS_PER_LONG - 1)) \
)

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_BITS_H_ */
