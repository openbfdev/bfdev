/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/skiplist.h>

#define TEST_LOOP 100
#define TEST_LEVEL 32

struct skiplist_test {
    struct skip_head *head;
    uintptr_t values[TEST_LOOP];
};

static long skiplist_test_cmp(const void *nodea, const void *nodeb)
{
    uintptr_t valuea = (uintptr_t)nodea;
    uintptr_t valueb = (uintptr_t)nodeb;
    if (valuea == valueb) return 0;
    return valuea > valueb ? 1 : -1;
}

static int skiplist_test_testing(struct skiplist_test *test)
{
    unsigned int count;
    uintptr_t value;
    int retval;

    skiplist_reset(test->head, NULL);
    for (count = 0; count < TEST_LOOP; ++count)
        test->values[count] = rand() | 1;

    for (count = 0; count < TEST_LOOP; ++count) {
        retval = skiplist_insert(test->head,
                 (void *)test->values[count], skiplist_test_cmp);
        printf("skiplist insert test%02d: %#010lx ret %d\n",
               count, test->values[count], retval);
        if (retval)
            return retval;
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        value = (uintptr_t)skiplist_find(test->head,
                (void *)test->values[count], skiplist_test_cmp);
        printf("skiplist find test%02d: %#010lx ret %#010lx\n",
               count, test->values[count], value);
        if (!value)
            return -BFDEV_ENOENT;
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        skiplist_delete(test->head, (void *)test->values[count],
                        skiplist_test_cmp);
        printf("skiplist delete test%02d\n", count);
    }

    return 0;
}

int main(void)
{
    struct skiplist_test *test;
    int retval;

    test = malloc(sizeof(struct skiplist_test));
    if (unlikely(!test))
        return -BFDEV_ENOMEM;

    test->head = skiplist_create(TEST_LEVEL);
    if (unlikely(!test->head)) {
        free(test);
        return -BFDEV_ENOMEM;
    }

    retval = skiplist_test_testing(test);
    skiplist_destroy(test->head, NULL);
    free(test);

    return retval;
}
