/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_HASH_H_
#define _BFDEV_HASH_H_

#include <bfdev/config.h>
#include <bfdev/bits.h>

BFDEV_BEGIN_DECLS

#define BFDEV_GOLDEN_RATIO_32 0x61c88647UL
#define BFDEV_GOLDEN_RATIO_64 0x61c8864680b583ebULL

#ifdef BFDEV_ARCH_HAS_HASH
# include <bfdev/asm/hash.h>
#endif

#ifndef BFDEV_ARCH_HAS_HASH32
# define bfdev_hashv32 bfdev_hashv32_generic
#endif

#ifndef BFDEV_ARCH_HAS_HASH64
# define bfdev_hashv64 bfdev_hashv64_generic
#endif

/**
 * bfdev_hashv{32/64}_generic() - General func for calc hash value.
 * @value: value to hash.
 */
static __bfdev_always_inline
uint32_t bfdev_hashv32_generic(uint32_t value)
{
    /* Gold ratio method calc hash value. */
    return value * BFDEV_GOLDEN_RATIO_32;
}

static __bfdev_always_inline
uint64_t bfdev_hashv64_generic(uint64_t value)
{
    return value * BFDEV_GOLDEN_RATIO_64;
}

/**
 * bfdev_hash{32/64}() - Fast hashing routine for ints.
 * @value: value to hash.
 * @bits: bit number of result.
 */
static __bfdev_always_inline
uint32_t bfdev_hash32(uint32_t value, unsigned int bits)
{
    /* High bits are more random, so use them. */
    return bfdev_hashv32(value) >> (BFDEV_BITS_PER_U32 - bits);
}

static __bfdev_always_inline
uint64_t bfdev_hash64(uint64_t value, unsigned int bits)
{
    return bfdev_hashv64(value) >> (BFDEV_BITS_PER_U64 - bits);
}

#if BFDEV_BITS_PER_LONG == 32
# define BFDEV_GOLDEN_RATIO_LONG BFDEV_GOLDEN_RATIO_32
# define bfdev_hashvl(value) bfdev_hashv32(value)
# define bfdev_hashl(value, bits) bfdev_hash32(value, bits)
#else /* BFDEV_BITS_PER_LONG == 64 */
# define BFDEV_GOLDEN_RATIO_LONG BFDEV_GOLDEN_RATIO_64
# define bfdev_hashvl(value) bfdev_hashv64(value)
# define bfdev_hashl(value, bits) bfdev_hash64(value, bits)
#endif /* BFDEV_BITS_PER_LONG == 64 */

BFDEV_END_DECLS

#endif /* _BFDEV_HASH_H_ */
