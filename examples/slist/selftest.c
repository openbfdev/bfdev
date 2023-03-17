/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/bfdev.h>
#include <bfdev/slist.h>

#define TEST_LOOP 10

struct slist_test_node {
    struct slist_head list;
    unsigned long num;
};

struct slist_test_pdata {
    struct slist_test_node nodes[TEST_LOOP];
};

#define slist_to_test(ptr) \
    slist_entry(ptr, struct slist_test_node, list)

static int slist_selftest(struct slist_test_pdata *sdata)
{
    struct slist_test_node *node, *nnode, *tnode;
    struct slist_head *list, *nlist, *tlist;
    unsigned int count;

    SLIST_HEAD(test_head);

    for (count = 0; count < ARRAY_SIZE(sdata->nodes); ++count) {
        slist_add(&test_head, &sdata->nodes[count].list);
    }

    slist_for_each(list, &test_head) {
        node = slist_to_test(list);
        printf("slist 'slist_for_each' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    slist_for_each_continue(list) {
        node = slist_to_test(list);
        printf("slist 'slist_for_each_continue' test: %lu\n", node->num);
    }

    list = tlist;
    slist_for_each_from(list) {
        node = slist_to_test(list);
        printf("slist 'slist_for_each_from' test: %lu\n", node->num);
    }

    slist_for_each_safe(list, nlist, &test_head) {
        node = slist_to_test(list);
        printf("slist 'slist_for_each_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    slist_for_each_continue_safe(list, nlist) {
        node = slist_to_test(list);
        printf("slist 'slist_for_each_continue_safe' test: %lu\n", node->num);
    }

    list = tlist;
    slist_for_each_from_safe(list, nlist) {
        node = slist_to_test(list);
        printf("slist 'slist_for_each_from_safe' test: %lu\n", node->num);
    }

    slist_for_each_entry(node, &test_head, list) {
        printf("slist 'slist_for_each_entry' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    slist_for_each_entry_continue(node, list) {
        printf("slist 'slist_for_each_entry_continue' test: %lu\n", node->num);
    }

    node = tnode;
    slist_for_each_entry_from(node, list) {
        printf("slist 'slist_for_each_entry_from' test: %lu\n", node->num);
    }

    slist_for_each_entry_safe(node, nnode, &test_head, list) {
        printf("slist 'slist_for_each_entry_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
        slist_del(&test_head, &node->list);
    }

    tnode = node;
    slist_for_each_entry_continue_safe(node, nnode, list) {
        printf("slist 'slist_for_each_entry_continue_safe' test: %lu\n", node->num);
    }

    node = tnode;
    slist_for_each_entry_from_safe(node, nnode, list) {
        printf("slist 'slist_for_each_entry_from_safe' test: %lu\n", node->num);
        slist_del(&test_head, &node->list);
    }

    return 0;
}

int main(void)
{
    struct slist_test_pdata *sdata;
    unsigned int count;
    int retval;

    sdata = malloc(sizeof(struct slist_test_pdata));
    if (!sdata)
        return -1;

    for (count = 0; count < ARRAY_SIZE(sdata->nodes); ++count)
        sdata->nodes[count].num = TEST_LOOP - count - 1;

    retval = slist_selftest(sdata);
    free(sdata);

    return retval;
}
