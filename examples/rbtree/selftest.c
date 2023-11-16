/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <bfdev/rbtree.h>

#define TEST_LOOP 100

struct rbtree_test_node {
    struct bfdev_rb_node node;
    unsigned long data;
};

struct rbtree_test_pdata {
    struct rbtree_test_node nodes[TEST_LOOP];
};

#define rbnode_to_test(ptr) \
    bfdev_rb_entry(ptr, struct rbtree_test_node, node)

#define rbnode_to_test_safe(ptr) \
    bfdev_rb_entry_safe(ptr, struct rbtree_test_node, node)

static long
rbtest_rb_cmp(const struct bfdev_rb_node *rba, const struct bfdev_rb_node *rbb, void *pdata)
{
    struct rbtree_test_node *node1, *node2;

    node1 = rbnode_to_test(rba);
    node2 = rbnode_to_test(rbb);

    if (node1->data== node2->data)
        return 0;

    return node1->data < node2->data ? -1 : 1;
}

static long
rbtest_rb_find(const struct bfdev_rb_node *rb, void *key)
{
    struct rbtree_test_node *node;

    node = rbnode_to_test(rb);
    if (node->data == (unsigned long)key)
        return 0;

    return node->data < (unsigned long)key ? -1 : 1;
}

static int
rbtree_test_testing(struct rbtree_test_pdata *sdata)
{
    struct rbtree_test_node *node, *nnode, *tnode;
    struct bfdev_rb_node *rbnode, *nrbnode, *trbnode;
    unsigned long count;

    BFDEV_RB_ROOT_CACHED(test_root);

    for (count = 0; count < TEST_LOOP; ++count)
        bfdev_rb_cached_insert(&test_root, &sdata->nodes[count].node, rbtest_rb_cmp, NULL);

    for (count = 0; count < TEST_LOOP; ++count) {
        rbnode = bfdev_rb_find(&test_root.root, (void *)sdata->nodes[count].data, rbtest_rb_find);
        if (!(node = rbnode_to_test_safe(rbnode)))
            return 1;
        printf("rbtree 'rb_cached' test: %lu\n", node->data);
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        rbnode = bfdev_rb_cached_find(&test_root, (void *)sdata->nodes[count].data, rbtest_rb_find);
        if (!(node = rbnode_to_test_safe(rbnode)))
            return 1;
        printf("rbtree 'rb_cached_find' test: %lu\n", node->data);
    }

    count = 0;
    bfdev_rb_for_each(rbnode, &test_root.root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_for_each' test: %lu\n", node->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    trbnode = rbnode;
    bfdev_rb_for_each_continue(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_for_each_continue' test: %lu\n", node->data);
        count++;
    }

    rbnode = trbnode;
    bfdev_rb_for_each_from(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_for_each_from' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_for_each_reverse(rbnode, &test_root.root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_for_each_reverse' test: %lu\n", node->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    trbnode = rbnode;
    bfdev_rb_for_each_reverse_continue(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_for_each_reverse_continue' test: %lu\n", node->data);
        count++;
    }

    rbnode = trbnode;
    bfdev_rb_for_each_reverse_from(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_for_each_reverse_from' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_post_for_each(rbnode, &test_root.root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_post_for_each' test: %lu\n", node->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    trbnode = rbnode;
    bfdev_rb_post_for_each_continue(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_post_for_each_continue' test: %lu\n", node->data);
        count++;
    }

    rbnode = trbnode;
    bfdev_rb_post_for_each_from(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_post_for_each_from' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_post_for_each_safe(rbnode, nrbnode, &test_root.root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_post_for_each_safe' test: %lu\n", node->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    trbnode = rbnode;
    bfdev_rb_post_for_each_safe_continue(rbnode, nrbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_post_for_each_safe_continue' test: %lu\n", node->data);
        count++;
    }

    rbnode = trbnode;
    bfdev_rb_post_for_each_safe_from(rbnode, nrbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_post_for_each_safe_from' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_for_each_entry(node, &test_root.root, node) {
        printf("rbtree 'rb_for_each_entry' test: %lu\n", node->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_rb_for_each_entry_continue(node, node) {
        printf("rbtree 'rb_for_each_entry_continue' test: %lu\n", node->data);
        count++;
    }

    node = tnode;
    bfdev_rb_for_each_entry_from(node, node) {
        printf("rbtree 'rb_for_each_entry_from' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_for_each_entry_reverse(node, &test_root.root, node) {
        printf("rbtree 'rb_for_each_entry_reverse' test: %lu\n", node->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_rb_for_each_entry_reverse_continue(node, node) {
        printf("rbtree 'rb_for_each_entry_reverse_continue' test: %lu\n", node->data);
        count++;
    }

    node = tnode;
    bfdev_rb_for_each_entry_reverse_from(node, node) {
        printf("rbtree 'rb_for_each_entry_reverse_from' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_post_for_each_entry(node, &test_root.root, node) {
        printf("rbtree 'rb_post_for_each_entry' test: %lu\n", node->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_rb_post_for_each_entry_continue(node, node) {
        printf("rbtree 'rb_post_for_each_entry_continue' test: %lu\n", node->data);
        count++;
    }

    node = tnode;
    bfdev_rb_post_for_each_entry_from(node, node) {
        printf("rbtree 'rb_post_for_each_entry_from' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_pre_for_each(rbnode, &test_root.root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_pre_for_each' test: %lu\n", node->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    trbnode = rbnode;
    bfdev_rb_pre_for_each_continue(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_pre_for_each_continue' test: %lu\n", node->data);
        count++;
    }

    rbnode = trbnode;
    bfdev_rb_pre_for_each_from(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_pre_for_each_from' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_pre_for_each_entry(node, &test_root.root, node) {
        printf("rbtree 'rb_pre_for_each_entry' test: %lu\n", node->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_rb_pre_for_each_entry_continue(node, node) {
        printf("rbtree 'rb_pre_for_each_entry_continue' test: %lu\n", node->data);
        count++;
    }

    node = tnode;
    bfdev_rb_pre_for_each_entry_from(node, node) {
        printf("rbtree 'rb_pre_for_each_entry_from' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_post_for_each(rbnode, &test_root.root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_post_for_each' test: %lu\n", node->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_rb_post_for_each_entry_continue(node, node) {
        printf("rbtree 'rb_post_for_each_entry_continue' test: %lu\n", node->data);
        count++;
    }

    node = tnode;
    bfdev_rb_post_for_each_entry_from(node, node) {
        printf("rbtree 'rb_post_for_each_entry_from' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_post_for_each_entry_safe(node, nnode, &test_root.root, node) {
        printf("rbtree 'rb_post_for_each_entry_safe' test: %lu\n", node->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_rb_post_for_each_entry_continue_safe(node, nnode, node) {
        printf("rbtree 'rb_post_for_each_entry_continue_safe' test: %lu\n", node->data);
        count++;
    }

    node = tnode;
    bfdev_rb_post_for_each_entry_from_safe(node, nnode, node) {
        printf("rbtree 'rb_post_for_each_entry_from_safe' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_cached_for_each(rbnode, &test_root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_cached_for_each' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_for_each_reverse(rbnode, &test_root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_cached_for_each_reverse' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_for_each_entry(node, &test_root, node) {
        printf("rbtree 'rb_cached_for_each_entry' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_for_each_entry_reverse(node, &test_root, node) {
        printf("rbtree 'rb_cached_for_each_entry_reverse' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_pre_for_each(rbnode, &test_root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_cached_pre_for_each' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_pre_for_each_entry(node, &test_root, node) {
        printf("rbtree 'rb_cached_pre_for_each_entry' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_post_for_each(rbnode, &test_root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_cached_post_for_each' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_post_for_each_safe(rbnode, nrbnode, &test_root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_cached_post_for_each_safe' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_post_for_each_entry(node, &test_root, node) {
        printf("rbtree 'rb_cached_post_for_each_entry' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_post_for_each_entry_safe(node, nnode, &test_root, node) {
        printf("rbtree 'rb_cached_post_for_each_entry_safe' test: %lu\n", node->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    return 0;
}

int main(void)
{
    struct rbtree_test_pdata *rdata;
    unsigned int count;
    int retval;

    rdata = malloc(sizeof(struct rbtree_test_pdata));
    if (!rdata)
        return -1;

    printf("Sequence Test...\n");
    for (count = 0; count < TEST_LOOP; ++count)
        rdata->nodes[count].data = count;

    retval = rbtree_test_testing(rdata);
    if (retval) {
        printf("Abort1.\n");
        free(rdata);
        return retval;
    }

    printf("Random Test...\n");
    srand(time(NULL));
    for (count = 0; count < TEST_LOOP; ++count)
        rdata->nodes[count].data = rand();

    retval = rbtree_test_testing(rdata);
    if (retval) {
        printf("Abort2.\n");
        free(rdata);
        return retval;
    }

    printf("Done.\n");
    free(rdata);

    return 0;
}
