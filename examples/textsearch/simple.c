/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <bfdev/textsearch.h>

#define TEST_STRING "abcceabcaabcd"
#define TEST_PATTERN "abcd"
#define TEST_OFFSET 9

static int
test_textsearch(const char *algo)
{
    bfdev_ts_context_t *context;
    bfdev_ts_linear_t linear;
    unsigned int offset;

    printf("test %s algorithm: ", algo);
    context = bfdev_textsearch_create(
        NULL, algo, TEST_PATTERN,
        sizeof(TEST_PATTERN) - 1, 0
    );
    if (!context) {
        printf("failed\n");
        return 1;
    }

    offset = bfdev_textsearch_linear_find(
        context, &linear, TEST_STRING,
        sizeof(TEST_STRING) - 1
    );
    if (offset != TEST_OFFSET) {
        printf("failed\n");
        return 1;
    }

    printf("passed\n");
    bfdev_textsearch_destroy(context);

    return 0;
}

int main(int argc, const char *argv[])
{
    int retval;

    retval = test_textsearch("kmp");
    if (retval)
        return retval;

    retval = test_textsearch("bm");
    if (retval)
        return retval;

    retval = test_textsearch("sunday");
    if (retval)
        return retval;

    return 0;
}
