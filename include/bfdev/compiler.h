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
#ifndef __bfdev_barrier
# define __bfdev_barrier(carrier) __asm__ __volatile__("":carrier:"memory")
#endif

#ifndef bfdev_barrier
# define bfdev_barrier() __bfdev_barrier(:)
# define bfdev_barrier_data(ptr) __bfdev_barrier(:"r"(ptr))
#endif

/*
 * Whether 'type' is a signed type or an unsigned type.
 * Supports scalar types, bool and also pointer types.
 */
#define bfdev_is_signed(type) (((type)(-1)) < (type)1)
#define bfdev_is_unsigned(type) (!bfdev_is_signed(type))

/* Not-quite-unique ID. */
#ifndef __BFDEV_UNIQUE_ID
# define __BFDEV_UNIQUE_ID(prefix) __BFDEV_PASTE(__BFDEV_PASTE(__UNIQUE_ID_, prefix), __LINE__)
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_COMPILER_H_ */
