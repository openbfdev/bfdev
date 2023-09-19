/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BCD_H_
#define _BFDEV_BCD_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/limits.h>

BFDEV_BEGIN_DECLS

#define bfdev_bcd2bin_const(bcd) ( \
    ((bcd) & 0x0f) + ((bcd) >> 4) * 10 \
)

#define bfdev_bin2bcd_const(bin) ( \
    (((bin) / 10) << 4) + (bin) % 10 \
)

extern const uint8_t bfdev_bcd2bin_table[256];
extern const uint8_t bfdev_bin2bcd_table[256];

static __bfdev_attribute_const inline
uint8_t bfdev_bcd2bin_dynamic(uint8_t bcd)
{
    return bfdev_bcd2bin_table[bcd];
}

static inline __bfdev_attribute_const
uint8_t bfdev_bin2bcd_dynamic(uint8_t bin)
{
    return bfdev_bin2bcd_table[bin];
}

/**
 * bfdev_bcd2bin - convert bcd to bin.
 * @bcd: the bcd to convert.
 */
#define bfdev_bcd2bin(bcd) ({       \
    uint8_t __bcd = (uint8_t)(bcd); \
    __builtin_constant_p(__bcd)     \
    ? bfdev_bcd2bin_const(__bcd)    \
    : bfdev_bcd2bin_dynamic(__bcd); \
})

/**
 * bfdev_bin2bcd - convert bin to bcd.
 * @bin: the bin to convert.
 */
#define bfdev_bin2bcd(bin) ({       \
    uint8_t __bin = (uint8_t)(bin); \
    __builtin_constant_p(__bin)     \
    ? bfdev_bin2bcd_const(__bin)    \
    : bfdev_bin2bcd_dynamic(__bin); \
})

BFDEV_END_DECLS

#endif /* _BFDEV_BCD_H_ */
