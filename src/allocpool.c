/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Fredrik Anderson <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/allocpool.h>
#include <export.h>

export __bfdev_malloc void *
bfdev_allocpool_alloc(bfdev_allocpool_t *pool, size_t size, size_t align)
{
    uintptr_t offset;
    void *retval;

    if (bfdev_unlikely(!size))
        return NULL;

    align = bfdev_align_high(align ?: 1, sizeof(retval));
    retval = bfdev_align_ptr_high(pool->block + pool->last, align);
    offset = (retval - pool->block) + size;

    if (bfdev_unlikely(offset > pool->size))
        return NULL;

    pool->last = offset;
    pool->count++;

    return retval;
}

export void
bfdev_allocpool_free(bfdev_allocpool_t *pool, const void *block)
{
    if (!--pool->count)
        pool->last = 0;
}
