/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRYPTO_CRC_CCITT_INLINE_H_
#define _BFDEV_CRYPTO_CRC_CCITT_INLINE_H_

#include <bfdev/config.h>
#include <bfdev/crypto/crc-inline.h>

BFDEV_BEGIN_DECLS

#include <bfdev/crypto/crc-ccitt-table.h>

BFDEV_CRC_INLINE(
    bfdev_crc_ccitt, uint16_t,
    bfdev_crc_ccitt_table, bfdev_cpu_to_le16
)

BFDEV_END_DECLS

#endif /* _BFDEV_CRYPTO_CRC_CCITT_INLINE_H_ */
