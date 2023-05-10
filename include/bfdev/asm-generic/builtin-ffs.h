/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _BFDEV_ASM_GENERIC_BUILTIN_FFS_H_
#define _BFDEV_ASM_GENERIC_BUILTIN_FFS_H_

#include <bfdev/bits.h>
#include <bfdev/attributes.h>

#ifdef __cplusplus
extern "C" {
#endif

#define bfdev_arch_ffs bfdev_arch_ffs
static __always_inline unsigned int bfdev_arch_ffs(unsigned long value)
{
	return __builtin_ffsl(value);
}

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_ASM_GENERIC_BUILTIN_FFS_H_ */
