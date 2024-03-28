/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#include <testsuite.h>

#define TEST_LOOP 4096
#define TEST_SIZE 65536

TESTSUITE(
    "array:resize", NULL, NULL,
    "array resize fuzzy test"
) {
    BFDEV_DEFINE_ARRAY(array, NULL, 1);
    unsigned int count;
    int retval;

    retval = 0;
    for (count = 0; count < TEST_LOOP; ++count) {
        retval = array_resize(&array, rand() % TEST_SIZE);
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
    BFDEV_DEFINE_ARRAY(array, NULL, 1);
    unsigned int count;
    int retval;

    retval = 0;
    for (count = 0; count < TEST_LOOP; ++count) {
        retval = array_apply(&array, rand() % TEST_SIZE);
        if (retval)
            break;
    }

    bfdev_array_release(&array);

    return retval;
}
