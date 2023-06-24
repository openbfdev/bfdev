/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/heap.h>

#define TEST_LOOP 10

struct test_node {
    struct bfdev_heap_node node;
    unsigned short num;
};

struct test_pdata {
    struct test_node nodes[TEST_LOOP];
};

#define hpnode_to_test(ptr) \
    bfdev_heap_entry(ptr, struct test_node, node)

static long bfdev_heap_test_cmp(const struct bfdev_heap_node *hpa, const struct bfdev_heap_node *hpb)
{
    struct test_node *nodea = hpnode_to_test(hpa);
    struct test_node *nodeb = hpnode_to_test(hpb);
    return nodea->num < nodeb->num ? -1 : 1;
}

static int bfdev_heap_test_testing(struct test_pdata *hdata)
{
    struct test_node *node, *tnode;
    struct bfdev_heap_node *hpnode, *thpnode;
    unsigned long count, index, tindex;

    BFDEV_HEAP_ROOT(bfdev_heap_root);

    for (count = 0; count < TEST_LOOP; ++count)
        bfdev_heap_insert(&bfdev_heap_root, &hdata->nodes[count].node, bfdev_heap_test_cmp);

    count = 0;
    bfdev_heap_for_each(hpnode, &index, &bfdev_heap_root) {
        node = hpnode_to_test(hpnode);
        printf("heap 'bfdev_heap_for_each' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    thpnode = hpnode;
    tindex = index;
    bfdev_heap_for_each_continue(hpnode, &index, &bfdev_heap_root) {
        node = hpnode_to_test(hpnode);
        printf("heap 'bfdev_heap_for_each_continue' test: %u\n", node->num);
    }

    hpnode = thpnode;
    index = tindex;
    bfdev_heap_for_each_from(hpnode, &index, &bfdev_heap_root) {
        node = hpnode_to_test(hpnode);
        printf("heap 'bfdev_heap_for_each_from' test: %u\n", node->num);
    }

    count = 0;
    bfdev_heap_for_each_entry(node, &index, &bfdev_heap_root, node) {
        printf("heap 'bfdev_heap_for_each_entry' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    tindex = index;
    bfdev_heap_for_each_entry_continue(node, &index, &bfdev_heap_root, node) {
        printf("heap 'bfdev_heap_for_each_entry_continue' test: %u\n", node->num);
    }

    node = tnode;
    index = tindex;
    bfdev_heap_for_each_entry_from(node, &count, &bfdev_heap_root, node) {
        printf("heap 'bfdev_heap_for_each_entry_from' test: %u\n", node->num);
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        node = hpnode_to_test(bfdev_heap_root.node);
        printf("heap 'bfdev_heap_delete' test: %u\n", node->num);
        bfdev_heap_delete(&bfdev_heap_root, &node->node, bfdev_heap_test_cmp);
    }

    return 0;
}

int main(void)
{
    struct test_pdata *rdata;
    unsigned int count;
    int retval;

    rdata = malloc(sizeof(struct test_pdata));
    if (!rdata)
        return 1;

    for (count = 0; count < TEST_LOOP; ++count)
        rdata->nodes[count].num = rand();

    retval = bfdev_heap_test_testing(rdata);
    free(rdata);

    return retval;
}
