/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "heap-benchmark"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/log.h>
#include <bfdev/heap.h>
#include "../time.h"

#define HEAP_DEBUG 0
#define TEST_LEN 1000000

struct bench_node {
    bfdev_heap_node_t node;
    unsigned int num;
    unsigned int data;
};

#define bfdev_heap_to_bench(ptr) \
    bfdev_heap_entry_safe(ptr, struct bench_node, node)

#if HEAP_DEBUG
static void
node_dump(struct bench_node *node)
{
    bfdev_log_info("\t%04d: ", node[count].num);
    bfdev_log_info("parent %-4d ", node[count].node.parent ? bfdev_heap_to_bench(node[count].node.parent)->num : 0);
    bfdev_log_info("left %-4d ", node[count].node.left ? bfdev_heap_to_bench(node[count].node.left)->num : 0);
    bfdev_log_info("right %-4d ", node[count].node.right ? bfdev_heap_to_bench(node[count].node.right)->num : 0);
    bfdev_log_info("data 0x%8x ", node[count].data);
    bfdev_log_info("\n");
}
#else
# define node_dump(node) ((void)(node))
#endif

static unsigned int
test_deepth(bfdev_heap_node_t *node)
{
    unsigned int left_deepth, right_deepth;

    if (!node)
        return 0;

    left_deepth = test_deepth(node->left);
    right_deepth = test_deepth(node->right);

    return left_deepth > right_deepth ? (left_deepth + 1) : (right_deepth + 1);
}

static long
bench_cmp(const bfdev_heap_node_t *hpa,
          const bfdev_heap_node_t *hpb, void *pdata)
{
    struct bench_node *node1, *node2;

    node1 = bfdev_heap_to_bench(hpa);
    node2 = bfdev_heap_to_bench(hpb);

    if (node1->data == node2->data)
        return 0;

    return node1->data < node2->data ? -1 : 1;
}

int main(int argc, const char *argv[])
{
    struct bench_node *bnode;
    unsigned int count;
    unsigned long index;
    void *block;

    BFDEV_HEAP_ROOT(bench_root);

    block = bnode = malloc(sizeof(*bnode) * TEST_LEN);
    if (!bnode) {
        bfdev_log_err("Insufficient memory!\n");
        return 1;
    }

    srand(time(NULL));
    bfdev_log_info("Generate %u node:\n", TEST_LEN);
    for (count = 0; count < TEST_LEN; ++count) {
        bnode[count].num = count + 1;
        bnode[count].data = rand();
#if HEAP_DEBUG
        bfdev_log_info("\t%08d: 0x%8x\n", bnode[count].num, bnode[count].data);
#endif
    }

    bfdev_log_info("Insert nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_LEN; ++count)
            bfdev_heap_insert(&bench_root, &bnode[count].node, bench_cmp, NULL);
        0;
    );
    count = test_deepth(bench_root.node);
    bfdev_log_info("\theap deepth: %u\n", count);

    count = 0;
    bfdev_log_info("Level-order iteration:\n");
    EXAMPLE_TIME_STATISTICAL(
        bfdev_heap_for_each_entry(bnode, &index, &bench_root, node) {
            node_dump(bnode);
            count++;
        }
        0;
    );
    bfdev_log_info("\ttotal num: %u\n", count);

    bfdev_log_info("Deletion all nodes...\n");
    while (bench_root.count) {
        bnode = bfdev_heap_to_bench(bench_root.node);
        node_dump(bnode);
        bfdev_heap_delete(&bench_root, &bnode->node, bench_cmp, NULL);
    }

    bfdev_log_info("Done.\n");
    free(block);

    return 0;
}
