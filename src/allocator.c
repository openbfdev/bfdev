/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/allocator.h>
#include <export.h>

static __always_inline void *
generic_malloc(size_t size)
{
    return malloc(size);
}

static __always_inline void *
generic_realloc(const void *block, size_t resize)
{
    return realloc((void *)block, resize);
}

static __always_inline void
generic_free(const void *block)
{
    free((void *)block);
}

export __malloc void *
bfdev_malloc(const struct bfdev_alloc *alloc, size_t size)
{
    void *pdata, *retval;

    if (!alloc || !alloc->malloc)
        return generic_malloc(size);

    pdata = alloc->pdata;
    retval = alloc->malloc(size, pdata);

    return retval;
}

export __malloc void *
bfdev_zalloc(const struct bfdev_alloc *alloc, size_t size)
{
    void *pdata, *retval;

    if (!alloc || !alloc->malloc)
        return generic_malloc(size);

    pdata = alloc->pdata;
    retval = alloc->malloc(size, pdata);

    if (likely(retval))
        memset(retval, 0, size);

    return retval;
}

export __malloc void *
bfdev_realloc(const struct bfdev_alloc *alloc, const void *block, size_t resize)
{
    void *pdata, *retval;

    if (!alloc || !alloc->realloc)
        return generic_realloc(block, resize);

    pdata = alloc->pdata;
    retval = alloc->realloc(block, resize, pdata);

    return retval;
}

export void
bfdev_free(const struct bfdev_alloc *alloc, const void *block)
{
    void *pdata;

    if (!alloc || !alloc->free)
        generic_free(block);

    pdata = alloc->pdata;
    alloc->free(block, pdata);
}
