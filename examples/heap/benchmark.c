/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <bfdev/heap.h>
#include <sys/times.h>

#define HEAP_DEBUG  0
#define TEST_LEN    1000000

struct bench_node {
    struct bfdev_heap_node node;
    unsigned int num;
    unsigned int data;
};

#define bfdev_heap_to_bench(ptr) \
    bfdev_heap_entry_safe(ptr, struct bench_node, node)

#if HEAP_DEBUG
static void node_dump(struct bench_node *node)
{
    printf("\t%04d: ", node[count].num);
    printf("parent %-4d ", node[count].node.parent ? bfdev_heap_to_bench(node[count].node.parent)->num : 0);
    printf("left %-4d ", node[count].node.left ? bfdev_heap_to_bench(node[count].node.left)->num : 0);
    printf("right %-4d ", node[count].node.right ? bfdev_heap_to_bench(node[count].node.right)->num : 0);
    printf("data 0x%8x ", node[count].data);
    printf("\n");
}
#else
# define node_dump(node) ((void)(node))
#endif

static BFDEV_HEAP_ROOT(bench_root);

static void
time_dump(int ticks, clock_t start, clock_t stop, struct tms *start_tms, struct tms *stop_tms)
{
    printf("\treal time: %lf\n", (stop - start) / (double)ticks);
    printf("\tuser time: %lf\n", (stop_tms->tms_utime - start_tms->tms_utime) / (double)ticks);
    printf("\tkern time: %lf\n", (stop_tms->tms_stime - start_tms->tms_stime) / (double)ticks);
}

static unsigned int
test_deepth(struct bfdev_heap_node *node)
{
    unsigned int left_deepth, right_deepth;

    if (!node)
        return 0;

    left_deepth = test_deepth(node->left);
    right_deepth = test_deepth(node->right);
    return left_deepth > right_deepth ? (left_deepth + 1) : (right_deepth + 1);
}

static long
bench_cmp(const struct bfdev_heap_node *hpa, const struct bfdev_heap_node *hpb, void *pdata)
{
    struct bench_node *node1 = bfdev_heap_to_bench(hpa);
    struct bench_node *node2 = bfdev_heap_to_bench(hpb);
    return node1->data < node2->data ? -1 : 1;
}

int main(void)
{
    struct bench_node *bnode;
    struct tms start_tms, stop_tms;
    clock_t start, stop;
    unsigned int count, ticks;
    unsigned long index;
    void *block;

    block = bnode = malloc(sizeof(*bnode) * TEST_LEN);
    if (!bnode) {
        printf("Insufficient Memory!\n");
        return 1;
    }

    printf("Generate %u node:\n", TEST_LEN);
    srand(time(NULL));
    for (count = 0; count < TEST_LEN; ++count) {
        bnode[count].num = count + 1;
        bnode[count].data = rand();
    }

    printf("Insert Nodes:\n");
    ticks = sysconf(_SC_CLK_TCK);
    start = times(&start_tms);
    for (count = 0; count < TEST_LEN; ++count) {
#if HEAP_DEBUG
        printf("\t%08d: 0x%8x\n", bnode[count].num, bnode[count].data);
#endif
        bfdev_heap_insert(&bench_root, &bnode[count].node, bench_cmp, NULL);
    }
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    count = test_deepth(bench_root.node);
    printf("\theap deepth: %u\n", count);

    start = times(&start_tms);
    count = 0;
    printf("Levelorder Iteration:\n");
    bfdev_heap_for_each_entry(bnode, &index, &bench_root, node) {
        node_dump(bnode);
        count++;
    }
    stop = times(&stop_tms);
    printf("\ttotal num: %u\n", count);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    printf("Deletion All node...\n");
    while (bench_root.count) {
        bnode = bfdev_heap_to_bench(bench_root.node);
        node_dump(bnode);
        bfdev_heap_delete(&bench_root, &bnode->node, bench_cmp, NULL);
    }

    printf("Done.\n");
    free(block);

    return 0;
}
