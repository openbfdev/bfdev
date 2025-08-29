/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021-2022 Zhenlin Wang <sanpeqf@gmail.com>
 */

#define MODULE_NAME "rbtree-benchmark"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/log.h>
#include <bfdev/rbtree.h>
#include <bfdev/macro.h>
#include "../time.h"

#define RB_DEBUG 0
#define RB_CACHED 1

#define TEST_LOOP 3
#define TEST_WARMUP 32
#define TEST_LEN 1000000

struct bench_node {
    bfdev_rb_node_t node;
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
# define bc_root                        (&(bench_root).root)
# define bc_insert                      bfdev_rb_cached_insert
# define bc_delete                      bfdev_rb_cached_delete
# define bc_for_each_entry              bfdev_rb_cached_for_each_entry
# define bc_post_for_each_entry         bfdev_rb_cached_post_for_each_entry
# define bc_post_for_each_entry_safe    bfdev_rb_cached_post_for_each_entry_safe
# define bc_deepth(cached)              test_deepth((cached)->root.node)
#else
static BFDEV_RB_ROOT(bench_root);
# define bc_root                        (&bench_root)
# define bc_insert                      bfdev_rb_insert
# define bc_delete                      bfdev_rb_delete
# define bc_for_each_entry              bfdev_rb_for_each_entry
# define bc_post_for_each_entry         bfdev_rb_post_for_each_entry
# define bc_post_for_each_entry_safe    bfdev_rb_post_for_each_entry_safe
# define bc_deepth(root)                test_deepth((root)->node)
#endif

static unsigned int
test_deepth(bfdev_rb_node_t *node)
{
    unsigned int left_deepth, right_deepth;

    if (!node)
        return 0;

    left_deepth = test_deepth(node->left);
    right_deepth = test_deepth(node->right);

    if (left_deepth > right_deepth)
        left_deepth = right_deepth;

    return right_deepth + 1;
}

static long
demo_cmp(const bfdev_rb_node_t *node1,
         const bfdev_rb_node_t *node2, void *pdata)
{
    unsigned long test1, test2;

    test1 = rb_to_bench(node1)->data;
    test2 = rb_to_bench(node2)->data;

    /* Ignoring conflicts */
    return bfdev_cmp(test1 > test2);
}

static long
demo_find(const bfdev_rb_node_t *node, void *pdata)
{
    unsigned long test1, test2;

    test1 = rb_to_bench(node)->data;
    test2 = (unsigned long)pdata;

    if (test1 == test2)
        return 0;

    return bfdev_cmp(test1 > test2);
}

int
main(int argc, const char *argv[])
{
    struct bench_node *nodes, *node;
    unsigned int count, loop;

    nodes = malloc(sizeof(*node) * TEST_LEN);
    if (!nodes) {
        bfdev_log_err("Insufficient memory!\n");
        return 1;
    }

    srand(time(NULL));
    bfdev_log_info("Generate %u node:\n", TEST_LEN);
    for (count = 0; count < TEST_LEN; ++count) {
        nodes[count].data = ((uint64_t)rand() << 32) | rand();
#if RB_DEBUG
        nodes[count].num = count + 1;
        bfdev_log_info("\t%08d: 0x%016lx\n", nodes->num, nodes->data);
#endif
    }

    bfdev_log_info("Insert nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_LEN; ++count)
            bc_insert(&bench_root, &nodes[count].node, demo_cmp, NULL);
        0;
    );
    count = bc_deepth(&bench_root);
    bfdev_log_info("\trb deepth: %u\n", count);

    bfdev_log_notice("Warnup cache:\n");
    for (loop = 0; loop < TEST_WARMUP; ++loop) {
        for (count = 0; count < TEST_LEN; ++count) {
            if (!bfdev_rb_find(bc_root, (void *)nodes[count].data, &demo_find))
                return 1;
        }
    }

    for (loop = 0; loop < TEST_LOOP; ++loop) {
        bfdev_log_info("Find nodes loop%u...\n", loop);
        EXAMPLE_TIME_STATISTICAL(
            for (count = 0; count < TEST_LEN; ++count) {
                if (!bfdev_rb_find(bc_root, (void *)nodes[count].data, &demo_find))
                    return 1;
            }
            0;
        );
    }

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
    if (count != TEST_LEN)
        return 1;

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
    if (count != TEST_LEN)
        return 1;

    bfdev_log_info("Done.\n");
    free(nodes);

    return 0;
}
