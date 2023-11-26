/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_COMPILER_H_
#define _BFDEV_COMPILER_H_

#include <bfdev/config.h>
#include <bfdev/compiler/types.h>

BFDEV_BEGIN_DECLS

#ifndef bfdev_likely
# define bfdev_likely(x) __builtin_expect(!!(x), 1)
# define bfdev_unlikely(x) __builtin_expect(!!(x), 0)
# define bfdev_likely_notrace(x) __builtin_expect(!!(x), 1)
# define bfdev_unlikely_notrace(x) __builtin_expect(!!(x), 0)
#endif

/*
 * Optimization barrier
 * The "volatile" is due to gcc bugs
 */
#ifndef bfdev_barrier
# define bfdev_barrier() __asm__ __volatile__("": : :"memory")
#endif

/* Not-quite-unique ID. */
#ifndef __BFDEV_UNIQUE_ID
# define __BFDEV_UNIQUE_ID(prefix) __BFDEV_PASTE(__BFDEV_PASTE(__UNIQUE_ID_, prefix), __LINE__)
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_COMPILER_H_ */
