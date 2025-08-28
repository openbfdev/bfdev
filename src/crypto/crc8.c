/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <bfdev/crc.h>
#include <bfdev/crypto/crc8-inline.h>
#include <export.h>

export bfdev_u8
bfdev_crc8(const void *data, bfdev_size_t len, bfdev_u8 crc)
{
    return bfdev_crc8_inline(data, len, crc);
}
