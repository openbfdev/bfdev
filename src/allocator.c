/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 */

#include <base.h>
#include <bfdev/allocator.h>
#include <export.h>

export bfdev_alloc_t
bfdev_alloc_default;

export bfdev_alloc_ops_t
bfdev_alloc_default_ops;

#define __INSIDE_ALLOCATOR__
#include <port/allocator.h>

static inline const bfdev_alloc_ops_t *
alloc_ops(const bfdev_alloc_t *alloc, void **pdata)
{
    const bfdev_alloc_ops_t *ops;

    if (!alloc)
        alloc = &bfdev_alloc_default;
    *pdata = alloc->pdata;

    if (alloc->ops)
        ops = alloc->ops;
    else
        ops = &bfdev_alloc_default_ops;

    return ops;
}

export __bfdev_malloc void *
bfdev_malloc(const bfdev_alloc_t *alloc, size_t size)
{
    const bfdev_alloc_ops_t *ops;
    void *pdata, *retval;

    if (bfdev_unlikely(!size))
        return NULL;

    ops = alloc_ops(alloc, &pdata);
    if (!ops->alloc)
        retval = generic_alloc(size, pdata);
    else
        retval = ops->alloc(size, pdata);

    return retval;
}

export __bfdev_malloc void *
bfdev_zalloc(const bfdev_alloc_t *alloc, size_t size)
{
    const bfdev_alloc_ops_t *ops;
    void *pdata, *retval;

    if (bfdev_unlikely(!size))
        return NULL;

    ops = alloc_ops(alloc, &pdata);
    if (!ops->zalloc)
        retval = generic_zalloc(size, pdata);
    else
        retval = ops->zalloc(size, pdata);

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

    ops = alloc_ops(alloc, &pdata);
    if (!ops->realloc)
        retval = generic_realloc((void *)block, resize, pdata);
    else
        retval = ops->realloc((void *)block, resize, pdata);

    return retval;
}

export void
bfdev_free(const bfdev_alloc_t *alloc, const void *block)
{
    const bfdev_alloc_ops_t *ops;
    void *pdata;

    if (!block)
        return;

    ops = alloc_ops(alloc, &pdata);
    if (!ops->free)
        generic_free((void *)block, pdata);
    else
        ops->free((void *)block, pdata);
}
