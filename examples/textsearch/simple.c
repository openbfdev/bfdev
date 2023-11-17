/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <bfdev/textsearch.h>

#define TEST_STRING "abcceabcaabcd"
#define TEST_PATTERN "abcd"
#define TEST_OFFSET 9

int main(int argc, const char *argv[])
{
    struct bfdev_ts_context *context;
    struct bfdev_ts_linear linear;
    unsigned int offset;

    printf("test bm algorithm: ");
    context = bfdev_textsearch_create(
        NULL, "bm", TEST_PATTERN,
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

    printf("test kmp algorithm: ");
    context = bfdev_textsearch_create(
        NULL, "kmp", TEST_PATTERN,
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

    printf("test sunday algorithm: ");
    context = bfdev_textsearch_create(
        NULL, "sunday", TEST_PATTERN,
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
