/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021-2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>
#include <bfdev/rbtree.h>

#define RB_DEBUG 0
#define RB_CACHED 1
#define TEST_LEN 1000000

struct bench_node {
    struct bfdev_rb_node node;
    unsigned int num;
    unsigned long data;
};

#define rb_to_bench(ptr) \
    bfdev_rb_entry_safe(ptr, struct bench_node, node)

#if RB_DEBUG
static void
node_dump(struct bench_node *node)
{
    printf("\t%04d: ", node->num);
    printf("parent %-4d ", node->node.parent ? rb_to_bench(node->node.parent)->num : 0);
    printf("left %-4d ", node->node.left ? rb_to_bench(node->node.left)->num : 0);
    printf("right %-4d ", node->node.right ? rb_to_bench(node->node.right)->num : 0);
    printf("data 0x%16lx ", node->data);
    printf("color'%s' ", node->node.color ? "black" : "red");
    printf("\n");
}
#else
# define node_dump(node) ((void)(node))
#endif

#if RB_CACHED
static BFDEV_RB_ROOT_CACHED(bench_root);
# define bc_insert                      bfdev_rb_cached_insert
# define bc_delete                      bfdev_rb_cached_delete
# define bc_for_each_entry              bfdev_rb_cached_for_each_entry
# define bc_post_for_each_entry         bfdev_rb_cached_post_for_each_entry
# define bc_post_for_each_entry_safe    bfdev_rb_cached_post_for_each_entry_safe
# define bc_deepth(cached)              test_deepth((cached)->root.node)
#else
static BFDEV_RB_ROOT(bench_root);
# define bc_insert                      bfdev_rb_insert
# define bc_delete                      bfdev_rb_delete
# define bc_for_each_entry              bfdev_rb_for_each_entry
# define bc_post_for_each_entry         bfdev_rb_post_for_each_entry
# define bc_post_for_each_entry_safe    bfdev_rb_post_for_each_entry_safe
# define bc_deepth(root)                test_deepth((root)->node)
#endif

static void
time_dump(int ticks, clock_t start, clock_t stop, struct tms *start_tms, struct tms *stop_tms)
{
    printf("\treal time: %lf\n", (stop - start) / (double)ticks);
    printf("\tuser time: %lf\n", (stop_tms->tms_utime - start_tms->tms_utime) / (double)ticks);
    printf("\tkern time: %lf\n", (stop_tms->tms_stime - start_tms->tms_stime) / (double)ticks);
}

static unsigned int
test_deepth(struct bfdev_rb_node *node)
{
    unsigned int left_deepth, right_deepth;

    if (!node)
        return 0;

    left_deepth = test_deepth(node->left);
    right_deepth = test_deepth(node->right);
    return left_deepth > right_deepth ? (left_deepth + 1) : (right_deepth + 1);
}

static long
demo_cmp(const struct bfdev_rb_node *node1,
         const struct bfdev_rb_node *node2, void *pdata)
{
    struct bench_node *test1, *test2;

    test1 = rb_to_bench(node1);
    test2 = rb_to_bench(node2);

    if (test1->data == test2->data)
        return 0;

    return test1->data < test2->data ? -1 : 1;
}

int main(void)
{
    struct bench_node *node, *tmp;
    struct tms start_tms, stop_tms;
    clock_t start, stop;
    unsigned int count, ticks;
    void *block;

    node = block = malloc(sizeof(*node) * TEST_LEN);
    if (!block) {
        printf("Insufficient Memory!\n");
        return 1;
    }

    printf("Generate %u Node:\n", TEST_LEN);
    srand(time(NULL));
    for (count = 0; count < TEST_LEN; ++count) {
        node[count].data = ((uint64_t)rand() << 32) | rand();
#if RB_DEBUG
        node[count].num = count + 1;
        printf("\t%08d: 0x%016lx\n", node->num, node->data);
#endif
    }

    printf("Insert Nodes:\n");
    ticks = sysconf(_SC_CLK_TCK);
    start = times(&start_tms);
    for (count = 0; count < TEST_LEN; ++count)
        bc_insert(&bench_root, &node[count].node, demo_cmp, NULL);
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    count = bc_deepth(&bench_root);
    printf("\trb deepth: %u\n", count);

    /* Start detection middle order iteration. */
    start = times(&start_tms);
    count = 0;
    printf("Middle Iteration:\n");
    bc_for_each_entry(node, &bench_root, node) {
        node_dump(node);
        count++;
    }
    stop = times(&stop_tms);
    printf("\ttotal num: %u\n", count);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    /* Start detection postorder order iteration. */
    start = times(&start_tms);
    count = 0;
    printf("Postorder Iteration:\n");
    bc_post_for_each_entry(node, &bench_root, node) {
        node_dump(node);
        count++;
    }
    stop = times(&stop_tms);
    printf("\ttotal num: %u\n", count);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    /* Start detection postorder order safe iteration. */
    start = times(&start_tms);
    count = 0;
    printf("Postorder Safe Iteration:\n");
    bc_post_for_each_entry_safe(node, tmp, &bench_root, node) {
        node_dump(node);
        count++;
    }
    stop = times(&stop_tms);
    printf("\ttotal num: %u\n", count);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    printf("Done.\n");
    free(block);

    return 0;
}
