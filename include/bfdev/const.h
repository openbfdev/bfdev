/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CONST_H_
#define _BFDEV_CONST_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

#ifdef __BFDEV_ASSEMBLY__
# define BFDEV_AC(X, Y) X
# define BFDEV_AT(T, X) X
#else
# define _BFDEV_AC(X, Y) (X##Y)
# define BFDEV_AC(X, Y) _BFDEV_AC(X, Y)
# define BFDEV_AT(T, X) ((T)(X))
#endif

#define BFDEV_UL(x) (BFDEV_AC(x, UL))
#define BFDEV_ULL(x) (BFDEV_AC(x, ULL))

#define BFDEV_BITUL(x) (BFDEV_UL(1) << (x))
#define BFDEV_BITULL(x) (BFDEV_ULL(1) << (x))

BFDEV_END_DECLS

#endif /* _BFDEV_CONST_H_ */
