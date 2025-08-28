/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <bfdev/crc.h>
#include <bfdev/crypto/crc7-inline.h>
#include <export.h>

export bfdev_u8
bfdev_crc7(const void *data, bfdev_size_t len, bfdev_u8 crc)
{
    return bfdev_crc7_inline(data, len, crc);
}
