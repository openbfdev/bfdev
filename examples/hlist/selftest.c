/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/bfdev.h>
#include <bfdev/hlist.h>

#define TEST_LOOP 10

struct hlist_test_node {
    struct hlist_node list;
    unsigned long num;
};

struct hlist_test_pdata {
    struct hlist_test_node nodes[TEST_LOOP];
};

#define hlist_to_test(ptr) \
    hlist_entry(ptr, struct hlist_test_node, list)

static int hlist_selftest(struct hlist_test_pdata *hdata)
{
    struct hlist_test_node *node, *nnode, *tnode;
    struct hlist_node *list, *nlist, *tlist;
    unsigned int count;

    HLIST_HEAD(test_head);

    for (count = 0; count < ARRAY_SIZE(hdata->nodes); ++count) {
        hlist_head_add(&test_head, &hdata->nodes[count].list);
    }

    hlist_for_each(list, &test_head) {
        node = hlist_to_test(list);
        printf("hlist 'hlist_for_each' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    hlist_for_each_continue(list) {
        node = hlist_to_test(list);
        printf("hlist 'hlist_for_each_continue' test: %lu\n", node->num);
    }

    list = tlist;
    hlist_for_each_from(list) {
        node = hlist_to_test(list);
        printf("hlist 'hlist_for_each_from' test: %lu\n", node->num);
    }

    hlist_for_each_safe(list, nlist, &test_head) {
        node = hlist_to_test(list);
        printf("hlist 'hlist_for_each_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    hlist_for_each_continue_safe(list, nlist) {
        node = hlist_to_test(list);
        printf("hlist 'hlist_for_each_continue_safe' test: %lu\n", node->num);
    }

    list = tlist;
    hlist_for_each_from_safe(list, nlist) {
        node = hlist_to_test(list);
        printf("hlist 'hlist_for_each_from_safe' test: %lu\n", node->num);
    }

    hlist_for_each_entry(node, &test_head, list) {
        printf("hlist 'hlist_for_each_entry' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    hlist_for_each_entry_continue(node, list) {
        printf("hlist 'hlist_for_each_entry_continue' test: %lu\n", node->num);
    }

    node = tnode;
    hlist_for_each_entry_from(node, list) {
        printf("hlist 'hlist_for_each_entry_from' test: %lu\n", node->num);
    }

    hlist_for_each_entry_safe(node, nnode, &test_head, list) {
        printf("hlist 'hlist_for_each_entry_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
        hlist_del(&node->list);
    }

    tnode = node;
    hlist_for_each_entry_continue_safe(node, nnode, list) {
        printf("hlist 'hlist_for_each_entry_continue_safe' test: %lu\n", node->num);
    }

    node = tnode;
    hlist_for_each_entry_from_safe(node, nnode, list) {
        printf("hlist 'hlist_for_each_entry_from_safe' test: %lu\n", node->num);
        hlist_del(&node->list);
    }

    return 0;
}

int main(void)
{
    struct hlist_test_pdata *hdata;
    unsigned int count;
    int retval;

    hdata = malloc(sizeof(struct hlist_test_pdata));
    if (!hdata)
        return -1;

    for (count = 0; count < ARRAY_SIZE(hdata->nodes); ++count)
        hdata->nodes[count].num = TEST_LOOP - count - 1;

    retval = hlist_selftest(hdata);
    free(hdata);

    return retval;
}
