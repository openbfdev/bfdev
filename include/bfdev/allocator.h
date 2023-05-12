/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ALLOCATOR_H_
#define _BFDEV_ALLOCATOR_H_

#include <bfdev/config.h>
#include <bfdev/stddef.h>
#include <bfdev/stdlib.h>
#include <bfdev/attributes.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bfdev_alloc {
    void *(*malloc)(size_t size, void *pdata);
    void *(*realloc)(const void *block, size_t resize, void *pdata);
    void (*free)(const void *block, void *pdata);
    void *pdata;
};

static inline __malloc void *
bfdev_alloc_malloc(const struct bfdev_alloc *alloc, size_t size)
{
    void *pdata = alloc->pdata;
    return alloc->malloc(size, pdata);
}

static inline __malloc void *
bfdev_alloc_realloc(const struct bfdev_alloc *alloc, const void *block, size_t resize)
{
    void *pdata = alloc->pdata;
    return alloc->realloc(block, resize, pdata);
}

static inline void
bfdev_alloc_free(const struct bfdev_alloc *alloc, const void *block)
{
    void *pdata = alloc->pdata;
    return alloc->free(block, pdata);
}

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_ALLOCATOR_H_ */
