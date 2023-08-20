/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bfdev/array.h>

#define TEST_LOOP 100
#define TEST_SIZE 64
BFDEV_DEFINE_ARRAY(test_array, NULL, TEST_SIZE);

int main(void)
{
    unsigned int count;
    void *array;

    for (count = 0; count < TEST_LOOP; ++count) {
        unsigned int num;

        num = rand() % TEST_SIZE;
        array = bfdev_array_push(&test_array, num);
        memset(array, 0, TEST_SIZE * num);

        printf("array bfdev_array_push test: %02u: %u\n",
                count, num);
    }

    bfdev_array_release(&test_array);
    return 0;
}
