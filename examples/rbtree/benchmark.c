/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021-2022 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "rbtree-benchmark"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/log.h>
#include <bfdev/rbtree.h>
#include "../time.h"

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
    bfdev_log_info("\t%04d: ", node->num);
    bfdev_log_info("parent %-4d ", node->node.parent ? rb_to_bench(node->node.parent)->num : 0);
    bfdev_log_info("left %-4d ", node->node.left ? rb_to_bench(node->node.left)->num : 0);
    bfdev_log_info("right %-4d ", node->node.right ? rb_to_bench(node->node.right)->num : 0);
    bfdev_log_info("data 0x%16lx ", node->data);
    bfdev_log_info("color'%s' ", node->node.color ? "black" : "red");
    bfdev_log_info("\n");
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

int main(int argc, const char *argv[])
{
    struct bench_node *node, *tmp;
    unsigned int count;
    void *block;

    node = block = malloc(sizeof(*node) * TEST_LEN);
    if (!block) {
        bfdev_log_err("Insufficient memory!\n");
        return 1;
    }

    srand(time(NULL));
    bfdev_log_info("Generate %u node:\n", TEST_LEN);
    for (count = 0; count < TEST_LEN; ++count) {
        node[count].data = ((uint64_t)rand() << 32) | rand();
#if RB_DEBUG
        node[count].num = count + 1;
        bfdev_log_info("\t%08d: 0x%016lx\n", node->num, node->data);
#endif
    }

    bfdev_log_info("Insert nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_LEN; ++count)
            bc_insert(&bench_root, &node[count].node, demo_cmp, NULL);
        0;
    );
    count = bc_deepth(&bench_root);
    bfdev_log_info("\trb deepth: %u\n", count);

    count = 0;
    bfdev_log_info("Middle iteration:\n");
    EXAMPLE_TIME_STATISTICAL(
        bc_for_each_entry(node, &bench_root, node) {
            node_dump(node);
            count++;
        }
        0;
    );
    bfdev_log_info("\ttotal num: %u\n", count);

    count = 0;
    bfdev_log_info("Postorder iteration:\n");
    EXAMPLE_TIME_STATISTICAL(
        bc_post_for_each_entry(node, &bench_root, node) {
            node_dump(node);
            count++;
        }
        0;
    );
    bfdev_log_info("\ttotal num: %u\n", count);

    count = 0;
    bfdev_log_info("Postorder safe iteration:\n");
    EXAMPLE_TIME_STATISTICAL(
        bc_post_for_each_entry_safe(node, tmp, &bench_root, node) {
            node_dump(node);
            count++;
        }
        0;
    );
    bfdev_log_info("\ttotal num: %u\n", count);

    bfdev_log_info("Done.\n");
    free(block);

    return 0;
}
