/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 */

#include <base.h>
#include <bfdev/allocator.h>
#include <export.h>

static __bfdev_always_inline void *
generic_alloc(size_t size)
{
    return malloc(size);
}

static __bfdev_always_inline void *
generic_zalloc(size_t size)
{
    return calloc(1, size);
}

static __bfdev_always_inline void *
generic_realloc(const void *block, size_t resize)
{
    return realloc((void *)block, resize);
}

static __bfdev_always_inline void
generic_free(const void *block)
{
    free((void *)block);
}

static inline const bfdev_alloc_ops_t *
alloc_check(const bfdev_alloc_t *alloc)
{
    if (!alloc)
        return NULL;
    return alloc->ops;
}

export __bfdev_malloc void *
bfdev_malloc(const bfdev_alloc_t *alloc, size_t size)
{
    const bfdev_alloc_ops_t *ops;
    void *pdata, *retval;

    if (bfdev_unlikely(!size))
        return NULL;

    ops = alloc_check(alloc);
    if (!ops || !ops->alloc)
        retval = generic_alloc(size);
    else {
        pdata = alloc->pdata;
        retval = ops->alloc(size, pdata);
    }

    return retval;
}

export __bfdev_malloc void *
bfdev_zalloc(const bfdev_alloc_t *alloc, size_t size)
{
    const bfdev_alloc_ops_t *ops;
    void *pdata, *retval;

    if (bfdev_unlikely(!size))
        return NULL;

    ops = alloc_check(alloc);
    if (!ops || !ops->zalloc)
        retval = generic_zalloc(size);
    else {
        pdata = alloc->pdata;
        retval = ops->zalloc(size, pdata);
    }

    return retval;
}

export __bfdev_malloc void *
bfdev_realloc(const bfdev_alloc_t *alloc, const void *block, size_t resize)
{
    const bfdev_alloc_ops_t *ops;
    void *pdata, *retval;

    if (!block)
        return bfdev_malloc(alloc, resize);

    if (!resize) {
        bfdev_free(alloc, block);
        return NULL;
    }

    ops = alloc_check(alloc);
    if (!ops || !ops->realloc)
        retval = generic_realloc((void *)block, resize);
    else {
        pdata = alloc->pdata;
        retval = ops->realloc((void *)block, resize, pdata);
    }

    return retval;
}

export void
bfdev_free(const bfdev_alloc_t *alloc, const void *block)
{
    const bfdev_alloc_ops_t *ops;
    void *pdata;

    if (!block)
        return;

    ops = alloc_check(alloc);
    if (!ops || !ops->free)
        generic_free(block);
    else {
        pdata = alloc->pdata;
        ops->free((void *)block, pdata);
    }
}
