/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bfdev/hashtbl.h>

#define TEST_SIZE 16
#define TEST_LOOP 100

BFDEV_DEFINE_HASHTBL(test_table, TEST_SIZE);

struct test_node {
    struct bfdev_hlist_node node;
    unsigned int value;
};

int main(int argc, const char *argv[])
{
    struct test_node *nodes, *walk;
    unsigned int count, value;

    nodes = malloc(sizeof(*nodes) * TEST_LOOP);
    if (!nodes)
        return 1;

    printf("hashtbl 'bfdev_hashtbl_add':\n");
    srand(time(NULL));
    for (count = 0; count < TEST_LOOP; ++count) {
        value = (unsigned int)rand();
        nodes[count].value = value;
        printf("\ttest %02u: value %u\n", count, value);
        bfdev_hashtbl_add(test_table, TEST_SIZE, &nodes[count].node, value);
    }

    printf("hashtbl 'bfdev_hashtbl_for_each_entry':\n");
    bfdev_hashtbl_for_each_entry(walk, test_table, TEST_SIZE, node, count) {
        value = walk->value;
        printf("\tindex %02u: value %u\n", count, value);
    }

    free(nodes);
    return 0;
}
