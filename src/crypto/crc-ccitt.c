/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <bfdev/crc.h>
#include <bfdev/crypto/crc-ccitt-inline.h>
#include <export.h>

export bfdev_u16
bfdev_crc_ccitt(const void *data, bfdev_size_t len, bfdev_u16 crc)
{
    return bfdev_crc_ccitt_inline(data, len, crc);
}
