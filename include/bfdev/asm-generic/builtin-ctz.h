/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_BUILTIN_CTZ_H_
#define _BFDEV_ASM_GENERIC_BUILTIN_CTZ_H_

#include <bfdev/config.h>
#include <bfdev/bits.h>

BFDEV_BEGIN_DECLS

#define bfdev_arch_ctz bfdev_arch_ctz
static __bfdev_always_inline unsigned int
bfdev_arch_ctz(unsigned long value)
{
    return __builtin_ctzl(value);
}

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_BUILTIN_CTZ_H_ */
