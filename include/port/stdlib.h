/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _LOCAL_PORT_STDLIB_H_
#define _LOCAL_PORT_STDLIB_H_

#include <bfdev/config.h>
#include <bfdev/types.h>

BFDEV_BEGIN_DECLS

extern __bfdev_malloc void *
bfport_malloc(bfdev_size_t size);

extern __bfdev_malloc void *
bfport_calloc(bfdev_size_t nmemb, bfdev_size_t size);

extern __bfdev_malloc void *
bfport_realloc(void *ptr, bfdev_size_t size);

extern void
bfport_free(void *ptr);

extern int
bfport_rand(void);

extern __bfdev_noreturn void
bfport_abort(void);

BFDEV_END_DECLS

#endif /* _LOCAL_PORT_STDLIB_H_ */
