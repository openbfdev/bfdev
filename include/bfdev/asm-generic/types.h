/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_TYPES_H_
#define _BFDEV_ASM_GENERIC_TYPES_H_

#include <bfdev/config.h>
#include <bfdev/asm/bitsperlong.h>

BFDEV_BEGIN_DECLS

typedef __signed__ char bfdev_s8;
typedef __signed__ short bfdev_s16;
typedef __signed__ int bfdev_s32;

typedef unsigned char bfdev_u8;
typedef unsigned short bfdev_u16;
typedef unsigned int bfdev_u32;

#ifdef __GNUC__
__extension__ typedef __signed__ BFDEV_TYPE_W64 bfdev_s64;
__extension__ typedef unsigned BFDEV_TYPE_W64 bfdev_u64;
#else
typedef __signed__ BFDEV_TYPE_W64 bfdev_s64;
typedef unsigned BFDEV_TYPE_W64 bfdev_u64;
#endif

typedef unsigned BFDEV_TYPE_ADDR bfdev_size_t;
typedef __signed__ BFDEV_TYPE_ADDR bfdev_ssize_t;

typedef unsigned BFDEV_TYPE_ADDR bfdev_uintptr_t;
typedef __signed__ BFDEV_TYPE_ADDR bfdev_intptr_t;
typedef __signed__ BFDEV_TYPE_ADDR bfdev_ptrdiff_t;

typedef __signed__ BFDEV_TYPE_MAX bfdev_intmax_t;
typedef unsigned BFDEV_TYPE_MAX bfdev_uintmax_t;

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_TYPES_H_ */
