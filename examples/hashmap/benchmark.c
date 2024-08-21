/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "hashmap-benchmark"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/log.h>
#include <bfdev/hashmap.h>
#include "../time.h"

#define TEST_LOOP 10
#define TEST_SIZE 1000000

struct test_node {
    bfdev_hlist_node_t node;
    unsigned long value;
};

#define node_to_test(ptr) \
    bfdev_container_of(ptr, struct test_node, node)

static inline unsigned long
test_hash_key(const void *key, void *pdata)
{
    return (unsigned long)key;
}

static inline unsigned long
test_hash_node(const bfdev_hlist_node_t *node, void *pdata)
{
    struct test_node *tnode;

    tnode = node_to_test(node);

    return tnode->value;
}

static inline long
test_equal(const bfdev_hlist_node_t *node1,
           const bfdev_hlist_node_t *node2, void *pdata)
{
    struct test_node *tnode1, *tnode2;

    tnode1 = node_to_test(node1);
    tnode2 = node_to_test(node2);

    return tnode1->value - tnode2->value;
}

static inline long
test_find(const bfdev_hlist_node_t *node, const void *key, void *pdata)
{
    struct test_node *tnode;

    tnode = node_to_test(node);

    return tnode->value - (unsigned long)key;
}

static bfdev_hashmap_ops_t
test_ops = {
    .hash_key = test_hash_key,
    .hash_node = test_hash_node,
    .equal = test_equal,
    .find = test_find,
};

int
main(int argc, const char *argv[])
{
    struct test_node *nodes;
    bfdev_hlist_node_t *hnode;
    unsigned long value;
    unsigned int count, loop;
    void *block;
    int retval;

    BFDEV_DEFINE_HASHMAP(test_map, NULL, &test_ops, NULL);
    nodes = block = malloc(sizeof(*nodes) * TEST_SIZE);
    if (!block) {
        bfdev_log_err("Insufficient memory!\n");
        return 1;
    }

    srand(time(NULL));
    bfdev_log_info("Generate %u node:\n", TEST_SIZE);
    for (count = 0; count < TEST_SIZE; ++count)
        nodes[count].value = ((uint64_t)rand() << 32) | rand();

    bfdev_log_info("Insert nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_SIZE; ++count) {
            value = ((uint64_t)rand() << 32) | rand();
            nodes[count].value = value;

            retval = bfdev_hashmap_add(&test_map, &nodes[count].node);
            if (retval)
                return retval;
        }
        0;
    );

    for (loop = 0; loop < TEST_LOOP; ++loop) {
        bfdev_log_info("Find nodes loop%u...\n", loop);
        EXAMPLE_TIME_STATISTICAL(
            for (count = 0; count < TEST_SIZE; ++count) {
                value = nodes[count].value;
                hnode = bfdev_hashmap_find(&test_map, (void *)value);
                if (!hnode)
                    return 1;
            }
            0;
        );
    }

    bfdev_log_info("Delete nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_SIZE; ++count) {
            value = nodes[count].value;
            retval = bfdev_hashmap_del(&test_map, (void *)value, &hnode);
            if (retval)
                return retval;
        }
        0;
    );

    bfdev_log_info("Done.\n");
    bfdev_hashmap_release(&test_map);
    free(nodes);

    return 0;
}
