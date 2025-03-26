/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/crc.h>
#include <bfdev/crypto/crc64-inline.h>
#include <export.h>

export bfdev_u64
bfdev_crc64(const void *data, bfdev_size_t len, bfdev_u64 crc)
{
    return bfdev_crc64_inline(data, len, crc);
}
