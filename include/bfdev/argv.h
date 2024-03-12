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

/**
 * bfdev_argv_split() - split a string at whitespace, returning an argv.
 * @alloc: the allocator used to allocate memory.
 * @string: the string to be split.
 * @argcp: returned argument count.
 */
extern char **
bfdev_argv_split(const bfdev_alloc_t *alloc, const char *string,
                 unsigned int *argcp);

/**
 * bfdev_argv_destory() - destory an argv.
 * @alloc: the allocator used to free memory.
 * @argv: the argument vector to be freed.
 */
extern void
bfdev_argv_destory(const bfdev_alloc_t *alloc, char **argv);

BFDEV_END_DECLS

#endif /* _BFDEV_ARGV_H_ */
