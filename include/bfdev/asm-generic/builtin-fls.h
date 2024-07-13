/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Fredrik Anderson <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_BUILTIN_FLS_H_
#define _BFDEV_ASM_GENERIC_BUILTIN_FLS_H_

#include <bfdev/config.h>
#include <bfdev/bits.h>

BFDEV_BEGIN_DECLS

#define bfdev_arch_fls bfdev_arch_fls
static __bfdev_always_inline unsigned int
bfdev_arch_fls(unsigned long value)
{
    return value ? BFDEV_BITS_PER_LONG - __builtin_clzl(value) : 0;
}

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_BUILTIN_FLS_H_ */
