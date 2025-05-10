/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_BARRIER_H_
#define _BFDEV_ASM_GENERIC_BARRIER_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

#ifndef bfdev_arch_mb
# define bfdev_arch_mb() __sync_synchronize()
#endif

#ifndef bfdev_arch_rmb
# define bfdev_arch_rmb() bfdev_arch_mb()
#endif

#ifndef bfdev_arch_wmb
# define bfdev_arch_wmb() bfdev_arch_mb()
#endif

#ifndef bfdev_arch_smp_mb
# define bfdev_arch_smp_mb() bfdev_arch_mb()
#endif

#ifndef bfdev_arch_smp_rmb
# define bfdev_arch_smp_rmb() bfdev_arch_rmb()
#endif

#ifndef bfdev_arch_smp_wmb
# define bfdev_arch_smp_wmb() bfdev_arch_wmb()
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_BARRIER_H_ */
