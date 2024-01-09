/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bfdev/macro.h>
#include <bfdev/segtree.h>

#define TEST_LOOP 10

struct test_node {
    bfdev_segtree_node_t node;
    unsigned int num;
};

struct test_pdata {
    struct test_node nodes[TEST_LOOP];
    unsigned int queries[TEST_LOOP];
};

#define segtree_to_test(ptr) \
    bfdev_segtree_entry(ptr, struct test_node, node)

static int
segtree_testing(struct test_pdata *sdata)
{
    struct test_node *node, *tnode;
    bfdev_segtree_node_t *snode, *tsnode;
    unsigned int count;

    BFDEV_RB_ROOT_CACHED(segtree_root);

    for (count = 0; count < TEST_LOOP; ++count)
        bfdev_segtree_insert(&segtree_root, &sdata->nodes[count].node);

    for (count = 0; count < TEST_LOOP; ++count) {
        bfdev_segtree_for_each_entry(node, 0, sdata->queries[count], &segtree_root, node) {
            printf("srgtree search %u: %lu - %lu\n", sdata->queries[count],
                    node->node.start, node->node.end);
        }
    }

    count = 0;
    bfdev_segtree_for_each(snode, 0, ~0UL, &segtree_root) {
        node = segtree_to_test(snode);
        printf("srgtree 'segtree_for_each' test: %lu - %lu\n",
                node->node.start, node->node.end);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tsnode = snode;
    bfdev_segtree_for_each_form(snode, 0, ~0UL) {
        node = segtree_to_test(snode);
        printf("srgtree 'segtree_for_each_form' test: %lu - %lu\n",
                node->node.start, node->node.end);
    }

    snode = tsnode;
    bfdev_segtree_for_each_continue(snode, 0, ~0UL) {
        node = segtree_to_test(snode);
        printf("srgtree 'segtree_for_each_continue' test: %lu - %lu\n",
                node->node.start, node->node.end);
    }

    count = 0;
    bfdev_segtree_for_each_entry(node, 0, ~0UL, &segtree_root, node) {
        printf("srgtree 'segtree_for_each_entry' test: %lu - %lu\n",
                node->node.start, node->node.end);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_segtree_for_each_entry_from(node, 0, ~0UL, node) {
        printf("srgtree 'segtree_for_each_entry_from' test: %lu - %lu\n",
                node->node.start, node->node.end);
    }

    node = tnode;
    bfdev_segtree_for_each_entry_continue(node, 0, ~0UL, node) {
        printf("srgtree 'segtree_for_each_entry_continue' test: %lu - %lu\n",
                node->node.start, node->node.end);
    }

    for (count = 0; count < TEST_LOOP; ++count)
        bfdev_segtree_delete(&segtree_root, &sdata->nodes[count].node);

    return 0;
}

int main(int argc, const char *argv[])
{
    struct test_pdata *sdata;
    unsigned long random;
    unsigned int count;
    int retval;

    sdata = malloc(sizeof(struct test_pdata));
    if (!sdata)
        return -1;

    srand(time(NULL));
    for (count = 0; count < BFDEV_ARRAY_SIZE(sdata->nodes); ++count) {
        sdata->queries[count] = rand();
        sdata->nodes[count].node.end = (random = rand());
        sdata->nodes[count].node.start = rand() % random;
    }

    retval = segtree_testing(sdata);
    free(sdata);

    return retval;
}
