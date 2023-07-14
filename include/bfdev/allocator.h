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

BFDEV_BEGIN_DECLS

struct bfdev_alloc {
    void *(*malloc)(size_t size, void *pdata);
    void *(*realloc)(const void *block, size_t resize, void *pdata);
    void (*free)(const void *block, void *pdata);
    void *pdata;
};

extern __bfdev_malloc void *bfdev_malloc(const struct bfdev_alloc *alloc, size_t size);
extern __bfdev_malloc void *bfdev_zalloc(const struct bfdev_alloc *alloc, size_t size);
extern __bfdev_malloc void *bfdev_realloc(const struct bfdev_alloc *alloc, const void *block, size_t resize);
extern void bfdev_free(const struct bfdev_alloc *alloc, const void *block);

/**
 * bfdev_malloc_array - allocate memory for an array.
 * @nr: number of elements.
 * @size: single element size.
 */
static __bfdev_always_inline __bfdev_malloc void *
bfdev_malloc_array(const struct bfdev_alloc *alloc,
                   size_t nr, size_t size)
{
    return bfdev_malloc(alloc, size * nr);
}

/**
 * bfdev_zalloc_array - zero allocate memory for an array.
 * @nr: number of elements.
 * @size: single element size.
 */
static __bfdev_always_inline __bfdev_malloc void *
bfdev_zalloc_array(const struct bfdev_alloc *alloc,
                   size_t nr, size_t size)
{
    return bfdev_zalloc(alloc, size * nr);
}

/**
 * bfdev_realloc_array - reallocate memory for an array.
 * @block: number of elements.
 * @nr: number of elements.
 * @size: single element size.
 */
static __bfdev_always_inline __bfdev_malloc void *
bfdev_realloc_array(const struct bfdev_alloc *alloc,
                    void *block, size_t nr, size_t size)
{
    return bfdev_realloc(alloc, block, size * nr);
}

BFDEV_END_DECLS

#endif /* _BFDEV_ALLOCATOR_H_ */
