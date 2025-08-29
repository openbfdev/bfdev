/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRC_H_
#define _BFDEV_CRC_H_

#include <bfdev/config.h>
#include <bfdev/types.h>

BFDEV_BEGIN_DECLS

extern bfdev_u8
bfdev_crc4(const void *data, bfdev_size_t bits, bfdev_u8 crc);

extern bfdev_u8
bfdev_crc7(const void *data, bfdev_size_t len, bfdev_u8 crc);

extern bfdev_u8
bfdev_crc8(const void *data, bfdev_size_t len, bfdev_u8 crc);

extern bfdev_u16
bfdev_crc16(const void *data, bfdev_size_t len, bfdev_u16 crc);

extern bfdev_u32
bfdev_crc32(const void *data, bfdev_size_t len, bfdev_u32 crc);

extern bfdev_u64
bfdev_crc64(const void *data, bfdev_size_t len, bfdev_u64 crc);

extern bfdev_u16
bfdev_crc_ccitt(const void *data, bfdev_size_t len, bfdev_u16 crc);

extern bfdev_u16
bfdev_crc_itut(const void *data, bfdev_size_t len, bfdev_u16 crc);

extern bfdev_u16
bfdev_crc_t10dif(const void *data, bfdev_size_t len, bfdev_u16 crc);

extern bfdev_u64
bfdev_crc_rocksoft(const void *data, bfdev_size_t len, bfdev_u64 crc);

BFDEV_END_DECLS

#endif /* _BFDEV_CRC_H_ */
