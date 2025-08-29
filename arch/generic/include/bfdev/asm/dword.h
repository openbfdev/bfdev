/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_DWORD_H_
#define _BFDEV_ASM_DWORD_H_

#include <bfdev/config.h>
#include <bfdev/asm/bitsperlong.h>

BFDEV_BEGIN_DECLS

#if BFDEV_BITS_PER_LONG == 32
# define __BFDEV_ASM_HAVE_DWORD 1
#else /* BFDEV_BITS_PER_LONG == 64 */
# define __BFDEV_ASM_HAVE_DWORD __SIZEOF_INT128__
#endif

#ifndef BFDEV_ASM_HAVE_DWORD
# define BFDEV_ASM_HAVE_DWORD __BFDEV_ASM_HAVE_DWORD
#endif

BFDEV_END_DECLS

#ifdef BFDEV_ASM_HAVE_DWORD
# include <bfdev/asm-generic/dword-compiler.h>
#endif

#include <bfdev/asm-generic/dword.h>

#endif /* _BFDEV_ASM_DWORD_H_ */
