/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRYPTO_CRC64_INLINE_H_
#define _BFDEV_CRYPTO_CRC64_INLINE_H_

#include <bfdev/config.h>
#include <bfdev/crypto/crc-inline.h>

BFDEV_BEGIN_DECLS

#include <bfdev/crypto/crc64-table.h>

BFDEV_CRC_INLINE(
    bfdev_crc64, uint64_t,
    bfdev_crc64_table, bfdev_cpu_to_be64
)

BFDEV_END_DECLS

#endif /* _BFDEV_CRYPTO_CRC64_INLINE_H_ */
