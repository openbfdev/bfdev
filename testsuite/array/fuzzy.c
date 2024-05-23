/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#include <testsuite.h>

#define TEST_CELLS 16
#define TEST_LOOP 4096
#define TEST_SIZE 65536

TESTSUITE(
    "array:resize", NULL, NULL,
    "array resize fuzzy test"
) {
    BFDEV_DEFINE_ARRAY(array, NULL, 1);
    unsigned int count;
    size_t size;
    int retval;

    retval = 0;
    for (count = 0; count < TEST_LOOP; ++count) {
        /* Not allow zero: TEST_SIZE ~ TEST_SIZE*2-1 */
        size = TEST_SIZE + rand() % TEST_SIZE;
        retval = array_resize(&array, size);
        if (retval)
            break;
    }

    bfdev_array_release(&array);

    return retval;
}

TESTSUITE(
    "array:apply", NULL, NULL,
    "array apply fuzzy test"
) {
    bfdev_array_t array;
    unsigned int count, cells;
    int retval;

    retval = 0;
    for (cells = 1; cells < TEST_CELLS; ++cells) {
        bfdev_array_init(&array, NULL, cells);

        for (count = 0; count < TEST_LOOP; ++count) {
            /* Allow zero: 0 ~ TEST_SIZE-1 */
            retval = array_apply(&array, rand() % TEST_SIZE);
            if (retval)
                break;
        }

        bfdev_array_release(&array);
    }

    return retval;
}
