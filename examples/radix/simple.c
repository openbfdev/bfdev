/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/radix.h>

#define TEST_LOOP 128

static
BFDEV_DECLARE_RADIX(root, unsigned int);

int
main(int argc, const char *argv[])
{
    unsigned int count, *block;
    int retval;

    root = BFDEV_RADIX_INIT(&root, NULL);
    for (count = 0; count < TEST_LOOP; ++count) {
        block = bfdev_radix_alloc(&root, count);
        printf("radix 'bfdev_radix_insert' test%02u: ", count);
        if (!block) {
            printf("failed\n");
            return 1;
        }
        *block = count;
        printf("passed\n");
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        block = bfdev_radix_find(&root, count);
        printf("radix 'bfdev_radix_find' test%02u: ", count);
        if (!block || count != *block) {
            printf("failed\n");
            return 1;
        }
        printf("passed\n");
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        retval = bfdev_radix_free(&root, count);
        printf("radix 'bfdev_radix_delete' test%02u: ", count);
        if (retval) {
            printf("failed\n");
            return retval;
        }
        printf("passed\n");
    }

    bfdev_radix_release(&root);
    return 0;
}
