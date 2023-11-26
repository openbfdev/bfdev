/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <bfdev/hashmap.h>

#define TEST_LOOP 100

struct test_node {
    struct bfdev_hlist_node node;
    unsigned long value;
};

#define node_to_test(ptr) \
    bfdev_container_of(ptr, struct test_node, node)

static inline unsigned long
hashmap_hash_key(const void *key, void *pdata)
{
    return (unsigned long)key;
}

static inline unsigned long
hashmap_hash_node(const struct bfdev_hlist_node *node, void *pdata)
{
    const struct test_node *tnode = node_to_test(node);
    return tnode->value;
}

static inline long
hashmap_equal(const struct bfdev_hlist_node *node1,
              const struct bfdev_hlist_node *nodeb, void *pdata)
{
    const struct test_node *tnode1 = node_to_test(node1);
    const struct test_node *tnodeb = node_to_test(nodeb);
    return tnode1->value - tnodeb->value;
}

static inline long
hashmap_find(const struct bfdev_hlist_node *node, const void *key, void *pdata)
{
    const struct test_node *tnode = node_to_test(node);
    return tnode->value - (unsigned long)key;
}

static struct bfdev_hashmap_ops
test_ops = {
    .hash_key = hashmap_hash_key,
    .hash_node = hashmap_hash_node,
    .equal = hashmap_equal,
    .find = hashmap_find,
};

int main(int argc, const char *argv[])
{
    struct test_node *nodes, *find;
    struct bfdev_hlist_node *hnode;
    unsigned long value;
    unsigned int count;
    int retval;

    BFDEV_DEFINE_HASHMAP(test_map, NULL, &test_ops, NULL);
    nodes = malloc(sizeof(*nodes) * TEST_LOOP);
    if (!nodes)
        return 1;

    printf("hashmap 'bfdev_hashmap_add':\n");
    srand(time(NULL));
    for (count = 0; count < TEST_LOOP; ++count) {
        value = ((uint64_t)rand() << 32) | rand();
        nodes[count].value = value;

        printf("\ttest %02u: value %lu\n", count, value);
        retval = bfdev_hashmap_add(&test_map, &nodes[count].node);
        if (retval)
            return retval;
    }

    printf("hashmap 'bfdev_hashmap_find':\n");
    for (count = 0; count < TEST_LOOP; ++count) {
        value = nodes[count].value;
        hnode = bfdev_hashmap_find(&test_map, (void *)value);
        if (!hnode)
            return 1;

        find = node_to_test(hnode);
        printf("\ttest %02u: value %lu\n", count, find->value);
    }

    printf("hashmap 'bfdev_hashmap_del':\n");
    for (count = 0; count < TEST_LOOP; ++count) {
        value = nodes[count].value;
        retval = bfdev_hashmap_del(&test_map, (void *)value, &hnode);
        if (retval)
            return retval;

        find = node_to_test(hnode);
        printf("\ttest %02u: value %lu\n", count, find->value);
    }

    bfdev_hashmap_release(&test_map);
    free(nodes);

    return 0;
}
