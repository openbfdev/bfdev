/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/crc.h>
#include <bfdev/crypto/crc-t10dif-table.h>
#include <export.h>

export uint16_t
bfdev_crc_t10dif(const uint8_t *src, size_t len, uint16_t crc)
{
    return bfdev_crc_t10dif_inline(src, len, crc);
}
