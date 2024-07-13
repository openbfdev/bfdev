/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Fredrik Anderson <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <bfdev/hlist.h>

#define TEST_LEN 100
static BFDEV_HLIST_HEAD(demo_list);

struct hlist_simple {
    bfdev_hlist_node_t list;
    unsigned int num;
    unsigned long data;
};

#define hlist_to_simple(node) \
    bfdev_hlist_entry(node, struct hlist_simple, list)

int
main(int argc, const char *argv[])
{
    struct hlist_simple *node, *tmp;
    unsigned int count;
    int retval = 0;

    printf("Generate %d Node:\n", TEST_LEN);
    srand(time(NULL));
    for (count = 0; count < TEST_LEN; ++count) {
        node = malloc(sizeof(*node));
        if ((retval = !node)) {
            printf("insufficient memory\n");
            return 1;
        }

        node->num = count;
        node->data = ((uint64_t)rand() << 32) | rand();
        bfdev_hlist_head_add(&demo_list, &node->list);
    }

    bfdev_hlist_for_each_entry(node, &demo_list, list)
        printf("\t%04u: 0x%016lx\n", node->num, node->data);

    printf("Deletion All Node...\n");
    bfdev_hlist_for_each_entry_safe(node, tmp, &demo_list, list) {
        bfdev_hlist_del(&node->list);
        free(node);
    }

    return retval;
}
