/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bfdev/guards.h>

BFDEV_DEFINE_CLEAN(malloc, void *,
    if (_T) {
        printf("cleanup %p\n", _T);
        free(_T);
    }
)

static void
test_malloc_gc(void)
{
    BFDEV_CLEAN(malloc) void *block;
    block = malloc(8);
    (void)block;
}

static void *
test_malloc_keep(void)
{
    BFDEV_CLEAN(malloc) void *block;
    block = malloc(8);
    return bfdev_taken_ptr(block);
}

int
main(int argc, const char *argv[])
{
    void *block;

    test_malloc_gc();
    block = test_malloc_keep();
    free(block);

    return 0;
}
