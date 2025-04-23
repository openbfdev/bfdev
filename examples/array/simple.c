/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bfdev/array.h>

#define TEST_LOOP 100
#define TEST_SIZE 64

int
main(int argc, const char *argv[])
{
    BFDEV_DEFINE_ARRAY(array, NULL, TEST_SIZE);
    unsigned int count;

    bfdev_array_append(&array, 0, 0);

    for (count = 0; count < TEST_LOOP; ++count) {
        unsigned int num;
        void *buff;

        num = rand() % TEST_SIZE;
        buff = bfdev_array_push(&array, num);
        if (!buff)
            return 1;

        memset(buff, 0, TEST_SIZE * num);
        printf("array bfdev_array_push test: %02u: %u\n", count, num);
    }

    bfdev_array_release(&array);

    return 0;
}
