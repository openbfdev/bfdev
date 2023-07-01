/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_STRING_H_
#define _BFDEV_STRING_H_

#include <bfdev/config.h>

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/systm.h>
#else
# include <string.h>
#endif

BFDEV_BEGIN_DECLS

extern char *
bfdev_strdiff(const char *addr, int c);

extern void *
bfdev_memdiff(const void *addr, int c, size_t size);

BFDEV_END_DECLS

#endif /* _BFDEV_STRING_H_ */
