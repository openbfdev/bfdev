/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRYPTO_CRC_T10DIF_INLINE_H_
#define _BFDEV_CRYPTO_CRC_T10DIF_INLINE_H_

#include <bfdev/config.h>
#include <bfdev/crypto/crc-inline.h>

BFDEV_BEGIN_DECLS

#include <bfdev/crypto/crc-t10dif-table.h>

BFDEV_CRC_INLINE(
    bfdev_crc_t10dif, uint16_t,
    bfdev_crc_t10dif_table, bfdev_cpu_to_be16
)

BFDEV_END_DECLS

#endif /* _BFDEV_CRYPTO_CRC_T10DIF_INLINE_H_ */
