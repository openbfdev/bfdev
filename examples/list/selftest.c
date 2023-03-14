/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/bfdev.h>
#include <bfdev/list.h>

/**
 * ARRAY_SIZE - get the number of elements in array.
 * @arr: array to be sized.
 */
#define ARRAY_SIZE(arr) ( \
    sizeof(arr) / sizeof((arr)[0]) \
)

#define TEST_LOOP 10

struct list_test_node {
    struct list_head list;
    unsigned long num;
};

struct list_test_pdata {
    struct list_test_node nodes[TEST_LOOP];
};

#define list_to_test(ptr) \
    list_entry(ptr, struct list_test_node, list)

static long list_test_sort(struct list_head *nodea, struct list_head *nodeb, void *pdata)
{
    struct list_test_node *testa = list_to_test(nodea);
    struct list_test_node *testb = list_to_test(nodeb);
    return testa->num - testb->num;
}

static int list_selftest(struct list_test_pdata *ldata)
{
    struct list_test_node *node, *nnode, *tnode;
    struct list_head *list, *nlist, *tlist;
    unsigned int count;

    LIST_HEAD(test_head);

    for (count = 0; count < ARRAY_SIZE(ldata->nodes); ++count) {
        if (count % 1)
            list_add_prev(&test_head, &ldata->nodes[count].list);
        else
            list_add(&test_head, &ldata->nodes[count].list);
    }

    list_qsort(&test_head, list_test_sort, NULL);

    list_for_each(list, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    list_for_each_continue(list, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_continue' test: %lu\n", node->num);
    }

    list = tlist;
    list_for_each_from(list, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_from' test: %lu\n", node->num);
    }

    list_for_each_reverse(list, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_reverse' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    list_for_each_reverse_continue(list, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_reverse_continue' test: %lu\n", node->num);
    }

    list = tlist;
    list_for_each_reverse_from(list, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_reverse_from' test: %lu\n", node->num);
    }

    list_for_each_safe(list, nlist, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    list_for_each_continue_safe(list, nlist, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_continue_safe' test: %lu\n", node->num);
    }

    list = tlist;
    list_for_each_from_safe(list, nlist, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_from_safe' test: %lu\n", node->num);
    }

    list_for_each_reverse_safe(list, nlist, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_reverse_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    list_for_each_reverse_continue_safe(list, nlist, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_reverse_continue_safe' test: %lu\n", node->num);
    }

    list = tlist;
    list_for_each_reverse_from_safe(list, nlist, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_reverse_from_safe' test: %lu\n", node->num);
    }

    list_for_each_entry(node, &test_head, list) {
        printf("list 'list_for_each_entry' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    list_for_each_entry_continue(node, &test_head, list) {
        printf("list 'list_for_each_entry_continue' test: %lu\n", node->num);
    }

    node = tnode;
    list_for_each_entry_from(node, &test_head, list) {
        printf("list 'list_for_each_entry_from' test: %lu\n", node->num);
    }

    list_for_each_entry_reverse(node, &test_head, list) {
        printf("list 'list_for_each_entry_reverse' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    list_for_each_entry_reverse_continue(node, &test_head, list) {
        printf("list 'list_for_each_entry_reverse_continue' test: %lu\n", node->num);
    }

    node = tnode;
    list_for_each_entry_reverse_from(node, &test_head, list) {
        printf("list 'list_for_each_entry_reverse_from' test: %lu\n", node->num);
    }

    list_for_each_entry_safe(node, nnode, &test_head, list) {
        printf("list 'list_for_each_entry_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    list_for_each_entry_continue_safe(node, nnode, &test_head, list) {
        printf("list 'list_for_each_entry_continue_safe' test: %lu\n", node->num);
    }

    node = tnode;
    list_for_each_entry_from_safe(node, nnode, &test_head, list) {
        printf("list 'list_for_each_entry_form_safe' test: %lu\n", node->num);
    }

    list_for_each_entry_reverse_safe(node, nnode, &test_head, list) {
        printf("list 'list_for_each_entry_reverse_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
        list_del(&node->list);
    }

    tnode = node;
    list_for_each_entry_reverse_continue_safe(node, nnode, &test_head, list) {
        printf("list 'list_for_each_entry_reverse_continue_safe' test: %lu\n", node->num);
    }

    node = tnode;
    list_for_each_entry_reverse_from_safe(node, nnode, &test_head, list) {
        printf("list 'list_for_each_entry_reverse_form_safe' test: %lu\n", node->num);
        list_del(&node->list);
    }

    return 0;
}

int main(void)
{
    struct list_test_pdata *ldata;
    unsigned int count;
    int retval;

    ldata = malloc(sizeof(struct list_test_pdata));
    if (!ldata)
        return -1;

    for (count = 0; count < ARRAY_SIZE(ldata->nodes); ++count)
        ldata->nodes[count].num = count;

    retval = list_selftest(ldata);
    free(ldata);

    return retval;
}
