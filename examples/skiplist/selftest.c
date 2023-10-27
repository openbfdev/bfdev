/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <bfdev/skiplist.h>

#define TEST_LOOP 100
#define TEST_LEVEL 32

struct test_node {
    struct bfdev_skip_head *head;
    uintptr_t values[TEST_LOOP];
};

static long
test_cmp(const void *nodea, const void *nodeb, void *pdata)
{
    uintptr_t valuea, valueb;

    valuea = (uintptr_t)nodea;
    valueb = (uintptr_t)nodeb;

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

static int skiplist_test_testing(struct test_node *test)
{
    unsigned int count;
    uintptr_t value;
    int retval;

    bfdev_skiplist_reset(test->head, NULL);
    for (count = 0; count < TEST_LOOP; ++count)
        test->values[count] = rand() | 1;

    for (count = 0; count < TEST_LOOP; ++count) {
        retval = bfdev_skiplist_insert(
            test->head, (void *)test->values[count],
            test_cmp, NULL
        );
        printf("skiplist insert test%02d: %#010lx ret %d\n",
               count, (unsigned long)test->values[count], retval);
        if (retval)
            return retval;
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        value = (uintptr_t)bfdev_skiplist_find(
            test->head,
            test_find, (void *)test->values[count]
        );
        printf("skiplist find test%02d: %#010lx ret %#010lx\n",
               count, (unsigned long)test->values[count], (unsigned long)value);
        if (!value)
            return -BFDEV_ENOENT;
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        bfdev_skiplist_delete(
            test->head,
            test_find, (void *)test->values[count]
        );
        printf("skiplist delete test%02d\n", count);
    }

    return 0;
}

int main(void)
{
    struct test_node *test;
    int retval;

    test = malloc(sizeof(struct test_node));
    if (!test)
        return -BFDEV_ENOMEM;

    test->head = bfdev_skiplist_create(NULL, TEST_LEVEL);
    if (!test->head) {
        free(test);
        return -BFDEV_ENOMEM;
    }

    retval = skiplist_test_testing(test);
    bfdev_skiplist_destroy(test->head, NULL);
    free(test);

    return retval;
}
