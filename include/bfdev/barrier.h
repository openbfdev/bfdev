/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BARRIER_H_
#define _BFDEV_BARRIER_H_

#include <bfdev/config.h>
#include <bfdev/asm/barrier.h>

BFDEV_BEGIN_DECLS

/* Read and write memory barrier */
#ifndef bfdev_mb
# define bfdev_mb() bfdev_arch_mb()
#endif

/* Read memory barrier */
#ifndef bfdev_rmb
# define bfdev_rmb() bfdev_arch_rmb()
#endif

/* Write memory barrier */
#ifndef bfdev_wmb
# define bfdev_wmb() bfdev_arch_wmb()
#endif

/* SMP read and write memory barrier */
#ifndef bfdev_smp_mb
# define bfdev_smp_mb() bfdev_arch_smp_mb()
#endif

/* SMP read memory barrier */
#ifndef bfdev_smp_rmb
# define bfdev_smp_rmb() bfdev_arch_smp_rmb()
#endif

/* SMP write memory barrier */
#ifndef bfdev_smp_wmb
# define bfdev_smp_wmb() bfdev_arch_smp_wmb()
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_BARRIER_H_ */
