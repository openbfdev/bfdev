/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdlib.h>
#include <bfdev/allocator.h>

static void *
test_alloc(size_t size, void *pdata)
{
    return malloc(size);
}

static void *
test_zalloc(size_t size, void *pdata)
{
    return calloc(1, size);
}

static void *
test_realloc(void *block, size_t resize, void *pdata)
{
    return realloc((void *)block, resize);
}

static void
test_free(void *block, void *pdata)
{
    return free((void *)block);
}

BFDEV_DEFINE_ALLOC_OPS(
    test_ops,
    test_alloc, test_zalloc,
    test_realloc, test_free
);

int
main(int argc, char const *argv[])
{
    BFDEV_DEFINE_ALLOC(test, &test_ops, NULL);
    int *block;

    block = bfdev_malloc(&test, sizeof(*block));
    if (!block)
        return 1;
    bfdev_free(&test, block);

    block = bfdev_zalloc(&test, sizeof(*block));
    if (!block)
        return 1;

    block = bfdev_realloc(&test, block, sizeof(*block) * 2);
    if (!block)
        return 1;
    bfdev_free(&test, block);

    return 0;
}
