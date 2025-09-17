/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/guards.h>

BFDEV_DEFINE_CLEAN(malloc, void *,
    if (_T) {
        printf("cleanup %p\n", _T);
        free(_T);
    }
)

static void
test_clean_gc(void)
{
    BFDEV_CLEAN(malloc) void *block;
    block = malloc(8);
    (void)block;
}

static void *
test_clean_keep(void)
{
    BFDEV_CLEAN(malloc) void *block;
    block = malloc(8);
    return bfdev_taken(block);
}

int
main(int argc, const char *argv[])
{
    void *block;

    test_clean_gc();
    block = test_clean_keep();
    free(block);

    return 0;
}
