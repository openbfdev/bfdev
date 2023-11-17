/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/guards.h>

BFDEV_CLEAN_TEMPLATE(malloc, void *,
    if (_T) {
        printf("cleanup %p\n", _T);
        free(_T);
    }
)

static void
test_gc_cleanup(void)
{
    bfdev_clean(malloc) void *block;
    block = malloc(8);
    (void)block;
}

static void *
test_gc_lasting(void)
{
    bfdev_clean(malloc) void *block;
    block = malloc(8);
    bfdev_clean_return(block);
}

int main(int argc, const char *argv[])
{
    void *block;

    test_gc_cleanup();
    block = test_gc_lasting();
    free(block);

    return 0;
}
