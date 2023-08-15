/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 */

#include <bfdev.h>
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

static inline const struct bfdev_alloc_ops *
alloc_check(const struct bfdev_alloc *alloc)
{
    if (!alloc)
        return NULL;
    return alloc->ops;
}

export __bfdev_malloc void *
bfdev_malloc(const struct bfdev_alloc *alloc, size_t size)
{
    const struct bfdev_alloc_ops *ops;
    void *pdata, *retval;

    if (!(ops = alloc_check(alloc)) || !ops->malloc)
        retval = generic_malloc(size);
    else {
        pdata = alloc->pdata;
        retval = ops->malloc(size, pdata);
    }

    return retval;
}

export __bfdev_malloc void *
bfdev_zalloc(const struct bfdev_alloc *alloc, size_t size)
{
    const struct bfdev_alloc_ops *ops;
    void *pdata, *retval;

    if (!(ops = alloc_check(alloc)) || !ops->malloc)
        retval = generic_malloc(size);
    else {
        pdata = alloc->pdata;
        retval = ops->malloc(size, pdata);
    }

    if (likely(retval))
        memset(retval, 0, size);

    return retval;
}

export __bfdev_malloc void *
bfdev_realloc(const struct bfdev_alloc *alloc, const void *block, size_t resize)
{
    const struct bfdev_alloc_ops *ops;
    void *pdata, *retval;

    if (!(ops = alloc_check(alloc)) || !ops->realloc)
        retval = generic_realloc(block, resize);
    else {
        pdata = alloc->pdata;
        retval = ops->realloc(block, resize, pdata);
    }

    return retval;
}

export void
bfdev_free(const struct bfdev_alloc *alloc, const void *block)
{
    const struct bfdev_alloc_ops *ops;
    void *pdata;

    if (!(ops = alloc_check(alloc)) || !ops->free)
        generic_free(block);
    else {
        pdata = alloc->pdata;
        ops->free(block, pdata);
    }
}
