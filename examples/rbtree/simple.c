/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021-2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <bfdev/rbtree.h>

#define TEST_LEN 10
static BFDEV_RB_ROOT(simple_root);

struct simple_node {
    struct bfdev_rb_node node;
    unsigned long data;
};

#define rb_to_simple(ptr) \
    bfdev_rb_entry_safe(ptr, struct simple_node, node)

static long
demo_cmp(const struct bfdev_rb_node *a, const struct bfdev_rb_node *b, void *pdata)
{
    struct simple_node *demo_a = rb_to_simple(a);
    struct simple_node *demo_b = rb_to_simple(b);
    return demo_a->data - demo_b->data;
}

int main(int argc, const char *argv[])
{
    struct simple_node *node, *tmp;
    unsigned int count;

    srand(time(NULL));
    for (count = 0; count < TEST_LEN; ++count) {
        node = malloc(sizeof(*node));
        if (!node) {
            printf("Abort.\n");
            return 1;
        }

        node->data = ((uint64_t)rand() << 32) | rand();
        bfdev_rb_insert(&simple_root, &node->node, demo_cmp, NULL);
    }

    printf("Middle Iteration:\n");
    bfdev_rb_for_each_entry(node, &simple_root, node)
        printf("\t0x%16lx\n", node->data);

    printf("Postorder Iteration:\n");
    bfdev_rb_post_for_each_entry(node, &simple_root, node)
        printf("\t0x%16lx\n", node->data);

    bfdev_rb_post_for_each_entry_safe(node, tmp, &simple_root, node)
        free(node);

    printf("Done.\n");
    return 0;
}
