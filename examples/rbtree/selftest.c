/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <bfdev/rbtree.h>

#define TEST_LOOP 100

struct demo_node {
    bfdev_rb_node_t node;
    unsigned long data;
};

struct demo_data {
    struct demo_node nodes[TEST_LOOP];
};

#define rbnode_to_test(ptr) \
    bfdev_rb_entry(ptr, struct demo_node, node)

#define rbnode_to_test_safe(ptr) \
    bfdev_rb_entry_safe(ptr, struct demo_node, node)

static long
rbtest_rb_cmp(const bfdev_rb_node_t *node1, const bfdev_rb_node_t *node2, void *pdata)
{
    struct demo_node *demo1, *demo2;

    demo1 = rbnode_to_test(node1);
    demo2 = rbnode_to_test(node2);

    /* Ignoring conflicts */
    return demo1->data < demo2->data ? -1 : 1;
}

static long
rbtest_rb_find(const bfdev_rb_node_t *node, void *key)
{
    struct demo_node *demo;

    demo = rbnode_to_test(node);
    if (demo->data == (unsigned long)key)
        return 0;

    return demo->data < (unsigned long)key ? -1 : 1;
}

static int
rbtree_testing(struct demo_data *sdata)
{
    struct demo_node *demo, *ndemo, *tdemo;
    bfdev_rb_node_t *node, *nnode, *tnode;
    unsigned long count;

    BFDEV_RB_ROOT_CACHED(test_root);

    for (count = 0; count < TEST_LOOP; ++count)
        bfdev_rb_cached_insert(&test_root, &sdata->nodes[count].node, rbtest_rb_cmp, NULL);

    for (count = 0; count < TEST_LOOP; ++count) {
        node = bfdev_rb_find(&test_root.root, (void *)sdata->nodes[count].data, rbtest_rb_find);
        if (!(demo = rbnode_to_test_safe(node)))
            return 1;
        printf("rbtree 'rb_cached' test: %lu\n", demo->data);
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        node = bfdev_rb_cached_find(&test_root, (void *)sdata->nodes[count].data, rbtest_rb_find);
        if (!(demo = rbnode_to_test_safe(node)))
            return 1;
        printf("rbtree 'rb_cached_find' test: %lu\n", demo->data);
    }

    count = 0;
    bfdev_rb_for_each(node, &test_root.root) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_for_each' test: %lu\n", demo->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_rb_for_each_continue(node) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_for_each_continue' test: %lu\n", demo->data);
        count++;
    }

    node = tnode;
    bfdev_rb_for_each_from(node) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_for_each_from' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_for_each_reverse(node, &test_root.root) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_for_each_reverse' test: %lu\n", demo->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_rb_for_each_reverse_continue(node) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_for_each_reverse_continue' test: %lu\n", demo->data);
        count++;
    }

    node = tnode;
    bfdev_rb_for_each_reverse_from(node) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_for_each_reverse_from' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_post_for_each(node, &test_root.root) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_post_for_each' test: %lu\n", demo->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_rb_post_for_each_continue(node) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_post_for_each_continue' test: %lu\n", demo->data);
        count++;
    }

    node = tnode;
    bfdev_rb_post_for_each_from(node) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_post_for_each_from' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_post_for_each_safe(node, nnode, &test_root.root) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_post_for_each_safe' test: %lu\n", demo->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_rb_post_for_each_safe_continue(node, nnode) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_post_for_each_safe_continue' test: %lu\n", demo->data);
        count++;
    }

    node = tnode;
    bfdev_rb_post_for_each_safe_from(node, nnode) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_post_for_each_safe_from' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_for_each_entry(demo, &test_root.root, node) {
        printf("rbtree 'rb_for_each_entry' test: %lu\n", demo->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tdemo = demo;
    bfdev_rb_for_each_entry_continue(demo, node) {
        printf("rbtree 'rb_for_each_entry_continue' test: %lu\n", demo->data);
        count++;
    }

    demo = tdemo;
    bfdev_rb_for_each_entry_from(demo, node) {
        printf("rbtree 'rb_for_each_entry_from' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_for_each_entry_reverse(demo, &test_root.root, node) {
        printf("rbtree 'rb_for_each_entry_reverse' test: %lu\n", demo->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tdemo = demo;
    bfdev_rb_for_each_entry_reverse_continue(demo, node) {
        printf("rbtree 'rb_for_each_entry_reverse_continue' test: %lu\n", demo->data);
        count++;
    }

    demo = tdemo;
    bfdev_rb_for_each_entry_reverse_from(demo, node) {
        printf("rbtree 'rb_for_each_entry_reverse_from' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_post_for_each_entry(demo, &test_root.root, node) {
        printf("rbtree 'rb_post_for_each_entry' test: %lu\n", demo->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tdemo = demo;
    bfdev_rb_post_for_each_entry_continue(demo, node) {
        printf("rbtree 'rb_post_for_each_entry_continue' test: %lu\n", demo->data);
        count++;
    }

    demo = tdemo;
    bfdev_rb_post_for_each_entry_from(demo, node) {
        printf("rbtree 'rb_post_for_each_entry_from' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_pre_for_each(node, &test_root.root) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_pre_for_each' test: %lu\n", demo->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_rb_pre_for_each_continue(node) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_pre_for_each_continue' test: %lu\n", demo->data);
        count++;
    }

    node = tnode;
    bfdev_rb_pre_for_each_from(node) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_pre_for_each_from' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_pre_for_each_entry(demo, &test_root.root, node) {
        printf("rbtree 'rb_pre_for_each_entry' test: %lu\n", demo->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tdemo = demo;
    bfdev_rb_pre_for_each_entry_continue(demo, node) {
        printf("rbtree 'rb_pre_for_each_entry_continue' test: %lu\n", demo->data);
        count++;
    }

    demo = tdemo;
    bfdev_rb_pre_for_each_entry_from(demo, node) {
        printf("rbtree 'rb_pre_for_each_entry_from' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_post_for_each(node, &test_root.root) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_post_for_each' test: %lu\n", demo->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tdemo = demo;
    bfdev_rb_post_for_each_entry_continue(demo, node) {
        printf("rbtree 'rb_post_for_each_entry_continue' test: %lu\n", demo->data);
        count++;
    }

    demo = tdemo;
    bfdev_rb_post_for_each_entry_from(demo, node) {
        printf("rbtree 'rb_post_for_each_entry_from' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_post_for_each_entry_safe(demo, ndemo, &test_root.root, node) {
        printf("rbtree 'rb_post_for_each_entry_safe' test: %lu\n", demo->data);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tdemo = demo;
    bfdev_rb_post_for_each_entry_continue_safe(demo, ndemo, node) {
        printf("rbtree 'rb_post_for_each_entry_continue_safe' test: %lu\n", demo->data);
        count++;
    }

    demo = tdemo;
    bfdev_rb_post_for_each_entry_from_safe(demo, ndemo, node) {
        printf("rbtree 'rb_post_for_each_entry_from_safe' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return 1;

    count = 0;
    bfdev_rb_cached_for_each(node, &test_root) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_cached_for_each' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_for_each_reverse(node, &test_root) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_cached_for_each_reverse' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_for_each_entry(demo, &test_root, node) {
        printf("rbtree 'rb_cached_for_each_entry' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_for_each_entry_reverse(demo, &test_root, node) {
        printf("rbtree 'rb_cached_for_each_entry_reverse' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_pre_for_each(node, &test_root) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_cached_pre_for_each' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_pre_for_each_entry(demo, &test_root, node) {
        printf("rbtree 'rb_cached_pre_for_each_entry' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_post_for_each(node, &test_root) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_cached_post_for_each' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_post_for_each_safe(node, nnode, &test_root) {
        demo = rbnode_to_test(node);
        printf("rbtree 'rb_cached_post_for_each_safe' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_post_for_each_entry(demo, &test_root, node) {
        printf("rbtree 'rb_cached_post_for_each_entry' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    count = 0;
    bfdev_rb_cached_post_for_each_entry_safe(demo, ndemo, &test_root, node) {
        printf("rbtree 'rb_cached_post_for_each_entry_safe' test: %lu\n", demo->data);
        count++;
    }

    if (count != TEST_LOOP)
        return 1;

    return 0;
}

int main(int argc, const char *argv[])
{
    struct demo_data *rdata;
    unsigned int count;
    int retval;

    rdata = malloc(sizeof(struct demo_data));
    if (!rdata)
        return -1;

    printf("Sequence Test...\n");
    for (count = 0; count < TEST_LOOP; ++count)
        rdata->nodes[count].data = count;

    retval = rbtree_testing(rdata);
    if (retval) {
        printf("Abort1.\n");
        free(rdata);
        return retval;
    }

    printf("Random Test...\n");
    srand(time(NULL));
    for (count = 0; count < TEST_LOOP; ++count)
        rdata->nodes[count].data = rand();

    retval = rbtree_testing(rdata);
    if (retval) {
        printf("Abort2.\n");
        free(rdata);
        return retval;
    }

    printf("Done.\n");
    free(rdata);

    return 0;
}
