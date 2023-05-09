/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _BFDEV_ASM_GENERIC_BUILTIN_CTZ_H_
#define _BFDEV_ASM_GENERIC_BUILTIN_CTZ_H_

#include <bfdev/bits.h>
#include <bfdev/attributes.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !__has_builtin(__builtin_ctzl)
# error "compiler not supported ctzl"
#endif

#define bfdev_arch_ctz bfdev_arch_ctz
static __always_inline unsigned int bfdev_arch_ctz(unsigned long value)
{
	return __builtin_ctzl(value);
}

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_ASM_GENERIC_BUILTIN_CTZ_H_ */
