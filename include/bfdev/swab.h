/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_SWAB_H_
#define _BFDEV_SWAB_H_

#include <bfdev/config.h>
#include <bfdev/stdint.h>
#include <bfdev/asm/swab.h>

BFDEV_BEGIN_DECLS

/*
 * casts are necessary for constants, because we never know how for sure
 * how U/UL/ULL map to uint16_t, uint32_t, uint64_t. At least not in a portable way.
 */

#define bfdev_swab16_const(x) ((uint16_t)(          \
    (((uint16_t)(x) & (uint16_t)0x00ffU) << 8) |    \
    (((uint16_t)(x) & (uint16_t)0xff00U) >> 8)      \
))

#define bfdev_swab32_const(x) ((uint32_t)(              \
    (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) |  \
    (((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) |  \
    (((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) |  \
    (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)    \
))

#define bfdev_swab64_const(x) ((uint64_t)(                      \
    (((uint64_t)(x) & (uint64_t)0x00000000000000ffULL) << 56) | \
    (((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 40) | \
    (((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 24) | \
    (((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) <<  8) | \
    (((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >>  8) | \
    (((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 24) | \
    (((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 40) | \
    (((uint64_t)(x) & (uint64_t)0xff00000000000000ULL) >> 56)   \
))

#define bfdev_swahb32_const(x) ((uint32_t)(             \
    (((uint32_t)(x) & (uint32_t)0x00ff00ffUL) << 8) |   \
    (((uint32_t)(x) & (uint32_t)0xff00ff00UL) >> 8)     \
))

#define bfdev_swahw32_const(x) ((uint32_t)(             \
    (((uint32_t)(x) & (uint32_t)0x0000ffffUL) << 16) |  \
    (((uint32_t)(x) & (uint32_t)0xffff0000UL) >> 16)    \
))

#define bfdev_swahb64_const(x) ((uint64_t)(                     \
    (((uint64_t)(x) & (uint64_t)0x00ff00ff00ff00ffULL) << 8) |  \
    (((uint64_t)(x) & (uint64_t)0xff00ff00ff00ff00ULL) >> 8)    \
))

#define bfdev_swahw64_const(x) ((uint64_t)(                     \
    (((uint64_t)(x) & (uint64_t)0x0000ffff0000ffffULL) << 16) | \
    (((uint64_t)(x) & (uint64_t)0xffff0000ffff0000ULL) >> 16)   \
))

#define bfdev_swahl64_const(x) ((uint64_t)(                     \
    (((uint64_t)(x) & (uint64_t)0x00000000ffffffffULL) << 32) | \
    (((uint64_t)(x) & (uint64_t)0xffffffff00000000ULL) >> 32)   \
))

/*
 * Implement the following as inlines, but define the interface using
 * macros to allow constant folding when possible:
 * _swab16, _swab32, _swab64, ___swahw32, _swahb32
 */

static inline __bfdev_attribute_const
uint16_t bfdev_fswab16(uint16_t val)
{
#ifdef bfdev_arch_swab16
    return bfdev_arch_swab16(val);
#else
    return bfdev_swab16_const(val);
#endif
}

static inline __bfdev_attribute_const
uint32_t bfdev_fswab32(uint32_t val)
{
#ifdef bfdev_arch_swab32
    return bfdev_arch_swab32(val);
#else
    return bfdev_swab32_const(val);
#endif
}

static inline __bfdev_attribute_const
uint64_t bfdev_fswab64(uint64_t val)
{
#ifdef bfdev_arch_swab64
    return bfdev_arch_swab64(val);
#else
    return bfdev_swab64_const(val);
#endif
}

static inline __bfdev_attribute_const
uint32_t bfdev_fswahb32(uint32_t val)
{
#ifdef bfdev_arch_swahb32
    return bfdev_arch_swahb32(val);
#else
    return bfdev_swahb32_const(val);
#endif
}

static inline __bfdev_attribute_const
uint32_t bfdev_fswahw32(uint32_t val)
{
#ifdef bfdev_arch_swahw32
    return bfdev_arch_swahw32(val);
#else
    return bfdev_swahw32_const(val);
#endif
}

static inline __bfdev_attribute_const
uint64_t bfdev_fswahb64(uint64_t val)
{
#ifdef bfdev_arch_swahb64
    return bfdev_arch_swahb64(val);
#else
    return bfdev_swahb64_const(val);
#endif
}

static inline __bfdev_attribute_const
uint64_t bfdev_fswahw64(uint64_t val)
{
#ifdef bfdev_arch_swahw64
    return bfdev_arch_swahw64(val);
#else
    return bfdev_swahw64_const(val);
#endif
}

static inline __bfdev_attribute_const
uint64_t bfdev_fswahl64(uint64_t val)
{
#ifdef bfdev_arch_swahl64
    return bfdev_arch_swahl64(val);
#else
    return bfdev_swahl64_const(val);
#endif
}

/**
 * swab16 - return a byteswapped 16-bit value.
 * @x: value to byteswap.
 */
#ifdef __HAVE_BUILTIN_BSWAP16__
# define bfdev_swab16(x) (uint16_t)__builtin_bswap16((uint16_t)(x))
#else
# define bfdev_swab16(x) (                  \
    __builtin_constant_p((uint16_t)(x)) ?   \
    bfdev_swab16_const(x) :                 \
    bfdev_fswab16(x)                        \
)
#endif

/**
 * swab32 - return a byteswapped 32-bit value.
 * @x: value to byteswap.
 */
#ifdef __HAVE_BUILTIN_BSWAP32__
# define bfdev_swab32(x) (uint32_t)__builtin_bswap32((uint32_t)(x))
#else
# define bfdev_swab32(x) (                  \
    __builtin_constant_p((uint32_t)(x)) ?   \
    bfdev_swab32_const(x) :                 \
    bfdev_fswab32(x)                        \
)
#endif

/**
 * swab64 - return a byteswapped 64-bit value.
 * @x: value to byteswap.
 */
#ifdef __HAVE_BUILTIN_BSWAP64__
# define bfdev_swab64(x) (uint64_t)__builtin_bswap64((uint64_t)(x))
#else
# define bfdev_swab64(x) (                  \
    __builtin_constant_p((uint64_t)(x)) ?   \
    bfdev_swab64_const(x) :                 \
    bfdev_fswab64(x)                        \
)
#endif

/**
 * swahb32 - return a high and low byte-swapped 32-bit value.
 * @x: value to byteswap.
 */
#define bfdev_swahb32(x) (                  \
    __builtin_constant_p((uint32_t)(x)) ?   \
    bfdev_swahb32_const(x) :                \
    bfdev_fswahb32(x)                       \
)

/**
 * swahw32 - return a word-swapped 32-bit value.
 * @x: value to wordswap.
 */
#define bfdev_swahw32(x) (                  \
    __builtin_constant_p((uint32_t)(x)) ?   \
    bfdev_swahw32_const(x) :                \
    bfdev_fswahw32(x)                       \
)

/**
 * swahb64 - return a high and low byte-swapped 64-bit value.
 * @x: value to byteswap.
 */
#define bfdev_swahb64(x) (                  \
    __builtin_constant_p((uint64_t)(x)) ?   \
    bfdev_swahb64_const(x) :                \
    bfdev_fswahb64(x)                       \
)

/**
 * swahw64 - return a word-swapped 64-bit value.
 * @x: value to wordswap.
 */
#define bfdev_swahw64(x) (                  \
    __builtin_constant_p((uint64_t)(x)) ?   \
    bfdev_swahw64_const(x) :                \
    bfdev_fswahw64(x)                       \
)

/**
 * swahl64 - return a long-swapped 64-bit value.
 * @x: value to wordswap.
 */
#define bfdev_swahl64(x) (                  \
    __builtin_constant_p((uint64_t)(x)) ?   \
    bfdev_swahl64_const(x) :                \
    bfdev_fswahl64(x)                       \
)

/**
 * swab16p - return a byteswapped 16-bit value from a pointer.
 * @p: pointer to a naturally-aligned 16-bit value.
 */
static __bfdev_always_inline uint16_t
bfdev_swab16p(const uint16_t *p)
{
#ifdef bfdev_arch_swab16p
    return bfdev_arch_swab16p(p);
#else
    return bfdev_swab16(*p);
#endif
}

/**
 * swab32p - return a byteswapped 32-bit value from a pointer.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static __bfdev_always_inline uint32_t
bfdev_swab32p(const uint32_t *p)
{
#ifdef bfdev_arch_swab32p
    return bfdev_arch_swab32p(p);
#else
    return bfdev_swab32(*p);
#endif
}

/**
 * swab64p - return a byteswapped 64-bit value from a pointer.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static __bfdev_always_inline uint64_t
bfdev_swab64p(const uint64_t *p)
{
#ifdef bfdev_arch_swab64p
    return bfdev_arch_swab64p(p);
#else
    return bfdev_swab64(*p);
#endif
}

/**
 * swahb32p - return a high and low byteswapped 32-bit value from a pointer.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static inline uint32_t
bfdev_swahb32p(const uint32_t *p)
{
#ifdef bfdev_arch_swahb32p
    return bfdev_arch_swahb32p(p);
#else
    return bfdev_swahb32(*p);
#endif
}

/**
 * swahw32p - return a wordswapped 32-bit value from a pointer.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static inline uint32_t
bfdev_swahw32p(const uint32_t *p)
{
#ifdef bfdev_arch_swahw32p
    return bfdev_arch_swahw32p(p);
#else
    return bfdev_swahw32(*p);
#endif
}

/**
 * swahb64p - return a high and low byteswapped 64-bit value from a pointer.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline uint64_t
bfdev_swahb64p(const uint64_t *p)
{
#ifdef bfdev_arch_swahb64p
    return bfdev_arch_swahb64p(p);
#else
    return bfdev_swahb64(*p);
#endif
}

/**
 * swahw64p - return a wordswapped 64-bit value from a pointer.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline uint64_t
bfdev_swahw64p(const uint64_t *p)
{
#ifdef bfdev_arch_swahw64p
    return bfdev_arch_swahw64p(p);
#else
    return bfdev_swahw64(*p);
#endif
}

/**
 * swahl64p - return a longswapped 64-bit value from a pointer.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline uint64_t
bfdev_swahl64p(const uint64_t *p)
{
#ifdef bfdev_arch_swahl64p
    return bfdev_arch_swahl64p(p);
#else
    return bfdev_swahl64(*p);
#endif
}

/**
 * swab16s - byteswap a 16-bit value in-place.
 * @p: pointer to a naturally-aligned 16-bit value.
 */
static inline void
bfdev_swab16s(uint16_t *p)
{
#ifdef bfdev_arch_swab16s
    bfdev_arch_swab16s(p);
#else
    *p = bfdev_swab16p(p);
#endif
}

/**
 * swab32s - byteswap a 32-bit value in-place.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static __bfdev_always_inline void
bfdev_swab32s(uint32_t *p)
{
#ifdef bfdev_arch_swab32s
    bfdev_arch_swab32s(p);
#else
    *p = bfdev_swab32p(p);
#endif
}

/**
 * swab64s - byteswap a 64-bit value in-place.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static __bfdev_always_inline void
bfdev_swab64s(uint64_t *p)
{
#ifdef bfdev_arch_swab64s
    bfdev_arch_swab64s(p);
#else
    *p = bfdev_swab64p(p);
#endif
}

/**
 * swahb32s - high and low byteswap a 32-bit value in-place.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static inline void
bfdev_swahb32s(uint32_t *p)
{
#ifdef bfdev_arch_swahb32s
    bfdev_arch_swahb32s(p);
#else
    *p = bfdev_swahb32p(p);
#endif
}

/**
 * swahw32s - wordswap a 32-bit value in-place.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static inline void
bfdev_swahw32s(uint32_t *p)
{
#ifdef bfdev_arch_swahw32s
    bfdev_arch_swahw32s(p);
#else
    *p = bfdev_swahw32p(p);
#endif
}

/**
 * swahb64s - high and low byteswap a 64-bit value in-place.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline void
bfdev_swahb64s(uint64_t *p)
{
#ifdef bfdev_arch_swahb64s
    bfdev_arch_swahb64s(p);
#else
    *p = bfdev_swahb64p(p);
#endif
}

/**
 * swahw64s - wordswap a 64-bit value in-place.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline void
bfdev_swahw64s(uint64_t *p)
{
#ifdef bfdev_arch_swahw64s
    bfdev_arch_swahw64s(p);
#else
    *p = bfdev_swahw64p(p);
#endif
}

/**
 * swahw64s - longswap a 64-bit value in-place.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline void
bfdev_swahl64s(uint64_t *p)
{
#ifdef bfdev_arch_swahw64s
    bfdev_arch_swahw64s(p);
#else
    *p = bfdev_swahl64p(p);
#endif
}

static inline void
bfdev_swab16_array(uint16_t *buff, unsigned int count)
{
    while (count--) {
        bfdev_swab16s(buff);
        buff++;
    }
}

static inline void
bfdev_swab32_array(uint32_t *buff, unsigned int count)
{
    while (count--) {
        bfdev_swab32s(buff);
        buff++;
    }
}

static inline void
bfdev_swab64_array(uint64_t *buff, unsigned int count)
{
    while (count--) {
        bfdev_swab64s(buff);
        buff++;
    }
}

BFDEV_END_DECLS

#endif /* _BFDEV_SWAB_H_ */
