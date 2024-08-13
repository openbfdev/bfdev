/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRYPTO_CRC8_INLINE_H_
#define _BFDEV_CRYPTO_CRC8_INLINE_H_

#include <bfdev/config.h>
#include <bfdev/crypto/crc-inline.h>

BFDEV_BEGIN_DECLS

#include <bfdev/crypto/crc8-table.h>

BFDEV_CRC_INLINE(
    bfdev_crc8, uint8_t,
    bfdev_crc8_table, (uint8_t)
)

BFDEV_END_DECLS

#endif /* _BFDEV_CRYPTO_CRC8_INLINE_H_ */
