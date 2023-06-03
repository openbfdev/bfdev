/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_STDINT_H_
#define _BFDEV_STDINT_H_

#include <bfdev/config.h>
#include <bfdev/cdefs.h>

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/stdint.h>
#else
# include <stdint.h>
#endif

BFDEV_BEGIN_DECLS

#ifdef __CHECKER__
# define __bitwise __attribute__((bitwise))
#else
# define __bitwise
#endif

typedef uint16_t __bitwise bfdev_le16;
typedef uint32_t __bitwise bfdev_le32;
typedef uint64_t __bitwise bfdev_le64;
typedef uint16_t __bitwise bfdev_be16;
typedef uint32_t __bitwise bfdev_be32;
typedef uint64_t __bitwise bfdev_be64;

typedef unsigned long bfdev_atomic_t;

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

BFDEV_END_DECLS

#endif /* _BFDEV_STDINT_H_ */
