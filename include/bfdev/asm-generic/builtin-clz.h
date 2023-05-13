/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _BFDEV_ASM_GENERIC_BUILTIN_CLZ_H_
#define _BFDEV_ASM_GENERIC_BUILTIN_CLZ_H_

#include <bfdev/config.h>
#include <bfdev/bits.h>
#include <bfdev/attributes.h>

#ifdef __cplusplus
extern "C" {
#endif

#define bfdev_arch_clz bfdev_arch_clz
static __always_inline unsigned int bfdev_arch_clz(unsigned long value)
{
	return __builtin_clzl(value);
}

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_ASM_GENERIC_BUILTIN_CLZ_H_ */
