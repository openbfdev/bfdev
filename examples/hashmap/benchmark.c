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

#define TEST_LOOP 1000000

struct test_node {
    struct bfdev_hlist_node node;
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
test_hash_node(const struct bfdev_hlist_node *node, void *pdata)
{
    const struct test_node *tnode = node_to_test(node);
    return tnode->value;
}

static inline long
test_equal(const struct bfdev_hlist_node *node1,
              const struct bfdev_hlist_node *nodeb, void *pdata)
{
    const struct test_node *tnode1 = node_to_test(node1);
    const struct test_node *tnodeb = node_to_test(nodeb);
    return tnode1->value - tnodeb->value;
}

static inline long
test_find(const struct bfdev_hlist_node *node, const void *key, void *pdata)
{
    const struct test_node *tnode = node_to_test(node);
    return tnode->value - (unsigned long)key;
}

static bfdev_hashmap_ops_t
test_ops = {
    .hash_key = test_hash_key,
    .hash_node = test_hash_node,
    .equal = test_equal,
    .find = test_find,
};

int main(int argc, const char *argv[])
{
    struct test_node *nodes;
    struct bfdev_hlist_node *hnode;
    unsigned long value;
    unsigned int count;
    void *block;
    int retval;

    BFDEV_DEFINE_HASHMAP(test_map, NULL, &test_ops, NULL);
    nodes = block = malloc(sizeof(*nodes) * TEST_LOOP);
    if (!block) {
        bfdev_log_err("Insufficient memory!\n");
        return 1;
    }

    srand(time(NULL));
    bfdev_log_info("Generate %u node:\n", TEST_LOOP);
    for (count = 0; count < TEST_LOOP; ++count)
        nodes[count].value = ((uint64_t)rand() << 32) | rand();

    bfdev_log_info("Insert nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_LOOP; ++count) {
            value = ((uint64_t)rand() << 32) | rand();
            nodes[count].value = value;

            retval = bfdev_hashmap_add(&test_map, &nodes[count].node);
            if (retval)
                return retval;
        }
        0;
    );

    bfdev_log_info("Find nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_LOOP; ++count) {
            value = nodes[count].value;
            hnode = bfdev_hashmap_find(&test_map, (void *)value);
            if (!hnode)
                return 1;
        }
        0;
    );

    bfdev_log_info("Delete nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_LOOP; ++count) {
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
