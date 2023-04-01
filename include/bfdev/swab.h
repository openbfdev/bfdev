/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_SWAB_H_
#define _BFDEV_SWAB_H_

#include <bfdev/stdint.h>
#include <bfdev/attributes.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * casts are necessary for constants, because we never know how for sure
 * how U/UL/ULL map to uint16_t, uint32_t, uint64_t. At least not in a portable way.
 */

#define swab16_constant(x) ((uint16_t)(                             \
    (((uint16_t)(x) & (uint16_t)0x00ffU) << 8) |                    \
    (((uint16_t)(x) & (uint16_t)0xff00U) >> 8)                      \
))

#define swab32_constant(x) ((uint32_t)(                             \
    (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) |              \
    (((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) |              \
    (((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) |              \
    (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)                \
))

#define swab64_constant(x) ((uint64_t)(                             \
    (((uint64_t)(x) & (uint64_t)0x00000000000000ffULL) << 56) |     \
    (((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 40) |     \
    (((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 24) |     \
    (((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) <<  8) |     \
    (((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >>  8) |     \
    (((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 24) |     \
    (((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 40) |     \
    (((uint64_t)(x) & (uint64_t)0xff00000000000000ULL) >> 56)       \
))

#define swahb32_constant(x) ((uint32_t)(                            \
    (((uint32_t)(x) & (uint32_t)0x00ff00ffUL) << 8) |               \
    (((uint32_t)(x) & (uint32_t)0xff00ff00UL) >> 8)                 \
))

#define swahw32_constant(x) ((uint32_t)(                            \
    (((uint32_t)(x) & (uint32_t)0x0000ffffUL) << 16) |              \
    (((uint32_t)(x) & (uint32_t)0xffff0000UL) >> 16)                \
))

#define swahb64_constant(x) ((uint64_t)(                            \
    (((uint64_t)(x) & (uint64_t)0x00ff00ff00ff00ffULL) << 8) |      \
    (((uint64_t)(x) & (uint64_t)0xff00ff00ff00ff00ULL) >> 8)        \
))

#define swahw64_constant(x) ((uint64_t)(                            \
    (((uint64_t)(x) & (uint64_t)0x0000ffff0000ffffULL) << 16) |     \
    (((uint64_t)(x) & (uint64_t)0xffff0000ffff0000ULL) >> 16)       \
))

#define swahl64_constant(x) ((uint64_t)(                            \
    (((uint64_t)(x) & (uint64_t)0x00000000ffffffffULL) << 32) |     \
    (((uint64_t)(x) & (uint64_t)0xffffffff00000000ULL) >> 32)       \
))

/*
 * Implement the following as inlines, but define the interface using
 * macros to allow constant folding when possible:
 * _swab16, _swab32, _swab64, ___swahw32, _swahb32
 */

static inline __attribute_const__ uint16_t fswab16(uint16_t val)
{
#ifdef arch_swab16
    return arch_swab16(val);
#else
    return swab16_constant(val);
#endif
}

static inline __attribute_const__ uint32_t fswab32(uint32_t val)
{
#ifdef arch_swab32
    return arch_swab32(val);
#else
    return swab32_constant(val);
#endif
}

static inline __attribute_const__ uint64_t fswab64(uint64_t val)
{
#ifdef arch_swab64
    return arch_swab64(val);
#else
    return swab64_constant(val);
#endif
}

static inline __attribute_const__ uint32_t fswahb32(uint32_t val)
{
#ifdef arch_swahb32
    return arch_swahb32(val);
#else
    return swahb32_constant(val);
#endif
}

static inline __attribute_const__ uint32_t fswahw32(uint32_t val)
{
#ifdef arch_swahw32
    return arch_swahw32(val);
#else
    return swahw32_constant(val);
#endif
}

static inline __attribute_const__ uint64_t fswahb64(uint64_t val)
{
#ifdef arch_swahb64
    return arch_swahb64(val);
#else
    return swahb64_constant(val);
#endif
}

static inline __attribute_const__ uint64_t fswahw64(uint64_t val)
{
#ifdef arch_swahw64
    return arch_swahw64(val);
#else
    return swahw64_constant(val);
#endif
}

static inline __attribute_const__ uint64_t fswahl64(uint64_t val)
{
#ifdef arch_swahl64
    return arch_swahl64(val);
#else
    return swahl64_constant(val);
#endif
}

/**
 * swab16 - return a byteswapped 16-bit value.
 * @x: value to byteswap.
 */
#ifdef __HAVE_BUILTIN_BSWAP16__
# define swab16(x) (uint16_t)__builtin_bswap16((uint16_t)(x))
#else
# define swab16(x) (                        \
    __builtin_constant_p((uint16_t)(x)) ?   \
    swab16_constant(x) :                    \
    fswab16(x)                              \
)
#endif

/**
 * swab32 - return a byteswapped 32-bit value.
 * @x: value to byteswap.
 */
#ifdef __HAVE_BUILTIN_BSWAP32__
# define swab32(x) (uint32_t)__builtin_bswap32((uint32_t)(x))
#else
# define swab32(x) (                        \
    __builtin_constant_p((uint32_t)(x)) ?   \
    swab32_constant(x) :                    \
    fswab32(x)                              \
)
#endif

/**
 * swab64 - return a byteswapped 64-bit value.
 * @x: value to byteswap.
 */
#ifdef __HAVE_BUILTIN_BSWAP64__
# define swab64(x) (uint64_t)__builtin_bswap64((uint64_t)(x))
#else
# define swab64(x) (                        \
    __builtin_constant_p((uint64_t)(x)) ?   \
    swab64_constant(x) :                    \
    fswab64(x)                              \
)
#endif

/**
 * swahb32 - return a high and low byte-swapped 32-bit value.
 * @x: value to byteswap.
 */
#define swahb32(x) (                        \
    __builtin_constant_p((uint32_t)(x)) ?   \
    swahb32_constant(x) :                   \
    fswahb32(x)                             \
)

/**
 * swahw32 - return a word-swapped 32-bit value.
 * @x: value to wordswap.
 */
#define swahw32(x) (                        \
    __builtin_constant_p((uint32_t)(x)) ?   \
    swahw32_constant(x) :                   \
    fswahw32(x)                             \
)

/**
 * swahb64 - return a high and low byte-swapped 64-bit value.
 * @x: value to byteswap.
 */
#define swahb64(x) (                        \
    __builtin_constant_p((uint64_t)(x)) ?   \
    swahb64_constant(x) :                   \
    fswahb64(x)                             \
)

/**
 * swahw64 - return a word-swapped 64-bit value.
 * @x: value to wordswap.
 */
#define swahw64(x) (                        \
    __builtin_constant_p((uint64_t)(x)) ?   \
    swahw64_constant(x) :                   \
    fswahw64(x)                             \
)

/**
 * swahl64 - return a long-swapped 64-bit value.
 * @x: value to wordswap.
 */
#define swahl64(x) (                        \
    __builtin_constant_p((uint64_t)(x)) ?   \
    swahl64_constant(x) :                   \
    fswahl64(x)                             \
)

/**
 * swab16p - return a byteswapped 16-bit value from a pointer.
 * @p: pointer to a naturally-aligned 16-bit value.
 */
static __always_inline uint16_t swab16p(const uint16_t *p)
{
#ifdef arch_swab16p
    return arch_swab16p(p);
#else
    return swab16(*p);
#endif
}

/**
 * swab32p - return a byteswapped 32-bit value from a pointer.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static __always_inline uint32_t swab32p(const uint32_t *p)
{
#ifdef arch_swab32p
    return arch_swab32p(p);
#else
    return swab32(*p);
#endif
}

/**
 * swab64p - return a byteswapped 64-bit value from a pointer.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static __always_inline uint64_t swab64p(const uint64_t *p)
{
#ifdef arch_swab64p
    return arch_swab64p(p);
#else
    return swab64(*p);
#endif
}

/**
 * swahb32p - return a high and low byteswapped 32-bit value from a pointer.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static inline uint32_t swahb32p(const uint32_t *p)
{
#ifdef arch_swahb32p
    return arch_swahb32p(p);
#else
    return swahb32(*p);
#endif
}

/**
 * swahw32p - return a wordswapped 32-bit value from a pointer.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static inline uint32_t swahw32p(const uint32_t *p)
{
#ifdef arch_swahw32p
    return arch_swahw32p(p);
#else
    return swahw32(*p);
#endif
}

/**
 * swahb64p - return a high and low byteswapped 64-bit value from a pointer.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline uint64_t swahb64p(const uint64_t *p)
{
#ifdef arch_swahb64p
    return arch_swahb64p(p);
#else
    return swahb64(*p);
#endif
}

/**
 * swahw64p - return a wordswapped 64-bit value from a pointer.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline uint64_t swahw64p(const uint64_t *p)
{
#ifdef arch_swahw64p
    return arch_swahw64p(p);
#else
    return swahw64(*p);
#endif
}

/**
 * swahl64p - return a longswapped 64-bit value from a pointer.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline uint64_t swahl64p(const uint64_t *p)
{
#ifdef arch_swahl64p
    return arch_swahl64p(p);
#else
    return swahl64(*p);
#endif
}

/**
 * swab16s - byteswap a 16-bit value in-place.
 * @p: pointer to a naturally-aligned 16-bit value.
 */
static inline void swab16s(uint16_t *p)
{
#ifdef arch_swab16s
    arch_swab16s(p);
#else
    *p = swab16p(p);
#endif
}

/**
 * swab32s - byteswap a 32-bit value in-place.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static __always_inline void swab32s(uint32_t *p)
{
#ifdef arch_swab32s
    arch_swab32s(p);
#else
    *p = swab32p(p);
#endif
}

/**
 * swab64s - byteswap a 64-bit value in-place.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static __always_inline void swab64s(uint64_t *p)
{
#ifdef arch_swab64s
    arch_swab64s(p);
#else
    *p = swab64p(p);
#endif
}

/**
 * swahb32s - high and low byteswap a 32-bit value in-place.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static inline void swahb32s(uint32_t *p)
{
#ifdef arch_swahb32s
    arch_swahb32s(p);
#else
    *p = swahb32p(p);
#endif
}

/**
 * swahw32s - wordswap a 32-bit value in-place.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static inline void swahw32s(uint32_t *p)
{
#ifdef arch_swahw32s
    arch_swahw32s(p);
#else
    *p = swahw32p(p);
#endif
}

/**
 * swahb64s - high and low byteswap a 64-bit value in-place.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline void swahb64s(uint64_t *p)
{
#ifdef arch_swahb64s
    arch_swahb64s(p);
#else
    *p = swahb64p(p);
#endif
}

/**
 * swahw64s - wordswap a 64-bit value in-place.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline void swahw64s(uint64_t *p)
{
#ifdef arch_swahw64s
    arch_swahw64s(p);
#else
    *p = swahw64p(p);
#endif
}

/**
 * swahw64s - longswap a 64-bit value in-place.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline void swahl64s(uint64_t *p)
{
#ifdef arch_swahw64s
    arch_swahw64s(p);
#else
    *p = swahl64p(p);
#endif
}

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_SWAB_H_ */
