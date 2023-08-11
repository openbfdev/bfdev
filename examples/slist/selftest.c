/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/macro.h>
#include <bfdev/slist.h>

#define TEST_LOOP 10

struct test_node {
    struct bfdev_slist_head list;
    unsigned long num;
};

struct test_pdata {
    struct test_node nodes[TEST_LOOP];
};

#define slist_to_test(ptr) \
    bfdev_slist_entry(ptr, struct test_node, list)

static int bfdev_slist_selftest(struct test_pdata *sdata)
{
    struct test_node *node, *nnode, *tnode;
    struct bfdev_slist_head *list, *nlist, *tlist;
    unsigned int count;

    BFDEV_SLIST_HEAD(test_head);

    for (count = 0; count < BFDEV_ARRAY_SIZE(sdata->nodes); ++count) {
        bfdev_slist_add(&test_head, &sdata->nodes[count].list);
    }

    bfdev_slist_for_each(list, &test_head) {
        node = slist_to_test(list);
        printf("slist 'bfdev_slist_for_each' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    bfdev_slist_for_each_continue(list) {
        node = slist_to_test(list);
        printf("slist 'bfdev_slist_for_each_continue' test: %lu\n", node->num);
    }

    list = tlist;
    bfdev_slist_for_each_from(list) {
        node = slist_to_test(list);
        printf("slist 'bfdev_slist_for_each_from' test: %lu\n", node->num);
    }

    bfdev_slist_for_each_safe(list, nlist, &test_head) {
        node = slist_to_test(list);
        printf("slist 'bfdev_slist_for_each_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    bfdev_slist_for_each_continue_safe(list, nlist) {
        node = slist_to_test(list);
        printf("slist 'bfdev_slist_for_each_continue_safe' test: %lu\n", node->num);
    }

    list = tlist;
    bfdev_slist_for_each_from_safe(list, nlist) {
        node = slist_to_test(list);
        printf("slist 'bfdev_slist_for_each_from_safe' test: %lu\n", node->num);
    }

    bfdev_slist_for_each_entry(node, &test_head, list) {
        printf("slist 'bfdev_slist_for_each_entry' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_slist_for_each_entry_continue(node, list) {
        printf("slist 'bfdev_slist_for_each_entry_continue' test: %lu\n", node->num);
    }

    node = tnode;
    bfdev_slist_for_each_entry_from(node, list) {
        printf("slist 'bfdev_slist_for_each_entry_from' test: %lu\n", node->num);
    }

    bfdev_slist_for_each_entry_safe(node, nnode, &test_head, list) {
        printf("slist 'bfdev_slist_for_each_entry_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
        bfdev_slist_del(&test_head, &node->list);
    }

    tnode = node;
    bfdev_slist_for_each_entry_continue_safe(node, nnode, list) {
        printf("slist 'bfdev_slist_for_each_entry_continue_safe' test: %lu\n", node->num);
    }

    node = tnode;
    bfdev_slist_for_each_entry_from_safe(node, nnode, list) {
        printf("slist 'bfdev_slist_for_each_entry_from_safe' test: %lu\n", node->num);
        bfdev_slist_del(&test_head, &node->list);
    }

    return 0;
}

int main(void)
{
    struct test_pdata *sdata;
    unsigned int count;
    int retval;

    sdata = malloc(sizeof(struct test_pdata));
    if (!sdata)
        return -1;

    for (count = 0; count < BFDEV_ARRAY_SIZE(sdata->nodes); ++count)
        sdata->nodes[count].num = TEST_LOOP - count - 1;

    retval = bfdev_slist_selftest(sdata);
    free(sdata);

    return retval;
}
