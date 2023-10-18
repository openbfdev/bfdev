/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRC_H_
#define _BFDEV_CRC_H_

#include <bfdev/config.h>
#include <bfdev/types.h>

BFDEV_BEGIN_DECLS

extern uint8_t
bfdev_crc4(const uint8_t *src, size_t bits, uint8_t crc);

extern uint8_t
bfdev_crc7(const uint8_t *src, size_t len, uint8_t crc);

extern uint8_t
bfdev_crc8(const uint8_t *src, size_t len, uint8_t crc);

extern uint16_t
bfdev_crc16(const uint8_t *src, size_t len, uint16_t crc);

extern uint32_t
bfdev_crc32(const uint8_t *src, size_t len, uint32_t crc);

extern uint64_t
bfdev_crc64(const uint8_t *src, size_t len, uint64_t crc);

extern uint16_t
bfdev_crc_ccitt(const uint8_t *src, size_t len, uint16_t crc);

extern uint16_t
bfdev_crc_itut(const uint8_t *src, size_t len, uint16_t crc);

extern uint16_t
bfdev_crc_t10dif(const uint8_t *src, size_t len, uint16_t crc);

extern uint64_t
bfdev_crc_rocksoft(const uint8_t *src, size_t len, uint64_t crc);

BFDEV_END_DECLS

#endif /* _BFDEV_CRC_H_ */
