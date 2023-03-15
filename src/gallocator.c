/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 */

#include <bfdev/allocator.h>

static void *generic_malloc(size_t size, void *dummy)
{
    (void)dummy;
    return malloc(size);
}

static void *generic_realloc(void *ptr, unsigned long size, void *dummy)
{
    (void)dummy;
    return realloc(ptr, size);
}

static void generic_free(void *ptr, void *dummy)
{
    (void)dummy;
    free(ptr);
}

static allocator_t gallocator = {
    .malloc = generic_malloc,
    .realloc = generic_realloc,
    .free = generic_free,
    .pdata = NULL,
};

allocator_t *gallocator_create()
{
    return &gallocator;
}
