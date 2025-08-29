/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASCII85_H_
#define _BFDEV_ASCII85_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/errno.h>
#include <bfdev/math.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_ascii85_encode_length() - ascii85 encode buffer length.
 * @size: length to encode.
 *
 * Get the maximum length a block of
 * data will encode to.
 */
static inline bfdev_size_t
bfdev_ascii85_encode_length(bfdev_size_t size)
{
    return BFDEV_DIV_ROUND_UP(size, 4) * 5;
}

/**
 * bfdev_ascii85_decode_length() - ascii85 decode buffer length.
 * @size: length to decode.
 *
 * Get the maximum length a block of
 * data will decode to.
 */
static inline bfdev_size_t
bfdev_ascii85_decode_length(bfdev_size_t size)
{
    return BFDEV_DIV_ROUND_UP(size, 5) * 4;
}

/**
 * bfdev_base32_encode() - ascii85 encoder.
 * @buff: encode output buffer.
 * @data: data to encode.
 * @plen: save output length.
 * @size: length of @data.
 */
extern void
bfdev_ascii85_encode(void *buff, const void *data,
                     bfdev_size_t *plen, bfdev_size_t size);

/**
 * bfdev_ascii85_decode() - ascii85 decoder.
 * @buff: decode output buffer.
 * @data: data to decode.
 * @plen: save output length.
 * @size: length of @data.
 */
extern int
bfdev_ascii85_decode(void *buff, const void *data,
                     bfdev_size_t *plen, bfdev_size_t size);

BFDEV_END_DECLS

#endif /* _CRYPTO_ASCII85_H_ */
