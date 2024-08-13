/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BASE64_H_
#define _BFDEV_BASE64_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/errno.h>
#include <bfdev/math.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_base64_encode_length() - base64 encode buffer length.
 * @size: length to encode.
 */
static inline size_t
bfdev_base64_encode_length(size_t size)
{
    return BFDEV_DIV_ROUND_UP(size, 3) * 4;
}

/**
 * bfdev_base64_decode_length() - base64 decoder buffer length.
 * @size: length to decode.
 */
static inline size_t
bfdev_base64_decode_length(size_t size)
{
    return BFDEV_DIV_ROUND_UP(size, 4) * 3;
}

/**
 * bfdev_base64_encode() - base64 encoder.
 * @buff: encoder output buffer.
 * @data: data to encode.
 * @size: length of @data.
 */
extern void
bfdev_base64_encode(void *buff, const void *data, size_t size);

/**
 * bfdev_base64_decode() - base64 decoder.
 * @buff: decode output buffer.
 * @data: data to decode.
 * @size: length of @data.
 */
extern int
bfdev_base64_decode(void *buff, const void *data, size_t size);

BFDEV_END_DECLS

#endif /* _BFDEV_BASE64_H_ */
