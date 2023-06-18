/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _BFDEV_ASM_GENERIC_BUILTIN_FFS_H_
#define _BFDEV_ASM_GENERIC_BUILTIN_FFS_H_

#include <bfdev/config.h>
#include <bfdev/bits.h>

BFDEV_BEGIN_DECLS

#define bfdev_arch_ffs bfdev_arch_ffs
static __always_inline unsigned int bfdev_arch_ffs(unsigned long value)
{
	return __builtin_ffsl(value);
}

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_BUILTIN_FFS_H_ */
