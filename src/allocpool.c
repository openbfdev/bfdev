/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/allocpool.h>
#include <export.h>

export void *
bfdev_allocpool_alloc(struct bfdev_allocpool *pool, size_t size, size_t align)
{
    uintptr_t offset;
    void *retval;

    if (unlikely(!size))
        return NULL;

    align = bfdev_align_high(align ?: 1, sizeof(retval));
    retval = bfdev_align_ptr_high(pool->block + pool->last, align);
    offset = (retval - pool->block) + size;

    if (unlikely(offset > pool->size))
        return NULL;

    pool->last = offset;
    pool->count++;

    return retval;
}

export void
bfdev_allocpool_free(struct bfdev_allocpool *pool)
{
    if (!--pool->count)
        pool->last = 0;
}

export void
bfdev_allocpool_setup(struct bfdev_allocpool *pool, void *block, size_t size)
{
    pool->block = block;
    pool->size = size;
    pool->last = 0;
    pool->count = 0;
}
