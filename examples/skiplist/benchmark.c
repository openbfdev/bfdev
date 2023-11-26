/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "skiplist-benchmark"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/log.h>
#include <bfdev/skiplist.h>
#include "../time.h"

#define TEST_DEPTH 32
#define TEST_LEN 1000000

static long
test_cmp(const void *node1, const void *node2, void *pdata)
{
    uintptr_t valuea, valueb;

    valuea = (uintptr_t)node1;
    valueb = (uintptr_t)node2;

    if (valuea == valueb)
        return 0;

    return valuea > valueb ? 1 : -1;
}

static long
test_find(const void *node, void *pdata)
{
    uintptr_t valuea, valueb;

    valuea = (uintptr_t)node;
    valueb = (uintptr_t)pdata;

    if (valuea == valueb)
        return 0;

    return valuea > valueb ? 1 : -1;
}

int main(int argc, const char *argv[])
{
    struct bfdev_skip_head *head;
    struct bfdev_skip_node *node;
    uintptr_t value, *record;
    unsigned int count;

    record = malloc(TEST_LEN * sizeof(*record));
    if (!record)
        return 1;

    head = bfdev_skiplist_create(NULL, TEST_DEPTH);
    if (!head)
        return 1;

    srand(time(NULL));
    bfdev_log_info("Insert %u node:\n", TEST_LEN);
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_LEN; ++count) {
            value = ((uint64_t)rand() << 32) | rand();
            record[count] = value;
            retval = bfdev_skiplist_insert(head, (void *)value, test_cmp, NULL);
            if (retval)
                return 1;
        }
        0;
    );

    bfdev_log_info("Find %u node:\n", TEST_LEN);
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_LEN; ++count) {
            value = record[(unsigned long)rand() % TEST_LEN];
            node = bfdev_skiplist_find(head, test_find, (void *)value);
            if (!node)
                return 1;
        }
        0;
    );

    bfdev_skiplist_destroy(head, NULL);
    free(record);

    return 0;
}
