/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRYPTO_CRC_ITUT_INLINE_H_
#define _BFDEV_CRYPTO_CRC_ITUT_INLINE_H_

#include <bfdev/config.h>
#include <bfdev/crypto/crc-inline.h>

BFDEV_BEGIN_DECLS

#include <bfdev/crypto/crc-itut-table.h>

BFDEV_CRC_INLINE(
    bfdev_crc_itut, uint16_t,
    bfdev_crc_itut_table, bfdev_cpu_to_be16
)

BFDEV_END_DECLS

#endif /* _BFDEV_CRYPTO_CRC_ITUT_INLINE_H_ */
