/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BITREV_H_
#define _BFDEV_BITREV_H_

#include <bfdev/config.h>
#include <bfdev/types.h>

BFDEV_BEGIN_DECLS

/*
 * Interface for known constant arguments
 */

#define bfdev_bitrev8_const(value) ( \
    (((bfdev_u8)(value) & (bfdev_u8)0x01UL) << 7) | \
    (((bfdev_u8)(value) & (bfdev_u8)0x02UL) << 5) | \
    (((bfdev_u8)(value) & (bfdev_u8)0x04UL) << 3) | \
    (((bfdev_u8)(value) & (bfdev_u8)0x08UL) << 1) | \
    (((bfdev_u8)(value) & (bfdev_u8)0x10UL) >> 1) | \
    (((bfdev_u8)(value) & (bfdev_u8)0x20UL) >> 3) | \
    (((bfdev_u8)(value) & (bfdev_u8)0x40UL) >> 5) | \
    (((bfdev_u8)(value) & (bfdev_u8)0x80UL) >> 7))

#define bfdev_bitrev16_const(value) ( \
    (((bfdev_u16)(value) & (bfdev_u16)0x0001UL) << 15) | \
    (((bfdev_u16)(value) & (bfdev_u16)0x0002UL) << 13) | \
    (((bfdev_u16)(value) & (bfdev_u16)0x0004UL) << 11) | \
    (((bfdev_u16)(value) & (bfdev_u16)0x0008UL) <<  9) | \
    (((bfdev_u16)(value) & (bfdev_u16)0x0010UL) <<  7) | \
    (((bfdev_u16)(value) & (bfdev_u16)0x0020UL) <<  5) | \
    (((bfdev_u16)(value) & (bfdev_u16)0x0040UL) <<  3) | \
    (((bfdev_u16)(value) & (bfdev_u16)0x0080UL) <<  1) | \
    (((bfdev_u16)(value) & (bfdev_u16)0x0100UL) >>  1) | \
    (((bfdev_u16)(value) & (bfdev_u16)0x0200UL) >>  3) | \
    (((bfdev_u16)(value) & (bfdev_u16)0x0400UL) >>  5) | \
    (((bfdev_u16)(value) & (bfdev_u16)0x0800UL) >>  7) | \
    (((bfdev_u16)(value) & (bfdev_u16)0x1000UL) >>  9) | \
    (((bfdev_u16)(value) & (bfdev_u16)0x2000UL) >> 11) | \
    (((bfdev_u16)(value) & (bfdev_u16)0x4000UL) >> 13) | \
    (((bfdev_u16)(value) & (bfdev_u16)0x8000UL) >> 15))

#define bfdev_bitrev32_const(value) ( \
    (((bfdev_u32)(value) & (bfdev_u32)0x00000001UL) << 31) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00000002UL) << 29) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00000004UL) << 27) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00000008UL) << 25) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00000010UL) << 23) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00000020UL) << 21) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00000040UL) << 19) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00000080UL) << 17) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00000100UL) << 15) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00000200UL) << 13) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00000400UL) << 11) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00000800UL) <<  9) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00001000UL) <<  7) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00002000UL) <<  5) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00004000UL) <<  3) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00008000UL) <<  1) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00010000UL) >>  1) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00020000UL) >>  3) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00040000UL) >>  5) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00080000UL) >>  7) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00100000UL) >>  9) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00200000UL) >> 11) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00400000UL) >> 13) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x00800000UL) >> 15) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x01000000UL) >> 17) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x02000000UL) >> 19) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x04000000UL) >> 21) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x08000000UL) >> 25) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x10000000UL) >> 23) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x20000000UL) >> 27) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x40000000UL) >> 29) | \
    (((bfdev_u32)(value) & (bfdev_u32)0x80000000UL) >> 31))

#define bfdev_bitrev64_const(value) ( \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000000001ULL) << 63) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000000002ULL) << 61) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000000004ULL) << 59) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000000008ULL) << 57) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000000010ULL) << 55) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000000020ULL) << 53) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000000040ULL) << 51) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000000080ULL) << 49) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000000100ULL) << 47) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000000200ULL) << 45) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000000400ULL) << 43) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000000800ULL) << 41) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000001000ULL) << 39) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000002000ULL) << 37) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000004000ULL) << 35) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000008000ULL) << 33) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000010000ULL) << 31) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000020000ULL) << 29) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000040000ULL) << 27) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000080000ULL) << 25) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000100000ULL) << 23) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000200000ULL) << 21) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000400000ULL) << 19) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000000800000ULL) << 17) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000001000000ULL) << 15) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000002000000ULL) << 13) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000004000000ULL) << 11) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000008000000ULL) <<  9) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000010000000ULL) <<  7) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000020000000ULL) <<  5) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000040000000ULL) <<  3) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000080000000ULL) <<  1) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000100000000ULL) >>  1) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000200000000ULL) >>  3) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000400000000ULL) >>  5) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000000800000000ULL) >>  7) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000001000000000ULL) >>  9) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000002000000000ULL) >> 11) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000004000000000ULL) >> 13) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000008000000000ULL) >> 15) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000010000000000ULL) >> 17) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000020000000000ULL) >> 19) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000040000000000ULL) >> 21) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000080000000000ULL) >> 25) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000100000000000ULL) >> 23) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000200000000000ULL) >> 27) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000400000000000ULL) >> 29) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0000800000000000ULL) >> 31) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0001000000000000ULL) >> 33) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0002000000000000ULL) >> 35) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0004000000000000ULL) >> 37) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0008000000000000ULL) >> 39) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0010000000000000ULL) >> 41) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0020000000000000ULL) >> 43) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0040000000000000ULL) >> 45) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0080000000000000ULL) >> 47) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0100000000000000ULL) >> 49) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0200000000000000ULL) >> 51) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0400000000000000ULL) >> 53) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x0800000000000000ULL) >> 55) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x1000000000000000ULL) >> 57) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x2000000000000000ULL) >> 59) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x4000000000000000ULL) >> 61) | \
    (((bfdev_u64)(value) & (bfdev_u64)0x8000000000000000ULL) >> 63))

/*
 * Interface for known dynamic arguments
 */

extern const bfdev_u8
bfdev_bitrev_byte_table[256];

static inline bfdev_u8
bfdev_bitrev8_dynamic(bfdev_u8 value)
{
    return bfdev_bitrev_byte_table[value];
}

static inline bfdev_u16
bfdev_bitrev16_dynamic(bfdev_u16 value)
{
    return ((bfdev_u16)bfdev_bitrev8_dynamic(value & 0xff) << 8) |
        bfdev_bitrev8_dynamic(value >> 8);
}

static inline bfdev_u32
bfdev_bitrev32_dynamic(bfdev_u32 value)
{
    return ((bfdev_u32)bfdev_bitrev16_dynamic(value & 0xffff) << 16) |
        bfdev_bitrev16_dynamic(value >> 16);
}

static inline bfdev_u64
bfdev_bitrev64_dynamic(bfdev_u64 value)
{
    return ((bfdev_u64)bfdev_bitrev32_dynamic(value & 0xffffffff) << 32) |
        bfdev_bitrev32_dynamic(value >> 32);
}

#define bfdev_bitrev8(value) ({           \
    bfdev_u8 __value = (bfdev_u8)(value); \
    __builtin_constant_p(__value)         \
    ? bfdev_bitrev8_const(__value)        \
    : bfdev_bitrev8_dynamic(__value);     \
})

#define bfdev_bitrev16(value) ({            \
    bfdev_u16 __value = (bfdev_u16)(value); \
    __builtin_constant_p(__value)           \
    ? bfdev_bitrev16_const(__value)         \
    : bfdev_bitrev16_dynamic(__value);      \
})

#define bfdev_bitrev32(value) ({            \
    bfdev_u32 __value = (bfdev_u32)(value); \
    __builtin_constant_p(__value)           \
    ? bfdev_bitrev32_const(__value)         \
    : bfdev_bitrev32_dynamic(__value);      \
})

#define bfdev_bitrev64(value) ({            \
    bfdev_u64 __value = (bfdev_u64)(value); \
    __builtin_constant_p(__value)           \
    ? bfdev_bitrev64_const(__value)         \
    : bfdev_bitrev64_dynamic(__value);      \
})

BFDEV_END_DECLS

#endif /* _BFDEV_BITREV_H_ */
