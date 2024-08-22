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

#define TEST_LOOP 10
#define TEST_SIZE 1000000

static
BFDEV_DECLARE_RADIX(root, unsigned int);

int
main(int argc, const char *argv[])
{
    unsigned int count, loop, *data;
    int retval;

    bfdev_log_info("Generate %u node:\n", TEST_SIZE);
    data = malloc(sizeof(*data) * TEST_SIZE);
    if (!data) {
        bfdev_log_err("Insufficient memory!\n");
        return 1;
    }

    srand(time(NULL));
    for (count = 0; count < TEST_SIZE; ++count)
        data[count] = rand();

    root = BFDEV_RADIX_INIT(&root, NULL);
    retval = bfdev_radix_charge(&root, 0, TEST_SIZE);
    if (retval) {
        bfdev_log_err("Insufficient memory!\n");
        return retval;
    }

    bfdev_log_info("Alloc nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_SIZE; ++count) {
            unsigned int *block;
            block = bfdev_radix_alloc(&root, count);
            *block = data[count];
        }
        0;
    );

    for (loop = 0; loop < TEST_LOOP; ++loop) {
        bfdev_log_info("Find nodes loop%u...\n", loop);
        EXAMPLE_TIME_STATISTICAL(
            for (count = 0; count < TEST_SIZE; ++count) {
                unsigned int *block;
                block = bfdev_radix_find(&root, count);
                if (!block || *block != data[count]) {
                    bfdev_log_info("Data verification failed!\n");
                    return 1;
                }
            }
            0;
        );
    }

    bfdev_log_info("Free nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_SIZE; ++count) {
            retval = bfdev_radix_free(&root, count);
            if (retval) {
                bfdev_log_info("Sequence error!\n");
                return 1;
            }
        }
        0;
    );

    count = root.tree.level;
    bfdev_log_info("\tradix level: %u\n", count);
    if (count) {
        bfdev_log_info("Pruning error!\n");
        return 1;
    }

    bfdev_log_info("Done.\n");
    bfdev_radix_release(&root);
    free(data);

    return 0;
}
