/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "btree-benchmark"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/log.h>
#include <bfdev/btree.h>
#include "../time.h"

#define BTREE_DEBUG 0
#define TEST_LEN 1000000

struct bench_node {
    unsigned int num;
    uintptr_t data;
};

#if BTREE_DEBUG
static void
node_dump(struct bench_node *node)
{
    bfdev_log_info("\t%04d: ", node->num);
    bfdev_log_info("data %#018lx ", node->data);
    bfdev_log_info("\n");
}
#else
# define node_dump(node) ((void)(node))
#endif

static const bfdev_btree_ops_t
bench_ops = {
    .alloc = bfdev_btree_alloc,
    .free = bfdev_btree_free,
    .find = bfdev_btree_key_find,
};

int main(int argc, const char *argv[])
{
    struct bench_node *node;
    unsigned int count;
    uintptr_t key;
    void *block;

    BFDEV_BTREE_ROOT(
        bench_root, &bfdev_btree_layoutptr,
        &bench_ops, NULL
    );

    node = block = malloc(sizeof(*node) * TEST_LEN);
    if (!block) {
        bfdev_log_err("Insufficient memory!\n");
        return 1;
    }

    srand(time(NULL));
    bfdev_log_info("Generate %u node:\n", TEST_LEN);
    for (count = 0; count < TEST_LEN; ++count) {
        node[count].num = count + 1;
        node[count].data = ((uint64_t)rand() << 32) | rand();
#if BTREE_DEBUG
        bfdev_log_info("\t%08d: 0x%016lx\n", node->num, node->data);
#endif
    }

    bfdev_log_info("Insert nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_LEN; ++count)
            bfdev_btree_insert(&bench_root, &node[count].data, node);
        0;
    );

    count = 0;
    bfdev_log_info("Btree iteration:\n");
    EXAMPLE_TIME_STATISTICAL(
        bfdev_btree_for_each(&bench_root, &key, node) {
            node_dump(node);
            count++;
        }
        0;
    );
    bfdev_log_info("\ttotal num: %u\n", count);

    count = 0;
    bfdev_log_info("Btree reverse iteration:\n");
    EXAMPLE_TIME_STATISTICAL(
        bfdev_btree_for_each_reverse(&bench_root, &key, node) {
            node_dump(node);
            count++;
        }
        0;
    );
    bfdev_log_info("\ttotal num: %u\n", count);

    bfdev_log_info("Done.\n");
    bfdev_btree_destroy(&bench_root);
    free(block);

    return 0;
}
