/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/hlist.h>

#define TEST_LEN 100
static HLIST_HEAD(demo_list);

struct list_sample {
    struct hlist_node list;
    unsigned int num;
    unsigned long data;
};

#define list_to_sample(node) \
    container_of(node, struct list_sample, hlist)

int main(void)
{
    struct list_sample *node, *tmp;
    unsigned int count;
    int ret = 0;

    printf("Generate %d Node:\n", TEST_LEN);
    for (count = 0; count < TEST_LEN; ++count) {
        node = malloc(sizeof(*node));
        if ((ret = !node)) {
            printf("insufficient memory\n");
            goto error;
        }

        node->num = count;
        node->data = ((unsigned long)rand() << 32) | rand();
        hlist_head_add(&demo_list, &node->list);
    }

    hlist_for_each_entry(node, &demo_list, list)
        printf("\t%04u: 0x%016lx\n", node->num, node->data);

    printf("Deletion All Node...\n");
error:
    hlist_for_each_entry_safe(node, tmp, &demo_list, list) {
        hlist_del(&node->list);
        free(node);
    }

    return ret;
}
