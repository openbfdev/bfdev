/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BASE32_H_
#define _BFDEV_BASE32_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/errno.h>
#include <bfdev/math.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_base32_encode_length() - base32 encode buffer length.
 * @size: length to encode.
 */
static inline size_t
bfdev_base32_encode_length(size_t size)
{
    return BFDEV_DIV_ROUND_UP(size, 5) * 8;
}

/**
 * bfdev_base32_decode_length() - base32 decoder buffer length.
 * @size: length to decode.
 */
static inline size_t
bfdev_base32_decode_length(size_t size)
{
    return BFDEV_DIV_ROUND_UP(size, 8) * 5;
}

/**
 * bfdev_base32_encode() - base32 encoder.
 * @buff: encoder output buffer.
 * @data: data to encode.
 * @size: length of @data.
 */
extern void __bfdev_nonnull(1, 2)
bfdev_base32_encode(void *buff, const void *data, size_t size);

/**
 * bfdev_base32_decode() - base32 decoder.
 * @buff: decode output buffer.
 * @data: data to decode.
 * @size: length of @data.
 */
extern int __bfdev_nonnull(1, 2)
bfdev_base32_decode(void *buff, const void *data, size_t size);

BFDEV_END_DECLS

#endif /* _BFDEV_BASE32_H_ */
