/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 */

#include <bfdev/allocator.h>
#include <export.h>

export void *
bfdev_galloc_malloc(size_t size, void *pdata)
{
    return malloc(size);
}

export void *
bfdev_galloc_realloc(const void *block, size_t resize, void *pdata)
{
    return realloc((void *)block, resize);
}

export void
bfdev_galloc_free(const void *block, void *pdata)
{
    free((void *)block);
}

export const struct bfdev_alloc bfdev_galloc = {
    .malloc = bfdev_galloc_malloc,
    .realloc = bfdev_galloc_realloc,
    .free = bfdev_galloc_free,
};
