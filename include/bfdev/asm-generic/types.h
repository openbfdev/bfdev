/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_TYPES_H_
#define _BFDEV_ASM_GENERIC_TYPES_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

typedef __signed__ char bfdev_s8;
typedef __signed__ short bfdev_s16;
typedef __signed__ int bfdev_s32;

typedef unsigned char bfdev_u8;
typedef unsigned short bfdev_u16;
typedef unsigned int bfdev_u32;

#ifdef __GNUC__
__extension__ typedef __signed__ long long bfdev_s64;
__extension__ typedef unsigned long long bfdev_u64;
#else
typedef __signed__ long long bfdev_s64;
typedef unsigned long long bfdev_u64;
#endif

typedef unsigned long bfdev_size_t;
typedef bfdev_s64 bfdev_intmax_t;
typedef bfdev_u64 bfdev_uintmax_t;

typedef unsigned long bfdev_uintptr_t;
typedef __signed__ long bfdev_intptr_t;
typedef __signed__ long bfdev_ptrdiff_t;

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_TYPES_H_ */
