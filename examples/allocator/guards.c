/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <string.h>
#include <bfdev/allocator.h>
#include <bfdev/size.h>

#define TEST_SIZE BFDEV_SZ_64KiB

static int
test_cleanup(void)
{
    BFDEV_CLEAN(bfdev_free) void *ptr;

    ptr = bfdev_malloc(NULL, TEST_SIZE);
    if (!ptr)
        return 1;
    bzero(ptr, TEST_SIZE);

    return 0;
}

static int
test_class_malloc(void)
{
    BFDEV_CLASS(bfdev_malloc, ptr)(NULL, TEST_SIZE);

    if (!ptr)
        return 1;
    bzero(ptr, TEST_SIZE);

    return 0;
}

static int
test_class_zalloc(void)
{
    BFDEV_CLASS(bfdev_zalloc, ptr)(NULL, TEST_SIZE);

    if (!ptr)
        return 1;
    bzero(ptr, TEST_SIZE);

    return 0;
}

int
main(int argc, char const *argv[])
{
    int retval;

    if ((retval = test_cleanup()) ||
        (retval = test_class_malloc()) ||
        (retval = test_class_zalloc()))
        return retval;

    return 0;
}
