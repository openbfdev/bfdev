/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BITWALK_COMP_H_
#define _BFDEV_BITWALK_COMP_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

extern unsigned int
bfdev_comp_find_first_bit(const unsigned long *block,
                          unsigned int bits, bool swap);

extern unsigned int
bfdev_comp_find_last_bit(const unsigned long *block,
                         unsigned int bits, bool swap);

extern unsigned int
bfdev_comp_find_first_zero(const unsigned long *block,
                           unsigned int bits, bool swap);

extern unsigned int
bfdev_comp_find_last_zero(const unsigned long *block,
                          unsigned int bits, bool swap);

extern unsigned int
bfdev_comp_find_next_bit(const unsigned long *addr1, const unsigned long *addr2,
                         unsigned int bits, unsigned int start,
                         unsigned long invert, bool swap);

extern unsigned int
bfdev_comp_find_prev_bit(const unsigned long *addr1, const unsigned long *addr2,
                         unsigned int bits, unsigned int start,
                         unsigned long invert, bool swap);

BFDEV_END_DECLS

#endif /* _BFDEV_BITWALK_COMP_H_ */
