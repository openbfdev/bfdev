/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _BFDEV_ASM_GENERIC_BUILTIN_FLSUF_H_
#define _BFDEV_ASM_GENERIC_BUILTIN_FLSUF_H_

#include <bfdev/bits.h>
#include <bfdev/attributes.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !__has_builtin(__builtin_clzl)
# error "compiler not supported clzl"
#endif

#define bfdev_arch_flsuf bfdev_arch_flsuf
static __always_inline unsigned int bfdev_arch_flsuf(unsigned long value)
{
	return BFDEV_BITS_PER_LONG - 1 - __builtin_clzl(value);
}

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_ASM_GENERIC_BUILTIN_FLSUF_H_ */
