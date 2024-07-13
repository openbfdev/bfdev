/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Fredrik Anderson <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRYPTO_CRC32_INLINE_H_
#define _BFDEV_CRYPTO_CRC32_INLINE_H_

#include <bfdev/config.h>
#include <bfdev/crypto/crc-inline.h>

BFDEV_BEGIN_DECLS

#include <bfdev/crypto/crc32-table.h>

BFDEV_CRC_INLINE(
    bfdev_crc32, uint32_t,
    bfdev_crc32_table, bfdev_cpu_to_le32
)

BFDEV_END_DECLS

#endif /* _BFDEV_CRYPTO_CRC32_INLINE_H_ */
