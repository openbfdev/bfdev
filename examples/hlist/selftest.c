/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/macro.h>
#include <bfdev/hlist.h>

#define TEST_LOOP 10

struct test_node {
    struct bfdev_hlist_node list;
    unsigned long num;
};

struct test_pdata {
    struct test_node nodes[TEST_LOOP];
};

#define bfdev_hlist_to_test(ptr) \
    bfdev_hlist_entry(ptr, struct test_node, list)

static int bfdev_hlist_selftest(struct test_pdata *hdata)
{
    struct test_node *node, *nnode, *tnode;
    struct bfdev_hlist_node *list, *nlist, *tlist;
    unsigned int count;

    BFDEV_HLIST_HEAD(test_head);

    for (count = 0; count < ARRAY_SIZE(hdata->nodes); ++count) {
        bfdev_hlist_head_add(&test_head, &hdata->nodes[count].list);
    }

    bfdev_hlist_for_each(list, &test_head) {
        node = bfdev_hlist_to_test(list);
        printf("hlist 'bfdev_hlist_for_each' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    bfdev_hlist_for_each_continue(list) {
        node = bfdev_hlist_to_test(list);
        printf("hlist 'bfdev_hlist_for_each_continue' test: %lu\n", node->num);
    }

    list = tlist;
    bfdev_hlist_for_each_from(list) {
        node = bfdev_hlist_to_test(list);
        printf("hlist 'bfdev_hlist_for_each_from' test: %lu\n", node->num);
    }

    bfdev_hlist_for_each_safe(list, nlist, &test_head) {
        node = bfdev_hlist_to_test(list);
        printf("hlist 'bfdev_hlist_for_each_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    bfdev_hlist_for_each_continue_safe(list, nlist) {
        node = bfdev_hlist_to_test(list);
        printf("hlist 'bfdev_hlist_for_each_continue_safe' test: %lu\n", node->num);
    }

    list = tlist;
    bfdev_hlist_for_each_from_safe(list, nlist) {
        node = bfdev_hlist_to_test(list);
        printf("hlist 'bfdev_hlist_for_each_from_safe' test: %lu\n", node->num);
    }

    bfdev_hlist_for_each_entry(node, &test_head, list) {
        printf("hlist 'bfdev_hlist_for_each_entry' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_hlist_for_each_entry_continue(node, list) {
        printf("hlist 'bfdev_hlist_for_each_entry_continue' test: %lu\n", node->num);
    }

    node = tnode;
    bfdev_hlist_for_each_entry_from(node, list) {
        printf("hlist 'bfdev_hlist_for_each_entry_from' test: %lu\n", node->num);
    }

    bfdev_hlist_for_each_entry_safe(node, nnode, &test_head, list) {
        printf("hlist 'bfdev_hlist_for_each_entry_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
        bfdev_hlist_del(&node->list);
    }

    tnode = node;
    bfdev_hlist_for_each_entry_continue_safe(node, nnode, list) {
        printf("hlist 'bfdev_hlist_for_each_entry_continue_safe' test: %lu\n", node->num);
    }

    node = tnode;
    bfdev_hlist_for_each_entry_from_safe(node, nnode, list) {
        printf("hlist 'bfdev_hlist_for_each_entry_from_safe' test: %lu\n", node->num);
        bfdev_hlist_del(&node->list);
    }

    return 0;
}

int main(void)
{
    struct test_pdata *hdata;
    unsigned int count;
    int retval;

    hdata = malloc(sizeof(struct test_pdata));
    if ((retval = !hdata))
        return retval;

    for (count = 0; count < ARRAY_SIZE(hdata->nodes); ++count)
        hdata->nodes[count].num = TEST_LOOP - count - 1;

    retval = bfdev_hlist_selftest(hdata);
    free(hdata);

    return retval;
}
