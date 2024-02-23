/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "sort-benchmark"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <bfdev/sort.h>
#include <bfdev/log.h>

#define TEST_LOOP 100

struct sort_test {
    unsigned int index;
    unsigned int value;
};

static long
sort_test_cmp(const void *nodea, const void *nodeb, void *pdata)
{
    const struct sort_test *testa = nodea;
    const struct sort_test *testb = nodeb;
    return testa->value > testb->value ? 1 : -1;
}

static int
sort_testing(struct sort_test *test)
{
    unsigned int count;

    srand(time(NULL));
    for (count = 0; count < TEST_LOOP; ++count) {
        test[count].index = count;
        test[count].value = (unsigned int)rand();
        bfdev_log_info("qsort generating: %02d = %u\n",
                       count, test[count].value);
    }

    bfdev_sort(test, TEST_LOOP, sizeof(*test), sort_test_cmp, NULL);
    for (count = 0; count < TEST_LOOP; ++count) {
        bfdev_log_info("sort qsort%02u: %02u = %u\n",
                       count, test[count].index, test[count].value);

        if (count && test[count].value < test[count - 1].value) {
            bfdev_log_err("sort failed\n");
            return 1;
        }
    }

    return 0;
}

int main(int argc, const char *argv[])
{
    struct sort_test *test;
    int retval;

    test = malloc(sizeof(struct sort_test) * TEST_LOOP);
    if (!test)
        return 1;

    retval = sort_testing(test);
    free(test);

    return retval;
}
