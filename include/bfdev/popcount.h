/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_POPCOUNT_H_
#define _BFDEV_POPCOUNT_H_

#include <bfdev/config.h>
#include <bfdev/stdint.h>
#include <bfdev/stdbool.h>

BFDEV_BEGIN_DECLS

extern const uint8_t bfdev_popcount_table[256];
extern const uint8_t bfdev_popparity_table[256];

/*
 * Interface for known constant arguments
 */

static __bfdev_attribute_const __bfdev_always_inline
unsigned int bfdev_popcount8_const(uint8_t value)
{
    value = (value & 0x55) + ((value >> 1) & 0x55);
    value = (value & 0x33) + ((value >> 2) & 0x33);
    value = (value & 0x0f) + ((value >> 4) & 0x0f);
    return (unsigned int)value;
}

static __bfdev_attribute_const __bfdev_always_inline
unsigned int bfdev_popcount16_const(uint16_t value)
{
    value = (value & 0x5555) + ((value >> 1) & 0x5555);
    value = (value & 0x3333) + ((value >> 2) & 0x3333);
    value = (value & 0x0f0f) + ((value >> 4) & 0x0f0f);
    value = (value & 0x00ff) + ((value >> 8) & 0x00ff);
    return (unsigned int)value;
}

static __bfdev_attribute_const __bfdev_always_inline
unsigned int bfdev_popcount32_const(uint32_t value)
{
    value = (value & 0x55555555UL) + ((value >>  1) & 0x55555555);
    value = (value & 0x33333333UL) + ((value >>  2) & 0x33333333);
    value = (value & 0x0f0f0f0fUL) + ((value >>  4) & 0x0f0f0f0f);
    value = (value & 0x00ff00ffUL) + ((value >>  8) & 0x00ff00ff);
    value = (value & 0x0000ffffUL) + ((value >> 16) & 0x0000ffff);
    return (unsigned int)value;
}

static __bfdev_attribute_const __bfdev_always_inline
unsigned int bfdev_popcount64_const(uint64_t value)
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
bool bfdev_popparity8_const(uint8_t value)
{
    value ^= value >> 4;
    value &= 0xf;
    return (0x6996 >> value) & 1;
}

static __bfdev_attribute_const __bfdev_always_inline
bool bfdev_popparity16_const(uint16_t value)
{
    value ^= value >> 8;
    value ^= value >> 4;
    value &= 0xf;
    return (0x6996 >> value) & 1;
}

static __bfdev_attribute_const __bfdev_always_inline
bool bfdev_popparity32_const(uint32_t value)
{
    value ^= value >> 16;
    value ^= value >> 8;
    value ^= value >> 4;
    value &= 0xf;
    return (0x6996 >> value) & 1;
}

static __bfdev_attribute_const __bfdev_always_inline
bool bfdev_popparity64_const(uint64_t value)
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

static inline __bfdev_attribute_const
unsigned int bfdev_popcount8_dynamic(uint8_t value)
{
    return bfdev_popcount_table[value];
}

static inline __bfdev_attribute_const
unsigned int bfdev_popcount16_dynamic(uint16_t value)
{
    uint8_t high = value >> 8;
    return bfdev_popcount8_dynamic(high) +
           bfdev_popcount8_dynamic((uint8_t)value);
}

static inline __bfdev_attribute_const
unsigned int bfdev_popcount32_dynamic(uint32_t value)
{
    uint16_t high = value >> 16;
    return bfdev_popcount16_dynamic(high) +
           bfdev_popcount16_dynamic((uint16_t)value);
}

static inline __bfdev_attribute_const
unsigned int bfdev_popcount64_dynamic(uint64_t value)
{
    uint32_t high = value >> 32;
    return bfdev_popcount32_dynamic(high) +
           bfdev_popcount32_dynamic((uint32_t)value);
}

static inline __bfdev_attribute_const
bool bfdev_popparity8_dynamic(uint8_t value)
{
    return bfdev_popparity_table[value];
}

static inline __bfdev_attribute_const
bool bfdev_popparity16_dynamic(uint16_t value)
{
    uint8_t high = value >> 8;
    return (bfdev_popparity8_dynamic((uint8_t)value) +
            bfdev_popparity8_dynamic(high)) & 1;
}

static inline __bfdev_attribute_const
bool bfdev_popparity32_dynamic(uint32_t value)
{
    uint16_t high = value >> 16;
    return (bfdev_popparity16_dynamic((uint16_t)value) +
            bfdev_popparity16_dynamic(high)) & 1;
}

static inline __bfdev_attribute_const
bool bfdev_popparity64_dynamic(uint64_t value)
{
    uint32_t high = value >> 32;
    return (bfdev_popparity32_dynamic((uint32_t)value) +
            bfdev_popparity32_dynamic(high)) & 1;
}

/**
 * bfdev_popcount() - count the total number of bits set.
 * @value: value to count.
 *
 * Returns the number of 1-bits in x.
 */
#define bfdev_popcount8(value) ({           \
    uint8_t __value = (value);              \
    __builtin_constant_p(__value)           \
    ? bfdev_popcount8_const(__value)        \
    : bfdev_popcount8_dynamic(__value);     \
})

#define bfdev_popcount16(value) ({          \
    uint16_t __value = (value);             \
    __builtin_constant_p(__value)           \
    ? bfdev_popcount16_const(__value)       \
    : bfdev_popcount16_dynamic(__value);    \
})

#define bfdev_popcount32(value) ({          \
    uint32_t __value = (value);             \
    __builtin_constant_p(__value)           \
    ? bfdev_popcount32_const(__value)       \
    : bfdev_popcount32_dynamic(__value);    \
})

#define bfdev_popcount64(value) ({          \
    uint64_t __value = (value);             \
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
    uint8_t __value = (value);              \
    __builtin_constant_p(__value)           \
    ? bfdev_popparity8_const(__value)       \
    : bfdev_popparity8_dynamic(__value);    \
})

#define bfdev_popparity16(value) ({         \
    uint16_t __value = (value);             \
    __builtin_constant_p(__value)           \
    ? bfdev_popparity16_const(__value)      \
    : bfdev_popparity16_dynamic(__value);   \
})

#define bfdev_popparity32(value) ({         \
    uint32_t __value = (value);             \
    __builtin_constant_p(__value)           \
    ? bfdev_popparity32_const(__value)      \
    : bfdev_popparity32_dynamic(__value);   \
})

#define bfdev_popparity64(value) ({         \
    uint64_t __value = (value);             \
    __builtin_constant_p(__value)           \
    ? bfdev_popparity64_const(__value)      \
    : bfdev_popparity64_dynamic(__value);   \
})

BFDEV_END_DECLS

#endif /* _BFDEV_POPCOUNT_H_ */
