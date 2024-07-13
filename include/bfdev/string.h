/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Fredrik Anderson <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_STRING_H_
#define _BFDEV_STRING_H_

#include <bfdev/config.h>
#include <bfdev/port/string.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

extern char *
bfdev_strdiff(const char *addr, int c);

extern void *
bfdev_memdiff(const void *addr, int c, size_t size);

extern char *
bfdev_strdup(const bfdev_alloc_t *alloc,
             const char *string);

extern char *
bfdev_strndup(const bfdev_alloc_t *alloc,
              const char *string, size_t len);

BFDEV_END_DECLS

#endif /* _BFDEV_STRING_H_ */
