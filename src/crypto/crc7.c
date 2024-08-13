/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/crc.h>
#include <bfdev/crypto/crc7-inline.h>
#include <export.h>

export uint8_t
bfdev_crc7(const void *data, size_t len, uint8_t crc)
{
    return bfdev_crc7_inline(data, len, crc);
}
