/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "list-benchmark"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/log.h>
#include <bfdev/list.h>
#include "../time.h"

#define LIST_DEBUG 0
#define TEST_LEN 1000000
static BFDEV_LIST_HEAD(demo_list);

struct benchmark {
    struct bfdev_list_head list;
    unsigned int num;
    unsigned long data;
};

#define list_to_demo(node) \
    bfdev_list_entry(node, struct benchmark, list)

#if LIST_DEBUG
static void node_dump(struct benchmark *node)
{
    bfdev_log_info("\t%08d: data 0x%016lx\n", node->num, node->data);
}
#else
# define node_dump(node) ((void)(node))
#endif

static long
demo_cmp(const struct bfdev_list_head *node1,
         const struct bfdev_list_head *node2, void *pdata)
{
    struct benchmark *test1, *test2;

    test1 = list_to_demo(node1);
    test2 = list_to_demo(node2);

    if (test1->num == test2->num)
        return 0;

    return test1->num < test2->num ? -1 : 1;
}

int main(void)
{
    struct benchmark *node, *tmp;
    unsigned int count;
    void *block;

    node = block = malloc(sizeof(*node) * TEST_LEN);
    if (!block) {
        bfdev_log_err("insufficient memory!\n");
        return 1;
    }

    srand(time(NULL));
    bfdev_log_info("Generate %u node:\n", TEST_LEN);
    for (count = 0; count < TEST_LEN; ++count) {
        node[count].num = count;
        node[count].data = ((uint64_t)rand() << 32) | rand();
    }

    bfdev_log_info("Insert nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_LEN; ++count) {
            bfdev_list_add(&demo_list, &node[count].list);
            node_dump(node);
        }
        0;
    );

    bfdev_log_info("Sort nodes:\n");
    EXAMPLE_TIME_STATISTICAL(
        bfdev_list_sort(&demo_list, demo_cmp, NULL);
        0;
    );

    bfdev_log_info("List for each:\n");
    EXAMPLE_TIME_STATISTICAL(
        bfdev_list_for_each_entry(node, &demo_list, list)
            node_dump(node);
        0;
    );

    bfdev_log_info("Deletion all node...\n");
    bfdev_list_for_each_entry_safe(node, tmp, &demo_list, list)
        bfdev_list_del(&node->list);

    bfdev_log_info("Done.\n");
    free(block);

    return 0;
}
