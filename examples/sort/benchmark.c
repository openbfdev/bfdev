/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "sort-benchmark"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdlib.h>
#include <time.h>
#include <bfdev/sort.h>
#include <bfdev/log.h>
#include "../time.h"

#define TEST_LOOP 3
#define TEST_SIZE 10000

#define GENERIC_TEST(name, size)                    \
for (count = 0; count < TEST_LOOP; ++count) {       \
    unsigned int index;                             \
                                                    \
    for (index = 0; index < size; ++index)          \
        buffer[index] = rand();                     \
                                                    \
    EXAMPLE_TIME_LOOP(&loop, 1000,                  \
        bfdev_sort(buffer, size, sizeof(*buffer),   \
                    test_cmp, NULL);                \
        0;                                          \
    );                                              \
                                                    \
    bfdev_log_info("sort " name " %u: %uops/s\n",   \
                   count, loop);                    \
}

static long
test_cmp(const void *node1, const void *node2, void *pdata)
{
    const int *test1, *test2;

    test1 = node1;
    test2 = node2;

    return *test1 < *test2 ? -1 : 1;
}

int
main(int argc, const char *argv[])
{
    unsigned int count, loop;
    int *buffer;

    buffer = malloc(sizeof(int) * TEST_SIZE);
    if (!buffer)
        return 1;

    srand(time(NULL));
    GENERIC_TEST("1k", 1000)
    GENERIC_TEST("5k", 5000)
    GENERIC_TEST("10k", 10000)
    free(buffer);

    return 0;
}
