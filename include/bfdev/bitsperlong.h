/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BITSPERLONG_H_
#define _BFDEV_BITSPERLONG_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

#if __SIZEOF_POINTER__ == 4
# define BFDEV_BITS_PER_LONG 32
#else /* __SIZEOF_POINTER__ == 8 */
# define BFDEV_BITS_PER_LONG 64
#endif /* __SIZEOF_POINTER__ == 8 */

#define bfdev_const_small_nbits(nbits) ( \
    __builtin_constant_p(nbits) && (nbits) <= \
    BFDEV_BITS_PER_LONG && (nbits) > 0 \
)

BFDEV_END_DECLS

#endif /* _BFDEV_BITSPERLONG_H_ */
