/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/crc.h>
#include <bfdev/crypto/crc32-inline.h>
#include <export.h>

export bfdev_u32
bfdev_crc32(const void *data, bfdev_size_t len, bfdev_u32 crc)
{
    return bfdev_crc32_inline(data, len, crc);
}
