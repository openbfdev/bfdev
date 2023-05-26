/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/slist.h>

#define TEST_LEN 100
static SLIST_HEAD(demo_list);

struct list_sample {
    struct slist_head list;
    unsigned int num;
    unsigned long data;
};

#define list_to_sample(node) \
    container_of(node, struct list_sample, slist)

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
        slist_add(&demo_list, &node->list);
    }

    slist_for_each_entry(node, &demo_list, list)
        printf("\t%04u: 0x%016lx\n", node->num, node->data);

    printf("Deletion All Node...\n");
error:
    slist_for_each_entry_safe(node, tmp, &demo_list, list) {
        slist_del(&demo_list, &node->list);
        free(node);
    }

    return ret;
}
