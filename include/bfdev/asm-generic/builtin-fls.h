/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _BFDEV_ASM_GENERIC_BUILTIN_FLS_H_
#define _BFDEV_ASM_GENERIC_BUILTIN_FLS_H_

#include <bfdev/bits.h>
#include <bfdev/attributes.h>

#ifdef __cplusplus
extern "C" {
#endif

#define bfdev_arch_fls bfdev_arch_fls
static __always_inline unsigned int bfdev_arch_fls(unsigned long value)
{
	return value ? BFDEV_BITS_PER_LONG - __builtin_clzl(value) : 0;
}

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_ASM_GENERIC_BUILTIN_FLS_H_ */
