/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/segtree.h>

#define TEST_LOOP 10

struct segtree_test_node {
    struct segtree_node segnode;
    unsigned int num;
};

struct segtree_test_pdata {
    struct segtree_test_node nodes[TEST_LOOP];
    unsigned int queries[TEST_LOOP];
};

#define segtree_to_test(ptr) \
    segtree_entry(ptr, struct segtree_test_node, segnode)

static int segtree_test_testing(struct segtree_test_pdata *sdata)
{
    struct segtree_test_node *node, *tnode;
    struct segtree_node *snode, *tsnode;
    unsigned int count;

    RB_ROOT_CACHED(segtree_root);

    for (count = 0; count < TEST_LOOP; ++count)
        segtree_insert(&segtree_root, &sdata->nodes[count].segnode);

    for (count = 0; count < TEST_LOOP; ++count) {
        segtree_for_each_entry(node, 0, sdata->queries[count], &segtree_root, segnode) {
            printf("srgtree search %u: %lu - %lu\n", sdata->queries[count],
                    node->segnode.start, node->segnode.end);
        }
    }

    count = 0;
    segtree_for_each(snode, 0, ~0UL, &segtree_root) {
        node = segtree_to_test(snode);
        printf("srgtree 'segtree_for_each' test: %lu - %lu\n",
                node->segnode.start, node->segnode.end);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tsnode = snode;
    segtree_for_each_form(snode, 0, ~0UL) {
        node = segtree_to_test(snode);
        printf("srgtree 'segtree_for_each_form' test: %lu - %lu\n",
                node->segnode.start, node->segnode.end);
    }

    snode = tsnode;
    segtree_for_each_continue(snode, 0, ~0UL) {
        node = segtree_to_test(snode);
        printf("srgtree 'segtree_for_each_continue' test: %lu - %lu\n",
                node->segnode.start, node->segnode.end);
    }

    count = 0;
    segtree_for_each_entry(node, 0, ~0UL, &segtree_root, segnode) {
        printf("srgtree 'segtree_for_each_entry' test: %lu - %lu\n",
                node->segnode.start, node->segnode.end);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    segtree_for_each_entry_from(node, 0, ~0UL, segnode) {
        printf("srgtree 'segtree_for_each_entry_from' test: %lu - %lu\n",
                node->segnode.start, node->segnode.end);
    }

    node = tnode;
    segtree_for_each_entry_continue(node, 0, ~0UL, segnode) {
        printf("srgtree 'segtree_for_each_entry_continue' test: %lu - %lu\n",
                node->segnode.start, node->segnode.end);
    }

    for (count = 0; count < TEST_LOOP; ++count)
        segtree_delete(&segtree_root, &sdata->nodes[count].segnode);

    return 0;
}

int main(void)
{
    struct segtree_test_pdata *sdata;
    unsigned long random;
    unsigned int count;
    int retval;

    sdata = malloc(sizeof(struct segtree_test_pdata));
    if (!sdata)
        return -1;

    for (count = 0; count < ARRAY_SIZE(sdata->nodes); ++count) {
        sdata->queries[count] = rand();
        sdata->nodes[count].segnode.end = (random = rand());
        sdata->nodes[count].segnode.start = rand() % random;
    }

    retval = segtree_test_testing(sdata);
    free(sdata);

    return retval;
}
