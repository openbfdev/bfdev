/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CONST_H_
#define _BFDEV_CONST_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __ASSEMBLY__
# define _AC(X,Y)   X
# define _AT(T,X)   X
#else
# define __AC(X,Y)  (X##Y)
# define _AC(X,Y)   __AC(X,Y)
# define _AT(T,X)   ((T)(X))
#endif

#define _UL(x)      (_AC(x, UL))
#define _ULL(x)     (_AC(x, ULL))

#define _BITUL(x)   (_UL(1) << (x))
#define _BITULL(x)  (_ULL(1) << (x))

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_CONST_H_ */
