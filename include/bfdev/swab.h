/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_SWAB_H_
#define _BFDEV_SWAB_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/asm/swab.h>

BFDEV_BEGIN_DECLS

/*
 * casts are necessary for constants, because we never know how for sure
 * how U/UL/ULL map to bfdev_u16, bfdev_u32, bfdev_u64. At least not in a portable way.
 */

#define bfdev_swab16_const(x) ((bfdev_u16)(         \
    (((bfdev_u16)(x) & (bfdev_u16)0x00ffU) << 8) |  \
    (((bfdev_u16)(x) & (bfdev_u16)0xff00U) >> 8)    \
))

#define bfdev_swab32_const(x) ((bfdev_u32)(                 \
    (((bfdev_u32)(x) & (bfdev_u32)0x000000ffUL) << 24) |    \
    (((bfdev_u32)(x) & (bfdev_u32)0x0000ff00UL) <<  8) |    \
    (((bfdev_u32)(x) & (bfdev_u32)0x00ff0000UL) >>  8) |    \
    (((bfdev_u32)(x) & (bfdev_u32)0xff000000UL) >> 24)      \
))

#define bfdev_swab64_const(x) ((bfdev_u64)(                         \
    (((bfdev_u64)(x) & (bfdev_u64)0x00000000000000ffULL) << 56) |   \
    (((bfdev_u64)(x) & (bfdev_u64)0x000000000000ff00ULL) << 40) |   \
    (((bfdev_u64)(x) & (bfdev_u64)0x0000000000ff0000ULL) << 24) |   \
    (((bfdev_u64)(x) & (bfdev_u64)0x00000000ff000000ULL) <<  8) |   \
    (((bfdev_u64)(x) & (bfdev_u64)0x000000ff00000000ULL) >>  8) |   \
    (((bfdev_u64)(x) & (bfdev_u64)0x0000ff0000000000ULL) >> 24) |   \
    (((bfdev_u64)(x) & (bfdev_u64)0x00ff000000000000ULL) >> 40) |   \
    (((bfdev_u64)(x) & (bfdev_u64)0xff00000000000000ULL) >> 56)     \
))

#define bfdev_swahb32_const(x) ((bfdev_u32)(            \
    (((bfdev_u32)(x) & (bfdev_u32)0x00ff00ffUL) << 8) | \
    (((bfdev_u32)(x) & (bfdev_u32)0xff00ff00UL) >> 8)   \
))

#define bfdev_swahw32_const(x) ((bfdev_u32)(                \
    (((bfdev_u32)(x) & (bfdev_u32)0x0000ffffUL) << 16) |    \
    (((bfdev_u32)(x) & (bfdev_u32)0xffff0000UL) >> 16)      \
))

#define bfdev_swahb64_const(x) ((bfdev_u64)(                        \
    (((bfdev_u64)(x) & (bfdev_u64)0x00ff00ff00ff00ffULL) << 8) |    \
    (((bfdev_u64)(x) & (bfdev_u64)0xff00ff00ff00ff00ULL) >> 8)      \
))

#define bfdev_swahw64_const(x) ((bfdev_u64)(                        \
    (((bfdev_u64)(x) & (bfdev_u64)0x0000ffff0000ffffULL) << 16) |   \
    (((bfdev_u64)(x) & (bfdev_u64)0xffff0000ffff0000ULL) >> 16)     \
))

#define bfdev_swahl64_const(x) ((bfdev_u64)(                        \
    (((bfdev_u64)(x) & (bfdev_u64)0x00000000ffffffffULL) << 32) |   \
    (((bfdev_u64)(x) & (bfdev_u64)0xffffffff00000000ULL) >> 32)     \
))

/*
 * Implement the following as inlines, but define the interface using
 * macros to allow constant folding when possible:
 * _swab16, _swab32, _swab64, ___swahw32, _swahb32
 */

static inline __bfdev_attribute_const
bfdev_u16 bfdev_fswab16(bfdev_u16 val)
{
#ifdef bfdev_arch_swab16
    return bfdev_arch_swab16(val);
#else
    return bfdev_swab16_const(val);
#endif
}

static inline __bfdev_attribute_const
bfdev_u32 bfdev_fswab32(bfdev_u32 val)
{
#ifdef bfdev_arch_swab32
    return bfdev_arch_swab32(val);
#else
    return bfdev_swab32_const(val);
#endif
}

static inline __bfdev_attribute_const
bfdev_u64 bfdev_fswab64(bfdev_u64 val)
{
#ifdef bfdev_arch_swab64
    return bfdev_arch_swab64(val);
#else
    return bfdev_swab64_const(val);
#endif
}

static inline __bfdev_attribute_const
bfdev_u32 bfdev_fswahb32(bfdev_u32 val)
{
#ifdef bfdev_arch_swahb32
    return bfdev_arch_swahb32(val);
#else
    return bfdev_swahb32_const(val);
#endif
}

static inline __bfdev_attribute_const
bfdev_u32 bfdev_fswahw32(bfdev_u32 val)
{
#ifdef bfdev_arch_swahw32
    return bfdev_arch_swahw32(val);
#else
    return bfdev_swahw32_const(val);
#endif
}

static inline __bfdev_attribute_const
bfdev_u64 bfdev_fswahb64(bfdev_u64 val)
{
#ifdef bfdev_arch_swahb64
    return bfdev_arch_swahb64(val);
#else
    return bfdev_swahb64_const(val);
#endif
}

static inline __bfdev_attribute_const
bfdev_u64 bfdev_fswahw64(bfdev_u64 val)
{
#ifdef bfdev_arch_swahw64
    return bfdev_arch_swahw64(val);
#else
    return bfdev_swahw64_const(val);
#endif
}

static inline __bfdev_attribute_const
bfdev_u64 bfdev_fswahl64(bfdev_u64 val)
{
#ifdef bfdev_arch_swahl64
    return bfdev_arch_swahl64(val);
#else
    return bfdev_swahl64_const(val);
#endif
}

/**
 * bfdev_swab16 - return a byteswapped 16-bit value.
 * @x: value to byteswap.
 */
#ifdef __HAVE_BUILTIN_BSWAP16__
# define bfdev_swab16(x) (bfdev_u16)__builtin_bswap16((bfdev_u16)(x))
#else
# define bfdev_swab16(x) (                  \
    __builtin_constant_p((bfdev_u16)(x)) ?   \
    bfdev_swab16_const(x) :                 \
    bfdev_fswab16(x)                        \
)
#endif

/**
 * bfdev_swab32 - return a byteswapped 32-bit value.
 * @x: value to byteswap.
 */
#ifdef __HAVE_BUILTIN_BSWAP32__
# define bfdev_swab32(x) (bfdev_u32)__builtin_bswap32((bfdev_u32)(x))
#else
# define bfdev_swab32(x) (                  \
    __builtin_constant_p((bfdev_u32)(x)) ?   \
    bfdev_swab32_const(x) :                 \
    bfdev_fswab32(x)                        \
)
#endif

/**
 * bfdev_swab64 - return a byteswapped 64-bit value.
 * @x: value to byteswap.
 */
#ifdef __HAVE_BUILTIN_BSWAP64__
# define bfdev_swab64(x) (bfdev_u64)__builtin_bswap64((bfdev_u64)(x))
#else
# define bfdev_swab64(x) (                  \
    __builtin_constant_p((bfdev_u64)(x)) ?   \
    bfdev_swab64_const(x) :                 \
    bfdev_fswab64(x)                        \
)
#endif

/**
 * bfdev_swab - return a byteswapped long value.
 * @x: value to byteswap.
 */
static __bfdev_always_inline
unsigned long bfdev_swab(const unsigned long y)
{
#if BFDEV_BITS_PER_LONG == 32
    return bfdev_swab32(y);
#else /* BFDEV_BITS_PER_LONG == 64 */
    return bfdev_swab64(y);
#endif
}

/**
 * bfdev_swahb32 - return a high and low byte-swapped 32-bit value.
 * @x: value to byteswap.
 */
#define bfdev_swahb32(x) (                  \
    __builtin_constant_p((bfdev_u32)(x)) ?   \
    bfdev_swahb32_const(x) :                \
    bfdev_fswahb32(x)                       \
)

/**
 * bfdev_swahw32 - return a word-swapped 32-bit value.
 * @x: value to wordswap.
 */
#define bfdev_swahw32(x) (                  \
    __builtin_constant_p((bfdev_u32)(x)) ?   \
    bfdev_swahw32_const(x) :                \
    bfdev_fswahw32(x)                       \
)

/**
 * bfdev_swahb64 - return a high and low byte-swapped 64-bit value.
 * @x: value to byteswap.
 */
#define bfdev_swahb64(x) (                  \
    __builtin_constant_p((bfdev_u64)(x)) ?   \
    bfdev_swahb64_const(x) :                \
    bfdev_fswahb64(x)                       \
)

/**
 * bfdev_swahw64 - return a word-swapped 64-bit value.
 * @x: value to wordswap.
 */
#define bfdev_swahw64(x) (                  \
    __builtin_constant_p((bfdev_u64)(x)) ?   \
    bfdev_swahw64_const(x) :                \
    bfdev_fswahw64(x)                       \
)

/**
 * bfdev_swahl64 - return a long-swapped 64-bit value.
 * @x: value to wordswap.
 */
#define bfdev_swahl64(x) (                  \
    __builtin_constant_p((bfdev_u64)(x)) ?   \
    bfdev_swahl64_const(x) :                \
    bfdev_fswahl64(x)                       \
)

/**
 * bfdev_swab16p - return a byteswapped 16-bit value from a pointer.
 * @p: pointer to a naturally-aligned 16-bit value.
 */
static __bfdev_always_inline bfdev_u16
bfdev_swab16p(const bfdev_u16 *p)
{
#ifdef bfdev_arch_swab16p
    return bfdev_arch_swab16p(p);
#else
    return bfdev_swab16(*p);
#endif
}

/**
 * bfdev_swab32p - return a byteswapped 32-bit value from a pointer.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static __bfdev_always_inline bfdev_u32
bfdev_swab32p(const bfdev_u32 *p)
{
#ifdef bfdev_arch_swab32p
    return bfdev_arch_swab32p(p);
#else
    return bfdev_swab32(*p);
#endif
}

/**
 * bfdev_swab64p - return a byteswapped 64-bit value from a pointer.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static __bfdev_always_inline bfdev_u64
bfdev_swab64p(const bfdev_u64 *p)
{
#ifdef bfdev_arch_swab64p
    return bfdev_arch_swab64p(p);
#else
    return bfdev_swab64(*p);
#endif
}

/**
 * bfdev_swabp - return a byteswapped long value.
 * @x: value to byteswap.
 */
static __bfdev_always_inline
unsigned long bfdev_swabp(const unsigned long *y)
{
#if BFDEV_BITS_PER_LONG == 32
    return bfdev_swab32p((const bfdev_u32 *)y);
#else /* BFDEV_BITS_PER_LONG == 64 */
    return bfdev_swab64p((const bfdev_u64 *)y);
#endif
}

/**
 * bfdev_swahb32p - return a high and low byteswapped 32-bit value from a pointer.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static inline bfdev_u32
bfdev_swahb32p(const bfdev_u32 *p)
{
#ifdef bfdev_arch_swahb32p
    return bfdev_arch_swahb32p(p);
#else
    return bfdev_swahb32(*p);
#endif
}

/**
 * bfdev_swahw32p - return a wordswapped 32-bit value from a pointer.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static inline bfdev_u32
bfdev_swahw32p(const bfdev_u32 *p)
{
#ifdef bfdev_arch_swahw32p
    return bfdev_arch_swahw32p(p);
#else
    return bfdev_swahw32(*p);
#endif
}

/**
 * bfdev_swahb64p - return a high and low byteswapped 64-bit value from a pointer.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline bfdev_u64
bfdev_swahb64p(const bfdev_u64 *p)
{
#ifdef bfdev_arch_swahb64p
    return bfdev_arch_swahb64p(p);
#else
    return bfdev_swahb64(*p);
#endif
}

/**
 * bfdev_swahw64p - return a wordswapped 64-bit value from a pointer.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline bfdev_u64
bfdev_swahw64p(const bfdev_u64 *p)
{
#ifdef bfdev_arch_swahw64p
    return bfdev_arch_swahw64p(p);
#else
    return bfdev_swahw64(*p);
#endif
}

/**
 * bfdev_swahl64p - return a longswapped 64-bit value from a pointer.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline bfdev_u64
bfdev_swahl64p(const bfdev_u64 *p)
{
#ifdef bfdev_arch_swahl64p
    return bfdev_arch_swahl64p(p);
#else
    return bfdev_swahl64(*p);
#endif
}

/**
 * bfdev_swab16s - byteswap a 16-bit value in-place.
 * @p: pointer to a naturally-aligned 16-bit value.
 */
static inline void
bfdev_swab16s(bfdev_u16 *p)
{
#ifdef bfdev_arch_swab16s
    bfdev_arch_swab16s(p);
#else
    *p = bfdev_swab16p(p);
#endif
}

/**
 * bfdev_swab32s - byteswap a 32-bit value in-place.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static __bfdev_always_inline void
bfdev_swab32s(bfdev_u32 *p)
{
#ifdef bfdev_arch_swab32s
    bfdev_arch_swab32s(p);
#else
    *p = bfdev_swab32p(p);
#endif
}

/**
 * bfdev_swab64s - byteswap a 64-bit value in-place.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static __bfdev_always_inline void
bfdev_swab64s(bfdev_u64 *p)
{
#ifdef bfdev_arch_swab64s
    bfdev_arch_swab64s(p);
#else
    *p = bfdev_swab64p(p);
#endif
}

/**
 * bfdev_swabs - byteswap a long value in-place.
 * @p: pointer to a naturally-aligned long value.
 */
static __bfdev_always_inline void
bfdev_swabs(unsigned long *p)
{
#if BFDEV_BITS_PER_LONG == 32
    bfdev_swab32s((bfdev_u32 *)p);
#else /* BFDEV_BITS_PER_LONG == 64 */
    bfdev_swab64s((bfdev_u64 *)p);
#endif
}

/**
 * bfdev_swahb32s - high and low byteswap a 32-bit value in-place.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static inline void
bfdev_swahb32s(bfdev_u32 *p)
{
#ifdef bfdev_arch_swahb32s
    bfdev_arch_swahb32s(p);
#else
    *p = bfdev_swahb32p(p);
#endif
}

/**
 * bfdev_swahw32s - wordswap a 32-bit value in-place.
 * @p: pointer to a naturally-aligned 32-bit value.
 */
static inline void
bfdev_swahw32s(bfdev_u32 *p)
{
#ifdef bfdev_arch_swahw32s
    bfdev_arch_swahw32s(p);
#else
    *p = bfdev_swahw32p(p);
#endif
}

/**
 * bfdev_swahb64s - high and low byteswap a 64-bit value in-place.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline void
bfdev_swahb64s(bfdev_u64 *p)
{
#ifdef bfdev_arch_swahb64s
    bfdev_arch_swahb64s(p);
#else
    *p = bfdev_swahb64p(p);
#endif
}

/**
 * bfdev_swahw64s - wordswap a 64-bit value in-place.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline void
bfdev_swahw64s(bfdev_u64 *p)
{
#ifdef bfdev_arch_swahw64s
    bfdev_arch_swahw64s(p);
#else
    *p = bfdev_swahw64p(p);
#endif
}

/**
 * bfdev_swahw64s - longswap a 64-bit value in-place.
 * @p: pointer to a naturally-aligned 64-bit value.
 */
static inline void
bfdev_swahl64s(bfdev_u64 *p)
{
#ifdef bfdev_arch_swahw64s
    bfdev_arch_swahw64s(p);
#else
    *p = bfdev_swahl64p(p);
#endif
}

static inline void
bfdev_swab16_array(bfdev_u16 *buff, unsigned int count)
{
    while (count--) {
        bfdev_swab16s(buff);
        buff++;
    }
}

static inline void
bfdev_swab32_array(bfdev_u32 *buff, unsigned int count)
{
    while (count--) {
        bfdev_swab32s(buff);
        buff++;
    }
}

static inline void
bfdev_swab64_array(bfdev_u64 *buff, unsigned int count)
{
    while (count--) {
        bfdev_swab64s(buff);
        buff++;
    }
}

static inline void
bfdev_swab_array(unsigned long *buff, unsigned int count)
{
#if BFDEV_BITS_PER_LONG == 32
    bfdev_swab32_array((bfdev_u32 *)buff, count);
#else /* BFDEV_BITS_PER_LONG == 64 */
    bfdev_swab64_array((bfdev_u64 *)buff, count);
#endif
}

BFDEV_END_DECLS

#endif /* _BFDEV_SWAB_H_ */
