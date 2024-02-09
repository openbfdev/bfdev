/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_DWORD_H_
#define _BFDEV_ASM_GENERIC_DWORD_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/bits.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_DWORD_BITS
# define BFDEV_DWORD_BITS (BFDEV_BITS_PER_LONG >> 1)
# define BFDEV_DWORD_SIZE (1UL << BFDEV_DWORD_BITS)
#endif

#define BFDEV_DWORD_LOWER(val) \
    ((bfdev_uw_t)(val) & (BFDEV_DWORD_SIZE - 1))

#define BFDEV_DWORD_HIGHER(val) \
    ((bfdev_uw_t)(val) >> BFDEV_DWORD_BITS)

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_DWORD_H_ */
