/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "radix-benchmark"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/log.h>
#include <bfdev/radix.h>
#include "../time.h"

#define TEST_LOOP 1000000
BFDEV_RADIX_ROOT(bench_root, unsigned int, NULL);

int
main(int argc, const char *argv[])
{
    unsigned int count, *data;
    int retval;

    bfdev_log_info("Generate %u node:\n", TEST_LOOP);
    data = malloc(sizeof(*data) * TEST_LOOP);
    if (!data) {
        bfdev_log_err("Insufficient memory!\n");
        return 1;
    }

    srand(time(NULL));
    for (count = 0; count < TEST_LOOP; ++count)
        data[count] = rand();

    retval = bfdev_radix_charge(&bench_root, 0, TEST_LOOP);
    if (retval) {
        bfdev_log_err("Insufficient memory!\n");
        return retval;
    }

    bfdev_log_info("Alloc Nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_LOOP; ++count) {
            unsigned int *block;
            block = bfdev_radix_alloc(&bench_root, count);
            *block = data[count];
        }
        0;
    );

    bfdev_log_info("Find Nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_LOOP; ++count) {
            unsigned int *block;
            block = bfdev_radix_find(&bench_root, count);
            if (!block || *block != data[count]) {
                bfdev_log_info("Data verification failed!\n");
                return 1;
            }
        }
        0;
    );

    bfdev_log_info("Free Nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_LOOP; ++count) {
            retval = bfdev_radix_free(&bench_root, count);
            if (retval) {
                bfdev_log_info("Sequence error!\n");
                return 1;
            }
        }
        0;
    );

    count = bench_root.tree.level;
    bfdev_log_info("\tradix level: %u\n", count);
    if (count) {
        bfdev_log_info("Pruning error!\n");
        return 1;
    }

    bfdev_log_info("Done.\n");
    bfdev_radix_release(&bench_root);
    free(data);

    return 0;
}
