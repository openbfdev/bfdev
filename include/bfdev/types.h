/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Fredrik Anderson <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_TYPES_H_
#define _BFDEV_TYPES_H_

#include <bfdev/config.h>
#include <bfdev/port/stdint.h>
#include <bfdev/port/stddef.h>
#include <bfdev/asm/bitsperlong.h>

BFDEV_BEGIN_DECLS

#ifdef __CHECKER__
# define __bfdev_bitwise __attribute__((bitwise))
# define __bfdev_force __attribute__((force))
#else
# define __bfdev_bitwise
# define __bfdev_force
#endif

typedef uint16_t __bfdev_bitwise bfdev_le16;
typedef uint32_t __bfdev_bitwise bfdev_le32;
typedef uint64_t __bfdev_bitwise bfdev_le64;
typedef uint16_t __bfdev_bitwise bfdev_be16;
typedef uint32_t __bfdev_bitwise bfdev_be32;
typedef uint64_t __bfdev_bitwise bfdev_be64;

typedef int bfdev_sqi_t __bfdev_mode(QI);
typedef int bfdev_shi_t __bfdev_mode(HI);
typedef int bfdev_ssi_t __bfdev_mode(SI);
typedef int bfdev_sdi_t __bfdev_mode(DI);
typedef unsigned bfdev_uqi_t __bfdev_mode(QI);
typedef unsigned bfdev_uhi_t __bfdev_mode(HI);
typedef unsigned bfdev_usi_t __bfdev_mode(SI);
typedef unsigned bfdev_udi_t __bfdev_mode(DI);

typedef long bfdev_sw_t;
typedef unsigned long bfdev_uw_t;

#if BFDEV_BITS_PER_LONG == 32
typedef bfdev_shi_t bfdev_shw_t;
typedef bfdev_uhi_t bfdev_uhw_t;
#else /* BFDEV_BITS_PER_LONG == 64 */
typedef bfdev_ssi_t bfdev_shw_t;
typedef bfdev_usi_t bfdev_uhw_t;
#endif

typedef int bfdev_state_t;
typedef intptr_t bfdev_atomic_t;

#define BFDEV_EQ 0
#define BFDEV_BT 1
#define BFDEV_LT -1

#define BFDEV_BYTES_PER_CHAR        sizeof(char)
#define BFDEV_BYTES_PER_SHORT       sizeof(short)
#define BFDEV_BYTES_PER_INT         sizeof(int)
#define BFDEV_BYTES_PER_LONG        sizeof(long)
#define BFDEV_BYTES_PER_LONG_LONG   sizeof(long long)

#define BFDEV_BYTES_PER_U8          sizeof(uint8_t)
#define BFDEV_BYTES_PER_U16         sizeof(uint16_t)
#define BFDEV_BYTES_PER_U32         sizeof(uint32_t)
#define BFDEV_BYTES_PER_U64         sizeof(uint64_t)
#define BFDEV_BYTES_PER_UINTMAX     sizeof(uintmax_t)
#define BFDEV_BYTES_PER_UINTPTR     sizeof(uintptr_t)

/* Generic callback functions */
#define BFDEV_CALLBACK_FIND(name, type) \
    typedef long (*name)(type key, void *pdata)

#define BFDEV_CALLBACK_CMP(name, type) \
    typedef long (*name)(type key1, type key2, void *pdata)

#define BFDEV_CALLBACK_RELEASE(name, type) \
    typedef void (*name)(type value, void *pdata)

BFDEV_CALLBACK_FIND(bfdev_find_t, const void *);
BFDEV_CALLBACK_CMP(bfdev_cmp_t, const void *);
BFDEV_CALLBACK_RELEASE(bfdev_release_t, void *);

typedef void *(*bfdev_malloc_t)(size_t size, void *pdata);
typedef void *(*bfdev_realloc_t)(void *block, size_t resize, void *pdata);
typedef void (*bfdev_free_t)(void *block, void *pdata);

BFDEV_END_DECLS

#endif /* _BFDEV_TYPES_H_ */
