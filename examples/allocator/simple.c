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
test_realloc(void *block, size_t resize, void *pdata)
{
    return realloc((void *)block, resize);
}

static void
test_free(void *block, void *pdata)
{
    return free((void *)block);
}

BFDEV_DEFINE_ALLOC(
    test_ops,
    test_alloc,
    test_realloc,
    test_free,
    NULL
);

int main(int argc, char const *argv[])
{
    int *block;

    block = bfdev_malloc(&test_ops, sizeof(*block));
    if (!block)
        return 1;
    bfdev_free(&test_ops, block);

    block = bfdev_zalloc(&test_ops, sizeof(*block));
    if (!block)
        return 1;
    block = bfdev_realloc(&test_ops, block, sizeof(*block) * 2);
    if (!block)
        return 1;
    bfdev_free(&test_ops, block);

    return 0;
}
