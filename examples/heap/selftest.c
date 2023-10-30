/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "heap-selftest"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bfdev/heap.h>
#include <bfdev/log.h>

#define TEST_LOOP 100

struct test_node {
    struct bfdev_heap_node node;
    unsigned short num;
};

struct test_pdata {
    struct test_node nodes[TEST_LOOP];
};

#define hpnode_to_test(ptr) \
    bfdev_heap_entry(ptr, struct test_node, node)

static long
bfdev_heap_test_cmp(const struct bfdev_heap_node *hpa, const struct bfdev_heap_node *hpb, void *pdata)
{
    struct test_node *nodea = hpnode_to_test(hpa);
    struct test_node *nodeb = hpnode_to_test(hpb);
    return nodea->num < nodeb->num ? -1 : 1;
}

static int
bfdev_heap_test_testing(struct test_pdata *hdata)
{
    struct test_node *node, *nnode, *tnode;
    struct bfdev_heap_node *hpnode, *nhpnode, *thpnode;
    unsigned long count, index, tindex;

    BFDEV_HEAP_ROOT(root);

    for (count = 0; count < TEST_LOOP; ++count)
        bfdev_heap_insert(&root, &hdata->nodes[count].node, bfdev_heap_test_cmp, NULL);

    count = 0;
    bfdev_heap_for_each(hpnode, &index, &root) {
        node = hpnode_to_test(hpnode);
        bfdev_log_info("'heap_for_each' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    thpnode = hpnode;
    tindex = index;
    bfdev_heap_for_each_continue(hpnode, &index, &root) {
        node = hpnode_to_test(hpnode);
        bfdev_log_info("'heap_for_each_continue' test: %u\n", node->num);
        count++;
    }

    hpnode = thpnode;
    index = tindex;
    bfdev_heap_for_each_from(hpnode, &index, &root) {
        node = hpnode_to_test(hpnode);
        bfdev_log_info("'heap_for_each_from' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2)) {
        bfdev_log_err("count verification failed: %lu\n", count);
        return 1;
    }

    count = 0;
    bfdev_heap_for_each_entry(node, &index, &root, node) {
        bfdev_log_info("'heap_for_each_entry' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    tindex = index;
    bfdev_heap_for_each_entry_continue(node, &index, &root, node) {
        bfdev_log_info("'heap_for_each_entry_continue' test: %u\n", node->num);
        count++;
    }

    node = tnode;
    index = tindex;
    bfdev_heap_for_each_entry_from(node, &index, &root, node) {
        bfdev_log_info("'heap_for_each_entry_from' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2)) {
        bfdev_log_err("count verification failed: %lu\n", count);
        return 1;
    }

    count = 0;
    bfdev_heap_post_for_each(hpnode, &root) {
        node = hpnode_to_test(hpnode);
        bfdev_log_info("'heap_post_for_each' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    thpnode = hpnode;
    bfdev_heap_post_for_each_continue(hpnode) {
        node = hpnode_to_test(hpnode);
        bfdev_log_info("'heap_post_for_each_continue' test: %u\n", node->num);
        count++;
    }

    hpnode = thpnode;
    bfdev_heap_post_for_each_from(hpnode) {
        node = hpnode_to_test(hpnode);
        bfdev_log_info("'heap_post_for_each_from' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2)) {
        bfdev_log_err("count verification failed: %lu\n", count);
        return 1;
    }

    count = 0;
    bfdev_heap_post_for_each_safe(hpnode, nhpnode, &root) {
        node = hpnode_to_test(hpnode);
        bfdev_log_info("'heap_post_for_each_safe' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    thpnode = hpnode;
    bfdev_heap_post_for_each_safe_continue(hpnode, nhpnode) {
        node = hpnode_to_test(hpnode);
        bfdev_log_info("'heap_post_for_each_safe_continue' test: %u\n", node->num);
        count++;
    }

    hpnode = thpnode;
    bfdev_heap_post_for_each_safe_from(hpnode, nhpnode) {
        node = hpnode_to_test(hpnode);
        bfdev_log_info("'heap_post_for_each_safe_from' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2)) {
        bfdev_log_err("count verification failed: %lu\n", count);
        return 1;
    }

    count = 0;
    bfdev_heap_post_for_each_entry(node, &root, node) {
        bfdev_log_info("'heap_post_for_each_entry' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_heap_post_for_each_entry_continue(node, node) {
        bfdev_log_info("'heap_post_for_each_entry_continue' test: %u\n", node->num);
        count++;
    }

    node = tnode;
    bfdev_heap_post_for_each_entry_from(node, node) {
        bfdev_log_info("'heap_post_for_each_entry_from' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2)) {
        bfdev_log_err("count verification failed: %lu\n", count);
        return 1;
    }

    count = 0;
    bfdev_heap_post_for_each_entry_safe(node, nnode, &root, node) {
        bfdev_log_info("'heap_post_for_each_entry_safe' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_heap_post_for_each_entry_continue_safe(node, nnode, node) {
        bfdev_log_info("'heap_post_for_each_entry_continue_safe' test: %u\n", node->num);
        count++;
    }

    node = tnode;
    bfdev_heap_post_for_each_entry_from_safe(node, nnode, node) {
        bfdev_log_info("'heap_post_for_each_entry_from_safe' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2)) {
        bfdev_log_err("count verification failed: %lu\n", count);
        return 1;
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        node = hpnode_to_test(root.node);
        bfdev_log_info("'heap_delete' test: %u\n", node->num);
        bfdev_heap_delete(&root, &node->node, bfdev_heap_test_cmp, NULL);
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

    srand(time(NULL));
    for (count = 0; count < TEST_LOOP; ++count)
        rdata->nodes[count].num = rand();

    retval = bfdev_heap_test_testing(rdata);
    bfdev_log_notice("heap test: %s\n", retval ? "failed" : "passed");
    free(rdata);

    return retval;
}
