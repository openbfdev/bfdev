/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 */

#include <base.h>
#include <bfdev/allocator.h>
#include <export.h>

static __bfdev_always_inline void *
generic_malloc(size_t size)
{
    return malloc(size);
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

    if (!(ops = alloc_check(alloc)) || !ops->alloc)
        retval = generic_malloc(size);
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

    if (!(ops = alloc_check(alloc)) || !ops->alloc)
        retval = generic_malloc(size);
    else {
        pdata = alloc->pdata;
        retval = ops->alloc(size, pdata);
    }

    if (bfdev_likely(retval))
        memset(retval, 0, size);

    return retval;
}

export __bfdev_malloc void *
bfdev_realloc(const bfdev_alloc_t *alloc, const void *block, size_t resize)
{
    const bfdev_alloc_ops_t *ops;
    void *pdata, *retval;

    if (!(ops = alloc_check(alloc)) || !ops->realloc)
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

    if (!(ops = alloc_check(alloc)) || !ops->free)
        generic_free(block);
    else {
        pdata = alloc->pdata;
        ops->free((void *)block, pdata);
    }
}
