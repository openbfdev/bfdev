/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASCII85_H_
#define _BFDEV_ASCII85_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/errno.h>
#include <bfdev/math.h>

BFDEV_BEGIN_DECLS

static inline size_t
bfdev_ascii85_encode_length(size_t size)
{
    return BFDEV_DIV_ROUND_UP(size, 4) * 5;
}

static inline size_t
bfdev_ascii85_decode_length(size_t size)
{
    return BFDEV_DIV_ROUND_UP(size, 5) * 4;
}

extern void
bfdev_ascii85_encode(void *buff, const void *data, size_t size);

extern int
bfdev_ascii85_decode(void *buff, const void *data, size_t size);

BFDEV_END_DECLS

#endif /* _CRYPTO_ASCII85_H_ */
