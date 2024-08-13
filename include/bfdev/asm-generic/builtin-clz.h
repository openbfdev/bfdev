/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_BUILTIN_CLZ_H_
#define _BFDEV_ASM_GENERIC_BUILTIN_CLZ_H_

#include <bfdev/config.h>
#include <bfdev/bits.h>

BFDEV_BEGIN_DECLS

#define bfdev_arch_clz bfdev_arch_clz
static __bfdev_always_inline unsigned int
bfdev_arch_clz(unsigned long value)
{
    return __builtin_clzl(value);
}

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_BUILTIN_CLZ_H_ */
