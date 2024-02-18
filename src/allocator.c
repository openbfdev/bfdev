/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 */

#include <base.h>
#include <bfdev/allocator.h>
#include <export.h>

export bfdev_alloc_ops_t
bfdev_alloc_default;

static __bfdev_always_inline void *
generic_alloc(size_t size, void *pdata)
{
    if (!bfdev_alloc_default.alloc)
        return malloc(size);

    return bfdev_alloc_default.alloc(size, pdata);
}

static __bfdev_always_inline void *
generic_zalloc(size_t size, void *pdata)
{
    if (!bfdev_alloc_default.zalloc)
        return calloc(1, size);

    return bfdev_alloc_default.zalloc(size, pdata);
}

static __bfdev_always_inline void *
generic_realloc(void *block, size_t resize, void *pdata)
{
    if (!bfdev_alloc_default.realloc)
        return realloc(block, resize);

    return bfdev_alloc_default.realloc(block, resize, pdata);
}

static __bfdev_always_inline void
generic_free(void *block, void *pdata)
{
    if (!bfdev_alloc_default.free)
        return free((void *)block);

    return bfdev_alloc_default.free(block, pdata);
}

static inline const bfdev_alloc_ops_t *
alloc_check(const bfdev_alloc_t *alloc, void **pdata)
{
    if (!alloc) {
        *pdata = NULL;
        return NULL;
    }

    *pdata = alloc->pdata;
    return alloc->ops;
}

export __bfdev_malloc void *
bfdev_malloc(const bfdev_alloc_t *alloc, size_t size)
{
    const bfdev_alloc_ops_t *ops;
    void *pdata, *retval;

    if (bfdev_unlikely(!size))
        return NULL;

    ops = alloc_check(alloc, &pdata);
    if (!ops || !ops->alloc)
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

    ops = alloc_check(alloc, &pdata);
    if (!ops || !ops->zalloc)
        return generic_zalloc(size, pdata);
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

    ops = alloc_check(alloc, &pdata);
    if (!ops || !ops->realloc)
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

    ops = alloc_check(alloc, &pdata);
    if (!ops || !ops->free)
        generic_free((void *)block, pdata);
    else
        ops->free((void *)block, pdata);
}
