/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CDEFS_H_
#define _BFDEV_CDEFS_H_

/*
 * GCC and clang have various useful declarations that can be made with
 * the '__attribute__' syntax.  All of the ways we use this do fine if
 * they are omitted for compilers that don't understand it.
 */
#if !(defined __GNUC__ || defined __clang__)
# define __attribute__(xyz) /* Ignore */
#endif

#ifdef __cplusplus
# define BFDEV_BEGIN_DECLS extern "C" {
# define BFDEV_END_DECLS }
#else
# define BFDEV_BEGIN_DECLS
# define BFDEV_END_DECLS
#endif

#endif /* _BFDEV_CDEFS_H_ */
