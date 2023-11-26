/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/crc.h>
#include <bfdev/crypto/crc16-inline.h>
#include <export.h>

export uint16_t
bfdev_crc16(const void *data, size_t len, uint16_t crc)
{
    return bfdev_crc16_inline(data, len, crc);
}
