/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ARGV_H_
#define _BFDEV_ARGV_H_

#include <bfdev/config.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

extern unsigned int
bfdev_argv_count(const char *string);

extern char **
bfdev_argv_split(const struct bfdev_alloc *alloc, const char *string, unsigned int *argcp);

BFDEV_END_DECLS

#endif /* _BFDEV_ARGV_H_ */
