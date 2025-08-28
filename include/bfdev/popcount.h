/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_POPCOUNT_H_
#define _BFDEV_POPCOUNT_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

/*
 * Interface for known constant arguments
 */

static __bfdev_attribute_const __bfdev_always_inline
unsigned int bfdev_popcount8_const(bfdev_u8 value)
{
    value = (value & 0x55) + ((value >> 1) & 0x55);
    value = (value & 0x33) + ((value >> 2) & 0x33);
    value = (value & 0x0f) + ((value >> 4) & 0x0f);
    return (unsigned int)value;
}

static __bfdev_attribute_const __bfdev_always_inline
unsigned int bfdev_popcount16_const(bfdev_u16 value)
{
    value = (value & 0x5555) + ((value >> 1) & 0x5555);
    value = (value & 0x3333) + ((value >> 2) & 0x3333);
    value = (value & 0x0f0f) + ((value >> 4) & 0x0f0f);
    value = (value & 0x00ff) + ((value >> 8) & 0x00ff);
    return (unsigned int)value;
}

static __bfdev_attribute_const __bfdev_always_inline
unsigned int bfdev_popcount32_const(bfdev_u32 value)
{
    value = (value & 0x55555555UL) + ((value >>  1) & 0x55555555);
    value = (value & 0x33333333UL) + ((value >>  2) & 0x33333333);
    value = (value & 0x0f0f0f0fUL) + ((value >>  4) & 0x0f0f0f0f);
    value = (value & 0x00ff00ffUL) + ((value >>  8) & 0x00ff00ff);
    value = (value & 0x0000ffffUL) + ((value >> 16) & 0x0000ffff);
    return (unsigned int)value;
}

static __bfdev_attribute_const __bfdev_always_inline
unsigned int bfdev_popcount64_const(bfdev_u64 value)
{
    value = (value & 0x5555555555555555ULL) + ((value >>  1) & 0x5555555555555555ULL);
    value = (value & 0x3333333333333333ULL) + ((value >>  2) & 0x3333333333333333ULL);
    value = (value & 0x0f0f0f0f0f0f0f0fULL) + ((value >>  4) & 0x0f0f0f0f0f0f0f0fULL);
    value = (value & 0x00ff00ff00ff00ffULL) + ((value >>  8) & 0x00ff00ff00ff00ffULL);
    value = (value & 0x0000ffff0000ffffULL) + ((value >> 16) & 0x0000ffff0000ffffULL);
    value = (value & 0x00000000ffffffffULL) + ((value >> 32) & 0x00000000ffffffffULL);
    return (unsigned int)value;
}

static __bfdev_attribute_const __bfdev_always_inline
bfdev_bool bfdev_popparity8_const(bfdev_u8 value)
{
    value ^= value >> 4;
    value &= 0xf;
    return (0x6996 >> value) & 1;
}

static __bfdev_attribute_const __bfdev_always_inline
bfdev_bool bfdev_popparity16_const(bfdev_u16 value)
{
    value ^= value >> 8;
    value ^= value >> 4;
    value &= 0xf;
    return (0x6996 >> value) & 1;
}

static __bfdev_attribute_const __bfdev_always_inline
bfdev_bool bfdev_popparity32_const(bfdev_u32 value)
{
    value ^= value >> 16;
    value ^= value >> 8;
    value ^= value >> 4;
    value &= 0xf;
    return (0x6996 >> value) & 1;
}

static __bfdev_attribute_const __bfdev_always_inline
bfdev_bool bfdev_popparity64_const(bfdev_u64 value)
{
    value ^= value >> 32;
    value ^= value >> 16;
    value ^= value >> 8;
    value ^= value >> 4;
    value &= 0xf;
    return (0x6996 >> value) & 1;
}

/*
 * Interface for known dynamic arguments
 */

extern const bfdev_u8
bfdev_popcount_table[256];

extern const bfdev_bool
bfdev_popparity_table[256];

static inline __bfdev_attribute_const
unsigned int bfdev_popcount8_dynamic(bfdev_u8 value)
{
    return bfdev_popcount_table[value];
}

static inline __bfdev_attribute_const
unsigned int bfdev_popcount16_dynamic(bfdev_u16 value)
{
    bfdev_u8 high = value >> 8;
    return bfdev_popcount8_dynamic(high) +
           bfdev_popcount8_dynamic((bfdev_u8)value);
}

static inline __bfdev_attribute_const
unsigned int bfdev_popcount32_dynamic(bfdev_u32 value)
{
    bfdev_u16 high = value >> 16;
    return bfdev_popcount16_dynamic(high) +
           bfdev_popcount16_dynamic((bfdev_u16)value);
}

static inline __bfdev_attribute_const
unsigned int bfdev_popcount64_dynamic(bfdev_u64 value)
{
    bfdev_u32 high = value >> 32;
    return bfdev_popcount32_dynamic(high) +
           bfdev_popcount32_dynamic((bfdev_u32)value);
}

static inline __bfdev_attribute_const
bfdev_bool bfdev_popparity8_dynamic(bfdev_u8 value)
{
    return bfdev_popparity_table[value];
}

static inline __bfdev_attribute_const
bfdev_bool bfdev_popparity16_dynamic(bfdev_u16 value)
{
    bfdev_u8 high = value >> 8;
    return (bfdev_popparity8_dynamic((bfdev_u8)value) +
            bfdev_popparity8_dynamic(high)) & 1;
}

static inline __bfdev_attribute_const
bfdev_bool bfdev_popparity32_dynamic(bfdev_u32 value)
{
    bfdev_u16 high = value >> 16;
    return (bfdev_popparity16_dynamic((bfdev_u16)value) +
            bfdev_popparity16_dynamic(high)) & 1;
}

static inline __bfdev_attribute_const
bfdev_bool bfdev_popparity64_dynamic(bfdev_u64 value)
{
    bfdev_u32 high = value >> 32;
    return (bfdev_popparity32_dynamic((bfdev_u32)value) +
            bfdev_popparity32_dynamic(high)) & 1;
}

/**
 * bfdev_popcount() - count the total number of bits set.
 * @value: value to count.
 *
 * Returns the number of 1-bits in x.
 */
#define bfdev_popcount8(value) ({           \
    bfdev_u8 __value = (value);             \
    __builtin_constant_p(__value)           \
    ? bfdev_popcount8_const(__value)        \
    : bfdev_popcount8_dynamic(__value);     \
})

#define bfdev_popcount16(value) ({          \
    bfdev_u16 __value = (value);            \
    __builtin_constant_p(__value)           \
    ? bfdev_popcount16_const(__value)       \
    : bfdev_popcount16_dynamic(__value);    \
})

#define bfdev_popcount32(value) ({          \
    bfdev_u32 __value = (value);            \
    __builtin_constant_p(__value)           \
    ? bfdev_popcount32_const(__value)       \
    : bfdev_popcount32_dynamic(__value);    \
})

#define bfdev_popcount64(value) ({          \
    bfdev_u64 __value = (value);            \
    __builtin_constant_p(__value)           \
    ? bfdev_popcount64_const(__value)       \
    : bfdev_popcount64_dynamic(__value);    \
})

/**
 * bfdev_popparity() - count the parity of bits set.
 * @value: value to parity.
 *
 * Returns the parity of x, i.e. the number of 1-bits in x modulo 2.
 */
#define bfdev_popparity8(value) ({          \
    bfdev_u8 __value = (value);             \
    __builtin_constant_p(__value)           \
    ? bfdev_popparity8_const(__value)       \
    : bfdev_popparity8_dynamic(__value);    \
})

#define bfdev_popparity16(value) ({         \
    bfdev_u16 __value = (value);            \
    __builtin_constant_p(__value)           \
    ? bfdev_popparity16_const(__value)      \
    : bfdev_popparity16_dynamic(__value);   \
})

#define bfdev_popparity32(value) ({         \
    bfdev_u32 __value = (value);            \
    __builtin_constant_p(__value)           \
    ? bfdev_popparity32_const(__value)      \
    : bfdev_popparity32_dynamic(__value);   \
})

#define bfdev_popparity64(value) ({         \
    bfdev_u64 __value = (value);            \
    __builtin_constant_p(__value)           \
    ? bfdev_popparity64_const(__value)      \
    : bfdev_popparity64_dynamic(__value);   \
})

BFDEV_END_DECLS

#endif /* _BFDEV_POPCOUNT_H_ */
