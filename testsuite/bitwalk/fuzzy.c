/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bitwalk-fuzzy"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <bfdev/macro.h>
#include <bfdev/bitwalk.h>
#include <bfdev/bitmap.h>
#include <bfdev/sort.h>
#include <bfdev/log.h>
#include <testsuite.h>

#define TEST_SMALL_LOOP 10
#define TEST_SMALL_SIZE 30

#define TEST_LARGE_LOOP 200
#define TEST_LARGE_SIZE 500

static long
index_sort(const void *key1, const void *key2, void *pdata)
{
    unsigned int index1, index2;

    index1 = *(unsigned int *)key1;
    index2 = *(unsigned int *)key2;

    return bfdev_cmp(index1 > index2);
}

static int
bitwalk_bit(unsigned int size, unsigned int loop)
{
    unsigned long *bitmap;
    unsigned int *record, count, index;
    unsigned int verify, tmp1, tmp2;
    int retval;
    bool test;

    bitmap = bfdev_bitmap_alloc(NULL, size);
    if (!bitmap)
        return -BFDEV_ENOMEM;

    record = malloc(sizeof(*record) * loop);
    if (!record) {
        bfdev_bitmap_free(NULL, bitmap);
        return -BFDEV_ENOMEM;
    }

    srand(time(NULL));
    bfdev_bitmap_zero(bitmap, size);

    for (count = 0; count < loop; ++count) {
        do {
            index = (unsigned int)rand() % size;
            test = bfdev_bit_test(bitmap, index);
        } while (test);

        record[count] = index;
        bfdev_bit_set(bitmap, index);
    }

    bfdev_sort(record, loop, sizeof(*record), index_sort, NULL);
    retval = -BFDEV_ENOERR;

    count = 0;
    verify = 0;

    bfdev_for_each_bit(index, bitmap, size) {
        bfdev_log_debug("'for_each_bit' test%u: %u\n", count, index);
        verify++;

        if (index != record[count++]) {
            retval = -BFDEV_EFAULT;
            goto failed;
        }

        if (count == loop / 2)
            break;
    }

    tmp1 = index;
    tmp2 = count;

    bfdev_for_each_bit_continue(index, bitmap, size) {
        bfdev_log_debug("'for_each_bit_continue' test%u: %u\n", count, index);
        verify++;

        if (index != record[count++]) {
            retval = -BFDEV_EFAULT;
            goto failed;
        }
    }

    index = tmp1;
    count = tmp2 - 1;

    bfdev_for_each_bit_form(index, bitmap, size) {
        bfdev_log_debug("'for_each_bit_form' test%u: %u\n", count, index);
        verify++;

        if (index != record[count++]) {
            retval = -BFDEV_EFAULT;
            goto failed;
        }
    }

    if (verify - 1 != loop + (loop / 2)) {
        retval = -BFDEV_ENODATA;
        goto failed;
    }

    count = 0;
    verify = 0;

    bfdev_for_each_bit_reverse(index, bitmap, size) {
        bfdev_log_debug("'for_each_bit_reverse' test%u: %u\n", count, index);
        verify++;

        if (index != record[loop - ++count]) {
            retval = -BFDEV_EFAULT;
            goto failed;
        }

        if (count == loop / 2)
            break;
    }

    tmp1 = index;
    tmp2 = count;

    bfdev_for_each_bit_reverse_continue(index, bitmap, size) {
        bfdev_log_debug("'for_each_bit_reverse_continue' test%u: %u\n", count, index);
        verify++;

        if (index != record[loop - ++count]) {
            retval = -BFDEV_EFAULT;
            goto failed;
        }
    }

    index = tmp1;
    count = tmp2 - 1;

    bfdev_for_each_bit_reverse_form(index, bitmap, size) {
        bfdev_log_debug("'for_each_bit_reverse_form' test%u: %u\n", count, index);
        verify++;

        if (index != record[loop - ++count]) {
            retval = -BFDEV_EFAULT;
            goto failed;
        }
    }

    if (verify - 1 != loop + (loop / 2)) {
        retval = -BFDEV_ENODATA;
        goto failed;
    }

failed:
    free(record);
    bfdev_bitmap_free(NULL, bitmap);
    return retval;
}

static int
bitwalk_zero(unsigned int size, unsigned int loop)
{
    unsigned long *bitmap;
    unsigned int *record, count, index;
    unsigned int verify, tmp1, tmp2;
    int retval;
    bool test;

    bitmap = bfdev_bitmap_alloc(NULL, size);
    if (!bitmap)
        return -BFDEV_ENOMEM;

    record = malloc(sizeof(*record) * loop);
    if (!record) {
        bfdev_bitmap_free(NULL, bitmap);
        return -BFDEV_ENOMEM;
    }

    srand(time(NULL));
    bfdev_bitmap_fill(bitmap, size);

    for (count = 0; count < loop; ++count) {
        do {
            index = (unsigned int)rand() % size;
            test = bfdev_bit_test(bitmap, index);
        } while (!test);

        record[count] = index;
        bfdev_bit_clr(bitmap, index);
    }

    bfdev_sort(record, loop, sizeof(*record), index_sort, NULL);
    retval = -BFDEV_ENOERR;

    count = 0;
    verify = 0;

    bfdev_for_each_zero(index, bitmap, size) {
        bfdev_log_debug("'for_each_zero' test%u: %u\n", count, index);
        verify++;

        if (index != record[count++]) {
            retval = -BFDEV_EFAULT;
            goto failed;
        }

        if (count == loop / 2)
            break;
    }

    tmp1 = index;
    tmp2 = count;

    bfdev_for_each_zero_continue(index, bitmap, size) {
        bfdev_log_debug("'for_each_zero_continue' test%u: %u\n", count, index);
        verify++;

        if (index != record[count++]) {
            retval = -BFDEV_EFAULT;
            goto failed;
        }
    }

    index = tmp1;
    count = tmp2 - 1;

    bfdev_for_each_zero_form(index, bitmap, size) {
        bfdev_log_debug("'for_each_zero_form' test%u: %u\n", count, index);
        verify++;

        if (index != record[count++]) {
            retval = -BFDEV_EFAULT;
            goto failed;
        }
    }

    if (verify - 1 != loop + (loop / 2)) {
        retval = -BFDEV_ENODATA;
        goto failed;
    }

    count = 0;
    verify = 0;

    bfdev_for_each_zero_reverse(index, bitmap, size) {
        bfdev_log_debug("'for_each_zero_reverse' test%u: %u\n", count, index);
        verify++;

        if (index != record[loop - ++count]) {
            retval = -BFDEV_EFAULT;
            goto failed;
        }

        if (count == loop / 2)
            break;
    }

    tmp1 = index;
    tmp2 = count;

    bfdev_for_each_zero_reverse_continue(index, bitmap, size) {
        bfdev_log_debug("'for_each_zero_reverse_continue' test%u: %u\n", count, index);
        verify++;

        if (index != record[loop - ++count]) {
            retval = -BFDEV_EFAULT;
            goto failed;
        }
    }

    index = tmp1;
    count = tmp2 - 1;

    bfdev_for_each_zero_reverse_form(index, bitmap, size) {
        bfdev_log_debug("'for_each_zero_reverse_form' test%u: %u\n", count, index);
        verify++;

        if (index != record[loop - ++count]) {
            retval = -BFDEV_EFAULT;
            goto failed;
        }
    }

    if (verify - 1 != loop + (loop / 2)) {
        retval = -BFDEV_ENODATA;
        goto failed;
    }

failed:
    free(record);
    bfdev_bitmap_free(NULL, bitmap);
    return retval;
}

TESTSUITE(
    "bitwalk:bit_small", NULL, NULL,
    ""
) {
    return bitwalk_bit(TEST_SMALL_SIZE, TEST_SMALL_LOOP);
}

TESTSUITE(
    "bitwalk:bit_large", NULL, NULL,
    ""
) {
    return bitwalk_bit(TEST_LARGE_SIZE, TEST_LARGE_LOOP);
}

TESTSUITE(
    "bitwalk:zero_small", NULL, NULL,
    ""
) {
    return bitwalk_zero(TEST_SMALL_SIZE, TEST_SMALL_LOOP);
}


TESTSUITE(
    "bitwalk:zero_large", NULL, NULL,
    ""
) {
    return bitwalk_zero(TEST_LARGE_SIZE, TEST_LARGE_LOOP);
}
