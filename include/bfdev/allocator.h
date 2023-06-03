/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ALLOCATOR_H_
#define _BFDEV_ALLOCATOR_H_

#include <bfdev/config.h>
#include <bfdev/cdefs.h>
#include <bfdev/stddef.h>
#include <bfdev/stdlib.h>
#include <bfdev/attributes.h>

BFDEV_BEGIN_DECLS

struct bfdev_alloc {
    void *(*malloc)(size_t size, void *pdata);
    void *(*realloc)(const void *block, size_t resize, void *pdata);
    void (*free)(const void *block, void *pdata);
    void *pdata;
};

extern __malloc void *bfdev_malloc(const struct bfdev_alloc *alloc, size_t size);
extern __malloc void *bfdev_zalloc(const struct bfdev_alloc *alloc, size_t size);
extern __malloc void *bfdev_realloc(const struct bfdev_alloc *alloc, const void *block, size_t resize);
extern void bfdev_free(const struct bfdev_alloc *alloc, const void *block);

BFDEV_END_DECLS

#endif /* _BFDEV_ALLOCATOR_H_ */
