/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRYPTO_CRC_ROCKSOFT_INLINE_H_
#define _BFDEV_CRYPTO_CRC_ROCKSOFT_INLINE_H_

#include <bfdev/config.h>
#include <bfdev/crypto/crc-inline.h>

BFDEV_BEGIN_DECLS

#include <bfdev/crypto/crc-rocksoft-table.h>

BFDEV_CRC_INLINE(
    bfdev_crc_rocksoft, uint64_t,
    bfdev_crc_rocksoft_table, bfdev_cpu_to_le64
)

BFDEV_END_DECLS

#endif /* _BFDEV_CRYPTO_CRC_ROCKSOFT_INLINE_H_ */
