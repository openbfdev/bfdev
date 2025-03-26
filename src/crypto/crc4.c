/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/crc.h>
#include <bfdev/crypto/crc4-inline.h>
#include <export.h>

export bfdev_u8
bfdev_crc4(const void *data, bfdev_size_t len, bfdev_u8 crc)
{
    return bfdev_crc4_inline(data, len, crc);
}
