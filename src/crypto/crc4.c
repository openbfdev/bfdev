/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/crc.h>
#include <bfdev/crypto/crc4-table.h>
#include <export.h>

export uint8_t
bfdev_crc4(const uint8_t *src, size_t len, uint8_t crc)
{
    return bfdev_crc4_inline(src, len, crc);
}
