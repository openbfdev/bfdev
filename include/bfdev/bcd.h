/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BCD_H_
#define _BFDEV_BCD_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/limits.h>

BFDEV_BEGIN_DECLS

/*
 * Interface for known constant arguments
 */

#define bfdev_bcd2bin_const(bcd) ( \
    ((bcd) & 0x0f) + ((bcd) >> 4) * 10 \
)

#define bfdev_bin2bcd_const(bin) ( \
    (((bin) / 10) << 4) + (bin) % 10 \
)

/*
 * Interface for known dynamic arguments
 */

extern const bfdev_u8
bfdev_bcd2bin_table[256];

extern const bfdev_u8
bfdev_bin2bcd_table[256];

static __bfdev_attribute_const __bfdev_always_inline
bfdev_u8 bfdev_bcd2bin_dynamic(bfdev_u8 bcd)
{
    return bfdev_bcd2bin_table[bcd];
}

static __bfdev_attribute_const __bfdev_always_inline
bfdev_u8 bfdev_bin2bcd_dynamic(bfdev_u8 bin)
{
    return bfdev_bin2bcd_table[bin];
}

/**
 * bfdev_bcd2bin - convert bcd to bin.
 * @bcd: the bcd to convert.
 */
#define bfdev_bcd2bin(bcd) ({         \
    bfdev_u8 __bcd = (bfdev_u8)(bcd); \
    __builtin_constant_p(__bcd)       \
    ? bfdev_bcd2bin_const(__bcd)      \
    : bfdev_bcd2bin_dynamic(__bcd);   \
})

/**
 * bfdev_bin2bcd - convert bin to bcd.
 * @bin: the bin to convert.
 */
#define bfdev_bin2bcd(bin) ({         \
    bfdev_u8 __bin = (bfdev_u8)(bin); \
    __builtin_constant_p(__bin)       \
    ? bfdev_bin2bcd_const(__bin)      \
    : bfdev_bin2bcd_dynamic(__bin);   \
})

BFDEV_END_DECLS

#endif /* _BFDEV_BCD_H_ */
