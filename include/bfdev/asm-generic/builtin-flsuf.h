/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_BUILTIN_FLSUF_H_
#define _BFDEV_ASM_GENERIC_BUILTIN_FLSUF_H_

#include <bfdev/config.h>
#include <bfdev/bits.h>

BFDEV_BEGIN_DECLS

#define bfdev_arch_flsuf bfdev_arch_flsuf
static __bfdev_always_inline unsigned int
bfdev_arch_flsuf(unsigned long value)
{
    return BFDEV_BITS_PER_LONG - 1 - __builtin_clzl(value);
}

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_BUILTIN_FLSUF_H_ */
